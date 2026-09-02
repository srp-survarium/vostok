// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLIENT_PLAYER_HISTORY_ITEM_H_INCLUDED
#define CLIENT_PLAYER_HISTORY_ITEM_H_INCLUDED

#include <vostok/game_core/server_player_update.h>

namespace survarium {

struct client_player_history_item : private boost::noncopyable {
		client_player_history_item	( );
		~client_player_history_item	( );

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	server_player_update	action;
	/* 0x005c */	u32						time_in_ms;
}; // struct client_player_history_item

STATIC_SIZE_ASSERT(client_player_history_item, 0x60);

} // namespace survarium

#endif // #ifndef CLIENT_PLAYER_HISTORY_ITEM_H_INCLUDED
