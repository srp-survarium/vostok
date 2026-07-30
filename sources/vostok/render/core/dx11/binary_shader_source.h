#ifndef VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_SOURCE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_SOURCE_H_INCLUDED

#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/shader_configuration.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/resources_unmanaged_resource.h>

struct ID3D10Blob;

namespace vostok {
namespace render {

struct binary_shader_source : public resources::unmanaged_resource {
	binary_shader_source( )
		: m_shader_byte_code( 0 ),
		  m_compiled_shader_byte_code( 0 ),
		  m_shader_byte_code_size( 0 ),
		  error_code( 0 ),
		  shader_type( enum_shader_type_vertex )
	{
	}

	virtual ~binary_shader_source( ) { }

	resources::managed_resource_ptr shader_source;
	void* m_shader_byte_code;
	void* m_compiled_shader_byte_code;
	u32 m_shader_byte_code_size;
	ID3D10Blob* error_code;
	shader_configuration configuration;
	fs_new::virtual_path_string shader_name;
	enum_shader_type shader_type;
};

typedef resources::resource_ptr<
	binary_shader_source,
	resources::unmanaged_intrusive_base
> binary_shader_source_ptr;

STATIC_SIZE_ASSERT( binary_shader_source, 0x248 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_SOURCE_H_INCLUDED
