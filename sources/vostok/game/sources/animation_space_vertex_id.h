////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_VERTEX_ID_H_INCLUDED
#define ANIMATION_SPACE_VERTEX_ID_H_INCLUDED

#include <boost/crc.hpp>

namespace survarium {

struct animation_space_vertex_id {
public:
	/* 0x0000 */	math::quaternion	rotation;
	/* 0x0010 */	float3				translation;
}; // struct animation_space_vertex_id

STATIC_SIZE_ASSERT(animation_space_vertex_id, 0x1C);

} // namespace survarium

namespace vostok {
namespace ai {
namespace vertex_manager {

inline u32 hash_value( survarium::animation_space_vertex_id const& vertex_id )
{
	boost::crc_32_type processor;
	processor.process_bytes( &vertex_id, sizeof( vertex_id ) );
	return processor.checksum( );
}

} // namespace vertex_manager
} // namespace ai
} // namespace vostok

#endif // #ifndef ANIMATION_SPACE_VERTEX_ID_H_INCLUDED
