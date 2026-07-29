////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_MANAGER_SHADER_CACHE_INFO_H_INCLUDED
#define RENDER_EFFECT_MANAGER_SHADER_CACHE_INFO_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
struct vostok::render::shader_configuration;

namespace vostok {
namespace render {

struct effect_manager::shader_cache_info {
	inline		shader_cache_info	( ) { /* no source */ }

	/* 0x0000 */	fs_new::virtual_path_string		vertex_shader_name;
	/* 0x0114 */	fs_new::virtual_path_string		pixel_shader_name;
	/* 0x0228 */	shader_configuration			configuration;
}; // struct effect_manager::shader_cache_info

STATIC_SIZE_ASSERT(effect_manager::shader_cache_info, 0x238);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_MANAGER_SHADER_CACHE_INFO_H_INCLUDED
