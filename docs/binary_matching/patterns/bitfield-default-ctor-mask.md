# A default constructor clearing seven bitfields emits `and byte, 0x80`

tags: cpp:bitfield cpp:ctor cpp:inline | asm:and asm:mov | topic:codegen-idiom topic:shared-header topic:source-fidelity
symptoms: a one-byte struct's target construction preserves only the unused high bit; the reconstructed inline ctor is empty; a generic placement/copy helper independently contains the same mask
confidence: 9/10

For seven one-bit `bool` members occupying bits 0 through 6, an inline default
constructor whose initializer list sets every member to `false` can collapse to:

```asm
and byte ptr [object], 80h
```

The mask preserves the unused high bit and clears all declared fields. Do not
imitate it with caller-local assignments when the same mask appears in an
independent generic construction path. A placement/copy helper that constructs
`T` at a destination before assigning the source is especially strong evidence:
the helper has no knowledge of any caller's desired values, so its mask belongs
to `T::T()` in the shared header.

`render::scene_configuration` supplied the worked case. Its target
`main_menu::query_resources` body differed from the base only by `and bl,80h`
versus `and bl,0FCh`, while target
`concrete_type_helper<scene_configuration>::copy` also performed
`and byte ptr [dest],80h`. Restoring all seven `false` bitfield initializers in
the shared constructor made both `main_menu::query_resources` and
`login_menu::query_resources` byte-exact. The copy helper retained a separate
helper-call versus direct-copy inline-depth residual; that does not weaken the
constructor evidence.

An extracted PDB header may print an inline constructor as `{ /* no source */ }`
because optimized statement records do not preserve its initializer list. Treat
the mask at multiple real construction sites as the stronger source evidence.
