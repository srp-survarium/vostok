// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "network_client.h"

#include <vostok/network_core/packet_reader.h>		// packet.r<u32>() / eof() in the player-action loop

// network_client holds player_ptr / boost::array<player_desc> members whose
// resource_ptr<player> dtors instantiate here, needing the complete player type
#include "player.h"

namespace survarium {

// claude@NOTE: switch on the leading server-message-type byte (compiled as a
// jump table over message_type - 0x81), each case forwarding to a sibling
// process_*/on_* handler. The match_server_message_types_enum is empty in the PDB
// (param/cast-only -> no enumerators recovered), so the case labels are the raw
// recovered byte values rather than named constants. process_team_bases inlines to
// m_game.get_game_world().game_ui.initialize_base_points(packet) at its case.
void network_client::on_match_packet_received( const u8 message_type, network_core::packet_reader& packet )
{
	switch ( message_type )
	{
		case 0x81:	process_match_info					( packet );	break;
		case 0x92:	process_player_profile				( packet );	break;
		case 0x93:	process_team_bases					( packet );	break;
		case 0x82:
		{
			const u32 time_in_ms = packet.r< u32 >( );

			do
				process_player_action( packet, time_in_ms );
			while ( !packet.eof( ) );
			break;
		}
		case 0x83:	process_player_kill					( packet );	break;
		case 0x84:	process_player_respawn				( packet );	break;
		case 0x94:	process_initialize_victory_items	( packet );	break;
		case 0x85:	process_base_capture_progress		( packet );	break;
		case 0x86:	process_match_time					( packet );	break;
		case 0x87:	process_respawn_timer				( packet );	break;
		case 0x9b:	process_match_wait_timer			( packet );	break;
		case 0x9a:	process_game_status					( packet );	break;
		case 0x88:	process_player_kd_stats				( packet );	break;
		case 0x95:	process_victory_item_take_or_put	( packet );	break;
		case 0x89:	process_player_hit					( packet );	break;
		case 0x8a:	process_affect_damage_model			( packet );	break;
		case 0x8b:	process_sync_response				( packet );	break;
		case 0x8c:	process_match_finished				( packet );	break;
		case 0x91:	player_visibility_change			( packet );	break;
		case 0x96:	on_trap_placed						( packet );	break;
		case 0x97:	on_trap_removed						( packet );	break;
		case 0x98:	on_trap_fired						( packet );	break;
		case 0x99:	on_trap_disarmed					( packet );	break;
		case 0x9c:	game_world_object_state_arrived		( packet );	break;
		case 0x9d:	on_world_sync_request				( );		break;
		case 0x9e:	damage_model_state_arrived			( packet );	break;
		default:	NODEFAULT( );
	}
}

} // namespace survarium
