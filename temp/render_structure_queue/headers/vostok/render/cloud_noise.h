////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUD_NOISE_H_INCLUDED
#define RENDER_CLOUD_NOISE_H_INCLUDED

namespace vostok {
namespace render {

struct cloud_noise {

	static	float	evaluate( float x, float y, u32 num_octaves );
}; // struct cloud_noise

STATIC_SIZE_ASSERT(cloud_noise, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUD_NOISE_H_INCLUDED
