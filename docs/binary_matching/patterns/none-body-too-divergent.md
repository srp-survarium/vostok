# objdiff None can mean "body too divergent", not only bad mangling
tags: cpp:if | topic:scoring-artifact
symptoms: fuzzy_match_percent omitted, mangled name matches exactly, symbol present in base obj
confidence: 8/10
variants: mangled-join-failure.md, none-paired-at-zero.md, switch-case0-return-not-break.md

report.json can omit the percent for a function whose mangled name matches and is in the
base obj: the body diverges structurally enough (e.g. a whole missing compare/branch
block) that objdiff's symbol diff bails without a number. Before assuming
unreachable/mangling: byte-compare the two .text regions; if the symbol is present with
the right name, fix the body SHAPE, not the header.
