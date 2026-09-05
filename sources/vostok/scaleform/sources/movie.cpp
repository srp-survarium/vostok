// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "flash_movie.h"
#include "flash_text.h"
#include "flash_text_manager.h"

#include "GFx.h"
#include "GFx/GFx_DrawText.h"

namespace survarium {

flash_text_manager::flash_text_manager( Scaleform::GFx::Loader* loader )
	:	need_capture	( false )
{
	text_manager_impl	= SF_NEW Scaleform::GFx::DrawTextManager( loader );
	text_manager_impl->SetFontProvider( loader->GetFontProvider( ) );

	Scaleform::GFx::DrawTextManager::TextParams	defParams	= text_manager_impl->GetDefaultTextParams( );
	defParams.TextColor	= Scaleform::Render::Color( 0, 0xff, 0, 0xff );
	defParams.FontName	= "Arial";
	defParams.FontSize	= 16.f;
	text_manager_impl->SetDefaultTextParams( defParams );
}

void flash_text_manager::tick( )
{
	if ( need_capture )
	{
		text_manager_impl->Capture( true );
		need_capture	= false;
	}
}

flash_text flash_text_manager::create_text( pcstr text )
{
	flash_text	result;
	Scaleform::Render::SizeF	extent	= text_manager_impl->GetTextExtent( text );
	extent.Width	+= 5.f;

	result.text_impl	= text_manager_impl->CreateText( text, Scaleform::Render::RectF( 0.f, 0.f, extent.Width, extent.Height ) );

	result.visible		= true;

	need_capture		= true; result.owner	= this;

	return result;
}

flash_text flash_text_manager::create_text_w( wchar_t* text )
{
	flash_text	result;
	Scaleform::Render::SizeF	extent	= text_manager_impl->GetTextExtent( text );
	extent.Width	+= 5.f;

	result.text_impl	= text_manager_impl->CreateText( text, Scaleform::Render::RectF( 0.f, 0.f, extent.Width, extent.Height ) );

	result.visible		= true;

	need_capture		= true; result.owner	= this;

	return result;
}

void flash_text_manager::set_viewport( u32 width, u32 height )
{
	u32	output_window_width		= width;
	u32	output_window_height	= height;

	m_output_width	= output_window_width;
	m_output_height	= output_window_height;

	Scaleform::GFx::Viewport	viewport( output_window_width, output_window_height, 0, 0, output_window_width, output_window_height );
	text_manager_impl->SetViewport( viewport );

	need_capture	= true;
}

void flash_text_manager::destroy_text( flash_text& text )
{
	text.text_impl->Release( );
	text.text_impl	= NULL;
	text.owner		= NULL;
	text.visible	= false;
	need_capture	= true;
}

float flash_text::get_width( )
{
	return text_impl->GetRect( ).Width( );
}

float flash_text::get_height( )
{
	return text_impl->GetRect( ).Height( );
}

void flash_text::set_visible( bool value )
{
	if ( visible != value )
	{
		visible	= value;
		text_impl->SetVisible( value );
		owner->need_capture	= true;
	}
}

void flash_text::set_font_size( const float font_size )
{
	text_impl->SetFontSize( font_size );

	Scaleform::GFx::DrawTextManager::TextParams	params	= owner->text_manager_impl->GetDefaultTextParams( );

	params.FontSize	= font_size;
	Scaleform::Render::SizeF	size	= owner->text_manager_impl->GetTextExtent( text_impl->GetText( ), 0.f, &params );
	size	+= 5.f;

	Scaleform::Render::RectF	rect	= text_impl->GetRect( );
	rect.x2	= size.Width + rect.x1;
	rect.y2	= size.Height + rect.y1;
	text_impl->SetRect( rect );
	owner->need_capture	= true;
}

void flash_text::set_position( const float screen_position_x, const float screen_position_y )
{
	Scaleform::Render::RectF	rect	= text_impl->GetRect( );
	rect	= Scaleform::Render::RectF( screen_position_x, screen_position_y, screen_position_x + rect.Width( ), screen_position_y + rect.Height( ) );
	text_impl->SetRect( rect );
	owner->need_capture	= true;
}

void flash_text::set_color( u8 r, u8 g, u8 b, u8 a )
{
	text_impl->SetColor( Scaleform::Render::Color( r, g, b, a ) );
	owner->need_capture	= true;
}

void flash_text::set_text( pcstr text )
{
	text_impl->SetText( text );
	Scaleform::Render::SizeF	size	= owner->text_manager_impl->GetTextExtent( text );
	size	+= 5.f;

	Scaleform::Render::RectF	rect	= text_impl->GetRect( );
	rect.x2	= size.Width + rect.x1;
	rect.y2	= size.Height + rect.y1;
	text_impl->SetRect( rect );
	owner->need_capture	= true;
}

void flash_movie::Advance( const float time_delta, const u32 frame_catch_up_count )
{
	VOSTOK_UNREFERENCED_PARAMETER	( frame_catch_up_count );
	m_movie->Advance( time_delta, 0 );
}

void flash_movie::SetBackgroundAlpha( const float alpha )
{
	m_movie->SetBackgroundAlpha( alpha );
}

void flash_movie::SetViewAlignment( flash_movie::AlignType alignment )
{
	m_movie->SetViewAlignment( ( Scaleform::GFx::Movie::AlignType )alignment );
}

void flash_movie::SetViewScaleMode( flash_movie::ScaleModeType scale_mode )
{
	VOSTOK_UNREFERENCED_PARAMETER	( scale_mode );
	m_movie->SetViewScaleMode( Scaleform::GFx::Movie::SM_NoScale );
}

void flash_movie::Restart( )
{
	m_movie->Restart( );
}

void flash_movie::SetViewport( u32 width, u32 height )
{
	u32	output_window_width		= width;
	u32	output_window_height	= height;

	m_output_width	= output_window_width;
	m_output_height	= output_window_height;

	Scaleform::GFx::Viewport	viewport( output_window_width, output_window_height, 0, 0, output_window_width, output_window_height );
	m_movie->SetViewport( viewport );
}

void flash_movie::HandleMouseMove( const float x, const float y, const float scroll_delta )
{
	Scaleform::GFx::MouseEvent	mevent( Scaleform::GFx::Event::MouseMove, 0, x, y );
	m_movie->HandleEvent( mevent );

	mevent	= Scaleform::GFx::MouseEvent( Scaleform::GFx::Event::MouseWheel, 0, x, y, scroll_delta );
	m_movie->HandleEvent( mevent );
}

void flash_movie::HandleMouseBtn(
		flash_movie::mouse_btn_action	action,
		u32								button,
		const float						x,
		const float						y
	)
{
	Scaleform::GFx::Event::EventType	type	= Scaleform::GFx::Event::Unknown;
	if ( action == ms_key_down )
		type	= Scaleform::GFx::Event::MouseDown;
	else if ( action == ms_key_up )
		type	= Scaleform::GFx::Event::MouseUp;

	Scaleform::GFx::MouseEvent	mevent( type, button, x, y );
	m_movie->HandleEvent( mevent );
}

void flash_movie::HandleKeyboard( flash_movie::keyb_btn_action action, s32 scan )
{
	Scaleform::GFx::KeyEvent	ev(
		action == kb_key_up ? Scaleform::GFx::Event::KeyDown : Scaleform::GFx::Event::KeyUp,
		( Scaleform::Key::Code )scan
	);
	m_movie->HandleEvent( ev );
}

void flash_movie::HandleChar( wchar_t c )
{
	Scaleform::GFx::CharEvent	ev( c );
	m_movie->HandleEvent( ev );
}

void flash_movie::ForceCollectGarbage( )
{
	m_movie->ForceCollectGarbage( );
}

inline void flash_text::set_alignment( flash_text::alignment alignment )
{
	text_impl->SetAlignment( static_cast<Scaleform::GFx::DrawText::Alignment>( alignment ) );
	owner->need_capture = true;
}

} // namespace survarium
