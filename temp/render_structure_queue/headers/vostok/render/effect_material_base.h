////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_MATERIAL_BASE_H_INCLUDED
#define RENDER_EFFECT_MATERIAL_BASE_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;
class vostok::render::shader_configuration;

namespace vostok {
namespace render {

class effect_material_base : public effect_descriptor {
public:
			void	compile_begin			(
						pcstr							vertex_shader_name,
						pcstr							pixel_shader_name,
						effect_compiler&				compiler,
						shader_configuration*			shader_config,
						custom_config_value const&		config
					);
			void	compile_begin			(
						pcstr							vertex_shader_name,
						pcstr							geometry_shader_name,
						pcstr							pixel_shader_name,
						effect_compiler&				compiler,
						shader_configuration*			shader_config,
						custom_config_value const&		config
					);
			void	compile_end				( effect_compiler& compiler );

	inline			effect_material_base	( ) { /* no source */ }
	virtual			~effect_material_base	( ) { /* no source */ }
}; // class effect_material_base

STATIC_SIZE_ASSERT(effect_material_base, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_MATERIAL_BASE_H_INCLUDED
