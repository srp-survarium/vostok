// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
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

// sushi@TODO: verify the restored predicate/early-return seams and one-element erase spelling against the remaining statements.
animation::callback_return_type_enum weapon_sound_effect::on_sound_event( animation::animation_callback_params& params )
{
	if ( params.domain_data == 0xff )
		m_sounds_counter = u8( m_sounds_counter + 1 ) % m_first_view_sounds.sounds_emitters.size( );
	else
		m_sounds_counter = params.domain_data;

	base_game_scene& scene		= *m_weapon.get_game_scene( );
	sound::world_user& user		= scene.get_game( ).get_sound_world( ).get_logic_world_user( );
	const bool first_view		= m_weapon.is_player_current( );

	sound::sound_instance_proxy_ptr instance;
	if ( first_view )
	{
		instance = m_first_view_sounds.sounds_emitters[ m_sounds_counter ]->emit_hud_sound( scene.get_sound_scene( ), user );
		if ( !instance.c_ptr( ) )
			return animation::callback_return_type_call_me_again;

		instance->set_callback( boost::bind( &on_sound_finished, boost::ref( m_first_view_sounds.sounds_instances ), boost::cref( *instance ) ) );

		if ( m_first_view_sounds.sounds_instances.size( ) == m_simultaneous_sounds_queue_size )
			m_first_view_sounds.sounds_instances.erase( m_first_view_sounds.sounds_instances.begin( ) );
		m_first_view_sounds.sounds_instances.push_back( instance );

		instance->play( );
	}
	else
	{
		instance = m_third_view_sounds.sounds_emitters[ m_sounds_counter ]->emit_point_sound( scene.get_sound_scene( ), user );
		if ( !instance.c_ptr( ) )
			return animation::callback_return_type_call_me_again;

		instance->set_position( m_weapon.get_bullet_transform( ).c.xyz( ) );

		instance->set_callback( boost::bind( &on_sound_finished, boost::ref( m_third_view_sounds.sounds_instances ), boost::cref( *instance ) ) );

		if ( m_third_view_sounds.sounds_instances.size( ) == m_simultaneous_sounds_queue_size )
			m_third_view_sounds.sounds_instances.erase( m_third_view_sounds.sounds_instances.begin( ) );
		m_third_view_sounds.sounds_instances.push_back( instance );

		instance->play( );
	}

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
