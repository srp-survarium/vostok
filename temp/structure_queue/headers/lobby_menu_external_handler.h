////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED
#define LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED

/* INCLUDES */
class survarium::game;
struct survarium::flash_external_handler;

/* FORWARD REFS */
class vostok::network::login_client;
class survarium::flash_movie;
class survarium::flash_value;
class survarium::lobby_client;
class survarium::messaging_client;

namespace survarium {

class lobby_menu_external_handler : public flash_external_handler , public boost::noncopyable {
public:
	inline	explicit					lobby_menu_external_handler	( game& arg_0 ) { /* no source */ }
	inline	lobby_client&				lobby_client				( ) { /* no source */ }
	inline	network::login_client&		login_client				( ) { /* no source */ }
	inline	messaging_client&			messaging_client			( ) { /* no source */ }

	virtual	void						callback					(
											flash_movie*			pmovieView,
											pcstr					methodName,
											flash_value const*		args,
											u32						argCount
										) override;

	virtual								~lobby_menu_external_handler( ) { /* no source */ }

private:
	/* 0x0000 */	/* flash_external_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;
}; // class lobby_menu_external_handler

STATIC_SIZE_ASSERT(lobby_menu_external_handler, 0xC);

} // namespace survarium

#endif // #ifndef LOBBY_MENU_EXTERNAL_HANDLER_H_INCLUDED
