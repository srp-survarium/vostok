////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_PROPAGATOR_INFO_H_INCLUDED
#define SOUND_PROPAGATOR_INFO_H_INCLUDED

#include "new_sound_propagator.h"

namespace vostok {
namespace sound {

struct propagator_info
{
	inline propagator_info	( ) : distance_to_listener( 0.0f ), prop( 0 ) {}

	float3						in_graph_position;
	float						distance_to_listener;
	new_sound_propagator*		prop;
}; // struct propagator_info

STATIC_SIZE_ASSERT( propagator_info, 0x14 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_PROPAGATOR_INFO_H_INCLUDED
