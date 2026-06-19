////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for game_options and the
// swf_input_translator forwarding helpers it calls. game_options is constructed
// by game (m_game_options member), so its ctor/dtor/vtable are reachable; but the
// menu-input path that drives activate/deactivate/process_key_input and friends
// is still stubbed in game.cpp (activate_main_menu / deactivate_main_menu), so the
// out-of-line game_options bodies are otherwise dead-stripped.
//
// swf_input_translator::process_keyboard is an out-of-line stub with NO other
// referencer in the enabled build; game_options::on_keyboard_action is its first
// caller, so without an anchor reference its IL never materialises a standalone
// COMDAT and the EXE link fails (LNK2001). use_game_options() forces every
// out-of-line game_options method to be emitted.
//
// Retire once the real game menu-input call graph (game::activate_main_menu and
// the input dispatch) reaches these for itself.

#include "pch.h"

#include "game_options.h"
#include "game.h"
#include "swf_input_translator.h"

#include <vostok/input/world.h>
#include <vostok/scaleform/sources/flash_movie.h>

namespace survarium {

void use_game_options( )
{
	// swf_input_translator::process_keyboard is referenced only by
	// game_options::on_keyboard_action, which is itself only vtable-reachable, so
	// the whole chain (and any reference inside a never-true guard) is dead-stripped
	// by LTCG before code-gen, leaving on_keyboard_action's `call` dangling
	// (LNK2001). Pin its standalone COMDAT with an UNCONDITIONAL address-take into a
	// volatile sink, which whole-program opt cannot prove dead.
	static pcvoid volatile s_sink = 0;
	bool ( swf_input_translator::* const proc_kb )(
		input::world*, input::enum_keyboard, input::enum_keyboard_action, flash_movie*, u32 )
		= &swf_input_translator::process_keyboard;
	s_sink = *( pcvoid const* )&proc_kb;

	// self-guarded: the compiler still emits every reference (/OPT:REF keeps the
	// symbols) but the body never runs, so the placeholder refs are never deref'd.
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	game& g = *( game* )NULL;
	game_options options( g );

	base_game_scene* scene = ( base_game_scene* )NULL;
	input::world* world = ( input::world* )NULL;

	options.tick( 0, 0, false );
	options.show_options( false );
	options.initialize( );
	options.initialize_bindings( );
	options.fill_labels( );
	options.fill_settings_data( );
	options.fill_menu_buttons( false );
	options.activate( scene );
	options.deactivate( );
	options.refill_item_data( 0, 0 );
	options.on_resources_ready( *( resources::queries_result* )NULL );		// private (friend)
	options.process_key_input( 0 );											// private (friend)
	options.assign_binding( kLEFT, "" );									// private (friend)
	options.finish_binding( );												// private (friend)
	options.apply_default_graphic( );										// private (friend)
	options.apply_key_bindings( );											// private (friend)
	options.reset_bindings( false );										// private (friend)
	options.reset_bindings_to_defaults( );									// private (friend)
	options.on_keyboard_action( world, input::enum_keyboard( 0 ), input::enum_keyboard_action( 0 ) );
	options.on_gamepad_action( world, input::gamepad_button( 0 ), input::enum_gamepad_action( 0 ) );
	options.on_mouse_key_action( world, input::mouse_button( 0 ), input::enum_mouse_key_action( 0 ) );
	options.on_mouse_move( world, 0, 0, 0 );
}

} // namespace survarium
