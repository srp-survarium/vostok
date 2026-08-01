# objdiff None can mean "body too divergent", not only bad mangling - and REPEATED blocks force it at ~90% equal
tags: cpp:if cpp:macro | topic:scoring-artifact topic:logging
symptoms: fuzzy_match_percent omitted, mangled name matches exactly, symbol present in base obj, objdiff-cli match_percent 0.0 while pdb_fetch --view diff reports most instructions equal
confidence: 7/10
variants: mangled-join-failure.md, none-paired-at-zero.md, switch-case0-return-not-break.md

report.json can omit the percent for a function whose mangled name matches and is in the
base obj: the body diverges structurally enough (e.g. a whole missing compare/branch
block) that objdiff's symbol diff bails without a number. Before assuming
unreachable/mangling: byte-compare the two .text regions; if the symbol is present with
the right name, fix the body SHAPE, not the header.

## The repeated-block trap - do NOT read 0.0 as "the body is wrong"
A function that expands the SAME macro many times - N `LOG_*` sites, N `ASSERT`s, an
unrolled ladder - gives objdiff N near-identical instruction blocks on both sides. The
alignment then has many equally-good pairings; add one genuinely moved block and the
symbol comes back **`match_percent: 0.0`** even though the bodies are nearly the same.
Cross-check with a second aligner before believing the number:

```sh
objdiff-cli diff -1 <target.obj> -2 <base.obj> -o - --format json "<mangled>"   # 0.0
pdb_fetch --target-index ... --base-index ... --function <fn> --view diff       # N/M equal
pdb_fetch ... --view structure-diff                                             # statement shape
```

Confirm the pairing is real first: parse both `.obj` symbol tables (the name must be a
defined function in section `.text` on both sides), or read `target_symbol` in the
objdiff-cli JSON - if the two entries cross-reference each other, objdiff DID pair them
and 0.0 is a scoring outcome, not a missing symbol.

Evidence: `render::device::on_device_removed` - six `LOG_ERROR` arms, base 0x469 vs target
0x43f, symbols mutually paired. objdiff-cli reports `match_percent 0.0` on both sides, so
report.json prints no percent and match_score counts it unpaired. `pdb_fetch --view diff`
on the same pair lists ~40 diverging instructions out of ~330 - the first 0x93 bytes are
byte-identical, `__FILE__`/`__LINE__` immediates included - and `--view structure-diff`
shows 8 target statements vs 10 base with a single moved block. The real residual was one
inline-budget artifact, not a shape error.
Corollary: for macro-repetitive functions treat pdb_fetch's diff as the quality signal and
objdiff's percent as an indicator that can collapse to zero without cause.
