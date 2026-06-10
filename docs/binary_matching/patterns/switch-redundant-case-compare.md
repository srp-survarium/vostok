# MSVC /Od drops the case-compare for a case sharing the `default:` arm (non-steerable)
tags: cpp:switch | asm:cmp | topic:structure-shape topic:convention
symptoms: target emits N compares incl. a redundant one, base N-1, +0x3 on the dispatch row
confidence: 8/10
variants: switch-case0-return-not-break.md

When `case X:` and `default:` label the SAME arm, our compiler omits X's cmp/je (anything
unmatched falls to default = same block); the target emits all N compares including the
redundant one. Flipping the label order (`default: case X:` vs `case X: default:`)
changes nothing - both spellings dedup. Bank the few-byte SIZE row on the switch line.

Evidence: udp_match_connection::process_low_level_message (+0x3 on the dispatch row).
