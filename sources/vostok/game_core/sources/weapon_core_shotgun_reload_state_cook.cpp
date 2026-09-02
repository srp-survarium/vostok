// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state_cook.h>

#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_animations_timescale_inline.h>
#include "game_core_memory.h"

namespace survarium {

weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook( )
{
}

// claude@NOTE: line 35 is the VOSTOK_UNREFERENCED_PARAMETERS dead-guard, NOT an ASSERT
//   (patterns/optimized-comdat-in-od-unit.md, dead-guard-fold variant): the leading
//   empty_stub is the identity(false) lvalue materialization, the 0x96-dword rep-movsd
//   is the by-reference in_query promoted-by-value through the variadic eater. Structure
//   now matches (2 stmts, 0 locals - malloc folded into the return at line 36). Residual:
//   the optimized COMDAT folds the dead-branch movzx/test/je guard that our /Od build
//   keeps (~7 bytes) + the by-value query copy - the known dead-guard ceiling.
mutable_buffer weapon_core_shotgun_reload_state_cook::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	VOSTOK_UNREFERENCED_PARAMETERS( in_query, raw_file_data, file_exist );
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( weapon_core_shotgun_reload_state ), "weapon_core_shotgun_reload_state" ), sizeof( weapon_core_shotgun_reload_state ) );
}

void weapon_core_shotgun_reload_state_cook::deallocate_resource( void* buffer )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );	// claude@MATCH: target keeps free_helper out-of-line (free_helper_impl); base inlines it.
}

void weapon_core_shotgun_reload_state_cook::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );
	configs::binary_config_value		cfg;
	if ( !parent.user_data( )->try_get( cfg ) )
	{
		DEBUG_BREAK		( );
		parent.finish_query( result_error );
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION_T( cfg.value_exists( "start_substate" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["start_substate"].value_exists( "animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["start_substate"]["animations"].size( ) == 4 ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["start_substate"].value_exists( "user_animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["start_substate"]["user_animations"].size( ) == 4 ) );

	ASSERT( UNKNOWN_EXPRESSION_T( cfg.value_exists( "reload_one_substate" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["reload_one_substate"].value_exists( "animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["reload_one_substate"]["animations"].size( ) == 4 ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["reload_one_substate"].value_exists( "user_animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["reload_one_substate"]["user_animations"].size( ) == 4 ) );

	ASSERT( UNKNOWN_EXPRESSION_T( cfg.value_exists( "finish_substate" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["finish_substate"].value_exists( "animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["finish_substate"]["animations"].size( ) == 4 ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["finish_substate"].value_exists( "user_animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg["finish_substate"]["user_animations"].size( ) == 4 ) );

	enum {
		total_weapon_anim_count	= 12,
		total_user_anim_count	= 12,
		requests_count			= total_weapon_anim_count + total_user_anim_count,
	};

	typedef fixed_vector< resources::request, requests_count >	requests_fixed_type;

	requests_fixed_type	requests;

	configs::binary_config_value start_weapon_anim_cfg = cfg["start_substate"]["animations"];
	for ( u32 i = 0; i != 4; ++i )
	{
		requests.push_back( resources::create_request( start_weapon_anim_cfg[ i ], resources::animation_class ) );
	}
	configs::binary_config_value start_user_anim_cfg = cfg["start_substate"]["user_animations"];
	for ( u32 i = 0; i != 4; ++i )
	{
		requests.push_back( resources::create_request( start_user_anim_cfg[ i ], resources::animation_class ) );
	}

	configs::binary_config_value reload_one_weapon_anim_cfg = cfg["reload_one_substate"]["animations"];
	for ( u32 i = 0; i != 4; ++i )
	{
		requests.push_back( resources::create_request( reload_one_weapon_anim_cfg[ i ], resources::animation_class ) );
	}
	configs::binary_config_value reload_one_user_anim_cfg = cfg["reload_one_substate"]["user_animations"];
	for ( u32 i = 0; i != 4; ++i )
	{
		requests.push_back( resources::create_request( reload_one_user_anim_cfg[ i ], resources::animation_class ) );
	}

	configs::binary_config_value finish_weapon_anim_cfg = cfg["finish_substate"]["animations"];
	for ( u32 i = 0; i != 4; ++i )
	{
		requests.push_back( resources::create_request( finish_weapon_anim_cfg[ i ], resources::animation_class ) );
	}
	configs::binary_config_value finish_user_anim_cfg = cfg["finish_substate"]["user_animations"];
	for ( u32 i = 0; i != 4; ++i )
	{
		requests.push_back( resources::create_request( finish_user_anim_cfg[ i ], resources::animation_class ) );
	}

	ASSERT( UNKNOWN_EXPRESSION_T( requests.size( ) == requests_count ) );

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &weapon_core_shotgun_reload_state_cook::on_subresources_ready, this, _1, in_out_unmanaged_resource_buffer, params ),
		g_allocator,
		NULL,
		&parent
	);
	parent.finish_query( result_postponed );
}

void weapon_core_shotgun_reload_state_cook::on_subresources_ready( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params )
{
	typedef fixed_vector< resources::managed_resource_ptr, 8 >	start_animations_fixed_type;
	typedef fixed_vector< resources::managed_resource_ptr, 8 >	finish_animations_fixed_type;
	typedef fixed_vector< resources::managed_resource_ptr, 8 >	reload_one_animations_fixed_type;

	ASSERT( UNKNOWN_EXPRESSION_T( data.is_successful( ) ) );

	u32 resource_index					= 0;

	start_animations_fixed_type			start_animations;
	for ( u32 i = 0; i != 8; ++i )
	{
		start_animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ resource_index++ ].get_managed_resource( ) ) );
	}

	reload_one_animations_fixed_type	reload_one_animations;
	for ( u32 i = 0; i != 8; ++i )
	{
		reload_one_animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ resource_index++ ].get_managed_resource( ) ) );
	}

	finish_animations_fixed_type		finish_animations;
	for ( u32 i = 0; i != 8; ++i )
	{
		finish_animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ resource_index++ ].get_managed_resource( ) ) );
	}

	float animations_timescale			= computed_shotgun_reload_animation_time_scale( reload_one_animations[ 0 ], params->weapon.get_magazine_capacity( ), params->reload_time );

	weapon_core_shotgun_reload_start_substate* reload_start =
		VOSTOK_NEW_IMPL( g_allocator, weapon_core_shotgun_reload_start_substate )( params->weapon, 1.f, start_animations.begin( ), start_animations.size( ) );

	weapon_core_shotgun_reload_one_round_substate* reload_one_round =
		VOSTOK_NEW_IMPL( g_allocator, weapon_core_shotgun_reload_one_round_substate )( params->weapon, animations_timescale, reload_one_animations.begin( ), reload_one_animations.size( ) );

	weapon_core_shotgun_reload_finish_substate* reload_finish =
		VOSTOK_NEW_IMPL( g_allocator, weapon_core_shotgun_reload_finish_substate )( params->weapon, 1.f, finish_animations.begin( ), finish_animations.size( ) );

	weapon_core_shotgun_reload_state* object_to_cook = new ( buffer.c_ptr( ) ) weapon_core_shotgun_reload_state( params->weapon, reload_start, reload_one_round, reload_finish );

	// claude@MATCH: target pushes 0x20 here - sizeof of the cook itself, not of weapon_core_shotgun_reload_state (0x140).
	data.get_parent_query( )->set_unmanaged_resource( object_to_cook, resources::memory_usage_type( resources::nocache_memory, sizeof( *this ) ) );
	data.get_parent_query( )->finish_query( result_success );
}

void weapon_core_shotgun_reload_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	weapon_core_shotgun_reload_state*	wpn_state	= static_cast< weapon_core_shotgun_reload_state* >( resource );
	wpn_state->~weapon_core_shotgun_reload_state( );
}

} // namespace survarium
