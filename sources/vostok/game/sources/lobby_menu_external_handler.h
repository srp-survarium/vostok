// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED
#define LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED

#include <vostok/scaleform/sources/flash_external_handler.h>
#include "game.h"
#include "base_network_client.h"

namespace vostok {
namespace network {
	class login_client;
} // namespace network
} // namespace vostok

namespace survarium {

class game;
class lobby_client;
class messaging_client;
struct flash_movie;
struct flash_value;

class lobby_menu_external_handler : public flash_external_handler , private boost::noncopyable {
private:
	/* 0x0000 */	/* flash_external_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;

public:
	inline	explicit					lobby_menu_external_handler	( game& arg_0 ) : m_game( arg_0 ) { }
	// sushi@TODO: Retail callback uses this client path; verify the original named getter boundary.
	inline	lobby_client&				lobby_client				( ) { return m_game.get_network_client( )->lobby_client( ); }
	// sushi@TODO: Sibling-owner forwarding model; bind a consumer selecting this handler's login getter.
	inline	network::login_client&		login_client				( ) { return m_game.get_network_client( )->login_client( ); }
	// sushi@TODO: Retail callback uses this client path; verify the original named getter boundary.
	inline	messaging_client&			messaging_client			( ) { return m_game.get_network_client( )->messaging_client( ); }

	virtual	void						callback					(
											flash_movie*			pmovieView,
											pcstr					methodName,
											flash_value const*		args,
											u32						argCount
										) override;

}; // class lobby_menu_external_handler

STATIC_SIZE_ASSERT(lobby_menu_external_handler, 0xC);

} // namespace survarium

#endif // #ifndef LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED
