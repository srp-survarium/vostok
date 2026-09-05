// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_GRAPHICS_QUALITY_SELECTOR_H_INCLUDED
#define OPTIONS_GRAPHICS_QUALITY_SELECTOR_H_INCLUDED

#include "options_item_int.h"

namespace survarium {

class options_graphics_quality_selector : public options_item_int {
	typedef options_item_int super;

public:
			explicit	options_graphics_quality_selector	( options_tab& parent_tab );

	virtual	void		call								( flash_function_handler_params& params ) override;

	inline	u8			value								( ) { return m_current_value; }

}; // class options_graphics_quality_selector

STATIC_SIZE_ASSERT(options_graphics_quality_selector, 0x20);

} // namespace survarium

#endif // #ifndef OPTIONS_GRAPHICS_QUALITY_SELECTOR_H_INCLUDED
