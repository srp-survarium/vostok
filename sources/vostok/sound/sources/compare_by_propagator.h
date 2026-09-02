// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SOUND_COMPARE_BY_PROPAGATOR_H_INCLUDED
#define SOUND_COMPARE_BY_PROPAGATOR_H_INCLUDED

#include "unique_propagator_info.h"

namespace vostok {
namespace sound {

struct compare_by_propagator
{
	inline explicit compare_by_propagator	( new_sound_propagator* value ) : prop( value ) {}

	inline bool operator( )	( unique_propagator_info const& value ) const
	{
		return value.prop == prop;
	}

	new_sound_propagator* prop;
}; // struct compare_by_propagator

STATIC_SIZE_ASSERT( compare_by_propagator, 0x4 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_COMPARE_BY_PROPAGATOR_H_INCLUDED
