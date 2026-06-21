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

// claude@NOTE: this TU now builds /Ox (Master Gold Optimization=3). The flash_text
// glue SOURCE STRUCTURE is faithful (set_position records the same 3 locals
// [screen_position_x/y, rect]; set_text/create_text pair SetText/GetTextExtent/
// `size += 5.f`/GetRect/SetRect in order). The residual is a genuine /Ox line-table
// difference, NOT /Od and NOT source-steerable: the target emits the inline RectF
// temp construction and the SetRect(temp) call as TWO separate statements (the
// TRGT_ONLY rows), while our /Ox folds the const-float copies and the RectF-arg
// build into the SetRect statement. The target keeps NO extra RectF local (only
// the 3), so we cannot add a named temp to force the split without a LOCALS diff.
// get_width/get_height/set_font_size/create_text_w stay UNPAIRED on reachability,
// not matching: game's own flash_text.h / flash_text_manager.h inline-stub these
// four, so the scaleform out-of-line bodies are /OPT:REF-stripped (no caller).
// Pairing them needs a scaleform-module anchor chain (none exists) - parked,
// cross-module reachability work.
float flash_text::get_height( )
{
	return text_impl->GetRect( ).Height( );
}

float flash_text::get_width( )
{
	return text_impl->GetRect( ).Width( );
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

void flash_text::set_text( pcstr text )
{
	text_impl->SetText( text );
	Scaleform::Render::SizeF	size	= owner->text_manager_impl->GetTextExtent( text );
	size	+= 5.f;

	Scaleform::Render::RectF	rect	= text_impl->GetRect( );
	text_impl->SetRect( Scaleform::Render::RectF(
		rect.x1,
		rect.y1,
		rect.x1 + size.Width,
		rect.y1 + size.Height
	) );
	owner->need_capture	= true;
}

void flash_text::set_position( const float x, const float y )
{
	Scaleform::Render::RectF	rect	= text_impl->GetRect( );

	const float	screen_position_x	= x;
	const float	screen_position_y	= y;

	text_impl->SetRect( Scaleform::Render::RectF(
		screen_position_x,
		screen_position_y,
		screen_position_x + rect.Width( ),
		screen_position_y + rect.Height( )
	) );

	owner->need_capture	= true;
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

flash_text flash_text_manager::create_text_w( wchar_t* text )
{
	flash_text	result;
	Scaleform::Render::SizeF	extent	= text_manager_impl->GetTextExtent( text );
	extent.Width	+= 5.f;

	result.text_impl	= text_manager_impl->CreateText( text, Scaleform::Render::RectF( 0.f, 0.f, extent.Width, extent.Height ) );

	result.visible		= true;

	need_capture		= true;
	result.owner		= this;

	return result;
}

flash_text flash_text_manager::create_text( pcstr text )
{
	flash_text	result;
	Scaleform::Render::SizeF	extent	= text_manager_impl->GetTextExtent( text );
	extent.Width	+= 5.f;

	result.text_impl	= text_manager_impl->CreateText( text, Scaleform::Render::RectF( 0.f, 0.f, extent.Width, extent.Height ) );

	result.visible		= true;

	need_capture		= true;
	result.owner		= this;

	return result;
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

void flash_text::set_font_size( const float font_size )
{
	text_impl->SetFontSize( font_size );

	Scaleform::GFx::DrawTextManager::TextParams	params	= owner->text_manager_impl->GetDefaultTextParams( );

	params.FontSize	= font_size;
	Scaleform::Render::SizeF	size	= owner->text_manager_impl->GetTextExtent( text_impl->GetText( ), 0.f, &params );
	size	+= 5.f;

	Scaleform::Render::RectF	rect	= text_impl->GetRect( );
	text_impl->SetRect( Scaleform::Render::RectF(
		rect.x1,
		rect.y1,
		rect.x1 + size.Width,
		rect.y1 + size.Height
	) );
	owner->need_capture	= true;
}

} // namespace survarium
