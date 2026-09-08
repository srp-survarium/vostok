<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Game-core inline-body audit

The PR 569 scope contains one no-source game-core inline:
`jump_logic::get_animated_object`. The retail PDB declares it, but emits no
procedure and no shipped source calls it. `m_animated_object` is initialized to
zero and otherwise unused by the surviving `jump_logic` implementation. The
getter now returns that field, explicitly classified as a semantic model in
[row G01](../pr569_semantic_reconstruction.md), not as a measured retail body.
No build was run for this follow-up.
