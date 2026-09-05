// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_ITEM_BASE_H_INCLUDED
#define OPTIONS_ITEM_BASE_H_INCLUDED

#include <vostok/scaleform/sources/flash_function_handler.h>
#include "option_item_type_enum.h"

namespace vostok {
namespace console_commands {
	class console_command;
} // namespace console_commands
} // namespace vostok

namespace survarium {

class options_tab;
struct flash_function_handler_params;
struct flash_value;

class options_item_base : public flash_function_handler , private boost::noncopyable {
public:
										options_item_base	(
											options_tab&				parent_tab,
											pcstr						console_command,
											u8							option_item_id,
											option_item_type_enum		type
										);

	virtual	void						initialize			( ) = 0;

	virtual	void						fill_data			( flash_value& arg_0 ) = 0;
	virtual	void						fill_value			( flash_value& arg_0 ) = 0;

	virtual	void						apply				( ) = 0;

	virtual	void						revert				( );

	virtual	void						call				( flash_function_handler_params& arg_0 ) override = 0;

	inline	option_item_type_enum		option_type			( ) { return m_type; }


protected:
	/* 0x0000 */	/* flash_function_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	console_commands::console_command*		m_console_command;
	/* 0x000c */	option_item_type_enum					m_type;
	/* 0x0010 */	options_tab&							m_parent_tab;
	/* 0x0014 */	u8										m_option_item_id;
}; // class options_item_base

STATIC_SIZE_ASSERT(options_item_base, 0x18);

} // namespace survarium

#endif // #ifndef OPTIONS_ITEM_BASE_H_INCLUDED
