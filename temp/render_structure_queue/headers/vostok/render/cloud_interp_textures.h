////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUD_INTERP_TEXTURES_H_INCLUDED
#define RENDER_CLOUD_INTERP_TEXTURES_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

struct cloud_interp_textures {
	inline		cloud_interp_textures	( ) { /* no source */ }
	inline		~cloud_interp_textures	( ) { /* no source */ }

	/* 0x0000 */	res_texture_ptr		cloud_density_0;
	/* 0x0004 */	res_texture_ptr		cloud_density_1;
}; // struct cloud_interp_textures

STATIC_SIZE_ASSERT(cloud_interp_textures, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUD_INTERP_TEXTURES_H_INCLUDED
