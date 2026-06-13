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

// STATE[STUB]
 ai_sound_player::sounds_collection_type::sounds_collection_type(
	ai_sound_player*				parent,
	ai::sound_collection_types		collection_type,
	sound::sound_emitter_ptr		emitter_ptr,
	u32								collection_priority
) :
	// same-named params are the obvious sources (parent unused by the layout);
	// a matcher confirms when this TU is enabled
	type( collection_type ),
	priority( collection_priority )
{
	emitter.initialize_and_set_parent( parent, emitter_ptr.c_ptr() );

	// FUNCTION BODY[0x5be300]: 1
	// <0x5be300>|0x000|+0x01f:'24'
	// ******
}

// STATE[STUB]
 ai_sound_player::ai_sound_player(
	resources::unmanaged_resource_ptr&		scene,
	u32										sounds_count,
	sound::world_user&						user,
	sound::sound_producer const* const		producer,
	sound::sound_receiver const* const		ignorable_receiver
) :
	// ref/const members from the same-named params - a matcher confirms when
	// this TU is enabled
	m_scene( scene ),
	m_user( user ),
	m_sound_producer( producer ),
	m_ignorable_receiver( ignorable_receiver ),
	m_sounds_count( sounds_count )
{
	// FUNCTION BODY[0x5be090]: 14
	// <0x5be090>|0x000|+0x057:'44'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x5be0e7>|0x057|      :'59'	}
	// ******
}

// STATE[STUB]
 ai_sound_player::~ai_sound_player( )
{
	m_active_sound					= 0;

	sounds_collection_type const* const begin	= sounds( );
	sounds_collection_type const* const end		= begin + m_sounds_count;
	for ( sounds_collection_type const* i = begin; i != end; ++i )
	{
		i->~sounds_collection_type	( );
	}

	// CALL SITE INFO
	// <0x5be14f> -> void < unknown >( resources::resource_base* )
	// ******

	// FUNCTION BODY[0x5be0f0]: 13
	// <0>
	// <1>
	// <0x5be0fa>|0x00a|+0x02b:'71'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5be125>|0x035|+0x011:'77'
	// <0x5be136>|0x046|+0x00a:'78'
	// <0>
	// <0x5be140>|0x050|+0x050:'80'
	// <0>
	// ******
}

// STATE[STUB]
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

	// FUNCTION BODY[0x5bdc80]: 8
	// <0x5bdc80>|0x000|+0x006:'88'
	// <0x5bdc86>|0x006|+0x00b:'89'
	// <0x5bdc91>|0x011|+0x004:'90'
	// <0>
	// <0x5bdc95>|0x015|+0x00b:'92'
	// <0>
	// <1>
	// <0x5bdca0>|0x020|+0x002:'95'
	// ******
}

// STATE[STUB]
void ai_sound_player::play(
	ai::sound_collection_types		sound_type,
	const bool						sound_is_positioned,
	float3 const&					position
)
{
	sounds_collection_type const* type	= find( sound_type );
	R_ASSERT						( type, "such a type is absent in sound collections" );

	m_active_sound					= type->emitter->emit( m_scene, m_user );
	m_active_sound->set_callback	( boost::bind( &ai_sound_player::on_finish_playing, this ) );

	if ( sound_is_positioned )
		m_active_sound->set_position( position );
	m_active_sound->play			( sound::once, m_sound_producer, m_ignorable_receiver );

	// CALL SITE INFO
	// <0x5bdf1e> -> void < unknown >( float3 const& )
	// <0x5bdf3b> -> void < unknown >( sound::playback_mode, sound::sound_producer const* const, sound::sound_receiver const* const )
	// ******

	// FUNCTION BODY[0x5bdd90]: 13
	// <0x5bdda5>|0x015|+0x028:'104'
	// <0>
	// <1>
	// <0x5bddcd>|0x03d|+0x0ce:'107'
	// <0x5bde9b>|0x10b|+0x06e:'108'
	// <0x5bdf09>|0x179|+0x006:'109'
	// <0x5bdf0f>|0x17f|+0x011:'110'
	// <0x5bdf20>|0x190|+0x01d:'111'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void ai_sound_player::play(
	resources::unmanaged_resource_ptr		sound_to_be_played,
	boost::function< void() > const&		finish_callback,
	float3 const&							position
)
{
	sound::sound_emitter_ptr sound	= static_cast_resource_ptr< sound::sound_emitter_ptr >( sound_to_be_played );
	m_active_sound					= sound->emit( m_scene, m_user );
	m_active_sound->set_callback	( finish_callback );
	m_active_sound->set_position	( position );
	m_active_sound->play			( sound::once, m_sound_producer, m_ignorable_receiver );

	// LOCALS
	// sound::sound_emitter_ptr 		sound
	// ******

	// CALL SITE INFO
	// <0x5be28b> -> void < unknown >( float3 const& )
	// <0x5be2a8> -> void < unknown >( sound::playback_mode, sound::sound_producer const* const, sound::sound_receiver const* const )
	// ******

	// FUNCTION BODY[0x5be1d0]: 5
	// <0x5be1d3>|0x003|+0x019:'125'
	// <0x5be1ec>|0x01c|+0x079:'126'
	// <0x5be265>|0x095|+0x016:'127'
	// <0x5be27b>|0x0ab|+0x012:'128'
	// <0x5be28d>|0x0bd|+0x01d:'129'
	// ******
}

// STATE[STUB]
void ai_sound_player::play_once(
	ai::sound_collection_types		sound_type,
	bool							sound_is_positioned,
	float3 const&					position
)
{
	VOSTOK_UNREFERENCED_PARAMETER			( sound_is_positioned );
	sounds_collection_type const* type		= find( sound_type );
	R_ASSERT								( type, "such a type is absent in sound collections" );

	sound::sound_instance_proxy_ptr proxy	= type->emitter->emit( m_scene, m_user );
	type->emitter->emit_and_play_once( m_scene, m_user, position, m_sound_producer, m_ignorable_receiver );

	// LOCALS
	// sound::sound_instance_proxy_ptr 	proxy
	// ******

	// FUNCTION BODY[0x5bdcb0]: 6
	// <0>
	// <0x5bdcb3>|0x003|+0x094:'139'
	// <0x5bdd47>|0x097|-0x06a:'139'
	// <0>
	// <1>
	// <0x5bdcdd>|0x02d|+0x01b:'142'
	// <0x5bdcf8>|0x048|+0x02b:'143'
	// ******
}

// STATE[STUB]
void ai_sound_player::tick( )
{
	// FUNCTION BODY[0x5bdc70]: 39
	// <0x5bdc70>|0x000|+0x000:'147'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <0x5bdc70>|0x000|      :'187'	}
	// ******
}

// STATE[STUB]
sound::command_result_enum ai_sound_player::on_finish_playing( )
{
	return sound::command_result_executed;	// buildability return

	// FUNCTION BODY[0x5bdc50]: 3
	// <0>
	// <1>
	// <0x5bdc50>|0x000|+0x002:'374'
	// ******
}

// STATE[STUB]
void ai_sound_player::clear_resources( )
{
	// dbg scene/active-sound members removed from the canonical layout
	m_active_sound			= 0;

	// FUNCTION BODY[0x5bdd50]: 5
	// <0x5bdd50>|0x000|+0x001:'378'	{
	// <0>
	// <1>
	// <0x5bdd51>|0x001|+0x02d:'381'
	// <0x5bdd7e>|0x02e|-0x003:'381'
	// <0>
	// <1>
	// <0x5bdd7b>|0x02b|+0x005:'384'
	// <0x5bdd80>|0x030|      :'384'	}
	// ******
}

// STATE[STUB]
void ai_sound_player::on_active_sound_serialized( memory::writer* sound_thread_writer, memory::writer* current_thread_writer )
{
	current_thread_writer->write		( sound_thread_writer->pointer(), sound_thread_writer->size() );
	if ( !current_thread_writer->save_to( "Z:/test.sound_player" ) )
	{
		LOG_ERROR( "unable to write file [Z:/test.sound_player]" );
	}

	VOSTOK_DELETE_IMPL					( g_allocator, current_thread_writer );

	// CALL SITE INFO
	// <0x5bdf77> -> void < unknown >( pcvoid, u32 )
	// <0x5be071> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5bdf50]: 7
	// <0x5bdf59>|0x009|+0x015:'404'
	// <0>
	// <1>
	// <0x5bdf6e>|0x01e|+0x0e3:'407'
	// <0>
	// <1>
	// <0x5be051>|0x101|+0x034:'410'
	// ******
}

// STATE[STUB]
void ai_sound_player::on_active_sound_deserialized( memory::reader* reader, void* buf )
{
	VOSTOK_UNREFERENCED_PARAMETERS		( reader, buf );

	// FUNCTION BODY[0x5bdc60]: 2
	// <0x5bdc60>|0x000|+0x000:'456'	{
	// <0>
	// <1>
	// <0x5bdc60>|0x000|      :'459'	}
	// ******
}


} // namespace survarium
