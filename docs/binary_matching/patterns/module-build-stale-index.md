# Module-only ninja_build does NOT relink: the base index is stale for new symbols
tags: cpp:template | topic:tooling
symptoms: freshly-anchored symbols missing from binaries/rich/base/index.jsonl
confidence: 5/10
variants: stale-comdat-other-module.md

`scripts/ninja_build.py <module>` (and `rebuild.py <module>`) compiles the module's objs
but skips the EXE/PDB link. The base structure/rich index is generated from the PDB, so
brand-new symbols (freshly-anchored template instantiations) won't appear in the base
index until a FULL relink: run `python3 scripts/rebuild.py` with NO module arg (~10 min)
before scoring anchors you just added.
