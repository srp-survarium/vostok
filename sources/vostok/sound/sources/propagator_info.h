// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SOUND_PROPAGATOR_INFO_H_INCLUDED
#define SOUND_PROPAGATOR_INFO_H_INCLUDED

#include "sound_propagator.h"

namespace vostok {
namespace sound {

struct propagator_info
{
	inline propagator_info	( ) {}

	float3						in_graph_position;
	float						distance_to_listener;
	new_sound_propagator*		prop;
}; // struct propagator_info

STATIC_SIZE_ASSERT( propagator_info, 0x14 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_PROPAGATOR_INFO_H_INCLUDED
