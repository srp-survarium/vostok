#include "pch.h"
#include "../../game_core/sources/anchor.h"	// survarium::IncludeAll + the anchor_* decls

// Game-module /OPT:REF reachability anchor for the menu non-virtual methods.
// use_game_skeleton() (anchor_game.cpp) constructs lobby_menu / main_menu, which
// keeps their ctors/dtors/vtables (so the virtual on_activate / on_*_action are
// reachable through the vtable). The non-virtual private/public menu methods
// (lobby_client(), query_account_data(), request_status_from_server(), ...) are
// reached only through the still-stubbed giant dispatchers, so /OPT:REF strips
// them from the base EXE and the delink leaves them unpaired. Address-take each
// through a volatile sink to keep them linked while the matchers fill the bodies.
//
// Dispatched from anchor_game()::anchor_game (a single call line there). Retire
// once the real menu call graph reaches these for itself.

#include "main_menu.h"
#include "lobby_menu.h"

namespace vostok
{
	void use_game_menus( )
	{
		static volatile bool s_run = false;
		if( !s_run )
			return;

		static pcvoid volatile s_sink = 0;

		// main_menu non-virtual methods
		void ( survarium::main_menu::* const mm_query )( )				= &survarium::main_menu::query_resources;
		void ( survarium::main_menu::* const mm_ready )( vostok::resources::queries_result& ) = &survarium::main_menu::on_resources_ready;
		s_sink = *( pcvoid const* )&mm_query;
		s_sink = *( pcvoid const* )&mm_ready;

		// lobby_menu non-virtual methods
		survarium::lobby_client& ( survarium::lobby_menu::* const lm_lc )( )		= &survarium::lobby_menu::lobby_client;
		survarium::messaging_client& ( survarium::lobby_menu::* const lm_mc )( )	= &survarium::lobby_menu::messaging_client;
		void ( survarium::lobby_menu::* const lm_conn )( )			= &survarium::lobby_menu::on_connected_to_lobby;
		void ( survarium::lobby_menu::* const lm_disc )( )			= &survarium::lobby_menu::on_disconnected_from_lobby;
		void ( survarium::lobby_menu::* const lm_qli )( )			= &survarium::lobby_menu::query_lobby_info;
		void ( survarium::lobby_menu::* const lm_qad )( )			= &survarium::lobby_menu::query_account_data;
		void ( survarium::lobby_menu::* const lm_stll )( )			= &survarium::lobby_menu::switch_to_level_loading;
		void ( survarium::lobby_menu::* const lm_rss )( u32 )		= &survarium::lobby_menu::request_status_from_server;
		void ( survarium::lobby_menu::* const lm_rssi )( const u32, const u32 ) = &survarium::lobby_menu::request_status_from_server_impl;
		void ( survarium::lobby_menu::* const lm_rfss )( u32 )		= &survarium::lobby_menu::request_friends_status_from_server;
		void ( survarium::lobby_menu::* const lm_rfssi )( const u32, const u32 ) = &survarium::lobby_menu::request_friends_status_from_server_impl;
		s_sink = *( pcvoid const* )&lm_lc;
		s_sink = *( pcvoid const* )&lm_mc;
		s_sink = *( pcvoid const* )&lm_conn;
		s_sink = *( pcvoid const* )&lm_disc;
		s_sink = *( pcvoid const* )&lm_qli;
		s_sink = *( pcvoid const* )&lm_qad;
		s_sink = *( pcvoid const* )&lm_stll;
		s_sink = *( pcvoid const* )&lm_rss;
		s_sink = *( pcvoid const* )&lm_rssi;
		s_sink = *( pcvoid const* )&lm_rfss;
		s_sink = *( pcvoid const* )&lm_rfssi;
	}
}
