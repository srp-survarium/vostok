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
| `ai` | 2 | 0 | [AI](ai/inline_recovery.md) |
| `animation` | 30 | 0 | [animation](animation/inline_recovery.md) |
| `game_core` | 1 | 0 | [game_core](game_core/inline_recovery.md) |
| `network` | 0 | 0 | [network](network/inline_recovery.md) |
| `network_core` | 0 | 0 | [network_core](network_core/inline_recovery.md) |
| `physics` | 21 | 3 | [physics](physics/inline_recovery.md) |
| `render` | 7 | 0 | [render](render/inline_recovery.md) |
| **Total** | **61** | **3** | |

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

The remaining 61 bodies outside network divide into three evidence classes
documented in the module reports:

1. dedicated-server or editor-only code absent from `survarium.exe`;
2. declarations with no source consumer and no retained procedure;
3. plausible helper names near live logic whose target caller partitions prove
   the logic was retained at the current ownership boundary.

None is treated as an intentionally empty implementation. `STATE[UNMATCHABLE]`
means this client cannot establish the body; original source or another binary
that consumes the declaration is required.

No build was run for the network follow-up, at the user's explicit request.
