////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_VERTEX_ID_H_INCLUDED
#define ANIMATION_SPACE_VERTEX_ID_H_INCLUDED

namespace survarium {

struct animation_space_vertex_id {
public:
	/* 0x0000 */	math::quaternion	rotation;
	/* 0x0010 */	float3				translation;
}; // struct animation_space_vertex_id

STATIC_SIZE_ASSERT(animation_space_vertex_id, 0x1C);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_VERTEX_ID_H_INCLUDED
