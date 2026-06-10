# A header fix that "does not take" = a STALE OTHER-MODULE COMDAT winning the link
tags: cpp:template cpp:operator | topic:tooling
symptoms: rebuilt objs carry new bytes but linked base shows the old compare, anchor TU never recompiled
confidence: 8/10
variants: module-build-stale-index.md

After a header fix the rebuilt module objs can carry the new bytes while the linked base
STILL shows the old code: the surviving COMDAT emission came from ANOTHER module's
temp_include_all anchor whose TU never recompiled (PCH staleness in that module). If a
one-line header fix provably does not move the diff, `touch` the pch.h of EVERY module
instantiating the COMDAT (the anchor TU especially) and rebuild.

Evidence: udp_match_stats.h items operator>= - game_core's temp_include_all emission won the link; after the touch 97.74 -> 100.00.
