// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAIN_MENU_EXTERNAL_HANDLER_H_INCLUDED
#define MAIN_MENU_EXTERNAL_HANDLER_H_INCLUDED

#include <vostok/scaleform/sources/flash_external_handler.h>

namespace survarium {

class game;
struct flash_movie;
struct flash_value;

class main_menu_external_handler : public flash_external_handler , private boost::noncopyable {
private:
	/* 0x0000 */	/* flash_external_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;

public:
	// sushi@TODO: Game-reference binding model; recover this handler's original construction/registration consumer.
	inline	explicit	main_menu_external_handler	( game& arg_0 ) : m_game( arg_0 ) { }

	// sushi@TODO: Recover the UI method/argument dispatch; sibling menu callbacks do not establish this contract.
	virtual	void		callback					(
							flash_movie*			arg_0,
							pcstr					arg_1,
							flash_value const*		arg_2,
							u32						arg_3
						) override { /* no source */ }

}; // class main_menu_external_handler

STATIC_SIZE_ASSERT(main_menu_external_handler, 0xC);

} // namespace survarium

#endif // #ifndef MAIN_MENU_EXTERNAL_HANDLER_H_INCLUDED
