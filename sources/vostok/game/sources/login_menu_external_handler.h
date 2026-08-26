////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOGIN_MENU_EXTERNAL_HANDLER_H_INCLUDED
#define LOGIN_MENU_EXTERNAL_HANDLER_H_INCLUDED

#include <vostok/scaleform/sources/flash_external_handler.h>

namespace survarium {

class game;
class login_menu;
struct flash_movie;
struct flash_value;

class login_menu_external_handler : public flash_external_handler , private boost::noncopyable {
private:
	/* 0x0000 */	/* flash_external_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&			m_game;
	/* 0x000c */	login_menu&		m_login_menu;

public:
	inline			login_menu_external_handler	( game& arg_0, login_menu& arg_1 )
		: m_game( arg_0 ), m_login_menu( arg_1 ) { /* no source */ }

	virtual	void	callback					(
						flash_movie*			pmovieView,
						pcstr					methodName,
						flash_value const*		args,
						u32						argCount
					) override;

}; // class login_menu_external_handler

STATIC_SIZE_ASSERT(login_menu_external_handler, 0x10);

} // namespace survarium

#endif // #ifndef LOGIN_MENU_EXTERNAL_HANDLER_H_INCLUDED
