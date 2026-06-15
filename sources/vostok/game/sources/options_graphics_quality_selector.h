////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_GRAPHICS_QUALITY_SELECTOR_H_INCLUDED
#define OPTIONS_GRAPHICS_QUALITY_SELECTOR_H_INCLUDED

#include "options_item_int.h"

namespace survarium {

class options_graphics_quality_selector : public options_item_int {
public:
			explicit	options_graphics_quality_selector	( options_tab& parent_tab );

	virtual	void		call								( flash_function_handler_params& params ) override;

	// buildability return (the base's m_current_value is private)
	inline	u8			value								( ) { /* no source */ return 0; }

	inline				~options_graphics_quality_selector	( ) { /* no source */ }
}; // class options_graphics_quality_selector

STATIC_SIZE_ASSERT(options_graphics_quality_selector, 0x20);

} // namespace survarium

#endif // #ifndef OPTIONS_GRAPHICS_QUALITY_SELECTOR_H_INCLUDED
