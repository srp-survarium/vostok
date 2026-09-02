// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_GAMMA_SELECTOR_H_INCLUDED
#define OPTIONS_GAMMA_SELECTOR_H_INCLUDED

#include "options_item_float.h"

namespace survarium {

class options_gamma_selector : public options_item_float {
	typedef options_item_float super;

public:
	inline	explicit	options_gamma_selector	( options_tab& arg_0 )
		: options_item_float( arg_0, "r_gamma_correction_factor", 6, 0.01f ) { /* no source */ }

	virtual	void		call					( flash_function_handler_params& params ) override;

	virtual	void		revert					( ) override;

}; // class options_gamma_selector

STATIC_SIZE_ASSERT(options_gamma_selector, 0x24);

} // namespace survarium

#endif // #ifndef OPTIONS_GAMMA_SELECTOR_H_INCLUDED
