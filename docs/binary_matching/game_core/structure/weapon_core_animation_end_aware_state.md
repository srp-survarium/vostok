# Structure verification - weapon_core_animation_end_aware_state.cpp (PR #127)

Unit: sources/vostok/game_core/sources/weapon_core_animation_end_aware_state.cpp
Indexes: binaries/rich/{target,base}/index.jsonl ; binaries built locally, FRESH at 12:49.
This verification did NOT rebuild and changed NO compiled bytes (carcass/comment edits only).

## Resolved RVAs

| function                | target rva | base rva  | report fuzzy % | STATE     |
|-------------------------|------------|-----------|----------------|-----------|
| on_animation_end        | 0x6ed050   | 0x44b6c0  | 90.916664      | PARTIAL   |
| finalize                | 0x6ed0c0   | 0x44b730  | 100.0          | DONE      |
| set_animation_to_wait   | 0x6ed0e0   | 0x44b840  | 77.333336      | PARTIAL   |
| initialize              | 0x6ed130   | 0x44b750  | 100.0          | DONE      |

Unit fuzzy_match_percent (report.json): 94.09. All four numbers confirmed against
binaries/objdiff/report.json - they match the STATE markers and the PROGRESS ledger.

## Per-function verdicts (one line each)

- initialize           -> STRUCTURE MATCH  (4 vs 4 statements, 0xe2 vs 0xe2 bytes; all sizes equal) - genuine 100% DONE
- finalize             -> STRUCTURE MATCH  (3 vs 3 statements, 0x1f vs 0x1f bytes; all sizes equal) - genuine 100% DONE
- set_animation_to_wait-> STRUCTURE MATCH (size)  (4 vs 4 statements; one statement +0x7 larger: get_user() inline-vs-call) - PARTIAL residual, NOT a source bug
- on_animation_end     -> STRUCTURE MATCH (size)  (9 vs 9 statements; one statement +0x1 larger: == operand-order from inline-vs-call) - PARTIAL residual, NOT a source bug

No QUANTITY divergence anywhere: every function has the SAME statement count, the same
[n] block-opens, and the same <n> no-address marker lines on both sides. The two PARTIALs'
residuals are pure SIZE divergences localized to a single statement each, both the
documented trivial-accessor inline-vs-call LTCG class (assembly_patterns.md "const setter/
getter that calls a TRIVIAL inline accessor is the get_user() inline-vs-call class").


## 1. initialize()  -- STRUCTURE MATCH, genuine 100% DONE

```
TARGET (0x6ed130): 4 statements, 0xe2 bytes      BASE (0x44b750): 4 statements, 0xe2 bytes
0x00  <0xd>   L16                                0x00  <0xd>   {
0x0d  <0xa>   L18                                0x0d  <0xa>   m_animation_has_been_ended = false;
0x17  <0xc6>  L19                                0x17  <0xc6>  );  (set_animation_callback boost::bind)
0xdd  <0x5>   L20                                0xdd  <0x5>   }
```
QUANTITY: equal (4 vs 4). SIZE: equal at every statement (0xd, 0xa, 0xc6, 0x5).
No divergence. Genuine match over MATCHING structure. Keep STATE[100%|DONE], carcass
correctly stripped (clean DONE).


## 2. finalize()  -- STRUCTURE MATCH, genuine 100% DONE

```
TARGET (0x6ed0c0): 3 statements, 0x1f bytes      BASE (0x44b730): 3 statements, 0x1f bytes
0x00  <0x7>   L23                                0x00  <0x7>   {
0x07  <0x14>  L24                                0x07  <0x14>  m_weapon.remove_animation_callback( ... );
0x1b  <0x4>   L25                                0x1b  <0x4>   }
```
QUANTITY: equal (3 vs 3). SIZE: equal at every statement (0x7, 0x14, 0x4).
No divergence. Genuine match over MATCHING structure. Keep STATE[100%|DONE], carcass
correctly stripped (clean DONE).


## 3. set_animation_to_wait( managed_resource_ptr const& ) const  -- STRUCTURE MATCH (size only), PARTIAL

```
TARGET (0x6ed0e0): 4 statements, 0x46 bytes      BASE (0x44b840): 4 statements, 0x4d bytes
0x00  <0x9>   L28                                0x00  <0x9>   {
0x09  <0x25>  L29  <- SIZE DIFF                  0x09  <0x2c>  if ( !m_weapon.get_user()->is_replaying_history() )
0x2e  <0x12>  L30                                0x35  <0x12>  m_animation_to_wait_for = animation;
0x40  <0x6>   L31                                0x47  <0x6>   }
```
QUANTITY: equal (4 vs 4). The control structure is identical: the `if` guard plus its
single brace-less body assignment, no extra [n] block, no missing/extra <n> line.

SIZE: ONE diverging statement - L29 (the `if` condition), target 0x25 vs base 0x2c
(base is +0x7 larger). The rest are byte-identical (0x9, 0x12, 0x6); total 0x46 vs 0x4d
= +0x7 from that one statement.

CAUSE (from --view diff): the trivial accessor `weapon_core::get_user()` inline-vs-call.
- TARGET: `mov eax,[ebp-14h]; mov eax,[eax+128h]; call survarium::weapon_core::get_user;
  mov [ebp-18h],eax`  -- get_user() kept OUT-OF-LINE (a real symbol @ target rva 0x9b330).
- BASE:  LTCG/`/GL` inlines get_user() whole-program to `mov edx,[ecx+128h];
  mov eax,[edx+44Ch]; mov [ebp-1Ch],eax` -- a direct read of m_user (@0x44C) off m_weapon
  (@0x128), plus an extra `[ebp-1Ch]` temp slot (frame grows 0x18 -> 0x1C).
Confirmed get_user() has NO standalone symbol in the BASE rich index (inlined everywhere);
it exists out-of-line in TARGET. This is the documented inline-vs-call LTCG residual, not a
control-structure or member bug. The source is correct as written.

SOURCE FIX: none available from this function's source - it is a whole-program inline
decision (same class as vectora::size()/is_aimed()/fixed_string inline-vs-call). Stays
PARTIAL. Will only close when get_user() is forced out-of-line whole-program (not steerable
from the caller). NO rebuild needed; nothing to change.


## 4. on_animation_end( animation_callback_params& )  -- STRUCTURE MATCH (size only), PARTIAL

```
TARGET (0x6ed050): 9 statements, 0x6d bytes      BASE (0x44b6c0): 9 statements, 0x6e bytes
0x00  <0x9>   L34                                0x00  <0x9>   {
0x09  <0x7>   L35                                0x09  <0x7>   params.interrupt_animation_player_tick = false;
0x10  <0x10>  L37                                0x10  <0x10>  if ( params.animated_object == &m_weapon )
0x20  <0xc>   L38                                0x20  <0xc>   ASSERT( UNKNOWN_EXPRESSION );
0x2c  <0x1b>  L40  <- SIZE DIFF                  0x2c  <0x1c>  if ( m_animation_to_wait_for == params.animation )
0x47  <0xa>   L41                                0x48  <0xa>   m_animation_has_been_ended = true;
0x51  <0x14>  L42                                0x52  <0x14>  on_animation_end_impl( params.interrupt_animation_player_tick );
0x65  <0x2>   L46                                0x66  <0x2>   return animation::callback_return_type_call_me_again;
0x67  <0x6>   L47                                0x68  <0x6>   }
```
QUANTITY: equal (9 vs 9). Both sides carry the recovered ASSERT statement (L38, 0xc bytes
on both - the `mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub` compiled-out ASSERT,
delinker-misnamed `finalize_impl` on target / `debug_render` on base). The two nested `if`
guards, the two body statements, the `return`, and the scope braces all line up 1:1. No
extra/missing [n] block, no extra/missing <n> line.

SIZE: ONE diverging statement - L40 (`if ( m_animation_to_wait_for == params.animation )`),
target 0x1b vs base 0x1c (base +0x1 larger). All other statements are byte-identical
(0x9, 0x7, 0x10, 0xc, 0xa, 0x14, 0x2, 0x6); total 0x6d vs 0x6e = +0x1.

CAUSE (from --view diff): operand-evaluation order inside the `==` setup, a consequence of
the same inline-vs-call frame shape.
- TARGET: `mov edx,[ebp-8]; add edx,138h; mov eax,[ebp+8]; mov eax,[eax+4]` then
  `call intrusive_ptr::operator==`  -- computes &m_animation_to_wait_for (@0x138) FIRST,
  then loads params.animation.
- BASE:  `mov edx,[ebp+8]; mov eax,[edx+4]; push eax; mov ecx,[ebp-8]; add ecx,138h`
  -- loads params.animation FIRST, then &m_animation_to_wait_for; one extra byte.
This is a materialization/ordering difference flowing from the LTCG inline-vs-call frame
shape, not a control-structure or member-selection bug. The 90.92% (up from 78.03% before
the ASSERT was recovered) confirms the ASSERT recovery landed.

SOURCE FIX: none - the control structure and member offsets are already correct; the only
residual is the operand-order/frame shape from the inline-vs-call class. Stays PARTIAL.
NO rebuild needed.


## Carcass restoration (the worker's mistake, fixed in this commit)

Per MATCHING.md "The carcass": a non-100% (PARTIAL) function must PRESERVE its
`// FUNCTION BODY` block VERBATIM, including the `<0> <1> <2>` / `<n>` no-address marker
lines and the original whitespace; only a clean 100% DONE deletes the carcass. The worker
had rewritten both PARTIAL carcasses LOSSILY:

- set_animation_to_wait: header reduced to `// FUNCTION BODY` (dropped the
  `[0x6fd0e0]: 2` statement-count header) and the `// CALL SITE INFO` block was dropped.
  (This function's authoritative carcass has no `<n>` lines, so none were lost there.)
- on_animation_end: header reduced to `// FUNCTION BODY` (dropped `[0x6fd050]: 12`) AND all
  four `<n>` no-address marker lines were stripped: the `<0>` after L35, the `<0>` after
  L37/L38, and the `<0> <1> <2>` group before L46. The `// CALL SITE INFO` block was also
  dropped.

Both carcasses were regenerated VERBATIM from the authoritative generated structure stub
(binaries/structure/target/sources/vostok/game_core/sources/weapon_core_animation_end_aware_state.cpp),
restoring the `[rva]: N` headers, every `<n>` no-address line, the `// CALL SITE INFO`
blocks, and the blank-line layout. The worker's matched-statement annotations were kept to
the RIGHT of each addressed line (and the two SIZE-diff statements are annotated with the
target/base byte sizes). The two 100% DONE functions' (correctly stripped) carcasses were
left untouched.


## Final verdict

- initialize            -> STRUCTURE MATCH (genuine 100% DONE over matching structure)
- finalize              -> STRUCTURE MATCH (genuine 100% DONE over matching structure)
- set_animation_to_wait -> STRUCTURE MATCH (size); residual = get_user() inline-vs-call LTCG; correctly PARTIAL
- on_animation_end      -> STRUCTURE MATCH (size); residual = inline-vs-call frame/operand-order; correctly PARTIAL

Nothing is DEFINITELY INCORRECT. No source/structure bug found: both 100% DONEs are real
matches over the target's structure (not high-%-over-wrong-structure), and both PARTIAL
residuals are honest inline-vs-call LTCG (the narrowed MATCHING.md call-boundary excuse),
each a single localized SIZE delta over an otherwise byte-identical, structurally identical
body. No STATE downgrade, no PROGRESS/.md change needed (the numbers already match). The
only fix applied is the carcass restoration described above (zero compiled bytes changed).
