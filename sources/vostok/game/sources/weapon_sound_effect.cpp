// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "weapon_sound_effect.h"

#include "weapon.h"
#include "base_game_scene.h"
#include "game.h"
#include "base_network_client.h"
#include "player.h"

#include <vostok/game_core/hit_initiator.h>
#include <vostok/sound/world.h>
#include <vostok/sound/world_user.h>
#include <vostok/sound/sound_instance_proxy.h>

#include <boost/bind.hpp>

#include <algorithm>		// std::find over the sound-instance list

namespace survarium {

weapon_sound_effect::weapon_sound_effect(
	weapon&								weapon,
	sound::sound_emitter_ptr* const		first_view_sounds_emitters,
	const u8							first_view_sounds_emitters_count,
	sound::sound_instance_proxy_ptr* const	first_view_sounds_instances,
	const u8							max_first_view_sounds_instances_count,
	sound::sound_emitter_ptr* const		third_view_sounds_emitters,
	const u8							third_view_sounds_emitters_count,
	sound::sound_instance_proxy_ptr* const	third_view_sounds_instances,
	const u8							max_third_view_sounds_instances_count,
	bool								stop_sounds_on_state_finalize,
	u8									simultaneous_sounds_queue_size
)
	:	m_first_view_sounds				( first_view_sounds_emitters, first_view_sounds_emitters_count, first_view_sounds_instances, max_first_view_sounds_instances_count ),
		m_third_view_sounds				( third_view_sounds_emitters, third_view_sounds_emitters_count, third_view_sounds_instances, max_third_view_sounds_instances_count ),
		m_weapon						( weapon ),
		m_stop_sounds_on_state_finalize	( stop_sounds_on_state_finalize ),
		m_sounds_counter				( 0 ),
		m_simultaneous_sounds_queue_size( simultaneous_sounds_queue_size )
{
}

void weapon_sound_effect::initialize( )
{
	m_sounds_counter = 0xff;
}

// claude@NOTE: structure faithful (guard + 2 clear_instances, both inlining to
// buffer_vector::destroy). Capped: target folds the `if` condition into the prologue
// block (no line-table row), while our base emits `push esi` + a separate line for the
// `if` -> 1 extra statement / 1-byte SIZE. Codegen line-attribution residual, not
// source-steerable.
void weapon_sound_effect::finalize( )
{
	if ( m_stop_sounds_on_state_finalize )
	{
		m_first_view_sounds.clear_instances( );
		m_third_view_sounds.clear_instances( );
	}
}

void on_sound_finished(
	buffer_vector< sound::sound_instance_proxy_ptr >&	instances,
	sound::sound_instance_proxy const&		instance
)
{
	sound::sound_instance_proxy_ptr* const found = std::find( instances.begin( ), instances.end( ), &instance );
	instances.erase( found );
}

// claude@NOTE: structure faithful (26-stmt sound-event state machine, both first/third-view
// playback branches present, correct members/branches/calls). Residual is MSVC register
// scheduling: the target keeps &scene.get_sound_scene() (scene+0x9c) live in edi from the
// scene statement (lea edi,[eax+9Ch]) while our base defers it (add edi,9Ch at the emit), plus
// per-branch reg churn; and the if/erase statements differ only in line attribution (the begin()
// CSE'd into size()'s m_begin load on the target). Not source-steerable.
animation::callback_return_type_enum weapon_sound_effect::on_sound_event( animation::animation_callback_params& params )
{
	if ( params.domain_data == 0xff )
		m_sounds_counter = u8( m_sounds_counter + 1 ) % m_first_view_sounds.sounds_emitters.size( );
	else
		m_sounds_counter = params.domain_data;

	base_game_scene& scene		= *m_weapon.get_game_scene( );
	sound::world_user& user		= scene.get_game( ).get_sound_world( ).get_logic_world_user( );
	const bool first_view		= scene.get_game( ).get_network_client( )->is_player_current( m_weapon.hit_initiator_holder( )->id );

	sound::sound_instance_proxy_ptr instance;
	if ( first_view )
	{
		instance = m_first_view_sounds.sounds_emitters[ m_sounds_counter ]->emit_hud_sound( scene.get_sound_scene( ), user );
		if ( instance.c_ptr( ) )
		{
			instance->set_callback( boost::bind( &on_sound_finished, boost::ref( m_first_view_sounds.sounds_instances ), boost::cref( *instance ) ) );

			if ( m_first_view_sounds.sounds_instances.size( ) == m_simultaneous_sounds_queue_size )
				m_first_view_sounds.sounds_instances.erase( m_first_view_sounds.sounds_instances.begin( ), m_first_view_sounds.sounds_instances.begin( ) + 1 );
			m_first_view_sounds.sounds_instances.push_back( instance );

			instance->play( );
		}
	}
	else
	{
		instance = m_third_view_sounds.sounds_emitters[ m_sounds_counter ]->emit_point_sound( scene.get_sound_scene( ), user );
		if ( instance.c_ptr( ) )
		{
			instance->set_position( m_weapon.get_bullet_transform( ).c.xyz( ) );

			instance->set_callback( boost::bind( &on_sound_finished, boost::ref( m_third_view_sounds.sounds_instances ), boost::cref( *instance ) ) );

			if ( m_third_view_sounds.sounds_instances.size( ) == m_simultaneous_sounds_queue_size )
				m_third_view_sounds.sounds_instances.erase( m_third_view_sounds.sounds_instances.begin( ), m_third_view_sounds.sounds_instances.begin( ) + 1 );
			m_third_view_sounds.sounds_instances.push_back( instance );

			instance->play( );
		}
	}

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
