// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "stage_shadow_direct.h"
#include "remove_inappropriate_models.h"
#include "remove_model_if_in_frustum_predicate.h"
#include "shadow_cascade_volume.h"

#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/resource_manager.h>
#include "effect_shadow_map.h"
#include "grass_world.h"
#include "light.h"
#include "material_effects.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"
#include "lights_db.h"
#include "statistics.h"

namespace vostok {
namespace render {
static bool s_test_shadow_in_frustum = false;
static console_commands::cc_bool s_test_shadow_in_frustum_cc( "r_test_shadow_in_frustum", s_test_shadow_in_frustum, false, console_commands::command_type_user_specific );

static bool s_test_shadow_culling0 = true;
static console_commands::cc_bool s_test_shadow_culling0_cc( "r_test_shadow_culling0", s_test_shadow_culling0, false, console_commands::command_type_user_specific );

static bool s_test_shadow_culling1 = true;
static console_commands::cc_bool s_test_shadow_culling1_cc( "r_test_shadow_culling1", s_test_shadow_culling1, false, console_commands::command_type_user_specific );

static bool s_draw_grass_shadows_value = true;
static console_commands::cc_bool s_draw_grass_shadows_value_cc( "r_draw_grass_shadows", s_draw_grass_shadows_value, false, console_commands::command_type_user_specific );

static bool s_sun_moving_value = false;
static console_commands::cc_bool s_sun_moving( "sun_moving", s_sun_moving_value, false, console_commands::command_type_user_specific );

static bool s_use_batched_shadow_geometry_value = false;
static console_commands::cc_bool s_use_batched_shadow_geometry_cc( "use_batched_shadow_geometry", s_use_batched_shadow_geometry_value, false, console_commands::command_type_user_specific );

static bool s_one_cascade_value = false;
static console_commands::cc_bool s_one_cascade( "one_cascade_value", s_one_cascade_value, false, console_commands::command_type_user_specific );

static u32 s_shadows_in_cascade_value = 5;
static console_commands::cc_u32 s_shadows_in_cascade( "shadows_in_cascade", s_shadows_in_cascade_value, 0, 5, true, console_commands::command_type_user_specific );

namespace {

static float3 corners[8] = {
	float3( -1.f, -1.f, 0.f ), float3( -1.f, -1.f, 1.f ),
	float3( -1.f, 1.f, 1.f ), float3( -1.f, 1.f, 0.f ),
	float3( 1.f, 1.f, 1.f ), float3( 1.f, 1.f, 0.f ),
	float3( 1.f, -1.f, 1.f ), float3( 1.f, -1.f, 0.f )
};

static s32 facetable[6][4] = {
	{ 6, 7, 5, 4 },
	{ 1, 0, 7, 6 },
	{ 1, 2, 3, 0 },
	{ 3, 2, 4, 5 },
	{ 0, 3, 5, 7 },
	{ 1, 6, 4, 2 }
};
} // namespace

stage_shadow_direct::stage_shadow_direct(
	renderer*			in_renderer,
	renderer_context*	context
	) : stage( in_renderer, context )
{
	m_invalid_shadow = false;
	m_cascade_texture_index[0] = false;
	m_cascade_texture_index[1] = false;
	m_cascade_texture_index[2] = false;
	m_cascade_texture_index[3] = false;

	effect_manager::ref().create_effect<effect_shadow_map>(&m_effect_shadow_direct);

	m_c_light_direction = backend::ref().register_constant_host( "light_direction", rc_float );
	m_c_light_position = backend::ref().register_constant_host( "light_position", rc_float );
	m_c_light_attenuation_power = backend::ref().register_constant_host( "light_attenuation", rc_float );
	m_c_start_corner = backend::ref().register_constant_host("start_corner", rc_float);
	m_wind_info_parameters = backend::ref().register_constant_host( "wind_info_parameters", rc_float );
	m_shadow_cascade_index = backend::ref().register_constant_host( "shadow_cascade_index", rc_int );
	m_enabled = options::ref().current.m_enabled_sun_shadows_stage;
	m_cascade_shadow_map_size = options::ref( ).current.m_cascaded_shadow_map_size;
	for ( u32 i = 0; i < 4; ++i )
	{
		m_previous_adjastment[i] = float3( 0.0f, 0.0f, 0.0f );
	}
	m_rt_shadow_map = resource_manager::ref( ).create_render_target( "$user$cascaded_shadow_map", m_cascade_shadow_map_size * 2, m_cascade_shadow_map_size * 2, DXGI_FORMAT_R16_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_shadow_map = resource_manager::ref( ).create_texture( "$user$cascaded_shadow_map", 0, 0, false, true, true, u32(-1) );
	for ( u32 i = 0; i < 4; ++i )
	{
		m_view_to_shadow[i] = float4x4( ).identity( );
		m_previous_view_matrix[i] = float4x4( ).identity( );
		m_previous_projection_matrix[i] = float4x4( ).identity( );
		m_previous_real_view_matrix[i] = float4x4( ).identity( );
		m_previous_position[i] = float3( 0.0f, 0.0f, 0.0f );
		m_prev_view_to_shadow[i] = float4x4( ).identity( );
	}
	m_previous_direction = float3( 0.0f, 0.0f, 1.0f );
}

bool stage_shadow_direct::is_effects_ready( ) const
{

	return m_effect_shadow_direct.c_ptr() != NULL;
}
stage_shadow_direct::~stage_shadow_direct( )
{

}
void stage_shadow_direct::execute_disabled( )
{
	if ( !is_effects_ready( ) )
		return;

	light* sun = &*m_context->scene( )->lights( ).get_sun( ).c_ptr( );
	if ( !sun )
		return;
	backend::ref( ).set_render_targets( 0, 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( &*m_rt_shadow_map );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );
}

void stage_shadow_direct::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	light* sun = &*m_context->scene( )->lights( ).get_sun( ).c_ptr( );

	if ( !is_enabled( ) || (sun && !sun->is_cast_shadows( )) )
	{
		execute_disabled( );
		return;
	}

	if ( !sun )
		return;

	static math::float3 const s_prev_sun_direction = sun->direction;
	m_t_shadow_map = m_context->m_t_shadow_cascade;

	backend::ref( ).set_render_targets( 0, 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( &*m_rt_shadow_map );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );

	u32 first_cascade = u32(sun_cascade::num_max_sun_shadow_cascades) - options::ref( ).current.m_num_shadow_cascades;
	for ( u32 cascade_id = first_cascade, cascade_index = 0; cascade_id < sun_cascade::num_max_sun_shadow_cascades; ++cascade_id, ++cascade_index )
		execute_cascade( cascade_id, cascade_index, m_cascade_shadow_map_size );

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );

	m_invalid_shadow = false;
}

void stage_shadow_direct::draw_debug( u32 in_cascade_id )
{

	(void)&in_cascade_id;

	m_context->set_w					( float4x4().identity() );
}

bool remove_model_if_in_frustum_predicate::operator()(
	render_surface_instance* in_model
)
{

	math::aabb bbox = in_model->m_parent->get_aabb( );
	bbox *= float3( 2.f, 3.f, 2.f );
	bbox.modify( *in_model->m_transform );

	if ( m_frustum->test_inexact( bbox ) == math::intersection_inside ) {
		++statistics::ref( ).cascaded_sun_shadow_stat_group.num_clipped_dips.value;
		return true;
	}

	return false;
}

bool remove_inappropriate_models::operator()( render_surface_instance* in_model )
{
	float dynamic_screen_factor = in_model->m_render_surface->get_material_effects( ).has_translucency ? 0.0001f : 0.0004f;
	if ( !options::ref( ).current.m_shadow_quality )

		dynamic_screen_factor = in_model->m_render_surface->get_material_effects( ).has_translucency ? 0.0004f : 0.0016f;
	if ( dynamic_screen_factor > in_model->m_dynamic_screen_factor || in_model->is_occluded( ) )
	{
		++statistics::ref( ).cascaded_sun_shadow_stat_group.num_clipped_dips.value;
		return true;
	}

	if ( options::ref( ).current.m_shadow_quality || !s_test_shadow_in_frustum )
		return false;

	math::aabb bbox = in_model->m_parent->get_aabb( );
	bbox.modify( *in_model->m_transform );

	return m_frustum->test_inexact( bbox ) == math::intersection_outside;
}
void stage_shadow_direct::prepare_models(
	vector<render_surface_instance*>&	m_caster_model,
	float4x4 const&						orig_view_projection,
	u32 const							cascade_index,
	u32 const							shadow_map_size,
	float3 const&						view_pos
	)
{

	m_context->scene( )->select_models(
		m_context->get_culling_vp( ),
		m_caster_model,
		view_pos,
		cast_shadow_flag,
		false
	);

	if ( cascade_index ) {
		math::frustum view_frustum(
			m_previous_view_matrix[cascade_index - 1] *
			m_previous_projection_matrix[cascade_index - 1]
		);

		if ( s_test_shadow_culling0 )
			m_caster_model.erase(
				std::remove_if(
					m_caster_model.begin( ),
					m_caster_model.end( ),
					remove_model_if_in_frustum_predicate( view_frustum )
				),
				m_caster_model.end( )
			);
	}
	vector<render_surface_instance*> opaque_models;
	render_surface_instance* const* i = m_caster_model.begin( );

	render_surface_instance* const* e = m_caster_model.end( );
	for ( ; i != e; ++i ) {
		material_effects& effects = ( *i )->m_render_surface->get_material_effects( );
		if ( effects.m_effects[shadow_render_stage].c_ptr( ) )
			opaque_models.push_back( *i );
	}

	light* sun = &*m_context->scene( )->lights( ).get_sun( ).c_ptr( );
	if ( !sun )
		return;
	math::frustum frust( orig_view_projection );

	if ( s_test_shadow_culling1 )
		opaque_models.erase( std::remove_if( opaque_models.begin( ), opaque_models.end( ), remove_inappropriate_models( m_context, frust, sun->direction, m_context->m_sun_cascades[cascade_index].size ) ), opaque_models.end( ) );

	m_renderer->sort_models( opaque_models, shadow_render_stage, 0 );

	m_caster_model = opaque_models;
	VOSTOK_UNREFERENCED_PARAMETER( shadow_map_size );
}

void stage_shadow_direct::render_models(
	vector<render_surface_instance*>&	m_caster_model,
	float4x4 const&						orig_view_projection,
	u32 const							cascade_index,
	u32 const							shadow_map_size,
	float3 const&						real_view_pos,
	u32 const							pass_index,
	u32 const							refresh_rate
	)
{
	u32 num_render = m_caster_model.size( ) / (refresh_rate - pass_index);

	if ( !num_render )
		return;

	backend::ref( ).set_render_targets( 0, 0, 0, 0 );
	backend::ref( ).set_depth_stencil_target( &*m_rt_shadow_map );
	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );
	struct int4 {
		int4( s32 in_x, s32 in_y, s32 in_z, s32 in_w )
		{
			x = in_x;
			y = in_y;
			z = in_z;
			w = in_w;
		}

		s32 x;
		s32 y;
		s32 z;
		s32 w;
	};

	int4 shadow_viewport[4] = {
		int4( 0, 0, shadow_map_size, shadow_map_size ),
		int4( shadow_map_size, 0, shadow_map_size, shadow_map_size ),
		int4( 0, shadow_map_size, shadow_map_size, shadow_map_size ),
		int4( shadow_map_size, shadow_map_size, shadow_map_size, shadow_map_size )
	};

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = float(shadow_viewport[cascade_index].x);
	tmp_viewport.TopLeftY = float(shadow_viewport[cascade_index].y);
	tmp_viewport.Width = float(shadow_viewport[cascade_index].z);
	tmp_viewport.Height = float(shadow_viewport[cascade_index].w);
	tmp_viewport.MinDepth = 0.f;
	tmp_viewport.MaxDepth = 1.f;

	backend::ref( ).set_viewport( tmp_viewport );
	render_surface_instance** begin_d = m_caster_model.begin( );
	render_surface_instance** it_d = begin_d;
	render_surface_instance* const* end_d = m_caster_model.end( );
	u32 render_index = 0;
	for ( ; it_d != end_d && render_index < num_render; ++it_d, ++render_index )
	{
		render_surface_instance& instance = **it_d;
		render_surface* surface = instance.m_render_surface;
		material_effects& effects = surface->get_material_effects( );
		if ( effects.m_effects[forward_render_stage] ) continue;

		if ( !effects.m_effects[gbuffer_render_stage] && effects.m_effects[lighting_render_stage] ) continue;
		if ( !effects.is_cast_shadow ) continue;
		render_geometry& geometry = surface->m_render_geometry;
		if ( !geometry.geom.c_ptr( ) && !geometry.shadow_pass_geom.c_ptr( ) ) continue;
		if ( effects.stage_enable[shadow_render_stage] && effects.m_effects[shadow_render_stage].c_ptr( ) )
			effects.m_effects[shadow_render_stage]->apply( 0, 0 );

		else
			m_effect_shadow_direct->apply( 0, 0 );

		switch ( cascade_index ) {
			case 0:
				++statistics::ref( ).cascaded_sun_shadow_stat_group.num_dips_cascade_1.value;
				break;
			case 1:
				++statistics::ref( ).cascaded_sun_shadow_stat_group.num_dips_cascade_2.value;
				break;
			case 2:
				++statistics::ref( ).cascaded_sun_shadow_stat_group.num_dips_cascade_3.value;
				break;
			case 3:
				++statistics::ref( ).cascaded_sun_shadow_stat_group.num_dips_cascade_4.value;
				break;
		}

		++statistics::ref( ).cascaded_sun_shadow_stat_group.num_dips.value;

		statistics::ref( ).cascaded_sun_shadow_stat_group.num_triangles.value += geometry.primitive_count;
		instance.set_constants( );
		if ( geometry.shadow_pass_geom.c_ptr( ) )
			geometry.shadow_pass_geom->apply( );
		else
			geometry.geom->apply( );
		if ( effects.is_wind_swings )
		{
			post_process_parameters const& parameters = m_context->scene_view( )->post_process_parameters( );
			float3 wind_info_parameters(

				parameters.wind_direction.x,
				parameters.wind_direction.z,
				parameters.wind_strength
			);
			backend::ref( ).set_vs_constant( m_wind_info_parameters, wind_info_parameters );
		}
		backend::ref( ).set_vs_constant( m_shadow_cascade_index, cascade_index );

		m_context->set_w( *instance.m_transform );
		backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, geometry.primitive_count * 3, 0, 0 );
	}
	if ( it_d != end_d )
		m_caster_model.erase( begin_d, it_d );
	if ( !pass_index && s_draw_grass_shadows_value && m_context->scene( )->get_grass( ) )
		m_context->scene( )->get_grass( )->render( m_context, real_view_pos, shadow_render_stage, 0, 25.f, false, 0, true, cascade_index );

	backend::ref( ).set_viewport( orig_viewport );
	VOSTOK_UNREFERENCED_PARAMETER( orig_view_projection );
}

void stage_shadow_direct::render_dynamic_models(
	u32 const,
	u32 const,
	float3 const&
)
{
}
void stage_shadow_direct::execute_cascade( u32 cascade_id, u32 cascade_index, u32 shadow_map_size )
{
	static float cascade_offsets[4] = { -5.f, -10.f, -20.f, -40.f };
	static float cascade_align_mults[4] = { 1.f, 1.f, 1.f, 1.f };
	static float cascade_pos_mults[4] = { 1.f, 3.f, 6.f, 12.f };
	static u32 refresh_rate[4] = { 1, 1, 1, 1 };
	static bool do_it = true;
	options::ref( ).current.m_update_shadows_every_frame = do_it;
	u32 const pass_index = m_context->scene_view( )->get_render_frame_index( ) % refresh_rate[cascade_index];
	bool const first_pass = pass_index == 0;
	vector<render_surface_instance*> m_caster_model;

	light* sun = &*m_context->scene( )->lights( ).get_sun( ).c_ptr( );

	if ( !sun )
		return;

	float const size = m_context->m_sun_cascades[cascade_id].size;

	if ( size < math::epsilon_7 )
		return;
	float current_cascade_align_mult = 1.f / math::pow( 2048.f / float(shadow_map_size), 2.f ) * cascade_align_mults[cascade_index];
	float current_cascade_offset = cascade_offsets[cascade_index];
	if ( options::ref( ).current.m_update_shadows_every_frame )
	{
		if ( cascade_index == 0 )
			current_cascade_offset = -0.25f;
		else
			current_cascade_offset = 0.f;

		current_cascade_align_mult = 1.f;
	}
	float3 view_pos = m_context->get_view_pos( );
	float3 view_dir = m_context->get_view_dir( );

	if ( !m_invalid_shadow && !options::ref( ).current.m_update_shadows_every_frame )
	{
		if ( math::abs( m_previous_direction.dot_product( view_dir ) ) < 0.75f )
			m_previous_direction = view_dir;
		else
		{
			if ( (view_pos - m_previous_position[cascade_index]).length( ) > cascade_pos_mults[cascade_index] )
				m_previous_position[cascade_index] = view_pos;
			else
			{
				view_pos = m_previous_position[cascade_index];
				view_dir = m_previous_direction;
			}
		}
	}
	float3 offset_to_viewer = view_dir * current_cascade_offset;
	float const sun_distance = 300.f;
	sun->position = view_pos - sun->direction * sun_distance;

	float4x4 light_view_transform = math::create_camera_direction( sun->position, sun->direction, float3( 1.f, 0.f, 0.f ) );

	float4x4 light_projection_transform = math::create_orthographic_projection( size, size, 0.1f, sun_distance + 1.41421f * size );
	shadow_cascade_volume::cascade_volume_planes cull_planes;

	float3 light_shift_xz( 0.f, 0.f, 0.f );

	shadow_cascade_volume cascade_volume;
	if ( cascade_index == 0 )
	{
		for ( u32 i = 0; i < 4; ++i )
		{
			float3 eye_ray = m_context->get_eye_rays( )[i];
			eye_ray.normalize( );
			eye_ray = m_context->get_v_inverted( ).transform_direction( eye_ray );
			cascade_volume.view_frustum_rays.push_back( ray( view_pos + m_context->get_near( ) * eye_ray, eye_ray ) );
		}
	}
	else
		cascade_volume.view_frustum_rays = m_context->m_sun_cascades[cascade_id].rays;

	cascade_volume.view_ray.origin = view_pos;
	cascade_volume.view_ray.direction = view_dir;

	cascade_volume.light_ray.origin = sun->position;
	cascade_volume.light_ray.direction = sun->direction;
	float4x4 light_full_transform = light_view_transform * light_projection_transform;

	float4x4 light_full_transform_invert = math::invert4x3( light_full_transform );

	for ( u32 p = 0; p < 8; ++p )
	{
		float4 xf = light_full_transform_invert.transform( float4( corners[p], 1.f ) );
		cascade_volume.light_cuboid_points[p] = xf.xyz( );
	}
	for ( u32 plane = 0; plane < 4; ++plane )
		for ( u32 point = 0; point < 4; ++point )

		cascade_volume.light_cuboid_polys[plane].points[point] = facetable[plane][point];

	cascade_volume.compute_caster_model_fixed( cull_planes, light_shift_xz, size, true );
	float3 gran;

	float3 adjastment = compute_aligment( light_shift_xz, light_view_transform * light_projection_transform, float(shadow_map_size), current_cascade_align_mult, gran );

	float4x4 light_view_transform_offset = math::create_camera_direction( sun->position + light_shift_xz + offset_to_viewer, sun->direction, float3( 1.f, 0.f, 0.f ) );
	float3 gran0;

	float3 adjastment_of_offset = compute_aligment( light_shift_xz, light_view_transform_offset * light_projection_transform, float(shadow_map_size), 1.f, gran0 );
	light_view_transform = math::create_camera_direction( sun->position + light_shift_xz + adjastment + offset_to_viewer + adjastment_of_offset, sun->direction, float3( 1.f, 0.f, 0.f ) );
	bool const need_refresh = options::ref( ).current.m_update_shadows_every_frame || m_invalid_shadow || !math::is_similar( adjastment.x, m_previous_adjastment[cascade_index].x, 0.01f ) || !math::is_similar( adjastment.y, m_previous_adjastment[cascade_index].y, 0.01f );

	if ( first_pass )
	{
		m_previous_view_matrix[cascade_index] = light_view_transform;
		m_previous_projection_matrix[cascade_index] = light_projection_transform;
	}
	if ( need_refresh )
	{
		float4x4 orig_view_projection = m_context->get_vp( );

		m_context->push_set_v( light_view_transform );
		m_context->push_set_p( light_projection_transform );

		if ( first_pass )
			prepare_models( m_caster_model, orig_view_projection, cascade_index, shadow_map_size, view_pos );

		render_models( m_caster_model, orig_view_projection, cascade_index, shadow_map_size, view_pos, pass_index, refresh_rate[cascade_index] );

		float4x4 shadow_trans = m_context->get_vp( );

		m_context->pop_v( );
		m_context->pop_p( );
		float4x4 texture_space( float4( 0.5f, 0.f, 0.f, 0.f ), float4( 0.f, -0.5f, 0.f, 0.f ), float4( 0.f, 0.f, 1.f, 0.f ), float4( 0.5f, 0.5f, -m_context->m_sun_cascades[cascade_id].bias, 1.f ) );
		if ( first_pass )
		{
			m_prev_view_to_shadow[cascade_index] = m_view_to_shadow[cascade_index];
			m_view_to_shadow[cascade_index] = shadow_trans * texture_space;
			if ( cascade_id < m_context->m_sun_cascades.size( ) - 1 )
				m_context->m_sun_cascades[cascade_id + 1].rays = cascade_volume.view_frustum_rays;

			m_previous_real_view_matrix[cascade_index] = m_context->get_v( );
		}
	}

	if ( !options::ref( ).current.m_update_shadows_every_frame )
	{
		float3 real_vp = m_context->get_view_pos( );

		m_context->push_set_v( m_previous_view_matrix[cascade_index] );
		// 1 target line is likely retail-compiled-out source.
		m_context->push_set_p( m_previous_projection_matrix[cascade_index] );

		render_dynamic_models( cascade_index, shadow_map_size, real_vp );

		m_context->pop_v( );
		m_context->pop_p( );
	}
	m_context->set_view2shadow( m_context->get_v_inverted( ) * m_view_to_shadow[cascade_index], cascade_index );

	m_previous_adjastment[cascade_index] = adjastment;
}
float3 stage_shadow_direct::compute_aligment( float3 const& lightXZshift, float4x4 const& light_space_transform, float smap_res, float mult, float3& gran )
{
	float3 align_origin( 0.f, 0.f, 0.f);
	align_origin -= lightXZshift;

	float4x4 viewport( float4( smap_res/2.f, 0.0f, 0.0f, 0.0f), float4( 0.0f, -smap_res/2.f, 0.0f, 0.0f), float4( 0.0f, 0.0f, 1.0f, 0.0f), float4( smap_res/2.f, smap_res/2.f, 0.0f, 1.0f));
	float4x4 viewport_invert = math::invert4x3( viewport);

	float4x4 light_space_transform_invert = math::invert4x3( light_space_transform);

	float4 origin_pixel = light_space_transform.transform( float4( align_origin, 1.f) );
	origin_pixel *= (1.f/origin_pixel.w);
	origin_pixel = viewport.transform( origin_pixel );
	const float align_granularity = 4.f * mult;
	origin_pixel.x = origin_pixel.x / align_granularity - floorf( origin_pixel.x / align_granularity );
	origin_pixel.y = origin_pixel.y / align_granularity - floorf( origin_pixel.y / align_granularity );
	origin_pixel.x *= align_granularity;
	origin_pixel.y *= align_granularity;
	origin_pixel.z = 0;
	gran = origin_pixel.xyz();
	origin_pixel.xyz() = viewport_invert.transform_direction( origin_pixel.xyz() );
	origin_pixel.xyz() = light_space_transform_invert.transform_direction( origin_pixel.xyz() );
	float3 diff = origin_pixel.xyz();
	static float sign_test = 1.f;
	diff *= sign_test;
	return diff;
}

} // namespace render
} // namespace vostok
