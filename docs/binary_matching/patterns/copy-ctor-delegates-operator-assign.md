# Copy ctor delegating to operator= + member-wise scalar operator=
tags: cpp:ctor cpp:operator | asm:call asm:fld asm:fstp asm:cmp | topic:codegen-idiom
symptoms: push other call operator= ret 4, cmp eax [ebp+8] je self-assign guard, fld/fstp pairs per member
confidence: 9/10

A copy ctor whose body is exactly `*this = other;` is one push + `call operator=` + the
standard MSVC `return this` (`mov eax,[ebp-4]`, not your source). The hand-written
operator= is a self-assignment guard then one fld/fstp pair per float member in
DECLARATION order.

```cpp
player_stealth::player_stealth( player_stealth const& other ) { *this = other; }
player_stealth& player_stealth::operator=( player_stealth const& other )
{
	if ( this != &other ) { m_a = other.m_a; /* ... declaration order ... */ }
	return *this;
}
```
```asm
; copy ctor: mov eax,[ebp+8]; push eax; mov ecx,[ebp-4]; call operator=; mov eax,[ebp-4]; ret 4
; operator=: mov eax,[ebp-4]; cmp eax,[ebp+8]; je .end
;            fld dword [src+0xNN]; fstp dword [dst+0xNN]   ; per member, 0x00,0x04,...
;            .end: mov eax,[ebp-4]; ret 4
```
Anchor (game_core): default-construct, copy-construct + a direct `b = a`, escape `&a`/`&b` through example_callback so LTCG does not DSE the stores.
Evidence: player_stealth pair 100% first rebuild; identical shape to the already-100% player_stamina pair.
