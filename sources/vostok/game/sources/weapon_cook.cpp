// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "weapon_cook.h"
#include "weapon.h"
#include "game.h"
#include "shotgun_weapon_reload_state_cook.h"
#include "weapon_sound_events_handler_state_cook.h"

#include <vostok/game_core/double_barreled_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/double_barreled_weapon_core_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_idle_state.h>
#include <vostok/game_core/weapon_core_aimed_state.h>
#include <vostok/game_core/weapon_core_idle_state.h>
#include <vostok/game_core/weapon_core_inactive_state_cook.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

weapon_cook::weapon_cook( game& g ) :
	m_game( g )
{
}

void weapon_cook::register_cooks_for_logic_states( )
{
	static weapon_core_inactive_state_cook s_weapon_core_inactive_state_cook;
	static weapon_core_state_cook_template< weapon_core_idle_state > s_weapon_core_idle_state_cook;
	static weapon_core_state_cook_template< weapon_core_aimed_state > s_weapon_core_aimed_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > > s_show_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_hide_state > > s_hide_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_reload_state > > s_reload_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > > s_chamber_a_round_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_aimed_state > > s_chamber_a_round_aimed_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_fire_state > > s_fire_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_aimed_fire_state > > s_aimed_fire_cook;
	static shotgun_weapon_reload_state_cook s_shotgun_weapon_reload_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate > > s_shotgun_reload_start_substate_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_one_round_substate > > s_shotgun_reload_one_round_substate_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_shotgun_reload_finish_substate > > s_shotgun_reload_finish_substate_cook;
	static weapon_core_state_cook_template< double_barreled_weapon_core_idle_state > s_double_barreled_weapon_core_idle_state_cook;
	static weapon_core_state_cook_template< double_barreled_weapon_core_aimed_idle_state > s_double_barreled_weapon_core_aimed_idle_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_show_state > > s_double_barreled_show_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state > > s_double_barreled_hide_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_reload_state > > s_double_barreled_reload_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_fire_state > > s_double_barreled_fire_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state > > s_double_barreled_aimed_fire_cook;
	static weapon_core_state_cook_template< pistol_weapon_core_idle_state > s_pistol_weapon_core_idle_state_cook;
	static weapon_core_state_cook_template< pistol_weapon_core_aimed_idle_state > s_pistol_weapon_core_aimed_idle_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_show_state > > s_pistol_show_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > > s_pistol_hide_state_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_reload_state > > s_pistol_reload_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_fire_state > > s_pistol_fire_cook;
	static weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_aimed_fire_state > > s_pistol_aimed_fire_cook;
}

void weapon_cook::on_weapon_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );

	configs::binary_config_ptr config_ptr = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const& config = config_ptr->get_root( );

	u8 fire_pfx_count = config["particles"].value_exists( "bullet_shells_count" ) ? (u8)config["particles"]["bullet_shells_count"] : 10;
	u8 shells_pfx_count = config["particles"].value_exists( "shoot_pfx_count" ) ? (u8)config["particles"]["shoot_pfx_count"] : 3;

	u32 requests_count = fire_pfx_count + shells_pfx_count + 4;
	pcstr rifle_scope = NULL;
	if ( config.value_exists( "addons" ) && config["addons"].value_exists( "rifle_scope_dict_id" ) )
		rifle_scope = m_game.items_dictionary( ).item_by_id( (u32)config["addons"]["rifle_scope_dict_id"] ).item_cfg_name.c_str( );

	buffer_vector< resources::request > requests(
		ALLOCA( sizeof( resources::request ) * ( requests_count + ( rifle_scope ? 1 : 0 ) ) ),
		requests_count + ( rifle_scope ? 1 : 0 )
	);
	if ( rifle_scope )
		requests.push_back( resources::create_request( "gameplay/items/scopes/leupold", resources::rifle_scope_class ) );

	requests.push_back( resources::create_request( config["object"]["model"], resources::skeleton_model_instance_class ) );
	configs::binary_config_value const& first_view_animations = config["user_animations_in_place"]["death_hud"];
	for ( u32 i = 0; i < first_view_animations.size( ); ++i )
		requests.push_back( resources::create_request( first_view_animations[i], resources::animation_class ) );

	configs::binary_config_value const& third_view_animations = config["user_animations_in_place"]["death"];
	for ( u32 i = 0; i < third_view_animations.size( ); ++i )
		requests.push_back( resources::create_request( third_view_animations[i], resources::animation_class ) );

	configs::binary_config_value const& preview_animations = config["user_animations_in_place"]["preview"];
	for ( u32 i = 0; i < preview_animations.size( ); ++i )
		requests.push_back( resources::create_request( preview_animations[i], resources::animation_class ) );

	u32 const object_size = sizeof( weapon ) +
		( first_view_animations.size( ) + third_view_animations.size( ) + preview_animations.size( ) ) * sizeof( resources::managed_resource_ptr );
	weapon* object_to_cook = static_cast< weapon* >( VOSTOK_MALLOC_IMPL( g_allocator, object_size, "weapon" ) );
	new ( object_to_cook ) weapon( first_view_animations.size( ), third_view_animations.size( ), preview_animations.size( ) );

	object_to_cook->m_shells_pfx_list = VOSTOK_NEW_ARRAY_IMPL( g_allocator, resources::unmanaged_resource_ptr, fire_pfx_count );
	object_to_cook->m_shells_pfx_count = fire_pfx_count;
	object_to_cook->m_fire_pfx_list = VOSTOK_NEW_ARRAY_IMPL( g_allocator, resources::unmanaged_resource_ptr, shells_pfx_count );
	object_to_cook->m_fire_pfx_count = shells_pfx_count;

	for ( u32 i = 0; i < fire_pfx_count; ++i )
		requests.push_back( resources::create_request( config["particles"]["bullet_shells"], resources::particle_system_instance_class ) );
	for ( u32 i = 0; i < shells_pfx_count; ++i )
		requests.push_back( resources::create_request( config["particles"]["shoot"], resources::particle_system_instance_class ) );

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &weapon_cook::on_weapon_subresources_ready, this, _1, config_ptr, object_to_cook ),
		g_allocator,
		NULL,
		parent
	);
}

void weapon_cook::on_weapon_subresources_ready(
	resources::queries_result&		data,
	configs::binary_config_ptr		config_ptr,
	weapon_core* const				object_to_cook
)
{
	u32 resource_index = 0;
	resources::query_result_for_cook* const parent = data.get_parent_query( );
	rifle_scope_ptr scope;
	if ( config_ptr->get_root( ).value_exists( "addons" ) &&
		 config_ptr->get_root( )["addons"].value_exists( "rifle_scope_dict_id" ) )
	{
		scope = static_cast_resource_ptr< rifle_scope_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	}

	render::skeleton_model_ptr base_model = static_cast_resource_ptr< render::skeleton_model_ptr >( data[resource_index++].get_unmanaged_resource( ) );
	weapon* const weapon_object = static_cast< weapon* >( object_to_cook );
	resources::managed_resource_ptr* animations = ( resources::managed_resource_ptr* )( weapon_object + 1 );

	configs::binary_config_value const& first_view_animations = config_ptr->get_root( )["user_animations_in_place"]["death_hud"];
	for ( u32 i = 0; i < first_view_animations.size( ); ++i )
		new ( animations++ ) resources::managed_resource_ptr( static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) ) );

	configs::binary_config_value const& third_view_animations = config_ptr->get_root( )["user_animations_in_place"]["death"];
	for ( u32 i = 0; i < third_view_animations.size( ); ++i )
		new ( animations++ ) resources::managed_resource_ptr( static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) ) );

	configs::binary_config_value const& preview_animations = config_ptr->get_root( )["user_animations_in_place"]["preview"];
	for ( u32 i = 0; i < preview_animations.size( ); ++i )
		new ( animations++ ) resources::managed_resource_ptr( static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) ) );

	u32 i = 0;
	for ( ; i < weapon_object->m_shells_pfx_count; ++i ) weapon_object->m_shells_pfx_list[i] = static_cast_resource_ptr< resources::unmanaged_resource_ptr >( data[resource_index++].get_unmanaged_resource( ) );

	i = 0;
	for ( ; i < weapon_object->m_fire_pfx_count; ++i ) weapon_object->m_fire_pfx_list[i] = static_cast_resource_ptr< resources::unmanaged_resource_ptr >( data[resource_index++].get_unmanaged_resource( ) );

	weapon_object->load_weapon( base_model, scope );
	process_loading_weapon_core( parent, config_ptr, weapon_object );
}

// claude@NOTE: structure matches (cast, count, ~weapon, the two pfx-array deletes, the
// appended-animations dtor loop). Capped ~77%: the final free is the strip_pointer/
// delete_helper inline-vs-call wall. Target emits TWO statements - __RTCastToVoid
// (dynamic_cast<pvoid> via get_top_pointer, line 249) then mspace_free under an if(cast)
// guard (line 250) - whereas VOSTOK_FREE_IMPL's free_helper passes (remove_cv*)pointer
// (a static cast, no dynamic_cast) and folds to ONE statement. The target free_helper
// variant routes the pointer through get_top_pointer; not reproducible from this header.
void weapon_cook::delete_resource( resources::resource_base* const resource )
{
	weapon* to_delete = static_cast< weapon* >( resource );

	u32 const animations_count	= to_delete->m_first_view_death_animations_count + to_delete->m_third_view_death_animations_count + to_delete->m_preview_animations_count;
	to_delete->~weapon( );

	VOSTOK_DELETE_ARRAY_IMPL( g_allocator, to_delete->m_shells_pfx_list );
	VOSTOK_DELETE_ARRAY_IMPL( g_allocator, to_delete->m_fire_pfx_list );

	resources::managed_resource_ptr* const animations = ( resources::managed_resource_ptr* )( to_delete + 1 );
	for ( u32 i = 0; i != animations_count; ++i )
		animations[ i ].~resource_ptr( );

	VOSTOK_FREE_IMPL( g_allocator, to_delete );
}

u32 weapon_cook::cooked_object_size( weapon_core& object_to_cook ) const
{
	return sizeof( weapon ) + ( static_cast< weapon& >( object_to_cook ).first_view_death_animations_count( ) + static_cast< weapon& >( object_to_cook ).third_view_death_animations_count( ) + static_cast< weapon& >( object_to_cook ).preview_animations_count( ) ) * sizeof( float4x4* );
}

} // namespace survarium
