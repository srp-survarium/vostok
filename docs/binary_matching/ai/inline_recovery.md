<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# AI missing-body audit

The client PDB declares `fsm::remove_state` and `fsm::remove_transition` as
public inline methods, immediately after their corresponding add methods. It
does not emit either procedure. No target or reconstructed client function
uses either name, and no body exists in repository history. Their list-erasure
semantics are plausible from the surrounding API, but are not recoverable from
this target; both declarations remain `STATE[UNMATCHABLE]` rather than gaining
invented bodies.

The same retail class record starts with the nested name
`fsm::states_type : fsm_state_list`. Restoring that typedef at declaration
position zero also restores the class's `contains nested type` property. The
alias is used by both `states` overloads and `m_states`, which preserves the
resolved types recorded by the PDB. The remaining `struct fsm` forward
declaration in `jump_logic.h` was changed to `class fsm`; the rebuilt PDB now
contains one topology-identical class record instead of two duplicate semantic
records.

`ai_navigation` contains no `/* no source */` declarations at this audit
baseline, and all fourteen of its tracked procedures are byte- and
structure-matched.
