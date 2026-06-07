# Structure audit: inventory_item::inventory_item(action_behaviour_type)

- target rva: 0x590870   base rva: 0x449c20
- mangled: ??0inventory_item@survarium@@QAE@W4action_behaviour_type@01@@Z
- size: 111 bytes (0x6f)
- report.json fuzzy_match_percent: 78.8

## Verdict

STRUCTURE MATCH (0 quantity diffs, 0 size diffs).

The 78.8% is LOW while the structure is CORRECT - this is exactly the trap the
structure verifier exists to separate from a real divergence. The gap is an
LTCG inline-vs-call artifact of the base-class constructor, NOT a defect in this
ctor's source shape.

## Condensed structure-diff

    target: 0x590870            base: 0x449c20
    ; survarium::inventory_item::inventory_item(survarium::inventory_item::action_behaviour_type) ; target 0 stmts / base 0 stmts
    ; aligned 0, size-diffs 0, quantity-diffs 0

Both sides are 0-statement: the ctor body is empty `{}`, all work is in the
member-initializer list and is attributed to the decl line (collapsed). There
are no per-statement rows to diverge.

## Skeletons

Target: 0 statements, 0x6f bytes - member-init only.
Base:   0 statements, 0x6f bytes - member-init only.

Member-init order and field writes are identical on both sides (from --view diff):

    +0x108  m_action_behaviuor = type      (from [ebp+8])
    +0x10c  m_inventory        = 0
    +0x110  m_slot_id          = 0x13      (max_slots_count)
    +0x114  m_amount           = 0         (u16)
    +0x116  m_dict_id          = 0         (u16)

Same offsets, same values, same order - the member-init list in the source
(`m_action_behaviuor(type), m_inventory(NULL), m_slot_id(max_slots_count),
m_amount(0), m_dict_id(0)`) reproduces the target field-init shape exactly.

## Where the 78.8% comes from (NOT a structure diff)

From `--view diff`, the only divergence is how the `interactive_object` base
subobject is constructed:

- TARGET: `mov esi,[ebp-4]; call survarium::interactive_object::interactive_object`
  - the base ctor is emitted as a CALL.
- BASE: `push 1; mov ecx,[ebp-4]; call unmanaged_resource::unmanaged_resource;
  mov [eax],??_7interactive_object@survarium@@6B@`
  - the body of `interactive_object::interactive_object` was INLINED here
  (its `unmanaged_resource(1)` base init + its own vtable store).

This inline-vs-call decision is a property of how `interactive_object::
interactive_object` is defined/visible to the compiler under LTCG, not of this
ctor's statement structure. It also shifts a few register choices (esi vs ecx
to hold `this`, an extra push/pop esi) - register/stack noise, not structure.

## Source fix

None required for STRUCTURE. The member-init list is already correct and in
order. The remaining bytes need the base-class ctor `interactive_object::
interactive_object` to be emitted as a call (or matched as inlined) - a
base-ctor codegen matter to be chased separately, NOT a restructure of this
function. A rebuild would be needed to confirm any such base-ctor change, which
is outside this audit's scope.

## STATE

Left as PARTIAL (78.8%). STATE note updated to make clear the member-init shape
matches and the gap is the LTCG base-ctor inline (was already roughly noted).
No downgrade needed (it was not mislabeled DONE); no carcass to restore (empty
body ctor). The condensed structure-diff is embedded in the .cpp above the
definition.
