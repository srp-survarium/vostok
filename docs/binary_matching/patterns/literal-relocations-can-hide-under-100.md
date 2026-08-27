# A 100% function score can still reference the wrong string literal

## Signal

A function is byte-exact or near-exact, but its target and base rich-index
instruction streams name different `??_C@...` operands at otherwise identical
loads or pushes. This commonly survives as 100% because object comparison
normalizes data relocations and does not compare the referenced string bytes.

## Proof and handling

Compare the ordered literal-symbol sets in
`binaries/rich/{target,base}/index.jsonl`. Ignore only proven build-path
metadata such as `__FILE__`. For every other difference, decode it with
`vostok sema strings <fn>` and confirm long or truncated values directly with
`strings -a binaries/nix-store/survarium-game/survarium.exe`.

Restore the target spelling even when the base spelling looks cleaner. Literal
ownership is also evidence: two file-local functions with the same demangled
name can each score exact while carrying one another's strings, so pair the
literal with the PDB source file rather than with the name alone.

Render examples include statistics labels that lost capitalization or gained
`num ` prefixes, shader identifiers whose retail spelling ends in `backed`,
and the two `create_color_grading_base_lut` helpers whose texture names were
swapped between source files.
