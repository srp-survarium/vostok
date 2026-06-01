# game_core::get_bone_matrix_in_object_space{,_impl}

Two related free functions in `namespace survarium` (file `ik_processor.cpp`),
matched as ONE unit (shared scaffolding). The wrapper does a real `call` to the
impl in the target (NOT inlined), so this is a same-file pair, not an inlined
cluster.

## Target asm

### _impl (rva 0x57ebd0)
Args (with hidden float4x4 return ptr): [ebp+8]=ret, [ebp+0Ch]=bone&,
[ebp+10h]=matrices, [ebp+14h]=first_non_root_bone.

```
0x0b: mov eax,[ebp+0Ch]; sub eax,[ebp+14h]; cdq; mov ecx,14h; idiv ecx; mov [ebp-4],eax
        ; matrix_index = u32(&bone - first_non_root_bone)   (sizeof skeleton_bone = 0x14)
0x1c: mov edx,[ebp+0Ch]; mov eax,[edx+4]; mov [ebp-0CCh],eax; cmp 0; je .1
        ; if ( bone.parent() )   (m_parent @ +0x04)
0x31..0x78 (parent != NULL):
        push first_non_root_bone; push matrices; push bone.parent(); lea &tmp; push
        call get_bone_matrix_in_object_space_impl          ; recurse on *parent
        push eax(recursive result); ecx=matrix_index<<6 + matrices; push; lea &res; push
        call vostok::math::operator*                       ; float4x4 op*(float4x4,float4x4)
        ; arg order = ( matrices[matrix_index], recursive_result )
0x7a..0x8c (.1, parent == NULL):
        lea &tmp; call allocator<char>(=default float4x4() ctor); mov ecx,eax; call float4x4::identity
.2: rep movsd 64 bytes into the return slot  -> single `return <ternary>`
```

Source:
```cpp
u32 const matrix_index = u32( &bone - first_non_root_bone );
return bone.parent( ) ?
    matrices[matrix_index] * get_bone_matrix_in_object_space_impl( *bone.parent( ), matrices, first_non_root_bone ) :
    float4x4( ).identity( );
```

### wrapper (rva 0x57ec90)
Args (with hidden ret ptr): [ebp+8]=ret, [ebp+0Ch]=bone&, [ebp+10h]=skeleton&,
[ebp+14h]=matrices.

```
mov eax,[ebp+10h]; call skeleton::get_root      -> esi = &root
mov eax,[ebp+10h]; call skeleton::get_root_bones_count; imul eax,14h; add esi,eax
mov [ebp-4],esi   ; first_non_root_bone = &get_root() + get_root_bones_count()
push first_non_root_bone; push matrices; push bone&; push ret
call get_bone_matrix_in_object_space_impl
```

Source:
```cpp
skeleton_bone const* const first_non_root_bone = &skeleton.get_root() + skeleton.get_root_bones_count();
return get_bone_matrix_in_object_space_impl( bone, matrices, first_non_root_bone );
```

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function get_bone_matrix_in_object_space --list
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_bone_matrix_in_object_space_impl --rva 0x57ebd0 --view target
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_bone_matrix_in_object_space --rva 0x57ec90 --view target
- (callees / info views for both)
- python3 scripts/rebuild.py        # NO module arg

## Iterations
1. INPUT: bodies above + anchor `use_game_core_ik_processor` in temp_include_all.cpp
   (escape wrapper result through example_callback, take impl address).
   BUILD: get_bone_matrix_in_object_space = 100.0%, _impl = 100.0% (report.json).
   Both jumped 0.00% -> 100.00% in report-changes. Helpers skeleton::get_root and
   skeleton::get_root_bones_count also went 0 -> 100 (kept alive via the wrapper).
   No iteration needed - first build matched.

## Outcome
STATE[100%|DONE] for both functions. No blocker. Got both right in one rebuild by
reading the full target asm + member offsets (skeleton_bone size 0x14, m_parent @4,
float4x4 size 0x40) and the operator* push order up front.

Regressions caused: NONE attributable to this change. The 53 report-changes
regressions are stale-baseline / COMDAT artifacts: (a) dozens of `scalar/vector
deleting destructor`, trivial ctors and smart-ptr ctors across particle/bt/render/
ai/vfs that reshuffle on any EXE relink (54 OTHER functions improved the same
build, e.g. hit_initiator dtor, vfs intrusive_ptr ctor); (b) functions living on
OTHER open-PR branches not present on this bare branch (off
feature/agentic-matching-loop): `fixed_string<46>()` 95->0, `fill_new_stats_item`
91.78->0, `dump_state` 17->0, `inventory_item::deserialize_game_world_object`
13.4->0. My git diff only touches ik_processor.cpp + temp_include_all.cpp; none of
the regressed functions is defined there.

Inlining: none relevant - the wrapper does a real `call` to _impl in the target,
so this is a same-file pair sharing scaffolding, NOT an inlined cluster.
