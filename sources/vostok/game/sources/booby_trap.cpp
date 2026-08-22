////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "booby_trap.h"
#include "game_world.h"
#include "game.h"
#include "base_network_client.h"
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

booby_trap::booby_trap( game_world& gw ) :
	m_game_world( gw )
{
}

booby_trap::~booby_trap( )
{
	if ( m_particle_fired )
		m_game_world.scene_renderer( ).remove_particle_system_instance(
			m_game_world.render_scene( ),
			m_particle_fired );
}

void booby_trap::defuse_completed( )
{
	if ( m_game_world.get_game( ).network_client( ).has_bandwidth( ) )
		return;

	booby_trap_core::defuse_completed( );
}

void booby_trap::switch_to_state( booby_trap_state new_state )
{
	booby_trap_state const old_state = m_trap_state;

	if ( !m_game_world.get_game( ).network_client( ).has_bandwidth( ) )
		booby_trap_core::switch_to_state( new_state );
	else
		m_trap_state = new_state;

	if ( m_trap_state != old_state )
		on_new_state( old_state );
}

// claude@NOTE: 13/13 statements, this-in-esi convention matched. Residual is the
// booby_trap_set_core::config() inliner wall - our base emits `call config` where the
// target inlines the trivial accessor to a direct m_config.defuse_by_hit load
// ([m_owner+0x140]); same wall family as inventory::holder() / traps(), not steerable
// here (would force config() inline in game_core/booby_trap_set_core.h, another unit).
// The new-state model slot is captured as a reference so /Ox hoists &m_models[m_trap_state]
// into the prologue exactly as the target (reading m_models[m_trap_state] twice did not).
void booby_trap::on_new_state( booby_trap_state old_state )
{
	render::static_model_ptr& new_model = m_models[ m_trap_state ];

	if ( m_models[ old_state ] )
		m_game_world.scene_renderer( ).remove_model( m_game_world.render_scene( ), m_models[ old_state ]->m_render_model );

	if ( new_model )
		m_game_world.scene_renderer( ).add_model( m_game_world.render_scene( ), new_model->m_render_model, m_transform );

	if ( old_state == booby_trap_state_armed )
	{
		if ( m_game_world.get_game( ).network_client( ).has_bandwidth( ) )
		{
			usable_object::remove( );
			collision_sensor::remove( );
			if ( m_owner->config( ).defuse_by_hit )
				hittable_object::remove( );
		}
	}

	if ( !m_game_world.get_game( ).network_client( ).has_bandwidth( ) )
	{
		if ( m_trap_state == booby_trap_state_fired )
			play_fired_effects( );
	}
}

void booby_trap::on_trap_fired_message( )
{
	switch_to_state( booby_trap_state_fired );
	play_fired_effects( );
}

void booby_trap::on_trap_disarmed_message( )
{
	switch_to_state( booby_trap_state_disarmed );
}

void booby_trap::register_tick( scheduler& scheduler )
{
	if ( m_game_world.get_game( ).network_client( ).has_bandwidth( ) )
		return;

	booby_trap_core::register_tick( scheduler );
}

void booby_trap::unregister_tick( scheduler& scheduler )
{
	if ( m_game_world.get_game( ).network_client( ).has_bandwidth( ) )
		return;

	booby_trap_core::unregister_tick( scheduler );
}

void booby_trap::play_fired_effects( ) const
{
	m_game_world.scene_renderer( ).play_particle_system( m_game_world.render_scene( ), m_particle_fired, m_transform );
	m_game_world.play_sound( m_sound_fired, m_transform.c.xyz( ) );
}


} // namespace survarium
