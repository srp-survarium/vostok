# `create_sound_propagator` source MAX

`sound_scene::create_sound_propagator` is source- and size-matched at six
statements and `0x24d` bytes. Its measured source maximum is 91.00641%.

The remaining instruction diff is not representable by one coherent
`sound_scene` layout:

- `sound_scene.cpp` target code reads `m_list_position` at `sound_scene+0x120`.
- `sound_scene_propagators.cpp` target code for this function reads the same
  logical listener position twice at `sound_scene+0x128`.
- The base uses the canonical `+0x120` layout in both translation units.

Changing the member layout to reproduce this TU's `+0x128` operands would
break the already-recovered scene functions. The other residual names are ICF
fold choices (`boost::function::clear` and allocator helper folds), not source
semantics.

Evidence commands:

```text
pdb_fetch --function sound_scene::create_sound_propagator --view structure-diff
pdb_fetch --function sound_scene::create_sound_propagator --view diff
pdb_fetch --function sound_scene::notify_listener --view target
```
