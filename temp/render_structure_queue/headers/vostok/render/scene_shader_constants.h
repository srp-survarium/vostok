////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_SHADER_CONSTANTS_H_INCLUDED
#define RENDER_SCENE_SHADER_CONSTANTS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

/* FORWARD REFS */
class vostok::render::post_process_parameters;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

struct scene_shader_constants {
			scene_shader_constants	( );

	void	set						(
				renderer_context*					context,
				float3 const&						height_lights,
				float3 const&						mid_tones,
				float3 const&						shadows,
				float3 const&						fade_color,
				float								fade_amount,
				float								gamma_correction_factor,
				float								desaturation,
				float4 const&						image_grain_parameters,
				post_process_parameters const&		parameters
			);

private:
	/* 0x0000 */	shader_constant_host*		m_frame_height_lights_and_desaturation_parameters;
	/* 0x0004 */	shader_constant_host*		m_scene_mid_tones_parameters;
	/* 0x0008 */	shader_constant_host*		m_scene_shadows_parameters;
	/* 0x000c */	shader_constant_host*		m_gamma_correction_factor;
	/* 0x0010 */	shader_constant_host*		m_scene_fade_parameters;
	/* 0x0014 */	shader_constant_host*		m_filmic_tonemap_packed_parameters_0;
	/* 0x0018 */	shader_constant_host*		m_filmic_tonemap_packed_parameters_1;
	/* 0x001c */	shader_constant_host*		m_c_eye_ray_corner;
	/* 0x0020 */	shader_constant_host*		m_image_grain_parameters;
}; // struct scene_shader_constants

STATIC_SIZE_ASSERT(scene_shader_constants, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_SHADER_CONSTANTS_H_INCLUDED
