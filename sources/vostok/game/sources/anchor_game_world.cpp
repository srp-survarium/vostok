#include "pch.h"

// Game-module /OPT:REF reachability anchor for the survarium::game public methods
// matched in game.cpp (the WORLD-cluster game-class drain). game is abstract
// (engine_user::world::load stays pure-virtual), so it can never be constructed;
// instead use_game_world() address-takes its public methods through a volatile sink
// so the linker keeps them past /OPT:REF and objdiff can pair them. The private
// helpers (switch_to_scene, load_cmd, ... ) are reached transitively from these
// public callers, and from use_game_skeleton() in anchor_game.cpp.
//
// Driven by a SINGLE call line from anchor_game() (anchor_game.cpp). Retire once the
// real game call graph (game_module::create_world) reaches these for itself.

#include "game.h"
#include "game_world.h"
#include "game_project.h"
#include "render_visual.h"
#include "base_game_scene.h"
#include "login_menu_status_enum.h"	// switch_to_login arg (anchor direct call)
#include <vostok/game_core/scheduler.h>

namespace survarium {

	// file-local free function in game.cpp (no public header)
	math::uint2 parse_resolution( pcstr in_str );

	// Keep the simple_game_project + render_visual public methods past /OPT:REF.
	// simple_game_project is concrete (no pure virtuals), so constructing one keeps
	// its ctor/dtor/vtable; the public methods are then called. Args are sourced
	// through volatile sinks so LTCG cannot prove the constant null this anchor is
	// the sole caller with. The real caller is project_cooker_simple.
	void use_game_project( )
	{
		static volatile bool s_run = false;
		if ( !s_run )
			return;

		static survarium::base_game_scene* volatile	s_scene	= 0;
		static survarium::scheduler* volatile			s_sched	= 0;
		static vostok::resources::queries_result* volatile	s_data	= 0;
		static u8 volatile								s_id	= 0;

		survarium::simple_game_project	project( *s_scene );
		project.insert( *s_sched );
		project.remove( );
		project.on_npc_loaded( *s_data );
		project.all_loaded( );
		project.get_items_container( s_id );

		survarium::render_visual		visual;
		visual.insert( *s_scene );
		visual.remove( *s_scene );
	}

	void use_game_world( )
	{
		use_game_project( );

		static pcvoid volatile s_sink = 0;

		// keep the file-local parse_resolution free function past /OPT:REF (its only
		// real caller, on_configs_loaded, is still a stub)
		static pcstr volatile s_res = 0;
		static volatile bool s_run_pr = false;
		if ( s_run_pr )
			parse_resolution( s_res );

		// These game methods are only ever called internally in the shipped EXE, so
		// LTCG gives their standalone copies a specialized this-in-register convention
		// (this in eax/esi/edi, not the __thiscall ecx). Address-taking a method forces
		// a stock __thiscall copy that mismatches that convention; a guarded DIRECT call
		// (never executed - s_run_g stays false) keeps the symbol past /OPT:REF AND lets
		// LTCG pick the same specialized convention, so they pair byte-exact. Pattern:
		// patterns/anchor-direct-call-this-convention.md.
		static survarium::game* volatile s_g = 0;
		static volatile bool s_run_g = false;
		// source switch_to_login's status through a volatile so LTCG cannot prove the
		// constant this anchor passes and propagate it into the body (it reads the param).
		static survarium::login_menu_status_enum volatile s_status = survarium::login_menu_status_connected;
		if ( s_run_g )
		{
			s_g->respawn_local_player( );
			s_g->discard_current_match( );
			s_sink = &s_g->get_flash_factory( );
			s_g->toggle_console( );
			s_g->switch_to_lobby( );
			s_g->switch_to_login( s_status );
			s_g->switch_to_main_menu( );
			s_g->switch_to_game_world( );
			s_g->create_debug_window( );
			s_g->activate_main_menu( );
			s_g->deactivate_main_menu( );
			s_g->create_lobby_menu( );
			s_g->create_login_menu( );

			// private AAE module-lifecycle helpers (game_initialize.cpp). Their
			// standalone target copies read this from a specialized register (esi/eax/
			// edi, not __thiscall ecx) because they are only ever reached internally;
			// a guarded direct call (never executed) keeps the symbol past /OPT:REF AND
			// lets LTCG pick the same convention. patterns/anchor-direct-call-this-convention.md
			s_g->initialize_input( );
			s_g->initialize_ui( );
			s_g->initialize_modules( );
			s_g->deinitialize_modules( );
		}

		void ( survarium::game::* const m_commit )( )			= &survarium::game::commit_suicide;
		void ( survarium::game::* const m_caanc )( vostok::fixed_string< 512 >, const bool )	= &survarium::game::create_and_assign_network_client;
		void ( survarium::game::* const m_oqbncsr )( survarium::scene_ready_type )	= &survarium::game::on_queried_by_network_client_scene_ready;

		// private AAE helpers (reachable via the friend declaration in game.h).
		// NOTE: switch_to_scene is deliberately NOT address-taken - the target's
		// standalone copy uses an edi/esi tail-call convention (it is only ever
		// reached inlined into switch_to_*); address-taking forces a push-prologue
		// __thiscall copy that mismatches. It stays reached via the inlined callers.
		void ( survarium::game::* const m_load_cmd )( pcstr )	= &survarium::game::load_cmd;
		void ( survarium::game::* const m_unload_cmd )( pcstr )	= &survarium::game::unload_cmd;
		void ( survarium::game::* const m_reg_cook )( )			= &survarium::game::register_cooks;
		void ( survarium::game::* const m_on_rend )( resources::queries_result& )	= &survarium::game::on_renderer_created;
		void ( survarium::game::* const m_qbr )( )				= &survarium::game::query_base_resources;
		void ( survarium::game::* const m_orowc )( resources::queries_result& )	= &survarium::game::on_render_output_window_created;


		s_sink = *( pcvoid const* )&m_commit;
		s_sink = *( pcvoid const* )&m_caanc;
		s_sink = *( pcvoid const* )&m_oqbncsr;
		s_sink = *( pcvoid const* )&m_load_cmd;
		s_sink = *( pcvoid const* )&m_unload_cmd;
		s_sink = *( pcvoid const* )&m_reg_cook;
		s_sink = *( pcvoid const* )&m_on_rend;
		s_sink = *( pcvoid const* )&m_qbr;
		s_sink = *( pcvoid const* )&m_orowc;
	}

} // namespace survarium
