# Structure verification: legs_ik_processor::leg_params:: setters (PR #134)

Unit: `sources/vostok/game_core/sources/legs_ik_processor.cpp`

Scope: the FOUR `leg_params::` MEMBER setters, all tagged `// STATE[100%|DONE]`.
The free functions `legs_ik_processor::set_heel_on_ground` /
`legs_ik_processor::set_toe_on_ground(leg_params&, bool)` lower in the file are
STUBs and are NOT part of this verification.

Verdict summary: ALL FOUR are STRUCTURE MATCH. Every function's base skeleton
reproduces the target's statement QUANTITY and per-statement SIZE exactly. The
two transition_time setters' freshly-fixed 100% is over the CORRECT structure.
Nothing downgraded; nothing blocks merge.

report.json `fuzzy_match_percent` (locally-built, fresh): all four = 100.0
(confirmed by grep of `binaries/objdiff/report.json`).

RVAs resolved from the rich indexes:

| function | target rva | base rva |
|---|---|---|
| set_heel_transition_time(float) | 0x6ea930 | 0x50a120 |
| set_toe_transition_time(float)  | 0x6ea900 | 0x50a0f0 |
| set_heel_on_ground(bool)        | 0x6ea890 | 0x50a080 |
| set_toe_on_ground(bool)         | 0x6ea820 | 0x50a010 |

(The free-function stubs are the OTHER symbols at target 0x6eab70 /
0x6eaa70 - not verified here.)

---

## 1. set_heel_transition_time(float) -- STRUCTURE MATCH

TARGET ; 3 statements, 0x29 bytes        BASE ; 3 statements, 0x29 bytes
  0x00  <0x9>   L70                         0x00  <0x9>   {
  0x09  <0x1a>  L71                         0x09  <0x1a>  heel_transition_time = math::min( heel_transition_time, tr_time );
  0x23  <0x6>   L72                         0x23  <0x6>   }

QUANTITY: 3 == 3. SIZE: 0x9 / 0x1a / 0x6 identical on both sides.
The body assignment statement is 0x1a on both sides. The operand-order fix
(`math::min( member, arg )` -> member-first) reproduces the target shape; the
83.69% (operands in swapped xmm registers) is gone, 100% is over the matching
structure. No braces/block divergence (a flat 3-statement setter).

## 2. set_toe_transition_time(float) -- STRUCTURE MATCH

TARGET ; 3 statements, 0x29 bytes        BASE ; 3 statements, 0x29 bytes
  0x00  <0x9>   L75                         0x00  <0x9>   {
  0x09  <0x1a>  L76                         0x09  <0x1a>  toe_transition_time = math::min( toe_transition_time, tr_time );
  0x23  <0x6>   L77                         0x23  <0x6>   }

QUANTITY: 3 == 3. SIZE: 0x9 / 0x1a / 0x6 identical. Same as setter #1 - the
member-first operand order yields the correct 0x1a-byte assignment statement.
Genuine 100% over matching structure.

## 3. set_heel_on_ground(bool) -- STRUCTURE MATCH

TARGET ; 5 statements, 0x62 bytes        BASE ; 5 statements, 0x62 bytes
  0x00  <0x9>   L80                         0x00  <0x9>   {
  0x09  <0x9>   L81                         0x09  <0x9>   m_heel_on_ground = value;
  0x12  <0x3a>  L82                         0x12  <0x3a>  if ( is_full_on_ground( ) )
  0x4c  <0x10>  L83                         0x4c  <0x10>  m_time_since_stance = 0.0f;
  0x5c  <0x6>   L84                         0x5c  <0x6>   }

QUANTITY: 5 == 5. SIZE: 0x9 / 0x9 / 0x3a / 0x10 / 0x6 identical.
Control-structure check: the `if ( is_full_on_ground( ) )` is ONE statement at
0x12 (size 0x3a) followed directly by the brace-less body
`m_time_since_stance = 0.0f;` at 0x4c. There is NO `[n]` block-open and NO
`+0x002` closing-brace jmp on either side - i.e. a single-statement brace-less
`if`, matching the target's flat shape (not a braced `if ( p ) { ... }` block).
The 0x3a-byte `if` statement carries the inlined `is_full_on_ground()` predicate
(documented in assembly_patterns.md: the predicate materializes a third bool
return-temp, giving the 0x0C frame). Both sides agree.

## 4. set_toe_on_ground(bool) -- STRUCTURE MATCH

TARGET ; 5 statements, 0x62 bytes        BASE ; 5 statements, 0x62 bytes
  0x00  <0x9>   L87                         0x00  <0x9>   {
  0x09  <0x9>   L88                         0x09  <0x9>   m_toe_on_ground = value;
  0x12  <0x3a>  L89                         0x12  <0x3a>  if ( is_full_on_ground( ) )
  0x4c  <0x10>  L90                         0x4c  <0x10>  m_time_since_stance = 0.0f;
  0x5c  <0x6>   L91                         0x5c  <0x6>   }

QUANTITY: 5 == 5. SIZE: 0x9 / 0x9 / 0x3a / 0x10 / 0x6 identical. Same shape as
setter #3 (brace-less single-statement `if`, no added block). Genuine 100% over
matching structure.

---

## Outcome

No QUANTITY divergence and no SIZE divergence in any of the four. The fresh
operand-order fix on the two transition_time setters lands the correct structure
(verified: the 0x1a assignment statement matches the target's L71/L76). The two
on_ground setters keep the brace-less single-statement `if` the target uses (no
spurious lexical block). No carcass restore, no STATE downgrade, no source change
needed. All four remain `100%|DONE` legitimately. Nothing blocks merge.
