////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_client.h"

// network_client's player_ptr / player_desc members instantiate
// resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

#include "game.h"			// m_game.lobby_menu()
#include "lobby_menu.h"		// query_lobby_info / show_disconnected_message + m_is_connected_to_lobby

namespace survarium {

// claude@NOTE: large server->client lobby dispatch (152 stmts, 6 locals: host[64], type,
// faction_id, op_id x2, description[512]). Reads an op_id and switches into lobby_client read_*
// handlers (read_status_info/read_enumerate_profiles_info/read_price_items/...) + messaging_client
// and lobby_menu refresh calls - all deps are on the base. Parked (NOT cross-module capped) as a
// large focused follow-up: the switch case set + 6-local frame layout need careful byte-exact
// reconstruction. lobby_client/messaging_client are matched on base, so this is reproducible.
// STATE[STUB]
void network_client::on_lobby_packet_received( network_core::packet_reader& reader )
{
}

// claude@NOTE: residual is a menu reload - the target holds m_game.lobby_menu()
// (an inline *m_lobby_menu) in a callee-saved reg across query_lobby_info /
// show_disconnected_message; MSVC re-loads m_lobby_menu here. The target records 0
// named locals, so the faithful 0-local form is kept over a cached-reference form
// (which adds a phantom local AND scores lower).
void network_client::on_connected_to_lobby( )
{
	m_game.lobby_menu( ).query_lobby_info( );
	if ( !m_game.lobby_menu( ).m_is_connected_to_lobby )
	{
		m_game.lobby_menu( ).m_is_connected_to_lobby = true;
		m_game.lobby_menu( ).show_disconnected_message( false );
	}
}

void network_client::on_disconnected_from_lobby( )
{
	if ( m_game.lobby_menu( ).m_is_connected_to_lobby )
	{
		m_game.lobby_menu( ).m_is_connected_to_lobby = false;
		m_game.lobby_menu( ).show_disconnected_message( true );
	}
}

// claude@NOTE: recovered structure (all deps present, good follow-up target):
//   if ( packet.r<u8>() ) return;               // 284 - shop op code; non-zero returns
//   inventory_item_instance new_item;           // local
//   new_item.dict_id = packet.r<u16>();         // 286
//   new_item.id      = packet.r<u32>();         // 292 (ebx)
//   new_item.condition_or_stack = packet.r<u32>(); // 293 (ebp), amount_in_inventory=0
//   // 296..308: find id in lobby_client().inventory_item_instances() (vectora at +0x788);
//   //   if present: existing.condition_or_stack += new_item.condition_or_stack;
//   //   else: push_back(new_item)  (emits _M_insert_overflow on overflow)
//   lobby_menu().fill_inventory_contents();      // 310 (m_game.lobby_menu())
//   lobby_client().query_client_status( (lobby::query_info_types)7 ); // 311
// Parked NOT capped: the find-or-update-or-insert over the inventory vector with only the
// 2 target locals (packet, new_item) needs careful byte-exact iteration of the inline search
// + _M_insert_overflow; left as a focused follow-up to avoid a speculative low-fidelity body.
// STATE[STUB]
void network_client::process_shop_action( network_core::packet_reader& packet )
{
}

// claude@NOTE: recovered structure:
//   m_game_status = game_status_inactive;                 // 320
//   if ( match_client().is_connected() ) match_client().disconnect();  // 321/323
//   m_game.get_game_world().unload();                      // 325
//   if ( user_initiate ) lobby_client().<+0x86C> = true;   // 327
//   if ( !lobby_client().<+0x194> ) lobby_client().<+0xA0> = true;  // 330/331
//   if ( !m_game.<+0x35A> ) m_game.switch_to_lobby();      // 334/335
// PARKED on the game carcass: m_game.<+0x35A> (a game bool gating switch_to_lobby) is absent
// from the simplified game.h, and the lobby_client +0x86C/+0x194/+0xA0 members are private with
// no accessor in lobby_client.h. Completes once the game layout + those lobby_client members are
// available; the match_client()/get_game_world().unload() parts are already local.
// STATE[STUB]
void network_client::close_current_match( bool user_initiate )
{
}

void network_client::process_match_finished( network_core::packet_reader& __formal )
{
	close_current_match	( false );
}

} // namespace survarium
