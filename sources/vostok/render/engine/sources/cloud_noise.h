// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_CLOUD_NOISE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CLOUD_NOISE_H_INCLUDED
namespace vostok {
namespace render {

struct cloud_noise {
	static float evaluate( float x, float y, u32 num_octaves );
};

STATIC_SIZE_ASSERT( cloud_noise, 0x1 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CLOUD_NOISE_H_INCLUDED
