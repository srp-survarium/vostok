#ifndef VOSTOK_RENDER_ENGINE_STAGE_SUN_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_SUN_H_INCLUDED

#include <vostok/render/core/res_effect.h>

#include "box_geometry.h"
#include "cloud_interp_textures.h"
#include "stage.h"

namespace vostok {
namespace render {

class cloud_simulation;
class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_sun : public stage {
public:
	stage_sun(
		renderer*				in_renderer,
		renderer_context*		in_context,
		cloud_interp_textures&	in_cloud_interp_textures,
		cloud_simulation&		in_simulation
	);

	virtual void execute( ) override;

private:
	bool is_effects_ready( ) const;

private:
	res_effect_ptr			m_sun_effect;
	res_texture_ptr			m_shadow_jitter;
	box_geometry			m_box_geometry;
	shader_constant_host*	m_c_light_direction;
	shader_constant_host*	m_c_light_color;
	shader_constant_host*	m_c_shadow_transparency;
	shader_constant_host*	m_c_light_intensity;
	shader_constant_host*	m_shadow[4];
	shader_constant_host*	m_c_diffuse_influence_factor;
	shader_constant_host*	m_c_specular_influence_factor;
	shader_constant_host*	m_c_environment_skylight_upper_color;
	shader_constant_host*	m_c_environment_skylight_lower_color;
	shader_constant_host*	m_c_clouds_offset;
	shader_constant_host*	m_c_world_to_cloud;
	shader_constant_host*	m_c_cloud_interp_alpha;
	shader_constant_host*	m_c_inverted_view_projection_matrix;
	shader_constant_host*	m_c_sun_fixed_matrix;
	shader_constant_host*	m_c_eye_ray_corner;
	cloud_interp_textures&	m_cloud_interp_textures;
	cloud_simulation&		m_simulation;
};

STATIC_SIZE_ASSERT( stage_sun, 0x78 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_SUN_H_INCLUDED
