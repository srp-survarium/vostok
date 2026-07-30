#ifndef VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_COOK_DATA_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_COOK_DATA_H_INCLUDED

#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/shader_configuration.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {
namespace render {

class res_effect;

struct binary_shader_cook_data {
	binary_shader_cook_data(
		shader_configuration in_configuration,
		fs_new::virtual_path_string in_shader_name,
		enum_shader_type in_shader_type,
		bool in_is_need_check_time,
		res_effect* in_effect_resource
	)
		: configuration( in_configuration ),
		  shader_name( in_shader_name ),
		  shader_type( in_shader_type ),
		  is_need_check_time( in_is_need_check_time ),
		  effect_resource( in_effect_resource )
	{
		// FUNCTION BODY[0x1303e0]
	}

	res_effect* effect_resource;
	shader_configuration configuration;
	fs_new::virtual_path_string shader_name;
	enum_shader_type shader_type;
	bool is_need_check_time;
};

STATIC_SIZE_ASSERT( binary_shader_cook_data, 0x138 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BINARY_SHADER_COOK_DATA_H_INCLUDED
