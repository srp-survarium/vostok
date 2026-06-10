# NODEFAULT jump-table switch: diff FOOTER reads ~55-65% while report.json says 100
tags: cpp:switch | asm:jmp | topic:scoring-artifact
symptoms: (bad) instructions past ret, jmp .N vs jmp .N+1 label shift, footer % far below report.json
confidence: 9/10
variants: switch-default-nodefault.md, switch-jump-table-relocs.md

A no-bounds-check jump-table switch can be a TRUE 100% while the `--view diff` footer
under-counts: (1) leaf `jmp .N`(base) vs `.N+1`(target) labels differ but resolve to the
SAME address (the embedded table is a distinct symbol whose label objdiff counts); (2)
the table dwords past `ret` disassemble as junk holding binary-specific RVAs, scored as
mismatches. The AUTHORITATIVE number is report.json units[].functions[].fuzzy_match_percent
(top-level, not .measures); always reconcile before banking a non-100%.

Do NOT chase the footer with result-temps or trailing returns - a trailing `return X;` after the switch re-introduces a cmp/ja + dead store (WORSE code). Keep `default: NODEFAULT();`.
Evidence: jump_logic::does_need_land_and_run (TRUE 100%, footer 63.7%); get_jump_animation_index (footer 55.0%).
