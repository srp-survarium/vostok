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
#include <vostok/game_core/scheduler.h>

namespace survarium {

	// file-local free function in game.cpp (no public header)
	math::uint2 parse_resolution( pcstr in_str );

	// Keep the simple_game_project + render_visual public methods past /OPT:REF.
	// simple_game_project is concrete (no pure virtuals), so constructing one keeps
	// its ctor/dtor/vtable; the public methods are then called. Args are sourced
	// through volatile sinks so LTCG cannot prove the constant null this anchor is
	// the sole caller with. The real caller is project_cooker_simple (still a stub).
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

		void ( survarium::game::* const m_commit )( )			= &survarium::game::commit_suicide;
		void ( survarium::game::* const m_respawn )( )			= &survarium::game::respawn_local_player;
		void ( survarium::game::* const m_discard )( )			= &survarium::game::discard_current_match;
		void ( survarium::game::* const m_exit )( pcstr )		= &survarium::game::exit;
		void ( survarium::game::* const m_sw_world )( )			= &survarium::game::switch_to_game_world;
		void ( survarium::game::* const m_sw_main )( )			= &survarium::game::switch_to_main_menu;
		void ( survarium::game::* const m_sw_login )( login_menu_status_enum )	= &survarium::game::switch_to_login;
		void ( survarium::game::* const m_act_menu )( )			= &survarium::game::activate_main_menu;
		void ( survarium::game::* const m_deact_menu )( )		= &survarium::game::deactivate_main_menu;
		survarium::flash_factory& ( survarium::game::* const m_flash )( )		= &survarium::game::get_flash_factory;
		void ( survarium::game::* const m_caanc )( vostok::fixed_string< 512 >, const bool )	= &survarium::game::create_and_assign_network_client;
		void ( survarium::game::* const m_oqbncsr )( survarium::scene_ready_type )	= &survarium::game::on_queried_by_network_client_scene_ready;
		void ( survarium::game::* const m_cnc )( const bool )	= &survarium::game::create_network_client;

		// private AAE helpers (reachable via the friend declaration in game.h).
		// NOTE: switch_to_scene is deliberately NOT address-taken - the target's
		// standalone copy uses an edi/esi tail-call convention (it is only ever
		// reached inlined into switch_to_*); address-taking forces a push-prologue
		// __thiscall copy that mismatches. It stays reached via the inlined callers.
		void ( survarium::game::* const m_load_cmd )( pcstr )	= &survarium::game::load_cmd;
		void ( survarium::game::* const m_unload_cmd )( pcstr )	= &survarium::game::unload_cmd;
		void ( survarium::game::* const m_reg_cc )( )			= &survarium::game::register_console_commands;
		void ( survarium::game::* const m_reg_cook )( )			= &survarium::game::register_cooks;
		void ( survarium::game::* const m_upd_stats )( const u32 )	= &survarium::game::update_stats;
		void ( survarium::game::* const m_on_rend )( resources::queries_result& )	= &survarium::game::on_renderer_created;
		void ( survarium::game::* const m_qbr )( )				= &survarium::game::query_base_resources;
		void ( survarium::game::* const m_orowc )( resources::queries_result& )	= &survarium::game::on_render_output_window_created;

		// game_world::load + its on_project_loaded callback are non-virtual and not yet
		// reached by the real call graph (game_module loader is still a stub), so address-
		// take them through the sink to keep them past /OPT:REF for pairing.
		void ( survarium::game_world::* const m_gw_load )(
			pcstr, resources::request*, resources::request*,
			vostok::variant< 32 > const**,
			boost::function< void( resources::queries_result& ) > const& )	= &survarium::game_world::load;

		s_sink = *( pcvoid const* )&m_commit;
		s_sink = *( pcvoid const* )&m_respawn;
		s_sink = *( pcvoid const* )&m_discard;
		s_sink = *( pcvoid const* )&m_exit;
		s_sink = *( pcvoid const* )&m_sw_world;
		s_sink = *( pcvoid const* )&m_sw_main;
		s_sink = *( pcvoid const* )&m_sw_login;
		s_sink = *( pcvoid const* )&m_act_menu;
		s_sink = *( pcvoid const* )&m_deact_menu;
		s_sink = *( pcvoid const* )&m_flash;
		s_sink = *( pcvoid const* )&m_caanc;
		s_sink = *( pcvoid const* )&m_oqbncsr;
		s_sink = *( pcvoid const* )&m_cnc;
		s_sink = *( pcvoid const* )&m_load_cmd;
		s_sink = *( pcvoid const* )&m_unload_cmd;
		s_sink = *( pcvoid const* )&m_reg_cc;
		s_sink = *( pcvoid const* )&m_reg_cook;
		s_sink = *( pcvoid const* )&m_upd_stats;
		s_sink = *( pcvoid const* )&m_on_rend;
		s_sink = *( pcvoid const* )&m_qbr;
		s_sink = *( pcvoid const* )&m_orowc;
		s_sink = *( pcvoid const* )&m_gw_load;
	}

} // namespace survarium
