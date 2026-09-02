// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SOUND_UNIQUE_PROPAGATOR_INFO_H_INCLUDED
#define SOUND_UNIQUE_PROPAGATOR_INFO_H_INCLUDED

#include "new_sound_propagator.h"
#include "sound_voice_params.h"

namespace vostok {
namespace sound {

struct unique_propagator_info
{
	unique_propagator_info	( );

	vectora< sound_voice_params >	voice_params;
	new_sound_propagator*			prop;
}; // struct unique_propagator_info

STATIC_SIZE_ASSERT( unique_propagator_info, 0x14 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_UNIQUE_PROPAGATOR_INFO_H_INCLUDED
