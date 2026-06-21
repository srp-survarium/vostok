////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_cook.h"
#include "profile_skin_visual_cook.h"
#include "player_creation_params.h"
#include "player_parameters_cooker_data.h"
#include "player.h"
#include "memory.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>
#include "base_game_scene.h"
#include "game.h"
#include <vostok/game_core/inventory_cooker_data.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/items_dictionary.h>

namespace survarium {

// claude@NOTE: the player_cook / profile_skin_visual_cook ctors are recorded by the
// PDB as the form INLINED into the single `static <cook> s_*;` construction site in
// game_world/game::register_cooks() - the ctor body writes to the absolute address
// of the static instance instead of `this`. Matching the standalone (this-relative)
// ctor caps at that addressing-mode delta; the init-list args (player_class /
// player_skin_visual_class, reuse_false, use_current_thread_id) are byte-correct.
 player_cook::player_cook( ) :
	translate_query_cook( resources::player_class, reuse_false, use_current_thread_id )
{
}

void player_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string							path;
	path.assignf										( "resources/%s", parent.get_requested_path() );

	resources::query_resource							(
		path.c_str(),
		resources::binary_config_class_impl,
		boost::bind( &player_cook::on_config_loaded, this, _1 ),
		g_allocator,
		0,
		&parent
	);
}

void player_cook::delete_resource( resources::resource_base* resource )
{
	player* player_resource								= static_cast_checked< player* >( resource );
	DELETE												( player_resource );
}

// claude@NOTE: on_config_loaded is CROSS-MODULE-BLOCKED on the (target-real but
// unbuilt) base_player_creation_params::base_player_creation_params() ctor - NEW(
// player_creation_params ) emits a call to it, but no game_core TU defines it
// (anchor_game_core.cpp explicitly side-steps it as "no compiled default ctor"),
// so the full body fails to LINK. Parked as a buildable stub until that base ctor
// is matched in game_core; the faithful reconstruction (config root, the
// vectora<request> skin/model/inventory/player_parameters push_backs, the recoil/
// breath/dispersion/stamina/stealth loads, the inventory + parameters cooker_data,
// and the 3-entry user_data query_resources -> on_subresources_loaded) is recorded
// in the commit message.
// STATE[STUB]
void player_cook::on_config_loaded( resources::queries_result& data )
{
	// FUNCTION BODY[0x5dde10]: 77
	data.get_parent_query()->finish_query				( result_success );
}

// claude@NOTE: on_subresources_loaded is reconstructed to its tractable core
// (free the cooker_data, assign character_model from data[0], requery the hit
// params config -> on_hit_params_loaded). The target carries ~9 more statements
// (server_character_model, empty_hands, foot/material-id and game_material lookups,
// further model assignments) that read player_creation_params members still defined
// only as game_core stubs; the residual is steerable once those are matched.
void player_cook::on_subresources_loaded(
	resources::queries_result&			data,
	player_creation_params*				params,
	inventory_cooker_data*				inventory_cook_data,
	player_parameters_cooker_data*		player_parameters_cook_data
)
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();

	FREE												( inventory_cook_data );
	FREE												( player_parameters_cook_data );

	params->character_model								= static_cast_resource_ptr< render::skeleton_model_ptr >( data[0].get_unmanaged_resource() );

	fs_new::virtual_path_string							hit_params_config_path;
	hit_params_config_path.assignf						( "resources/%s", data[2].get_requested_path() );

	resources::query_resource							(
		hit_params_config_path.c_str(),
		resources::binary_config_class_impl,
		boost::bind( &player_cook::on_hit_params_loaded, this, _1, params ),
		g_allocator,
		0,
		parent
	);
}

// claude@NOTE: byte-residual concentrates on params->damage_model = ... : our base
// emits the heavy resource_ptr copy-temp + intrusive_ptr<...>::dec path and reads
// data[0]'s unmanaged resource 8 bytes high (0x134 vs the target's 0x12C), while the
// sibling object_sound::on_sound_resources_ready (identical static_cast_resource_ptr
// idiom, but on unmanaged_resource_ptr) matches 99.98%. The delta is the game_core
// damage_model_ptr type (damage_model is a game_core stub on this branch) + the same
// query_result struct-offset gap seen in object_wire::resources_ready - cross-module,
// recovers when game_core's damage_model and the resources query_result layout land.
// The NEW( player ) / finish_query / set_unmanaged_resource tail is structurally faithful.
void player_cook::on_hit_params_loaded( resources::queries_result& data, player_creation_params* params )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();

	params->damage_model								= static_cast_resource_ptr< damage_model_ptr >( data[0].get_unmanaged_resource() );

	player* player_resource								= NEW( player )( *params );
	if ( !player_resource )
	{
		parent->finish_query							( result_out_of_memory );
		return;
	}

	DELETE												( params );

	parent->set_unmanaged_resource						( player_resource, resources::nocache_memory, sizeof( player ) );
	parent->finish_query								( result_success );
}

 profile_skin_visual_cook::profile_skin_visual_cook( game& g ) :
	translate_query_cook( resources::player_skin_visual_class, reuse_false, use_current_thread_id ),
	m_game( g )
{
}

// claude@NOTE: profile_skin_visual_cook::translate_query / on_configs_loaded /
// on_visual_loaded are CROSS-MODULE-CAPPED on render's skeleton_combined_cook_data
// (declared-only here; render module not built in this tree) and game_core
// player_profile slot iteration + items_dictionary torso/pants table lookups
// whose members are still sibling stubs. Bodies kept as buildable skeletons.
// STATE[STUB]
void profile_skin_visual_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x5dd580]: 29
	parent.finish_query									( result_success );
}

// STATE[STUB]
void profile_skin_visual_cook::on_configs_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	player_profile const*					profile
)
{
	// FUNCTION BODY[0x5dce70]: 94
	VOSTOK_UNREFERENCED_PARAMETERS						( &data, profile );
	parent->finish_query								( result_success );
}

// STATE[STUB]
void profile_skin_visual_cook::on_visual_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	render::skeleton_combined_cook_data*	cook_data
)
{
	// FUNCTION BODY[0x5dcc90]: 5
	VOSTOK_UNREFERENCED_PARAMETERS						( &data, cook_data );
	parent->finish_query								( result_success );
}

void profile_skin_visual_cook::delete_resource( resources::resource_base* __formal )
{
	VOSTOK_UNREFERENCED_PARAMETER						( __formal );
	NOT_IMPLEMENTED										( );
}

} // namespace survarium
