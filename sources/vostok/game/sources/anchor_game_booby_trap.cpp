#include "pch.h"

// Game-module /OPT:REF reachability anchor for the BOOBY-TRAP cluster:
//   booby_trap, booby_trap_set.
//
// These carcass TUs compile into game.lib but no reachable engine call graph
// touches them yet (game_world creates the cooks, not the runtime objects), so
// /OPT:REF strips them from the EXE and objdiff reports their target symbols as
// "unpaired". use_game_booby_trap() address-takes each method through a volatile
// sink so the linker keeps the symbol; the ctors are pinned by a self-guarded
// construction (a CALL inside the never-taken branch is elided, but the full-object
// construction reference survives). Retire once the real game call graph (trap
// placement messages from the network client) reaches these for itself.

#include "booby_trap.h"
#include "booby_trap_set.h"
#include "game_world.h"

namespace vostok
{
	static pcvoid volatile s_booby_trap_sink = 0;

	template < typename T >
	static void keep( T m ) { s_booby_trap_sink = *( pcvoid const* )&m; }

	void use_game_booby_trap( )
	{
		static volatile bool s_run = false;
		if ( s_run )
		{
			static survarium::game_world* volatile	s_world = 0;
			survarium::booby_trap			trap( *s_world );
			survarium::booby_trap_set		set( *s_world );
		}

		// ---- booby_trap ------------------------------------------------------
		typedef survarium::booby_trap bt;
		keep( &bt::on_trap_fired_message );
		keep( &bt::on_trap_disarmed_message );
		keep( &bt::defuse_completed );
		keep( &bt::switch_to_state );
		keep( &bt::register_tick );
		keep( &bt::unregister_tick );
		keep( &bt::on_new_state );
		keep( &bt::play_fired_effects );

		// ---- booby_trap_set --------------------------------------------------
		typedef survarium::booby_trap_set bts;
		keep( &bts::on_trap_placed_message );
		keep( &bts::on_trap_removed_message );
		keep( &bts::on_trap_fired_message );
		keep( &bts::on_trap_disarmed_message );
		keep( &bts::get_game_material_manager );
		keep( &bts::action );
		keep( &bts::remove );
		keep( &bts::holder_assigned );
		keep( &bts::holder_removed );
		keep( &bts::tick );
		keep( &bts::on_player_death );
		keep( &bts::toggle_ghost_model );
		keep( &bts::pick_ghost_model );
		keep( &bts::pick_current_ghost_model );
		keep( &bts::remove_current_ghost_model );
	}
} // namespace vostok
