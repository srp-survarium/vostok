////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "flash_movie.h"
#include "flash_text.h"
#include "flash_text_manager.h"

#include "GFx.h"
#include "GFx/GFx_DrawText.h"

namespace survarium {

// STATE[STUB]
void flash_movie::ForceCollectGarbage( )
{
	// FUNCTION BODY[0x5bb110]
}

// STATE[STUB]
void flash_movie::Restart( )
{
	// FUNCTION BODY[0x5bb120]
}

// STATE[STUB]
void flash_movie::SetViewScaleMode( flash_movie::ScaleModeType scale_mode )
{
	// FUNCTION BODY[0x5bb130]
	VOSTOK_UNREFERENCED_PARAMETER	( scale_mode );
}

// STATE[STUB]
void flash_movie::SetViewAlignment( flash_movie::AlignType alignment )
{
	// FUNCTION BODY[0x5bb140]
	VOSTOK_UNREFERENCED_PARAMETER	( alignment );
}

// STATE[STUB]
void flash_movie::SetBackgroundAlpha( const float alpha )
{
	// FUNCTION BODY[0x5bb150]
	VOSTOK_UNREFERENCED_PARAMETER	( alpha );
}

// STATE[STUB]
void flash_movie::Advance( const float time_delta, const u32 frame_catch_up_count )
{
	// FUNCTION BODY[0x5bb170]
	VOSTOK_UNREFERENCED_PARAMETERS	( time_delta, frame_catch_up_count );
}

// STATE[STUB]
float flash_text::get_height( )
{
	// FUNCTION BODY[0x5bb190]
	return 0.f;
}

// STATE[STUB]
float flash_text::get_width( )
{
	// FUNCTION BODY[0x5bb1c0]
	return 0.f;
}

// STATE[STUB]
void flash_text_manager::tick( )
{
	// FUNCTION BODY[0x5bb1f0]
}

// STATE[STUB]
void flash_movie::HandleChar( wchar_t character )
{
	// FUNCTION BODY[0x5bb210]
	VOSTOK_UNREFERENCED_PARAMETER	( character );
}

// STATE[STUB]
void flash_movie::HandleKeyboard( flash_movie::keyb_btn_action action, s32 key )
{
	// FUNCTION BODY[0x5bb240]
	VOSTOK_UNREFERENCED_PARAMETERS	( action, key );
}

// STATE[STUB]
void flash_movie::SetViewport( u32 width, u32 height )
{
	// FUNCTION BODY[0x5bb290]
	VOSTOK_UNREFERENCED_PARAMETERS	( width, height );
}

// STATE[STUB]
void flash_text::set_text( pcstr text )
{
	// FUNCTION BODY[0x5bb2f0]
	VOSTOK_UNREFERENCED_PARAMETER	( text );
}

// STATE[STUB]
void flash_text::set_position( const float x, const float y )
{
	// FUNCTION BODY[0x5bb390]
	VOSTOK_UNREFERENCED_PARAMETERS	( x, y );
}

// STATE[STUB]
void flash_text::set_visible( bool visible )
{
	// FUNCTION BODY[0x5bb400]
	VOSTOK_UNREFERENCED_PARAMETER	( visible );
}

// STATE[STUB]
void flash_text_manager::destroy_text( flash_text& text )
{
	// FUNCTION BODY[0x5bb420]
	VOSTOK_UNREFERENCED_PARAMETER	( text );
}

// STATE[STUB]
void flash_text_manager::set_viewport( u32 width, u32 height )
{
	// FUNCTION BODY[0x5bb440]
	VOSTOK_UNREFERENCED_PARAMETERS	( width, height );
}

// STATE[STUB]
flash_text flash_text_manager::create_text_w( wchar_t* text )
{
	// FUNCTION BODY[0x5bb4a0]
	VOSTOK_UNREFERENCED_PARAMETER	( text );
	return flash_text( );
}

// STATE[STUB]
flash_text flash_text_manager::create_text( pcstr text )
{
	// FUNCTION BODY[0x5bb530]
	VOSTOK_UNREFERENCED_PARAMETER	( text );
	return flash_text( );
}

// STATE[STUB]
void flash_movie::HandleMouseBtn(
		flash_movie::mouse_btn_action	action,
		u32								button,
		const float						x,
		const float						y
	)
{
	// FUNCTION BODY[0x5bb5c0]
	VOSTOK_UNREFERENCED_PARAMETERS	( action, button, x, y );
}

// STATE[STUB]
void flash_movie::HandleMouseMove( const float x, const float y, const float scroll_delta )
{
	// FUNCTION BODY[0x5bb630]
	VOSTOK_UNREFERENCED_PARAMETERS	( x, y, scroll_delta );
}

// STATE[STUB]
void flash_text::set_color( u8 r, u8 g, u8 b, u8 a )
{
	// FUNCTION BODY[0x5bb700]
	VOSTOK_UNREFERENCED_PARAMETERS	( r, g, b, a );
}

// STATE[STUB]
flash_text_manager::flash_text_manager( Scaleform::GFx::Loader* loader )
{
	// FUNCTION BODY[0x5bb730]
	VOSTOK_UNREFERENCED_PARAMETER	( loader );
}

// STATE[STUB]
void flash_text::set_font_size( const float font_size )
{
	// FUNCTION BODY[0x5bb810]
	VOSTOK_UNREFERENCED_PARAMETER	( font_size );
}

} // namespace survarium
