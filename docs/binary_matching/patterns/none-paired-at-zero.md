# report.json "None" can mean PAIRED AT 0.0% (protobuf default-value omission)
tags: cpp:template | topic:scoring-artifact
symptoms: fuzzy None but structure-diff resolves both sides, match_percent 0.0 in live diff
confidence: 8/10
variants: none-body-too-divergent.md, single-tu-anchor-inlines-helper.md

objdiff's report.json drops fuzzy_match_percent when the value is exactly 0.0 (proto3
JSON omits defaults), so paired-but-0% is indistinguishable from unpaired. Disambiguate
with `objdiff-cli diff -p binaries/objdiff -u <unit> <mangled>` (shows match_percent: 0.0
+ a target_symbol pairing) or structure-diff (resolves both sides by symbol).

Evidence: player_stamina::deserialize - anchored, paired, structure 4/4, every statement the r<T> cross-module inline wall -> fuzzy 0.0 -> "None".
