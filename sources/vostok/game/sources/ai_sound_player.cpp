// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ai_sound_player.h"
#include <vostok/sound/sound_emitter.h>
#include <vostok/sound/sound_instance_proxy.h>
#include <vostok/sound/world_user.h>
#include <vostok/memory_writer.h>

namespace survarium {

 ai_sound_player::sounds_collection_type::sounds_collection_type(
	ai_sound_player*				parent,
	ai::sound_collection_types		collection_type,
	sound::sound_emitter_ptr		emitter_ptr,
	u32								collection_priority
) :
	type( collection_type ),
	priority( collection_priority )
{
	emitter.initialize_and_set_parent( parent, emitter_ptr.c_ptr() );
}

 ai_sound_player::ai_sound_player(
	resources::unmanaged_resource_ptr&		scene,
	u32										sounds_count,
	sound::world_user&						user,
	sound::sound_producer const* const		producer,
	sound::sound_receiver const* const		ignorable_receiver
) :
	m_scene( scene ),
	m_user( user ),
	m_sound_producer( producer ),
	m_ignorable_receiver( ignorable_receiver ),
	m_sounds_count( sounds_count )
{
}

 ai_sound_player::~ai_sound_player( )
{
	m_active_sound					= 0;

	sounds_collection_type const* const end		= sounds( ) + m_sounds_count;
	for ( sounds_collection_type const* i = sounds( ); i != end; ++i )
	{
		i->~sounds_collection_type	( );
	}

}

ai_sound_player::sounds_collection_type const* ai_sound_player::find( ai::sound_collection_types sound_type ) const
{
	sounds_collection_type const* const begin	= sounds( );
	sounds_collection_type const* const end		= begin + m_sounds_count;
	for ( sounds_collection_type const* i = begin; i != end; ++i )
	{
		if ( i->type == sound_type )
			return i;
	}
	return 0;
}

void ai_sound_player::play(
	ai::sound_collection_types		sound_type,
	const bool						sound_is_positioned,
	float3 const&					position
)
{
	sounds_collection_type const* type	= find( sound_type );
	R_ASSERT						( type, "such a type is absent in sound collections" );

	m_active_sound					= sound_is_positioned ?
		type->emitter->emit_point_sound( m_scene, m_user ) :
		type->emitter->emit_hud_sound( m_scene, m_user );
	m_active_sound->set_callback	( boost::bind( &ai_sound_player::on_finish_playing, this ) );

	if ( sound_is_positioned )
		m_active_sound->set_position( position );
	m_active_sound->play			( sound::once, m_sound_producer, m_ignorable_receiver );

}

void ai_sound_player::play(
	resources::unmanaged_resource_ptr		sound_to_be_played,
	boost::function< void() > const&		finish_callback,
	float3 const&							position
)
{
	sound::sound_emitter_ptr sound	= static_cast_resource_ptr< sound::sound_emitter_ptr >( sound_to_be_played );
	m_active_sound					= sound->emit_point_sound( m_scene, m_user );
	m_active_sound->set_callback	( finish_callback );
	m_active_sound->set_position	( position );
	m_active_sound->play			( sound::once, m_sound_producer, m_ignorable_receiver );


}

void ai_sound_player::play_once(
	ai::sound_collection_types		sound_type,
	bool							sound_is_positioned,
	float3 const&					position
)
{
	VOSTOK_UNREFERENCED_PARAMETER			( sound_is_positioned );
	sounds_collection_type const* type		= find( sound_type );
	R_ASSERT								( type, "such a type is absent in sound collections" );

	sound::sound_instance_proxy_ptr proxy	= type->emitter->emit_point_sound( m_scene, m_user );
	type->emitter->emit_and_play_once( m_scene, m_user, position, m_sound_producer, m_ignorable_receiver );

}

void ai_sound_player::tick( )
{
}

sound::command_result_enum ai_sound_player::on_finish_playing( )
{
	return sound::command_result_executed;
}

void ai_sound_player::clear_resources( )
{
	m_active_sound			= 0;
}

// claude@NOTE: 3-stmt STRUCTURE MATCH (the braceless if merges save_to+LOG_ERROR into one
// statement, matching the target). Byte residual: target LOG_ERROR bakes __LINE__ 407
// (push 197h) vs our shorter file, and the LOG callback's boost::function is constructed
// inline in target vs a call here (the repo-wide logging-not-gold-stubbed wall).
void ai_sound_player::on_active_sound_serialized( memory::writer* sound_thread_writer, memory::writer* current_thread_writer )
{
	current_thread_writer->write		( sound_thread_writer->pointer(), sound_thread_writer->size() );
	if ( !current_thread_writer->save_to( "Z:/test.sound_player" ) )
		LOG_ERROR						( "unable to write file [Z:/test.sound_player]" );

	VOSTOK_DELETE_IMPL					( g_allocator, current_thread_writer );

}

void ai_sound_player::on_active_sound_deserialized( memory::reader* reader, void* buf )
{
	VOSTOK_UNREFERENCED_PARAMETERS		( reader, buf );
}


} // namespace survarium
