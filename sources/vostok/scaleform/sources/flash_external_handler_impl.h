// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef FLASH_EXTERNAL_HANDLER_IMPL_H_INCLUDED
#define FLASH_EXTERNAL_HANDLER_IMPL_H_INCLUDED

#include "GFx/GFx_Player.h"		// Scaleform::GFx::ExternalInterface (base)

namespace survarium {

struct flash_external_handler;

struct flash_external_handler_impl : public Scaleform::GFx::ExternalInterface , private boost::noncopyable {
	inline	explicit	flash_external_handler_impl	( flash_external_handler& arg_0 )
		:	owner	( arg_0 )
	{ }

	virtual	void		Callback					(
							Scaleform::GFx::Movie*			arg_0,
							pcstr							arg_1,
							Scaleform::GFx::Value const*	arg_2,
							u32								arg_3
						) override;


public:
	/* 0x0000 */	/* Scaleform::GFx::ExternalInterface */
	/* 0x000c */	/* boost::noncopyable */
	/* 0x000c */	flash_external_handler&		owner;
}; // struct flash_external_handler_impl

STATIC_SIZE_ASSERT(flash_external_handler_impl, 0x10);

} // namespace survarium

#endif // #ifndef FLASH_EXTERNAL_HANDLER_IMPL_H_INCLUDED
