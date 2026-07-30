#ifndef VOSTOK_RENDER_ENGINE_CLOUD_INTERP_TEXTURES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CLOUD_INTERP_TEXTURES_H_INCLUDED

#include <vostok/intrusive_ptr.h>

#include "res_texture.h"

namespace vostok {
namespace render {

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

struct cloud_interp_textures {
	cloud_interp_textures( ) { }
	~cloud_interp_textures( ) { }

	res_texture_ptr	cloud_density_0;
	res_texture_ptr	cloud_density_1;
};

STATIC_SIZE_ASSERT( cloud_interp_textures, 0x8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CLOUD_INTERP_TEXTURES_H_INCLUDED
