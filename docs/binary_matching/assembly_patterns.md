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
