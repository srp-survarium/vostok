# weapon_core_base_state::weapon_core_base_state (ctor) - structure audit

## Verdict (RESOLVED)

STRUCTURE MATCH (clean)

Moving the five body assignments into the member-initializer list collapsed the
body to the target's 0 statements. Rebuilt: structure-diff is now target 0 / base 0,
0 diffs (and bytes stay 100.0%). The "high % over the wrong structure" trap is
resolved - both bytes and shape match.

- target: 0x6ecf90   base: 0x452480
- `survarium::weapon_core_base_state::weapon_core_base_state(survarium::weapon_core&, bool)`
- target 0 stmts / base 0 stmts
- aligned 0, size-diffs 0, quantity-diffs 0

## Structure-diff (condensed)

```
--          | <0>         |    EMPTY only base
--          | 0x070 <0xd> | m_is_firing_ptr				= NULL;   ONLY base
--          | 0x07d <0xd> | m_body_part_mask_for_user	= animation::body_part_whole_body;	// -1   ONLY base
--          | 0x08a <0xa> | m_is_ready_to_be_deactivated	= false;   ONLY base
--          | 0x094 <0xa> | m_animation_has_been_ended	= false;   ONLY base
--          | 0x09e <0xc> | m_serialize_animation_state	= serialize_animation_state;   ONLY base
```

## The two skeletons

Target (0 statements): every member initialization is attributed to the ctor decl
line - the entire init region lives in the member-initializer list, the body is
empty. No body statements emit their own source line.

Base (6 statements): the ctor has a member-init list with only `m_weapon( weapon )`,
and the remaining FIVE members are assigned as separate body statements, each of
which gets its own attributed source line. The diff flags 6 ONLY-base quantity
divergences: the 5 body assignments plus 1 `EMPTY only base` (the collapsed
source-line gap for the body block the target does not have).

## Divergence

QUANTITY divergence, 6 statements. Cause: member-initializer list vs body
assignments. The five members
`m_is_firing_ptr`, `m_body_part_mask_for_user`, `m_is_ready_to_be_deactivated`,
`m_animation_has_been_ended`, `m_serialize_animation_state`
are written as body assignments in our base but are part of the decl-line
member-initializer region in the target. Same bytes, different shape.

## Concrete source fix

Move the five body assignments into the member-initializer list, after
`m_weapon( weapon )`, preserving the header's declaration order (which matches the
assignment order, so no reorder needed):

```cpp
weapon_core_base_state::weapon_core_base_state( weapon_core& weapon, bool serialize_animation_state )
	:	m_weapon						( weapon ),
		m_is_firing_ptr					( NULL ),
		m_body_part_mask_for_user		( animation::body_part_whole_body ),	// -1
		m_is_ready_to_be_deactivated	( false ),
		m_animation_has_been_ended		( false ),
		m_serialize_animation_state		( serialize_animation_state )
{
}
```

Header member order (weapon_core_base_state.h, offsets 0x0128..0x0136):
`m_weapon, m_is_firing_ptr, m_body_part_mask_for_user,
m_is_ready_to_be_deactivated, m_animation_has_been_ended,
m_serialize_animation_state` - the init-list order above matches it, so MSVC will
not warn and definition order is preserved.

This is a genuine restructure (a matcher's job, folded into #122 per the ticket).
Needs a rebuild to confirm the body collapses to the target's 0 statements - NOT
done here (this agent changes no compiled logic).

## report.json context

`fuzzy_match_percent = 100.0` for
`??0weapon_core_base_state@survarium@@IAE@AAVweapon_core@1@_N@Z` (size 179).
The byte view is fully green while the structure is wrong - exactly why this
function stays INPROGRESS and is NOT a clean DONE.

## Audit outcome

The structure-diff did NOT show this CLEAN - it still reports 6 quantity-diffs.
The function was already correctly marked STATE[INPROGRESS] with the
body-assigns-vs-member-init-list diagnosis, so no DONE->INPROGRESS downgrade was
needed. The one-sided `// FUNCTION BODY` carcass was replaced inline with the
condensed structure-diff, and the STATE note was refreshed to the tool's own
counting (target 0 / base 6, 6 ONLY-base quantity divergences). No new divergence
beyond the known member-init-list restructure was found.
