// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERVER_CONNECTION_INFO_H_INCLUDED
#define SERVER_CONNECTION_INFO_H_INCLUDED

namespace vostok {

// vostok-namespace shared struct whose only PDB-visible consumers are game's
// network clients (lobby/messaging/network_client) - parked under game with
// them, the lobby_enums.h/messaging_enums.h pattern
struct server_connection_info {

public:
	/* 0x0000 */	u32		session_id;
	/* 0x0004 */	u16		port;
	/* 0x0006 */	char	host[64];
	/* 0x0046 */	char	password[48];
	/* 0x0078 */	u32		connection_error_count;
	/* 0x007c */	bool	need_resolve;
}; // struct server_connection_info

STATIC_SIZE_ASSERT(server_connection_info, 0x80);

} // namespace vostok

#endif // #ifndef SERVER_CONNECTION_INFO_H_INCLUDED
