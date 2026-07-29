////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BINARY_SHADER_KEY_TYPE_H_INCLUDED
#define RENDER_BINARY_SHADER_KEY_TYPE_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
struct vostok::render::shader_configuration;
class vostok::render::enum_shader_type;

namespace vostok {
namespace render {

enum vostok::render::enum_shader_type
{
	enum_shader_type_vertex			= 0x0,
	enum_shader_type_pixel			= 0x1,
	enum_shader_type_geometry		= 0x2,
	enum_shader_types_count			= 0x3,
};

struct binary_shader_key_type {
	inline		binary_shader_key_type	( pcstr arg_0, enum_shader_type arg_1, shader_configuration arg_2 ) { /* no source */ }

	/* 0x0000 */	shader_configuration			configuration;
	/* 0x0010 */	fs_new::virtual_path_string		shader_name;
	/* 0x0124 */	enum_shader_type				type;
}; // struct binary_shader_key_type

STATIC_SIZE_ASSERT(binary_shader_key_type, 0x128);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BINARY_SHADER_KEY_TYPE_H_INCLUDED
