////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.06.2026
////////////////////////////////////////////////////////////////////////////

// Scaleform-module /OPT:REF reachability anchor. The survarium scaleform
// wrappers (flash_factory / flash_movie / flash_text(_manager) / flash_renderer /
// scaleform_render_command(_queue(_impl)) / vostok_scaleform_log /
// vostok_file_opener) are not all reached through the reconstructed game/render
// owners yet, so /OPT:REF would strip their matched out-of-line bodies.
// use_scaleform() references those remaining methods so the linker keeps them in
// the base EXE for the delinker to score.
//
// Self-guarded (s_run is never set), so the placeholder objects are never actually
// constructed/dereferenced; the compiler still emits every reference. Every scalar
// argument is laundered through a volatile sink so LTCG cannot prove this anchor is
// the sole caller with a constant argument and const-propagate it into the matched
// bodies.
//
// Retire each reference once its real owner reaches it.

#include "pch.h"

#include "flash_factory.h"
#include "flash_movie.h"
#include "flash_renderer.h"
#include "flash_text.h"
#include "flash_text_manager.h"
#include "flash_value.h"
#include "scaleform_engine.h"
#include "scaleform_game_engine.h"
#include "scaleform_render_command.h"
#include "scaleform_render_command_queue.h"
#include "scaleform_render_command_queue_impl.h"

#include "GFx.h"

namespace survarium {

void use_scaleform( )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	static scaleform_game_engine* volatile			s_engine		= 0;
	static Scaleform::GFx::Loader* volatile			s_loader		= 0;
	static flash_movie* volatile					s_movie			= 0;
	static flash_text_manager* volatile				s_text_manager	= 0;
	static scaleform_render_command_queue* volatile	s_queue			= 0;
	static Scaleform::Render::ThreadCommand* volatile	s_command	= 0;
	static Scaleform::Render::Interfaces* volatile	s_interfaces	= 0;
	static ID3D11Device* volatile					s_device		= 0;
	static ID3D11DeviceContext* volatile			s_context		= 0;
	static void* volatile							s_ptr			= 0;
	static pcstr volatile							s_str			= 0;
	static wchar_t* volatile						s_wstr			= 0;
	static u32 volatile								s_u32			= 0;
	static float volatile							s_f				= 0.f;

	scaleform_game_engine&	engine	= *s_engine;
	const u32				any_u32	= s_u32;
	const float				any_f	= s_f;

	// flash_factory
	flash_factory	factory( engine );
	factory.build_movie( s_ptr, any_u32, s_str );
	factory.destroy_movie( s_movie );
	factory.tick( );
	factory.create_text_manager( );

	// flash_movie out-of-line surface (value.cpp methods are reached by the game
	// anchors; movie.cpp methods need pinning here)
	flash_movie&	movie	= *s_movie;
	movie.SetViewport( any_u32, any_u32 );
	movie.Advance( any_f, any_u32 );
	movie.SetBackgroundAlpha( any_f );
	movie.SetViewAlignment( ( flash_movie::AlignType )any_u32 );
	movie.SetViewScaleMode( ( flash_movie::ScaleModeType )any_u32 );
	movie.Restart( );
	movie.HandleMouseMove( any_f, any_f, any_f );
	movie.HandleMouseBtn( ( flash_movie::mouse_btn_action )any_u32, any_u32, any_f, any_f );
	movie.HandleKeyboard( ( flash_movie::keyb_btn_action )any_u32, any_u32 );
	movie.HandleChar( ( wchar_t )any_u32 );
	movie.ForceCollectGarbage( );

	// flash_text_manager
	flash_text_manager	text_manager( s_loader );
	text_manager.set_viewport( any_u32, any_u32 );
	text_manager.create_text( s_str );
	text_manager.create_text_w( s_wstr );
	text_manager.tick( );

	// flash_text
	flash_text&	text	= *( flash_text* )s_text_manager;
	text.get_width( );
	text.get_height( );
	text.set_visible( any_u32 != 0 );
	text.set_font_size( any_f );
	text.set_position( any_f, any_f );
	text.set_color( ( u8 )any_u32, ( u8 )any_u32, ( u8 )any_u32, ( u8 )any_u32 );
	text.set_text( s_str );
	text_manager.destroy_text( text );

	// flash_renderer
	flash_movie*	movie_ptr	= s_movie;
	flash_renderer	renderer( s_queue, s_device, s_context );
	renderer.present( &movie_ptr, any_u32, s_text_manager );
	renderer.on_reset_device( any_u32, any_u32, s_device, s_context );

	// scaleform_render_command(_queue(_impl))
	scaleform_render_command_queue	queue( engine );

	scaleform_render_command_queue_impl	impl( engine );
	impl.PushThreadCommand( s_command );
	impl.GetRenderInterfaces( s_interfaces );

	scaleform_render_command	command;
	command.thread_command	= s_command;
	command.execute( );

	// flash_value: keep the generic GetVariable path alongside the methods not
	// otherwise reached by reconstructed owners. The sole live game call passes
	// "root.chat", which lets LTCG replace GetVariable's path parameter.
	flash_value	value;
	movie.GetVariable( &value, s_str );
	value.SetConvertStringW( );
}

} // namespace survarium

namespace vostok {

void anchor_scaleform( )
{
	survarium::use_scaleform( );
}

} // namespace vostok
