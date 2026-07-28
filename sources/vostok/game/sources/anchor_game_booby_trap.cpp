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

			// on_trap_fired_message is a non-virtual the target only ever calls
			// internally (inlined into bts::on_trap_fired_message), so LTCG gives the
			// standalone copy a custom this-in-esi convention. An address-take sink would
			// pin stock __thiscall (ecx) and diverge; a guarded DIRECT CALL participates
			// in the LTCG call graph as a normal call and reproduces the convention.
			// (see patterns/anchor-direct-call-this-convention.md)
			trap.on_trap_fired_message( );

			// pick_ghost_model(bool) is reached only internally (inlined into
			// pick_current_ghost_model); the target gives the standalone copy a custom
			// convention (this in ecx, sret in eax, ret 4). An address-take sink forces a
			// stack-sret/ret-8 copy that diverges; the guarded direct call reproduces it.
			static bool volatile s_flag = false;
			keep( set.pick_ghost_model( s_flag ) );
		}

		// ---- booby_trap ------------------------------------------------------
		typedef survarium::booby_trap bt;
		keep( &bt::on_trap_disarmed_message );
		keep( &bt::defuse_completed );
		keep( &bt::switch_to_state );
		keep( &bt::register_tick );
		keep( &bt::unregister_tick );
		// on_trap_fired_message reached by the guarded direct call above (custom
		// this-register convention). on_new_state / play_fired_effects are reached
		// transitively (switch_to_state -> on_new_state -> play_fired_effects;
		// on_trap_fired_message -> both); address-taking any of these would pin stock
		// __thiscall, so they are kept by the call graph, not a member-fn-ptr sink.

		// ---- booby_trap_set --------------------------------------------------
		typedef survarium::booby_trap_set bts;
		keep( &bts::on_trap_removed_message );
		keep( &bts::on_trap_disarmed_message );
		keep( &bts::get_game_material_manager );
		keep( &bts::action );
		keep( &bts::remove );
		keep( &bts::holder_assigned );
		keep( &bts::holder_removed );
		keep( &bts::tick );
		keep( &bts::on_player_death );
	}
} // namespace vostok
