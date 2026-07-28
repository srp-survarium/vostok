# Capture a reused member-array slot as a `T& ref = arr[idx];` so /Ox hoists its address to the prologue (matches a target that holds the slot address in a register)

tags: cpp:local cpp:member | asm:lea asm:mov | topic:pdb-locals topic:convention
symptoms: the target computes `lea reg, [this + idx*4 + arr_off]` ONCE in the prologue and reuses `reg` for every use of `arr[idx]` (an if-test `[reg]`, a deref `[reg+member_off]`); the base RE-READS the index member at each use (`mov eax,[this+idx_off]; ...[this+eax*4+arr_off]`), shifting statement boundaries; structure count off by one, the first statement is TRGT_ONLY
confidence: 8/10
variants: locals-are-structure.md

When a function references the SAME `m_arr[ m_index ]` slot more than once (the array element
plus the index member are both `this`-relative) and the target holds the slot ADDRESS in a
register across the body, the source captured a REFERENCE to that slot. Writing the element
expression literally twice makes /Ox re-read the index member at each use (no CSE across the
two statements at /Od-ish boundaries), so the slot-address hoist to the prologue never happens
and the statement structure diverges (an extra TRGT_ONLY prologue statement, off-by-one count).

Fix: bind a reference at the top and use it:

```cpp
// base re-reads m_trap_state at each use, structure 12/13, 44%:
if ( m_models[ m_trap_state ] )                 add_model( ..., m_models[ m_trap_state ]->m_render_model, ... );
// reference hoists &m_models[m_trap_state] to the prologue (matches target), 13/13, +30%:
render::static_model_ptr& new_model = m_models[ m_trap_state ];
...
if ( new_model )                                add_model( ..., new_model->m_render_model, ... );
```

The reference is the named local the original used; this build is /Ox so it is elided from the
PDB (the recorded local set is unchanged), but the address-CSE/prologue-hoist it forces is what
pairs the bytes. Use this when the same `this`-relative array slot recurs and the target keeps
its address live - NOT a generic "add a local for the %" (that violates locals-are-structure).

Evidence: booby_trap::on_new_state 44.3% -> 74.7%, structure 12/13 -> 13/13
(game/booby_trap.cpp); `render::static_model_ptr& new_model = m_models[m_trap_state]`.
