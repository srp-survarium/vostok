////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOGIN_MENU_EXTERNAL_HANDLER_H_INCLUDED
#define LOGIN_MENU_EXTERNAL_HANDLER_H_INCLUDED

/* INCLUDES */
class survarium::game;
class survarium::login_menu;
struct survarium::flash_external_handler;

/* FORWARD REFS */
class survarium::flash_movie;
class survarium::flash_value;

namespace survarium {

class login_menu_external_handler : public flash_external_handler , public boost::noncopyable {
public:
	inline			login_menu_external_handler	( game& arg_0, login_menu& arg_1 ) { /* no source */ }

	virtual	void	callback					(
						flash_movie*			pmovieView,
						pcstr					methodName,
						flash_value const*		args,
						u32						argCount
					) override;

	virtual			~login_menu_external_handler( ) { /* no source */ }

private:
	/* 0x0000 */	/* flash_external_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&			m_game;
	/* 0x000c */	login_menu&		m_login_menu;
}; // class login_menu_external_handler

STATIC_SIZE_ASSERT(login_menu_external_handler, 0x10);

} // namespace survarium

#endif // #ifndef LOGIN_MENU_EXTERNAL_HANDLER_H_INCLUDED
