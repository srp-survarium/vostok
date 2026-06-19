////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_options.h"

#include "base_game_scene.h"
#include "game.h"
#include "key_binder.h"
#include "options_tab.h"
#include "options_item_base.h"
#include "text_translator.h"

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

// STATE[STUB]
// claude@NOTE: structure is a loop over the file-static survarium::default_graphic_preset[]
// (graphic_preset[10]) building a flash_value[4] per entry and calling
// m_options_ui->movie->Invoke( "root.set_value", ..., 4 ). Blocked on the preset
// table's data (lives in target .rdata; not in the canonical dump), and byte-capped
// by the scaleform flash_value::SetInt / flash_movie::Invoke stubs anyway.
void game_options::apply_default_graphic( )
{
}

// STATE[STUB]
// claude@NOTE: flash external-interface dispatcher - a string-compare chain over
// methodName routing to options_tab::apply/revert, reset_bindings(_to_defaults),
// apply_default_graphic, apply_key_bindings, assign_binding + base_game_scene::hide_movie.
// Byte-capped by the base_game_scene movie stubs; several callees are themselves
// data-table blocked.
void game_options::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
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

// STATE[STUB]
// claude@NOTE: extracts the main_menu + cursor flash_movie_resources from the
// queries_result, configures both movies (SetBackgroundAlpha/SetViewScaleMode/...),
// then fill_labels() + initialize_bindings(). Byte-capped by the scaleform movie
// setters; tail callees are themselves data-table blocked (fill_labels/initialize_bindings).
void game_options::on_resources_ready( resources::queries_result& data )
{
}

// STATE[STUB]
// claude@NOTE: walks a file-static options_name_to_label[22] table translating each
// label via text_translator + flash_value array -> Movie::Invoke. Blocked on the
// label-name table data (target .rdata, not in the canonical dump); also scaleform-capped.
void game_options::fill_labels( )
{
}

// STATE[STUB]
// claude@NOTE: picks in_game_world vs lobby main_menu_button_name_to_action[] table,
// translates each entry and pushes a flash_value button array to Movie::Invoke. Blocked
// on the two file-static button-name tables (target .rdata); also scaleform-capped.
void game_options::fill_menu_buttons( bool in_game_world )
{
}

// STATE[STUB]
// claude@NOTE: builds the four per-tab fixed_string<64> label tables
// (controllers/gameplay/video/sound options) and pushes them via flash_value -> Invoke.
// Blocked on those file-static label tables (target .rdata); also scaleform-capped.
// Largest method here (107 target statements).
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
// key_bind_descriptions[33] table, detects conflicts against the action's existing
// bindings (a profile_slot_enum vector walk), and on success either assigns or pops a
// conflict dialog (text_translator + flash_value -> Invoke). Blocked on the
// key_bind_descriptions[] table data (target .rdata); also scaleform-capped.
bool game_options::process_key_input( s32 dik )
{
	return false;
}

// STATE[STUB]
// claude@NOTE: writes the chosen key into the action's binding and shows it in the UI
// (key_binder + text_translator + flash_value[3] -> Invoke). Scaleform/string capped;
// reachable only from process_key_input which is data-table blocked.
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
// label + current-key per action and CreateObject/Invoke into the UI. Blocked on the
// key_bind_descriptions[] table data (target .rdata); also scaleform-capped.
void game_options::initialize_bindings( )
{
}

// STATE[STUB]
// claude@NOTE: per-action loop over key_bind_descriptions[33] resolving each action's
// default (or current) key via the keyboards[] descriptor table, then SetStringW +
// Invoke. Blocked on the key_bind_descriptions[] / keyboards[] table data (target .rdata).
void game_options::reset_bindings( bool is_default )
{
}

// STATE[STUB]
// claude@NOTE: per-action loop over key_bind_descriptions[33] that STR_JOINs a console
// command from the action name and applies it via console_commands::find. Blocked on
// the key_bind_descriptions[] table data (target .rdata).
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
