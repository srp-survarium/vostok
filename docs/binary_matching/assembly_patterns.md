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
