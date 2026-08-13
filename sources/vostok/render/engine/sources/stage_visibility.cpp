#include "pch.h"
#include "stage_visibility.h"

#include <algorithm>
#include <vostok/collision/common_types.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/console_command.h>
#include <vostok/math_aabb.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_frustum.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/particle/world.h>
#include <vostok/render/core/options.h>

#include "ambient_volume.h"
#include "decal_instance.h"
#include "environment_probe.h"
#include "grass_patch.h"
#include "grass_world.h"
#include "hw_hiz_occlusion_manager.h"
#include "light.h"
#include "lights_db.h"
#include "render_model_instance_impl.h"
#include "render_particle_emitter_instance.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"
#include "statistics.h"

static bool s_no_trees_value = false;
static vostok::console_commands::cc_bool s_no_trees( "no_trees", s_no_trees_value, false, vostok::console_commands::command_type_engine_internal );
static bool s_no_bushes_value = false;
static vostok::console_commands::cc_bool s_no_bushes( "no_bushes", s_no_bushes_value, false, vostok::console_commands::command_type_engine_internal );
static bool s_no_terrain_value = false;
static vostok::console_commands::cc_bool s_no_terrain( "no_terrain", s_no_terrain_value, false, vostok::console_commands::command_type_engine_internal );
static bool s_no_house_value = false;
static vostok::console_commands::cc_bool s_no_house( "no_house", s_no_house_value, false, vostok::console_commands::command_type_engine_internal );
static bool s_no_background_value = false;
static vostok::console_commands::cc_bool s_no_background( "no_background", s_no_background_value, false, vostok::console_commands::command_type_engine_internal );
static bool s_no_flora_value = false;
static vostok::console_commands::cc_bool s_no_flora( "no_flora", s_no_flora_value, false, vostok::console_commands::command_type_engine_internal );
static bool s_no_other_value = false;
static vostok::console_commands::cc_bool s_no_other( "no_other", s_no_other_value, false, vostok::console_commands::command_type_engine_internal );

static u32 s_visible_surfaces_limit_value = 0;
static vostok::console_commands::cc_u32 s_visible_surfaces_limut( "visible_surfaces_limut", s_visible_surfaces_limit_value, 0, 100000, false, vostok::console_commands::command_type_engine_internal );

namespace vostok {
namespace render {

static float4 aabb_to_occlusion_bound(
	math::aabb const& in_aabb,
	float4x4 const& in_instance_transform
)
{
	math::aabb bound_box = in_aabb;
	bound_box.modify( in_instance_transform );
	float3 const extents = bound_box.extents( );
	float3 const center = bound_box.center( );
	return float4( center, extents.length( ) );
}

stage_visibility::stage_visibility(
	renderer* in_renderer,
	renderer_context* context
) :
	stage( in_renderer, context ),
	m_portals_offset_to_results( 0 )
{
	m_data_ready = true;
	// 4 target lines are likely retail-compiled-out source.
	m_occlusion_manager = NEW( hw_hiz_occlusion_manager )(
		true, options::ref( ).current.m_hiz_occlusion_culling_width, options::ref( ).current.m_hiz_occlusion_culling_height );
	m_static_bounds_array = NEW_ARRAY( float4, occlusion_buffer_size );
	m_static_results_array = NEW_ARRAY( u8, occlusion_buffer_size );
	m_current_occlusion_buffer_size = 0;
}

stage_visibility::~stage_visibility( )
{
	DELETE_ARRAY( m_static_bounds_array );
	DELETE_ARRAY( m_static_results_array );
	DELETE( m_occlusion_manager );
}

void stage_visibility::debug_render( )
{
	static bool s_draw = false;

	if ( s_draw )
		m_occlusion_manager->render_debug( m_context, m_static_bounds_array, m_static_results_array, m_current_occlusion_buffer_size );
}

void stage_visibility::execute( )
{
	if ( !options::ref( ).current.m_use_hiz_occlusion_culling )
		frustum_culling( );
	else {
		m_data_ready = query_data( );
		frustum_culling( );
		if ( m_data_ready ) {
			occlusion_culling( );
			m_data_ready = false;
		}
	}

	if (
		s_no_trees_value ||
		s_no_bushes_value ||
		s_no_terrain_value ||
		s_no_house_value ||
		s_no_background_value ||
		s_no_flora_value ||
		s_no_other_value
	) {
		vector< render_surface_instance* >& visible_models =
			m_context->get_scene_view( )->get_visible_models( );
		vector< render_surface_instance* >::iterator i = visible_models.begin( ),
			e = visible_models.end( );
		typedef vector< render_model_instance_impl* > models_array_type;
		models_array_type models;
		models.reserve( 1024 );
		for ( ; i != e; ++i ) {
			render_surface_instance* instance = *i;
			render_model_instance_impl* model = instance->m_parent;
			fixed_string< 128 > model_name( "<unknown>" );

			bool const is_other =
				s_no_other_value &&
				model_name.find( "flora" ) == model_name.npos &&
				model_name.find( "terrain" ) == model_name.npos &&
				model_name.find( "house" ) == model_name.npos &&
				model_name.find( "background" ) == model_name.npos &&
				model_name.find( "cane" ) == model_name.npos &&
				model_name.find( "poplar" ) == model_name.npos &&
				model_name.find( "fruit" ) == model_name.npos &&
				model_name.find( "tree" ) == model_name.npos &&
				model_name.find( "elka" ) == model_name.npos;

			if ( s_no_other_value && is_other ) {
				instance->m_occluded = true;
				continue;
			}

			if ( s_no_flora_value && model_name.find( "flora" ) != model_name.npos )
				instance->m_occluded = true;
			if ( s_no_terrain_value && model_name.find( "terrain" ) != model_name.npos )
				instance->m_occluded = true;
			if ( s_no_house_value && model_name.find( "house" ) != model_name.npos )
				instance->m_occluded = true;
			if ( s_no_background_value && model_name.find( "background" ) != model_name.npos )
				instance->m_occluded = true;
			if ( s_no_bushes_value && model_name.find( "cane" ) != model_name.npos )
				instance->m_occluded = true;

			if (
				( s_no_bushes_value && model->get_surfaces_count( ) == 1 ) ||
				( s_no_trees_value && model->get_surfaces_count( ) > 1 )
			) {
				if (
					model_name.find( "poplar" ) != model_name.npos ||
					model_name.find( "fruit" ) != model_name.npos ||
					model_name.find( "tree" ) != model_name.npos ||
					model_name.find( "elka" ) != model_name.npos
				)
					instance->m_occluded = true;
			}
		}
	}

	gather_statistics( );
}

bool stage_visibility::query_data( )
{
	return m_occlusion_manager->quary_and_get_results_if_ready(
		m_static_results_array,
		m_current_occlusion_buffer_size
	);
}

void stage_visibility::frustum_culling( )
{
	math::frustum view_frustum( m_context->get_culling_vp( ) );
	scene* render_scene = m_context->scene( );
	scene_view* view = m_context->get_scene_view( );

	view->get_visible_grass_patches( ).clear( );
	view->get_visible_decals( ).clear( );
	view->get_visible_environment_probes( ).clear( );
	view->get_visible_particle_instances( ).clear( );
	view->get_visible_lights( ).clear( );
	view->get_visible_models( ).clear( );
	view->get_visible_ambient_volumes( ).clear( );
	view->get_visible_moved_opaque_models( ).clear( );

	if ( render_scene->get_grass( ) ) {
		render_scene->get_grass( )->process_culling( m_context, 100.f );
		view->get_visible_grass_patches( ) =
			render_scene->get_grass( )->get_visible_patches( );
	}

	if ( render_scene->particle_world( ) )
		render_scene->particle_world( )->get_render_emitter_instances(
			m_context->get_culling_vp( ),
			view->get_visible_particle_instances( )
		);

	{
		collision::objects_type light_objects( g_allocator );
		light_objects.reserve( 1024 );
		render_scene->lights( ).lights_tree( ).cuboid_query(
			u32( -1 ),
			view_frustum,
			light_objects
		);
		for (
			collision::objects_type::const_iterator i = light_objects.begin( ),
				e = light_objects.end( );
			i != e;
			++i
		)
			view->get_visible_lights( ).push_back(
				static_cast< light* >( ( *i )->user_data( ) )
			);
	}

	{
		collision::objects_type decals_objects( g_allocator );
		decals_objects.reserve( render_scene->decals( ).size( ) );
		render_scene->decals_tree( ).cuboid_query(
			u32( -1 ),
			view_frustum,
			decals_objects
		);
		for (
			collision::objects_type::const_iterator i = decals_objects.begin( ),
				e = decals_objects.end( );
			i != e;
			++i
		)
			view->get_visible_decals( ).push_back(
				static_cast< decal_instance* >( ( *i )->user_data( ) )
			);
	}

	{
		collision::objects_type probe_objects( g_allocator );
		probe_objects.reserve( render_scene->environment_probes( ).size( ) );
		render_scene->environment_probes_tree( ).cuboid_query(
			u32( -1 ),
			view_frustum,
			probe_objects
		);
		for (
			collision::objects_type::const_iterator i = probe_objects.begin( ),
				e = probe_objects.end( );
			i != e;
			++i
		)
			view->get_visible_environment_probes( ).push_back(
				static_cast< environment_probe* >( ( *i )->user_data( ) )
			);
	}

	vector< ambient_volume* >& ambient_volumes = render_scene->ambient_volumes( );
	ambient_volume** end = ambient_volumes.end( );
	for ( ambient_volume** i = ambient_volumes.begin( ); i != end; ++i )
		if ( view_frustum.test_inexact( ( *i )->get_aabb( ) ) != math::intersection_outside )
			view->get_visible_ambient_volumes( ).push_back( *i );

	render_scene->select_models(
		m_context->get_culling_vp( ),
		view->get_visible_models( ),
		m_context->get_culling_view_pos( ),
		visible_flag,
		false
	);
	render_scene->select_models(
		m_context->get_culling_vp( ),
		view->get_visible_moved_opaque_models( ),
		m_context->get_culling_view_pos( ),
		visible_flag,
		true
	);

	if ( s_visible_surfaces_limit_value ) {
		math::random32 random;
		std::random_shuffle(
			view->get_visible_models( ).begin( ),
			view->get_visible_models( ).end( ),
			random
		);
		if ( view->get_visible_models( ).size( ) > s_visible_surfaces_limit_value )
			view->get_visible_models( ).resize( s_visible_surfaces_limit_value );
	}
}

void stage_visibility::occlusion_culling( )
{
	float4* bounds_it = m_static_bounds_array;
	u32 occlusion_info_index_iterator = 0;

	get_results_and_prepare_bounds_models( bounds_it, occlusion_info_index_iterator );
	get_results_and_prepare_bounds_lights( bounds_it, occlusion_info_index_iterator );
	get_results_and_prepare_bounds_grass( bounds_it, occlusion_info_index_iterator );
	get_results_and_prepare_bounds_decals( bounds_it, occlusion_info_index_iterator );
	get_results_and_prepare_bounds_env_probes( bounds_it, occlusion_info_index_iterator );
	get_results_and_prepare_bounds_ambient_volumes( bounds_it, occlusion_info_index_iterator );
	get_results_and_prepare_bounds_particles( bounds_it, occlusion_info_index_iterator );

	get_results_and_prepare_bounds_portals( bounds_it, occlusion_info_index_iterator );

	m_current_occlusion_buffer_size = occlusion_info_index_iterator;
	m_occlusion_manager->process_culling(
		m_context,
		m_static_bounds_array,
		m_current_occlusion_buffer_size
	);
}

bool stage_visibility::occluded( u32 const index ) const
{
	if ( index == u32( -1 ) )
		return false;
	return !m_static_results_array[index];
}

void stage_visibility::get_results_and_prepare_bounds_models(
	float4*& out_bounds,
	u32& out_counter
)
{
	vector< render_surface_instance* >& models =
		m_context->get_scene_view( )->get_visible_models( );
	for (
		vector< render_surface_instance* >::iterator i = models.begin( ), e = models.end( );
		i != e;
		++i
	) {
		render_surface_instance* instance = *i;
		instance->m_occluded = occluded( instance->m_occlusion_info_index );
		instance->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound(
			instance->m_render_surface->m_aabbox,
			*instance->m_transform
		);
	}
}

void stage_visibility::get_results_and_prepare_bounds_lights(
	float4*& out_bounds,
	u32& out_counter
)
{
	vector< light_ptr >& lights = m_context->get_scene_view( )->get_visible_lights( );
	for ( vector< light_ptr >::iterator i = lights.begin( ), end = lights.end( ); i != end; ++i ) {
		light* instance = i->c_ptr( );
		instance->m_occluded = occluded( instance->m_occlusion_info_index );
		instance->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound( instance->m_aabb, float4x4( ).identity( ) );
	}
}

void stage_visibility::get_results_and_prepare_bounds_grass(
	float4*& out_bounds,
	u32& out_counter
)
{
	vector< grass_patch* >& grass = m_context->get_scene_view( )->get_visible_grass_patches( );
	for ( vector< grass_patch* >::iterator i = grass.begin( ), end = grass.end( ); i != end; ++i ) {
		grass_patch* instance = *i;
		instance->m_occluded = occluded( instance->m_occlusion_info_index );
		instance->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound( instance->get_aabb( ), float4x4( ).identity( ) );
	}
}

void stage_visibility::get_results_and_prepare_bounds_decals(
	float4*& out_bounds,
	u32& out_counter
)
{
	vector< decal_instance* >::iterator i = m_context->get_scene_view( )->get_visible_decals( ).begin( ),
		e = m_context->get_scene_view( )->get_visible_decals( ).end( );
	for ( ; i != e; ++i ) {
		( *i )->m_occluded = occluded( ( *i )->m_occlusion_info_index );
		( *i )->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound( ( *i )->get_aabb( ), float4x4( ).identity( ) );
	}
}

void stage_visibility::get_results_and_prepare_bounds_env_probes(
	float4*& out_bounds,
	u32& out_counter
)
{
	vector< environment_probe* >::iterator i =
		m_context->get_scene_view( )->get_visible_environment_probes( ).begin( ),
		e = m_context->get_scene_view( )->get_visible_environment_probes( ).end( );
	for ( ; i != e; ++i ) {
		( *i )->m_occluded = occluded( ( *i )->m_occlusion_info_index );
		( *i )->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound( ( *i )->get_aabb( ), float4x4( ).identity( ) );
	}
}

void stage_visibility::get_results_and_prepare_bounds_ambient_volumes(
	float4*& out_bounds,
	u32& out_counter
)
{
	vector< ambient_volume* >::iterator i =
		m_context->get_scene_view( )->get_visible_ambient_volumes( ).begin( ),
		e = m_context->get_scene_view( )->get_visible_ambient_volumes( ).end( );
	for ( ; i != e; ++i ) {
		( *i )->m_occluded = occluded( ( *i )->m_occlusion_info_index );
		( *i )->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound( ( *i )->get_aabb( ), float4x4( ).identity( ) );
	}
}

void stage_visibility::get_results_and_prepare_bounds_particles(
	float4*& out_bounds,
	u32& out_counter
)
{
	vectora< particle::render_particle_emitter_instance* >::iterator i =
		m_context->get_scene_view( )->get_visible_particle_instances( ).begin( ),
		e = m_context->get_scene_view( )->get_visible_particle_instances( ).end( );
	for ( ; i != e; ++i ) {
		render_particle_emitter_instance* instance =
			static_cast< render_particle_emitter_instance* >( *i );
		instance->m_occluded = occluded( instance->m_occlusion_info_index );
		instance->m_occlusion_info_index = out_counter++;
		*out_bounds++ = aabb_to_occlusion_bound( instance->get_aabb( ), float4x4( ).identity( ) );
	}
}

void stage_visibility::get_results_and_prepare_bounds_portals(
	float4*& out_bounds,
	u32& out_counter
)
{
	scene* render_scene = m_context->scene( );
	u32 const portals_count = render_scene->get_portals_count( );
	if ( !portals_count )
		return;

	if ( m_portals_offset_to_results ) {
		render_scene->update_portals_occlusion_culling(
			m_static_results_array + m_portals_offset_to_results
		);
		statistics::ref( ).visibility_stat_group.num_occlusion_culled_portals.value =
			std::count(
				m_static_results_array + m_portals_offset_to_results,
				m_static_results_array + m_portals_offset_to_results + portals_count,
				u8( 0 )
			);
	}

	m_portals_offset_to_results = out_counter;
	render_scene->get_portals_occlusion_bounds( out_bounds );
	out_bounds += portals_count;
	out_counter += portals_count;
}

static bool is_not_occluded_predicate_light( light_ptr const& value )
{
	return !value->is_occluded( );
}

static bool is_occluded_predicate_light( light_ptr const& value )
{
	return value->is_occluded( );
}

template < typename T >
static bool is_not_occluded_predicate( T const* const value )
{
	return !value->is_occluded( );
}

template < typename T >
static bool is_occluded_predicate( T const* const value )
{
	return value->is_occluded( );
}

void stage_visibility::gather_statistics( ) const
{
	scene_view* view = m_context->get_scene_view( );

	statistics::ref( ).visibility_stat_group.num_surfaces.value = std::count_if(
		view->get_visible_models( ).begin( ),
		view->get_visible_models( ).end( ),
		&is_not_occluded_predicate< render_surface_instance >
	);
	statistics::ref( ).visibility_stat_group.num_lights.value = std::count_if(
		view->get_visible_lights( ).begin( ),
		view->get_visible_lights( ).end( ),
		&is_not_occluded_predicate_light
	);
	statistics::ref( ).visibility_stat_group.num_particle_instances.value = std::count_if(
		view->get_visible_particle_instances( ).begin( ),
		view->get_visible_particle_instances( ).end( ),
		&is_not_occluded_predicate< particle::render_particle_emitter_instance >
	);
	statistics::ref( ).grass_stat_group.num_visible_patches.value = std::count_if(
		view->get_visible_grass_patches( ).begin( ),
		view->get_visible_grass_patches( ).end( ),
		&is_not_occluded_predicate< grass_patch >
	);
	statistics::ref( ).deferred_decals_stat_group.num_decals.value = std::count_if(
		view->get_visible_decals( ).begin( ),
		view->get_visible_decals( ).end( ),
		&is_not_occluded_predicate< decal_instance >
	);
	statistics::ref( ).visibility_stat_group.num_environment_probes.value = std::count_if(
		view->get_visible_environment_probes( ).begin( ),
		view->get_visible_environment_probes( ).end( ),
		&is_not_occluded_predicate< environment_probe >
	);
	statistics::ref( ).visibility_stat_group.num_ambient_volumes.value = std::count_if(
		view->get_visible_ambient_volumes( ).begin( ),
		view->get_visible_ambient_volumes( ).end( ),
		&is_not_occluded_predicate< ambient_volume >
	);

	statistics::ref( ).visibility_stat_group.num_occlusion_culled_surfaces.value = std::count_if(
		view->get_visible_models( ).begin( ),
		view->get_visible_models( ).end( ),
		&is_occluded_predicate< render_surface_instance >
	);
	statistics::ref( ).visibility_stat_group.num_occlusion_culled_lights.value = std::count_if(
		view->get_visible_lights( ).begin( ),
		view->get_visible_lights( ).end( ),
		&is_occluded_predicate_light
	);
	statistics::ref( ).visibility_stat_group.num_occlusion_culled_particle_instances.value = std::count_if(
		view->get_visible_particle_instances( ).begin( ),
		view->get_visible_particle_instances( ).end( ),
		&is_occluded_predicate< particle::render_particle_emitter_instance >
	);
	statistics::ref( ).visibility_stat_group.num_occlusion_culled_grass_patches.value = std::count_if(
		view->get_visible_grass_patches( ).begin( ),
		view->get_visible_grass_patches( ).end( ),
		&is_occluded_predicate< grass_patch >
	);
	statistics::ref( ).visibility_stat_group.num_occlusion_culled_decals.value = std::count_if(
		view->get_visible_decals( ).begin( ),
		view->get_visible_decals( ).end( ),
		&is_occluded_predicate< decal_instance >
	);
	statistics::ref( ).visibility_stat_group.num_occlusion_culled_env_probes.value = std::count_if(
		view->get_visible_environment_probes( ).begin( ),
		view->get_visible_environment_probes( ).end( ),
		&is_occluded_predicate< environment_probe >
	);
	statistics::ref( ).visibility_stat_group.num_occlusion_culled_ambient_volumes.value = std::count_if(
		view->get_visible_ambient_volumes( ).begin( ),
		view->get_visible_ambient_volumes( ).end( ),
		&is_occluded_predicate< ambient_volume >
	);
}

} // namespace render
} // namespace vostok
