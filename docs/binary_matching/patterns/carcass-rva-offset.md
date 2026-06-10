# Carcass `// FUNCTION BODY[0xVA]` RVAs are +0x10000 off the rich index RVA
tags: cpp:member | topic:tooling
symptoms: FUNCTION BODY[0xa72f0] vs index rva 0x972f0
confidence: 5/10

The structure-carcass comment addresses are offset +0x10000 from the rva keys in
binaries/rich/target/index.jsonl (carcass 0xa72f0 == index 0x972f0, 0x8d690 == 0x7d690).
Subtract 0x10000 when fetching a carcass function's disasm by --rva, or look it up by
--function/name.
