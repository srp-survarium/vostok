////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FINGERS_TO_WEAPON_CORRECTOR_HAND_H_INCLUDED
#define FINGERS_TO_WEAPON_CORRECTOR_HAND_H_INCLUDED

namespace survarium {

struct fingers_to_weapon_corrector::hand {
	inline		hand( ) { /* no source */ }

public:
	/* 0x0000 */	float4x4	phalanges_matrices[15];
	/* 0x03c0 */	u32			phalanges_bones_indices[15];
	/* 0x03fc */	u32			start_transition_time_in_ms;
	/* 0x0400 */	bool		is_active;
}; // struct fingers_to_weapon_corrector::hand

STATIC_SIZE_ASSERT(fingers_to_weapon_corrector::hand, 0x404);

} // namespace survarium

#endif // #ifndef FINGERS_TO_WEAPON_CORRECTOR_HAND_H_INCLUDED
