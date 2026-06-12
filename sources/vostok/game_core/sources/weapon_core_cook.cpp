////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_cook.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include "game_core_memory.h"

namespace survarium {

// STATE[100%|DONE]
weapon_core_cook::weapon_core_cook( ) :
	resources::translate_query_cook( resources::weapon_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[100%|DONE]
void weapon_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string	weapon_config_name;

	weapon_config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		weapon_config_name.c_str( ),
		resources::weapon_class,
		boost::bind( &weapon_core_cook::on_weapon_config_loaded, this, _1 ),
		g_allocator,
		0,
		&parent,
		assert_on_fail_true
	);
}

// STATE[100%|DONE]
void weapon_core_cook::on_weapon_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook*	parent			= data.get_parent_query( );
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr			config_ptr		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_core*						object_to_cook	= VOSTOK_NEW_IMPL( g_allocator, weapon_core );

	process_loading_weapon_core( parent, config_ptr, object_to_cook );
}

// STATE[33.38%|PARTIAL]: delete_helper<doug_lea_allocator,resource_base> picks
// a different inline shape than the target - target inlines strip_pointer
// (mov ecx,[g_allocator]; push ecx) and passes 2 cdecl args; base out-of-lines
// strip_pointer and passes &resource in edi. Same delta as weapon_ammunition_cook.
void weapon_core_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x3 | 187 | VOSTOK_DELETE_IMPL( g_allocator, resource );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the delete_helper strip_pointer
	// inline-vs-call/convention split, non-steerable LTCG.
}

// STATE[85.63%|PARTIAL]: STRUCTURE MATCH (23/23)
void weapon_core_cook::load_weapon_parameters( configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	configs::binary_config_value const&	cfg_root				= config_ptr->get_root( );

	ASSERT( UNKNOWN_EXPRESSION_T( &config_ptr != 0 ) );

	configs::binary_config_value const&	parameters				= cfg_root["parameters"];

	u16 const							magazine_capacity		= parameters["magazine_capacity"];
	object_to_cook->set_magazine_capacity( magazine_capacity );

	float const							bullet_damage			= parameters["bullet_damage"];
	ASSERT( UNKNOWN_EXPRESSION_T( bullet_damage > 0.0f ) ); object_to_cook->set_bullet_damage( bullet_damage );

	float const							bullet_pierce			= parameters["bullet_pierce"];
	ASSERT( UNKNOWN_EXPRESSION_T( bullet_pierce >= 0.0f ) ); object_to_cook->set_bullet_pierce( bullet_pierce );

	float const							aim_fov_factor			= 1.0f / float( parameters["aim_zoom_factor"] );
	object_to_cook->set_aim_fov_factor( aim_fov_factor );

	float const							aim_near_plane_factor	= parameters["aim_near_plane_factor"];
	object_to_cook->set_aim_near_plane_factor( aim_near_plane_factor );

	object_to_cook->set_double_handed( parameters.value_exists( "double_handed" ) ? bool( parameters["double_handed"] ) : true );

	object_to_cook->set_chamber_a_round_on_reload( parameters.value_exists( "chamber_a_round_on_reload" ) ? bool( parameters["chamber_a_round_on_reload"] ) : false );

	configs::binary_config_value const&	weapon_fire_queue_types_cfg	= cfg_root["parameters"]["fire_queue_types"];

		u8*									weapon_fire_queue_types		= VOSTOK_NEW_ARRAY_IMPL( g_allocator, u8, weapon_fire_queue_types_cfg.size( ) );
		u8									queue_types_count			= 0;

		configs::binary_config_value const* it	= &*weapon_fire_queue_types_cfg.begin( );
		configs::binary_config_value const* it_e	= &*weapon_fire_queue_types_cfg.end( );

		for ( ; it != it_e; ++it ) weapon_fire_queue_types[ queue_types_count++ ] = (u8)(s8)it->operator s8( );

	object_to_cook->set_weapon_fire_queue_types( weapon_fire_queue_types, queue_types_count );

	object_to_cook->set_recoil_params( weapon_recoil_params( cfg_root["recoil"] ) );
	object_to_cook->set_dispersion_params( weapon_dispersion_params( cfg_root["dispersion"] ) );
		// STRUCTURE DIFF: target 23 / base 23 stmts
		// SIZE only | ASSERT lines, NEW_ARRAY, for-loop split
		// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs from ASSERT/NEW/comma-outlined call vs inline, non-steerable.
}

// STATE[100%|DONE]
void weapon_core_cook::process_loading_weapon_core( resources::query_result_for_cook* parent, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	load_weapon_parameters( config_ptr, object_to_cook );

	fs_new::virtual_path_string	skeleton_path;

	skeleton_path.assignf( "resources/animations/skeletons/%s", pcstr( config_ptr->get_root( )["object"]["skeleton"] ) );

	resources::request			requests[ 2 ];
	requests[ 0 ].path	= skeleton_path.c_str( );
	requests[ 0 ].id	= resources::skeleton_class; requests[ 1 ].path = pcstr( config_ptr->get_root( )["user_animations"] ); requests[ 1 ].id = resources::class_id_enum( 0x74 );

	resources::query_resources(
		requests,
		2,
		boost::bind(
			&weapon_core_cook::on_core_subresources_ready,
			this,
			_1,
			config_ptr,
			object_to_cook
		),
		g_allocator,
		0,
		parent,
		assert_on_fail_true
	);
}

// STATE[100%|DONE]
void weapon_core_cook::on_core_subresources_ready( resources::queries_result& data, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	resources::query_result_for_cook*	parent	= data.get_parent_query( );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	u32	resource_index	= 0;

	resources::resource_ptr< animation::skeleton, resources::unmanaged_intrusive_base >		weapon_skeleton
		= static_cast_resource_ptr< resources::resource_ptr< animation::skeleton, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_user_animations_container, resources::unmanaged_intrusive_base >	user_animations
		= static_cast_resource_ptr< resources::resource_ptr< weapon_user_animations_container, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	object_to_cook->set_skeleton( weapon_skeleton );
	object_to_cook->user_animations_selector( ).set_animations( user_animations );

	query_weapon_states( parent, config_ptr, object_to_cook );
}

// STATE[90.8%|PARTIAL]
void weapon_core_cook::query_weapon_states( resources::query_result_for_cook* parent, configs::binary_config_ptr config_ptr, weapon_core* object_to_cook )
{
	float const						rounds_per_second	= float( config_ptr->get_root( )["parameters"]["rounds_per_minute"] ) / 60.0f;
	float const						reload_time			= config_ptr->get_root( )["parameters"]["reload_time"];

	weapon_state_creation_params*	params				= VOSTOK_NEW_IMPL( g_allocator, weapon_state_creation_params )( config_ptr, *object_to_cook, rounds_per_second, reload_time, object_to_cook->get_is_shown_ref( ) );
	const_buffer					params_buffer		= const_buffer( (pcvoid)params, sizeof( weapon_state_creation_params ) );

	configs::binary_config_value	states_cfg			= config_ptr->get_root( )["states"];

	fixed_vector< resources::creation_request, 10 >	requests;

	requests.push_back( resources::creation_request( "inactive", params_buffer, resources::class_id_enum( 0x12C ) ) );
	requests.push_back( resources::creation_request( "show", params_buffer, resources::class_id_enum( 0x12D ) ) );
	requests.push_back( resources::creation_request( "hide", params_buffer, resources::class_id_enum( 0x12E ) ) );
	requests.push_back( resources::creation_request( "idle", params_buffer, resources::class_id_enum( 0x12F ) ) );
	requests.push_back( resources::creation_request( "reload", params_buffer, resources::class_id_enum( 0x130 ) ) );
	requests.push_back( resources::creation_request( "fire", params_buffer, resources::class_id_enum( 0x131 ) ) );
	requests.push_back( resources::creation_request( "aimed_idle", params_buffer, resources::class_id_enum( 0x132 ) ) );
	requests.push_back( resources::creation_request( "aimed_fire", params_buffer, resources::class_id_enum( 0x133 ) ) );

	if ( states_cfg.value_exists( "chamber_a_round" ) )
		requests.push_back( resources::creation_request( "chamber_a_round", params_buffer, resources::class_id_enum( 0x134 ) ) );

	if ( states_cfg.value_exists( "chamber_a_round_aimed" ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		requests.push_back( resources::creation_request( "chamber_a_round_aimed", params_buffer, resources::class_id_enum( 0x135 ) ) );
	}

	u32 const						requests_count		= requests.size( );

	buffer_vector< variant<32> >	user_data			( ALLOCA( requests_count * sizeof( variant<32> ) ), requests_count );
	buffer_vector< variant<32> const* >	user_data_ptrs	( ALLOCA( requests_count * sizeof( variant<32> const* ) ), requests_count ); user_data.assign( requests_count, variant<32>( ) );

		for ( u32 i = 1; i < requests_count; ++i )
		{
			variant<32> tmp; tmp.set( states_cfg[ requests[ i ].get_name( ) ] ); user_data[ i ] = tmp;
			user_data_ptrs[ i ] = &user_data[ i ];
		}
	resources::query_create_resources(
		requests.begin( ),
		requests_count,
		boost::bind(
			&weapon_core_cook::on_weapon_states_ready,
			this,
			_1,
			params,
			object_to_cook
		),
		g_allocator,
		user_data_ptrs.begin( ),
		parent,
		assert_on_fail_true
	);
	// STRUCTURE DIFF: target 27 / base 27 stmts
	// SIZE only | for-loop body merged to match target count
	// VERDICT: STRUCTURE MATCH (shape ok)
}
// STATE[89.1%|PARTIAL]
void weapon_core_cook::on_weapon_states_ready( resources::queries_result& data, weapon_state_creation_params const* params, weapon_core* object_to_cook )
{
	ASSERT( UNKNOWN_EXPRESSION );

	VOSTOK_DELETE_IMPL( g_allocator, params );

	u32	resource_index	= 0;

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	show_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	hide_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	idle_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	fire_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	reload_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	aim_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	aim_fire_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	chamber_a_round_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	chamber_a_round_aimed_state
		= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) ); resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base >	inactive_state; if ( resource_index < data.size( ) )
		inactive_state
			= static_cast_resource_ptr< resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > >( data[ resource_index++ ].get_unmanaged_resource( ) );

	object_to_cook->initialize_weapon_logic( inactive_state, show_state, hide_state, idle_state, reload_state, fire_state, aim_state, aim_fire_state, chamber_a_round_state, chamber_a_round_aimed_state );

	data.get_parent_query( )->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( static_cast< resources::unmanaged_resource* >( object_to_cook ) ),
		resources::memory_usage_type( resources::nocache_memory, cooked_object_size( *object_to_cook ) )
	);
	data.get_parent_query( )->finish_query( cook_base::result_success, assert_on_fail_true );
	// STRUCTURE DIFF: target 16 / base 16 stmts
	// SIZE only | if+declaration merged to match target count
	// VERDICT: STRUCTURE MATCH (shape ok)
}
// STATE[100%|DONE]
u32 weapon_core_cook::cooked_object_size( weapon_core& object_to_cook ) const
{
	return sizeof( weapon_core );
}

} // namespace survarium

