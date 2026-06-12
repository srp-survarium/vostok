////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_GAMMA_SELECTOR_H_INCLUDED
#define OPTIONS_GAMMA_SELECTOR_H_INCLUDED

#include "options_item_float.h"

namespace survarium {

class options_gamma_selector : public options_item_float {
public:
	// buildability base init (options_item_float has only the 4-arg ctor);
	// a matcher supplies the real console-command/id/step arguments
	inline	explicit	options_gamma_selector	( options_tab& arg_0 )
		: options_item_float( arg_0, NULL, 0, 0.0f ) { /* no source */ }

	virtual	void		call					( flash_function_handler_params& params ) override;

	virtual	void		revert					( ) override;

	inline				~options_gamma_selector	( ) { /* no source */ }
}; // class options_gamma_selector

STATIC_SIZE_ASSERT(options_gamma_selector, 0x24);

} // namespace survarium

#endif // #ifndef OPTIONS_GAMMA_SELECTOR_H_INCLUDED
