// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_ITEM_BOOL_H_INCLUDED
#define OPTIONS_ITEM_BOOL_H_INCLUDED

#include "options_item_base.h"

namespace survarium {

class options_item_bool : public options_item_base {
	typedef	options_item_base	super;

public:
						options_item_bool	( options_tab& parent_tab, pcstr console_command, u8 option_item_id );

	virtual	void	initialize			( ) override;

	virtual	void	fill_data			( flash_value& __formal ) override;
	virtual	void	fill_value			( flash_value& val ) override;

	virtual	void	apply				( ) override;

	virtual	void	revert				( ) override;

	virtual	void	call				( flash_function_handler_params& params ) override;

protected:
	/* 0x0000 */	/* options_item_base */
	/* 0x0018 */	bool	m_current_value;
	/* 0x0019 */	bool	m_source_value;

}; // class options_item_bool

STATIC_SIZE_ASSERT(options_item_bool, 0x1C);

} // namespace survarium

#endif // #ifndef OPTIONS_ITEM_BOOL_H_INCLUDED
