## Index

### Build
* [toolchain-build.md](toolchain-build.md) — building the vostok-toolchain on Linux/Wine (mspdb DLLs, SP1 via `PATCH=`) and why Wine must be **staging** (cl `/Zi` → C1902 on wine-10.0 stable).

### Matching dept

* `static` functions are not delinked by the ghidra script.
* Delinker extensions fails for `vostok/core` functions.
* `pdb-parser` script is missing `static` keyword for static functions.
* `pdb-parser` script is missing typedefs for `pstr`, `pvoid`.

### Matching types
* `DONE` - the function is matched. No further attention is required.
* `PARTIAL` - the function is matched, but there are some unanswered questions.
* `SKIPPED` - the function is not matched. I tried and failed. Will return to it later.
* `BLOCKED` - the function is not matched, since it requires some other function to be present/matched first.
* `INPROGRESS` - the function is being matched.
* `STUB` - the function is in the state of the being matching.
