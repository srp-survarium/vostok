// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "sound_player_cook.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/ai/npc.h>
#include <vostok/sound/sound_emitter.h>
#include "ai_sound_player.h"
#include "human_npc.h"

namespace survarium {

 sound_player_cook::sound_player_cook( sound::world* world, resources::class_id_enum class_id ) :
	resources::translate_query_cook( class_id, reuse_false, use_current_thread_id ),
	m_world( world )
{
}

void sound_player_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource	(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &sound_player_cook::on_config_loaded, this, _1 ),
		g_allocator,
		0,
		&parent
	);
}

// Target keeps strip_pointer( g_allocator ) out of line on the free path;
// the current allocator header inlines it. Both source statements match.
void sound_player_cook::delete_resource( resources::resource_base* resource )
{
	resource->~resource_base			( );
	VOSTOK_FREE_IMPL						( g_allocator, resource );
}

void sound_player_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't retrieve sound collection options" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr const>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& sounds_value	= (*config)["sounds"];

	configs::binary_config_value::const_iterator it		= sounds_value.begin();
	configs::binary_config_value::const_iterator it_end	= sounds_value.end();

	buffer_vector< resources::request >	requests		( ALLOCA( sizeof( resources::request ) * sounds_value.size() ), sounds_value.size() );

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& sound_value	= *it;
		resources::request								request;
		request.path									= sound_value["filename"];
		request.id										= resources::sound_collection_class;
		requests.push_back								( request );
	}

	resources::query_resources	(
		&requests.front(),
		requests.size(),
		boost::bind( &sound_player_cook::on_sounds_loaded, this, _1, config ),
		g_allocator,
		0,
		parent
	);

}

void sound_player_cook::on_sounds_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
	resources::query_result_for_cook* const parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't load sounds" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_value const& sounds_value	= (*config)["sounds"];

	resources::unmanaged_resource_ptr sound_scene;
	sound::world_user* sound_world_user					= 0;
	human_npc const* npc								= 0;

	resources::query_result_for_cook* const brain_unit_query = parent->get_parent_query();
	if ( brain_unit_query && brain_unit_query->get_class_id() == resources::brain_unit_class )
	{
		resources::user_data_variant* user_data			= brain_unit_query->user_data();
		ai::brain_unit_cook_params params;
		if ( user_data && user_data->try_get( params ) )
		{
			npc										= static_cast_checked< human_npc const* const >( params.npc );
			sound_scene								= params.sound_scene;
			sound_world_user							= params.sound_world_user;
		}
	}

	u32 const ai_sound_player_buffer_size				= sizeof( ai_sound_player );
	u32 const sounds_size								= sizeof( ai_sound_player::sounds_collection_type ) * sounds_value.count;
	u8* sound_player_buffer								= VOSTOK_ALLOC_IMPL( g_allocator, u8, ai_sound_player_buffer_size + sounds_size );

	ai_sound_player* const player						= new( sound_player_buffer )ai_sound_player(
		sound_scene,
		sounds_value.count,
		*sound_world_user,
		npc,
		npc
	);

	ai_sound_player::sounds_collection_type* const begin = pointer_cast< ai_sound_player::sounds_collection_type* >( sound_player_buffer + ai_sound_player_buffer_size );
	ai_sound_player::sounds_collection_type* const end	= begin + sounds_value.count;

	configs::binary_config_value::const_iterator it		= sounds_value.begin();
	configs::binary_config_value::const_iterator it_end	= sounds_value.end();

	for ( ai_sound_player::sounds_collection_type* i = begin; ( it != it_end ) && ( i != end ); ++it, ++i )
	{
		configs::binary_config_value const& sound_value	= *it;
		u32 type										= sound_value["collection_type"];
		new( i )ai_sound_player::sounds_collection_type(
			player,
			(ai::sound_collection_types)type,
			static_cast_resource_ptr< sound::sound_emitter_ptr >( data[std::distance( begin, i )].get_unmanaged_resource() ),
			sound_value["priority"]
		);
	}

	parent->set_unmanaged_resource						( player, resources::nocache_memory, ai_sound_player_buffer_size + sounds_size );
	parent->finish_query								( result_success );
}


} // namespace survarium
