// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_STAGE_RAIN_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_RAIN_H_INCLUDED
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/render/core/res_effect.h>

#include "res_texture.h"
#include "sphere_geometry.h"
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

class stage_rain : public stage {
public:
	stage_rain( renderer* in_renderer, renderer_context* context );
	virtual ~stage_rain( );

	virtual void execute( ) override;

private:
	bool is_effects_ready( ) const;

private:
	res_texture_ptr			m_t_rain_shadow_map;
	res_effect_ptr			m_rain_effect;
	res_effect_ptr			m_effect_shadow_direct;
	sphere_geometry			m_rain_geometry;
	float2*					m_rain_offsets;
	float					m_camera_offset_view;
	float					m_camera_offset_right;
	float					m_rain_rotation_x[100];
	float					m_rain_rotation_y[100];
	u32						m_shadow_map_size;
	float3					m_previous_view_position;
	float3					m_moving_direction;
	shader_constant_host*	m_radius_parameter;
	shader_constant_host*	m_rain_speed_parameter;
	shader_constant_host*	m_rain_density_parameter;
	shader_constant_host*	m_rain_uv_scales_parameter;
	shader_constant_host*	m_view_to_shadow_parameter;
};

STATIC_SIZE_ASSERT( stage_rain, 0x38C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_RAIN_H_INCLUDED
