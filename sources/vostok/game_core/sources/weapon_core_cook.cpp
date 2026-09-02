// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_cook.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include "game_core_memory.h"

namespace survarium {

weapon_core_cook::weapon_core_cook( ) :
	resources::translate_query_cook( resources::weapon_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

void weapon_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string weapon_config_name;
	weapon_config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		weapon_config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &weapon_core_cook::on_weapon_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

void weapon_core_cook::on_weapon_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook*	parent			= data.get_parent_query( );
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr			config_ptr		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_core*						object_to_cook	= VOSTOK_NEW_IMPL( g_allocator, weapon_core );

	process_loading_weapon_core( parent, config_ptr, object_to_cook );
}

void weapon_core_cook::load_weapon_parameters( configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	configs::binary_config_value const& cfg_root	= config_ptr->get_root( );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_root.value_exists( "parameters" ) ) );

	configs::binary_config_value const& parameters	= cfg_root["parameters"];

	u16 const magazine_capacity		= parameters["magazine_capacity"];
	object_to_cook->set_magazine_capacity( magazine_capacity );

	float const bullet_damage		= parameters["bullet_damage"];
	object_to_cook->set_bullet_damage( bullet_damage );

	float const bullet_pierce		= parameters["bullet_pierce"];
	object_to_cook->set_bullet_pierce( bullet_pierce );

	float const aim_fov_factor		= 1.f / (float)parameters["aim_zoom_factor"];
	object_to_cook->set_aim_fov_factor( aim_fov_factor );

	float const aim_near_plane_factor	= parameters["aim_near_plane_factor"];
	object_to_cook->set_aim_near_plane_factor( aim_near_plane_factor );

	object_to_cook->m_is_double_handed = parameters.value_exists( "double_handed" ) ? (bool)parameters["double_handed"] : true;

	object_to_cook->m_chamber_a_round_on_reload = parameters.value_exists( "chamber_a_round_on_reload" ) ? (bool)parameters["chamber_a_round_on_reload"] : false;

	configs::binary_config_value const& weapon_fire_queue_types_cfg = cfg_root["parameters"]["fire_queue_types"];	// claude@MATCH: target recomputes cfg_root["parameters"] here instead of reusing `parameters`

	pbyte weapon_fire_queue_types	= VOSTOK_NEW_ARRAY_IMPL( g_allocator, u8, weapon_fire_queue_types_cfg.size( ) );
	u8 queue_types_count			= 0;
	for ( configs::binary_config_value const* it = weapon_fire_queue_types_cfg.begin( ), *it_e = weapon_fire_queue_types_cfg.end( ); it != it_e; ++it, ++queue_types_count )
		weapon_fire_queue_types[ queue_types_count ]	= (s8)*it;

	object_to_cook->set_weapon_fire_queue_types( weapon_fire_queue_types, queue_types_count );

	object_to_cook->set_recoil_params( weapon_recoil_params( cfg_root["recoil"] ) );
	object_to_cook->set_dispersion_params( weapon_dispersion_params( cfg_root["dispersion"] ) );
}

void weapon_core_cook::process_loading_weapon_core( resources::query_result_for_cook* const parent, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	load_weapon_parameters( config_ptr, object_to_cook );

	fs_new::virtual_path_string skeleton_path;
	skeleton_path.assignf( "resources/animations/skeletons/%s", (pcstr)config_ptr->get_root( )["object"]["skeleton"] );

	resources::request requests[] = {
		{ skeleton_path.c_str( ),						resources::skeleton_class },
		{ config_ptr->get_root( )["user_animations"],	resources::animation_container_class }
	};

	resources::query_resources(
		requests,
		array_size( requests ),
		boost::bind( &weapon_core_cook::on_core_subresources_ready, this, _1, config_ptr, object_to_cook ),
		g_allocator,
		NULL,
		parent
	);
}

void weapon_core_cook::on_core_subresources_ready( resources::queries_result& data, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	resources::query_result_for_cook* parent = data.get_parent_query( );
	ASSERT( UNKNOWN_EXPRESSION_T( parent ) );
	ASSERT( UNKNOWN_EXPRESSION_T( data.is_successful( ) ) );

	u32 resource_index = 0;
	animation::skeleton_ptr weapon_skeleton					= static_cast_resource_ptr< animation::skeleton_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_user_animations_container_ptr user_animations	= static_cast_resource_ptr< weapon_user_animations_container_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	object_to_cook->set_skeleton( weapon_skeleton );
	object_to_cook->user_animations_selector( ).set_animations( user_animations );

	query_weapon_states( data.get_parent_query( ), config_ptr, object_to_cook );
}

// claude@MATCH: unlike the sibling cooks (items_cook, weapon_ammunition_cook, ...) this
// target site has NO strip_pointer call - the original bypasses VOSTOK_DELETE_IMPL and
// calls delete_helper with the dereferenced allocator directly.
void weapon_core_cook::delete_resource( resources::resource_base* resource )
{
	memory::delete_helper( *g_allocator, resource );
}

void weapon_core_cook::query_weapon_states( resources::query_result_for_cook* const parent, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	float const rounds_per_second	= (float)config_ptr->get_root( )["parameters"]["rounds_per_minute"] / 60.f;
	float const reload_time			= config_ptr->get_root( )["parameters"]["reload_time"];
	weapon_state_creation_params const* params = VOSTOK_NEW_IMPL( g_allocator, weapon_state_creation_params )( config_ptr, *object_to_cook, rounds_per_second, reload_time, object_to_cook->m_is_shown );
	const_buffer params_buffer( (pcvoid)params, sizeof( weapon_state_creation_params ) );


	configs::binary_config_value states_cfg = config_ptr->get_root( )["states"];
	fixed_vector< resources::creation_request, 10 > requests;
	requests.push_back( resources::creation_request( "inactive",	params_buffer, resources::weapon_inactive_state_class ) );
	requests.push_back( resources::creation_request( "show",		params_buffer, ( resources::class_id_enum )( u32 )states_cfg["show"]["state_type"] ) );
	requests.push_back( resources::creation_request( "hide",		params_buffer, ( resources::class_id_enum )( u32 )states_cfg["hide"]["state_type"] ) );
	requests.push_back( resources::creation_request( "idle",		params_buffer, ( resources::class_id_enum )( u32 )states_cfg["idle"]["state_type"] ) );
	requests.push_back( resources::creation_request( "reload",		params_buffer, ( resources::class_id_enum )( u32 )states_cfg["reload"]["state_type"] ) );
	requests.push_back( resources::creation_request( "fire",		params_buffer, ( resources::class_id_enum )( u32 )states_cfg["fire"]["state_type"] ) );
	requests.push_back( resources::creation_request( "aimed_idle",	params_buffer, ( resources::class_id_enum )( u32 )states_cfg["aimed_idle"]["state_type"] ) );
	requests.push_back( resources::creation_request( "aimed_fire",	params_buffer, ( resources::class_id_enum )( u32 )states_cfg["aimed_fire"]["state_type"] ) );

	if ( states_cfg.value_exists( "chamber_a_round" ) )
		requests.push_back( resources::creation_request( "chamber_a_round", params_buffer, ( resources::class_id_enum )( u32 )states_cfg["chamber_a_round"]["state_type"] ) );

	if ( states_cfg.value_exists( "chamber_a_round_aimed" ) )
	{
		ASSERT( UNKNOWN_EXPRESSION_T( states_cfg.value_exists( "chamber_a_round" ) ) );
		requests.push_back( resources::creation_request( "chamber_a_round_aimed", params_buffer, ( resources::class_id_enum )( u32 )states_cfg["chamber_a_round_aimed"]["state_type"] ) );
	}

	u32 const requests_count = requests.size( );
	buffer_vector< variant<32> >		user_data		( ALLOCA( requests_count * sizeof( variant<32> ) ), requests_count, requests_count, variant<32>( ) );
	buffer_vector< variant<32> const* >	user_data_ptrs	( ALLOCA( requests_count * sizeof( variant<32> const* ) ), requests_count, requests_count, NULL );

	for ( u32 i = 1; i != requests_count; ++i ) {
		user_data[ i ].set( states_cfg[ requests[ i ].get_name( ) ] );
		user_data_ptrs[ i ] = &user_data[ i ];
	}

	resources::query_create_resources(
		requests.begin( ),
		requests_count,
		boost::bind( &weapon_core_cook::on_weapon_states_ready, this, _1, params, object_to_cook ),
		g_allocator,
		user_data_ptrs.begin( ),
		parent
	);
}

void weapon_core_cook::on_weapon_states_ready( resources::queries_result& data, weapon_state_creation_params const* params, weapon_core* object_to_cook )
{
	ASSERT( UNKNOWN_EXPRESSION_T( data.is_successful( ) ) );
	VOSTOK_DELETE_IMPL( g_allocator, params );

	u32 resource_index = 0;
	weapon_core_base_state_ptr inactive_state	= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr show_state		= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr hide_state		= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr idle_state		= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr reload_state		= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr fire_state		= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr aim_state		= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon_core_base_state_ptr aim_fire_state	= static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) );

	weapon_core_base_state_ptr chamber_a_round_state		= resource_index < data.size( ) ? static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) ) : weapon_core_base_state_ptr( NULL );
	weapon_core_base_state_ptr chamber_a_round_aimed_state	= resource_index < data.size( ) ? static_cast_resource_ptr< weapon_core_base_state_ptr >( data[resource_index++].get_unmanaged_resource( ) ) : weapon_core_base_state_ptr( NULL );

	object_to_cook->initialize_weapon_logic(
		inactive_state,
		show_state,
		hide_state,
		idle_state,
		reload_state,
		fire_state,
		aim_state,
		aim_fire_state,
		chamber_a_round_state,
		chamber_a_round_aimed_state
	);

	data.get_parent_query( )->set_unmanaged_resource( object_to_cook, resources::memory_usage_type( resources::nocache_memory, cooked_object_size( *object_to_cook ) ) );
	data.get_parent_query( )->finish_query( result_success );
}

u32 weapon_core_cook::cooked_object_size( weapon_core& object_to_cook ) const
{
	return sizeof( weapon_core );
}

} // namespace survarium
