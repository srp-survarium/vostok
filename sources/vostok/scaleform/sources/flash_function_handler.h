// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef FLASH_FUNCTION_HANDLER_H_INCLUDED
#define FLASH_FUNCTION_HANDLER_H_INCLUDED

namespace survarium {

struct flash_function_handler_impl;
struct flash_function_handler_params;

// PDB: dtor is NON-virtual (only `call` populates the vtable after the ctor)
struct flash_function_handler {
					flash_function_handler	( );
					~flash_function_handler	( );

	virtual	void	call					( flash_function_handler_params& arg_0 ) = 0;

public:
	/* 0x0004 */	flash_function_handler_impl*	impl;
}; // struct flash_function_handler

STATIC_SIZE_ASSERT(flash_function_handler, 0x8);

} // namespace survarium

#endif // #ifndef FLASH_FUNCTION_HANDLER_H_INCLUDED
