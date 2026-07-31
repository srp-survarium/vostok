#ifndef VOSTOK_RENDER_ENGINE_STAGE_SHADOW_DIRECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_SHADOW_DIRECT_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>

#include "cuboid2.h"
#include "render_target.h"
#include "res_texture.h"
#include "stage.h"

namespace vostok {
namespace render {

struct geometry_batch;
class renderer;
class renderer_context;
class shader_constant_host;
struct render_surface_instance;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_shadow_direct : public stage {
public:
	stage_shadow_direct( renderer* in_renderer, renderer_context* context );
	virtual ~stage_shadow_direct( );

	void invalidate_shadow( )
	{
		m_invalid_shadow = true;
	}

	virtual void execute( ) override;
	virtual void execute_disabled( ) override;

	void execute_cascade( u32 cascade_id, u32 cascade_index, u32 shadow_map_size );
	void draw_debug( u32 in_cascade_id );

	void prepare_models(
		vector<render_surface_instance*>&	caster_models,
		float4x4 const&						original_view_projection,
		u32									cascade_index,
		u32									shadow_map_size,
		float3 const&						view_position
	);

	void render_models(
		vector<render_surface_instance*>&	caster_models,
		float4x4 const&						original_view_projection,
		u32									cascade_index,
		u32									shadow_map_size,
		float3 const&						real_view_position,
		u32									pass_index,
		u32									refresh_rate
	);

	void render_dynamic_models(
		u32				cascade_index,
		u32				shadow_map_size,
		float3 const&	view_position
	);

private:
	float3 compute_aligment(
		float3 const&		light_xz_shift,
		float4x4 const&		light_space_transform,
		float				shadow_map_resolution,
		float				multiplier,
		float3&				granularity
	);

	float3 compute_aligment1(
		float3 const&		value,
		float4x4 const&,
		float
	)
	{
		return value;
	}

	u32 index_to_shadow_size( u32 size_index ) const
	{
		switch ( size_index ) {
			case 0: return 2048;
			case 1: return 1024;
			case 2: return 512;
			case 3: return 256;
			case 4: return 128;
			default: return 1024;
		}
	}

	void render_speedtree_instances( float3 const&, u32 ) { }
	void pre_batch_render( geometry_batch const& ) { }
	void post_batch_render( geometry_batch const& ) { }

	bool is_effects_ready( ) const;

private:
	shader_constant_host*	m_c_light_direction;
	shader_constant_host*	m_c_light_position;
	shader_constant_host*	m_c_light_attenuation_power;
	shader_constant_host*	m_c_start_corner;
	shader_constant_host*	m_wind_info_parameters;
	shader_constant_host*	m_shadow_cascade_index;
	res_effect_ptr			m_effect_shadow_direct;
	bool					m_cascade_texture_index[4];
	cuboid2					m_frustums[4];
	float3					m_previous_adjastment[4];
	float3					m_previous_direction;
	float3					m_previous_position[4];
	float4x4				m_previous_view_matrix[4];
	float4x4				m_previous_projection_matrix[4];
	float4x4				m_previous_real_view_matrix[4];
	float4x4				m_view_to_shadow[4];
	float4x4				m_prev_view_to_shadow[4];
	u32						m_cascade_shadow_map_size;
	render_target_ptr		m_rt_shadow_map;
	res_texture_ptr			m_t_shadow_map;
	bool					m_invalid_shadow;
};

STATIC_SIZE_ASSERT( stage_shadow_direct, 0xABC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_SHADOW_DIRECT_H_INCLUDED
