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
- LTCG / the linker can change how values cross a `call` boundary (which registers
  / how much stack) **and how the frame is laid out** (stack-slot assignment) - but
  it does not reorder the body. A body that is implementation-identical but differs
  only in register / `[ebp-XX]` stack-slot allocation is a **match** (see
  `MATCHING.md` - LTCG). Other linker effects exist and are not fully enumerated,
  so trust the operand-aware match % over a raw instruction-difference count.

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
