////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_options.h"

#include "base_game_scene.h"
#include "base_network_client.h"
#include "game.h"
#include "key_binder.h"
#include "options_tab.h"
#include "options_item_base.h"
#include "text_translator.h"

#include "graphic_preset.h"

#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/input/world.h>
#include <vostok/math_constants.h>
#include <vostok/math_functions.h>
#include <vostok/math_uint2.h>
#include <vostok/resources.h>
#include <vostok/resources_classes.h>
#include <vostok/resources_queries_result.h>
#include <boost/bind.hpp>

namespace survarium {

// the table values live in .rdata and do not affect this TU's code bytes; the real
// preset contents are recovered separately.
static graphic_preset default_graphic_preset[10];

void game_options::apply_default_graphic( )
{
	for ( u32 i = 0; i < 10; ++i )
	{
		flash_value new_resolution_data[4];
		new_resolution_data[0].SetUInt( 2 );
		new_resolution_data[1].SetUInt( default_graphic_preset[i].option_id );
		new_resolution_data[2].SetUInt( default_graphic_preset[i].option_value );
		new_resolution_data[3].SetUInt( 0 );

		m_options_ui->movie->Invoke( "root.set_value", NULL, new_resolution_data, 4 );
	}
}

void game_options::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	if ( strings::equal( methodName, "menu_button" ) )
	{
		pcstr button = args[0].GetString( );

		if ( strings::equal( button, "back" ) )
			m_game.deactivate_main_menu( );
		else if ( strings::equal( button, "exit_to_os" ) )
			m_game.exit( "quit" );
		else if ( strings::equal( button, "settings" ) )
			show_options( true );
		else if ( strings::equal( button, "leave_match" ) )
			m_game.network_client( ).close_current_match( true );
	}
	else if ( strings::equal( methodName, "accept_changes" ) )
	{
		u32 tab_id = args[0].GetInt( );

		if ( tab_id == 1 )
			apply_key_bindings( );

		m_options[tab_id]->apply( m_options_ui );
	}
	else if ( strings::equal( methodName, "cancel_changes" ) )
	{
		u32 tab_id = args[0].GetInt( );

		m_options[tab_id]->revert( m_options_ui );

		if ( tab_id == 1 )
			reset_bindings( true );
	}
	else if ( strings::equal( methodName, "button_optimal_video" ) )
	{
	}
	else if ( strings::equal( methodName, "button_default_video" ) )
		apply_default_graphic( );
	else if ( strings::equal( methodName, "button_default_controls" ) )
		reset_bindings_to_defaults( );
	else if ( strings::equal( methodName, "start_bind_key" ) )
	{
		m_waiting_for_bind_action = game_action_id( args[0].GetInt( ) );
		m_parent_scene->hide_movie( m_cursor_ui );
	}
	else if ( strings::equal( methodName, "reassign_ok_clicked" ) )
	{
		assign_binding( m_conflicted_action_to_bind, m_conflicted_key_name );

		for ( vector< game_action_id >::iterator it = m_conflicted_action_ids.begin( ); it != m_conflicted_action_ids.end( ); ++it )
			assign_binding( *it, "" );
	}
}

void game_options::show_options( bool b_val )
{
	flash_value b_show_value;
	b_show_value.SetBoolean( b_val );
	m_options_ui->movie->Invoke( "root.show_settings", NULL, &b_show_value, 1 );
}

game_options::game_options( game& g ) :
	m_game( g ),
	m_waiting_for_bind_action( kLASTACTION )
{
}

game_options::~game_options( )
{
	for ( u32 i = 0; i < 4; ++i )
		DELETE( m_options[i] );
}

void game_options::initialize( )
{
	for ( u32 i = 0; i < options_count; ++i )
		m_options[i] = NEW( options_tab )( m_game, m_options_ui, options_enum( i ) );

	resources::request requests[2];
	requests[0].set( "resources/flash_movies/main_menu.swf", resources::flash_movie_class );
	requests[1].set( "resources/flash_movies/cursor.swf", resources::flash_movie_class );

	resources::query_resources( requests, boost::bind( &game_options::on_resources_ready, this, _1 ), g_allocator );
}

void game_options::on_resources_ready( resources::queries_result& data )
{
	m_options_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[0].get_unmanaged_resource( ) );

	m_options_ui->movie->SetBackgroundAlpha( 0.f );
	m_options_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_options_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );

	m_cursor_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[1].get_unmanaged_resource( ) );

	m_cursor_ui->movie->SetBackgroundAlpha( 0.f );
	m_cursor_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_cursor_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );

	m_options_ui->movie->SetExternalInterface( this );

	fill_labels( );
	fill_settings_data( );

	for ( u32 i = 0; i < options_count; ++i )
		m_options[i]->initialize_data( m_options_ui );

	initialize_bindings( );
}

// STATE[STUB]
// claude@NOTE: walks a file-static label-name table translating each entry via
// m_game.text_translator().translate_text(id, wbuf) + a flash_value array -> Movie::Invoke
// (34 target statements). Table is a named file-static (recoverable, cf. default_graphic_preset
// which now matches at 93%). Deferred only on reconstruction volume - the per-entry shape is
// the same proven flash_value-array+Invoke idiom; needs the label-name table defined + careful
// per-statement reconstruction over many build cycles.
void game_options::fill_labels( )
{
}

// STATE[STUB]
// claude@NOTE: picks in_game_world vs lobby button-name table, translates each entry and
// pushes a flash_value button array to Movie::Invoke. Same table-walk + flash_value-array
// + Invoke shape as fill_labels; deferred on reconstruction volume.
void game_options::fill_menu_buttons( bool in_game_world )
{
}

// STATE[STUB]
// claude@NOTE: builds the four per-tab fixed_string label tables
// (controllers/gameplay/video/sound options) and pushes them via flash_value -> Invoke.
// Largest method here (77 target statements). Same table-walk shape as fill_labels;
// deferred on reconstruction volume.
void game_options::fill_settings_data( )
{
}

void game_options::activate( base_game_scene* parent_scene )
{
	if ( m_is_active )
		return;

	m_parent_scene = parent_scene;
	m_parent_scene->show_movie( m_options_ui );
	m_parent_scene->show_movie( m_cursor_ui );

	m_is_active = true;
	m_game.input_world( ).add_handler( *this );

	fill_menu_buttons( parent_scene == &m_game.get_game_world( ) );
}

void game_options::deactivate( )
{
	if ( !m_is_active )
		return;

	m_parent_scene->hide_movie( m_options_ui );
	m_parent_scene->hide_movie( m_cursor_ui );

	m_parent_scene = NULL;
	m_is_active = false;
	m_game.input_world( ).remove_handler( *this );
}

void game_options::tick( const u32 frame_delta, const u32, const bool )	// PDB: last two params __formal, genuinely unused
{
	float deltaTime = frame_delta * math::epsilon_3;

	m_options_ui->movie->Advance( deltaTime, 0 );
	m_cursor_ui->movie->Advance( deltaTime, 0 );
}

// STATE[STUB]
// claude@NOTE: looks dik up in key_binder::dik_to_ptr + the file-static
// key_bind_descriptions[33] table (a named symbol, recoverable), detects conflicts
// against the action's existing bindings (a profile_slot_enum vector walk), and on
// success either assigns or pops a conflict dialog (text_translator + flash_value ->
// Invoke). Deferred with assign_binding (its main callee) on reconstruction volume.
bool game_options::process_key_input( s32 dik )
{
	return false;
}

// STATE[STUB]
// claude@NOTE: walks key_bind_descriptions[33] for the entry whose action_id == action_id,
// records key into its new_binded_key (a fixed_string assignf, marking is_default), then
// builds flash_value[3] bind_value { SetStringW(m_game.text_translator().translate_text(key,
// w_key_name_txt)), SetUInt(action_id), SetBoolean(is_default) } and Invokes "root.set_keybind".
// Locals: char const* key, bool is_default, flash_value[3] bind_value, wchar_t[512]
// w_key_name_txt. Structure fully recovered; deferred only on reconstruction volume + the shared
// key_bind_descriptions table definition (the proven flash_value-array+Invoke idiom, cf.
// apply_default_graphic 93% / refill_item_data 85%).
void game_options::assign_binding( game_action_id action_id, pcstr key )
{
}

void game_options::finish_binding( )
{
	m_options_ui->movie->Invoke( "root.end_keybind", NULL, NULL, 0 );
	m_parent_scene->show_movie( m_cursor_ui );
	m_waiting_for_bind_action = kLASTACTION;
}

bool game_options::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	if ( m_waiting_for_bind_action != kLASTACTION && action == input::kb_key_down )
	{
		if ( process_key_input( key ) )
			finish_binding( );
	}
	else
		m_parent_scene->get_game( ).input_translator( ).process_keyboard( input_world, key, action, m_options_ui->movie, m_game.game_time_ms( ) );

	return true;
}

bool game_options::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	return true;
}

bool game_options::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	if ( m_waiting_for_bind_action != kLASTACTION && action == input::ms_key_down )
	{
		if ( process_key_input( button ) )
		{
			m_options_ui->movie->Invoke( "root.end_keybind", NULL, NULL, 0 );
			m_parent_scene->show_movie( m_cursor_ui );
			m_waiting_for_bind_action = kLASTACTION;
		}
	}
	else
	{
		u32 mouse_btn = 0;
		switch ( button )
		{
			case input::mouse_button_left:		mouse_btn = 0; break;
			case input::mouse_button_right:		mouse_btn = 1; break;
			case input::mouse_button_middle:	mouse_btn = 2; break;
		}

		m_options_ui->movie->HandleMouseBtn(
			action == input::ms_key_down ? flash_movie::mouse_btn_down : flash_movie::mouse_btn_up,
			mouse_btn,
			( float )m_mouse_pos.x,
			( float )m_mouse_pos.y );
	}

	return true;
}

bool game_options::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	if ( m_waiting_for_bind_action == kLASTACTION )
	{
		m_mouse_pos.x += x;
		m_mouse_pos.y += y;

		math::uint2 const& window_size = m_parent_scene->output_window_size( );
		math::clamp( m_mouse_pos.x, 0, ( s32 )window_size.x );
		math::clamp( m_mouse_pos.y, 0, ( s32 )window_size.y );

		float scroll_delta = z * ( 1.f / 120.f );
		m_cursor_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
		m_options_ui->movie->HandleMouseMove( ( float )m_mouse_pos.x, ( float )m_mouse_pos.y, scroll_delta );
	}

	return true;
}

void game_options::refill_item_data( u8 options_tab_id, u8 options_item_id )
{
	flash_value options_item_data[3];
	options_item_data[0].SetUInt( options_tab_id );
	options_item_data[1].SetUInt( options_item_id );
	m_options_ui->movie->CreateArray( &options_item_data[2] );
	get_options_item( options_tab_id, options_item_id )->fill_data( options_item_data[2] );
	m_options_ui->movie->Invoke( "root.set_data_provider", NULL, options_item_data, 3 );
}

// STATE[STUB]
// claude@NOTE: walks key_bind_descriptions[33], building a keybinds flash array of
// label + current-key per action and CreateObject/Invoke into the UI. Tables are named
// file-statics (key_bind_descriptions / keyboards, both recoverable); deferred with the
// rest of the binding set (shares the translate_text-signature wall).
void game_options::initialize_bindings( )
{
}

// STATE[STUB]
// claude@NOTE: per-action loop over key_bind_descriptions[33] resolving each action's
// default (or current) key via the keyboards[] descriptor table (?keyboards@survarium@@,
// defined in key_binder.cpp - needs an extern decl to reference here), then SetStringW +
// Invoke. Structure clear; deferred with the binding set.
void game_options::reset_bindings( bool is_default )
{
}

// STATE[STUB]
// claude@NOTE: loop over key_bind_descriptions[33]: where new_binded_key != old_binded_key,
// STR_JOIN a "bind"/"unbind" console command (id_to_action_name + the new key) and apply it
// via console_commands::find(...)->execute. Table ref is &key_bind_descriptions[0].new_binded_key
// (symbol+0x10 addend). Structure clear; deferred - id_to_action_name's call convention here
// (action_id in edx, no obvious `this`) and the STR_JOIN/unbind path need byte validation.
void game_options::apply_key_bindings( )
{
}

void game_options::reset_bindings_to_defaults( )
{
	m_game.get_key_binder( ).set_default_controls( );
	reset_bindings( false );
	apply_key_bindings( );
}


} // namespace survarium
