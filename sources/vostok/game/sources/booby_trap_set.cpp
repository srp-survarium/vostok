////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
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

// claude@NOTE: the ctor (+ remove / on_player_death / remove_current_ghost_model) carry a
// cluster-wide codegen residual: the target frames these with an 8-byte-aligned stack
// (`push ebp; mov ebp,esp; and esp,0FFFFFFF8h`) - the alignment the boost::function /
// scheduler buffer copy (the `movq xmm0` moves) wants - where our /Ox base uses a plain
// `sub esp` frame and tail-calls the trailing base call. The bodies match; the frame
// shape + tail-call are an MSVC stack-alignment heuristic difference, not a body change.
booby_trap_set::booby_trap_set( game_world& game_world ) :
	m_player_death_subscriber( boost::bind( &booby_trap_set::on_player_death, this ) ),
	m_game_world( game_world )
{
}

// claude@NOTE: 5/7 statements; is_player_current() inlines (its m_current_player [+8] +
// ->id [+0x34] body is direct, matching the target) once base_network_client.h is
// complete here. The `player` reference is captured first so the cast (line 40) and the
// is_player_current test (line 43) split into the target's separate statements. Residual:
// the two-condition guard fuses into one stmt under /Ox (vs the target's split lines) and
// the holder() chain rides the inventory::holder() inliner wall (another unit).
void booby_trap_set::action( bool key_down )
{
	if ( !m_game_world.get_game( ).network_client( ).has_bandwidth( )
		&& !key_down )
		booby_trap_set_core::try_place_trap( );

	base_player* player = m_inventory->holder( ).cast_to_base_player( );
	if ( m_game_world.get_game( ).network_client( ).is_player_current( player->id )
		&& m_amount )
		toggle_ghost_model( key_down );
}

// claude@NOTE: front half (lines 58-68) reconstructed; the back half is a cross-module
// wall. The target's last two statements (lines 71-72) push this->profile_slot_id() into
// a vectora held on base_network_client::m_current_player ([client+8] -> player[+0x208]),
// gated by is_player_current(player->id). m_current_player is a PROTECTED member of
// base_network_client (network-client carcass module) and the target reuses the same
// player register the inlined is_player_current() leaves behind, so the push cannot be
// expressed here without either friending base_network_client or a public accessor that
// returns the live current player. NEXT: add a base_network_client accessor (or friend)
// exposing m_current_player + the player's placed-slot vectora, both in the network-client
// layer, then the if(is_player_current(...)) { current_player->...push_back(profile_slot_id()) }.
void booby_trap_set::on_trap_placed_message( u8 index, float3 const& position, float3 const& angles )
{
	booby_trap_core_ptr trap = traps( )[ index ];

	float4x4	transform = mul4x3( create_rotation( angles ), create_translation( position ) );

	insert_trap( *trap, transform );

	--m_amount;
}

// claude@NOTE: the three on_trap_*_message bodies are structurally faithful (1 stmt,
// only the `index` param local). The byte residual is the booby_trap_set_core::traps()
// inliner wall: our base emits `call traps` + `buffer_vector::operator[]` out-of-line
// where the target inlines them to a direct m_traps load ([this+0x118]) + index. Same
// inliner-heuristic wall as inventory::holder(); not steerable from this unit (would
// require forcing the accessors inline in game_core/booby_trap_set_core.h).
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

// claude@NOTE: 12/12 statements but two byte residuals remain. (1) The early-out
// `if (model == m_current) return false;` is a separate statement in our base; the target
// folds the model-temp release into a SHARED epilogue (a single ret path), so our explicit
// `return false` emits its own model dtor + ret. (2) The boost::bind register_for_update
// argument block (TRGT_ONLY rows) schedules differently. The `model` by-value local is fine
// (this build is /Ox, the named local is elided from the PDB like action's `player`). The
// shape is faithful; the residual is the early-return cleanup + boost::bind scheduling.
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
