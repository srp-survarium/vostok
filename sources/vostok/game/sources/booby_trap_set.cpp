////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "booby_trap_set.h"
#include "booby_trap.h"
#include "game_world.h" // get_game_material_manager() needs complete type
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

booby_trap_set::booby_trap_set( game_world& game_world ) :
	m_player_death_subscriber( boost::bind( &booby_trap_set::on_player_death, this ) ),
	m_game_world( game_world )
{
}

// STATE[STUB]
// claude@NOTE: parked - couples to base_network_client current-player internals.
// asm: line 33 m_game_world.get_game().network_client().has_bandwidth() gate; line 36
// try_place_trap(); line 40 m_inventory->holder().cast_to_base_player(); line 43
// compares that player's id byte [+0x34] against network_client().m_current_player
// ([+8], read directly w/o refcount) [+0x34] AND a sound_emitter resource_ptr null
// check; line 46 if(m_amount); line 49 toggle_ghost_model(key_down). NEXT: need the
// base_network_client current-player accessor that compiles to a direct [+8] member
// load (not the by-value get_current_player()) and base_player::id() -> [+0x34], both
// from the network-client layer which is the divergent carcass module.
void booby_trap_set::action( bool key_down )
{
	// FUNCTION BODY[0x740770]: 17
	// <0x740774>|0x004|+0x021:'33'
	// <0x740795>|0x025|+0x004:'35'
	// <0x740799>|0x029|+0x007:'36'
	// <0x7407a0>|0x030|+0x013:'40'
	// <0x7407b3>|0x043|+0x02a:'43'
	// <0x7407dd>|0x06d|+0x00a:'46'
	// <0x7407e7>|0x077|+0x00a:'49'
	// ******
}

// STATE[STUB]
// claude@NOTE: parked - the back half couples to base_network_client current-player
// internals. asm: line 58 booby_trap_core_ptr trap = traps()[index]; line 62 float4x4
// transform = mul4x3(create_rotation(angles), create_translation(position)); line 64
// insert_trap(*trap, transform) (vtable [+0x78]); line 68 --m_amount + holder().
// cast_to_base_player(); lines 71-72 the same network_client().m_current_player [+0x34]
// id compare as action(), then push the player's profile_slot_enum into a vectora
// (_M_insert_overflow_aux). NEXT: same base_network_client current-player accessor wall
// as action(); the front half (trap fetch + transform + insert_trap) is matchable once
// the network-client layer's current-player accessor lands.
void booby_trap_set::on_trap_placed_message( u8 index, float3 const& position, float3 const& angles )
{
	// LOCALS
	// float4x4 						transform
	// ******

	// FUNCTION BODY[0x740280]: 19
	// <0x740289>|0x009|+0x028:'58'
	// <0x7402b1>|0x031|+0x035:'62'
	// <0x7402e6>|0x066|+0x012:'64'
	// <0x7402f8>|0x078|+0x01f:'68'
	// <0x740317>|0x097|+0x02a:'71'
	// <0x740341>|0x0c1|+0x031:'72'
	// ******
}

// claude@NOTE: the three on_trap_*_message bodies are structurally correct but walled
// low by two cross-unit inline gaps, not a source issue here: (1) the booby_trap method
// they call (on_trap_fired_message / on_trap_disarmed_message) is still a STUB in
// booby_trap.cpp, so the call inlines to nothing (target inlines switch_to_state +
// play_fired_effects); (2) booby_trap_set_core::traps() is emitted OUT-OF-LINE in our
// base build (call traps + buffer_vector::operator[]) where the target inlines it to a
// direct m_traps load ([this+0x118]) - the same inliner-heuristic wall as
// inventory::holder(). NEXT: recovers when booby_trap.cpp is matched and the traps()
// inline wall is resolved (both other units).
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

// claude@NOTE: parked at structure level. The selected model is held in a register
// (ebx) across the whole body but the PDB records only 3 named locals
// (transform, is_placing_allowed, scheduler) - NO `model` local. pick_ghost_model()
// returns static_model_ptr BY VALUE, so a temporary cannot legally outlive its
// full-expression to be reused by add_model + the m_current assignment; yet the asm
// reuses ebx throughout. The faithful 3-local shape (model as an unnamed temp) is not
// expressible without a 4th named local. Needs the exact original construct (likely a
// pick_ghost_model variant returning a reference, or RVO into m_current). Body below is
// buildable but carries the extra `model` local.
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
		scheduler.register_for_update(
			&m_scheduler_identifier,
			boost::bind( &booby_trap_set::tick, this, _1, _2 ),
			true,
			0x7fffffff,
			0,
			0
		);
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

// claude@NOTE: holder_assigned / holder_removed are source-complete; the byte residual
// is the inventory::holder() inliner wall - our base emits `call inventory::holder`
// where the target inlines the trivial accessor to a direct m_holder load
// ([m_inventory+0x158]). Not steerable from this unit (forcing __forceinline would edit
// inventory.h, another unit). Same wall affects the holder() chains in
// remove_current_ghost_model / on_player_death / remove.
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
