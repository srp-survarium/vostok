# Review TODOs (`sushi@TODO:`)

Open matching questions sushi raised in PR review, recorded in-source as `sushi@TODO:`
markers and indexed here so a later agent can sweep them in one pass.

- **Live set** (source of truth): `grep -rn "sushi@TODO:" sources/` — the markers travel with
  the code, so this is always current even if the table below drifts.
- **This table** is the curated index: one row per marker, with the function and the PR it came
  from. Append a row whenever you add a `sushi@TODO:` to the source; tick `Done` (and drop the
  marker) when the matching question is resolved.

Append-only ledger (union-merged across branches, like `PROGRESS.md` / `unanswered_questions.md`).

| Status | Function | File:line | PR | TODO |
|--------|----------|-----------|----|------|
| not done | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` | #155 | Function is **not matched-done**: `STATE[None]` with no STRUCTURE DIFF — objdiff can't pair it (the LTCG inline-vs-call of the `animation_lexeme_parameters` setters shortens the body past the pairing threshold). Needs structure recovery before it earns a real % / embed. |
| open | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (`.bones_mask( 2 )`) | #155 | Is `2` a bare magic constant or a named value defined somewhere? |
| open | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (the `bool dummy` dead store) | #155 | Unlikely a `bool dummy` — if it were, `dummy` would appear in the locals (it does not). The lone 4-byte dead store needs further matching to recover the compiled-out structure (would let objdiff pair it: currently `STATE[None]`). |
| investigate | `legs_ik_processor::process` | `game_core/sources/legs_ik_processor.cpp` (the `process()` STRUCTURE DIFF, `ONLY base` rows) | #159 | The 3 `ONLY base` rows (recomputed `hip_obj_matrix` + two `process_leg` calls) are statements **we** emit that target doesn't align here. Labeling them an "aligner swap" is suspect: if a stmt is only in base we can't know the target stmt is really there — likely a real **structure** problem (an extra/duplicated branch), not a diff-aligner artifact. Investigate before trusting the %. |
| investigate | `s_ik_*_cc` dynamic initializers | `game_core/sources/legs_ik_processor.cpp` | #159 | Four compiler-generated dynamic-initializer (+ atexit destructor) thunks marked `STATE[STUB]`. Unscored (`None`, name-pairing artifact). Try to match them so a mis-written static init would surface. |
| structure-wrong | `weapon_core_reload_state_base::initialize` | `game_core/sources/weapon_core_reload_state_base.cpp` | #167 | sushi: **the structure is wrong** here — disputes the verifier's "STRUCTURE MATCH" verdict. The `L31 ONLY-target` row is more than just `round_is_chambered` inline-vs-call. Deal with it later (also depends on `round_is_chambered` actually emitting out-of-line — currently still inlined, so #167 is 92%\|INPROGRESS not DONE). |
| structure-wrong | `udp_network_flow_emulator::~dtor` | `network_core/sources/udp_network_flow_emulator.cpp:58` | #269 | sushi: **structure is wrong** - disputes the matcher's "4/4 aligns, 58%". The while-condition (`m_delayed_packets.begin() != end()`) codegen divergence is a real shape miss, not just /Od regalloc. Likely a different loop form (cached-end / for vs the begin!=end while). Revisit before trusting 58%. |
| investigate | `udp_network_flow_emulator::make_packet_lost` | `network_core/sources/udp_network_flow_emulator.cpp:175` | #269 | sushi: **100% without structure** - objdiff pairs it at 100% but no structure-diff backs it (trivial shape: 2 unreferenced-param eaters + one `is_low_level_packet` call). The 100% is not structurally verified; confirm the body shape vs target. |
| coverage-gap | `delayed_packets_predicate` (class + ctor) | `network_core/sources/udp_network_flow_emulator.cpp:16` | #269 | sushi: size verifier **missed this class** (and its ctor) - it swept only headers, so .cpp-local classes are skipped. FIXED: added `STATIC_SIZE_ASSERT(.., 0x8)`. Confirmed it's the ONLY .cpp-defined class in network_core (no others). ctor/operator() emit no standalone symbol until tick's remove_if instantiates them. |
| investigate | `get_ip_address` | `network_core/sources/network_core_entry_point.cpp:49` | #269 (network_core carcass) | 99.01%, residual is a **DEAD `jmp short` at 0x216** the target emits as the if's closing-brace (L148) breakpoint anchor (label resolution: if-false target is .7=0x218, loop-exit .8=0x21d, 0x216 preceded by unconditional `0x211 jmp .11` -> unreachable; sushi confirmed). The "closing `}` carries a breakpoint" artifact - MSVC emits a no-op jmp so `}` is breakpointable; our build omits it. The 2 bytes shift every later offset = whole residual. NOT ICF naming (earlier verdict wrong), NOT brace/whitespace (confirmed cosmetic). **Open:** what source/flag makes MSVC emit the dead closing-brace anchor here - the general "loop closing-brace breakpoint" lever, worth solving once (recurs across the codebase). |
| investigate | `custom_alloc_handler<H>::operator()` | `network_core/custom_alloc_handler.h:18` | #269 (network_core carcass) | matcher 1 wrote `operator()` **inline**, but target emits **6 real out-of-line symbols** (`vostok::network_core::custom_alloc_handler<..>::operator()`) — currently **unmatched/unscored**. The structure generator put only the 4 *layout* records in this header's carcass (no `operator()` body), so the 6 must be recovered from `binaries/rich/target/index.jsonl` directly. **Open question:** why did the generator omit the `operator()` carcasses despite the symbols existing? (template-member filtering quirk?) — resolve before relying on this header as "complete". NOTE: `make_custom_alloc_handler` and the `asio_handler_allocate/deallocate/invoke` friends fully inline in target (0 vostok symbols both sides; the 32 `asio_handler_allocate` hits are all `boost::asio::` default) → consistent as written, no action. |
| audit | all `ASSERT_U` / `ASSERT_CMP_U` (27 across 11 files) | `game_core/sources/*.cpp`, `animation/sources/*.cpp` | #164 | **Re-audit: many of our `ASSERT_U(x)` are probably `VOSTOK_UNREFERENCED_PARAMETER(S)(x)`.** Both emit the same `if(identity(false)){ folded_empty(args) }` eater shape; distinguish by whether `x` is an *asserted expression* (ASSERT_U) or the function's own *otherwise-unused parameter* (unreferenced macro). Prime suspect: `ASSERT_U( weight_driving_animation )` recurs in 6 weapon-state `weapon_and_hands_expression` files where that param is used nowhere else. Per-function: pull the target asm, check what the eater pushes, swap mislabeled ones to `VOSTOK_UNREFERENCED_PARAMETER(S)`, rebuild and require `regressed: []`. See assembly_patterns.md (the ASSERT_U + unreferenced-parameter entries). |

## Provisional idiom candidates (NOT definitive — DO NOT promote to `assembly_patterns.md` yet)

Both were drafted from `game_core/get_weapon_lexeme_pair_impl`, which is **unverified** (`STATE[None]`
— it doesn't pair, so the whole-function match can't be confirmed). They differ in confidence:
**#1 is likely right** — its asm reproduces byte-for-byte at that single arg site, which is checkable
locally regardless of the function pairing — while **#2 (the `bool dummy`) is the one questioned in
review** above. Kept here (not in `assembly_patterns.md`) so the future matcher has both in hand;
promote #1 once any paired function exercises it, and resolve #2 with the further matching it needs.

**1. Conditional-pointer arg `val != u32(-1) ? &obj : NULL`**  *(likely right — reproduces byte-for-byte at the site)*
```
push 0                  ; (an unrelated arg pushed first)
mov  edx, [ebp+20h]     ; val (a u32)
sub  edx, 0FFFFFFFFh    ; edx - (-1) == edx + 1
neg  edx                ; CF set iff edx+1 != 0  (i.e. val != -1)
sbb  edx, edx           ; edx = (val != -1) ? 0xFFFFFFFF : 0
lea  eax, [ebp-0E0h]    ; &obj
and  edx, eax           ; (val != -1) ? &obj : NULL
push edx
```
Source: write the ternary `val != u32(-1) ? &obj : NULL` (a pointer-or-NULL select on an unsigned
`!= -1` test) — NOT hand-rolled asm. MSVC /Od lowers `!= u32(-1)` to `sub r,-1; neg; sbb r,r` (mask
= all-ones iff non-equal) and ANDs it with the address. Candidate site: the offset lexeme's
`time_driving_animation` arg = `time_synchronization_group != u32(-1) ? &main_lexeme : NULL`.

**2. Lone 4-byte `mov byte[ebp-N],0` (no lea/call) = unused `bool b = false;`, not an ASSERT**
A structure statement of size `<0x4>` whose only instruction is `mov byte ptr [ebp-N],0` with NO
following `lea eax,[ebp-N]; call <empty_stub>` is a plain unused `bool` local dead-stored under /Od.
DISTINGUISH from a compiled-out `ASSERT` (`<0xc>` = the byte-store PLUS `lea+call`). Writing
`ASSERT(UNKNOWN_EXPRESSION)` for a `<0x4>` slot over-produces the lea+call. The 4-vs-7-byte disp is
just small-vs-large `[ebp-N]` offset (allocation noise). Candidate site: `get_weapon_lexeme_pair_impl`
L40, target `<0x4>` `mov byte[ebp-5],0`. **Caveat:** only valid if the surrounding match is correct
— here it is not yet (see the `not done` row).
