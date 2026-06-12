////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_OBJECT_H_INCLUDED
#define HIT_OBJECT_H_INCLUDED

/* INCLUDES */
struct vostok::ai::game_object;

namespace survarium {

struct hit_object {
	inline		hit_object	( ) { /* no source */ }

public:
	/* 0x0000 */	ai::game_object*	m_source;
	/* 0x0004 */	float3				m_position;
	/* 0x0010 */	u16					m_target_bone;
	/* 0x0014 */	float				m_power;
}; // struct hit_object

STATIC_SIZE_ASSERT(hit_object, 0x18);

} // namespace survarium

#endif // #ifndef HIT_OBJECT_H_INCLUDED
