# game_core::inventory_item::inventory_item(action_behaviour_type)

Constructor at target rva `0x590870`, base rva `0x4472c0`.
File: `sources/vostok/game_core/sources/inventory_item.cpp:13`.

## Target asm (pdb_fetch --view target, rva 0x590870)

```
0x00:    push  ebp
0x01:    mov   ebp, esp
0x03:    push  ecx
0x04:    push  esi
0x05:    mov   [ebp-4], ecx
0x08:    mov   esi, [ebp-4]
0x0b:    call  survarium::interactive_object::interactive_object   ; <-- out-of-line base ctor, NOT inlined
0x10:    mov   ecx, [ebp-4]
0x13:    add   ecx, 108h
0x19:    call  stlp_std::allocator<char>::allocator<char>          ; allocator at +0x108 (stateless)
0x1e:    mov   eax, [ebp-4]
0x21:    mov   dword ptr [eax], ??_7inventory_item vtable
0x27:    mov   ecx, [ebp-4]
0x2a:    mov   edx, [ebp+8]
0x2d:    mov   [ecx+108h], edx          ; m_action_behaviuor = type
0x33:    mov   eax, [ebp-4]
0x36:    mov   dword ptr [eax+10Ch], 0  ; m_inventory = NULL
0x40:    mov   ecx, [ebp-4]
0x43:    mov   dword ptr [ecx+110h], 13h; m_slot_id = max_slots_count (0x13)
0x4d:    xor   edx, edx
0x4f:    mov   eax, [ebp-4]
0x52:    mov   [eax+114h], dx           ; m_amount = 0 (u16)
0x59:    xor   ecx, ecx
0x5b:    mov   edx, [ebp-4]
0x5e:    mov   [edx+116h], cx           ; m_dict_id = 0 (u16)
0x65:    mov   eax, [ebp-4]
0x68:    pop   esi
0x69:    mov   esp, ebp
0x6b:    pop   ebp
0x6c:    ret   4
```

Member offsets confirmed from `inventory_item.h`: m_action_behaviuor +0x108,
m_inventory +0x10c, m_slot_id +0x110, m_amount +0x114 (u16), m_dict_id +0x116 (u16).
`max_slots_count = 0x13` from `profile_slot_enum.h`.

Target callees: `interactive_object::interactive_object()` (no args) +
`stlp_std::allocator<char>::allocator<char>()`.

## Commands run (verbatim, in order)

- `pdb_rich_query --index binaries/rich/target/index.jsonl --function inventory_item --list`
  -> ctor at `0x590870`.
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --function inventory_item --rva 0x590870 --view target`
- `pdb_fetch ... --rva 0x590870 --view callees` -> interactive_object::interactive_object() + allocator<char>().
- `pdb_fetch ... --rva 0x590870 --view info` -> 0 locals.
- `pdb_rich_query --index binaries/rich/base/index.jsonl --function inventory_item --list`
  -> ctor already present in base at `0x4472c0` (so already reachable).
- `pdb_fetch --target-index ... --base-index binaries/rich/base/index.jsonl --function inventory_item --rva 0x590870 --view diff --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target`
  -> objdiff fuzzy match 78.73%.
- `pdb_fetch --base-index ... --rva 0x4472c0 --view callees`
  -> base calls `unmanaged_resource::unmanaged_resource(unsigned int)` (inlined base ctor).
- `pdb_fetch --target-index ... --function interactive_object --rva 0x9ccb0 --view target`
  -> `push 1; call unmanaged_resource(uint); mov [esi], vtable; ret` = the out-of-line
  interactive_object default ctor that inventory_item's ctor `call`s.
- Reachability: `rg inventory_item temp_include_all.cpp` + grep `oxygen_tank.cpp:15`
  `inventory_item( use_silent )` -> oxygen_tank ctor anchors the base ctor; already alive.
- `report.json`: function `??0inventory_item@survarium@@QAE@W4action_behaviour_type@01@@Z`
  = 78.8%.

## Iterations

1. INPUT: pre-existing STUB body (correct member-init list already present):
   `m_action_behaviuor(type), m_inventory(NULL), m_slot_id(max_slots_count),
    m_amount(0), m_dict_id(0)`.
   BUILD: 78.8% (report.json) / 78.73% (operand-aware diff). No rebuild needed - this
   is the already-compiled base object.
   DIFF: the only non-LTCG divergence:
     - target: `push esi; mov esi,[ebp-4]; call interactive_object::interactive_object`
     - base:   `push 1; mov ecx,[ebp-4]; call unmanaged_resource(uint); mov [eax],0`
   i.e. base **inlines** the trivial base-class init; target **calls** an out-of-line
   `interactive_object::interactive_object()`. All other `~` rows are pure ecx/eax/edx
   register reassignment + the `push esi`/`pop esi` pair = LTCG/frame noise.

No source edit to `inventory_item.cpp` can change this: the inlining decision is
governed by `interactive_object`'s declaration (no user-declared ctor -> trivial base
init inlined), not by this constructor's body. The member-init body is already byte-exact.

## Outcome

STATE[78.8%|PARTIAL]: member-init body byte-exact. Remaining divergence is the
interactive_object base ctor being inlined in base vs `call`ed in target. Fixing it
requires giving `interactive_object` a user-declared out-of-line default constructor
(a separate function = separate unit of work, and a wide blast radius across every
interactive_object-derived ctor, e.g. oxygen_tank/weapon_core - high regression risk).
Out of scope for this leaf.

Regressions caused: none (comment-only edit; compiled bytes unchanged).
Inlining: interactive_object::interactive_object() inlined into base ctor (target keeps
it out-of-line at rva 0x9ccb0); not a cluster I can match from this function's source.
