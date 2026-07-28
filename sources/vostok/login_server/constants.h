////////////////////////////////////////////////////////////////////////////
//	Created		: 24.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_CONSTANTS_H_INCLUDED
#define ENGINE_CONSTANTS_H_INCLUDED

namespace vostok {

// claude@NOTE: the enum NAMES are a buildability device - the login-stack match
// made these enumerators PDB-reachable, and pdb_parser panics merging multiple
// same-scope ANONYMOUS enums of different lengths ("Enums cannot be of
// different length"); naming them is byte-neutral (enumerators are pure
// compile-time constants)
enum max_lengths_enum {
	max_account_name_length		= 32,
	max_password_length			= 48,
	max_email_length			= 100,	// PDB: sign_up_info::email[100] (0xB4 total)
	max_host_name_length		= 64,
};

enum network_ports_enum {
	login_tcp_port				= 25100,
	login_udp_port				= 25100,

	lobby_tcp_port				= 25101,

	chat_tcp_port				= 25102,

	match_tcp_port				= 25103,
	match_udp_port				= 25103,
};

enum login_retry_counts_enum {
	login_resolve_retry_count						= 6,
	login_connect_retry_count						= 6,
	login_handshake_retry_count						= 1,
	login_seconds_to_next_sign_in_attempt			= 1,
	login_seconds_to_next_sign_in_attempt_if_many	= 10,
};

} // namespace vostok

#endif // #ifndef ENGINE_CONSTANTS_H_INCLUDED