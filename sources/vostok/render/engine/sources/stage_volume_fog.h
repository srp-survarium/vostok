#ifndef VOSTOK_RENDER_ENGINE_STAGE_VOLUME_FOG_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_VOLUME_FOG_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/res_effect.h>

#include "fog_box_geometry.h"
#include "sphere_geometry.h"
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

class effect_simple_fog : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

STATIC_SIZE_ASSERT( effect_simple_fog, 0x4 );

class stage_volume_fog : public stage {
public:
	stage_volume_fog( renderer* in_renderer, renderer_context* in_context );
	virtual ~stage_volume_fog( ) { }

	virtual void execute( ) override;

private:
	bool is_effects_ready( ) const;

private:
	fog_box_geometry		m_fog_box_geometry;
	sphere_geometry			m_fog_sphere_geometry;
	res_effect_ptr			m_exponential_volume_fog_effect;
	res_effect_ptr			m_simple_fog_effect;
	shader_constant_host*	m_eye_ray_corner_parameter;
	shader_constant_host*	m_inverted_world_matrix_parameter;
	shader_constant_host*	m_eye_pos_os_parameter;
	shader_constant_host*	m_eye_pos_ws_parameter;
	shader_constant_host*	m_is_inside_volume_parameter;
	shader_constant_host*	m_fog_parameters0;
	shader_constant_host*	m_fog_parameters1;
	shader_constant_host*	m_fog_parameters2;
	shader_constant_host*	m_fog_parameters3;
	shader_constant_host*	m_far_fog_color_and_distance;
	shader_constant_host*	m_near_fog_distance;
	shader_constant_host*	m_fog_alpha;
};

STATIC_SIZE_ASSERT( stage_volume_fog, 0x60 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_VOLUME_FOG_H_INCLUDED
