#ifndef VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_KEY_TYPE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_KEY_TYPE_H_INCLUDED

#include <vostok/associative_vector.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/binary_shader_source.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/shader_configuration.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {
namespace render {

struct binary_shader_key_type {
	binary_shader_key_type(
		pcstr shader_name,
		enum_shader_type type,
		shader_configuration configuration
	)
		: configuration( configuration ),
		  shader_name( shader_name ),
		  type( type )
	{
	}

	shader_configuration configuration;
	fs_new::virtual_path_string shader_name;
	enum_shader_type type;
};

inline bool operator<(
	binary_shader_key_type const& left,
	binary_shader_key_type const& right
)
{
	// FUNCTION BODY[0x1249b0]
	return left.shader_name < right.shader_name
		|| (left.shader_name == right.shader_name
			&& (union_base::operator<( left.configuration, right.configuration )
				|| (left.configuration.configuration[0]
						== right.configuration.configuration[0]
					&& left.configuration.configuration[1]
						== right.configuration.configuration[1]
					&& left.type < right.type)));
}

inline bool operator==(
	binary_shader_key_type const& left,
	binary_shader_key_type const& right
)
{
	return left.shader_name == right.shader_name
		&& left.configuration.configuration[0]
			== right.configuration.configuration[0]
		&& left.configuration.configuration[1]
			== right.configuration.configuration[1]
		&& left.type == right.type;
}

typedef associative_vector<
	binary_shader_key_type,
	binary_shader_source_ptr,
	render::vector
> binary_shader_sources_type;

STATIC_SIZE_ASSERT( binary_shader_key_type, 0x128 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_KEY_TYPE_H_INCLUDED
