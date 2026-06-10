# Assembly patterns (MSVC 8.0 / VS2008, x86, /Od + LTCG)

How recovered-assembly shapes map back to Vostok C++ source. Every agent in the
matching loop reads this first and **appends** to it whenever it recognizes a new
asm -> source mapping. This is the shared memory that makes each run faster than
the last. Keep entries short - one pattern each.

Baseline for these modules (`/Od`, no optimization, plus LTCG):
- every statement is emitted, in source order; no strength reduction or
  reordering.
- every local and parameter has a stack slot, addressed `ebp`-relative; standard
  `push ebp / mov ebp, esp` frame.
- LTCG / the linker can change how values cross a `call` boundary (which registers /
  how much stack, or drop an argument proven constant). **That call-boundary argument
  passing is the ONLY thing you may write off as LTCG** (see `MATCHING.md`). Frame size,
  `[ebp-XX]` slot assignment, switch-dispatch shape, an extra `cmp/ja`, a stray `fld1`
  are NOT LTCG; they are source-steerable and each has a cause (a missing ASSERT, a
  missing `case`, a `default: NODEFAULT()`). The body is never reordered. Trust the
  operand-aware match % over a raw instruction-difference count, but do not let it lull
  you into banking a non-argument diff as "LTCG".

## Entry format

```
### <short name>
ASM:
    <a few representative instructions>
SOURCE:
    <the C++ that produces it>
NOTES: <when it applies, the overload/intrinsic involved, gotchas>
```

## Patterns

> **Lower-signal notes live in [`assembly_patterns_low_confidence.md`](assembly_patterns_low_confidence.md)**: low-confidence/negative results, unsteerable
> LTCG & /Od frame-slot residuals, and tooling/build/compile caveats - kept OUT of this
> trusted pattern list. Append a REAL asm->source mapping here; a caveat/residual there.

### float literals 0.0f / 1.0f on the FPU
ASM:
    fldz            ; 0.0f
    fld1            ; 1.0f
SOURCE: a bare `0.0f` / `1.0f` literal used in an FPU expression.
NOTES: if your base instead does `movss xmm, [__real@...]` (constant pool load),
the source used a named constant / variable, not the bare literal. Observed in
`game_core/sources/bullet.cpp` (`squared_length( ) < 1.f` emitted `fld1`).

### member sub-object method call via `add ecx, <offset>`
ASM:
    mov   ecx, [ebp-0Ch]      ; this
    add   ecx, 14h            ; &this->member  (member at +0x14)
    call  survarium::player_state::serialize
SOURCE: `member.serialize( arg )` where `member` lives at offset 0x14 in `this`.
NOTES: a member at offset 0 has no `add` (`this` == `&member`). The pushed args
come first, then `ecx` is loaded with the sub-object address. Confirmed in
`game_core/client_player_update::serialize` (input @+0x00 no add, state @+0x14).

### `packet.append( field )` -> packet<T>::append(u32) by value
ASM:
    mov   edx, [ebp-0Ch]      ; this
    mov   eax, [edx+58h]      ; this->field (u32)
    push  eax                 ; value pushed by value (not address)
    mov   ecx, [ebp+8]        ; &packet  (the `this` of append)
    call  vostok::network_core::packet<...>::append
SOURCE: `packet.append( field )` resolving to the `append( u32 )` overload.
NOTES: the scalar overloads push the value itself; the `append(pcvoid,u32)` and
`append(float3 const&)` overloads would instead push an *address* (lea/push of a
stack slot). Use that to disambiguate which `append` overload was called.
Confirmed in `game_core/client_player_update::serialize`.

### base-class ctor: out-of-line `call` vs inlined trivial init
ASM (target, derived ctor):
    mov   esi, [ebp-4]
    call  survarium::interactive_object::interactive_object   ; one call, no args
ASM (base, derived ctor) for the *same* source:
    push  1                                                   ; arg to base-of-base ctor
    mov   ecx, [ebp-4]
    call  vostok::resources::unmanaged_resource::unmanaged_resource
    mov   dword ptr [eax], 0                                  ; base-of-base vtable
SOURCE: a derived ctor whose base class is default-constructed.
NOTES: whether the base ctor is a real `call` or gets inlined is decided by the
**base class's declaration**, not the derived ctor's body. If the base class has a
user-declared out-of-line ctor, the derived ctor emits one `call BaseClass::BaseClass`;
if the base has no declared ctor (compiler-generated trivial init), MSVC inlines the
grand-base ctor + vtable store directly into the derived ctor. You cannot steer this
from the derived ctor's source - it requires adding/removing an out-of-line ctor on the
base (a separate function + wide blast radius). Confirmed in
`game_core/inventory_item::inventory_item` (base `interactive_object` has out-of-line
ctor at target rva 0x9ccb0; base build inlines `unmanaged_resource(1)` instead).
COROLLARY (it can ALSO come out a matched `call`): when the base class has NO user-declared
ctor but DOES declare a virtual (so an implicit ctor exists that stores the vtable), MSVC may
emit the implicit base ctor as a folded out-of-line symbol that BOTH binaries `call` - in which
case the derived ctor's `call BaseClass::BaseClass` matches with no source change. Confirmed in
`game_core/player_logic_base_state::player_logic_base_state` (100%): base `ai::fsm_state` has
only a pure-virtual dtor (no declared ctor); its implicit ctor is a folded out-of-line symbol
(absent from both rich indexes as a named fn, but `call vostok::ai::fsm_state::fsm_state` appears
identically in target AND base). So a base-ctor `call` is NOT automatically a divergence - check
the base index before banking BLOCKED.

### anchoring an abstract fsm_state subclass: override EVERY inherited pure virtual
A `concrete_state : survarium::<state>` anchor stub (the temp_include_all pattern for abstract
state classes) must override ALL still-pure virtuals or it stays abstract -> C2259. The whole
hierarchy derives from `ai::fsm_state` whose `initialize`/`execute`/`finalize`/
`is_ready_for_transition` are pure. A mid-hierarchy class often re-declares only SOME of them:
`player_logic_base_state` overrides only `is_ready_for_transition` (+ adds its own pure
`selected_animations`), leaving `initialize`/`execute`/`finalize` PURE. The stub needs `virtual
void initialize/execute/finalize() override {}` AND the class's own pure(s). Check the .h chain
for which pures the class did NOT re-declare. References for the ctor/setter args can be
fabricated from `*reinterpret_cast<T*>(NULL)` (the anchor never runs) to avoid constructing a
noncopyable owner type. Confirmed in `use_game_core_player_logic_base_state`.

### scalar default ctor: sequential `movss [this+off], const` from the constant pool
ASM (target, /Od default ctor of a float-only POD struct):
    mov   eax, [ebp-4]          ; this
    movss xmm0, [rdata_slot]    ; each /Od float literal gets its OWN rdata slot
    movss [eax], xmm0           ; member at offset 0
    mov   ecx, [ebp-4]
    movss xmm0, [rdata_slot]
    movss [ecx+4], xmm0         ; member at offset 4
    ...
SOURCE: a member-initializer list of scalar float constants. Maps 1:1, in
**declaration order** (MSVC ignores init-list order). A member with NO store
(gap in the offset sequence, e.g. [this+1Ch] then [this+24h] skipping 0x20) is
simply **absent from the init list** - leave it out.
NOTES: read the actual constants from the obj's `.rdata` + `.text` relocations
(objdiff/rich masks them as `[0]`/`[offset]`). A named symbol on the reloc (the
delinker resolves it, e.g. `epsilon_3`) means the literal is a named engine
constant: `epsilon_3 = .001f` is `vostok::math::epsilon_3` from `math_constants.h`.
COFF-parse recipe (binaries/objdiff/target/<unit>.obj): walk section headers, dump
`.rdata` as f32, then walk `.text` relocations (type 20 = DIR32) - each
`movss xmm0,[offset]` site's reloc gives `symsec/symval` into `.rdata`.
Confirmed in `game_core/weapon_recoil_params::weapon_recoil_params()` (default ctor,
target rva 0x5830c0).

### LTCG out-of-line-call vs inline of a trivial COMDAT template method
SYMPTOM: for `container.size()` / `container[i]` on a `vostok::vectora<T>` (thin
`std::vector` wrapper), TARGET emits a real `call` to e.g.
`vostok::vectora<T>::size` / `stlp_std::vector<T,...>::operator[]`, while BASE
(same `/Od` source) **inlines** them - `size()` becomes `[+4]-[+0]; cdq; idiv 0x38`
and `operator[]` becomes `imul edx,0x38; add edx,base`.
TELL it is LTCG, not a source bug: query both rich indexes -
`pdb_rich_query --index .../target/index.jsonl --function "vectora<T>::size" --list`
shows the wrapper exists out-of-line in TARGET, but the same query on
`.../base/index.jsonl` returns nothing (the COMDAT was inlined everywhere in base).
Under `/Od`+`/GL` whether a trivial `inline` COMDAT template method is emitted-and-
called or inlined is a whole-program/linker decision you cannot force from the
calling function's source. Leave it; match the member access + control flow and
mark PARTIAL. Confirmed in `game_core/scheduler::on_frame(u32,u32)` (target rva
0x77de80, 46.39% PARTIAL): the only residual diff is this size()/operator[]
inline-vs-call. Corollary: a "wrong member but shorter inlined form" can score
*higher* fuzzy % than the correct member - don't chase the metric, match the
offset the target reads (here m_active_objects @0x10, not m_inactive @0x00).

### intrusive_ptr-by-value in a `&&` chain: temp construct + safe-bool test + dtor
ASM (target, `if ( ptr_holder->smart_ptr_getter() && ... )` where the getter returns
a `resource_ptr`/`intrusive_ptr` BY VALUE):
    mov dword ptr [ebp-18h], 0          ; EH guard word for the temp
    ...
    or  dword ptr [ebp-18h], 1          ; mark "temp needs destruction"
    lea ecx, [ebp-0Ch]                  ; &temp
    push <&source_ptr_member>
    call intrusive_ptr<...>::set         ; construct temp = copy of the member ptr
    xor ecx, ecx
    cmp dword ptr [ebp-0Ch], 0           ; temp.m_object == 0 ?
    sete cl                              ; -> bool
    ...
    and dword ptr [ebp-18h], 0FFFFFFFEh  ; clear guard
    lea ecx, [ebp-0Ch]
    call intrusive_ptr<...>::dec          ; destroy temp
SOURCE: `getter()` returns a smart-ptr by value and is used in boolean context
(`a && getter() && b`); the temporary is materialized, tested via
`operator unspecified_bool_type` / `operator!`, and destroyed before the next term.
NOTES: the temp's whole construct/test/destroy is one term of the `&&`. Base (LTCG)
may lower the safe-bool conversion through an EXTRA intermediate bool stack slot
(`mov [slot],0; jmp; mov [slot],0; cmp [slot],0`) instead of the target's direct
`cmp/sete` - a materialization difference you cannot steer from source. Match the
control flow and the by-value getter call; leave the bool-slot shape. Confirmed in
`game_core/dispersion_calculator::get_dispersion` (target rva 0x586970, 87.49% PARTIAL),
where `m_weapon->ammunition()` returns `weapon_ammunition_ptr` by value.

### inline-vs-call of a trivial bool getter (is_aimed) - same LTCG class
SYMPTOM: `obj->is_aimed()` (an `inline bool is_aimed() const { return m_aimed; }`)
emits a real `call survarium::weapon_core::is_aimed` in TARGET but is INLINED in BASE
as `mov al, [obj+0x488]` (direct read of m_aimed). Identical class to the
`vectora::size()`/`operator[]` inline-vs-call entry above: whether a trivial COMDAT
inline member is emitted-and-called or inlined is a whole-program/linker decision,
not steerable from the caller. Match the member access; mark PARTIAL. Confirmed in
`game_core/dispersion_calculator::get_dispersion`.

### switch over a small enum (jump table) -> read case order from the obj's own .text relocs
SYMPTOM: `jmp dword ptr [edx*4 + <table>]` with the table inside the function's
`.text` (the trailing "garbage" after `ret` in `--view target`, e.g.
`add eax, 6B00595Fh`). The rich/objdiff view masks the per-case targets.
RECOVER: the N table entries are N dwords of `.text` with type-20 (REL32) relocs
back to the function symbol; the stored dword is the case target as a
*function-relative* offset. Parse the COFF: find the function's `.text` offset
from the symbol table, read the table dwords, subtract nothing (the raw value IS
the fn-relative target). Cross-reference each target block's leaf `movss
xmm0,[reg+off]` against the struct layout to label the case. Confirmed in
`game_core/character_dispersion_calculator::get_target_koef`: cases
type_stand(0)->0x25, type_crouch(1)->0x8b, type_sprint(2)->0xef, type_jump(3)->0xf9,
default(4)->0x103(=fld1, 1.0f). A `case X: return cond ? a : b;` lowers each leaf
to `movss xmm0,[m_params+off]; movss [tmp],xmm0; ... ; fld [tmp]; jmp end` and
matches byte-for-byte modulo register/slot.

### `mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub` = a compiled-out ASSERT (RECOVER it)
SYMPTOM: a zeroed byte local whose address is passed to a `call` the delinker
resolves to `empty_stub` (or misnames `finalize_impl`), ~`0x0c` bytes, which bumps
the frame and shifts the `[ebp-N]` slots. This is **NOT** an unsteerable stripped/
LTCG call - it is a **compiled-out `ASSERT`**: Master Gold drops the condition but
keeps the call. FIX: place `ASSERT( UNKNOWN_EXPRESSION )` (guess the condition via
`_T`) at that statement; it reproduces the `empty_stub` bytes and the frame/slot
shift resolves with it. Established house style - see `booby_trap_core.cpp`,
`inventory_cook.cpp` (their carcasses map `ASSERT( UNKNOWN_EXPRESSION );` to a `+0x0c`
slot). [CORRECTION: `get_target_koef` / `get_broken_hands_penalty` and the
weapon_core_*_state finalize/on_animation "residuals" were wrongly left unmatched -
they must recover the ASSERT.]

### `and X,mask; neg; sbb X,X; neg` = `bool b = (val & mask) != 0` (/Od bool-normalize)
ASM:
    mov ecx, [eax+10h]   ; val
    and ecx, 1           ; val & mask
    neg ecx
    sbb ecx, ecx
    neg ecx              ; -> 0 or 1
    mov [ebp-2], cl      ; store the bool
SOURCE: `bool b = ( val & MASK ) != 0;` (the explicit `!= 0` form). MSVC /Od emits the
`neg/sbb/neg` to normalize any-nonzero to exactly 1. The mask literal is read straight
from the `and` operand. Confirmed in `game_core/player_logic_base_state::movement_animation_index`
(actions_mask @ player_input+0x10, masks 1/2/4/8).

### nested if/else-if/else emits an extra join `jmp` per level -> flatten to early returns
SYMPTOM (text-diff fallback, body otherwise byte-identical): base emits a DOUBLE jump at
each return leaf (`jmp .13; jmp .5`) jumping to an intermediate join then to the epilogue,
while target does a SINGLE `jmp .end` per leaf to one shared epilogue. objdiff scored `None`
(too divergent to pair) until the jumps matched.
SOURCE: an `if (a) { ... } else if (b) { ... } else { ... }` chain whose branches all
`return` creates a join point (the `else` block's end) before the function end, so MSVC /Od
emits a jmp-to-join then jmp-to-end. The target was written as FLAT early returns:
`if (a) { ...; return; } if (b) { ...; return; } return;` (no `else`), so each leaf jumps
once directly to the single epilogue. Confirmed 100% in
`game_core/player_logic_base_state::movement_animation_index`. Rule: when the diff is only
redundant intermediate `jmp`s, drop the `else` keywords and use early returns.

### static member-function access codes: private=`C`, protected=`K`, public=`S` (after `@@`)
SYMPTOM: report.json `fuzzy_match_percent: None` for a static member that compiled and is in
the base obj; the COFF symbol differs from the target only in the storage-class char right
after `@@` (e.g. base `?fn@@CAI...` vs target `?fn@@KAI...`). Same pairing-failure class as the
instance-member `ABE`/`QBE` entry below, but for STATICS: `C`=private static, `K`=protected
static, `S`=public static (then `A`=__cdecl-ish, return-type, args). FIX: set the declaration's
access specifier to match the target char. Confirmed: `movement_animation_index` is `K`
(protected static) - public gave `S`, private gave `C`, both scored None; `protected:` -> `KAI`
== target -> 100%.

### static DATA-member access codes: private=`0`, protected=`1`, public=`2` (after `@@`)
SYMPTOM: a `mov ecx, ?m_x@Class@ns@@0V...` (or `mov [...], ...`) where TARGET reads
`@@2V...` but BASE reads `@@0V...` for the SAME static data member - objdiff pairs them
(same name) but the relocation/symbol differs, costing match %. Distinct from the
member-FUNCTION codes (Q/A/I) and the static-FUNCTION codes (C/K/S): for static DATA
members the char right after `@@` is the access - `0`=private, `1`=protected, `2`=public
(then the type, e.g. `PAV`=pointer, `V`=class by value, `IA`=u32). FIX: set the
declaration's access specifier so the char matches. A class can have two statics with
DIFFERENT access (interleave `private:`/`public:` to keep declaration order). Confirmed in
`game_core/damage_model_cook::on_hit_params_received`: `m_hit_types` is `@@0V` (private,
both sides) but `m_hit_types_strings` was target `@@2V` (PUBLIC) while our header had it
private (`@@0V`); a `public:` before it fixed the symbol.
### VIRTUAL member access codes: public=`U`, protected=`M`, private=`E` (after `@@`)
SYMPTOM: report.json `fuzzy_match_percent: None` for a virtual override that compiled and is in
the base obj; the COFF symbol differs from the target only in the access/virtual char right after
`@@`. For VIRTUALS the char encodes access AND virtual-ness: `U`=public virtual, `M`=protected
virtual, `E`=private virtual (then `AE`=__thiscall). Same pairing-failure class as the non-virtual
`Q`/`A`/`I` and the static `S`/`K`/`C` entries. FIX: set the override's access specifier to match.
A virtual override may legally be `private:` (virtual dispatch ignores access); the target often
makes the overrides private even when the base declares them public. Confirmed:
`weapon_core_idle_state_base::{initialize,finalize}` are `EAEXXZ` (private virtual) - declaring them
`private:` flipped both from None to 100%. The non-virtual ctor in the same class was `IAE`
(protected); `protected:` -> 100%. (To anchor private virtuals non-virtually from a free `use_*`,
befriend the anchor in the class - free decls/friends emit no bytes.)

### address-of a VIRTUAL member yields a vtable thunk, NOT the body (anchoring trap)
`&Class::virtual_member` does NOT ODR-use the function body - a pointer-to-member of a
virtual is a vtable index/thunk, so MSVC emits NOTHING for the body. An anchor built on
`void (Class::*p)() = &Class::virtual_member; example_callback(&p);` leaves ZERO `Class::member`
symbols in the obj (only the `use_*` anchor). To force the actual body without emitting the
vtable (instantiating would emit the vtable -> codegen of any still-STUB sibling virtual ->
C4716/LNK1257), use a QUALIFIED (devirtualized, non-virtual) call on a fabricated null pointer:
`Class& s = *reinterpret_cast<Class*>(NULL); s.Class::member();`. That ODR-uses the exact body
(emits the COMDAT) and never touches the vtable. Caveat: under /GL LTCG the linker then INLINES
the trivial one-liner into the reachable anchor, so no standalone body survives to the EXE (the
target keeps it standalone only because it is virtual in a LIVE vtable and can't be inlined there);
the source is still byte-correct - confirm by disassembling the anchor and reading the inlined
bytes. Confirmed on `jump_logic_state_{landing,start}::{execute,is_ready_for_transition}`.

### read the delinked TARGET `.h`-unit's OWN recovered symbol for access, not the ICF fold rep
A trivial virtual that ICF-folds shows the FOLD REPRESENTATIVE's mangling in the rich index
(e.g. landing::is_ready_for_transition folds to `?is_datatype@particle_action@...@@UBE_NXZ`,
public). But the delinker's per-unit target `.h`.obj keeps the function's OWN recovered symbol
(`report.json` lists it): `?is_ready_for_transition@jump_logic_state_landing@survarium@@EBE_NXZ`
(private). Trust the unit's own symbol for the access char, not the fold rep - they can disagree.

### derived state ctor: base-ctor delegation + N compiler-emitted vtable stores, empty body
ASM (target, `weapon_core_idle_state_base` ctor, multiple-inheritance state class):
    push 0; mov eax,[ebp+8]; push eax; mov ecx,[ebp-4]
    call survarium::weapon_core_base_state::weapon_core_base_state   ; (weapon, false)
    mov [ecx],    ??_7...@6Bfsm_state@ai@vostok@@           ; primary vtable @0x00
    mov [edx+18h],??_7...@6Bunmanaged_resource@...@         ; secondary vtable @0x18
SOURCE: ctor with ONLY an init-list base delegation (`: weapon_core_base_state( weapon, false )`)
and an EMPTY body. The two `mov [this+off], ??_7...` are the compiler-emitted vtable pointer
stores for each base subobject of a multiply-inherited class (fsm_state @0x00, unmanaged_resource
@0x18) - never write them, MSVC emits them after the base ctor returns. Matches 100% with an empty
`{}` body. Confirmed in `game_core/weapon_core_idle_state_base::weapon_core_idle_state_base`.

### private member function -> mangled `ABE`, not `QBE` (objdiff scores `None` if wrong)
SYMPTOM: report.json `fuzzy_match_percent = None` for a function that clearly
compiled and is in the base obj. CAUSE: the base/target mangled names differ only
in access (`?fn@...@@QBE...` public vs `?fn@...@@ABE...` private/`AAE` etc.), so
objdiff cannot pair them. FIX: read the exact mangled name from the target obj
symbol table and set the declaration's access specifier (public/private/protected)
to match the leading letter (Q=public, A=private, I=protected; the 2nd letter A=
non-const-this, B=const-this). Confirmed: `get_target_koef`/`get_broken_hands_penalty`
are `ABE` (private const) - moving them to `private:` flipped them from `None` to scored.

### switch with `case 0` whose body equals the default -> `case 0: return X;`, NOT `case 0: break;`
SYMPTOM: target has an explicit `cmp [v],0 / je <block>` and that block is the SAME
`fld1`/return reached by the no-default fall-through (one shared block, two predecessors).
WRONG: `case 0: break; ... return X;` - MSVC folds case 0 into the default path and emits
NO `cmp 0` at all (only `cmp 1 / cmp 2 ...`), so the base loses the whole comparison and
objdiff reports `fuzzy_match_percent: None` (too divergent to score - NOT a mangling/strip
issue; the symbol is present). RIGHT: `case 0: return X;` as a distinct labeled block plus a
tail `return X;` (the default); MSVC /Od emits `cmp 0/je` and FOLDS the two identical return
blocks into one `fld1` reached by both - matching the target. Confirmed on
`character_dispersion_calculator::get_broken_hands_penalty`: `break` -> None, `return 1.0f` -> 82.89%.

### `fixed_string<N>("literal")` - which ctor overload, and inline-vs-call is LTCG (not steerable)
SYMPTOM: a `vostok::fixed_string<N>("literal")` temporary. TARGET emits, inline at the call site:
    mov   dword ptr [ebp-XXh], <N>     ; capacity temp = N (e.g. 0x2E = 46), materialized to a SLOT
    push  ??_C@...@literal             ; src
    lea   ecx, [ebp-XXh]; push ecx     ; &capacity  (the ctor takes `size_type const&` -> by ADDRESS)
    lea   edx, [this+0Ch]; push edx    ; &m_buffer  (buffer_string base is 12 bytes; m_buffer @ this+0xC)
    lea   ecx, [this]                  ; the fixed_string temp
    call  vostok::buffer_string::buffer_string   ; the 3-arg (char*, u32 const&, char const*) overload
BASE (same source) instead emits: `push "literal"; lea ecx,[this]; call fixed_string<N>::fixed_string<N>`
i.e. a `call` to the OUT-OF-LINE `fixed_string<N>(char const*)` ctor.
WHY: `fixed_string<N>(value_type const* src) : buffer_string(m_buffer, Size, src) {}` - its whole body
IS that one 3-arg buffer_string call (the capacity-N-by-reference materialization included). The
source `fixed_string<N>("literal")` is therefore ALREADY correct; the only difference is whether the
compiler inlines this 5-instruction wrapper ctor at the call site or emits a `call` to it.
TELL it is LTCG, not source: query both rich indexes for `fixed_string<N>::fixed_string<N>` - TARGET
has NO out-of-line `(char const*)` symbol (inlined whole-program) while BASE keeps it (a real rva).
The default `fixed_string<N>()` ctor can simultaneously be CALLED out-of-line in BOTH within the same
target function - so the inline decision is per-ctor whole-program, NOT per-call-site-steerable under
/Od+/GL. Same class as the `vectora::size()`/`is_aimed()` inline-vs-call entries. Confirm by
disassembling the base out-of-line ctor: its body == the target's inlined sequence => leave the source
as `fixed_string<N>("literal")` and mark PARTIAL; the cascading +0x10 frame-size delta and reg/slot
renaming all follow from the single inline. Confirmed in
`game_core/body_part_parameters::fill_new_stats_item<statistics_item<46,16>>` (target rva 0x0ba3c0,
91.78% PARTIAL): base out-of-line `fixed_string<46>(char const*)` @ base rva 0x030b00 is exactly
`mov [ebp-4],2Eh; push src; add eax,0Ch; lea ecx,[ebp-4]; call buffer_string::buffer_string`.

### pointer subtraction of typed pointers -> `sub; cdq; idiv sizeof`
ASM:
    mov eax,[ebp+0Ch]      ; p (e.g. &bone, a skeleton_bone const*)
    sub eax,[ebp+14h]      ; p - q  (byte difference)
    cdq
    mov ecx, 14h           ; sizeof(*p)  (skeleton_bone = 0x14)
    idiv ecx               ; (byte diff) / sizeof -> element count
SOURCE: `p - q` where both are `T const*` (the compiler does the byte-diff /
sizeof division itself). The result is `ptrdiff_t`; a surrounding `u32(...)` cast
just narrows it. So `u32 const matrix_index = u32( &bone - first_non_root_bone );`.
NOTES: `idiv` (signed) because pointer difference is signed. The divisor is the
element size - read it to confirm which type's array you are indexing. Confirmed in
`game_core/get_bone_matrix_in_object_space_impl`.

### by-value `float4x4 operator*(float4x4 const&, float4x4 const&)` arg order from push order
ASM:
    push eax               ; (1st push) = RIGHTMOST arg = `right`
    ... compute &left ...
    push ecx               ; (2nd push) = `left`
    lea  edx,[ebp-XX]      ; &hidden_return (struct return by value)
    push edx               ; (last push) = hidden return ptr (conceptual 1st param)
    call vostok::math::operator*
    add  esp, 0Ch
SOURCE: `left * right`. For a `__cdecl` free function returning a struct by value,
args are pushed right-to-left and the hidden return pointer is pushed LAST. So the
FIRST thing pushed is the rightmost source operand. Map: first-push -> right,
second-push -> left. Confirmed in `game_core/get_bone_matrix_in_object_space_impl`
(`matrices[matrix_index] * recursive_result`: first push = recursive_result =
right operand).

### `lea ecx,[slot]; call <misnamed allocator<char>>; mov ecx,eax; call float4x4::identity`
ASM:
    lea ecx,[ebp-0C8h]
    call stlp_std::allocator<char>::allocator<char>   ; delinker MISNAME for float4x4()
    mov ecx, eax
    call vostok::math::float4x4::identity
SOURCE: `float4x4( ).identity( )` - a default-constructed temporary on which
`identity()` is called (`identity()` returns `float4x4&`, eax = this). The first
`call` is the trivial/empty default `float4x4()` ctor; the delinker resolves its
symbol to an unrelated `allocator<char>` ctor (a COMDAT-folding misname, same class
as the `empty_stub`/`finalize_impl` misnames). Don't take the symbol literally -
the `lea &slot; call <ctor>; mov ecx,eax; call identity` shape IS `float4x4().identity()`.
Confirmed in `game_core/get_bone_matrix_in_object_space_impl` (the parent==NULL leaf).

### derived ctor: member of struct-with-ctor is `call <ctor>` THEN explicit member stores in same TU
ASM (target, weapon_core_base_state ctor at +0x120 member):
    add ecx,120h; call <float4()>          ; (delinker MISNAME of the member's default ctor)
    ...vtable stores...
    add edx,120h; mov [ebp-8],edx          ; &member -> slot
    mov eax,[ebp-8]; mov dword [eax],0     ; member.field0 = 0
    mov ecx,[ebp-8]; movss [ecx+4],const   ; member.field1 = const
SOURCE: a value member (here `animation_playback_state m_animation_playback_state`) whose own
default ctor has a member-init list (`: interval_id(0), interval_time(0.0f) {}`). MSVC /Od emits the
member's empty/base sub-object ctor as a `call` (often delinker-misnamed, e.g. `float4()`), then
INLINES that ctor's member-init stores into the enclosing ctor right after. So fill the member
struct's own ctor body to produce the explicit stores; the misnamed `call` is the sub-object init.
Confirmed in `game_core/weapon_core_base_state::weapon_core_base_state` (100%): the `float4()` reloc
was actually `animation_playback_state`'s default ctor; rdata slot for the movss = 0.0f.
NOTES: a `member.reset()` would look identical (`&member->slot; store; store`) - decide by whether the
carcass body has a statement line vs the stores being in init-list (decl-order before later members).

---

## Hand-written copy ctor delegating to operator= + member-wise scalar operator=
(`survarium::player_stealth::{player_stealth(const&), operator=}`, PR pending)

COPY CTOR shape (`push ebp;...;mov [ebp-4],ecx` then):
```
mov eax,[ebp+8]    ; other
push eax
mov ecx,[ebp-4]    ; this
call operator=
mov eax,[ebp-4]    ; ctor returns this in eax
ret 4
```
=> SOURCE: ctor body is exactly `*this = other;`. The trailing `mov eax,[ebp-4]` is
the standard MSVC ctor `return this`, not part of your source.

OPERATOR= shape: self-assignment guard then one fld/fstp pair per float member in
declaration/offset order:
```
mov eax,[ebp-4]; cmp eax,[ebp+8]; je .end   ; if ( this != &other )
  fld dword [src+0xNN]; fstp dword [dst+0xNN]   ; one per member, 0x00,0x04,...
.end:
mov eax,[ebp-4]   ; return *this
ret 4
```
=> SOURCE: `if ( this != &other ) { m_a = other.m_a; ... } return *this;` with members
in DECLARATION order (each `[reg+0xNN]` maps to the `/* 0xNN */` member). Identical
shape to the already-100% `player_stamina` pair. ANCHOR (game_core): default-construct,
copy-construct + a direct `b = a`, then escape `&a`/`&b` through the opaque
`example_callback` sink so LTCG does not DSE the member stores. Landed both at 100% on
the first rebuild.

### per-call-site inline-vs-call of `operator|` (float3 dot) - same LTCG class, both keep the standalone
SYMPTOM: `a | b` (float3 dot, `float vostok::math::operator|(float3_pod const&, float3_pod const&)`)
emits a real `call vostok::math::operator|` in TARGET but is INLINED at the call site in BASE
(`mov [tmp],eax; movss xmm,[a+8]; mulss [b+8]; ...; addss; addss`). Querying BOTH rich indexes
shows operator| present out-of-line in EACH (target rva 0x8160, base rva 0x371e0) - so it is NOT
"inlined everywhere in base"; the divergence is a PER-CALL-SITE whole-program LTCG inline decision,
not steerable from the caller's source. The inlined dot also adds a frame temp + bumps `sub esp,N`
(0x24 vs 0x20) and shifts the trailing `[ebp-N]` result slot, all CASCADING from the one inline.
Same class as `vectora::size()`/`is_aimed()`/`fixed_string` ctor inline-vs-call. Source `a | b` is
already correct; mark PARTIAL. Confirmed in `game_core/survarium::get_additional_length` (target rva
0x0bb1f0, 65.38% PARTIAL): the lone diff is the inlined `upleg_dir | -leg_dir` dot.

RE-CONFIRMED (don't re-litigate): a structure verifier re-challenged this as "caller-source-steerable,
not LTCG". It is NOT. Three forms rebuilt: `a|b` and explicit `operator|(a,b)` both INLINE -> 65.375%;
`dot_product(a,b)` -> 90.4% BUT binds the TEMPLATE `dot_product<float3>` (math_functions_inline.h:16,
preferred over the non-template free `dot_product(float3_pod const&,...)` because float3 is an exact T
match) which is emitted `__cdecl` (push/push, x87) - a DIFFERENT function. The TARGET binary has NO
`dot_product<float3>` symbol (only `operator|` 0x8160 + member `dot_product` 0x8130), so the original
wrote `a|b`; the 90.4% is a coincidentally-higher % over a FABRICATED function and is forbidden. Two
real residuals, both whole-program LTCG: (1) per-site inline-vs-call, AND (2) the target's `operator|`
is an LTCG calling-convention-promoted COMDAT (`__fastcall` ecx=left/eax=right, xmm0 return) while our
base operator| standalone is plain `__cdecl` x87 - our base build never applies that promotion. No
caller spelling reaches either. (Note: game_core `Master Gold` is /Od but /Ob2=InlineFunctionExpansion=2
is ON, which is why an `inline` helper inlines at all under nominal "/Od".)

### empty function body
ASM:
    push  ebp
    mov   ebp, esp
    pop   ebp
    ret
SOURCE: an empty body `void f( ) { }` - the bare `/Od` frame, 5 bytes, byte-identical
for every empty `__cdecl` function. It DOES get its own standalone symbol and appears
as its own unit in objdiff/report (confirmed: `game_core/sources/entry_point.cpp`
`survarium::game_core_initialize`, symbol `?game_core_initialize@survarium@@YAXXZ`).
`/OPT:ICF` may fold byte-identical functions, but the symbol still resolves - do not
assume an empty function is "unscorable".

### switch dispatch: bounds check (`cmp/ja`) vs contiguous jump table - the `default:` shape
ASM (TARGET - no bounds check, contiguous table):
    mov   edx, [ebp-0Ch]            ; the switch value
    jmp   dword ptr [edx*4+table]   ; straight to the jump table, values 0..N
ASM (BASE - extra bounds check, table bounded at the top explicit case):
    cmp   dword ptr [ebp-0Ch], 3    ; <- the highest explicit `case`
    ja    default
    mov   edx, [ebp-0Ch]
    jmp   dword ptr [edx*4+table]   ; only values 0..3
SOURCE: the difference is the `default:`.
- No bounds check  => source covers the FULL contiguous case range and ends
  `default: NODEFAULT();` (`NODEFAULT` = `__assume(0)` in Master Gold,
  `sources/vostok/debug_macros.h`). `__assume(0)` tells MSVC the default is
  unreachable, so it drops the range check and emits a tight `[0..N]` table.
- `cmp max; ja default` => source has a reachable `default: return X;` (or stops
  short of the top enum value), so MSVC bounds the table at the highest `case`.
NOTES: if BASE has a `cmp/ja` the TARGET lacks, you are missing the top `case`(s)
plus a `default: NODEFAULT();` - NOT an LTCG artifact. A terminal value the target
reaches THROUGH the table (e.g. a final `fld1` for the highest enum value) is an
explicit `case`, not the `default`. Found on
`game_core/sources/character_dispersion_calculator.cpp::get_target_koef`
(enum 0..4; ours stopped at `type_jump`(3) + `default: return 1.0f;`, target had
`case type_preview: return 1.0f; default: NODEFAULT();`).

### `jmp short` (2 bytes) inside a switch = a closing brace `}`
ASM:
    eb xx           ; jmp short <end-of-switch / next>  (2 bytes)
CARCASS: shows as a `+0x002` step (a body line whose `+delta` from the previous is 2).
SOURCE: the `break` / scope-exit of a braced block - a `{ }`-scoped `case` (or a
braced `if` inside one). /Od emits the `jmp` for the `}` even when the block's last
statement `return`s (dead, not removed).
NOTES: this is how the carcass reveals where braces were. A `case`/`if` showing a
`+0x002`/`jmp` was braced - brace it in source. A `case` with no such entry was
brace-less. Do NOT decide braces from "has a local" alone (we mis-braced
`get_target_koef` and earlier switches that way; prior switch matches should be
re-checked against this).

### two identical `jmp .end` back-to-back = a returning if-block's `}` jumping OVER an else
ASM (target tail of an `if/else` where the if-body returns):
```
mov eax,[ebp+8]      ; load return value (sret ptr)
jmp short .end       ; <- the `return` statement's own jmp
jmp short .end       ; <- the if-block `}` jmp, jumping OVER the else body
.else: ...           ; else body (entered via the cond `jne .else` at the top)
mov eax,[ebp+8]
.end: <epilogue>
```
CARCASS: a `+0x002:'<L_close>'` brace entry SANDWICHED between the if-body's last
statement and the else-body's statement (e.g. L412 return, then `+0x002:'414'` `}`, then
L416 else). The brace jmp targets the EPILOGUE and skips the else.
SOURCE: it is a real `if (...) { ...; return a; } else { return b; }` - an `else` block,
NOT `if (...) { return a; } return b;`. A plain if-then + trailing return falls THROUGH to
the trailing statement (no jmp over it), and MSVC /Od folds the two identical `return` jmps
into one (you lose the brace jmp). The tell that it is an else, not a fall-through: the
if-`}` jmp jumps PAST the next statement to the epilogue (it would have to skip the else
body). Confirmed on `math::get_relative_matrix` (90.2->97.5 via DEBUG_BREAK int3, then
97.5->100 by changing `if(!x){...return;} return mul;` to `if(!x){...return;}else{return
mul;}` - the else recovered the second `jmp .end` at 0x47). Also note: a 1-byte `int3`
inside such a block is `DEBUG_BREAK( )` = `__debugbreak` (debug_macros.h), NOT an empty
Master-Gold ASSERT (which emits zero bytes).

### derived ctor OVERWRITES an inherited member that the base ctor already set
ASM (target, `weapon_core_show_state_base` ctor tail, after the base-ctor call + vtable stores):
```
mov eax,[ebp+0Ch]; mov [edx+140h],eax          ; m_is_shown ref store (the derived member)
mov dword ptr [ecx+130h], 0FFFFFFFDh           ; m_body_part_mask_for_user = -3
```
SOURCE: the derived ctor BODY re-assigns an inherited protected member the base ctor
already initialized. Here `weapon_core_base_state`'s ctor sets `m_body_part_mask_for_user
= body_part_whole_body (-1)`; the show ctor's body OVERWRITES it to
`m_body_part_mask_for_user = animation::body_part_whole_body_but_hands; // -3`. The store
lands at the inherited offset (0x130) AFTER the init-list member stores. The member is
`protected` on the base, so it is reachable from the derived body. Confirmed in
`game_core/weapon_core_show_state_base::weapon_core_show_state_base` (100%). NOTE the enum
values: body_part_whole_body=-1 (0xFFFFFFFF), body_part_whole_body_but_hands=-3 (0xFFFFFFFD).

### non-virtual member call through a member reference -> direct `call` (contrast virtual)
ASM (target, `m_weapon.instant_toggle_start()` where m_weapon is a weapon_core& @ 0x128):
```
mov ecx,[this]; mov ecx,[ecx+128h]      ; ecx = m_weapon (the reference)
call survarium::weapon_core::instant_toggle_start    ; DIRECT call, no vtable indirection
```
SOURCE: `m_weapon.method()` where `method` is a NON-virtual member of weapon_core. Compare
the virtual-call entry below (m_ref.virtual_method() -> `mov [vtbl+off]; call`). Same source
shape; the asm differs ONLY by the method's virtual-ness - read the .h `virtual` keyword to
decide. Confirmed: `weapon_core_show_state_base::{initialize,finalize}` call the non-virtual
`instant_toggle_start/end` (direct), vs `aimed_state_base` calling virtual instant_aim_start/end.

### virtual call on a member reference through its OWN vtable (m_ref.virtual_method())
ASM:
```
mov eax,[ebp-4]; mov ecx,[eax+128h]   ; load m_weapon (a weapon_core& @ 0x128)
mov edx,[ebp-4]; mov eax,[edx+128h]   ; (the slot is loaded twice under /Od)
mov edx,[ecx]                         ; vtable of *m_weapon
mov ecx,eax                           ; ecx = this for the callee
mov eax,[edx+8Ch]; call eax           ; indirect call, vtable slot 0x8c
```
=> SOURCE: `m_weapon.instant_aim_start();` where `m_weapon` is a `weapon_core&` and
`instant_aim_start` is declared `virtual` ON weapon_core. Calling a virtual through a
reference of the declaring class dispatches via the vtable (the `[vtbl+0xNN]; call`).
NOTES: contrast with a NON-virtual member of the same class, which compiles to a direct
`call survarium::weapon_core::method` (no `[vtbl]` indirection). Same source shape
(`m_weapon.foo()`), the asm differs ONLY by foo's virtual-ness. Caught on
`weapon_core_aimed_state_base::{initialize,finalize}` (virtual instant_aim_start/end @
slots 0x8c/0x90) vs `..._idle_state_base` (non-virtual instant_idle_start/end, direct
call). Read the `.h` `virtual` keyword to decide which the source needs.

### a const member function that ASSIGNS to a class member -> the member is `mutable`
SYMPTOM: a `... method() const` whose target asm takes the *address of a member* and
calls a mutating op on it (e.g. `mov ecx,[this+offset]; add ecx,0xNN; call
intrusive_ptr::operator=`, or `mov byte[this+0xNN], ...`). Writing the assignment
`m_x = arg;` in a const method fails to compile: `error C2678: binary '=' : no operator
found which takes a left-hand operand of type 'const T'`. FIX: declare that member
`mutable` in the header (a const "setter" caching/storing into a member). Do NOT drop
the `const` from the method - the target's mangled name carries `B` (const) and objdiff
pairs by mangled name, so removing const would mis-mangle and score None. Caught on
`weapon_core_animation_end_aware_state::set_animation_to_wait(...) const` (mangles `@@IBE`)
assigning `m_animation_to_wait_for` (@0x138) -> `mutable resources::managed_resource_ptr`.

### `if ( a && b )` materializing a THIRD bool temp = inlined predicate, not a bare `&&`
A `/Od` `if ( x && y )` where x,y are member-bool reads short-circuits DIRECTLY into the
`if` body: `movzx ecx,member; test ecx,ecx; je past_body; ...second test...; <body>`. No
intermediate result slot. But if the carcass/target instead computes the `&&` into a
SEPARATE bool temp first and then re-tests THAT temp:
```
mov al,[this+2Ch]; mov [ebp-1],al; movzx ecx,[ebp-1]; test; je .else   ; x -> temp
mov al,[this+2Dh]; mov [ebp-2],al; movzx ecx,[ebp-2]; test; je .else   ; y -> temp
mov [ebp-0Ch],1; jmp .end                                              ; && = 1
.else: mov [ebp-0Ch],0                                                 ; && = 0
.end: movzx edx,[ebp-0Ch]; test edx,edx; je .skip                      ; if ( && result )
```
the extra slot `[ebp-0Ch]` (and the bigger frame, e.g. `sub esp,0Ch` vs `sub esp,08`) is
the RETURN-VALUE temp of an inlined predicate whose body IS the `&&`. Source is
`if ( is_full_on_ground() )` with `inline bool is_full_on_ground() const { return
is_heel_on_ground() && is_toe_on_ground(); }` (and the leaf getters return their members).
Writing the bare `if ( is_heel_on_ground() && is_toe_on_ground() )` short-circuits without
the third temp and gives the wrong (smaller) frame. The per-member `mov al,[..]; mov
[ebp-N],al` round-trip is itself the inlined leaf getter's return temp. Caught on
`legs_ik_processor::leg_params::set_{heel,toe}_on_ground` (59.90 -> 78.19 with leaf getters,
-> 100 once the condition went through is_full_on_ground; frame 0x0C matched).

### `member = math::min( x, member )` xmm operand order is STEERABLE via source operand order (NOT LTCG)
A setter `m_x = vostok::math::min( a, b );` emits `movss xmm0,<op>; movss xmm1,<op>; call min;
movss [this+off],xmm0`. WHICH operand lands in xmm0 vs xmm1 is decided by the SOURCE operand
order, not by any link-time convention - so it is a source-steerable matching problem, NOT a
bankable "call-boundary arg passing" LTCG residual. If your base puts the operands in the
wrong xmm registers vs the target (3 instrs differ, ~84%), SWAP the two operands in source.
`min`/`max` are commutative, so the swap is semantics-preserving and is the correct fix.

Empirically, writing the MEMBER operand FIRST (`math::min( member, arg )`) makes the base load
`arg->xmm0, member->xmm1`. Reference (clean, 100%): `generic_anomaly_core::dec_energy` is
`m_energy_current -= math::min( m_energy_current, amount )` (member first) and its base emits
`arg->xmm0, member->xmm1`, matching the target. The `legs_ik_processor::leg_params::
set_{heel,toe}_transition_time` setters were wrongly written `min( arg, member )` (arg first)
and scored 83.69% with the two `movss` operands in swapped registers; the target wanted
`arg->xmm0, member->xmm1`, i.e. member-first source. Swapping to `math::min( member, arg )`
took them 83.69% -> 100%. Lesson: never bank an xmm-operand-order diff as LTCG - swap and
rebuild.

### rva 0x03f210 is the universal empty-function fold target - a `call` there is whatever empty fn the source needs
SYMPTOM: a ctor/function `call`s a symbol the delinker names
`fixed_size_allocator<task_type,mutex_tasks_unaware>::finalize_impl` (or `empty_stub`,
`float4()`, etc.). Querying `pdb_rich_query --function <name>` for SEVERAL unrelated empty
functions all return the SAME rva **0x03f210**: confirmed there are at least
`vostok::core::noncopyable::noncopyable()`, `...::finalize_impl()`, and
`vostok::ai::fsm_state::~fsm_state()`. MSVC `/OPT:ICF` folds every byte-identical empty
`{ }` body to one address; the delinker prints whichever name it picked, NOT the real callee.
RULE: do NOT take a `call ...finalize_impl` (or any 0x03f210 call) literally. Identify the
empty function the SOURCE at that spot actually needs - usually the **out-of-line base ctor**
(e.g. deriving `core::noncopyable`, whose declared `noncopyable()` is out-of-line) or a base
dtor. The ASSERT-class `empty_stub` (its own entry above) is DISTINGUISHED by its byte prefix:
an ASSERT is `mov byte[ebp-N],0; lea eax,[ebp-N]; call` (consumes a byte slot, ~0x0c bytes); a
bare base-ctor fold is just `mov eax,[ebp-4]; call` (this, no byte local). Read the prefix to
tell ASSERT vs base-ctor-fold. Found on `game_core::ik_processor::ik_processor()`: pre-store
`call ...finalize_impl` @0x0a = the folded `core::noncopyable` base ctor, source is a plain
member-init-list ctor deriving noncopyable. Same class as the player_logic_base_state
folded-base-ctor corollary.

### `member = T( args )` (assign a temporary class object) = copy data members + temp dtor; residual is the dtor's `this` (ICF call boundary)
Assigning a freshly-built temporary into a class-type member, `m_x = T( args );`, emits, under
/Od: (1) construct the temp at a stack slot `[ebp-N]` (`lea eax,[ebp-N]; call T::T`), (2)
memberwise-COPY the temp's data members into `m_x` (only the data, e.g. `fld[temp+4];
fstp[m_x+4]; fld[temp+8]; fstp[m_x+8]` - the vtable at +0 is NOT touched, the assignment is
the implicit `operator=`), then (3) destroy the temp (`lea ecx,[ebp-N]; call <dtor>`). The dtor
of a trivial class is COMDAT-folded, so its symbol NAME differs base vs target (target may show
`...finalize_impl`, base `boost::function1<...>::dummy::nonnull` - same folded empty function).
Residual to expect: the target may keep `lea ecx,[ebp-N]` (setting `this` for the folded
__thiscall dtor) while base omits it (this already in a register) or vice-versa - a 3-byte
delta. That `lea ecx` is the dtor's `this` ARGUMENT at the call boundary => permitted ICF/LTCG
arg-passing class, mark DONE (~98-99%), not PARTIAL. Two requirements to even compile the
assignment: the member-class needs NON-const data members AND an accessible (implicit) operator=
- if the reconstructed header has `float const` members or a private `operator=`, the assign is
illegal (C2248); check the PDB-ground-truth structure header (`binaries/structure/target/
headers/...`) for the real const-ness/access and fix the working header to match. Caught on
`legs_ik_processor::set_{heel,toe}_on_ground(leg_params&,bool)` (98.84/98.59%): `m_*_interpolator
= fermi_interpolator( time );` - the working fermi_interpolator.h wrongly had const members +
private op=; the structure-target header showed plain non-const floats and no op=.

### bone-object-space matrix index: `matrices[bone_index - skeleton.get_root_bones_count()]`, re-called per use (do NOT hoist)
ASM (repeated per matrix in a function that builds several bone matrices):
    mov ecx, [this]                  ; this
    mov eax, [ecx]                   ; m_skeleton (a private base member, read via inline accessor)
    call vostok::animation::skeleton::get_root_bones_count   ; leaf, `this`(m_skeleton) passed in EAX (LTCG)
    mov edx, [params]; mov edx, [edx+BONEOFF]   ; params.<bone>_bone_index
    sub edx, eax                     ; index - root_count
    shl edx, 6                       ; * sizeof(float4x4)=0x40
    add edx, [matrices_arg]          ; &matrices[index - root_count]
SOURCE: `matrices[params.<bone>_bone_index - get_skeleton().get_root_bones_count()]`.
NOTES: the original source calls `get_root_bones_count()` FRESH at every matrix site (the
index helper is inlined each time) - **do NOT hoist it into one `u32 root_count` local**. A
hoisted local caches `m_skeleton->get_root_bones_count()` once (`mov [ebp-N],eax` then reuse),
which DROPS the repeated `mov eax,[m_skeleton]; call get_root_bones_count` the target keeps at
every site (objdiff shows `+ call`/`+ mov` insertions per matrix). `add eax,0x30` after such an
address = `.c.xyz()` (the float4x4 position row @ +0x30; the `xyz()` accessor folds to the
0x3f210 empty-fn / delinker `finalize_impl`). A private base member (`ik_processor::m_skeleton`)
is reached via a protected inline `get_skeleton()` accessor added to the base (no byte change).
Confirmed in `legs_ik_processor::get_foot_fixed_transform` (84.16%, target rva 0x6ebae0): the
single biggest fix was un-hoisting root_count (81.55 -> 84.16). A `float4x4 const&` ref bound to
`matrices[...] * hip_world_matrix` materializes the product into a stack slot and stores its
address (`lea;mov [ebp-N],addr`); a recorded ref local that is only stored-then-reloaded-once
(no later use) is a declared-but-unused source local present in BOTH binaries - keep it (the
C4189 "initialized but not referenced" warning matches the target), do not delete it.

### A single low-byte store into a multi-byte member = a single-FIELD write (setter / single-arg ctor), NOT a multi-arg ctor
SYMPTOM: a statement that assigns into a class member emits only ONE byte store, e.g.
`mov byte[tmp],64h; mov cl,[tmp]; mov [member],cl` (0x64=100), where you wrote a full
multi-argument constructor. A full `color( r, g, b )` / `T( a, b, c )` ctor writes ALL the
fields (4 byte stores for a packed color, N for a struct); a single byte store means the
source touched exactly ONE field. So the original wrote either a SINGLE-FIELD constructor
or a single-FIELD SETTER, not the multi-arg ctor. RULE: when the byte count says one field
but your source builds the whole object, read the type's STRUCTURE for its single-field
options - a `T( one_value )` ctor (`color( u32 )`), or a per-field setter (`set_B( 0x64 )`,
`set_x( .. )`) - and pick the one whose store matches. Caught on
`legs_ik_processor::get_foot_fixed_transform` else-branch: target did
`original_color.set_B( 0x64u )` (one channel on the packed-union color), not
`color( 0x64u, 0x00u, 0x00u )` (which writes three). The structure's constructor/accessor
list is the menu; the byte count tells you how many fields the statement is allowed to touch.

### `if(identity(false)){ ... call empty-fn(args) }` = a Master-Gold ASSERT_*_U (the eater shape)
ASM:
    mov byte[ebp-N],0; lea eax,[ebp-N]; call <folded-empty>   ; if ( ::vostok::identity(false) )
    movzx eax,byte[eax]; test eax,eax; je .skip
    push <v2>; push <v1>; ...; call <folded-empty>; add esp,M ; expression_eater( args )
  .skip:
SOURCE: a compiled-out `ASSERT_CMP_U( v1, op, v2 )` / `ASSERT_U( expr )`. Master Gold defines the
`_U` asserts as `if (::vostok::identity(false)) { ::vostok::debug::detail::expression_eater(...); }`
(`debug_macros.h`); the eater is `void expression_eater(...) {}` (varargs, folded empty). So the
discarded args get pushed C-style and the call is a folded-empty (delinker misnames it
`finalize_impl`/`empty_stub`/0x3f210). DISTINGUISH from the plain non-`_U` `ASSERT(...)` which is
`VOSTOK_EMPTY_EXPRESSION` = just `if(identity(false)){}` (NO eater, NO arg pushes). So a guard
FOLLOWED BY arg pushes + a folded call = a `_U` assert; recover the args from the pushes:
- `ASSERT_CMP_U(v1,==,4)` -> `expression_eater(v1, 4, assert_untyped)` -> `push 0; push 4; push v1`.
- `ASSERT_U(expr)` -> `expression_eater(assert_untyped, expr)` -> pushes expr then `push 0`.
NOTE the order: `assert_untyped`(=0) is pushed LAST (it is an early macro arg) for `_U`, FIRST/last
varies by form - read the push sequence. A class-typed arg (e.g. an `animation_lexeme`, 0x84) is
copied by value (`sub esp,0x84; rep movsd`). If the target's eater receives EXACTLY TWO args, both
runtime values, with NO `assert_untyped push 0`, it is the raw typed-untyped form
`ASSERT_T_U( assert_type, expression )` -> `expression_eater(assert_type, expression)` (= the bare
`VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS` macro before `ASSERT_U` wraps it with `assert_untyped`). The
assert_type slot just holds the first runtime value - do NOT reach for `ASSERT_U`/`ASSERT_CMP_U`
(both ADD the `push 0`). Push order is C right-to-left: `expression_eater(a, b)` -> `push b; push a`.
Confirmed BYTE-PERFECT in `weapon_user_animations_selector::on_broken_limb_affect` (L337 =
`ASSERT_T_U( bodypart, type )` -> `push [ebp+10h](type); push [ebp+8](bodypart)`). Other confirmations:
`weapon_core_idle_state::weapon_core_idle_state` (ctor line 21 = `ASSERT_CMP_U(animations_count,==,4)`,
100%) and `weapon_core_idle_state::weapon_and_hands_expression` (line 32 `ASSERT_U(weight_driving_animation)`).
CRITICAL AMBIGUITY: this eater shape is NOT unique to ASSERT_U - `VOSTOK_UNREFERENCED_PARAMETERS` emits
the IDENTICAL `if(identity(false)){ helper(args); }` (see next entry). If the pushed args are the
function's OWN otherwise-unused parameters (a `user`/`packet`/`dt` it never reads), it is the
unreferenced-parameter macro, NOT an ASSERT. For an unused param, default to UNREFERENCED_PARAMETER(S).

### `(void)(&p)` / `if(identity(false)){ unreferenced_parameter_helper(params); }` = VOSTOK_UNREFERENCED_PARAMETER(S)
`macro_unreferenced_parameter.h` defines two macros, both to silence an unused parameter (the original
code wrote one whenever a param is taken but not read - virtual overrides, callbacks, stubs).
UNDER-USING them is a red flag: a `user`/`packet`/`dt`-style param with no body reads almost always had one.
- `VOSTOK_UNREFERENCED_PARAMETER(p)` = `(void)(&p)`. ASM /Od: a lone `lea reg,[ebp+p]` (address-of the
  param) that is discarded - NO mov-byte, NO call. A bare `lea` of a parameter slot with no consumer is
  this, not a dead local.
- `VOSTOK_UNREFERENCED_PARAMETERS(a,b,...)` = `if ( vostok::identity(false) ) { vostok::detail::
  unreferenced_parameter_helper(a,b,...); } else (void)0`. ASM: the SAME eater shape as the ASSERT_U entry
  above (the `mov byte[ebp-N],0; lea; call <folded-empty>` guard, then the never-taken block pushes
  a,b,... and calls the folded-empty `unreferenced_parameter_helper`). DISTINGUISH from ASSERT_U purely by
  WHAT is pushed: the function's own unused PARAMETERS -> this macro; an asserted expression's operands ->
  ASSERT_U. (Both helpers are varargs folded-empty; the delinker misnames both `empty_stub`/`finalize_impl`.)
SOURCE: write the macro, never hand-roll `(void)(&p)`. Def in `sources/vostok/macro_unreferenced_parameter.h`
(244 uses across our game_core/animation sources - reach for it before labeling an unused-param eater an ASSERT).

### `/Od` counted loop with `je` (not `jae`) exit = source `for(...; i != N; ...)` not `i < N`
ASM (target): `cmp [i], N; je .end` (loop exits on equality). SOURCE: `for ( u32 i = 0 ; i != N ; ++i )`.
If your base emits `cmp [i], N; jae .end` instead, your source wrote `i < N` (unsigned `<` lowers to
`jae`-exit). MSVC /Od preserves the exact comparator: `!=`->`jne`/`je`-exit, `<`->`jb`/`jae`-exit.
Match the operator the target used (the original counted loops here were written `i != 2`). Confirmed
100% in `weapon_core_idle_state`/`weapon_core_aimed_state` ctors (two nested `for(... != 2 ...)`).

### `movzx; neg; sbb X,X; neg` on a bool array index = `arr[ b != false ]`, NOT `arr[ b ]`
A bare `arr[bool_param]` index lowers to just `movzx eax, byte[param]` (the bool is already 0/1). If
the target instead emits the full `movzx; neg; sbb eax,eax; neg` normalize before the index scale,
the source wrote an explicit `!= 0` / `!= false` comparison: `arr[ is_third_view != false ]`. Same
`neg/sbb/neg` idiom as the `(val & mask) != 0` entry above, here applied to a plain bool used as an
index. Confirmed 100% in `weapon_core_idle_state::get_weapon_lexeme_pair`
(`m_weapon_animations[ is_third_view != false ][ user_state_id == type_crouch ]`; the 2nd index is a
plain `== type_crouch` -> `cmp 1; sete`).

### placement `new ( buffer ) T(...)` on a `mutable_buffer` -> `new ( buffer.c_ptr() ) T(...)`
`mutable_buffer` has NO conversion to `void*` (only `operator bool`), so `new ( buffer ) T` fails
C2665 (operator new overloads are only `(size_t,nothrow_t&)` and `(size_t,void*)`). The standard
placement form is `new ( buffer.c_ptr() ) T(...)`. ASM: `lea &buffer; call <c_ptr, misnamed
operator*>; push; push sizeof(T); call operator new; <null-check> je; <T ctor>`. The `call operator*`
is `mutable_buffer::c_ptr()` kept out-of-line (LTCG inline-vs-call) - do not read it literally. The
post-`operator new` `cmp [p],0; je` is the standard placement-new null guard (skip the ctor if null).
Confirmed 100% in `weapon_core_state_cook_template<weapon_core_idle_state>::new_object`
(`return new ( buffer.c_ptr() ) weapon_core_idle_state( params->weapon, animations, animations_count );`).

### a `call_destructor`-style helper (destruct WITHOUT freeing) -> `obj.~T()`, NOT `delete &obj`
A static helper whose job is to run an object's destructor but leave the storage (which an
allocator owns separately) is `obj.~T( );` - an EXPLICIT destructor call. Do NOT write
`delete &obj;`: that compiles to the dtor PLUS an inlined `operator delete` (it frees the
storage too), ~`0x50` bytes vs the target's ~`0x2c` (the difference being the extra
`operator delete`). Confirmed in `udp_match_packet::helper::call_destructor`
(43.2 -> 100.0 switching `delete &packet;` to `packet.~udp_match_packet( );`).

### allocator `new_X` / `delete_X` free-function helpers - the exact statement shapes
A `new_X( allocator )` that placement-news into raw storage is THREE statements: `T* const
result = (T*)allocator.allocate( ); new ( result ) T( ); return result;` - cast the
`allocate()` result into a `result` local, placement-new into it, return it (do NOT fuse to
`new ( allocator.allocate() ) T` - the target keeps the cast-into-local + separate `new`).
The mirror `delete_X( allocator, ptr )` is FOUR statements: `X::helper::call_destructor(
*ptr ); void* buffer = ptr; allocator.deallocate( buffer ); ptr = NULL;` - note the `void*
buffer = ptr;` temp (a real statement, NOT `deallocate( reinterpret_cast<pvoid&>(ptr) )`)
and the explicit `ptr = NULL;`. Confirmed in `new_udp_match_packet` (13.7 -> 99.7, 3/3
stmts) and `delete_udp_match_packet` (0 -> 61.3, 4/4 stmts); residuals are LTCG frame/inline.

### expression( lexeme_a + lexeme_b ) -> operator+ then expression ctor (each does a cloned_in_buffer)
`animation::mixing::expression( a + b )` where a,b are `animation_lexeme&`: `operator+<L,L>` builds an
`addition_lexeme(a,b)` temp, calls `.cloned_in_buffer()`, destroys the temp, returns the clone as
`addition_lexeme&`; then `expression::expression<addition_lexeme>(ref)` (an implicit template ctor)
does ANOTHER `cloned_in_buffer()` internally. The idiomatic source `expression( a + b )` is correct;
do NOT hand-expand it to `expression( *addition_lexeme(a,b).cloned_in_buffer() )` - that scores LOWER
(introduces extra `~expression` temps and inlines `base_lexeme::cloned_in_buffer<>` instead of the
target's out-of-line `addition_lexeme::cloned_in_buffer()`). Whether operator+ is inlined or kept a
`call` at the site is per-call-site LTCG (operator+ is standalone in BOTH indexes), same class as the
operator| entry. Confirmed in `weapon_core_idle_state::weapon_and_hands_expression` (85.65%, residual =
that operator+ inline-vs-call): `main + offset` scored 85.65 vs the hand-expansion's 80.30.

### Lone assert eater (no branch) = compiled-out NON-_U `ASSERT( ... )`
A standalone `mov byte ptr [ebp-XX], 0 ; lea eax, [ebp-XX] ; call empty_stub` with NO following
`movzx/test/je` and NO arg pushes is a compiled-out plain `ASSERT( expr )` (= `VOSTOK_EMPTY_EXPRESSION`
= `if(::vostok::identity(false)){}else(void)0`; debug_macros.h). Under /Od the empty-`if` body lets MSVC
drop the branch, leaving just the `identity(false)` guard call (the delinker misnames it
`empty_stub`/`finalize_impl`). SOURCE: `ASSERT( UNKNOWN_EXPRESSION )`. DISTINGUISH from the `_U` asserts
(`ASSERT_U`/`ASSERT_CMP_U`) which ALSO emit `movzx;test;je; push..; call expression_eater; add esp` (the
eater body) - using a `_U` form for a lone eater over-produces those bytes. Confirmed in
`pistol_/double_barreled_weapon_core_idle_state` ctors (trailing ASSERT after the fill loop) and the
double-barreled getter (leading ASSERT). The double-barreled ctor has a leading lone `ASSERT` then an
`ASSERT_CMP_U(count,==,12)` -> two byte-stores, only the second tested.

### Inline class-body accessor inlined by base but called by target -> move it out-of-line
When the target diff shows `call ?accessor@class@@...` but base shows the inlined member read
(e.g. `mov ax,[reg+off]; mov [ebp-XX],ax; movzx`), the cause is a trivial accessor defined IN the
class body (implicitly inline) that the target's build kept out-of-line at the call site. FIX: declare
it in the header and define it in the .cpp. This is NOT just one instruction - the inlined temp grows
the frame (`sub esp` differs by ~4) and clobbers a different register than a `call` would, cascading the
WHOLE function's register allocation. Confirmed: moving `weapon_core::ammo_in_magazine()` out-of-line
took `pistol_/double_barreled_weapon_core_idle_state::get_weapon_lexeme_pair` from 77/92% to 99.92%.

### Out-line an EMPTY inline virtual to recover the target's qualified `call` (STEERABLE)
SYMPTOM: `--view diff` on a derived state shows the TARGET emitting `call base::execute` (an empty
base virtual called qualified) then the member store, while OUR BASE has NO call - it inlined the
empty `{}` at the call site (~80%, the missing call bytes). CAUSE: the base method's empty body was
defined INLINE in the header, so /GL inlines the no-op at every qualified call site; the target keeps
a standalone out-of-line body and emits a real `call`. FIX: out-line the empty body - keep the decl
in the header (same access char), move the `{}` definition to the .cpp. Confirmed
`weapon_core_fire_state_base::execute` 80.91% -> 99.09% (via `weapon_core_base_state::execute`).
Regression-free ONLY if the out-of-line body's base method has a single qualified call site (else
out-lining changes other derived classes' codegen). Same device class as out-lining a trivial accessor
(round_is_chambered / ammo_in_magazine).
### `push 0; mov r,val; sub r,-1; neg r; sbb r,r; lea x,&obj; and r,x` = `val != u32(-1) ? &obj : NULL`
ASM (target, materializing a conditional pointer arg):
    push 0                       ; (an unrelated arg pushed first)
    mov  edx, [ebp+20h]          ; val (a u32)
    sub  edx, 0FFFFFFFFh         ; edx - (-1) == edx + 1
    neg  edx                     ; CF set iff edx+1 != 0  (i.e. val != -1)
    sbb  edx, edx                ; edx = (val != -1) ? 0xFFFFFFFF : 0
    lea  eax, [ebp-0E0h]         ; &obj
    and  edx, eax                ; (val != -1) ? &obj : NULL
    push edx
SOURCE: `val != u32(-1) ? &obj : NULL` (a pointer-or-NULL select on an unsigned `!= -1` test).
Write the idiom in source (a ternary), do NOT hand-roll the asm. MSVC /Od lowers the
`!= u32(-1)` compare to `sub r,-1; neg; sbb r,r` (mask = all-ones iff non-equal) and ANDs it
with the address to pick `&obj` or 0. Reproduces byte-for-byte. Confirmed in
`game_core/get_weapon_lexeme_pair_impl` (the offset lexeme's `time_driving_animation` arg =
`time_synchronization_group != u32(-1) ? &main_lexeme : NULL`).

### a lone 4-byte `mov byte[ebp-N],0` standalone statement (no lea/call) = an unused `bool b = false;`, NOT an ASSERT
A carcass/structure statement of size `<0x4>` whose only instruction is `mov byte ptr [ebp-N], 0`
with NO following `lea eax,[ebp-N]; call <empty_stub>` is a plain unused `bool` local initialized
to false (`bool b = false;`), kept as a dead store under /Od (no DCE). DISTINGUISH from a
compiled-out `ASSERT`, which is `<0xc>` (the byte-store PLUS `lea eax; call empty_stub`). Writing
an `ASSERT(UNKNOWN_EXPRESSION)` for such a `<0x4>` slot OVER-produces the lea+call. The disp size
(4 vs 7 bytes) only reflects whether MSVC put the slot at a small or large `[ebp-N]` offset -
allocation noise, not a mismatch. Confirmed in `game_core/get_weapon_lexeme_pair_impl` (L40,
target `<0x4>` `mov byte[ebp-5],0`).

### forward-kinematics chain: `mat = matrices[idx] * parent_obj` and the `operator*(out,A,B)` push order
A run of `operator*` calls each `rep movsd 0x10` into a fresh 0x40-byte `[ebp-N]` slot, where
each result is consumed as an operand of the NEXT call, is a forward-kinematics object-space
chain: `up_leg_obj = matrices[up_leg_idx] * hip; knee_obj = matrices[knee_idx] * up_leg_obj;
leg_obj = matrices[leg_idx] * knee_obj; ...`. PUSH ORDER for `operator*(float4x4 const& A,
float4x4 const& B)` (cdecl, hidden return ptr): right-to-left = `push B; push A; push out`, so
`result = A * B` = `(2nd-pushed) * (1st-pushed)`. Same convention for `operator-`/`operator^`
(float3): `operator-(out, ecx=A, edx=B)` -> `result = A - B`. Decode the subtraction ORDER
from which operand lands in ecx (A) vs edx (B) - getting it backwards negates the float3 and
diverges downstream.

### `.i.xyz()`/`.c.xyz()` fold = delinker-misnamed `finalize_impl` call, with/without `add eax,30h`
The trivial `float4_pod::xyz()` (and `float3_pod` identity/length) folds are emitted as a `call`
the delinker misnames `fixed_size_allocator<...>::finalize_impl`. `lea eax,[mat]; call finalize_impl`
= `mat.i.xyz()` (row .i at +0). `add eax,30h; call finalize_impl` = `mat.c.xyz()` (the .c position
row at +0x30). `add eax,10h`/`+0x20` = `.j`/`.k`. A separate `call float3_pod::length` after it =
`.xyz().length()`.

### by-value temp built BEFORE trailing arg pushes = HOIST it to a NAMED local
When the target materializes a by-value argument temp FIRST (`push 0; call ctor` writing
`[ebp-4]`) and only THEN pushes the remaining args (`push this`, recomputing `&temp` via
`lea ecx,[ebp-4]`), but inline-rvalue codegen reorders it (pushes `this` first, then builds
the temp and pushes the ctor's eax return), the cause is temp-scheduling - NOT LTCG, NOT a
wall. FIX: hoist the rvalue into a NAMED local declared on the line above the call:
`T tmp( ... ); f( ..., tmp, ... );`. The named local pins the ctor to its declaration point,
ahead of the argument pushes; its dtor still fires at end-of-scope, matching the target's
trailing dtor. Confirmed: `weapon_core::set_animation_callback` (both overloads) -
`managed_resource_ptr( NULL )` inline temp 80.52%/81.17% -> NAMED local 100%/100%.

### a write/read at `[ebp+arg]+0xNN` is a MEMBER of an argument struct, not a local
When asm writes `mov eax,[ebp+8]; add eax,0x1C; ...` (storing a normalize/operator result there),
[ebp+8] is the by-ref struct ARG and +0x1C is a member offset - it is `arg.member = ...`, NOT a
local. Confirmed in `game_core/legs_ik_processor::process_leg`: `params.rotation_axis` (leg_params
+0x1C, a public float3) is the IK rotation axis written via `params.rotation_axis = normalize(a^b)`
and re-read by `create_rotation(params.rotation_axis, angle)` - originally mis-decoded as a
`up_leg_obj_matrix.i.xyz()` matrix-row write, which over-produced the `.i.xyz()` lvalue xyz-fold.
Writing the member directly avoids that fold and matches.

### console-variable static initializer (`cc_bool`/`cc_float`/...) - `dynamic initializer for 's_*_cc'`
A `void survarium::`dynamic initializer for 's_NAME_cc''()` whose body is
`push <serializable>; push &s_NAME_value; push ??_C@...NAME; mov esi,&s_NAME_cc; call cc_TYPE::cc_TYPE;
push <atexit-dtor>; call atexit` is a file-static console command. Reproduce it as a single
`static console_commands::cc_TYPE s_NAME_cc( "NAME", s_NAME_value, ... );` (header
`<vostok/console_command.h>`; see dispersion_calculator.cpp / bullet.cpp). cc_bool args =
`(name, bool& value, bool serializable, command_type, execution_filter=general)`; cc_float =
`(name, float& value, float min, float max, bool serializable, command_type, execution_filter=general)`.
The string name is in the `??_C@_0..@..@NAME?$AA@` operand. min arrives in `xmm0`, max via
`fld [const]; fstp [esp]`. NOTE: objdiff scores EVERY `dynamic initializer`/`dynamic atexit destructor`
thunk 0% - it does not pair the base `??__E.../??__F...` mangled names with the demangled target names.
The emitted bytes still match (cc_float byte-identical; cc_bool's only diff is the ctor passing
command_type/execution_filter in registers under LTCG vs on the stack in base). Treat as DONE.

### compiler-generated dtor: missing member-dtor `this`-pointer setup is ICF folding
A destructor where the target sets `mov ecx,[this]; add ecx,0xNN` before each trivial member
`~T()` call but the base omits those `add ecx` setups (still issuing the calls) is ICF/codegen
folding the identical member-dtor `this` adjustments - not source-steerable. The only real source is
the explicit body (e.g. `DELETE(m_drawer)`); the member-dtor epilogue is auto-emitted.
(game_core/legs_ik_processor::~legs_ik_processor, 85.71%.)

### per-call `get_skeleton()`/reference-return spill = uniform `[ebp-N]` shift, not a brace bug
When `f(*ptr, accessor_returning_ref(), ...)` is called repeatedly and the BASE spills the
ref-returning accessor into a fresh stack temp per call while the TARGET inlines the deref
(`mov ecx,[this]; mov edx,[ecx]; push edx`), the base frame grows by 4 bytes per spilled site and
EVERY later `[ebp-N]` slot shifts by that total - a uniform slot-rename storm with NO `[n]`
block-open / `+`/`-` control-flow divergence. Distinguish from the genuine brace-scope storm
(§2a): if the diff has zero target-only/base-only rows and the slot deltas are all the SAME
constant, it is a temp-spill/LTCG artifact, not a missing brace. (game_core/legs_ik_processor::process.)

### three sequential `[1]` block-opens = three separate braced `{ }` scopes (not nesting)
When a `/Od` carcass shows N `[1]` block-opens (depth resets to 1 each time, never `[2]`) at
distinct srclines with no enclosing `[1]` between them, they are N SEPARATE sibling braced
blocks `{ ... }`, each opening and closing before the next - NOT one nested scope. Brace each
stage; the block-scoped locals then REUSE stack slots across blocks (the target's slot
overlap), which is what collapses a `[ebp-N]` slot-rename storm that flat-at-function-scope
source produced. Read the FIRST statement after each `[1]` from the asm: a recompute/assignment
that the flat source wrote at function scope BETWEEN stages actually belongs as the first
statement INSIDE the next block (e.g. `knee_obj = m[knee]*up_leg` opens block 2; `leg_obj =
m[leg]*knee` opens block 3). Also re-read intra-block statement ORDER from the asm - the target
may recompute a matrix earlier than the flat source did, so a dependent `normalize(a.c - b.c)`
reads the fresh value, not a stale one. (game_core/legs_ik_processor::process_leg, 78.81->80.96%.)

### `add reg,0x30; call <X>::finalize_impl; mov esi/ecx,eax` feeding a `float3 const&` = `matrix.c.xyz()`
On a `float4x4` (size 0x40, members i@0/j@0x10/k@0x20/c@0x30, each a `float4_pod`), an
`add reg, 0x30` computes `&matrix.c`, and the following `call <some_class>::finalize_impl`
returning `eax` is the COMDAT-folded `float4_pod::xyz()` (returns `float3 const&` = first 12
bytes of `c`). The delinker misnames the folded thunk after whatever symbol ICF kept (commonly
`vostok::memory::fixed_size_allocator<...>::finalize_impl`). When the returned `eax` is then
pushed/moved as a `float3 const&` argument, the source is `matrix.c.xyz()` - NOT an allocator
call and NOT a compiled-out ASSERT/`empty_stub`. (Confirmed: sibling legs_ik_processor.cpp uses
`.c.xyz()` throughout; game_core/legs_ik_drawer::draw_leg.)

### thin forwarder `m_renderer.draw_X(m_scene, ...)`: float/int arg passed in xmm0/eax vs spilled to stack = LTCG call-boundary residual, NOT a source bug
A one-line debug-draw forwarder `member_ref.method( other_member_ref, a, b, c )` (member_ref at
this+0, scene_ptr at this+4 via `add ecx,4`) can match 100% for some overloads and stall at
60-80% for sibling overloads with the SAME forwarding shape. The divergence is purely at the
call boundary: e.g. the TARGET passes a `const float` arg in `movss xmm0,[ebp+..]` (register)
while the BASE passes it on the stack (`fld [ebp+..]; fstp [esp]`), or which integer arg ends in
`eax` vs gets pushed differs. This is whole-program LTCG calling-convention specialization
dictated by the (possibly unmatched) callee; it cannot be steered from the forwarder's source -
the two 100%-matching siblings prove the source is right. Stop at PARTIAL and name the cause; do
NOT chase it. (game_core/legs_ik_drawer: draw_cross/draw_line_capsule 100%, draw_origin 62.88%,
draw_solid_capsule 79.43%, draw_leg 73.36% - same draw_origin xmm0-vs-fld/fstp residual x4.)

## A file-static `cc_bool` console command -> `dynamic initializer` is objdiff-UNSCORABLE (None)
A `static console_commands::cc_bool s_x_cc( "name", s_x_value, serializable, command_type )` at
file scope compiles its construction into a `dynamic initializer for 's_x_cc'` (plus a matching
`dynamic atexit destructor`). Both score **None** in report.json and have **no standalone symbol**
in `binaries/rich/base/index.jsonl` - the per-TU init/atexit thunks are LTCG/ICF-folded so the
delinker can't re-attach them. The body is still emitted and byte-correct. Recognize the asm:
`push 1`(serializable) / `push s_x_value` / `push "name"` ; `mov eax,<command_type>` ; `xor ecx,ecx`
(execution_filter_general default) ; `mov esi,s_x_cc`(this) ; `call cc_bool::cc_bool` ; then
`push <atexit dtor> ; call atexit`. Mirror an existing matched sibling
(dispersion_calculator's `s_dispersion_enabled_cc`, also None) and mark None|DONE - do not chase
the symbol. command_type values: engine_internal=0 (eax=0), user_specific=1 (eax=1).

## A `tick`/update with FPU vibration math hit 94% with structure 1:1; residual is /Od frame-slot churn
A member `tick` that reads `[ebp+8]`(time arg)/`[ebp+0Ch]`(scale) and does `fild qword; fmul
[epsilon_3]; fmul scale` (= `(a-b)*0.001f*scale`), `fsm::tick()`, a `static_cast<derived*>(
m_logic.current_state())` (`mov [+10h]`), a `[vtbl+N]` virtual tick, a `cond ? math::max(...) :
math::min(...)` clamp store, and a `[vtbl+M]` virtual returning u32 feeding a `sin(phase/period)*
amp*...` FPU chain - all reproduce 1:1 from source (member offsets straight off the asm,
math::max/min/sin stay out-of-line). The remaining ~6% is `sub esp,38h` (target) vs `30h` (base):
2 extra temp dword slots shift the saved-`this` slot ([ebp-24h] vs [ebp-1Ch]) and swap which
register holds the vtable at the second virtual call. Pure /Od register/slot allocation, NOT a
missing local/brace/ASSERT/statement - the LOCALS all map, the carcass structure matches. Stop at
PARTIAL. (breath_vibration_calculator::tick, 94.23%.)

### a `this`-UNUSED trivial member (`return literal`/`return 0.0f`) is FRAMELESS in the target, framed under /Od
SYMPTOM: a trivial member fn that NEVER references `this` (returns a string literal, `return 0.0f`,
`return NULL` that ignores members) has a TARGET obj body with NO ebp frame - e.g. `get_speed` =
`d9 ee c3` (`fldz; ret`), `use_info` = `b8 <reloc> c2 0400` (`mov eax,lit; ret 4`) - while our /Od
BASE emits the full frame (`55 8bec 51 894dfc <body> 8be5 5d c3`, i.e. `push ebp; mov ebp,esp; push
ecx; mov [ebp-4],ecx; ...`). The original build applied frame-pointer omission for `this`-unused
leaves; `/Od` ALWAYS emits the frame + the `mov [ebp-4],ecx` save-this. The 3-5 vs 11+ byte gap is
too large for objdiff to pair -> `fuzzy: None` even though the semantic body (the fldz / literal /
ret N) is correct. NOT source-steerable under /Od (frame omission is a build flag). DISTINGUISH from
ICF-fold None: these survive STANDALONE in the EXE (qualified-call anchor) at a real rva - they are
present-but-divergent, mark None|PARTIAL. CONTRAST: a member that USES `this` (reads a member, the
ctor/dtor storing into `this`) keeps its frame in BOTH and matches 100% (e.g. damage_protector
ctor/dtor). So frame-presence tracks `this`-usage. Confirmed: artefact_container_core::use_info,
booby_trap_core::get_speed (both None|PARTIAL, frameless target vs /Od frame).

### `u32_diff -> [lo]; mov [hi],0; fild qword; fdiv 1000.0` = `float dt = ( u32a - u32b ) / 1000.0f`
A millisecond->seconds delta `float dt = ( current_time - last_time ) / 1000.0f` where both
operands are `u32`: MSVC computes the u32 subtraction, stores it as the LOW dword of a 64-bit
slot, zeroes the HIGH dword, then `fild qword` (there is no unsigned-32 FPU load, so MSVC
zero-extends the u32 to i64 and converts the i64), and `fdiv [__real@447a0000]` (0x447a0000 =
1000.0f). One statement, ~0x1f bytes. Do NOT mistake the `fild qword`/zeroed-high for a real
`u64`/`__int64` local - it is the standard u32->float widening of a plain `u32` difference.
Confirmed in `game_core/character_dispersion_calculator::tick` (L59, dt the lone PDB local).

### asio completion bind: use `boost::asio::placeholders::error`/`::bytes_transferred`, not boost `_1`/`_2`
SYMPTOM: a `boost::bind(&on_xxx, this, _1, _2)` feeding an `async_read`/`async_write`
completion scores ~94% with a head divergence: target does
`mov eax,[?error@...placeholders@asio@boost@@...]; movzx byte[eax]; push` (reads the
placeholder object from a global), while base emits `movzx ecx, byte[_1]` (boost's global
`_1` literal). Both compile, but they are DIFFERENT placeholder objects.
FIX: for asio completion handlers the source uses the asio placeholders -
`boost::asio::placeholders::error` and `boost::asio::placeholders::bytes_transferred`
(or `::iterator` for resolvers) - NOT boost's `_1`/`_2`. Confirmed in
`network_core/tcp_packet_socket::start_receiving` (94 -> 99.88%). Member-callback binds
that are NOT asio handlers (e.g. `m_on_error = boost::bind(&on_error, this, _1, _2)`)
correctly use boost `_1`/`_2`.

### CRTP `packet<T>` writer/reader need `friend` access to base_packet's private buffer
`base_packet` keeps `m_buffer`/`m_buffer_size` private with only getters (the const
`buffer()` stays private - PDB-matched, see tcp_packet_socket note). The CRTP writer
`packet<T>` (resize/append set `m_buffer_size`, append reads `buffer()`), the reader
`packet_reader` (eof/size_to_eof read `buffer()`/`m_buffer`), and `tcp_packet`
(reallocate writes `m_buffer`) all touch those privates. Befriend exactly those three in
base_packet (`template<typename T> friend class packet; friend class packet_reader;
friend class tcp_packet;`) - no layout change, STATIC_SIZE_ASSERT stays 0x8. `reallocate`
lives in `tcp_packet::reallocate` (allocator + 3-byte length-prefix dance, mirrors the
older non-CRTP `network::packet::reallocate`); `packet<T>::reallocate` just forwards
`implementation().reallocate(n)` and COMDAT-folds onto `packet<tcp_packet>::reallocate`
(the surviving symbol). `packet<T>::append`/`reserve`/`resize`/`allocated_size` reach the
T-specific allocator/allocated_size purely through `implementation()` so the template still
compiles for `udp_match_packet` (whose reallocate is an empty/int3 stub).

### template carcasses need member-fn-ADDRESS anchors (a call-site inlines them away)
Calling `packet.append(...)` / `reader.r<u8>()` from the IncludeAll anchor fully inlines the
body into the anchor at /Od and emits NO standalone COMDAT, so the symbol never lands in the
base index and objdiff can't score it. Take each member-function's ADDRESS instead
(`void (tcp_base::*p)(u8) = &tcp_base::append; example_callback((pcstr)&p);`) - that ODR-uses
the out-of-line body without a call site, exactly like the jump_logic_state anchors. Private
members (e.g. `packet_reader::pointer`) need the anchor befriended via the
`namespace vostok { void use_network_core_packet_reader(); }` forward-decl + `friend void
::vostok::use_network_core_packet_reader();` idiom. Template member instances are addressed as
`&reader::r<u8>` / `&reader::r_string<16>`.

### call-site call sites prevent r<T>/append from inlining away (the real lever)
A template carcass like `packet<T>::append`/`packet_reader::r<T>` scores poorly when only
ADDRESS-anchored, because its own COMDAT is the debug-quality single-TU body while the target
keeps it standalone. In the target the standalone bodies survive because real `serialize`/
`deserialize` functions CALL them (so they are out-of-line in the LTCG image). Implementing the
typed `serialize(udp_match_packet&)` / `deserialize(packet_reader&)` methods (player_input,
player_state, hit_info, sequence_number, ...) and anchoring those gives the primitives genuine
call sites. Pair it with: every scalar `packet<T>::append(uN/sN/bool)` overload must have the
`append(&value, sizeof(value))` body - an empty `/* no source */` overload inlines to nothing
and SILENTLY DROPS the append statement at the call site (caught as a "ONLY target" stmt in
structure-diff, e.g. client_player_update::serialize missing its append(time_in_ms)).

### serial-number (RFC1982) operator< / operator<=
`a < b` for wrap-around sequence numbers compiles to a two-clause OR:
`(a < b && u32(a)+0x8000 > b) || (b < a && u32(b)+0x8000 <= a)`. operator<= differs by exactly
one byte: the first clause's `<` becomes `<=` (jge->jg). `operator-(left,right)` is
`right <= left ? s16(left.m - right.m) : -(right - left)`; the `+0x10000 & 0x8000FFFF` +
sign-extend sequence IS the compiler's `(s16)` cast, not extra logic.

### LOG_ERROR / __FILE__ residual is permanent on logging-heavy functions
Every `LOG_*` bakes `__FILE__` into the call. Target's is `C:\survarium\sources\vostok\...`;
our base build path is `Z:\home\...` (Wine). The `push <path-string>` therefore never
matches, capping logging-dominated functions (e.g. tcp_packet_socket::on_packet_received/
on_packet_size_received ~40%, on_packet_has_been_sent ~52%) well below 100% even when the
control flow is fully matched. Not source-steerable - same wall as http_client's LOG lines.

### `objdiff fuzzy_match_percent: None` can mean "body too divergent", not only bad mangling
SYMPTOM: report.json omits the percent for a function whose mangled name matches the target's
exactly and is present in the base obj. CAUSE (besides access-specifier, above): the base body
diverges structurally enough (e.g. a whole missing compare/branch block) that objdiff's symbol
diff bails without a number. Before assuming unreachable/mangling: byte-compare the two `.text`
regions; if the symbol is present with the right name, fix the body shape, not the header.

### LTCG dead-store elimination: a /Od+/GL ctor with no real caller compiles EMPTY
ASM (base, the same correct ctor source as above) for a function only reachable
via a synthetic `temp_include_all` anchor:
    push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx; mov eax,[ebp-4]; leave; ret
    ; i.e. NO member stores at all - the whole init list is gone.
CAUSE: Master Gold game_core is `Optimization=0` (/Od) **plus**
`WholeProgramOptimization=1` (/GL = LTCG). The obj holds IL, and the linker does
whole-program codegen: if the constructed object is never *observed* by a real
consumer, LTCG proves every member store dead and emits an empty ctor body.
NOT steerable from the anchor: escaping `&params` to an opaque external
(`example_callback` -> `printf("%s")`) does NOT count as observing the float
members, so the stores still vanish. Real matched value-struct ctors (e.g.
`animation_analysis_result::animation_analysis_result`, 85.98% DONE) are NOT
anchored in temp_include_all at all - they survive only because a real reachable
game caller observes them. So: a constant-only default ctor whose only caller is
the anchor is a PARTIAL until its real callers are matched - the body is right,
but the base bytes are LTCG-emptied. Confirmed in
`game_core/weapon_recoil_params::weapon_recoil_params()` (18.18%).

### Single-TU anchor INLINES a now-real inline helper where the target CALLS it
SYMPTOM: a consumer that calls a small `inline` helper (free function or template, e.g.
`packet_reader::r<u16>`, `delete_udp_match_packet`, `new_udp_match_packet`'s `udp_match_packet`
ctor) shows the target emitting a single `call <helper>` while the base inlines the helper body
inline at the call site (the `call_destructor`+`deallocate`+`decrement` dance instead of one
`call delete_udp_match_packet`; the `mov dx,[ecx]; add eax,2` instead of `call r<unsigned short>`).
A pure inline free function with no out-of-line definition may not emit a standalone COMDAT in
the base AT ALL (delete_udp_match_packet was absent from the base index -> 0%).
CAUSE: the target's whole-program (/GL LTCG) build keeps these inline helpers as one out-of-line
COMDAT and calls it from every site; our base instantiates them in a single anchor TU where
MSVC inlines the small inline body at each `/Od` call site. This is the SAME wall the
packet_reader_inline.h header note describes ("a single-TU anchor cannot reproduce that").
CONSEQUENCE: making the helper's body real does NOT make the consumer match - the consumer now
inlines the real body instead of the old empty stub. Mark such consumers PARTIAL with the
inline-vs-call residual; do not chase the % from the consuming unit's source. (Seen across the
whole udp_match_connection unit: enqueue 4/5 stmts aligned, is_low_level_packet, new_udp_match_packet.)

### A speculative ASSERT in an INLINE ctor multiplies into every inline site - verify against ALL of them
A header-inline ctor/method is expanded at every call site, so an `ASSERT( UNKNOWN_EXPRESSION )`
placed in it speculatively emits the ~0x10-byte eater (`mov byte[ebp-N],0; lea eax,[ebp-N]; call
<folded-empty>`) at EVERY inlining consumer - and if the target sites do not show the eater, the
guess silently caps every consumer's %. Before adding an ASSERT to an inline body, check the
target bytes at EACH inline site (rich-view the consumers; the expansion is delimited by the
member-address temp reload before and the member store after). Caught on
`network_core/handler_allocator::handler_allocator()`: a speculative ASSERT emitted the eater
inside THREE ctors while both target sites (`udp_match_connection` ctor 0x205-0x219,
`udp_match_client` ctor 0x191-0x1ad) show only `call <folded sub-ctor>; mov byte[this+400h],0`;
removing it moved async_connector ctor 81.38->91.79, connection ctor 95.58->98.52, client ctor
77.47->79.20.

### boost::function member assign: target calls folded operator=, /Od base may inline the copy-swap-clear body
`m_fn = value;` (boost::function1/2 operator=(function const&)) in the TARGET is one
`call <ICF-folded operator= rep>` (often shown under an unrelated signature like
`boost::function<void(char const*)>::operator=`, this/args LTCG-reg-promoted, e.g. dest in edi).
The /Od+LTCG BASE may instead INLINE operator='s body at the site: construct a 0x20-byte
`function` temp copy (`lea ecx,[tmp]; push src; call function::function`), `push dest; call
function1::swap`, then TWO `call clear` (temp dtor + bind-temp dtor) - growing the frame
(+0x0C..0x10) and freeing esi/edi (the missing `push esi/edi` in the prologue is a TELL).
Same per-call-site whole-program LTCG inline-vs-call class as operator|/vectora::size - the
source spelling (`m_fn = value`, or an inline setter containing it) is already correct; mark
PARTIAL. Confirmed in `network_core/udp_match_client::udp_match_client` (set_on_disconnect
inlined; body stmt 0x73 vs target 0x59, 79.20% PARTIAL).

### LOG_* macros compile __LINE__ immediates - do NOT pad for them, take the % hit
The logging helper (`LOG_ERROR`/`LOG_WARNING`, logging_extensions.h) pushes `__LINE__` as an
IMMEDIATE (`push 61h` = line 97) into the append call, so a function containing LOG sites only
byte-matches when each LOG statement sits at the target's original line number (read it off the
pushed immediate / the carcass `'NN'`). For a MULTI-LINE macro invocation, VS2008 takes
`__LINE__` (and the /Od line-table entry for the whole statement) from the CLOSING-PAREN line -
a LOG spread over 4 lines with `);` on line 97 pushes 97.
**Do NOT pad the `.cpp` to land these lines (sushi)** - accept the few-byte residual and note it
(`__LINE__ immediate, N sites; +1 size class if the true line crosses the imm8/imm32 boundary at
128`). Padding makes the file line-FRAGILE: every edit above the pins (sibling units, verifier
embeds, restacks) silently breaks the bytes, and the maintenance cost dwarfs the points (proven
by the `udp_match_client.cpp` pin churn). That file's existing padding (sites 97/103/109 + 172)
predates this rule, is already matched, and stays - preserve its pins while it lives, but don't
create new ones.

### logging helper expansion: base schedules the boost::function1 ctor at the append site (target: block entry)
Inside each inlined LOG block the target constructs the log-callback `boost::function<...>`
FIRST (block entry, `lea eax,[fn_slot]; call <fn-ctor COMDAT>` - this in eax), then evaluates
`message()`, then `push &fn` at the append; the /Od+LTCG base instead emits the ctor call AT the
append site (`lea esi,[fn_slot]; call <ctor>; push eax` - this in esi), which also SWAPS the two
guard-bit cleanups (`[ebp-guard] & 2` destroys the function1 in base vs the message std::string
in target). All slots/structure stay identical; the residual is a few bytes per LOG plus cascaded
register renames. Same non-steerable whole-program class as the http_client::on_error precedent -
mark PARTIAL. Confirmed in `network_core/udp_match_client::handle_receive` (93.54%, 23/23 stmts).

### boost::asio socket open/bind/endpoint idioms (all matched 100% from plain source)
`if ( m_socket.is_open( ) )` inlines to `&socket -> slot; load [socket+0](service) -> slot;
cmp dword[socket+4], -1; setne` (impl.socket_ != invalid_socket). `m_socket.open( udp::v4( ) )`
materializes the protocol temp `mov dword[ebp-4],2; lea/push; call basic_socket::open`.
`m_socket.bind( udp::endpoint( ) )` INLINES the default endpoint ctor: zero 7 dwords (0x1c union),
`family=2 (word), port=0 (word), addr=0 (dword)`, then push &temp; call bind. `m_server_endpoint =
udp::endpoint( address::from_string( host ), port )` = sret call to address::from_string (push host,
push &ret), out-of-line `detail::endpoint::endpoint(address const&, u16)` ctor into a temp, then
`rep movsd` 7 dwords into the member (trivial copy-assign). The huge /Od frame (sub esp,51Ch with a
~0x4b4 unused gap between named locals and the bottom spill temps) reproduces by itself - don't
chase it. Confirmed byte-perfect in `network_core/udp_match_client::connect` (100% first build).
### LOG_* sites: the pushed verbosity literal picks the macro; the pushed line literal pins the file layout
A `__LOG`-macro site pushes the verbosity TWICE (once into `has_passed_filters`, once into
`append`): `push 2` = `LOG_ERROR`, `push 3` = `LOG_WARNING`, `push 4` = `LOG_INFO`
(`vostok::logging::verbosity` in `logging/api.h`: silent=1,error=2,warning=3,info=4,debug=5,trace=6).
Do NOT pick the macro from the message tone - "disconnection initiated but new packet has been
enqueued" sounds like a warning but the target pushes 2 = LOG_ERROR (caught on
`udp_match_client::enqueue`, push 3 vs push 2). The site also pushes `__LINE__` as an immediate
(`push 0ACh` = line 172), so the LOG statement must sit on that PHYSICAL line of the `.cpp` -
pad/trim blank+comment lines above it to land it (markers above must stay single-line). The
remaining LOG residual after both match is the `log_callback_boost` function-ctor COMDAT call:
target may call it FIRST with this in EAX while /Od base calls it LAST (canonical right-to-left
arg order) with this in ESI, pushing its return - ICF/LTCG call-boundary convention, not steerable
(http_client precedent). NOTE network_core's `__FILE__` is the RELATIVE `.\udp_match_client.cpp`
and matches base, so the "__FILE__ never matches" cap does not apply to this module.

### inlined state getter: `cmp [m+off], K; sete` with NO temp slot = positive `return m_x == k;`
ASM (caller, /Od+/Ob2, getter inlined):
    mov eax,[this]; xor ecx,ecx
    cmp dword ptr [eax+11Ch], 0    ; K = the enum value the getter compares against
    sete cl; movzx edx,cl; test edx,edx; je <else>
SOURCE: `if ( m_connection.is_connected( ) )` where the getter is the POSITIVE inline one-liner
`return m_state == connected;`. The sete/movzx/test normalize chain (no [ebp-N] bool store) is the
inlined bool return. A NEGATED spelling (`if ( !has_disconnection_initiated() )` over
`return m_state != connected;`) would emit setne + inverted jcc - wrong bytes. K identifies which
getter: 0=connected -> is_connected, 3=disconnected -> is_disconnected
(udp_match_connection::state). Confirmed in udp_match_client::{enqueue,process_incoming_packet,
send_queued_packets} (all 94-100%).

### +0xC frame with byte-equal code = an ELIDED named-return temp; the spelling is proven at the emission that KEPT the copy
A target frame exactly +0xC over base with the instruction stream otherwise identical (only the
bottom-of-frame this/temp slot disp constants shift) is NOT a missing ASSERT (that adds ~0xc bytes
of CODE) - it is a dead 12-byte temp the target front-end materialized and the backend elided
code-free. Cause found for make_custom_alloc_handler: the original spells the helper with a NAMED
return value (`custom_alloc_handler< H > const result( a, h ); return result;`), not the direct
`return custom_alloc_handler< H >( a, h );`. The named local reserves its slot at EVERY inline
emission, but MSVC8 LTCG elides the 6-mov return copy per-emission: target kept the copy in
tcp_packet_client::start_reading (the tcp_packet_socket::start_receiving body inlined there -
100.00% only with the named-return spelling, the byte-proof) and elided it (slot kept) in the
standalone udp/tcp start_receiving COMDATs, where our LTCG does NOT elide (+0x12 bytes residual,
99.8% -> 87-91%). Same source, three emissions, two backend outcomes: when one emission
byte-proves a spelling, keep it and book the other emissions' copy as non-steerable backend
copy-prop variance. Net: +1 function at 100%, aggregate code% flat.

### Recover an INLINE ctor body from its consumer inline expansions (never guess NULL)
A header-inline ctor with no standalone symbol leaves its body unrecorded - but every /Od
consumer EXPANDS it, so the consumers are the ground truth. `packet_reader(base_packet const&)`
was reconstructed as `m_packet(packet), m_pointer(NULL)`; reading the expansions
(udp_match_connection::is_low_level_packet stmt 1; process_incoming_packet<..> L141) shows
`mov [reader+0], &packet;  lea/call <folded base_packet::buffer() const>;  mov [reader+4], eax`
=> the real init list is `m_packet(packet), m_pointer(packet.buffer())`. The folded `call` is
the tell: a NULL init would be a plain `mov [reader+4], 0`. Fixing the ctor moved
udp_match_client::process_incoming_packet 88.12 -> 99.86 alongside the template body. RULE:
before banking an inline ctor body, rich-view 1-2 consumers and read the expansion between the
member stores.

### interlocked_* on a member => the member is threading::atomic32_type, not long
`threading_functions_guard.h` defines template overloads `interlocked_*(T&, ...)` whose body is
`COMPILE_ASSERT(false, do_not_pass_NON_VOLATILE_values_to_INTERLOCKED_functions)`. For a
NON-volatile `long` member the template (exact match) beats the real
`interlocked_exchange(atomic32_type& = long volatile&, long)` (qualification conversion), so the
build breaks. Therefore any member a target function feeds to `interlocked_*` as the TARGET
operand was declared `threading::atomic32_type` in the original header, even when the PDB-dumped
structure shows plain `long` (the generator drops volatile). Caught on
`udp_match_connection::m_last_receive_time_in_ms` (process_incoming_packet does
`interlocked_exchange(m_last_receive_time_in_ms, m_last_send_attempt_time_in_ms)`). The VALUE
operand stays non-volatile.

### an /Od module unit can carry an OPTIMIZED LTCG COMDAT - correct source, unpairable bytes
A COMDAT instantiated from BOTH /Od and optimized TUs survives in the exe as whichever emission
the linker kept - sometimes the OPTIMIZED one (frameless, custom regs, unrolled), even though the
delinker files it under the /Od module's unit. Tells: no `push ebp` frame, `this` in eax,
flat unrolled stores / xmm pairs. Examples: `udp_match_stats::udp_match_stats()` (0x62 bytes, 32
flat dword zero-stores; `this` in eax), `udp_match_stats operator-` (xmm movq pairs, args
LTCG-promoted to edi/esi), `udp_match_packet::header_size` (8-byte `mov eax,[ecx]; sub; sub; ret`).
Your /Od body emits framed per-statement code and objdiff scores None or single digits NO MATTER
WHAT - write the source the gold LINE TABLE proves (the optimized emission still carries statement
lines in the PDB: e.g. operator-'s 10 statements at L212-224), mark PARTIAL citing the emission,
and do not chase the %.

### VOSTOK_UNREFERENCED_PARAMETERS (plural) EMITS code at /Od - use the singular form to stay row-free
The plural macro expands to `if ( vostok::identity(false) ) { detail::unreferenced_parameter_helper(...); } else (void)0`
- at /Od that is a REAL statement row (~0x25 bytes: identity call, test/branch, varargs call setup)
the target does not have. The singular `VOSTOK_UNREFERENCED_PARAMETER(x)` = `(void)(&x)` = zero code,
zero rows. Caught in `udp_network_flow_emulator::tick` (plural eater added a 16th base row and 0x25
bytes; two singular eaters restored 15/15).

### A fat early-`return` row = the inlined dtor walk of an in-scope local; a single `for` row = both iterator inits declared IN the for
Two `if (empty) return;` guards can look completely different in the carcass: before any non-trivial
local exists the `return` is a bare 5-byte `jmp epilogue`; after an alloca-backed `buffer_vector` is
live, the SAME `return;` row carries the container's inlined dtor (walk begin..end by elem size,
`end = begin`, then `jmp epilogue` - ~0x26 bytes). Don't misread the fat row as a missing statement.
Likewise one `for`-line row containing two slot inits + the loop control = a single-declaration
header `for ( pair* i = v.begin( ), * e = v.end( ); i != e; ++i )`; separate small `i =`/`e =` rows
before the for mean separate declarations. Both confirmed in `udp_network_flow_emulator::tick`
(structure went 14-vs-15 misaligned to 15/15, 48.44 -> 60.55).

### A header fix that "does not take" can be a STALE OTHER-MODULE COMDAT winning the link
After fixing `udp_match_stats.h`'s items operator>= the rebuilt network_core objs carried the new
bytes, but the linked base STILL showed the old compare: the surviving COMDAT emission came from
game_core's `temp_include_all.cpp` anchor, whose TU never recompiled (PCH staleness in the OTHER
module). If a one-line header fix provably does not move the diff, `touch` the pch.h of EVERY module
that instantiates the COMDAT (the anchor TU especially) and rebuild - the stream operator>= then
went 97.74 -> 100.00.

### Wrong access specifier = SILENT mangled-join failure - check access BEFORE banking 0%/unpaired
The base/target pairing joins on the EXACT mangled symbol, and the access specifier is baked into
the mangling (`QAE` public / `IAE` protected / `AAE` private; 2nd letter `B` = const `this`).
Declare a target-PRIVATE method under our `public:` and the names differ by ONE letter, the join
fails SILENTLY, and the function shows 0%/unpaired - even when the compiled bytes are IDENTICAL.
No error anywhere: not from the compiler, not from objdiff (it just never pairs them). RULE: before
banking any 0%/unpaired marker, read the target's mangled name (rich index `mangled` field) and
check the access letter AND every signature cv/byval const against our declaration. Confirmed on
`tcp_packet_socket` (PR #291): four methods declared public that the target mangles private sat at
0%/unpaired with byte-identical code; fixing the access flipped them to 85-100, with `new_packet`
landing 100 byte-perfect. Generalizes the older "private member function -> mangled `ABE`, not
`QBE`" entry above: 0%/unpaired + plausible source = ALWAYS suspect the mangled join (access
letter, const-ness, byval consts) before accepting the 0.
### A lone 2-byte TRGT_ONLY branch row after a guard = inverted-guard EARLY RETURN, not "codegen layout"
Under /Od every line-table statement row maps to a SOURCE statement - codegen cannot add or
drop rows. So a quantity diff whose extra TARGET row is a tiny (2-byte) bare branch with its
OWN line number, sitting between a guard's test and the guarded body, is a real `return;`
statement: the original spelled the guard INVERTED with an early return,
	if ( !cond )
		return;        // <- the 2-byte row: a standalone `jmp .end` attributed to this line
	body...;
which lowers as `jne .body; jmp .end`, while the condensed `if ( cond ) body;` folds into a
single `je .end` under the if's line record - one statement row fewer. DEDUCTION RULE: never
bank a 2-byte ONLY-target branch row as "if-branch codegen layout" - flip the guard to the
early-return form and re-diff; the same logic generalizes to any bare-branch ONLY row with its
own line (`return`/`break`/`continue`/`else` are all source statements). Confirmed: this exact
row was banked as a wall on `tcp_packet_client::~tcp_packet_client` (target 3 / base 2 stmts,
`<0x2> | L26 | ONLY target`), then PR #291's legacy adoption of `if ( !connected ) return;
disconnect( );` closed it - STRUCTURE MATCH 3/3, 90.98 -> 94.88; the same shape closed the
ONLY-target rows in `on_packet_received` (17+1 -> 18/18) and `on_packet_has_been_sent`
(9+1 -> 10/10).

### `else if` merges chain-jmp + test into ONE row - a flat second `if` splits them (ONLY row at the LOG line)
Sibling signature of the early-return entry above, seen when the FIRST arm ends in a
(possibly implicit) early exit. The /Od statement table attributes an `} else if ( C )`
spelling as ONE row (the else-jmp AND the `C` test bytes, on the `else if` line), and the
arm's body row then starts at the LOG. The original flat spelling
	if ( A ) { ...; return-ish; }
	if ( C ) {
		LOG_ERROR( ... );
gives `C` its OWN test row and the LOG its own full row - one statement MORE, with a telltale
size redistribution instead of a clean single diff:
	6: <0x5>  | <0xf>  | } else if ( bytes_transferred == 0 )   SIZE   (target = bare jmp; base = jmp+test)
	7: <0xa>  | <0x77> | LOG_ERROR( ... )                       SIZE   (target = test only; base = test+LOG merged)
	8: <0x7b> | --     | ONLY target (the LOG line)
DEDUCTION RULE: when a base mid-chain row is test-sized too SMALL on the target side, the next
base row is LOG-block-sized too BIG, and an ONLY-target row follows at the LOG's line - the
original used flat sequential `if`s, not an `else if` chain. Confirmed on
`tcp_packet_socket::on_packet_has_been_sent` (9+1 -> 10/10) and `on_packet_received`
(17+1 -> 18/18) via PR #291's flat early-return adoption.

### `dest = src & 0xffff` (u32->u16) emits `mov eax,src; and eax,0FFFFh; mov [dst],ax` (<0xe>), NOT a cast
A `u16 dest = (u16)u32_local` compiles to `mov cx,[local]; mov [dst],cx` (<0xa> bytes - a direct
16-bit load/store). The target instead masking through a full 32-bit register (`mov eax,[local];
and eax,0FFFFh; mov [dst],ax`, <0xe>) means the SOURCE wrote `dest = local & 0xffff` (explicit
mask), not a `(u16)` cast. (network_core_entry_point get_connection_info_from_string,
`dest_port = port & 0xffff;`. Recovered from pre-restack PR #268.)

### A ~0x26 `return;` row = the by-value boost iterator param destroyed AT the return (sp release + jmp)
A `basic_resolver_iterator` (or any shared_ptr-holding object) passed BY VALUE is callee-destroyed,
and under /Od an early `return;` gets its OWN row carrying that dtor inlined: `lea ecx,[ebp+14h]
(the param's shared_count slot); cmp [ecx],0; je .skip; call sp_counted_base::release; jmp .epilog`
- ~0x26 bytes attributed to the return's line. Two identical ~0x26 rows in one function = two
early `return;` statements, NOT codegen layout. Sibling of the "fat early-return = inlined dtor
walk" entry. Confirmed in `async_connector::on_resolved` (both error-arm returns).

### A fat mid-arm row calling the function's OWN bind + async op = a RETRY statement, not the success path
`on_resolved`'s error/not-exhausted arm was guessed as `m_host = iterator; connect( m_host );` -
the target bytes instead deref `*iterator` into a 0x1c endpoint temp (rep movsd 7 dwords from
values[index]*0x4C), build `boost::bind( &async_connector::on_resolved, this, resolver, _1, _2 )`,
and call `resolver_service::async_resolve` - i.e. the source RE-ISSUES the async op on the next
endpoint: `++iterator;` (its own 0x8 row - an increment() call) then
`if ( iterator != tcp::resolver::iterator( ) ) { resolver->async_resolve( *iterator, bind(...) );
return; }`. DEDUCTION RULE: when an arm's big row ends in a call whose HANDLER template type names
the enclosing function, the original re-arms the same async call - read the bind_t in the called
symbol's template args before guessing the arm. (63.37 -> 93.25, 22 -> 20 stmts.)

### MSVC /Od drops the case-compare for a case adjacent to `default:` - the target's redundant cmp is non-steerable
`switch` dispatch with `case X:` and `default:` labeling the SAME arm: our compiler omits X's
`cmp/je` (anything unmatched falls to default = the same block), emitting N-1 compares; the target
emits all N including the redundant one. Flipping the label order (`default: case X:` vs
`case X: default:`) changes NOTHING - both spellings dedup. Bank the few-byte SIZE row on the
switch line; do not chase it. (udp_match_connection::process_low_level_message, +0x3 on the
dispatch row.)
### a member `tick`/update whose body is byte-identical but with `sub esp` off by 4 = one extra unused /Od frame slot (DONE-quality PARTIAL)
When the base disasm matches the target instruction-for-instruction, member-offset-for-offset,
call-for-call, constant-for-constant, but the TARGET reserved 4 more stack bytes
(`sub esp,1Ch` vs base `sub esp,18h`) so the saved-`this` slot and every `[ebp-N]` shifts by 4
(target this@[ebp-10h] vs base this@[ebp-0Ch]), that is a single UNUSED /Od frame slot the
target build allocated - pure stack-allocation noise, NOT a missing local/brace/ASSERT/statement.
The PDB local set maps, the carcass statement structure matches 1:1. Non-steerable under /Od.
Mark PARTIAL at the resulting % (here 99.67%), not chase. Same class as
`breath_vibration_calculator::tick` (94%, frame-slot churn). Confirmed in
`game_core/character_dispersion_calculator::tick` (16 stmts, only diff = sub esp 1Ch vs 18h).

### two trivial accessors in ONE `&&` can split inline-vs-call (one matches, one is the residual)
A `if ( a() && getter_x() && getter_y() )` where BOTH `getter_x`/`getter_y` are one-line
header accessors (`{ return m_member; }`) can lower DIFFERENTLY under /GL: the linker keeps
ONE of them out-of-line (the TARGET emits `call ...getter_x`, standalone symbol present in the
target rich index) while INLINING the other (`mov al,[this+off]`, no standalone in either index).
Our /GL LTCG inlines BOTH, so the inlined-on-both-sides one matches byte-for-byte and the
target-kept-standalone one is the lone residual (`call` vs inlined member read), shifting the
trailing `[ebp-N]` slot/frame by the one inline. This is the same unsteerable trivial-accessor
inline-vs-call class as is_aimed()/get_user() - it is a per-method whole-program decision, NOT
steerable from the caller's source. Confirm which is the residual by querying BOTH rich indexes
per accessor (target standalone + base absent = the diverging one). Mark the caller PARTIAL at the
resulting %. Confirmed on `game_core/weapon_core_reload_state_base::initialize` (92%): in
`!deserializing() && chamber_a_round_on_reload() && round_is_chambered()`, chamber_a_round_on_reload
(@0x48F) inlined on both sides (matches), round_is_chambered (standalone target @0x09b360, base
inlines `mov cl,[+48Eh]`) is the residual.

### empty base virtual called via qualified `Base::method()` - LTCG inlines the empty body at the call site (PARTIAL)
SYMPTOM: a derived override calls the EMPTY base implementation, `Base::execute();` (base decl is
`virtual void execute() override { /* no source */ }`). TARGET emits `mov ecx,[ebp-4]; call
Base::execute` (the empty body kept out-of-line @ its rva). BASE (/GL LTCG) INLINES the empty `{}` at
this call site - the `call` simply vanishes, so the diff shows `+ call <addr>` present only on the
target side and the following member store with a different scratch reg. Both rich indexes STILL list
a standalone `Base::execute` (base @0x012c20, target @0x087f80), so it is NOT "inlined everywhere in
base" - it is the documented PER-CALL-SITE whole-program inline decision (same class as
animation_playback_state::reset() in weapon_core_aimed_state_base::finalize). Filling the empty body
would make /Od inline REAL bytes (worse); the empty stub elides one no-op call cleanly. Leave it,
mark PARTIAL [LTCG empty-callee inline-vs-call]. Confirmed on
`game_core/weapon_core_fire_state_base::execute` (80.91%): only the `call execute` (3 bytes) differs.

### a `boost::bind(&Derived::virtual_method, this, _1)` ICF-folds onto a SIBLING class's bind<> rep - don't be misled by the delinker name
SYMPTOM: `set_animation_callback("ch", this, boost::bind(&weapon_core_fire_state_base::on_shot_event,
this, _1))` - the `call boost::bind<...>` at the bind site is delinker-named with a DIFFERENT class
(`...weapon_core_animation_end_aware_state...`) than the actual bound method. The `boost::bind<>`
helper packs only {member-fn-ptr, this, arg} and is byte-identical across sibling state classes, so
/OPT:ICF folds them and the delinker prints whichever fold representative it picked. The TRUE class
shows on the un-folded `assign_to<bind_t<...weapon_core_fire_state_base...>>` and the
`Derived::vcall'{36}'` member-pointer (a vcall thunk because on_shot_event is VIRTUAL). Source is
`&weapon_core_fire_state_base::on_shot_event` - the mismatched bind<> name is an ICF artifact, not a
wrong source type. Confirmed in `game_core/weapon_core_fire_state_base::initialize` (99.71%).

### Trivial header getter WITHOUT the `inline` keyword -> standalone COMDAT + a `call` at the use site (our /GL inlines it)
SYMPTOM: a `return X && obj.trivial_getter();` (or `+ getter()`) function diffs ~85-90%: the target
emits `call survarium::...::getter` and round-trips the object through 2-3 `[ebp-XX]` ref copies,
while our base reads the member directly (`mov al,[ecx+NNh]`). The getter (e.g. weapon_core::
is_double_handed @+48A, weapon_core_base_state::has_animation_ended @+135, player_input::is_sprinting,
weapon_user_animations_selector::is_ready_to_be_deactivated, round_is_chambered) is defined in the
header but declared WITHOUT the `inline` keyword (siblings on the same lines that DO have `inline`
get inlined in BOTH builds). At /Od the target compiles each TU separately -> the getter is a COMDAT
standalone AND every caller emits a `call`; our whole-program /GL build inlines the trivial body in
the delinked EXE. This is a genuine LTCG inline-vs-call residual (same class as
reload_state_base::initialize round_is_chambered, fire_state_base::execute). NOT source-steerable
short of moving the getter out-of-line, which changes its COMDAT placement and risks other matches -
bank it PARTIAL [LTCG getter inline-vs-call]. Confirmed across weapon_core batch3:
is_trying_to_aim 66.75, on_user_sprint 89.72, the has_animation_ended predicates 85-87,
is_ready_to_be_deactivated 84.77.
### a no-bounds `default: NODEFAULT()` jump-table switch can be a TRUE 100% match while `--view diff` FOOTER reads ~55-65% - cross-check report.json
SYMPTOM: a switch compiled to `jmp dword ptr [reg*4+table]` with NO bounds check (source ends
`default: NODEFAULT();`, full contiguous case range). The function code matches byte-for-byte and
`--view structure` is identical to the target carcass, yet the `pdb_fetch --view diff` footer reads
~55-65%. The footer's "mismatches" are BOTH artifacts (NOT code diffs):
  (1) every leaf `jmp .N`(base) vs `jmp .N+1`(target) - the label NUMBER differs but BOTH resolve to
      the SAME address (verify in `--view base`/`--view target`: same epilogue offset). The index
      shift is because the embedded jump table is a distinct symbol whose label objdiff counts.
  (2) a tail of `(bad)` / nonsense instructions PAST the function `ret` - that is the jump TABLE
      itself, disassembled as code. base and target tables hold binary-specific RVAs (relocations),
      so they disassemble into different junk and the footer scores every table dword as a mismatch.
The AUTHORITATIVE objdiff measure (`report.json` units[].functions[].fuzzy_match_percent - the
TOP-LEVEL field, NOT `.measures.fuzzy_match_percent`) handles the relocation correctly and reports
the TRUE % - which for both functions below was 100.0%. So the diff FOOTER is a secondary/diagnostic
number that UNDER-counts inline jump-tables; ALWAYS reconcile it with report.json before banking a
non-100%. Do NOT chase the footer with result-temps or trailing returns - a trailing `return X;`
after the switch re-introduces a `cmp/ja` bounds check + dead store (WORSE code) while only the rich
footer misleadingly rises. Keep `default: NODEFAULT();`. Confirmed: TRUE 100%
`game_core/jump_logic::does_need_land_and_run` (footer 63.7%) and
`game_core/get_jump_animation_index` (footer 55.0%).

## mixing operator+ template selection (animation::mixing::expression `+` chains)

`weapon_and_hands_expression` (every weapon-state variant) returns a `+` chain of mixing
lexemes/expressions (e.g. `hands_expression + main_lexeme + offset_lexeme`). These cap at
~83-85% and the residual is a TEMPLATE-SELECTION divergence on the `operator+` overloads.

The target picks `operator+`s that return `expression` BY VALUE and call `expression::is_empty()`:
- `operator+<animation_lexeme>(expression& left, animation_lexeme& right) -> expression`
  (ONE explicit template arg; the FIXED `expression&` left operand means the real engine header
  had a `template<typename T> operator+(expression& left, T& right) -> expression` overload).
- `operator+(expression&, expression&) -> expression` and const variants (non-template).

The on-disk `vostok/animation/mixing_addition_lexeme_inline.h` provides ONLY
`template<T1,T2> operator+(T1&, T2&) -> addition_lexeme&`, and `mixing_expression.h`'s
`is_empty()` is a `return false;` STUB. So the base falls back to the addition_lexeme& form plus
extra `expression()` conversions, and NO reshaping of the return expression can select the
target's overloads (verified: wrapping operands in `expression(...)` only makes it WORSE -
83.52 -> 77.48 / 56.65 - because the desired overloads do not exist to be chosen).

CONCLUSION / pattern: when a `+` chain over `animation::mixing::expression`/lexemes diverges on
operator+ template instantiation, it is a CROSS-UNIT HEADER GAP (the missing mixing operator+
overload family + the stubbed `expression::is_empty`), NOT a source shape. Do NOT chase it with
parenthesization/`expression()` wraps in the consuming function - mark PARTIAL. The real fix is
to match the mixing operator+ overload set + `expression::is_empty` as their own unit; once those
overloads exist, the natural `a + b + c` source should select them and these functions lift
together. (Found re-matching game_core/weapon_core_reload_state::weapon_and_hands_expression.)

## Ternary returned via a named result local (extra slot + movss/movss/fld)

When the target's `float`-returning function ends with a ternary but the epilogue is
`movss xmm0,[result_slot]; movss [ebp-N],xmm0; fld dword ptr [ebp-N]` (an EXTRA stack slot
and an SSE round-trip) instead of a plain `fld [result_slot]`, the source assigned the
ternary to a NAMED local and returned THAT local:

    float const r = cond ? a : b;
    return r;          // -> movss/movss/fld through r's own slot

A bare `return cond ? a : b;` emits only the plain `fld` (no extra slot). The named local is
recorded as its own statement in the PDB, so prefer it whenever the carcass shows the extra
slot. (get_hand_coefficient: 95.54 -> 99.90 once the result local was added.) const vs
non-const on the local does NOT change the slot assignment.

## Hoisting a temporary's `.c.xyz()` is a float3 REFERENCE, not a full-matrix local

When the target keeps `get_X().c.xyz()` (or any `member.subobject()`) of a returned temporary
as its OWN srcline statement (carcass shows a `+small` step feeding the next), the materialized
local is the `.c.xyz()` RESULT bound by reference - NOT the whole returned struct:

    float3 const& p = get_bone_matrix_in_object_space( ... ).c.xyz( );   // <-- correct
    // NOT: float4x4 m = get_bone_matrix_in_object_space( ... );  float3 p = m.c.xyz();

The asm tell: after the call returns &temp in eax, the target does `add eax, <offset-to-member>`
then the member-accessor ASSERT call, then `mov [slot], eax` - it stores a POINTER to the
sub-object, never copies the full struct to the stack. A full-struct hoist allocates an extra
NRV stack slot the target never uses and REGRESSES the match. (process_hand: full-float4x4 hoist
89.69 -> 89.54; the .c.xyz() reference hoist 89.54 -> 90.37, restoring 37==37 statements.)
## bool-added-to-int with neg;sbb;neg = source wrote `(b != 0)`, not a bare bool
When the target adds a `bool` member to an integer and emits `neg eax; sbb eax,eax; neg eax`
(the `(x != 0)` normalize-to-0/1 idiom) BEFORE the `add`, the source did NOT write the bare
`int + boolmember` (that compiles to a direct add of the already-0/1 bool, no normalize). Write
the comparison explicitly: `int_expr + ( boolmember != 0 )`. (Found in weapon_core::reset_fire_queue
else-branch `m_ammo_in_magazine + ( m_is_round_chambered != 0 )`: bare `+ m_is_round_chambered` gave
a direct add @94%, `+ ( ... != 0 )` reproduced the neg;sbb;neg -> 99.65%.) Note the SAME bool in the
sibling `if (m_is_round_chambered) ++...` branch reads direct (no normalize) - the normalize is
specific to the arithmetic-add context.

## header-inline accessor the target keeps as a `call` -> move the body to the .cpp
If a trivial member accessor is `inline { return m_x; }` in the header but the target has a real
standalone symbol for it AND a caller emits `call accessor` (not the inlined member read), move the
definition out of the header into the .cpp (leave a forward decl in the header). The out-of-line .cpp
definition makes /GL stop inlining it at the call site. (weapon_core::fire_queue_length: inline-in-header
made reset_fire_queue inline `m_weapon_fire_queue_types[m_fire_queue_type]` @66%; out-lining it produced
the two `call fire_queue_length` the target has. Same pattern as the pre-existing ammo_in_magazine /
get_magazine_capacity NOTEs.) Confirm first the accessor is a real target symbol (pdb_rich_query --list).
### `(T)config_value` -> `binary_config_value::operator T` standalone, base inlines it to cast_number
SYMPTOM: a `(u8)cfg["x"]["y"]` (any `(T)binary_config_value`) where the TARGET emits
`call binary_config_value::operator unsigned char` then `movzx eax,al`, but the BASE
instead emits `mov [slot],eax; call cast_number<unsigned char,unsigned __int64,unsigned int>;
mov [slot],al; movzx ...`. The conversion chain is `operator u8() -> cast_unsigned_number<u8>()
-> cast_number<u8,u64,u32>()` (configs_binary_config_value_inline.h). Whole-program LTCG picks
a DIFFERENT cut point in each binary: target keeps the OUTER `operator unsigned char` out-of-line
(rva 0x52160) and inlines cast_number INTO it; base inlines the operator away and keeps the INNER
`cast_number<u8,...>` standalone (base rva 0x79dd0). TELL: query both rich indexes - the operator
exists out-of-line ONLY in target, cast_number<u8> ONLY in base. The `(T)cfg[...]` source is
already correct; not steerable from the call site. Same inline-vs-call class as is_aimed()/get_user().
Confirmed in `game_core/player_parameters_modifyer_cook::translate_query` (89.62% PARTIAL).

### `delete_resource` body `VOSTOK_DELETE_IMPL(g_allocator, resource)` - the strip_pointer inline-vs-call wall (~31%)
SYMPTOM: a one-line cook `delete_resource` whose only statement is `VOSTOK_DELETE_IMPL(g_allocator,
resource)`. TARGET: `lea eax,[ebp+8]; push eax; mov eax,[g_allocator]; call <finalize_impl-misname>;
push eax; call delete_helper; add esp,8` (TWO pushed args, strip_pointer kept out-of-line). BASE:
`mov eax,[g_allocator]; call <Release-misname>; push eax; lea edi,[ebp+8]; call delete_helper;
add esp,4` (ONE arg, strip_pointer inlined). The macro is `delete_helper(strip_pointer(allocator),
pointer)` (Master Gold no-debug form, memory_macros.h:41); the divergence is purely whole-program
inline-vs-call of `strip_pointer`/the delete_helper wrapper. Structure is 1 stmt / 1 stmt (matches);
fuzzy stalls ~31% because nearly every instruction's operand differs. Non-steerable - identical residual
across cooks: `items_cook::delete_resource` (31% DONE) and `player_parameters_modifyer_cook::delete_resource`
(31% PARTIAL) have byte-identical diffs. Mark PARTIAL/DONE, do not chase.

### Callee returns a small struct in register-pair eax:edx + caller COPY (vs base sret/RVO) -> non-steerable wall
SOURCE: `T r = some_factory( args );` where `T` is a small (<=8 byte) POD-ish struct
(e.g. `resources::request`, 8 bytes). Seen in `game_material_manager_cook::create_game_material_pairs`
at every `resources::request r = resources::create_request( name, class );` call.

    base   : push 0Fh ; lea eax,[esp+10Ch] ; call create_request    ; sret/RVO - writes directly into r's slot
    target : push 0Fh ; mov edx,[..] ; push edx ; call create_request ; add esp,8 ;
             mov [tmp],eax ; mov [tmp+4],edx ; mov [r],eax ; mov [r+4],ecx  ; returns in eax:edx, then COPIES into r

The target returns the struct in the eax:edx register pair and then copies the pair
into the local; the base lowers the same source as a hidden-sret (return-value-
optimization) call writing directly into `r`. This is a CALL-BOUNDARY return-ABI
difference decided by how the callee/`request` type's copy+return convention was
compiled under LTCG - it is NOT steerable from the calling function. It cascades:
every later `[esp+N]` offset shifts because the extra temp/copy enlarges the frame,
so a SINGLE root cause shows up as many SIZE/quantity rows downstream. Diagnose once
(one create_request site), then attribute the whole cascade to it. Mark PARTIAL;
do not chase the downstream rows individually.

### the ASSERT empty-stub call's COMDAT-fold MISNAME is scored MATCHED by report.json (don't chase it)
SYMPTOM: `--view diff` flags the compiled-out `ASSERT` call's target as a mismatch -
base resolves it to `boost::function1<void,char const*>::dummy::nonnull`, target to
`vostok::memory::fixed_size_allocator<...>::finalize_impl` (or other unrelated COMDAT-
fold names). It looks like a divergence on EVERY function in a unit that has an ASSERT.
TELL it is harmless: the same misname appears identically in the unit's 100% functions,
and report.json's `fuzzy_match_percent` scores it as MATCHED (the reloc pairs at the
fold representative). So a function whose ONLY `--view diff` flag is this ASSERT-call
misname is 100% in report.json - do NOT bank it as a sub-100% wall. Confirmed across
`game_core/collision_geometry.cpp` (get_overlapping_objects et al. - all 100% despite
the misname). CAVEAT: read the TRUE % from report.json (`fuzzy_match_percent`), not from
the `; N/M instructions equal` line `--view diff` prints (that operand-aware line counts
the misname as unequal and reads ~85-88% while report.json reads 100%). STATE markers
that quote the `--view diff` line are stale; re-confirm against report.json after rebuild.
### By-value `boost::function` argument copy: target copy-ctor (1 call) vs base default-ctor + `assign_to_own` (2 calls) -> non-steerable wall
SOURCE: a call passing a `boost::function<...>` BY VALUE, e.g.
`body_part->dump_state( callback, index++ )` where `dump_state` takes
`boost::function<...> callback` by value (`damage_model::dump_stats`, 79.26%). The
call constructs a temporary copy of the source `boost::function`.

    target : sub esp,20h ; mov eax,esp ; lea ecx,[ebp+8] ; call function<>::function<>  (COPY CTOR, one call)
             mov ecx,[this] ; call dump_state
    base   : sub esp,20h ; mov [ebp-10h],esp ; mov eax,[ebp-10h] ; call function<>::function<> (default ctor)
             lea ecx,[ebp+8] ; push ecx ; mov ecx,[ebp-10h] ; call assign_to_own  (EXTRA call)
             mov ecx,[this] ; call dump_state

The base default-constructs the temp then calls `assign_to_own`; the target copy-
constructs in one call. The extra call + extra `[ebp-10h]` slot enlarge the frame
(base `sub esp,20h` vs target `sub esp,14h`) and shift slot numbers, dropping the
byte %. Statement count matches (7/7); the lone SIZE row is this call. The choice
lives inside boost::function's own header inlining (copy-ctor vs default+assign);
the source already passes by value, the only available shape. Non-steerable boost::
function LTCG inline-vs-call. Mark PARTIAL; do not chase.

### Single-`this`-spill prologue: target `push ecx` (4-byte frame) vs base `sub esp,0Ch` -> non-steerable
SOURCE: a small method with NO declared locals, only `this` spilled to a stack slot
(`damage_model::on_broken_limb_affect`, 98.95%). Identical instruction stream apart
from the prologue: target `push ecx` (this at `[ebp-4]`, 4-byte frame) vs base
`sub esp,0Ch` (this at `[ebp-0Ch]`, 12-byte frame) + trailing alignment nops. MSVC's
single-slot `push reg` frame vs `sub esp,N` is a prologue/frame-allocation quirk not
expressible in C++ source. Non-steerable; mark DONE/PARTIAL with the residual noted.
### boost::bind argument: by-VALUE vs boost::cref (reference_wrapper) - SOURCE-STEERABLE
SYMPTOM: a `boost::bind( &f, this, a, b, ... )` where some args are large value types (float3,
resource_ptr). BASE pushes each arg by VALUE: inline struct-copy onto the stack (`sub esp,0Ch;
mov eax,esp; mov ecx,[src]; mov [eax],ecx; ...`) OR an out-of-line `call boost::_bi::value<T>::value`
per arg, then `call boost::bind<...,T,T,...>` (the bind type's `listN<...,value<float3>,value<float3>>`).
TARGET instead has a `call boost::addressof<...>` (== `boost::cref`) per arg and binds
`listN<...,reference_wrapper<float3 const>,reference_wrapper<float3 const>>`; the
`boost::function::assign_to<bind_t<...,list5<...,reference_wrapper<float3 const>,...>>>` symbol
names the wrappers.
FIX (steerable!): wrap each such bound arg in `boost::cref( functor->member )`. Read the TARGET
bind's `listN<...>` type to decide WHICH args are wrappers vs values - scalars often stay by value
(`value<unsigned short>`, `value<float>`) while float3/resource_ptr are `reference_wrapper<.. const>`.
This is NOT an LTCG wall; the original source spelled `boost::cref(...)`. Confirmed: bullet_manager
`play_particle` (67.87->98.81), `update_tracer` (58.56->89.38) - cref on the float3/resource args
collapsed the whole bind region to `.. same ..`.

### `bullet_functor_mt_allocator::malloc_impl` / `delete_helper<>` wrapper inline-vs-call (NON-steerable)
SYMPTOM: a `VOSTOK_NEW_IMPL( allocator, T )` / `VOSTOK_DELETE_IMPL( allocator, p )` whose alloc/free
SIZE-diffs by a few bytes and bumps the frame by 4. The new path: TARGET `call <allocator>::malloc_impl`
(or `call delete_helper<alloc,T>` wrapper), BASE inlines the wrapper to the leaf (`call try_pop` for
malloc; `call delete_helper_impl<...,call_destructor_predicate>` for delete, with the predicate bool
materialized at the call site). TELL: `pdb_rich_query target --function malloc_impl|delete_helper` finds
the wrapper at a real rva; the SAME query on the base index returns nothing (wrapper inlined away
whole-program). The base's extra `push 58h` (size) / extra predicate-bool push and the +4 frame all
cascade from the one inline. Same whole-program LTCG inline-vs-call class as vectora::size()/operator|.
Source (`VOSTOK_NEW_IMPL`/`VOSTOK_DELETE_IMPL`) correct; mark PARTIAL. Confirmed: bullet_manager
`add_decal` (malloc_impl @target 0xae5c0), `free_bullet` (delete_helper wrapper @target 0xae9f0,
delete_helper_impl @0xaed80).

### `a != b` for float3 folds to `!( operator==(a,b) )` when only operator== exists (NON-steerable)
SYMPTOM: `x != float3(...)` shows TARGET `call vostok::math::operator==` then a `je`/`jne` branch
polarity OPPOSITE to BASE, while BASE does `call vostok::math::operator!=` directly (no negate; plus a
`mov ecx,eax` to pass the temp's this). The target binary has NO float3 `operator!=` symbol (only
`operator==` @0x14b90); `operator!=` is `inline { return !(a==b); }` and is inlined to `call operator==`
+ negation. BASE keeps a standalone float3 `operator!=` (@0x6f250 math_float3_inline.h) and calls it.
Whole-program LTCG inline-vs-call (the `!=` wrapper folded to `!(==)` at the call site). Source `a != b`
correct; mark PARTIAL. Confirmed: `redundant_bullet_predicate::operator()` (87.98%).

### `static_cast_resource_ptr< P >( x.get_unmanaged_resource() )` extra copy-construct + 8B frame (NON-steerable)
SYMPTOM: a `P p = static_cast_resource_ptr< P >( data[i].get_unmanaged_resource() )` line SIZE-diffs and
the function frame is 8 bytes LARGER in BASE (`sub esp, 0BCh` vs target `0B4h`). TARGET builds the
`get_unmanaged_resource()` result as a direct stack prvalue passed into `static_cast_resource_ptr`
(`push ecx; mov esi, esp; ...; call get_unmanaged_resource; lea esi,[ebp-Ch]; call`), so ONE temp slot.
BASE materializes the result into a named slot `[ebp-0Ch]` and then emits an EXTRA `resource_ptr`
copy-construct call into a SECOND slot `[ebp-10h]` - the `+ call` and the recurring +8B frame.
`static_cast_resource_ptr` takes its arg BY VALUE (`const src_ptr`, resources_resource_ptr_inline.h:55);
whether the by-value param is constructed in place (target) or via an extra copy (base) is a header
template inline/RVO decision under whole-program LTCG, NOT steerable from the consuming `.cpp` - the source
is already the maximally direct nested-call form. Same inline-vs-temp class as the resource_ptr/create_request
ABI walls. Mark DONE (structure matches). Confirmed: booby_trap_set_core_cook `on_subresources_loaded`
(89.02%), `on_config_ready` (82.01%).
### Top-level `const` on a by-value pointer parameter GATES objdiff symbol pairing (source-steerable)
SYMPTOM: a function reads `?` / unpaired in report.json and `--view structure-diff` says
"not found in BASE index" even though the body is obviously identical to a sibling that DOES match.
CAUSE: the source declares the parameter `T* param` but the TARGET mangles it `T* const param`
(or vice versa). Top-level `const` on a BY-VALUE parameter is dropped for C++ overload resolution,
but MSVC still encodes it in the DECORATED (mangled) name, and objdiff pairs base<->target by mangled
symbol - so the const mismatch makes the two unpairable and the function scores 0/unpaired regardless
of byte identity. FIX: match the target's const exactly on the parameter; rebuild -> pairs and (if the
body matched) jumps straight to 100%. TELL: a sibling overload/functor in the same file already uses
`T* const` and matches. Confirmed: `protect_affect_predicate::operator()(damage_protector* const)`
in body_part_parameters.cpp (0.00 -> 100.00 by adding `const`; its twin
`protect_damage_predicate::operator()(damage_protector* const)` was already `* const`).

### `math::pow(x, INT_literal)` inlines pow(float,int) in target, out-of-line call in base
SYMPTOM: `math::pow( <float>, 5 )` (an INTEGER second arg -> binds `pow(float,int)`,
not `pow(float,float)`) emits in TARGET an inlined sign-dispatch block
(`mov edx,5; test edx; jne; ...; call pow_impl`) while BASE emits `mov ecx,5; call
vostok::math::pow`. Same per-call-site inline-vs-call LTCG class as operator| /
is_aimed / fixed_string ctor. TELL: query both rich indexes - `pow_impl(float,uint)`
is out-of-line in BOTH, but `pow(float,int)` is out-of-line ONLY in base (target inlined
it whole-program). Disassemble base `pow(float,int)`: its body
(`test ecx; jne; movss[1.0]; ret / jge; divss; neg; jmp pow_impl / jmp pow_impl`) IS the
target's inlined block. The inline also reorders operand evaluation (the int-pow's operand
gets computed first) and shifts surrounding x87-vs-xmm result codegen. Source
`math::pow(x, 5)` is already correct; mark PARTIAL. (Watch the overload: `pow(x,5)` with
`5` an int binds `pow(float,int)`; `pow(x,5.0f)` would bind `pow(float,float)` - different
function.) Confirmed in `game_core/pseudo_random::random_f` (target rva 0x57e420, 60.74%).

### ternary precedence trap: `a * b * c ? x : y` makes `(a*b*c)` the condition, not `c`
SYMPTOM: source `mult * koef * first_shoot ? side_a : side_b` where `first_shoot` is a
bool flag. `*` binds tighter than `?:`, so this is `(mult*koef*first_shoot) ? side_a :
side_b` - the whole float product is the ternary condition. TARGET instead tests the bool
ALONE: `movzx ecx, byte[first_shoot]; test ecx,ecx; je .else` then multiplies the selected
value. So the original source parenthesized the ternary:
`mult * koef * ( first_shoot ? side_a : side_b )`. This is a SOURCE bug in the
reconstruction, fully steerable - add the parens. Confirmed in
`game_core/weapon_recoil_calculator::fire` (79.80% -> 91.69% by parenthesizing both the
recoil and recoil_amount ternaries).

### private LTCG-inlined helper has NO base symbol -> objdiff None / not in base index = STATE[INLINED]
SYMPTOM: a private `AAE` member helper (e.g. `process_compensation`, `get_random_angle`,
`get_random_amount`) is "not found in BASE index" by structure-diff and shows None in
report.json, even though it exists out-of-line in TARGET. CAUSE: under /Od+/GL the linker
inlined the private helper whole-program into its only callers (here tick/fire), so no
standalone base symbol survives to pair. Not separately scorable; mark STATE[INLINED] and
match its body as a callee of the caller. (Distinct from the constant-symbol-naming None
below.) Confirmed in `game_core/weapon_recoil_calculator`.

### objdiff None from differing delinker constant-pool symbol names (body byte-identical)
SYMPTOM: a function whose body is byte-for-byte identical to the target (structure-diff
7/7 aligned, 0 size/quantity-diffs) still scores None. CAUSE: each `movss xmm0,[0.0f]`
reloc resolves to a DIFFERENT delinker-assigned symbol for the same 0.0f constant pool
slot - base `out_of_range_reward` vs target `offset` - so objdiff's relocation compare
cannot pair the instructions and bails to None. Non-steerable (the source `= 0.0f` is
correct; it is a delinker naming artifact, same misname class as empty_stub/finalize_impl).
Confirmed in `game_core/weapon_recoil_calculator::reset` (7 stores to 0.0f, all
out_of_range_reward vs offset).
### Array brace-init: per-element source lines -> per-element statement attribution (structure-only)
SYMPTOM: a `--view structure-diff` SIZE diff on a `pcstr captions[N] = { "a", "b", ... };` line,
with the target showing N separate `'srcline'` statements (one per element, each size 0x7 on x86)
where the base shows ONE combined statement (size N*0x7). The BYTES are identical
(`mov dword ptr [ebp-X], <str0>; mov dword ptr [ebp-Y], <str1>; ...`); only the statement
ATTRIBUTION differs, so report.json fuzzy is unchanged but the structure-diff flags it.
CAUSE: under /Od, MSVC attributes each array-initializer element-store to the SOURCE LINE that
element literal sits on. A single-line brace-init `= { "a", "b" }` collapses all stores onto one
line -> one statement; the original wrote each element on its OWN physical line.
FIX: split the brace-init across lines, one literal per line:
    pcstr captions[2] =
    {
        "a",
        "b"
    };
This re-attributes each store to its own line -> per-element statements aligning the target.
The blank-line gaps between elements show up as harmless `EMPTY only base` quantity-diffs, NOT
real divergences. Confirmed across all 7 `get_weapon_lexeme_pair` variants in the pistol/
double_barreled weapon_core idle/aimed_idle/show states (SIZE-diff 1 -> 0; report.json unchanged
at 99.92%, sole residual the ammo_in_magazine arg-passing register).

### `bool const` local IS codegen-visible: drops xor-before-sete / flips && temps to byte
SYMPTOM (a): `bool x = a == K;` emits `xor ecx,ecx; cmp eax,K; sete cl; mov [x],cl` in
base while target has NO xor (`cmp; sete cl; mov`). SYMPTOM (b): `bool x = a && !b;`
materializes the &&-result through a DWORD temp (`mov dword ptr [ebp-N],1/0`) in base
while target stores a BYTE temp (`mov byte ptr [ebp-N],1/0`).
CAUSE: the local's CONST-ness. `bool const x = ...` makes MSVC8 /Od emit the bare sete
(no register pre-zeroing) and byte-sized &&-temps; plain `bool x` zeroes the full register
and uses dword temps. The target PDB locals record (`; locals` in --view structure) shows
`const bool` - restore the const and the shape follows. NOT codegen-invisible, despite the
general rule that const on locals usually is.
Confirmed: weapon_core::on_hand_ik_event 96.55%->100% (xor) and
weapon_core::maximum_ammo_in_weapon 88.41%->100% (dword->byte temp; together with the
`!= 0` addend below). Check `const u8`/`const bool` locals FIRST when a sete/temp-width
diff shows up.

### `+ ( b != 0 )` vs `+ b` for a bool addend: neg;sbb;neg normalize
SYMPTOM: target adds a bool to an integer via `movzx ecx,byte [b]; neg ecx; sbb ecx,ecx;
neg ecx; add eax,ecx`; base emits plain `movzx ecx,[b]; add eax,ecx`.
SOURCE: target spelled the addend as a comparison - `m_magazine_capacity +
( flag != 0 )` - the int!=0 conversion produces the neg;sbb;neg (val!=0) idiom; a plain
bool->int promotion is a bare movzx. (weapon_core::maximum_ammo_in_weapon,
weapon_core::reset_fire_queue.)

### Named float-const global vs literal at a `float const&` parameter
SYMPTOM: target passes `push <global>` (the address of a named float constant, e.g.
`epsilon_3`) where base emits `movss xmm0,[const]; movss [ebp-N],xmm0; lea ecx,[ebp-N];
push ecx` (materialize a temp, pass its address).
SOURCE: the original named the GLOBAL (`bullet_direction * math::epsilon_3`); a literal
`0.001f` binds the const& through a fresh temp. Restoring the named constant removes the
temp (bullet::collide_front_face 95.13%->97.40% together with the min-order fix below).

### math::min/max promoted-convention reg order reveals SOURCE argument order
The LTCG-promoted float `vostok::math::min` in BOTH builds takes xmm0=SECOND arg,
xmm1=FIRST arg. So at a call site, if base loads `movss xmm1,xmm0` (shuffling the
previous call's result out of xmm0) while target consumes it in place, the source arg
ORDER is swapped relative to the original. bullet::tick wanted
`min( m_life_time, g_bullet_tracer_exposition )` and bullet::collide_front_face wanted
`min( 1.0f, max( ... ) )` - both recovered from the register shuffle alone.

### Defined function scores None: check the MANGLED ACCESS letters (and pointer-const)
SYMPTOM: a function defined in the base compiles fine but report.json shows no
fuzzy_match_percent (None) - objdiff cannot pair the symbols.
CAUSE: the mangled name differs in the ACCESS/virtual code or a parameter's top-level
pointer const: target `@@EBE`/`@@EAE` (private virtual) vs base `@@UBE`/`@@UAE` (public
virtual), `@@AAE`/`@@ABE` (private) vs `@@QAE`/`@@QBE` (public), or `QAV`/`QBV`
(`T* const`) vs `PAV`/`PBV` (`T*`) in the signature. grep both rich index.jsonl files for
the symbol and diff the letters; fix the header access section / add the pointer const.
Confirmed on weapon_core::serialize/deserialize (+update_bones_matrices, computed_*,
target_predicate...): pairing them moved serialize 0->62%, target_predicate 0->100%.
