// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLASH_FUNCTION_HANDLER_PARAMS_H_INCLUDED
#define FLASH_FUNCTION_HANDLER_PARAMS_H_INCLUDED

namespace survarium {

struct flash_movie;
struct flash_value;

// mirrors Scaleform::GFx::FunctionHandler::Params (hence the SDK field naming)
struct flash_function_handler_params {

public:
	/* 0x0000 */	flash_value*	pRetVal;
	/* 0x0004 */	flash_value*	pArgs;
	/* 0x0008 */	flash_movie*	pMovie;
	/* 0x000c */	u32				ArgCount;
}; // struct flash_function_handler_params

STATIC_SIZE_ASSERT(flash_function_handler_params, 0x10);

} // namespace survarium

#endif // #ifndef FLASH_FUNCTION_HANDLER_PARAMS_H_INCLUDED
