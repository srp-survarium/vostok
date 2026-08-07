#include "pch.h"
#include "stage_light_propagation_volumes.h"

#include "remove_model_if_in_frustum_predicate.h"

#include <vostok/collision/object.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/render/facade/vertex_input_type.h>
#include "effect_apply_indirect_lighting.h"
#include "effect_downsample_gbuffer.h"
#include "effect_downsample_reflective_shadow_map.h"
#include "effect_fill_reflective_shadow_map.h"
#include "geometry_batch.h"
#include "light.h"
#include "lights_db.h"
#include "material_effects.h"
#include "radiance_volume.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "scene_view.h"
#include "shared_names.h"
#include "statistics.h"

namespace vostok {
namespace render {

static bool s_lpv_dips_skipping0_value = false;
static console_commands::cc_bool s_lpv_dips_skipping0_cc(
	"r_lpv_dips_skipping0",
	s_lpv_dips_skipping0_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_lpv_dips_skipping1_value = false;
static console_commands::cc_bool s_lpv_dips_skipping1_cc(
	"r_lpv_dips_skipping1",
	s_lpv_dips_skipping1_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_draw_to_rsm_value = false;
static console_commands::cc_bool s_draw_to_rsm_cc(
	"r_draw_to_rsm",
	s_draw_to_rsm_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_use_batched_lpv_geometry = false;
static console_commands::cc_bool s_use_batched_lpv_geometry_cc(
	"r_use_batched_lpv_geometry",
	s_use_batched_lpv_geometry,
	false,
	console_commands::command_type_user_specific
);

static u32 s_start_cascade_to_use_batching = 0;
static console_commands::cc_u32 s_start_cascade_to_use_batching_cc(
	"r_start_cascade_to_use_batching",
	s_start_cascade_to_use_batching,
	0,
	3,
	true,
	console_commands::command_type_user_specific
);

static bool s_lpv0_value = false;
static console_commands::cc_bool s_lpv0(
	"lpv0",
	s_lpv0_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_use_smooothed_lpv_value = false;
static console_commands::cc_bool s_use_smooothed_lpv_cc(
	"r_use_smooothed_lpv",
	s_use_smooothed_lpv_value,
	false,
	console_commands::command_type_user_specific
);

struct screen_vertex {
	float4	position;
	float2	tc;

	void set( float4 const& in_position, float2 const& in_tc )
	{
		position	= in_position;
		tc			= in_tc;
	}
};

STATIC_SIZE_ASSERT( screen_vertex, 0x18 );

struct remove_lpv_inappropriate_models {
	bool operator()( lpv_render_surface const& surface )
	{
		if (
			surface.surface->m_dynamic_screen_factor < 0.0005f ||
			( options::ref( ).current.m_use_hiz_occlusion_culling && surface.surface->m_occluded ) ||
			surface.surface->m_render_surface->get_vertex_input_type( ) != static_mesh_vertex_input_type
		)
		{
			statistics::ref( ).lpv_stat_group.num_clipped_dips.value++;
			return true;
		}

		return false;
	}
};

stage_light_propagation_volumes::stage_light_propagation_volumes(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage						( in_renderer, context ),
	start_render_eye_position	( 0.0f, 0.0f, 0.0f ),
	m_radiance_volume			( 0 ),
	m_num_cascades				( 0 ),
	m_rsm_source_size			( 0 ),
	m_rsm_downsampled_size		( 0 ),
	m_grid_size					( 0 ),
	m_has_indirect_lighting		( false )
{
	// FUNCTION BODY[0x619bf0]
	m_num_cascades				= options::ref().current.m_lpv_num_cascades;

	math::clamp<u32>			(m_num_cascades, 1, 4);

	m_grid_size					= options::ref().current.m_num_radiance_volume_cells;

	m_rsm_source_size			= options::ref().current.m_light_propagation_volumes_rsm_size;
	m_rsm_downsampled_size		= options::ref().current.m_light_propagation_volumes_rsm_size / 2;

	m_radiance_volume			= ALLOC(radiance_volume, m_num_cascades);

	float cascade_scales			[] = {1.0f, 2.0f, 4.0f, 6.0f, };
	float cascade_cells_scales		[] = {1.0f, 1.0f, 1.0f, 1.0f, };
	float cascade_flux_scales		[] = {1.0f, 1.0f, 1.0f, 1.0f, };
	float cascade_iteration_scales	[] = {1.0f, 1.0f / 2.0f, 1.0f / 4.0f, 1.0f / 8.0f, };

	for (u32 cascade_index = 0; cascade_index < m_num_cascades; cascade_index++)
	{
		new(&m_radiance_volume[cascade_index])radiance_volume(
			m_rsm_downsampled_size,
			u32(options::ref().current.m_num_radiance_volume_cells * cascade_cells_scales[cascade_index]),
			u32(options::ref().current.m_num_propagate_iterations * cascade_iteration_scales[cascade_index]),
			options::ref().current.m_radiance_volume_scale * cascade_scales[cascade_index],
			options::ref().current.m_lpv_flux_amplifier * cascade_flux_scales[cascade_index]
		);

		m_previous_view_matrix[cascade_index].identity( );
		m_previous_proj_matrix[cascade_index].identity( );
	}

	m_rms_depth_stencil_source[0] = resource_manager::ref().create_render_target("$user$rms_depth_stencil0", m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);
	m_rms_depth_stencil_source[1] = resource_manager::ref().create_render_target("$user$rms_depth_stencil1", m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);
	m_rms_depth_stencil_source[2] = resource_manager::ref().create_render_target("$user$rms_depth_stencil2", m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);
	m_rms_depth_stencil_source[3] = resource_manager::ref().create_render_target("$user$rms_depth_stencil3", m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0);

	m_c_interreflection_contribution		= backend::ref().register_constant_host("interreflection_contribution", rc_float);
	m_c_cascade_index						= backend::ref().register_constant_host("cascade_index", rc_int);
	m_c_num_cascades						= backend::ref().register_constant_host("num_cascades", rc_int);
	m_c_ambient_color						= backend::ref().register_constant_host("ambient_color", rc_float);

	m_c_smaller_cascade_grid_cell_size		= backend::ref().register_constant_host("smaller_cascade_grid_cell_size", rc_float);
	m_c_smaller_cascade_grid_size			= backend::ref().register_constant_host("smaller_cascade_grid_size", rc_float);
	m_c_smaller_cascade_grid_origin			= backend::ref().register_constant_host("smaller_cascade_grid_origin", rc_float);
	m_c_radiance_blend_factor				= backend::ref().register_constant_host("radiance_blend_factor", rc_float);

	register_rsm_constans		();
	register_light_constans		();

	m_c_eye_ray_corner			= backend::ref().register_constant_host("s_eye_ray_corner", rc_float);

	for (u32 i = 0; i < num_vertex_input_types; i++)
	{
		if (i == post_process_vertex_input_type)
			continue;

		byte						data[Kb];
		effect_options_descriptor	desc(data, Kb);
		desc["vertex_input_type"]	= (enum_vertex_input_type)i;

		effect_manager::ref().create_effect<effect_fill_reflective_shadow_map>(&m_fill_rsm_effect[i], desc);
	}

	const D3D_INPUT_ELEMENT_DESC screen_vertex_layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	    0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	};

	u16 indices[6]			= { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib		= resource_manager::ref().create_buffer( 6*sizeof(u16), indices, enum_buffer_type_index, false, false);
	m_screen_vertex_geometry	= resource_manager::ref().create_geometry(
		screen_vertex_layout,
		sizeof(screen_vertex),
		backend::ref().vertex.buffer(),
		*m_screen_vertex_ib
	);

	effect_manager::ref().create_effect<effect_downsample_reflective_shadow_map>(&m_downsample_rsm_effect);
	effect_manager::ref().create_effect<effect_apply_indirect_lighting>(&m_apply_indirect_lighting_effect);
	effect_manager::ref().create_effect<effect_downsample_gbuffer>(&m_downsample_gbuffer_effect);

	m_enabled					= options::ref().current.m_enabled_light_propagation_volumes_stage;
}

stage_light_propagation_volumes::~stage_light_propagation_volumes( )
{
	// FUNCTION BODY[0x615980]
	FREE(m_radiance_volume);
}

bool stage_light_propagation_volumes::is_effects_ready( ) const
{
	// FUNCTION BODY[0x6146e0]
	for (u32 i = 0; i < num_vertex_input_types; i++)
	{
		if (i == post_process_vertex_input_type)
			continue;

		if (m_fill_rsm_effect[i].c_ptr() == NULL)
			return false;
	}

	for (u32 cascade_index = 0; cascade_index < m_num_cascades; cascade_index++)
	{
		if (!m_radiance_volume[cascade_index].is_effects_ready())
			return false;
	}

	return m_downsample_rsm_effect.c_ptr() != NULL
		&& m_apply_indirect_lighting_effect.c_ptr() != NULL
		&& m_downsample_gbuffer_effect.c_ptr() != NULL;
}

void stage_light_propagation_volumes::set_rsm_contants(
	float3 const&	light_direction,
	float3 const&	grid_origin,
	float			grid_scale
)
{
	// FUNCTION BODY[0x614cd0]
	backend::ref().set_ps_constant	(m_c_grid_origin, grid_origin);
	backend::ref().set_ps_constant	(m_c_grid_cell_size, grid_scale / float(m_grid_size));
	backend::ref().set_ps_constant	(m_c_invert_rsm_size, 1.0f / float(m_rsm_downsampled_size));
	backend::ref().set_ps_constant	(m_c_light_direction, light_direction);
}

void stage_light_propagation_volumes::register_rsm_constans( )
{
	// FUNCTION BODY[0x615510]
	m_c_grid_origin					= backend::ref().register_constant_host("grid_origin",  rc_float );
	m_c_grid_cell_size				= backend::ref().register_constant_host("grid_cell_size",  rc_float );
	m_c_grid_size					= backend::ref().register_constant_host("grid_size",  rc_float );
	m_c_invert_rsm_size				= backend::ref().register_constant_host("invert_rsm_size",  rc_float );
}

void stage_light_propagation_volumes::register_light_constans( )
{
	// FUNCTION BODY[0x614de0]
	m_c_view_to_light_matrix		= backend::ref().register_constant_host("view_to_light_matrix",  rc_float );
	m_c_light_color					= backend::ref().register_constant_host("light_color", rc_float );
	m_c_light_intensity				= backend::ref().register_constant_host("light_intensity", rc_float );
	m_c_light_position				= backend::ref().register_constant_host("light_position", rc_float );
	m_c_light_direction				= backend::ref().register_constant_host("light_direction", rc_float );
	m_c_light_attenuation_power		= backend::ref().register_constant_host("light_attenuation_power", rc_float );
	m_c_light_range					= backend::ref().register_constant_host("light_range", rc_float );
	m_c_lighting_model				= backend::ref().register_constant_host("lighting_model", rc_int );
	m_c_diffuse_influence_factor	= backend::ref().register_constant_host("light_diffuse_influence_factor", rc_float );
	m_c_specular_influence_factor	= backend::ref().register_constant_host("light_specular_influence_factor", rc_float );
	m_c_light_spot_penumbra_half_angle_cosine	= backend::ref().register_constant_host( "light_spot_penumbra_half_angle_cosine", rc_float );
	m_c_light_spot_umbra_half_angle_cosine		= backend::ref().register_constant_host( "light_spot_umbra_half_angle_cosine", rc_float );
	m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine
		= backend::ref().register_constant_host( "light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine", rc_float );
	m_c_light_spot_falloff			= backend::ref().register_constant_host("light_spot_falloff", rc_float );
	m_c_light_type					= backend::ref().register_constant_host("light_type", rc_int );
	m_c_light_capsule_half_width	= backend::ref().register_constant_host("light_capsule_half_width", rc_float );
	m_c_light_capsule_radius			= backend::ref().register_constant_host("light_capsule_radius", rc_float );
	m_c_light_sphere_radius			= backend::ref().register_constant_host("light_sphere_radius", rc_float );
	m_c_light_local_to_world		= backend::ref().register_constant_host("light_local_to_world", rc_float );
	m_c_light_sphere_radius			= backend::ref().register_constant_host("light_sphere_radius", rc_float );
}

void stage_light_propagation_volumes::pre_lpv_batch_render(
	float3 const&		light_color,
	float const			light_intensity,
	geometry_batch const& batch
)
{
	// FUNCTION BODY[0x6168f0]
	m_fill_rsm_effect[static_mesh_vertex_input_type]->apply( 0, 0 );
	m_context->set_w( float4x4( ).identity( ) );
	backend::ref( ).set_ps_constant( m_c_light_color, light_color );
	backend::ref( ).set_ps_constant( m_c_light_intensity, light_intensity );
}

void stage_light_propagation_volumes::post_lpv_batch_render( geometry_batch const& )
{
	// FUNCTION BODY[0x614750]
	statistics::ref( ).debug_stat_group.num_dips_in_lpv.value++;
}

void stage_light_propagation_volumes::render_to_rms(
	float3 const&		light_color,
	float const			light_intensity,
	float4x4 const&	view_matrix,
	float4x4 const&	projection_matrix,
	vector<float4x4>	transforms,
	u32 const			cascade_index
)
{
	// FUNCTION BODY[0x616c80]
	if ( !s_draw_to_rsm_value )
		return;

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_rsm_source_size );
	tmp_viewport.Height = float( m_rsm_source_size );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;
	backend::ref( ).set_viewport( tmp_viewport );

	m_context->push_set_v( view_matrix );
	m_context->push_set_p( projection_matrix );

	backend::ref( ).set_render_targets(
		&*m_radiance_volume[cascade_index].m_rt_rms_albedo_source,
		&*m_radiance_volume[cascade_index].m_rt_rms_normal_source,
		&*m_radiance_volume[cascade_index].m_rt_rms_position_source,
		0
	);
	backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );
	backend::ref( ).set_depth_stencil_target( &*m_rms_depth_stencil_source[cascade_index] );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );

	backend::ref( ).set_render_targets(
		&*m_radiance_volume[cascade_index].m_rt_rms_position_source,
		0,
		0,
		0
	);
	for (
		vector<float4x4>::const_iterator it = transforms.begin( ), end = transforms.end( );
		it != end;
		++it
	)
	{
		m_context->set_w( *it );
		m_fill_rsm_effect[null_vertex_input_type]->apply( 1, 0 );
		m_box_geometry.draw( );
	}

	backend::ref( ).set_render_targets(
		&*m_radiance_volume[cascade_index].m_rt_rms_albedo_source,
		&*m_radiance_volume[cascade_index].m_rt_rms_normal_source,
		&*m_radiance_volume[cascade_index].m_rt_rms_position_source,
		0
	);

	if ( s_use_batched_lpv_geometry )
	{
		m_context->scene( )->get_lpv_geometry( ).for_each_batch_render(
			m_context,
			boost::bind(
				&stage_light_propagation_volumes::pre_lpv_batch_render,
				this,
				light_color,
				light_intensity,
				_1
			),
			boost::bind( &stage_light_propagation_volumes::post_lpv_batch_render, this, _1 )
		);
	}
	else
	{
		vector<render_surface_instance*> visible_render_models;
		m_context->scene( )->select_models(
			m_context->get_culling_vp( ),
			visible_render_models,
			m_context->get_view_pos( ),
			visible_flag,
			false
		);

		vector<render_surface_instance*>::iterator it_d = visible_render_models.begin( );
		vector<render_surface_instance*>::const_iterator end_d = visible_render_models.end( );
		for ( ; it_d != end_d; ++it_d )
		{
			render_surface_instance& instance = **it_d;
			material_effects& me = instance.m_render_surface->get_material_effects( );
			render_geometry& geometry = instance.m_render_surface->m_render_geometry;

			if ( instance.m_render_surface->get_vertex_input_type( ) != static_mesh_vertex_input_type )
				continue;

			if ( !me.m_effects[light_propagation_volumes_render_stage] )
				continue;

			if ( !geometry.lpv_pass_geom )
				continue;

			me.m_effects[light_propagation_volumes_render_stage]->apply( 4, 0 );
			backend::ref( ).set_ps_constant( m_c_light_color, light_color );
			backend::ref( ).set_ps_constant( m_c_light_intensity, light_intensity );

			m_context->set_w( *instance.m_transform );
			instance.set_constants( );
			geometry.lpv_pass_geom->apply( );
			backend::ref( ).render_indexed(
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				geometry.primitive_count * 3,
				0,
				0
			);
			statistics::ref( ).debug_stat_group.num_dips_in_lpv.value++;
		}
	}

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).set_viewport( orig_viewport );
	m_context->pop_v( );
	m_context->pop_p( );
}

bool remove_model_if_in_frustum_predicate::operator()( lpv_render_surface const& surface )
{
	// FUNCTION BODY[0x7d230]
	math::aabb bbox = surface.surface->m_parent->get_aabb( );
	bbox *= 2.0f;
	bbox.modify( *surface.surface->m_transform );

	if ( m_frustum->test_inexact( bbox ) == math::intersection_inside )
	{
		statistics::ref( ).lpv_stat_group.num_clipped_dips.value++;
		return true;
	}

	return false;
}

void stage_light_propagation_volumes::render_to_rms_smoothed2(
	float3 const&		light_color,
	float const			light_intensity,
	float4x4 const&	view_matrix,
	float4x4 const&	projection_matrix,
	vector<float4x4>	transforms,
	u32 const			cascade_index,
	u32 const			render_stage_index,
	u32 const			num_render_stages
)
{
	if ( !s_draw_to_rsm_value )
		return;

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_rsm_source_size );
	tmp_viewport.Height = float( m_rsm_source_size );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;
	backend::ref( ).set_viewport( tmp_viewport );

	float4x4 real_view_projection = m_context->get_culling_vp( );
	math::frustum view_frustum( real_view_projection );

	m_context->push_set_v( view_matrix );
	m_context->push_set_p( projection_matrix );

	if ( render_stage_index == 0 )
	{
		device::ref( ).d3d_context( )->CopyResource(
			m_radiance_volume[cascade_index].m_t_rms_albedo_source_temp->hw_texture( ),
			m_radiance_volume[cascade_index].m_t_rms_albedo_source->hw_texture( )
		);
		device::ref( ).d3d_context( )->CopyResource(
			m_radiance_volume[cascade_index].m_t_rms_normal_source_temp->hw_texture( ),
			m_radiance_volume[cascade_index].m_t_rms_normal_source->hw_texture( )
		);
		device::ref( ).d3d_context( )->CopyResource(
			m_radiance_volume[cascade_index].m_t_rms_position_source_temp->hw_texture( ),
			m_radiance_volume[cascade_index].m_t_rms_position_source->hw_texture( )
		);

		backend::ref( ).set_render_targets(
			&*m_radiance_volume[cascade_index].m_rt_rms_albedo_source_temp,
			&*m_radiance_volume[cascade_index].m_rt_rms_normal_source_temp,
			&*m_radiance_volume[cascade_index].m_rt_rms_position_source_temp,
			0
		);
		backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );
		backend::ref( ).set_depth_stencil_target( &*m_rms_depth_stencil_source[cascade_index] );
		backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );

		vector<render_surface_instance*> caster_models;
		m_context->scene( )->select_models(
			m_context->get_culling_vp( ),
			caster_models,
			m_context->get_view_pos( ),
			visible_flag,
			false
		);

		vector<render_surface_instance*>::iterator it = caster_models.begin( );
		vector<render_surface_instance*>::iterator end = caster_models.end( );
		for ( ; it != end; ++it )
		{
			lpv_render_surface surface;
			surface.surface = *it;
			surface.model = surface.surface->m_parent;
			m_caster_models[cascade_index].push_back( surface );
		}

		if ( s_lpv_dips_skipping0_value )
		{
			m_caster_models[cascade_index].erase(
				std::remove_if(
					m_caster_models[cascade_index].begin( ),
					m_caster_models[cascade_index].end( ),
					remove_lpv_inappropriate_models( )
				),
				m_caster_models[cascade_index].end( )
			);
		}

		if ( cascade_index != 0 && s_lpv_dips_skipping1_value )
		{
			math::frustum prev_cascade_frustum(
				m_previous_view_matrix[cascade_index - 1] *
				m_previous_proj_matrix[cascade_index - 1]
			);
			m_caster_models[cascade_index].erase(
				std::remove_if(
					m_caster_models[cascade_index].begin( ),
					m_caster_models[cascade_index].end( ),
					remove_model_if_in_frustum_predicate( prev_cascade_frustum )
				),
				m_caster_models[cascade_index].end( )
			);
		}
	}

	backend::ref( ).set_render_targets(
		&*m_radiance_volume[cascade_index].m_rt_rms_position_source_temp,
		0,
		0,
		0
	);
	backend::ref( ).set_depth_stencil_target( &*m_rms_depth_stencil_source[cascade_index] );
	for (
		vector<float4x4>::const_iterator it = transforms.begin( ), end = transforms.end( );
		it != end;
		++it
	)
	{
		m_context->set_w( *it );
		m_fill_rsm_effect[null_vertex_input_type]->apply( 1, 0 );
		m_box_geometry.draw( );
	}

	backend::ref( ).set_render_targets(
		&*m_radiance_volume[cascade_index].m_rt_rms_albedo_source_temp,
		&*m_radiance_volume[cascade_index].m_rt_rms_normal_source_temp,
		&*m_radiance_volume[cascade_index].m_rt_rms_position_source_temp,
		0
	);

	u32 num_render = m_caster_models[cascade_index].size( ) /
		( num_render_stages - render_stage_index );
	u32 render_index = 0;
	lpv_render_surface* begin_d = m_caster_models[cascade_index].begin( );
	lpv_render_surface* it_d = begin_d;
	lpv_render_surface const* end_d = m_caster_models[cascade_index].end( );
	for ( ; it_d != end_d && render_index < num_render; ++it_d, ++render_index )
	{
		render_surface_instance& instance = *it_d->surface;
		material_effects& me = instance.m_render_surface->get_material_effects( );
		render_geometry& geometry = instance.m_render_surface->m_render_geometry;

		if ( instance.m_render_surface->get_vertex_input_type( ) != static_mesh_vertex_input_type )
			continue;

		if ( !me.m_effects[light_propagation_volumes_render_stage] )
			continue;

		if ( !geometry.lpv_pass_geom && !geometry.geom )
			continue;

		if ( geometry.lpv_pass_geom )
			me.m_effects[light_propagation_volumes_render_stage]->apply( 2, 0 );
		else
			me.m_effects[light_propagation_volumes_render_stage]->apply( 4, 0 );

		backend::ref( ).set_ps_constant( m_c_light_color, light_color );
		backend::ref( ).set_ps_constant( m_c_light_intensity, light_intensity );

		m_context->set_w( *instance.m_transform );
		instance.set_constants( );
		if ( geometry.lpv_pass_geom )
			geometry.lpv_pass_geom->apply( );
		else
			geometry.geom->apply( );

		statistics::ref( ).lpv_stat_group.num_dips.value++;
		switch ( cascade_index )
		{
		case 0:
			statistics::ref( ).lpv_stat_group.num_dips_in_cascade_0.value++;
			break;
		case 1:
			statistics::ref( ).lpv_stat_group.num_dips_in_cascade_1.value++;
			break;
		case 2:
			statistics::ref( ).lpv_stat_group.num_dips_in_cascade_2.value++;
			break;
		}

		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			geometry.primitive_count * 3,
			0,
			0
		);
		statistics::ref( ).debug_stat_group.num_dips_in_lpv.value++;
	}

	m_caster_models[cascade_index].erase( begin_d, it_d );

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).set_viewport( orig_viewport );
	m_context->pop_v( );
	m_context->pop_p( );
}

static float3 compute_aligment( float3 const& lightXZshift, float4x4 const& light_space_transform, float smap_res )
{
	// FUNCTION BODY[0x614760]
	float3 align_origin				(0.0f, 0.0f, 0.0f);

	align_origin					-= lightXZshift;

	float4x4 viewport(	float4(smap_res/2.f,	0.0f,				0.0f,		0.0f),
						float4(0.0f,			-smap_res/2.f,		0.0f,		0.0f),
						float4(0.0f,			0.0f,				1.0f,		0.0f),
						float4(smap_res/2.f,	smap_res/2.f,		0.0f,		1.0f));

	float4x4 viewport_invert				= math::invert4x3(viewport);
	float4x4 light_space_transform_invert	= math::invert4x3(light_space_transform);

	float4	origin_pixel			= light_space_transform.transform(float4(align_origin, 1.f));
	origin_pixel					*= (1.f/origin_pixel.w);
	origin_pixel					= viewport.transform(origin_pixel);

	float const align_granularity	= 4.f;

	origin_pixel.x					= origin_pixel.x / align_granularity - floorf	(origin_pixel.x / align_granularity);
	origin_pixel.y					= origin_pixel.y / align_granularity - floorf	(origin_pixel.y / align_granularity);
	origin_pixel.x					*= align_granularity;
	origin_pixel.y					*= align_granularity;
	origin_pixel.z					= 0;

	origin_pixel.xyz()				= viewport_invert.transform_direction(origin_pixel.xyz());
	origin_pixel.xyz()				= light_space_transform_invert.transform_direction(origin_pixel.xyz());

	return origin_pixel.xyz();
}

void stage_light_propagation_volumes::render_to_sun_rms(
	light*				sun,
	u32 const			cascade_index,
	vector<float4x4>	transforms
)
{
	// FUNCTION BODY[0x6188a0]
	float max_scale					= m_radiance_volume[cascade_index].get_scale();

	float3 sun_position				= m_context->get_view_pos() - sun->direction * max_scale * 1.41421f * 2.0f;// * 20.0f;

	float4x4 view_matrix			= create_camera_direction(sun_position, sun->direction, float3(1, 0, 0));
	float4x4 projection_matrix		= math::create_orthographic_projection(
		max_scale,
		max_scale,
		0.01f,
		1.41421f * max_scale * 20.0f
	);

	float3 adjastment				= compute_aligment(float3(0.0f, 0.0f, 0.0f), view_matrix * projection_matrix, m_rsm_source_size);
	view_matrix						= create_camera_direction(sun_position + adjastment, sun->direction, float3(1, 0, 0));

	render_to_rms					(sun->color, sun->intensity, view_matrix, projection_matrix, transforms, cascade_index);
}

void stage_light_propagation_volumes::render_to_sun_rms_smoothed(
	light*				sun,
	u32 const		cascade_index,
	vector<float4x4>	transforms,
	u32 const		stage_render_index,
	u32 const		num_render_stages
)
{
	float max_scale = m_radiance_volume[cascade_index].get_scale( );



	float3 sun_position = start_render_eye_position + -sun->direction * max_scale * 1.41421f * 2.0f;

	float4x4 view_matrix = create_camera_direction( sun_position, sun->direction, float3( 1, 0, 0 ) );
	float4x4 projection_matrix = math::create_orthographic_projection(
		max_scale,
		max_scale,
		0.01f,
		1.41421f * max_scale * 100.0f
	);

	if ( stage_render_index == 0 )
	{
		m_previous_view_matrix[cascade_index] = view_matrix;
		m_previous_proj_matrix[cascade_index] = projection_matrix;
	}

	float3 adjastment = compute_aligment(
		float3( 0.0f, 0.0f, 0.0f ),
		view_matrix * projection_matrix,
		m_rsm_source_size
	);
	view_matrix = create_camera_direction( sun_position + adjastment, sun->direction, float3( 1, 0, 0 ) );

	render_to_rms_smoothed2(
		sun->color,
		sun->intensity,
		view_matrix,
		projection_matrix,
		transforms,
		cascade_index,
		stage_render_index,
		num_render_stages
	);
}

static float3 view_matrix_parameters[6][3] = {
	{float3(0.0f, 0.0f, 0.0f), float3(1.0f, 0.0f, 0.0f),	float3(0.0f, 1.0f, 0.0f)},	//  x
	{float3(0.0f, 0.0f, 0.0f), float3(-1.0f, 0.0f, 0.0f),	float3(0.0f, 1.0f, 0.0f)},	// -x
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f),	float3(0.0f, 0.0f, -1.0f)},	//  y
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, -1.0f, 0.0f),	float3(0.0f, 0.0f, 1.0f)},	// -y
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f),	float3(0.0f, 1.0f, 0.0f)},	//  z
	{float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, -1.0f),	float3(0.0f, 1.0f, 0.0f)},	// -z
};

void stage_light_propagation_volumes::render_to_point_rms(
	light*				l,
	u32 const			face_index,
	vector<float4x4>	transforms
)
{
	// FUNCTION BODY[0x618750]
	math::float4x4 face_view_matrix = math::create_camera_at(
		l->position + view_matrix_parameters[face_index][0],
		l->position + view_matrix_parameters[face_index][1],
		view_matrix_parameters[face_index][2]
	);

	math::float4x4 face_projection_matrix = math::create_perspective_projection(math::pi_d2, 1.0f, 0.1f, l->range);

	render_to_rms					(l->color, l->intensity, face_view_matrix, face_projection_matrix, transforms, 0);
}

void stage_light_propagation_volumes::render_to_sky_rms(
	light*				sun,
	u32 const			face_index,
	u32 const			cascade_index,
	vector<float4x4>	transforms
)
{
	float3 light_color = m_context->scene_view()->post_process_parameters().environment_skycolor[face_index].xyz();
	float const light_intensity = 1.0f;





	float const max_scale = m_radiance_volume[cascade_index].get_scale();

	float4x4 sun_rotation = sun
		? math::create_rotation(float3(sun->direction.x, 0.0f, sun->direction.z))
		: float4x4().identity();

	float3 direction = math::normalize(sun_rotation.transform_direction(view_matrix_parameters[face_index][1]));
	float3 up = math::normalize(math::cross_product(
		math::normalize(math::cross_product(direction, float3(1.0f, 1.0f, 1.0f))),
		direction
	));

	float3 sky_position = m_context->get_view_pos() - direction * max_scale * 1.41421f * 2.0f;

	float4x4 view_matrix = create_camera_direction(sky_position, direction, up);
	float4x4 projection_matrix = math::create_orthographic_projection(
		max_scale * 1.41421f,
		max_scale * 1.41421f,
		0.01f,
		1.41421f * max_scale * 20.0f
	);

	float3 adjastment = compute_aligment(float3(0.0f, 0.0f, 0.0f), view_matrix * projection_matrix, m_rsm_source_size);
	view_matrix = create_camera_direction(sky_position + adjastment, direction, up);

	render_to_rms(light_color, light_intensity, view_matrix, projection_matrix, transforms, cascade_index);
}

void stage_light_propagation_volumes::render_to_spot_rms( light* l, vector<float4x4> transforms )
{
	// FUNCTION BODY[0x618010]
	float const max_angle			 = math::max(l->spot_umbra_angle, l->spot_penumbra_angle);

	math::float4x4 projection_matrix = math::create_perspective_projection(max_angle, 1.0f, l->range / 1000.0f, l->range);

	math::float4x4 view_matrix		 = math::create_camera_at(l->position,
															  l->position + l->direction,
															  math::normalize(math::cross_product(l->direction, l->right)));

	render_to_rms					(l->color, l->intensity, view_matrix, projection_matrix, transforms, 0);
}

void stage_light_propagation_volumes::propagate_lighting( u32 const cascade_index )
{
	// FUNCTION BODY[0x615f50]
	m_radiance_volume[cascade_index].propagate_lighting(cascade_index);
}

void stage_light_propagation_volumes::propagate_lighting_smoothed(
	u32 const cascade_index,
	u32 const propagation_iteration_index
)
{
	// FUNCTION BODY[0x615f30]
	m_radiance_volume[cascade_index].propagate_lighting_iter(
		cascade_index,
		propagation_iteration_index
	);
}

void stage_light_propagation_volumes::render_quad( )
{
	// FUNCTION BODY[0x615760]
	u32	offset;

	screen_vertex* pv = (screen_vertex*)backend::ref().vertex.lock(4, sizeof(screen_vertex), offset);
	pv->set(float4(-1.0f, -1.0f, 0.0f, 1.0f), float2(0.0f, 1.0f)); pv++;
	pv->set(float4(-1.0f,  1.0f, 0.0f, 1.0f), float2(0.0f, 0.0f)); pv++;
	pv->set(float4( 1.0f, -1.0f, 0.0f, 1.0f), float2(1.0f, 1.0f)); pv++;
	pv->set(float4( 1.0f,  1.0f, 0.0f, 1.0f), float2(1.0f, 0.0f)); pv++;
	backend::ref().vertex.unlock			();

	m_screen_vertex_geometry->apply			();
	backend::ref().render_indexed			(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

void stage_light_propagation_volumes::downsample_rsm(
	float3 const&	light_direction,
	float3 const&	grid_origin,
	float			grid_scale,
	u32 const		cascade_index
)
{
	// FUNCTION BODY[0x615d50]
	m_downsample_rsm_effect->apply( 0, 0 );
	set_rsm_contants( light_direction, grid_origin, grid_scale );

	backend::ref( ).set_render_targets(
		&*m_radiance_volume[cascade_index].m_rt_rms_albedo,
		&*m_radiance_volume[cascade_index].m_rt_rms_normal,
		&*m_radiance_volume[cascade_index].m_rt_rms_position,
		0
	);
	backend::ref( ).set_depth_stencil_target( 0 );

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_rsm_downsampled_size );
	tmp_viewport.Height = float( m_rsm_downsampled_size );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;
	backend::ref( ).set_viewport( tmp_viewport );

	backend::ref( ).set_ps_texture(
		"t_lpv_rsm_albedo_source",
		&*m_radiance_volume[cascade_index].m_t_rms_albedo_source
	);
	backend::ref( ).set_ps_texture(
		"t_lpv_rsm_normal_source",
		&*m_radiance_volume[cascade_index].m_t_rms_normal_source
	);
	backend::ref( ).set_ps_texture(
		"t_lpv_rsm_position_source",
		&*m_radiance_volume[cascade_index].m_t_rms_position_source
	);

	render_quad( );

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).set_viewport( orig_viewport );
}

void stage_light_propagation_volumes::inject_lighting(
	u32 const		cascade_index,
	float3 const&	light_position,
	float3 const&	light_direction,
	float			light_fov
)
{
	m_radiance_volume[cascade_index].inject_lighting(light_position, light_direction, light_fov, m_rsm_downsampled_size);
}

void stage_light_propagation_volumes::inject_occluders(
	u32 const		cascade_index,
	float3 const&	light_position,
	float3 const&	light_direction,
	vector<float4x4>	transforms
)
{
	m_radiance_volume[cascade_index].inject_occluders(m_context, light_position, light_direction, m_rsm_downsampled_size);
	m_radiance_volume[cascade_index].inject_occluder_geometry(
		m_context,
		light_position,
		light_direction,
		transforms
	);
}

static float blend_alpha = 0.0f;

void stage_light_propagation_volumes::execute_smoothed_impl(
	u32 const current_cascade_index,
	u32 const stage_index,
	u32 const propagation_iteration_index,
	u32 const render_stage_index,
	u32 const num_render_stages
)
{
	// FUNCTION BODY[0x6173c0]
	light_ptr sun = m_context->scene( )->lights( ).get_sun( );

	m_has_indirect_lighting = false;

	if ( sun && sun->use_with_lpv )
		m_has_indirect_lighting = true;

	m_has_indirect_lighting = !!m_has_indirect_lighting;

	vector< float4x4 > box_occluder_transforms;
	for (
		associative_vector< u32, float4x4, vector, std::less< u32 > >::const_iterator it =
			m_context->scene( )->lpv_occluders( ).begin( ),
			end = m_context->scene( )->lpv_occluders( ).end( );
		it != end;
		++it
	)
	{
		box_occluder_transforms.push_back( it->second );
	}

	if ( m_has_indirect_lighting )
	{
		if ( stage_index == 0 )
		{
			m_radiance_volume[current_cascade_index].prepare_gv( );
			m_radiance_volume[current_cascade_index].prepare(
				m_context->get_view_pos( ),
				m_context->get_view_dir( ),
				0.2f
			);
		}

		backend::ref( ).set_depth_stencil_target( &*m_context->get_rt( rt_apply_indirect_lighting_ds ) );
		backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );
		backend::ref( ).reset_depth_stencil_target( );

		m_radiance_volume[current_cascade_index].inject_camera_occluders( m_context );

		if ( sun && sun->use_with_lpv )
		{
			if ( stage_index == 0 )
				start_render_eye_position = m_context->get_view_pos( );

			if ( !options::ref( ).current.m_lpv_disable_rsm_generating )
			{
				render_to_sun_rms_smoothed(
					sun.c_ptr( ),
					current_cascade_index,
					box_occluder_transforms,
					render_stage_index,
					num_render_stages
				);
			}

			if ( stage_index == 0 )
			{
				if ( !options::ref( ).current.m_lpv_disable_rsm_downsampling )
				{
					downsample_rsm(
						sun->direction,
						m_radiance_volume[current_cascade_index].get_origin( ),
						m_radiance_volume[current_cascade_index].get_scale( ),
						current_cascade_index
					);
				}
			}
			else if ( stage_index == 1 )
			{
				if ( !options::ref( ).current.m_lpv_disable_vpl_injection )
				{
					inject_lighting(
						current_cascade_index,
						sun->position,
						sun->direction,
						1.0f
					);
				}
			}
			else if ( stage_index == 2 )
			{
				if ( !options::ref( ).current.m_lpv_disable_gv_injection )
				{
					inject_occluders(
						current_cascade_index,
						sun->position,
						sun->direction,
						box_occluder_transforms
					);
				}
			}

			m_has_indirect_lighting = true;
		}

		if ( stage_index > 2 && !options::ref( ).current.m_lpv_disable_propagation )
		{
			m_radiance_volume[current_cascade_index].propagate_lighting_iter(
				current_cascade_index,
				propagation_iteration_index
			);
		}

		if ( blend_alpha > 1.0f )
			blend_alpha = 0.0f;
	}

	if ( propagation_iteration_index == options::ref( ).current.m_num_propagate_iterations - 1 )
		m_radiance_volume[current_cascade_index].prepare_final( );

	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_lpv_accumulation ), 0, 0, 0 );
	backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );

	if ( m_has_indirect_lighting && !options::ref( ).current.m_lpv_disable_lpv_lookup )
	{
		D3D11_VIEWPORT orig_viewport;
		backend::ref( ).get_viewport( orig_viewport );

		D3D11_VIEWPORT tmp_viewport;
		tmp_viewport.TopLeftX = 0.0f;
		tmp_viewport.TopLeftY = 0.0f;
		tmp_viewport.MinDepth = 0.0f;
		tmp_viewport.MaxDepth = 1.0f;

		for ( u32 pass_index = 0; pass_index < 1; ++pass_index )
		{
			for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
			{
				if ( pass_index == 1 )
				{
					backend::ref( ).set_render_targets( &*m_context->get_rt( rt_indirect_lighting_specular ), 0, 0, 0 );
					m_apply_indirect_lighting_effect->apply_pass( 1 );
					tmp_viewport.Width = float( m_context->get_rt( rt_indirect_lighting_specular )->width( ) );
					tmp_viewport.Height = float( m_context->get_rt( rt_indirect_lighting_specular )->height( ) );
				}
				else
				{
					backend::ref( ).set_render_targets( &*m_context->get_rt( rt_lpv_accumulation ), 0, 0, 0 );
					m_apply_indirect_lighting_effect->apply_pass( 0 );
					tmp_viewport.Width = float( m_context->get_rt( rt_lpv_accumulation )->width( ) );
					tmp_viewport.Height = float( m_context->get_rt( rt_lpv_accumulation )->height( ) );
				}

				backend::ref( ).set_viewport( tmp_viewport );
				backend::ref( ).set_depth_stencil_target( 0 );

				backend::ref( ).set_ps_texture(
					"t_radiance_r",
					&*m_radiance_volume[cascade_index].get_accumulated_propagation_r( )
				);
				backend::ref( ).set_ps_texture(
					"t_radiance_g",
					&*m_radiance_volume[cascade_index].get_accumulated_propagation_g( )
				);
				backend::ref( ).set_ps_texture(
					"t_radiance_b",
					&*m_radiance_volume[cascade_index].get_accumulated_propagation_b( )
				);

				backend::ref( ).set_ps_constant( m_c_cascade_index, cascade_index );
				backend::ref( ).set_ps_constant( m_c_num_cascades, m_num_cascades );
				backend::ref( ).set_ps_constant(
					m_c_ambient_color,
					math::pow( m_context->scene_view( )->post_process_parameters( ).environment_ambient_color, 2.2f )
				);
				backend::ref( ).set_ps_constant(
					m_c_grid_cell_size,
					m_radiance_volume[cascade_index].get_scale( ) / float( m_grid_size )
				);
				backend::ref( ).set_ps_constant( m_c_grid_size, float( m_grid_size ) );
				backend::ref( ).set_ps_constant(
					m_c_grid_origin,
					m_radiance_volume[cascade_index].get_previous_origin( )
				);

				blend_alpha += m_context->get_time_delta( );
				backend::ref( ).set_ps_constant( m_c_radiance_blend_factor, blend_alpha );

				bool const first = cascade_index == 0;
				float smaller_cascade_grid_cell_size =
					m_radiance_volume[cascade_index - ( first ? 0 : 1 )].get_scale( ) / float( m_grid_size );
				float smaller_cascade_grid_size = float( m_grid_size );
				float3 smaller_cascade_grid_origin =
					m_radiance_volume[cascade_index - ( first ? 0 : 1 )].get_previous_origin( );

				backend::ref( ).set_ps_constant(
					m_c_smaller_cascade_grid_cell_size,
					smaller_cascade_grid_cell_size
				);
				backend::ref( ).set_ps_constant( m_c_smaller_cascade_grid_size, smaller_cascade_grid_size );
				backend::ref( ).set_ps_constant(
					m_c_smaller_cascade_grid_origin,
					smaller_cascade_grid_origin
				);
				backend::ref( ).set_ps_constant(
					m_c_interreflection_contribution,
					options::ref( ).current.m_lpv_interreflection_contribution
				);

				float3 const* const eye_rays = m_context->get_eye_rays( );
				backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( ( float4* )eye_rays )[0] );

				render_quad( );
				backend::ref( ).reset_render_targets( );
				backend::ref( ).reset_depth_stencil_target( );
			}
		}

		backend::ref( ).set_viewport( orig_viewport );
	}
}

void stage_light_propagation_volumes::execute_impl( )
{
	light_ptr sun = m_context->scene( )->lights( ).get_sun( );

	lights_db::lights_type const& e_lights = m_context->scene( )->lights( ).get_lights( );

	collision::objects_type objects = g_allocator;
	objects.reserve( e_lights.size( ) );

	math::frustum frustum = m_context->get_culling_vp( );
	m_context->scene( )->lights( ).lights_tree( ).cuboid_query( u32( -1 ), frustum, objects );

	bool is_need_refresh = m_context->scene_view( )->get_render_frame_index( ) %
		options::ref( ).current.m_lpv_refresh_once_per_frames == 0;

	m_has_indirect_lighting = false;

	if ( sun && sun->use_with_lpv )
	{
		m_has_indirect_lighting = true;
	}

	for ( collision::objects_type::const_iterator i = objects.begin( ), e = objects.end( ); i != e; ++i )
	{
		light* l = static_cast< light* >( ( *i )->user_data( ) );

		if ( !l->use_with_lpv )
			continue;

		switch ( l->get_type( ) )
		{
		case light_type_spot:
		case light_type_point:
			m_has_indirect_lighting = true;
			break;
		default:
			break;
		}
	}

	m_has_indirect_lighting = !!m_has_indirect_lighting;

	vector< float4x4 > box_occluder_transforms;
	for (
		associative_vector< u32, float4x4, vector, std::less< u32 > >::const_iterator it =
			m_context->scene( )->lpv_occluders( ).begin( ),
			end = m_context->scene( )->lpv_occluders( ).end( );
		it != end;
		++it
	)
	{
		box_occluder_transforms.push_back( it->second );
	}

	if ( is_need_refresh && m_has_indirect_lighting )
	{
		for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
			m_radiance_volume[cascade_index].prepare_gv( );

		backend::ref( ).set_depth_stencil_target( &*m_context->get_rt( rt_apply_indirect_lighting_ds ) );
		backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );
		backend::ref( ).reset_depth_stencil_target( );

		for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
		{
			m_radiance_volume[cascade_index].prepare(
				m_context->get_view_pos( ),
				m_context->get_view_dir( ),
				0.2f
			);
		}

		for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
			m_radiance_volume[cascade_index].inject_camera_occluders( m_context );

		if ( sun && sun->use_with_lpv )
		{
			for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
			{
				if ( !options::ref( ).current.m_lpv_disable_rsm_generating )
					render_to_sun_rms( sun.c_ptr( ), cascade_index, box_occluder_transforms );

				if ( !options::ref( ).current.m_lpv_disable_rsm_downsampling )
				{
					downsample_rsm(
						sun->direction,
						m_radiance_volume[cascade_index].get_origin( ),
						m_radiance_volume[cascade_index].get_scale( ),
						cascade_index
					);
				}

				if ( !options::ref( ).current.m_lpv_disable_vpl_injection )
				{
					m_radiance_volume[cascade_index].inject_lighting(
						sun->position,
						sun->direction,
						1.0f,
						m_rsm_downsampled_size
					);
				}

				if ( !options::ref( ).current.m_lpv_disable_gv_injection )
					inject_occluders( cascade_index, sun->position, sun->direction, box_occluder_transforms );
			}

			m_has_indirect_lighting = true;
		}

		for ( collision::objects_type::const_iterator i = objects.begin( ), e = objects.end( ); i != e; ++i )
		{
			light* l = static_cast< light* >( ( *i )->user_data( ) );

			if ( !l->use_with_lpv )
				continue;

			switch ( l->get_type( ) )
			{
			case light_type_spot:
				if ( !options::ref( ).current.m_lpv_disable_rsm_generating )
					render_to_spot_rms( l, box_occluder_transforms );

				for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
				{
					backend::ref( ).flush_rt_shader_resources( );

					if ( !options::ref( ).current.m_lpv_disable_rsm_downsampling )
					{
						downsample_rsm(
							l->direction,
							m_radiance_volume[cascade_index].get_origin( ),
							m_radiance_volume[cascade_index].get_scale( ),
							cascade_index
						);
					}

					if ( !options::ref( ).current.m_lpv_disable_vpl_injection )
					{
						m_radiance_volume[cascade_index].inject_lighting(
							l->position,
							l->direction,
							math::max( l->spot_umbra_angle, l->spot_penumbra_angle ),
							m_rsm_downsampled_size
						);
					}

					if ( !options::ref( ).current.m_lpv_disable_gv_injection )
						inject_occluders( cascade_index, l->position, l->direction, box_occluder_transforms );
				}

				m_has_indirect_lighting = true;
				break;

			case light_type_point:
				for ( s32 face_index = 0; face_index < 6; ++face_index )
				{
					float3 face_direction = view_matrix_parameters[face_index][1];

					if ( !options::ref( ).current.m_lpv_disable_rsm_generating )
						render_to_point_rms( l, face_index, box_occluder_transforms );

					for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
					{
						backend::ref( ).flush_rt_shader_resources( );

						if ( !options::ref( ).current.m_lpv_disable_rsm_downsampling )
						{
							downsample_rsm(
								face_direction,
								m_radiance_volume[cascade_index].get_origin( ),
								m_radiance_volume[cascade_index].get_scale( ),
								cascade_index
							);
						}

						if ( !options::ref( ).current.m_lpv_disable_vpl_injection )
						{
							m_radiance_volume[cascade_index].inject_lighting(
								l->position,
								face_direction,
								math::pi_d2,
								m_rsm_downsampled_size
							);
						}

						if ( !options::ref( ).current.m_lpv_disable_gv_injection )
							inject_occluders( cascade_index, l->position, face_direction, box_occluder_transforms );
					}

					m_has_indirect_lighting = true;
				}
				break;
			}
		}

		if ( !options::ref( ).current.m_lpv_disable_propagation )
		{
			for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
				m_radiance_volume[cascade_index].propagate_lighting( cascade_index );
		}

		if ( blend_alpha > 1.0f )
			blend_alpha = 0.0f;
	}

	if ( m_has_indirect_lighting && !options::ref( ).current.m_lpv_disable_lpv_lookup )
	{
		D3D11_VIEWPORT orig_viewport;
		backend::ref( ).get_viewport( orig_viewport );

		D3D11_VIEWPORT tmp_viewport;
		tmp_viewport.TopLeftX = 0.0f;
		tmp_viewport.TopLeftY = 0.0f;
		tmp_viewport.MinDepth = 0.0f;
		tmp_viewport.MaxDepth = 1.0f;

		for ( u32 pass_index = 0; pass_index < 1; ++pass_index )
		{
			for ( s32 cascade_index = m_num_cascades - 1; cascade_index >= 0; --cascade_index )
			{
				if ( pass_index == 1 )
				{
					backend::ref( ).set_render_targets( &*m_context->get_rt( rt_indirect_lighting_specular ), 0, 0, 0 );
					m_apply_indirect_lighting_effect->apply_pass( 1 );
					tmp_viewport.Width = float( m_context->get_rt( rt_indirect_lighting_specular )->width( ) );
					tmp_viewport.Height = float( m_context->get_rt( rt_indirect_lighting_specular )->height( ) );
				}
				else
				{
					backend::ref( ).set_render_targets( &*m_context->get_rt( rt_accumulator_diffuse ), 0, 0, 0 );
					m_apply_indirect_lighting_effect->apply_pass( 0 );
					tmp_viewport.Width = float( m_context->get_rt( rt_accumulator_diffuse )->width( ) );
					tmp_viewport.Height = float( m_context->get_rt( rt_accumulator_diffuse )->height( ) );
				}

				backend::ref( ).set_viewport( tmp_viewport );
				backend::ref( ).set_depth_stencil_target( 0 );

				backend::ref( ).set_ps_texture(
					"t_radiance_r",
					&*m_radiance_volume[cascade_index].get_accumulated_propagation_r( )
				);
				backend::ref( ).set_ps_texture(
					"t_radiance_g",
					&*m_radiance_volume[cascade_index].get_accumulated_propagation_g( )
				);
				backend::ref( ).set_ps_texture(
					"t_radiance_b",
					&*m_radiance_volume[cascade_index].get_accumulated_propagation_b( )
				);

				backend::ref( ).set_ps_constant( m_c_cascade_index, cascade_index );
				backend::ref( ).set_ps_constant( m_c_num_cascades, m_num_cascades );
				backend::ref( ).set_ps_constant(
					m_c_ambient_color,
					math::pow( m_context->scene_view( )->post_process_parameters( ).environment_ambient_color, 2.2f )
				);
				backend::ref( ).set_ps_constant(
					m_c_grid_cell_size,
					m_radiance_volume[cascade_index].get_scale( ) / float( m_grid_size )
				);
				backend::ref( ).set_ps_constant( m_c_grid_size, float( m_grid_size ) );
				backend::ref( ).set_ps_constant( m_c_grid_origin, m_radiance_volume[cascade_index].get_origin( ) );

				blend_alpha += m_context->get_time_delta( );
				backend::ref( ).set_ps_constant( m_c_radiance_blend_factor, blend_alpha );

				bool const first = cascade_index == 0;
				float smaller_cascade_grid_cell_size =
					m_radiance_volume[cascade_index - ( first ? 0 : 1 )].get_scale( ) / float( m_grid_size );
				float smaller_cascade_grid_size = float( m_grid_size );
				float3 smaller_cascade_grid_origin =
					m_radiance_volume[cascade_index - ( first ? 0 : 1 )].get_origin( );

				backend::ref( ).set_ps_constant(
					m_c_smaller_cascade_grid_cell_size,
					smaller_cascade_grid_cell_size
				);
				backend::ref( ).set_ps_constant( m_c_smaller_cascade_grid_size, smaller_cascade_grid_size );
				backend::ref( ).set_ps_constant(
					m_c_smaller_cascade_grid_origin,
					smaller_cascade_grid_origin
				);
				backend::ref( ).set_ps_constant(
					m_c_interreflection_contribution,
					options::ref( ).current.m_lpv_interreflection_contribution
				);

				float3 const* const eye_rays = m_context->get_eye_rays( );
				backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ( ( float4* )eye_rays )[0] );

				render_quad( );
				backend::ref( ).reset_render_targets( );
				backend::ref( ).reset_depth_stencil_target( );
			}
		}

		backend::ref( ).set_viewport( orig_viewport );
	}
}

void stage_light_propagation_volumes::execute_disabled( )
{
	// FUNCTION BODY[0x615690]
	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_accumulator_diffuse ), 0, 0, 0 );
	backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );
}

void stage_light_propagation_volumes::execute( )
{
	// FUNCTION BODY[0x619b30]
	if ( !is_effects_ready( ) )
		return;

	if ( !options::ref( ).current.m_lighting_quality || !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	if ( s_use_smooothed_lpv_value )
	{
		u32 const num_render_stages = options::ref( ).current.m_num_propagate_iterations + 3;
		u32 const render_stage_index = m_context->scene_view( )->get_render_frame_index( ) % num_render_stages;
		u32 propagation_step_index = 0;
		if ( render_stage_index > 2 )
			propagation_step_index = render_stage_index - 3;

		for ( u32 cascade_index = 0; cascade_index < m_num_cascades; ++cascade_index )
		{
			execute_smoothed_impl(
				cascade_index,
				render_stage_index,
				propagation_step_index,
				render_stage_index,
				num_render_stages
			);
		}
	}
	else
	{
		execute_impl( );
	}
}

void stage_light_propagation_volumes::draw_debug( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6146d0]
}

} // namespace render
} // namespace vostok
