////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_client.h"

// network_client holds player_ptr / boost::array<player_desc> members whose
// resource_ptr<player> dtors instantiate here, needing the complete player type
#include "player.h"

namespace survarium {

// claude@NOTE: switch(message_type - 0x81) over match_server_message_types_enum, each case
// calling a sibling process_* handler (process_match_info / process_player_profile /
// process_team_bases / process_player_action[loop] / process_player_kill / ... /
// damage_model_state_arrived), default = none. The handlers are declared and callable, BUT
// PARKED on the game_core carcass: enum match_server_message_types_enum is EMPTY in
// network_messages.h (the PDB stored no enumerators, and no enum dump exists), so the case
// labels cannot be spelled with their real constants. Completes once those enum values are
// recovered into game_core. (Case 0x40 inlines process_team_bases ->
// m_game.get_game_world().game_ui().initialize_base_points(packet).)
// STATE[STUB]
void network_client::on_match_packet_received( const u8 message_type, network_core::packet_reader& packet )
{
}

} // namespace survarium
