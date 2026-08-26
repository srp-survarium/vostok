////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED
#define LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED

#include <vostok/scaleform/sources/flash_external_handler.h>

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
	inline	explicit					lobby_menu_external_handler	( game& arg_0 ) : m_game( arg_0 ) { /* no source */ }
	// buildability returns; the real bodies reach the clients through m_game
	inline	lobby_client&				lobby_client				( ) { /* no source */ return *( survarium::lobby_client* )NULL; }
	inline	network::login_client&		login_client				( ) { /* no source */ return *( network::login_client* )NULL; }
	inline	messaging_client&			messaging_client			( ) { /* no source */ return *( survarium::messaging_client* )NULL; }

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
