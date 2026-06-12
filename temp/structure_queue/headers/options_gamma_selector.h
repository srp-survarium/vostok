////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_GAMMA_SELECTOR_H_INCLUDED
#define OPTIONS_GAMMA_SELECTOR_H_INCLUDED

/* INCLUDES */
class survarium::options_item_float;

/* FORWARD REFS */
class survarium::flash_function_handler_params;
class survarium::options_tab;

namespace survarium {

class options_gamma_selector : public options_item_float {
public:
	inline	explicit	options_gamma_selector	( options_tab& arg_0 ) { /* no source */ }

	virtual	void		call					( flash_function_handler_params& params ) override;

	virtual	void		revert					( ) override;

	inline				~options_gamma_selector	( ) { /* no source */ }
}; // class options_gamma_selector

STATIC_SIZE_ASSERT(options_gamma_selector, 0x24);

} // namespace survarium

#endif // #ifndef OPTIONS_GAMMA_SELECTOR_H_INCLUDED
