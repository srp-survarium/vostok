# `cmp [p],0; je; mov ecx, <3-byte fn>; test ecx,ecx; je` = the smart-pointer SAFE-BOOL `if ( ptr )`

tags: cpp:if cpp:bool cpp:smart-ptr | asm:mov asm:test asm:cmp asm:jcc | topic:codegen-idiom topic:condition-shape

## Symptom
An optimized guard tests a pointer member, then loads a **constant code address**
into a register and tests THAT for zero - a test that can never fail:

```
0x54:    cmp   dword ptr [esi], 0
0x57:    je    short .skip
0x59:    mov   ecx, vostok::intrusive_ptr<...>::c_ptr     ; mov ecx, imm32 (B9 xx xx xx xx)
0x5e:    test  ecx, ecx
0x60:    je    short .skip
```

The immediate resolves to a **3-byte function** (`8B 01 C3` = `mov eax,[ecx]; ret`)
that ICF has folded hundreds of accessors onto, so the delinker names it after an
arbitrary one (`intrusive_ptr<sound::sound_emitter,...>::c_ptr`). In survarium the
exact byte string `b9 50 39 06 00` occurs ~100 times.

## Cause
This is the **safe-bool idiom**, not dead code. `vostok/intrusive_ptr.h` declares

```
typedef object_type* ( self_type::*unspecified_bool_type )( ) const;
inline operator unspecified_bool_type ( ) const;      // returns m_object ? &self_type::c_ptr : 0
```

so `if ( ptr )` inlines to "if m_object then result = &c_ptr else result = 0" followed
by "if result != 0". MSVC folds the *value* but keeps the two-step test, hence the
constant load plus a redundant `test`.

## Source mapping
- `mov <reg>, imm32; test; jcc` present  ->  the source wrote the smart pointer
  **bare in a boolean context**: `if ( m_sky_material )`.
- the extra load **absent** (just `cmp dword ptr [p], 0; jcc`)  ->  the source went
  through the raw pointer instead: `if ( m_sky_material.c_ptr( ) )` (or `operator!` /
  an `operator==( T const* )` comparison).

Both spellings are byte-identical apart from that 7-byte block, so the asm is the
only way to tell them apart - reproduce whichever the target used. `scene::set_sky_material`
mixes both in one function (raw-pointer test before `remove_material_effects`, safe-bool
test before `add_material_effects`) and only matches 100% when each guard is spelled the
way the target's bytes demand.

## Related
- `positive-getter-inline-sete.md` (inlined getter in a condition, no constant load).
- `fold-misname-empty-fn.md` (why the delinker's name for the folded target is noise).
