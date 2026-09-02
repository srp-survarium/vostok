// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SOUND_SOUND_SCENE_CREATION_PARAMS_H_INCLUDED
#define SOUND_SOUND_SCENE_CREATION_PARAMS_H_INCLUDED

namespace vostok {
namespace sound {

struct sound_scene_creation_params {

public:
	u32		proxies_count;
	u32		propagators_count;
	u32		receivers_count;
}; // struct sound_scene_creation_params

STATIC_SIZE_ASSERT( sound_scene_creation_params, 0xC );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_SOUND_SCENE_CREATION_PARAMS_H_INCLUDED
