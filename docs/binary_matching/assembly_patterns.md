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

### `objdiff fuzzy_match_percent: None` can mean "body too divergent", not only bad mangling
SYMPTOM: report.json omits the percent for a function whose mangled name matches the target's
exactly and is present in the base obj. CAUSE (besides access-specifier, above): the base body
diverges structurally enough (e.g. a whole missing compare/branch block) that objdiff's symbol
diff bails without a number. Before assuming unreachable/mangling: byte-compare the two `.text`
regions; if the symbol is present with the right name, fix the body shape, not the header.

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

### stripped/folded empty out-of-line call can appear MID-BODY with a real source line
SYMPTOM: `mov byte[ebp-1],0; lea eax,[ebp-1]; call <fn>` mid-function (a 1-byte local set to
false, its address taken, then a `call`), where the call target is a single `ret` (delinker
COMDAT-misnames it `fixed_size_allocator<...>::finalize_impl`), the arg is passed in EAX with
NO push and NO stack cleanup (matches no writable C++ calling convention), and NO nameable
symbol for the callee exists in EITHER rich index. This is the same class as the prologue
`mov byte[ebp-1],0; ...; call empty_stub` artifact (above) - a call to a helper that is
empty/stripped/COMDAT-folded whole-program - but here it carries a genuine source-line number
(it IS a real source statement) rather than being a prologue artifact. NOT reproducible from
the function's own source. Match every other statement and mark PARTIAL. Verify it is this
class (not a real callee) by: (1) the call target disassembles to a bare `ret`; (2) the arg
register/convention is non-standard; (3) `pdb_rich_query` finds no standalone symbol in target
OR base. Confirmed in `game_core/weapon_core_show_state_base::{initialize (line 28, 75.13%),
on_animation_end_impl (line 42, 69.93%)}` - every other statement (base init/finalize call,
instant_toggle_start/end, *m_is_shown=true, result=true) is byte-exact.

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

### const setter/getter that calls a TRIVIAL inline accessor is the get_user() inline-vs-call class
A `m_ref.get_x()` where `get_x` is a one-line header accessor (`{ return m_member; }`)
compiles in the TARGET to an out-of-line `call get_x` (the accessor kept standalone), but
our `/GL` LTCG inlines it whole-program to a direct `mov eax,[m_ref+offset_of_m_member]`
(no standalone symbol in base; `pdb_rich_query base --function ...::get_x` -> "no function
matched"). The inline also adds a frame temp, so the surrounding `[ebp-XX]` slots shift -
all unsteerable from the caller's source. Same class as the documented trivial-accessor
inline-vs-call; recognize it (target standalone getter at a real rva + base has none) and
stop at PARTIAL, do not rewrite. Caught on
`weapon_core_animation_end_aware_state::set_animation_to_wait` (target `call
weapon_core::get_user` @0x9b330; base inlined `mov eax,[m_weapon+44Ch]` -> 77.33%).

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

### `member = math::min( arg, member )` residual = which xmm each arg lands in (LTCG arg passing)
A trivial setter `m_x = vostok::math::min( arg, m_x );` emits `movss xmm0,arg; movss
xmm1,[this+off]; call min; movss [this+off],xmm0`. Under our LTCG the out-of-line `float
min(const float,const float)` gets a custom register convention chosen at LINK time: the
TARGET assigns args by position (arg0->xmm0, arg1->xmm1) and loads arg0 first; our BASE may
reverse it (member->xmm0, arg->xmm1, member loaded first). Same instrs, same frame, same
store - ONLY the two `movss` operands swap registers. This is the permitted call-boundary
arg-passing class (register-instead-of-slot / link-time convention); no source change to
`min( arg, member )` steers it without altering semantics. Mark DONE at the resulting %
(~84%), not PARTIAL. Caught on `legs_ik_processor::leg_params::set_{heel,toe}_transition_time`
(83.69%).

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
