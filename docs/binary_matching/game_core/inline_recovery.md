<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Game-core inline-body audit

The PR 569 scope contains one no-source game-core inline:
`jump_logic::get_animated_object`. The retail PDB declares it, but emits no
procedure and no shipped source calls it. `m_animated_object` is initialized to
zero and otherwise unused by the surviving `jump_logic` implementation, so a
direct-field return would be plausible but untestable. The declaration remains
`STATE[UNMATCHABLE]` until a consuming target or original source establishes
the body.
