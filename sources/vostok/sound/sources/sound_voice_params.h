// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SOUND_VOICE_PARAMS_H_INCLUDED
#define SOUND_VOICE_PARAMS_H_INCLUDED

namespace vostok {
namespace sound {

struct sound_voice_params
{
	float	channel_matrix[2];
	float	lp_filter_coeff;
}; // struct sound_voice_params

STATIC_SIZE_ASSERT( sound_voice_params, 0xC );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_VOICE_PARAMS_H_INCLUDED
