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
| sushi-confirm | `udp_network_flow_emulator::~dtor` | `network_core/sources/udp_network_flow_emulator.cpp` | #269 | **Worker DISPUTES sushi's "structure is wrong" — left OPEN for sushi's sign-off before closing.** Worker's case (structure CORRECT, not wrong): structure-diff is 4/4 aligned, 0 quantity-diffs, 2 SIZE-diffs. Tried `!empty()`, `!(begin()==end())` and swapped operands - ALL fold to the identical base body (raw-pointer iterators -> builtin `cmp`/`je`), so it is NOT a different loop form. The real residual: TARGET resolves the iterator `!=` through a bool-returning operator (`cmp begin,[this+4]; sete; movzx; test; jne`, end read inline, no temp) while BASE inlines raw-pointer `!=` (begin+end cached to stack temps, direct `je`). Same source, divergent STLport iterator-compare lowering - env/config, unsteerable from C++. 58.46%. |
| done | `udp_network_flow_emulator::make_packet_lost` | `network_core/sources/udp_network_flow_emulator.cpp` | #269 | RESOLVED (100% structurally VERIFIED): structure-diff is 1/1 aligned, 0 size-diffs, 0 quantity-diffs; target and base are byte-identical. The `mov [ebp-1],al` IS the `is_low_level_packet` bool eater; `endpoint` ref-param needs no eater (`ret 0Ch` cleans all 3 args). The 100% is real. |
| coverage-gap | `delayed_packets_predicate` (class + ctor) | `network_core/sources/udp_network_flow_emulator.cpp:16` | #269 | sushi: size verifier **missed this class** (and its ctor) - it swept only headers, so .cpp-local classes are skipped. FIXED: added `STATIC_SIZE_ASSERT(.., 0x8)`. Confirmed it's the ONLY .cpp-defined class in network_core (no others). ctor/operator() emit no standalone symbol until tick's remove_if instantiates them. |
| investigate | `get_ip_address` | `network_core/sources/network_core_entry_point.cpp:49` | #269 (network_core carcass) | 99.01%, residual is a **DEAD `jmp short` at 0x216** the target emits as the if's closing-brace (L148) breakpoint anchor (label resolution: if-false target is .7=0x218, loop-exit .8=0x21d, 0x216 preceded by unconditional `0x211 jmp .11` -> unreachable; sushi confirmed). The "closing `}` carries a breakpoint" artifact - MSVC emits a no-op jmp so `}` is breakpointable; our build omits it. The 2 bytes shift every later offset = whole residual. NOT ICF naming (earlier verdict wrong), NOT brace/whitespace (confirmed cosmetic). **Open:** what source/flag makes MSVC emit the dead closing-brace anchor here - the general "loop closing-brace breakpoint" lever, worth solving once (recurs across the codebase). |
| done | `custom_alloc_handler<H>::operator()` | `network_core/custom_alloc_handler.h` | #269 (network_core carcass) | RESOLVED: the "6 out-of-line operator() symbols" claim is **false**. Exhaustive scan of `binaries/rich/{target,base}/index.jsonl` for mangled `??$?R...`/`??R...` whose leftmost class token is `?$custom_alloc_handler@` returns **0** on BOTH sides — `operator()` always inlines into the asio `read_op`/`write_op` that invokes the handler (those op symbols carry the body and are matched). The generator omitted no carcass because there is no out-of-line operator() to dump. Keeping `operator()` `inline` is correct; an out-of-line form would emit symbols the target lacks and HURT the score. Only emitted custom_alloc_handler member is the ctor (size 25, identical both sides). No action; comment in source updated to record this. |
| audit | all `ASSERT_U` / `ASSERT_CMP_U` (27 across 11 files) | `game_core/sources/*.cpp`, `animation/sources/*.cpp` | #164 | **Re-audit: many of our `ASSERT_U(x)` are probably `VOSTOK_UNREFERENCED_PARAMETER(S)(x)`.** Both emit the same `if(identity(false)){ folded_empty(args) }` eater shape; distinguish by whether `x` is an *asserted expression* (ASSERT_U) or the function's own *otherwise-unused parameter* (unreferenced macro). Prime suspect: `ASSERT_U( weight_driving_animation )` recurs in 6 weapon-state `weapon_and_hands_expression` files where that param is used nowhere else. Per-function: pull the target asm, check what the eater pushes, swap mislabeled ones to `VOSTOK_UNREFERENCED_PARAMETER(S)`, rebuild and require `regressed: []`. See assembly_patterns.md (the ASSERT_U + unreferenced-parameter entries). |
| open | `udp_match_items_stats`/`udp_match_stream_stats`/`udp_match_stats` no-source members (`reset`/`operator+=`/`operator/=`/`dump`) | `network_core/udp_match_stats.h:11` | #285 | sushi: shouldn't these no-source inlines be used by the implemented code below them? No inline-site evidence yet (no matched consumer accumulates/dumps stats), so implementing them now would be fabrication; when a consumer gets matched, reconstruct them from its bytes and revisit whether the implemented ctors / free `operator>=`/`operator-` should route through them. |
| open | `udp_match_connection` sham inline bodies | `network_core/udp_match_connection.h:110` | #283 | sushi: the remaining inline bodies (`has_disconnection_initiated`, `is_disconnecting`, `new_packet`, `are_there_any_queued_packets`, `last_send/receive/activity_time_in_ms`, `pending_operations_count`, `set_disconnected`, `set_max_packet_wait_time_in_ms`, `disconnect_impl`, `channel::reset`) are shams to compile - fix or mark. MARKED `/* no source */` for now (no compiled consumer references any of them yet, so no inline-site bytes to reconstruct from; `is_connected`/`delete_packet` get real bodies in PR #285, the `comparer` in PR #288). `channel::reset` HAS an exact out-of-line target body (RVA 0x1274b0: assert eater, `packets.clear()`, `received_order_id = 0xffff`, `sent_order_id = 0`) - carcass moved onto `reset` (it was misfiled under `~channel`, which target shows as 0 statements); write the body when a caller instantiates it so bytes can verify. Reconstruct the rest from consumers' target bytes as they get matched. |
| open | `udp_match_packet::reallocate` | `network_core/udp_match_packet.h:94` | #288 | Body unrecovered: the out-of-line `packet<udp_match_packet>::reallocate` (0x112e20) is a never-called `int3` stub, and the LTCG `append(pcvoid,u32)` COMDAT's grow path ends in an ICF-folded no-arg call (an eater/abort macro?) that cannot be disambiguated from the emission. `allocated_size` WAS recovered from the same COMDAT (`100h - movzx(header_size())` -> `sizeof( m_buffer ) - header_size( )`); `~udp_match_packet`/`helper::call_constructor` have no target symbol or inline site - left `/* no source */`. |
| done | `udp_match_connection::m_last_receive_time_in_ms` (atomic32 member) | `network_core/udp_match_connection.h:202` | #288 | RESOLVED: pdb-parser now renders cv-qualifiers (LF_MODIFIER volatile + pointer-attribute volatile) in the generated structure headers. A full-tree sweep against the regenerated target structure confirms the PDB records `volatile long` for `m_last_receive_time_in_ms` (the inference was right) and ALSO for `m_last_send_attempt_time_in_ms` (was plain `long` in ours - fixed to `threading::atomic32_type`). game_core has zero volatile members in the PDB; network_core's only other one (`udp_match_packets_allocator::m_reference_count`) was already atomic32_type. |

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
