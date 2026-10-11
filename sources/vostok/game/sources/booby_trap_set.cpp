// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "booby_trap_set.h"
#include "booby_trap.h"
#include "game_world.h" // get_game_material_manager() needs complete type
#include "game.h"					// network_client()
#include "base_network_client.h"	// has_bandwidth() / is_player_current()
#include <vostok/game_core/base_player.h>		// cast_to_base_player()->id
#include <vostok/game_core/inventory_holder.h>	// cast_to_base_player()
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {







booby_trap_set::booby_trap_set( game_world& game_world ) :
	m_player_death_subscriber( boost::bind( &booby_trap_set::on_player_death, this ) ),
	m_game_world( game_world )
{
}







void booby_trap_set::action( bool key_down )
{
	if ( !m_game_world.get_game( ).get_network_client( )->has_bandwidth( )
		&& !key_down )
		booby_trap_set_core::try_place_trap( );

	base_player* player = m_inventory->holder( ).cast_to_base_player( );
	if ( m_game_world.get_game( ).get_network_client( )->is_player_current( player->id )
		&& m_amount )
		toggle_ghost_model( key_down );
}

void booby_trap_set::on_trap_placed_message( u8 index, float3 const& position, float3 const& angles )
{
	booby_trap_core_ptr trap = traps( )[ index ];

	float4x4	transform = mul4x3( create_rotation( angles ), create_translation( position ) );

	insert_trap( *trap, transform );

	--m_amount;

	if ( m_game_world.get_game( ).get_network_client( )->is_player_current( m_inventory->holder( ).cast_to_base_player( )->id ) )
		m_game_world.game_ui.add_quick_slot_to_update( profile_slot_id( ) );
}







void booby_trap_set::on_trap_removed_message( u8 index )
{
	booby_trap_core_ptr trap = traps( )[ index ];
	remove_trap( *trap );
}

void booby_trap_set::on_trap_fired_message( u8 index )
{
	booby_trap_core_ptr trap = traps( )[ index ];
	static_cast< booby_trap* >( trap.c_ptr( ) )->on_trap_fired_message( );
}

void booby_trap_set::on_trap_disarmed_message( u8 index )
{
	booby_trap_core_ptr trap = traps( )[ index ];
	static_cast< booby_trap* >( trap.c_ptr( ) )->on_trap_disarmed_message( );
}

game_material_manager const& booby_trap_set::get_game_material_manager( )
{
	return m_game_world.get_game_material_manager( );
}

void booby_trap_set::tick( const u32 time_delta_ms, const u32 current_time_ms )
{
	if ( m_amount == 0 )
	{
		toggle_ghost_model( false );
		return;
	}

	float4x4	transform;
	bool		is_placing_allowed = get_visible_place_transform( transform );

	if ( pick_current_ghost_model( transform, is_placing_allowed ) )
		return;

	m_game_world.scene_renderer( ).update_model( m_game_world.render_scene( ), m_current_rendering_model->m_render_model, transform );
}

void booby_trap_set::toggle_ghost_model( bool enable )
{
	if ( enable )
	{
		float4x4	transform;
		bool		is_placing_allowed = get_visible_place_transform( transform );
		pick_current_ghost_model( transform, is_placing_allowed );
	}
	else
	{
		remove_current_ghost_model( );
	}
}

// PDB spells the return vostok::render::static_model_instance_ptr - our render
// tree typedefs it static_model_ptr
render::static_model_ptr booby_trap_set::pick_ghost_model( bool is_placing_allowed )
{
	return is_placing_allowed ? m_model_ghost_allowed : m_model_ghost_denied;
}








bool booby_trap_set::pick_current_ghost_model( float4x4 const& transform, bool is_placing_allowed )
{
	render::static_model_ptr model = pick_ghost_model( is_placing_allowed );
	if ( model == m_current_rendering_model )
		return false;

	if ( m_current_rendering_model )
		m_game_world.scene_renderer( ).remove_model( m_game_world.render_scene( ), m_current_rendering_model->m_render_model );
	m_game_world.scene_renderer( ).add_model( m_game_world.render_scene( ), model->m_render_model, transform );

	if ( !m_current_rendering_model )
	{
		scheduler& scheduler = m_inventory->holder( ).scheduler( );
		scheduler.register_on_frame( &m_scheduler_identifier, boost::bind( &booby_trap_set::tick, this, _1, _2 ), true );
	}

	m_current_rendering_model = model;
	return true;
}

void booby_trap_set::remove_current_ghost_model( )
{
	if ( m_current_rendering_model )
	{
		m_game_world.scene_renderer( ).remove_model( m_game_world.render_scene( ), m_current_rendering_model->m_render_model );
		m_current_rendering_model = NULL;
		m_inventory->holder( ).scheduler( ).unregister( &m_scheduler_identifier );
	}
}







void booby_trap_set::holder_assigned( )
{
	m_inventory->holder( ).cast_to_base_player( )
		->subscribe_on_player_death( &m_player_death_subscriber );
}

void booby_trap_set::holder_removed( )
{
	m_inventory->holder( ).cast_to_base_player( )
		->unsubscribe_from_player_death( &m_player_death_subscriber );
}

void booby_trap_set::on_player_death( )
{
	remove_current_ghost_model( );
}

void booby_trap_set::remove( )
{
	remove_current_ghost_model( );
	booby_trap_set_core::remove( );
}


} // namespace survarium
