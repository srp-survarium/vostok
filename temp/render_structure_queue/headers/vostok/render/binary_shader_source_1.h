////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BINARY_SHADER_SOURCE_H_INCLUDED
#define RENDER_BINARY_SHADER_SOURCE_H_INCLUDED

/* INCLUDES */
struct ID3D10Blob;
class vostok::fs_new::virtual_path_string;
class vostok::resources::unmanaged_resource;
struct vostok::render::shader_configuration;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::resources::managed_resource;

namespace vostok {
namespace render {

enum vostok::render::enum_shader_type
{
};

struct binary_shader_source : public resources::unmanaged_resource {
	inline		binary_shader_source	( ) { /* no source */ }
	virtual		~binary_shader_source	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	resources::managed_resource_ptr		shader_source;
	/* 0x010c */	void*								m_shader_byte_code;
	/* 0x0110 */	void*								m_compiled_shader_byte_code;
	/* 0x0114 */	u32									m_shader_byte_code_size;
	/* 0x0118 */	ID3D10Blob*							error_code;
	/* 0x0120 */	shader_configuration				configuration;
	/* 0x0130 */	fs_new::virtual_path_string			shader_name;
	/* 0x0244 */	enum_shader_type					shader_type;
}; // struct binary_shader_source

STATIC_SIZE_ASSERT(binary_shader_source, 0x248);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BINARY_SHADER_SOURCE_H_INCLUDED
