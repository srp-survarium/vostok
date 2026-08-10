#ifndef VOSTOK_RENDER_ENGINE_STAGE_AMBIENT_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_AMBIENT_LIGHTING_H_INCLUDED

#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>

#include "res_geometry.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_ambient_lighting : public stage {
public:
	stage_ambient_lighting( renderer* in_renderer, renderer_context* context );

	virtual void execute( ) override;

	bool is_effects_ready( ) const;

	void use_probes( bool value )
	{
		m_use_probes = value;
	}

	void set_ambient_multiplier( float value )
	{
		m_ambient_multiplier = value;
	}

	virtual ~stage_ambient_lighting( ) {}

private:
	res_effect_ptr			m_effect_accum_mask;
	res_effect_ptr			m_environment_probe_lighting_effect[2][2][2];
	res_effect_ptr			m_skylight_effect;
	res_effect_ptr			m_sky_ambient_occlusion_effect;
	res_effect_ptr			m_ambient_volume_effect;
	res_effect_ptr			m_reflection_mask_effect;
	res_effect_ptr			m_sh_ssao_downsample_position_and_normal;
	float					m_ambient_multiplier;
	bool					m_use_probes;
	shader_constant_host*	m_c_eye_ray_corner;
	shader_constant_host*	m_c_light_range;
	shader_constant_host*	m_c_num_mips;
	shader_constant_host*	m_c_color_parameters;
	shader_constant_host*	m_c_skylight_parameters0;
	shader_constant_host*	m_c_skylight_parameters1;
	shader_constant_host*	m_c_skylight_parameters2;
	shader_constant_host*	m_c_skylight_parameters3;
	shader_constant_host*	m_c_skylight_parameters4;
	shader_constant_host*	m_c_skylight_parameters5;
	shader_constant_host*	m_c_skylight_parameters6;
	shader_constant_host*	m_c_skylight_parameters7;
	shader_constant_host*	m_c_world_to_ao_map;
	shader_constant_host*	m_c_ambient_volume_multiplier;
	shader_constant_host*	m_c_world_to_probe;
	untyped_buffer_ptr		m_sphere_vertex_buffer;
	untyped_buffer_ptr		m_sphere_index_buffer;
	res_geometry_ptr		m_sphere_geometry;
	untyped_buffer_ptr		m_box_vertex_buffer;
	untyped_buffer_ptr		m_box_index_buffer;
	res_geometry_ptr		m_box_geometry;
};

STATIC_SIZE_ASSERT( stage_ambient_lighting, 0xA4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_AMBIENT_LIGHTING_H_INCLUDED
