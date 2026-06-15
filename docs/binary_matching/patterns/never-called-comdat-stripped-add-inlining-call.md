# A never-called private member is stripped (unpaired); a call site that INLINES rescues it
tags: cpp:member cpp:inline | asm:call | topic:anchoring topic:inline-vs-call
symptoms: objdiff `unpaired` for a small private method the target keeps as a COMDAT; no callers in the target binary either
confidence: 7/10
variants: real-call-sites-for-templates.md, single-tu-anchor-inlines-helper.md

The target keeps an unreferenced small private member as a standalone COMDAT (its build did
not aggressively /OPT:REF-strip it), but OUR linker strips it - objdiff reports it `unpaired`
(`no function matched` on the base side). You cannot fabricate a fake reference, but if a
sibling method's body is the SAME one-liner the helper holds (e.g. both do
`memory::zero( &m_buf, sizeof(m_buf) )`), switch those sibling sites from the open-coded body
to a `helper()` CALL: under /Ox /Ob2 the call INLINES (byte-identical to the open-coded form,
so the caller's score is unchanged) AND the source reference keeps the COMDAT alive so the
delinker pairs it. Net: the helper jumps from unpaired to 100% for free.

Verify the caller's % is unchanged after the swap (it must be - same inlined bytes); if it
drops, the call did NOT inline and you changed the caller's shape - revert.

Evidence: input/receiver::keyboard::reset_current_state - unpaired -> 100%; on_activate /
execute switched `memory::zero(&m_current_key_state,...)` -> `reset_current_state()` (both
inlined, 69% / 58.75% unchanged), receiver_keyboard_win.cpp.
