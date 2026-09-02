// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_STAGE_PRE_RAIN_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_PRE_RAIN_H_INCLUDED
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/res_effect.h>

#include "render_target.h"
#include "res_texture.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_pre_rain : public stage {
public:
	stage_pre_rain( renderer* in_renderer, renderer_context* context );
	virtual ~stage_pre_rain( );

	virtual void execute( ) override;

private:
	float3 compute_aligment(
		float3 const& light_xz_shift,
		float4x4 const& light_space_transform,
		float shadow_map_resolution
	);

	bool is_effects_ready( ) const;
	float4x4 render_rain_shadow_map( );

private:
	render_target_ptr		m_rt_rain_shadow_map;
	res_texture_ptr			m_t_rain_shadow_map;
	res_effect_ptr			m_wet_surface_effect;
	res_effect_ptr			m_effect_shadow_direct;
	u32						m_shadow_map_size;
	shader_constant_host*	m_view_to_shadow_parameter;
	shader_constant_host*	m_eye_ray_corner_parameter;
	shader_constant_host*	m_rain_offset_parameter;
	shader_constant_host*	m_rain_density_parameter;
	float					m_rain_offset;
	float					m_rain_offset_counter;
};

STATIC_SIZE_ASSERT( stage_pre_rain, 0x3C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_PRE_RAIN_H_INCLUDED
