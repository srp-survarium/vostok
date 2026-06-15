////////////////////////////////////////////////////////////////////////////
//	Created		: 24.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef MESSAGE_TYPES_H_INCLUDED
#define MESSAGE_TYPES_H_INCLUDED

#define VOSTOK_LOGIN_SERVER_USES_SSL					1

namespace vostok {

// client to server
// values pinned by the login-chain target immediates (sign_in_on_connected
// writes 1, sign_out_on_connected writes 2, sign_up_on_connected writes 0)
enum login_client_message_types_enum {
	sign_up_message_type							= 0,
	sign_in_message_type							= 1,
	sign_out_message_type							= 2,

	login_client_invalid_message_type				= 63,
}; // enum login_client_message_types_enum

// server to client
// values from the PDB type records
// (binaries/structure/target/headers/vostok/enums/login_server_message_types_enum.h)
enum login_server_message_types_enum {
	servers_connection_info_message_type			= 0x08,
	password_request_message_type					= 0x09,
	invalid_user_name_or_password_message_type		= 0x0a,
	valid_user_name_message_type					= 0x0b,
	sign_in_attempt_interval_violated_message_type	= 0x0c,
	sign_out_successful								= 0x0d,
	occupied_user_name_message_type					= 0x0e,
	send_sign_up_info_message_type					= 0x0f,
	sign_up_successful_message_type					= 0x10,
	user_banned_message_type						= 0x11,
	user_restricted_by_access_level_message_type	= 0x12,
	sign_in_user_already_signed_in					= 0x13,
	sign_in_invalid_version							= 0x14,

	login_server_invalid_message_type				= 0x1f,
}; // enum login_server_message_types_enum

// client to server
// values from the PDB type records (IDA extraction, PR #303 review; not in
// binaries/structure/target/headers/vostok/enums/ - see review_todos.md on the
// pdb_parser enum-reachability gap)
enum lobby_client_message_types_enum {
	set_status_ready_for_match						= 32,
	query_client_status								= 33,
	inventory_action								= 35,
	shop_action										= 36,
	skills_tree_action								= 37,
	lobby_client_sign_in_info						= 38,
	discard_playing_order							= 39,	// when disconnected from match (manually)
	ping_server										= 40,

	lobby_client_invalid_message_type				= 47,
}; // enum lobby_client_message_types_enum

// server to client
// values from the PDB type records
// (binaries/structure/target/headers/vostok/enums/lobby_server_message_types_enum.h)
enum lobby_server_message_types_enum {
	connection_successful							= 48,
	invalid_session_id								= 49,
	invalid_password								= 50,
	connect_to_match_server							= 51,
	operation_permitted								= 52,
	operation_denied								= 53,
	client_status									= 54,
	ping_server_answer								= 55,

	lobby_server_invalid_message_type				= 63,
}; // enum lobby_server_message_types_enum

// sushi@TODO: NOT a lobby_server message (PDB extraction above has no 128); it is
// the match server's first packet byte (match_client_impl::on_packet_received,
// target cmp 0x80) - the real enum is likely vostok::match_server_message_types_enum
// (type attested via survarium::network_packets_orderer's template args, enumerators
// not extracted); name is a guess, value pinned by the target immediate
enum {
	set_status_ready_for_battle						= 128,
};

enum socket_error_types_enum {
	no_socket_error									= 0,
	unable_to_write_to_socket						= 1,
	unable_to_read_from_socket						= 2,
	invalid_socket_error_type						= 255,
}; // enum socket_error_types_enum

enum resolve_error_types_enum {
	successfully_resolved							= 0,
	cannot_resolve									= 1,
	resolve_error_type								= 255,
}; // enum connection_error_tresolve_error_types_enumypes_enum

enum connection_error_types_enum {
	successfully_connected							= 0,
	cannot_connect									= 1,
	connection_error_type							= 255,
}; // enum connection_error_types_enum

enum handshaking_error_types_enum {
	successfully_handshaked							= 0,
	cannot_handshake								= 1,
	no_handshake 									= 2,
	handshaking_error_type							= 255,
}; // enum handshaking_error_types_enum

} // namespace vostok

#endif // #ifndef MESSAGE_TYPES_H_INCLUDED