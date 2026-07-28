////////////////////////////////////////////////////////////////////////////
//	Created		: 02.03.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "sound_player_cook.h"
#include <vostok/ai/sound_collection_types.h>
#include <vostok/ai/npc.h>
#include <vostok/sound/world.h>
#include <vostok/sound/sound_emitter.h>
#include "ai_sound_player.h"
#include "human_npc.h"

namespace survarium {

// NOT HARVESTED: structural divergence. Canonical sound_player_cook dropped the
// m_scene and m_dbg_input_world members and the scene-taking ctors, and the
// ai_sound_player ctor arity differs (no scene / input_world overloads). Needs
// manual review.
void sound_player_cook::on_sounds_loaded	( resources::queries_result& data, configs::binary_config_ptr config  )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't load sounds" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_value const& sounds_value	= (*config)["sounds"];

	sound::world_user* world_user					= 0;
	switch ( get_class_id() )
	{
		case resources::sound_player_logic_class:
			world_user									= &m_world->get_logic_world_user();
			break;

#ifndef MASTER_GOLD
		case resources::sound_player_editor_class:
			world_user									= &m_world->get_editor_world_user();
			break;
#endif // #ifndef MASTER_GOLD

		default:
			NODEFAULT									( );
	}
	R_ASSERT											( world_user );


	human_npc const* npc								= 0;

	resources::query_result_for_cook* const brain_unit_query	= parent->get_parent_query	( );
	if ( brain_unit_query && brain_unit_query->get_class_id( ) == resources::brain_unit_class )
	{
		resources::user_data_variant* user_data			= brain_unit_query->user_data( );
		ai::brain_unit_cook_params params;
		if ( user_data && user_data->try_get( params ) )
		{
			npc											= static_cast_checked<human_npc const* const>( params.npc );
		}
	}

	u32 const ai_sound_player_buffer_size				= sizeof( ai_sound_player );
	u32 const sounds_size								= sizeof( ai_sound_player::sounds_collection_type ) * sounds_value.count;
	u8* sound_player_buffer								= static_cast<u8*>( VOSTOK_MALLOC_IMPL( g_allocator, ai_sound_player_buffer_size + sounds_size, "ai_sound_player" ) );

	ai_sound_player* player;
	if ( m_dbg_input_world )
		player			= new( sound_player_buffer )ai_sound_player( m_scene, sounds_value.count, *world_user, m_dbg_input_world, npc, npc );
	else
		player			= new( sound_player_buffer )ai_sound_player( m_scene, sounds_value.count, *world_user, npc, npc );

	ai_sound_player::sounds_collection_type* const begin = pointer_cast<ai_sound_player::sounds_collection_type*>( sound_player_buffer + ai_sound_player_buffer_size );
	ai_sound_player::sounds_collection_type* const end	= begin + sounds_value.count;

	configs::binary_config_value::const_iterator it		= sounds_value.begin( );
	configs::binary_config_value::const_iterator it_end	= sounds_value.end( );

	for ( ai_sound_player::sounds_collection_type* i = begin; ( it != it_end ) && ( i != end ); ++it, ++i )
	{
		configs::binary_config_value const& sound_value	= *it;
		u32 type										= sound_value["collection_type"];
		new( i )ai_sound_player::sounds_collection_type	(
			player,
			(ai::sound_collection_types)type,
			static_cast_resource_ptr< sound::sound_emitter_ptr >( data[std::distance( begin, i )].get_unmanaged_resource() ),
			sound_value["priority"]
		);
	}

	parent->set_unmanaged_resource						( player, resources::nocache_memory, sizeof( ai_sound_player ) + sounds_size );
	parent->finish_query								( result_success );
}

} // namespace survarium
