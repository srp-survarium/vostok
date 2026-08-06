#ifndef VOSTOK_RENDER_ENGINE_STAGE_GBUFFER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_GBUFFER_H_INCLUDED

#include <vostok/render/core/res_state.h>
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/facade/render_stage_types.h>

#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;
struct render_surface_instance;

typedef intrusive_ptr<
	res_state,
	resource_intrusive_base,
	threading::single_threading_policy
> res_state_ptr;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class effect_copy_depth_rt : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

STATIC_SIZE_ASSERT( effect_copy_depth_rt, 0x4 );

struct remove_model_if_not_lod_predicate {
	remove_model_if_not_lod_predicate( ) :
		m_shader_lod_index( 0 )
	{
	}

	explicit remove_model_if_not_lod_predicate( u32 shader_lod_index ) :
		m_shader_lod_index( shader_lod_index )
	{
	}

	bool operator()( render_surface_instance* in_model );

	u32 m_shader_lod_index;
};

STATIC_SIZE_ASSERT( remove_model_if_not_lod_predicate, 0x4 );

struct remove_model_if_not_static_predicate {
	bool operator()( render_surface_instance* in_model );
};

STATIC_SIZE_ASSERT( remove_model_if_not_static_predicate, 0x1 );

struct remove_model_if_not_skeletal_predicate {
	bool operator()( render_surface_instance* in_model );
};

STATIC_SIZE_ASSERT( remove_model_if_not_skeletal_predicate, 0x1 );

struct remove_model_if_not_translucency_predicate {
	bool operator()( render_surface_instance* in_model );
};

STATIC_SIZE_ASSERT( remove_model_if_not_translucency_predicate, 0x1 );

struct sort_by_ps_predicate {
	sort_by_ps_predicate(
		enum_render_stage_type stage_type,
		u32 tech_index
	) :
		m_stage_type( stage_type ),
		m_tech_index( tech_index )
	{
	}

	bool operator()(
		render_surface_instance const* left,
		render_surface_instance const* right
	) const;

	enum_render_stage_type	m_stage_type;
	u32						m_tech_index;
};

STATIC_SIZE_ASSERT( sort_by_ps_predicate, 0x8 );

class stage_gbuffer : public stage {
public:
	stage_gbuffer( renderer* in_renderer, renderer_context* context );
	virtual ~stage_gbuffer( );

	virtual void execute( ) override;

	bool is_effects_ready( ) const;

	void render_models(
		vector<render_surface_instance*>&	models,
		u32								shader_lod_index,
		u32&								out_num_rendered,
		bool							z_only
	);

	void render_speedtree( );
	void render_grass( bool z_only );
	void render_particles( bool z_only );
	void z_only_pass( );

private:
	res_state_ptr			m_state;
	shader_constant_host*	m_sun_direction;
	shader_constant_host*	m_c_start_corner;
	shader_constant_host*	m_object_transparency_scale_parameter;
	shader_constant_host*	m_far_fog_color_and_distance;
	shader_constant_host*	m_c_sun_near_aabb_point;
	shader_constant_host*	m_c_bound_box_min;
	shader_constant_host*	m_c_bound_box_max;
	shader_constant_host*	m_near_fog_distance;
	shader_constant_host*	m_fog_alpha;
	shader_constant_host*	m_ambient_color;
	shader_constant_host*	m_c_environment_skylight_upper_color;
	shader_constant_host*	m_c_environment_skylight_lower_color;
	shader_constant_host*	m_c_environment_skylight_parameters;
	shader_constant_host*	m_c_gs_test_constant;
	shader_constant_host*	m_c_sun_direction;
	shader_constant_host*	m_c_translucency_max_scatter;
	shader_constant_host*	m_c_sun_color;
	shader_constant_host*	m_wind_info_parameters;
	shader_constant_host*	m_smoothness_multiplier;
	shader_constant_host*	m_shadow[4];
	res_effect_ptr			m_copy_depth_rt;
	res_effect_ptr			m_fill_depth_effect;
	bool					m_is_pre_pass;

public:
	bool m_fill_view_space_depth;
};

STATIC_SIZE_ASSERT( stage_gbuffer, 0x7C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_GBUFFER_H_INCLUDED
