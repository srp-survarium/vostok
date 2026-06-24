# Out-of-line method's standalone symbol is an ICF fold to a trivial body; the REAL body shows only inlined
tags: cpp:member cpp:inline | asm:call asm:mov | topic:fold-icf topic:inline-vs-call
symptoms: standalone `Class::method` symbol is 5-byte `mov byte[x],N; ret` (or a clear-only stub), yet a cross-TU caller INLINES a much richer body for the same method (set/remove callback, boost::function assign, extra branches)
confidence: 7/10
variants: assert-misname-scored-matched.md, fold-misname-empty-fn.md

When a method is defined out-of-line in its `.cpp` AND its body is small/trivial enough,
`/OPT:ICF` folds the emitted standalone COMDAT onto an unrelated trivial function, so the
delinker mis-names a 5-byte `mov byte ptr [eax+NN], 0FFh; ret` (a u8 setter) as
`Class::method`. Meanwhile, an LTCG build INLINES the method's *real* body into a caller in
another TU, and there you see the full source: e.g. a one-line `m_sound_effect.initialize()`
in `weapon_sound_events_handler_state<T>::initialize` expands to `m_sounds_counter = 0xff`
**plus** `m_weapon.set_animation_callback(..., boost::bind(&...::on_sound_event, ...))`.

Do NOT take the trivial standalone symbol as ground truth for the source body - reconstruct the
RICH body from the inlined expansion (the inline site is the real shape; the standalone is a
fold artifact). The caller's single source line attributes to the whole inlined block.

```
; standalone symbol (ICF-folded artifact - NOT the real body):
weapon_sound_effect::initialize:  mov byte ptr [eax+25h], 0FFh ; m_sounds_counter
                                  ret
; inlined into the handler (line 81 = m_sound_effect.initialize()) - the REAL body:
  mov byte ptr [edi+18Dh], 0FFh                              ; m_sounds_counter
  ... boost::function1<...>::assign_to< bind_t<...on_sound_event...> >
  call weapon_core::set_animation_callback                   ; "sound_events", &m_sound_effect, fn
```
Evidence: game/weapon_sound_effect.cpp initialize (std 5 bytes) / finalize (std 0x28 bytes, clear-only)
vs the rich inlined bodies in weapon_sound_events_handler_state<T>::initialize/finalize.
