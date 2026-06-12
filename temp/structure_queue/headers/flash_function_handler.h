////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_FUNCTION_HANDLER_H_INCLUDED
#define FLASH_FUNCTION_HANDLER_H_INCLUDED

/* INCLUDES */
struct survarium::flash_function_handler_impl;

/* FORWARD REFS */
class survarium::flash_function_handler_params;

namespace survarium {

struct flash_function_handler {
	inline			flash_function_handler	( ) { /* no source */ }
	inline			~flash_function_handler	( ) { /* no source */ }

	virtual	void	call					( flash_function_handler_params& arg_0 ) = 0;

public:
	/* 0x0004 */	flash_function_handler_impl*	impl;
}; // struct flash_function_handler

STATIC_SIZE_ASSERT(flash_function_handler, 0x8);

} // namespace survarium

#endif // #ifndef FLASH_FUNCTION_HANDLER_H_INCLUDED
