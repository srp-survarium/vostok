#ifndef VOSTOK_RENDER_ENGINE_EFFECT_MATERIAL_BASE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_MATERIAL_BASE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;
class shader_configuration;

class effect_material_base : public effect_descriptor {
public:
	void compile_begin(
		pcstr						vertex_shader_name,
		pcstr						pixel_shader_name,
		effect_compiler&			compiler,
		shader_configuration*		shader_config,
		custom_config_value const&	config
	);

	void compile_begin(
		pcstr						vertex_shader_name,
		pcstr						geometry_shader_name,
		pcstr						pixel_shader_name,
		effect_compiler&			compiler,
		shader_configuration*		shader_config,
		custom_config_value const&	config
	);

	void compile_end( effect_compiler& compiler );

};

STATIC_SIZE_ASSERT( effect_material_base, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_MATERIAL_BASE_H_INCLUDED
