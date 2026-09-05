<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# PR 569 no-source inline audit

This is the coverage index for the PR 569 review. It counts functions, not
comment lines, and covers every `/* no source */` body in the files touched by
the PR. Equivalent operations in a caller are not enough: a body is recovered
only when the target caller, declaration seam, and source ownership agree.

Access legality is a prerequisite, not a cosmetic follow-up. Every relevant
unambiguous PDB field, method, and base-class access is restored before a caller
is accepted as evidence: otherwise an incorrectly public field can let the
reconstruction open-code operations that retail could only express through an
inline member, accessor, or friend. The PR baseline exposed three private or
protected physics fields, one public UDP helper, and seven private
`boost::noncopyable` bases; all are restored in this stack.

| Module | Remaining inline bodies | Additional non-inline declarations | Detailed evidence |
|---|---:|---:|---|
| `ai` | 0 | 0 | [AI](ai/inline_recovery.md) |
| `animation` | 5 | 0 | [animation](animation/inline_recovery.md) |
| `game_core` | 0 | 0 | [game_core](game_core/inline_recovery.md) |
| `network` | 0 | 0 | [network](network/inline_recovery.md) |
| `network_core` | 0 | 0 | [network_core](network_core/inline_recovery.md) |
| `physics` | 5 | 1 | [physics](physics/inline_recovery.md) |
| `render` | 1 | 0 | [render](render/inline_recovery.md) |
| **Total** | **11** | **1** | |

The review found one falsely classified placeholder:
`bullet_character_controller::prevent_max_slope_moving_prestep`. Retail
attributes its complete vertical-velocity update in `player_step` to one source
statement, while the open-coded reconstruction split the same bytes across five
statements. The implementation now lives in the declared inline helper and the
caller retains that seam.

The network/network-core follow-up closed all 74 inert bodies in those modules:
71 network-core bodies from the combined `/* no source */` and fake-return
census, plus three `network::match_client` allocator stubs that carried explicit
`STATE[STUB]` markers. Each now has executable semantics and an individual
evidence disposition in the linked module reports. The dedicated-server web is
explicitly classified as a semantic reconstruction because the client PDB has
type declarations but no server procedures or inline expansions from which to
prove exact source spelling.

The follow-up [individual reconstruction register](pr569_semantic_reconstruction.md)
accounts for all 61 non-network inline bodies and all three additional
declarations from the earlier baseline. It fills 50 inline and two out-of-line
bodies, with historical source, retained caller logic and semantic models kept
distinct. Eleven inline bodies and `can_prone` remain open, each with a proposed
operation, competing interpretations, and the next discriminating evidence.

An absent procedure or header-attributed line range does not prove an absent
inline expansion. The earlier categorical caller-partition veto was too strong;
the extracted serialization, animation update and collision request seams need
the next batched structure comparison.

No build was run for either follow-up, at the user's explicit request. Body
counts are source-work coverage, not new exact-match counts. The four restored
render-world forwarders also have unresolved renderer callees; those remain
explicit dependencies, not completed editor functionality.
