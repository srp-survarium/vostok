////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BINARY_SHADER_COOK_DATA_H_INCLUDED
#define RENDER_BINARY_SHADER_COOK_DATA_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
class vostok::render::res_effect;
struct vostok::render::shader_configuration;
class vostok::render::enum_shader_type;

namespace vostok {
namespace render {

enum vostok::render::enum_shader_type
{
};

struct binary_shader_cook_data {
	inline		binary_shader_cook_data	(
					shader_configuration			in_configuration,
					fs_new::virtual_path_string		in_shader_name,
					enum_shader_type				in_shader_type,
					bool							in_is_need_check_time,
					res_effect*						in_effect_resource
				) { /* no source */ }

	/* 0x0000 */	res_effect*						effect_resource;
	/* 0x0008 */	shader_configuration			configuration;
	/* 0x0018 */	fs_new::virtual_path_string		shader_name;
	/* 0x012c */	enum_shader_type				shader_type;
	/* 0x0130 */	bool							is_need_check_time;
}; // struct binary_shader_cook_data

STATIC_SIZE_ASSERT(binary_shader_cook_data, 0x138);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BINARY_SHADER_COOK_DATA_H_INCLUDED
