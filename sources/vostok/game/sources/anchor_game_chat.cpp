////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for survarium::chat_handler. The
// chat handler is owned by game (game::m_chat_handler), but game is abstract and
// never constructed in our sources, so /OPT:REF would strip the matched
// chat_handler bodies. anchor_game() drives anchor_game_chat( *game ), which
// references every out-of-line chat_handler method; the heavy construction is
// guarded behind a never-true flag so the placeholder game& is never touched.
//
// Every argument is laundered through a volatile so LTCG can not const-propagate
// the anchor's literals into the matched bodies (a constant scene/delta folds the
// show_movie / hide_movie / Advance calls away - the anchor is the sole caller).
//
// Retire once the real game call graph (game::create_*_menu / show / hide reach
// the chat handler for themselves).

#include "pch.h"

#include "chat_handler.h"
#include "game.h"
#include "base_game_scene.h"
#include <vostok/scaleform/sources/flash_function_handler_params.h>
#include <vostok/scaleform/sources/flash_value.h>

namespace survarium {

void anchor_game_chat( game& g )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	static base_game_scene* volatile				s_scene		= 0;
	static resources::unmanaged_resource_ptr const*	volatile s_ui	= 0;
	static flash_function_handler_params* volatile	s_params	= 0;
	static flash_movie* volatile					s_movie		= 0;
	static flash_value* volatile					s_values	= 0;
	static pcstr volatile							s_str		= 0;
	static wchar_t const* volatile					s_wstr		= 0;
	static volatile u32								s_u32		= 0;

	base_game_scene*			scene	= s_scene;
	flash_function_handler_params&	params	= *s_params;
	flash_movie*				movie	= s_movie;
	flash_value const*			values	= s_values;
	pcstr						str		= s_str;
	wchar_t const*				wstr	= s_wstr;
	const u32					any_u32	= s_u32;
	const messaging::message_channel_enum any_channel = ( messaging::message_channel_enum )any_u32;

	chat_handler handler( g );

	handler.initialize( *s_ui );

	input::world* const input_world = ( input::world* )s_movie;
	handler.on_keyboard_action( input_world, ( input::enum_keyboard )any_u32, ( input::enum_keyboard_action )any_u32 );
	handler.on_gamepad_action( input_world, ( input::gamepad_button )any_u32, ( input::enum_gamepad_action )any_u32 );
	handler.on_mouse_key_action( input_world, ( input::mouse_button )any_u32, ( input::enum_mouse_key_action )any_u32 );
	handler.on_mouse_move( input_world, any_u32, any_u32, any_u32 );

	handler.input_priority( );
	handler.callback( movie, str, values, any_u32 );
	handler.call( params );

	handler.focus( any_u32 != 0 );
	handler.show( scene );
	handler.hide( scene );
	handler.tick( any_u32 );
	handler.is_active( );
	handler.set_mode( any_u32 != 0 );
	handler.on_message_typed( wstr, any_channel );
	handler.add_message( any_channel, wstr, wstr );
	handler.add_to_recent_list( wstr );
	handler.in_match( );
	handler.get_movie( );
	handler.set_local_player_name( str );
}

} // namespace survarium
