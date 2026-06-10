# SIZE rows decomposing into 3-byte deltas = disp8-vs-disp32 frame noise
tags: cpp:local | asm:mov | topic:pdb-locals topic:convention
symptoms: SIZE rows +-0x6..0x12, slices identical except [ebp-8Ch] vs [ebp-80h], 3 bytes per access
confidence: 8/10
variants: od-frame-slot-noise.md

Several SIZE rows whose two slices are instruction-for-instruction identical except
spill-slot addresses: each access past `-0x80` costs 3 extra bytes (disp8 -> disp32), so
a frame a few bytes bigger on one side turns pure allocation noise into SIZE rows. Read
the slices before chasing.

Evidence: collision_sensor::notify_and_erase_left_objects (99.82% - every delta byte was disp width).
