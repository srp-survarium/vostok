////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_TEXT_MANAGER_H_INCLUDED
#define FLASH_TEXT_MANAGER_H_INCLUDED

// create_text/create_text_w return flash_text by value
#include "flash_text.h"

namespace Scaleform {
namespace GFx {
	class DrawTextManager;
	class Loader;
} // namespace GFx
} // namespace Scaleform

namespace survarium {

struct flash_text_manager {
	inline	explicit		flash_text_manager	( Scaleform::GFx::Loader* arg_0 ) { /* no source */ }
	inline					~flash_text_manager	( ) { /* no source */ }

	inline	void			set_viewport		( u32 arg_0, u32 arg_1 ) { /* no source */ }

	// real out-of-line bodies live in the scaleform glue (movie.cpp); declared
	// (not inlined) so game-side callers resolve to movie.obj (avoids LNK2005)
			flash_text		create_text			( pcstr arg_0 );
	inline	flash_text		create_text_w		( wchar_t* arg_0 ) { /* no source */ return flash_text( ); }

			void			destroy_text		( flash_text& arg_0 );	// out-of-line: real body in scaleform movie.cpp (avoids LNK2005 vs player.obj)

	inline	void			capture				( ) { /* no source */ }

	inline	void			tick				( ) { /* no source */ }

public:
	/* 0x0000 */	Scaleform::GFx::DrawTextManager*	text_manager_impl;
	/* 0x0004 */	bool								need_capture;
	/* 0x0008 */	u32									m_output_width;
	/* 0x000c */	u32									m_output_height;
}; // struct flash_text_manager

STATIC_SIZE_ASSERT(flash_text_manager, 0x10);

} // namespace survarium

#endif // #ifndef FLASH_TEXT_MANAGER_H_INCLUDED
