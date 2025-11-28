## Index

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
