// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
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
#include <vostok/game_core/affects_applying_type_enum.h>
#include <vostok/game_core/artefact_spores_core.h>
#include <vostok/physics/animated_rigid_body.h>
#include <vostok/render/facade/skeleton_combined_cook_data.h>
#include <vostok/render/facade/model.h>
#include "slot_def.h"

namespace survarium {

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
	VOSTOK_DELETE_IMPL									( ::survarium::g_allocator, player_resource );
}

void player_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query( );

	configs::binary_config_ptr					config		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const&			root		= config->get_root( )["player"];

	vectora< resources::request >				requests	( g_allocator );

	requests.push_back							( resources::create_request( "combined_skin_123", resources::player_skin_visual_class ) );


	requests.push_back							( resources::create_request( "character/human/scavengers_01/scavengers_01", resources::skeleton_model_instance_class ) );

	fs_new::virtual_path_string					damage_config_path;
	damage_config_path.assignf					( "resources/models/%s.skinned_model/hit_targets", (pcstr)root["skeleton_model_instance"] );
	requests.push_back							( resources::create_request( damage_config_path.c_str( ), resources::binary_config_class_impl ) );

	fs_new::virtual_path_string					model_settings_config_path;
	model_settings_config_path.assignf			( "resources/models/%s.skinned_model/settings", (pcstr)root["skeleton_model_instance"] );
	requests.push_back							( resources::create_request( model_settings_config_path.c_str( ), resources::binary_config_class_impl ) );


	requests.push_back							( resources::create_request( "inventory", resources::inventory_class ) );
	requests.push_back							( resources::create_request( "player_parameters", resources::player_parameters_class ) );

	player_creation_params* params				= VOSTOK_NEW_IMPL( ::survarium::g_allocator, player_creation_params );

	parent->user_data( )->try_get				( params->initial_info );

	params->game_scene							= static_cast< base_game_scene* >( params->initial_info.game_scene );
	params->foot_3rd_view_game_material_id		= root["foot_material_id"];
	params->foot_1st_view_game_material_id		= root["foot_1st_view_material_id"];
	params->recoil_params.load					( root["character_recoil_params"] );
	params->breath_holding_params.load			( root["character_breath_holding_params"] );
	params->dispersion_params.load				( root["character_dispersion_params"] );
	params->initial_stamina.load					( root["stamina_params"] );
	params->initial_stealth.load					( root["stealth_params"] );
	params->items_dictionary						= &params->game_scene->get_game( ).items_dictionary( );

	if ( params->initial_info.is_demo_player )
		requests.push_back						( resources::create_request( root["empty_hands"], resources::empty_hands_class ) );

	u32 const requests_count					= requests.size( );
	buffer_vector< variant<32> const* > user_data	( ALLOCA( requests_count * sizeof( variant<32> const* ) ), requests_count, requests_count, NULL );

	inventory_cooker_data* inventory_cook_data		= VOSTOK_NEW_IMPL( ::survarium::g_allocator, inventory_cooker_data );
	inventory_cook_data->profile					= params->initial_info.profile;
	inventory_cook_data->dictionary				= params->items_dictionary;
	inventory_cook_data->damage_model				= NULL;

	variant<32> ud_skin_visual;
	ud_skin_visual.set							( ( player_profile const* )params->initial_info.profile );
	user_data[0]								= &ud_skin_visual;


	variant<32> id;
	id.set									( inventory_cook_data );
	user_data[4]								= &id;

	player_parameters_cooker_data* player_parameters_cook_data = VOSTOK_NEW_IMPL( ::survarium::g_allocator, player_parameters_cooker_data );
	player_parameters_cook_data->profile			= params->initial_info.profile;
	player_parameters_cook_data->dictionary			= params->items_dictionary;

	variant<32> pd;
	pd.set									( player_parameters_cook_data );
	user_data[5]								= &pd;

	resources::query_resources						(
		requests.begin( ), requests_count,
		boost::bind( &player_cook::on_subresources_loaded, this, _1, params, inventory_cook_data, player_parameters_cook_data ),
		g_allocator,
		user_data.begin( ),
		parent,
		assert_on_fail_true
	);
}

void player_cook::on_subresources_loaded(
	resources::queries_result&			data,
	player_creation_params*				params,
	inventory_cooker_data*				inventory_cook_data,
	player_parameters_cooker_data*		player_parameters_cook_data
)
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	VOSTOK_FREE_IMPL									( ::survarium::g_allocator, inventory_cook_data );
	VOSTOK_FREE_IMPL									( ::survarium::g_allocator, player_parameters_cook_data );

	params->character_model								= static_cast_resource_ptr< render::skeleton_model_ptr >( data[0].get_unmanaged_resource() );
	params->server_character_model							= static_cast_resource_ptr< render::skeleton_model_ptr >( data[1].get_unmanaged_resource() );
	configs::binary_config_ptr damage_cfg					= static_cast_resource_ptr< configs::binary_config_ptr >( data[2].get_unmanaged_resource() );
	configs::binary_config_ptr settings_cfg					= static_cast_resource_ptr< configs::binary_config_ptr >( data[3].get_unmanaged_resource() );
	params->inventory										= static_cast_resource_ptr< inventory_ptr >( data[4].get_unmanaged_resource() );
	params->player_parameters								= static_cast_resource_ptr< player_parameters_modifyer_ptr >( data[5].get_unmanaged_resource() );


	params->empty_hands										= params->initial_info.is_demo_player ?
		static_cast_resource_ptr< interactive_object_ptr >( data[6].get_unmanaged_resource() ) : NULL;
	params->damage_collision								= physics::new_animated_bt_hit_model(
		damage_cfg->get_root( ), params->character_model->m_skeleton, g_allocator );

	fs_new::virtual_path_string							hit_params_config_path;
	hit_params_config_path.assignf						( "resources/gameplay/hit_params/%s", (pcstr)settings_cfg->get_root( )["hit_params"] );


	affects_applying_type_enum const affects_applying_type = params->initial_info.profile->is_local ? type_apply_directly : type_read_only;
	resources::user_data_variant new_ud;
	new_ud.set										( affects_applying_type );
	resources::query_resource							(
		hit_params_config_path.c_str( ),
		resources::damage_model_class,
		boost::bind( &player_cook::on_hit_params_loaded, this, _1, params ),
		g_allocator,
		&new_ud,
		parent,
		assert_on_fail_true
	);
}

void player_cook::on_hit_params_loaded( resources::queries_result& data, player_creation_params* params )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();

	params->damage_model								= static_cast_resource_ptr< damage_model_ptr >( data[0].get_unmanaged_resource() );

	player* player_resource								= VOSTOK_NEW_IMPL( ::survarium::g_allocator, player )( *params );
	if ( !player_resource )
	{
		parent->set_out_of_memory						( resources::unmanaged_memory, sizeof( player ) );
		parent->finish_query							( result_out_of_memory );
		return;
	}

	VOSTOK_DELETE_IMPL									( ::survarium::g_allocator, params );

	parent->set_unmanaged_resource						( player_resource, resources::nocache_memory, sizeof( player ) );
	parent->finish_query								( result_success );
}

 profile_skin_visual_cook::profile_skin_visual_cook( game& g ) :
	translate_query_cook( resources::player_skin_visual_class, reuse_false, use_current_thread_id ),
	m_game( g )
{
	resources::register_cook							( this );
}

static slot_def body_parts[] = {
	{ gloves_slot,	"gloves" },
	{ torso_slot,	"torso" },
	{ boots_slot,	"boots" },
	{ pants_slot,	"pants" },
	{ helmet_slot,	"helmet" },
	{ mask_slot,		"mask" },
	{ back_slot,		"back" }
};

void profile_skin_visual_cook::translate_query( resources::query_result_for_cook& parent )
{
	player_profile const* profile = NULL;
	parent.user_data( )->try_get( profile );

	items_dictionary& items_dictionary = m_game.items_dictionary( );
	vectora< resources::request > requests( g_allocator );
	requests.push_back( resources::create_request( "resources/gameplay/bodyparts/default", resources::binary_config_class_impl ) );

	for ( u32 i = 0; i < array_size( body_parts ); ++i )
	{
		slot_def current = body_parts[i];
		profile_slot const& current_slot = profile->slots[current.slot];
		if ( current_slot.item.id )
		{
			pstr config_path = NULL;
			STR_JOINA(
				config_path,
				"resources/",
				items_dictionary.item_by_id( current_slot.item.dict_id ).item_cfg_name.c_str( )
			);
			requests.push_back( resources::create_request( config_path, resources::binary_config_class_impl ) );
		}
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &profile_skin_visual_cook::on_configs_loaded, this, _1, &parent, profile ),
		g_allocator,
		NULL,
		&parent,
		assert_on_fail_true
	);
}

void profile_skin_visual_cook::on_configs_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	player_profile const*					profile
)
{
	render::skeleton_combined_cook_data* cook_data = VOSTOK_NEW_IMPL( ::survarium::g_allocator, render::skeleton_combined_cook_data )( false );
	variant<32> ud;
	ud.set( cook_data );

	configs::binary_config_ptr default_config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value default_root = default_config->get_root( )["body_parts"];
	cook_data->models_count = 0;

	pcstr torso_table_id = "data";
	pcstr pants_table_id = "data";
	fixed_string<260> path;
	configs::binary_config_value current;

	render::skeleton_combined_cook_data::model_def& head = cook_data->model_defs[cook_data->models_count];
	++cook_data->models_count;
	current = default_root["head"];
	head.base_model_name += (pcstr)current["base_model"];
	head.part_name += (pcstr)current["part_name"];
	cook_data->skeleton_name += (pcstr)default_root["skeleton"];
	cook_data->bind_pose_name += (pcstr)default_root["bind_pose"];

	u32 config_index = 1;
	for ( u32 i = 0; i < array_size( body_parts ); ++i )
	{
		slot_def current_slot_def = body_parts[i];
		render::skeleton_combined_cook_data::model_def& model_def = cook_data->model_defs[cook_data->models_count];
		bool has_part = false;

		if ( profile->slots[current_slot_def.slot].item.id )
		{
			configs::binary_config_ptr cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[config_index++].get_unmanaged_resource( ) );
			current = cfg->get_root( )[
				current_slot_def.slot == torso_slot ? torso_table_id :
				current_slot_def.slot == pants_slot ? pants_table_id : "data"
			];
			has_part = true;
		}
		else if ( default_root.value_exists( current_slot_def.table_name ) )
		{
			current = default_root[current_slot_def.table_name];
			has_part = true;
		}

		path.appendf( "%d_", profile->slots[current_slot_def.slot].item.dict_id );
		if ( has_part )
		{
			if ( current.value_exists( "base_model_hud" ) && current.value_exists( "part_name_hud" ) )
			{
				model_def.base_model_name += (pcstr)current["base_model_hud"];
				model_def.part_name += (pcstr)current["part_name_hud"];
			}
			else
			{
				model_def.base_model_name = (pcstr)current["base_model"];
				model_def.part_name = (pcstr)current["part_name"];
			}

			if ( current.value_exists( "material" ) )
				model_def.material_name = (pcstr)current["material"];

			if ( current_slot_def.slot == gloves_slot )
				torso_table_id = current["model_type"];
			else if ( current_slot_def.slot == boots_slot )
				pants_table_id = current["model_type"];

			++cook_data->models_count;
		}
	}

	resources::query_resource(
		path.c_str( ),
		resources::skeleton_combined_model_class,
		boost::bind( &profile_skin_visual_cook::on_visual_loaded, this, _1, parent, cook_data ),
		g_allocator,
		&ud,
		parent,
		assert_on_fail_true
	);
}

void profile_skin_visual_cook::on_visual_loaded(
	resources::queries_result&				data,
	resources::query_result_for_cook*		parent,
	render::skeleton_combined_cook_data*	cook_data
)
{
	parent->set_unmanaged_resource(
		data[0].get_unmanaged_resource( ),
		resources::nocache_memory,
		sizeof( render::skeleton_model_instance )
	);
	parent->finish_query( result_success );
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, cook_data );
}

#line 363
void profile_skin_visual_cook::delete_resource( resources::resource_base* __formal )
{
	VOSTOK_UNREFERENCED_PARAMETER						( __formal );
	NOT_IMPLEMENTED										( );
}

} // namespace survarium
