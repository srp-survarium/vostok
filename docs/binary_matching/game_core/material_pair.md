# material_pair.cpp - structure verification (item/material batch)

Rebuild-fresh report.json percents (markers were stale):

| fn | old marker | report.json | structure | wall |
|---|---|---|---|---|
| material_pair() ctor | 99.29% DONE | 95.07% | MATCH 0/0 | m_particles vectora ctor inline-vs-call (LTCG) |
| load_from_config | 99.51% DONE | 100% | MATCH 6/6 | none - now 100% |
| add_particle | 100% DONE | 92.86% | MATCH 1 SIZE | resource_ptr arg copy-ctor out-of-line in target vs inlined in base |
| particle | 97.14% PARTIAL | 97.14% | MATCH 1 SIZE | m_particles operator[] out-of-line call in target vs inlined lea in base |

## ctor (95.07%)
structure-diff: target 0 / base 0 stmts, 0 size-diffs, 0 quantity-diffs.
Residual is the `m_particles( g_allocator )` vectora ctor: target emits a real
`call`, base inlines. Same vectora inline-vs-call LTCG class as scheduler::on_frame.

## add_particle (92.86%)
`m_particles.push_back( particle )` - single SIZE diff. The `particle` parameter is a
`resource_ptr` passed by value; the target copies it into a temp via an out-of-line
intrusive_ptr copy-ctor (`+ call 0x111`) that base inlines. Non-steerable resource_ptr
by-value LTCG.

## particle (97.14%)
`return m_particles[m_current_particle_idx++]` - single SIZE diff. The target's
`m_particles[idx]` emits a real `call` to vectora::operator[] (sushi's "useless call at
the end"); base inlines it to `lea eax,[edx+ecx*4]`. Non-steerable vectora operator[]
inline-vs-call LTCG.

## load_from_config: now 100%
6/6 stmts, byte-aligned. Marker bumped 99.51% -> 100%.

VERDICT: all STRUCTURE MATCH. Comment-only edits (marker sync + embeds + carcass strip).
No logic change.
