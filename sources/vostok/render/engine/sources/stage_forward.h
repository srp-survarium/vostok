#ifndef VOSTOK_RENDER_ENGINE_STAGE_FORWARD_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_FORWARD_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;
struct render_surface_instance;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

struct remove_model_if_not_forward_predicate {
	bool operator()( render_surface_instance const* in_model );
};

STATIC_SIZE_ASSERT( remove_model_if_not_forward_predicate, 0x1 );

class stage_forward : public stage {
public:
	enum stage_type {
		forward_base = 0,
		forward_sky = 1,
	};

public:
	stage_forward(
		renderer* in_renderer,
		renderer_context* in_context,
		stage_type type
	) :
		stage( in_renderer, in_context ),
		m_tree_position_and_scale_parameter( 0 ),
		m_tree_rotation_parameter( 0 ),
		m_far_fog_color_and_distance( 0 ),
		m_fog_alpha( 0 ),
		m_near_fog_distance( 0 ),
		m_eye_ray_corner_parameter( 0 ),
		m_view_to_shadow_parameter( 0 ),
		m_rain_offset_parameter( 0 ),
		m_use_rain_parameter( 0 ),
		m_tracer_debug_color_parameter( 0 ),
		m_c_inscatter_parameters( 0 ),
		m_rain_offset( 0.0f ),
		m_rain_offset_counter( 0.0f ),
		m_type( type )
	{
	}

	virtual ~stage_forward( );
	virtual void execute( ) override;

	void accumulate_local_reflections( );
	bool is_effects_ready( ) const;
	void render_opaque_models( );
	void render_forward_models(
		vector<render_surface_instance*>& dynamic_visuals,
		u32 pass_index
	);

private:
	res_effect_ptr			m_debug_tracer_effect;
	res_effect_ptr			m_opaque_geometry_mask_effect;
	res_effect_ptr			m_gbuffer_depth_effect[15];
	shader_constant_host*	m_tree_position_and_scale_parameter;
	shader_constant_host*	m_tree_rotation_parameter;
	shader_constant_host*	m_far_fog_color_and_distance;
	shader_constant_host*	m_fog_alpha;
	shader_constant_host*	m_near_fog_distance;
	shader_constant_host*	m_eye_ray_corner_parameter;
	shader_constant_host*	m_view_to_shadow_parameter;
	shader_constant_host*	m_rain_offset_parameter;
	shader_constant_host*	m_use_rain_parameter;
	shader_constant_host*	m_tracer_debug_color_parameter;
	shader_constant_host*	m_c_inscatter_parameters;
	float					m_rain_offset;
	float					m_rain_offset_counter;
	stage_type				m_type;
};

STATIC_SIZE_ASSERT( stage_forward, 0x8C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_FORWARD_H_INCLUDED
