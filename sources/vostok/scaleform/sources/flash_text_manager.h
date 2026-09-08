// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLASH_TEXT_MANAGER_H_INCLUDED
#define FLASH_TEXT_MANAGER_H_INCLUDED
#include <vostok/scaleform/api.h>

// create_text/create_text_w return flash_text by value
#include "flash_text.h"

namespace Scaleform {
namespace GFx {
	class DrawTextManager;
	class Loader;
} // namespace GFx
} // namespace Scaleform

namespace survarium {

struct VOSTOK_SCALEFORM_API flash_text_manager {
			explicit		flash_text_manager	( Scaleform::GFx::Loader* arg_0 );
	inline					~flash_text_manager	( ) { }

			void			set_viewport		( u32 arg_0, u32 arg_1 );

			flash_text		create_text			( pcstr arg_0 );
			flash_text		create_text_w		( wchar_t* arg_0 );

			void			destroy_text		( flash_text& arg_0 );

	inline	void			capture				( ) { }

			void			tick				( );

public:
	/* 0x0000 */	Scaleform::GFx::DrawTextManager*	text_manager_impl;
	/* 0x0004 */	bool								need_capture;
	/* 0x0008 */	u32									m_output_width;
	/* 0x000c */	u32									m_output_height;
}; // struct flash_text_manager

STATIC_SIZE_ASSERT(flash_text_manager, 0x10);

} // namespace survarium

#endif // #ifndef FLASH_TEXT_MANAGER_H_INCLUDED
