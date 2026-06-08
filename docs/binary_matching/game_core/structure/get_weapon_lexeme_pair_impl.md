# Structure verification: get_weapon_lexeme_pair_impl

Function: `survarium::get_weapon_lexeme_pair_impl(...)` (FREE function in namespace
`survarium`, not a class method - resolved via `pdb_rich_query --list`).
Target rva `0x7a13e0`, unit `vostok/game_core/sources/weapon_lexeme_pair.cpp`.
PR #155, branch `match/game_core-get-weapon-lexeme-pair-impl`.

## Verdict

STRUCTURE MATCH.

The source as written in this PR reproduces the target's statement skeleton 1:1:
23 target statements -> 23 source statements, with matching control structure
(braced `if` with a 2-statement body, two ctor statements, two fluent setter
chains attributed one-statement-per-line, the lone 4-byte dead-store bool, and the
single `return`). No QUANTITY, SIZE-shape, or ORDER divergence in the SOURCE
STRUCTURE.

The PR's `None` (unpaired) outcome is NOT a source-structure defect: it is the
whole-program LTCG inline-vs-call of the trivial inline-in-class
`animation_lexeme_parameters` setters, which live in the out-of-scope `animation`
module headers as inline COMDATs. /GL inlines ~6 of them at this call site (target
keeps them out-of-line `call`s), shifting the [ebp-XX] layout and shortening the
body below objdiff's pairing threshold. That is a codegen/linker artifact, not a
restructure the matcher can apply from this unit. Same unsteerable class as
scheduler::on_frame / operator| / fixed_string.

## Base availability

BASE NOT BUILT for this PR. The shared base index
(`binaries/rich/base/index.jsonl`) contains only the STALE trunk STUB at rva
`0x577c40` (3 statements / 0x26 bytes, old signature `playing_type_enum`), which
predates this PR's reconstruction. Comparison is therefore CARCASS-vs-SOURCE
(target live PDB vs source as written), per the dispatch caveat.

```
stale-trunk base STUB (NOT this PR):
; 3 statements, 0x26 bytes
0x00  <0x5>   {
0x05  <0x1d>  );
0x22  <0x4>   }
```

## The two skeletons (target live PDB vs PR source as written)

Target `--view structure` (rva 0x7a13e0): ; 23 statements, 0x1f7 bytes
The carcass embedded in weapon_lexeme_pair.cpp is verbatim-identical to this live
view (all 23 +delta sizes match).

```
TARGET (off  <size>  srcline)        BASE/PR SOURCE statement (file line)
0x00   <0xb>   L23   decl/prologue    function header                       L23-34
0x0b   <0x14>  L32   main params ctor main_lexeme_parameters(...,NULL,NULL)  L40
0x1f   <0xb>   L33   .animated_object   chain elem                          L42
0x2a   <0xb>   L34   .playback_type     chain elem                          L43
0x35   <0xd>   L35   .bones_mask(2)     chain elem                          L44
0x42   <0xd>   L36   .weight_sync(0x80) chain elem                          L45
0x4f   <0xb>   L37   .weight_interpolator chain elem                        L46
0x5a   <0xd>   L38   .time_scale        chain elem                          L47
0x67   <0xb>   L39   .time_sync_group   chain elem                          L48
0x72   <0x4>   L40   bool dead store    bool dummy = false;                 L52
0x76   <0xe>   L41   main lexeme ctor   animation_lexeme main_lexeme(...)   L53
0x84   <0x28>  L48   offset params ctor offset_lexeme_parameters( ...,      L58-64
                                          group!=u32(-1)?&main:NULL, NULL )
0xac   <0xe>   L49   .animated_object   chain elem                          L66
0xba   <0xe>   L50   .playback_type     chain elem                          L67
0xc8   <0x29>  L51   .start_anim_interval_id   chain elem                   L68
0xf1   <0x44>  L52   .start_anim_interval_time chain elem                   L69
0x135  <0x10>  L53   .bones_mask(-3)    chain elem                          L70
0x145  <0x10>  L54   .weight_sync(0x100) chain elem                         L71
0x155  <0x15>  L55   if(!time_driving_animation())  if (...)                L73
0x16a  <0x12>  L56   ASSERT(...)        if-body stmt 1                      L75
0x17c  <0x10>  L58   .time_scale(time_scale) if-body stmt 2                 L76
0x18c  <0x65>  L61   return weapon_lexeme_pair(offset_lexeme, main_lexeme)  L79+L81
0x1f1  <0x6>   L62   epilogue/close     }                                   L82
```

## Per-divergence analysis

- QUANTITY: none. 23 target statements <-> 23 source statements, exact.
- ORDER: none. Definition/statement order in the source follows the target line
  order L23,L32..L41,L48..L56,L58,L61,L62 exactly. This is a single free function,
  so no member/access-specifier reordering applies.
- CONTROL STRUCTURE: matches.
  - The `if` at L55 branches `jne short .1` (0x168) over offsets 0x16a-0x18b, with
    label `.1` at 0x18c. Body = exactly two statements (L56 ASSERT, L58 time_scale).
    The source braces them: `if (...) { ASSERT(...); .time_scale(...); }` (file
    L73-77). Correct - 2-statement body wants braces. No spurious `+0x002`
    scope-exit jmp at the block close (the only jmp is the guard's `jne`), so the
    bracing is the right shape.
  - The two fluent setter chains (`main_lexeme_parameters.X().Y()...;` and
    `offset_lexeme_parameters.X().Y()...;`) each emit one statement per `.setter()`
    call (attributed to its own source line), reproducing target L33-L39 (7) and
    L49-L54 (6). This is the correct source shape - these are returned-reference
    setters on a local, NOT a ctor member-init list, so there is no
    member-init-list-vs-body-assignment choice to make.
  - L40 `bool dummy = false;` reproduces the lone `<0x4>` 4-byte `mov byte,0` dead
    store (no following lea/call). Correctly NOT written as an ASSERT (an ASSERT
    emits lea+call). Confirmed by target asm at 0x72.
  - The offset-ctor 4th arg is the `group != u32(-1) ? &main_lexeme : NULL` select
    (target 0x84-0x98 `sub/neg/sbb/and` idiom), written as a ternary in source. One
    statement (L48), matching.
  - `return weapon_lexeme_pair( offset_lexeme, main_lexeme )` is the single L61
    statement (0x65) covering build-offset-lexeme + the two member copies
    (pair@0x00 <- offset, pair@0x84 <- main). Matching.
- SIZE-shape: the only divergence is the LTCG inline-vs-call residual described in
  the Verdict (foreign-module inline COMDAT setters inlined by /GL vs out-of-line
  in target). This is NOT attributable to the source structure of this unit; it is
  not a statement the matcher can restructure away from weapon_lexeme_pair.cpp.

## report.json (context only)

The symbol entry in the shared report.json reads `fuzzy_match_percent` ~54.6%, but
this scores the STALE TRUNK STUB against the target (old build), NOT this PR. The
PR is unbuilt; its own outcome is `None` (unpaired) per the matcher's notes. The
report number is not informative for this PR and is recorded here only for context
- it is neither a high-%-over-wrong-structure trap nor a meaningful score for the
reconstructed body.

## Action taken

None beyond writing this report. The function is already labeled
`STATE[None|PARTIAL]` (not a banked/DONE tag), the carcass is correctly preserved
verbatim, and the structure is a faithful match - nothing to downgrade or restore.
A genuine resolution of the residual would require moving the `animation`-module
setters out-of-line engine-wide, which is out of scope for this unit (matcher's
call, not the structure verifier's).
