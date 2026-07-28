# A recovered inline call site can preserve a standalone COMDAT
tags: cpp:member cpp:inline | asm:call | topic:anchoring topic:inline-vs-call
symptoms: a small target member has a standalone COMDAT, while the base has only an equivalent open-coded body at a real caller
confidence: 7/10
variants: real-call-sites-for-templates.md, single-tu-anchor-inlines-helper.md

A small member may disappear from the base even though the target retains a standalone
COMDAT. If target source lines, definition order, or caller assembly show that a sibling
method called the helper, reconstruct that real call site instead of keeping an open-coded
copy of the helper body. Under `/Ox /Ob2`, the call can inline to the same caller bytes while
the source reference also keeps the standalone COMDAT available for pairing.

Do not add a call merely to force emission. The call-site reconstruction needs independent
target evidence, and the caller's structure and bytes must remain consistent after the
change. If the call does not inline or changes the caller shape, the reconstruction is
wrong for that site.

Example: `receiver::keyboard::reset_current_state` is defined before `on_activate` in the
target source and the target `on_activate` assembly contains its `memory::zero` body inline.
Writing the source as `reset_current_state()` preserves both the real caller shape and the
standalone helper.
