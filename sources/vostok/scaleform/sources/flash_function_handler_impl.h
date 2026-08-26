////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_FUNCTION_HANDLER_IMPL_H_INCLUDED
#define FLASH_FUNCTION_HANDLER_IMPL_H_INCLUDED

#include "GFx/GFx_Player.h"		// Scaleform::GFx::FunctionHandler (base)

namespace survarium {

struct flash_function_handler;

struct flash_function_handler_impl : public Scaleform::GFx::FunctionHandler , private boost::noncopyable {
			explicit	flash_function_handler_impl	( flash_function_handler& arg_0 );

	virtual	void		Call						( Scaleform::GFx::FunctionHandler::Params const& arg_0 ) override;


public:
	/* 0x0000 */	/* Scaleform::GFx::FunctionHandler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	flash_function_handler&		owner;
}; // struct flash_function_handler_impl

STATIC_SIZE_ASSERT(flash_function_handler_impl, 0xC);

} // namespace survarium

#endif // #ifndef FLASH_FUNCTION_HANDLER_IMPL_H_INCLUDED
