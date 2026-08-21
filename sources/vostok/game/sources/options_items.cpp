////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "options_gamma_selector.h"
#include "options_graphics_quality_selector.h"
#include "options_item_base.h"
#include "options_item_bool.h"
#include "options_item_float.h"
#include "options_item_int.h"
#include "options_monitor_index_selector.h"
#include "options_resolution_selector.h"
#include "options_tab.h"
#include "graphic_preset.h"
#include "base_game_scene.h"
#include "game.h"
#include "game_options.h"
#include "text_translator.h"
#include "game_memory.h"
#include <vostok/console_command_processor.h>
#include <vostok/console_command.h>
#include <vostok/fixed_string.h>
#include <vostok/strings_functions.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_function_handler_params.h>

#undef NEW
#undef NEW_ARRAY
#undef DELETE
#undef DELETE_ARRAY
#define NEW( type ) VOSTOK_NEW_IMPL( ::survarium::g_allocator, type )
#define NEW_ARRAY( type, count ) VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, type, count )
#define DELETE( pointer ) VOSTOK_DELETE_IMPL( ::survarium::g_allocator, pointer )
#define DELETE_ARRAY( pointer ) VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, pointer )

// claude@NOTE: g_num_monitors is defined by render/core/dx11 (device.h names this file as
// its reader), but including that header drags d3d11/windows into a game TU and the winsdk
// ole2.h/ocidl.h then fail on MSG (C2061). Declared here instead.
namespace vostok {
namespace render {
	extern int g_num_monitors;
} // namespace render
} // namespace vostok

namespace survarium {

// the per-quality-preset table (presets x 10 options). String/data bytes live in
// .rdata and are recovered separately; the code here only relocates the symbol.
graphic_preset g_graphic_presets[ 5 ][ 10 ];

// claude@NOTE: structure matches (3 stmts: find, null-check, LOG_ERROR). Byte
// residual is the flash_function_handler base ctor, which the target INLINES (the
// Scaleform GlobalHeap alloc + flash_function_handler_impl construction); our
// scaleform::flash_function_handler::flash_function_handler is an out-of-line stub
// in value.cpp so the compiler emits a call instead - a cross-module scaleform cap.
 options_item_base::options_item_base(
	options_tab&				parent_tab,
	pcstr						console_command,
	u8							option_item_id,
	option_item_type_enum		type
)
	: m_type( type )
	, m_parent_tab( parent_tab )
	, m_option_item_id( option_item_id )
#line 108
{
	m_console_command = console_commands::find( console_command );

	if ( m_console_command == NULL )
		LOG_ERROR( "Console command [%s] not found for option item", console_command );
}

// claude@NOTE: structure matches (flash_value[4] + 3 SetUInt + fill_value + Invoke).
// Byte residual is the inlined scaleform flash_value setters/dtors + the inlined
// flash_movie::Invoke (-> Scaleform::GFx::Movie::Invoke), an SDK cap we do not
// reproduce. Matching this (real body, not inlined) also makes the derived
// options_item_{int,float,bool}::revert emit their tail-jmp to it.
void options_item_base::revert( )
#line 115
{
	flash_value source_data[ 4 ];

	source_data[ 0 ].SetUInt( m_parent_tab.type( ) );
	source_data[ 1 ].SetUInt( m_option_item_id );
	fill_value( source_data[ 2 ] );
	source_data[ 3 ].SetUInt( 1 );

	m_parent_tab.get_movie( )->movie->Invoke( "root.set_value", NULL, source_data, 4 );
}

 options_item_int::options_item_int(
	options_tab&	parent_tab,
	pcstr			console_command,
	u8				option_item_id,
	pcstr*			values,
	u8				values_count
)
	: options_item_base( parent_tab, console_command, option_item_id, string_selector )
	, m_values( values )
	, m_values_count( values_count )
#line 128
{
}

void options_item_int::initialize( )
#line 134
{
	m_source_value	= m_console_command ? ( u8 )( ( console_commands::cc_u32* )m_console_command )->get_value( ) : 0;
	m_current_value	= m_source_value;
}

void options_item_int::fill_data( flash_value& val )
#line 146
{
	for ( u8 i = 0; i < m_values_count; ++i )
	{
		flash_value str_val;
		wchar_t val_txt[ 512 ];
		m_parent_tab.get_game( ).text_translator( ).translate_text( m_values[ i ], val_txt );

		str_val.SetStringW( val_txt );

		val.SetElement( i, str_val );
	}
}

// claude@NOTE: scaleform cap (applies to every flash_value setter call in this TU -
// fill_value/fill_data/apply/call/revert). The target INLINES flash_value::SetUInt/
// SetNumber/SetBoolean/... (canonical flash_value.h declares them inline; the GFx::
// Value body lives in the Scaleform SDK we do not reproduce). Our value.cpp has them
// out-of-line, so the compiler emits a call - byte residual on an otherwise-correct
// 1-statement structure.
void options_item_int::fill_value( flash_value& val )
#line 163
{
	val.SetUInt( m_current_value );
}

void options_item_int::apply( )
#line 167
{
	m_source_value = m_current_value;

	if ( m_console_command )
	{
		fixed_string< 8 > args;
		args.assignf( "%d", m_current_value );
		m_console_command->execute( args.c_str( ) );
	}
}

void options_item_int::revert( )
#line 177
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_int::call( flash_function_handler_params& params )
#line 184
{
	m_current_value = ( u8 )params.pArgs[ 0 ].GetUInt( );

	if ( m_parent_tab.type( ) == video_options_type )
	{
		u8 graphics_quality = ( ( options_graphics_quality_selector* )m_parent_tab.option_by_id( 8 ) )->value( );

		if ( graphics_quality < 5 )
			for ( u8 i = 0; i < 10; ++i )
			{
				graphic_preset& preset = g_graphic_presets[ graphics_quality ][ i ];

				if ( preset.option_id == m_option_item_id && preset.option_value < m_values_count && m_current_value != preset.option_value )
				{
					flash_value new_resolution_data[ 4 ];
					new_resolution_data[ 0 ].SetUInt( video_options_type );
					new_resolution_data[ 1 ].SetUInt( 8 );
					new_resolution_data[ 2 ].SetUInt( 5 );
					new_resolution_data[ 3 ].SetUInt( 0 );

					m_parent_tab.get_movie( )->movie->Invoke( "root.set_value", NULL, new_resolution_data, 4 );
					break;
				}
			}
	}

	if ( m_current_value >= m_values_count && m_values_count )
		m_current_value = m_values_count - 1;

	params.pRetVal->SetUInt( m_current_value );
}

 options_item_float::options_item_float(
	options_tab&	parent_tab,
	pcstr			console_command,
	u8				option_item_id,
	float			step
)
	: options_item_base( parent_tab, console_command, option_item_id, slider_selector )
	, m_step( step )
#line 225
{
}

void options_item_float::initialize( )
#line 230
{
	m_source_value	= m_console_command ? ( ( console_commands::cc_value< float >* )m_console_command )->get_value( ) : 0.0f;
	m_current_value	= m_source_value;
}

void options_item_float::fill_data( flash_value& val )
#line 240
{
	console_commands::cc_value< float >* command = ( console_commands::cc_value< float >* )m_console_command;
	flash_value slider_data_member;

	slider_data_member.SetNumber( m_step );
	val.SetMember( "snapInterval", slider_data_member );

	slider_data_member.SetNumber( command ? command->get_min( ) : 0.0f );
	val.SetMember( "minimum", slider_data_member );

	slider_data_member.SetNumber( command ? command->get_max( ) : 100.0f );
	val.SetMember( "maximum", slider_data_member );
}

void options_item_float::fill_value( flash_value& val )
#line 256
{
	val.SetNumber( m_current_value );
}

void options_item_float::apply( )
#line 260
{
	m_source_value = m_current_value;

	if ( m_console_command )
	{
		fixed_string< 8 > args;
		args.assignf( "%.2f", m_current_value );
		m_console_command->execute( args.c_str( ) );
	}
}

void options_item_float::revert( )
#line 270
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_float::call( flash_function_handler_params& params )
#line 277
{
	m_current_value = ( float )params.pArgs[ 0 ].GetNumber( );
	params.pRetVal->SetNumber( m_current_value );
}

 options_item_bool::options_item_bool( options_tab& parent_tab, pcstr console_command, u8 option_item_id )
	: options_item_base( parent_tab, console_command, option_item_id, bool_selector )
#line 289
{
}

void options_item_bool::initialize( )
#line 293
{
	m_source_value	= m_console_command ? ( ( console_commands::cc_value< bool >* )m_console_command )->get_value( ) : false;
	m_current_value	= m_source_value;
}

void options_item_bool::fill_data( flash_value& __formal )
#line 303
{
	VOSTOK_UNREFERENCED_PARAMETER( __formal );
}

void options_item_bool::fill_value( flash_value& val )
#line 309
{
	val.SetBoolean( m_current_value );
}

void options_item_bool::apply( )
#line 313
{
	m_source_value = m_current_value;

	if ( m_console_command )
	{
		fixed_string< 8 > args;
		args.assignf( "%d", m_current_value );
		m_console_command->execute( args.c_str( ) );
	}
}

void options_item_bool::revert( )
#line 323
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_bool::call( flash_function_handler_params& params )
#line 330
{
	params.pRetVal->SetBoolean( m_current_value = params.pArgs[ 0 ].GetBool( ) );
}

void options_gamma_selector::call( flash_function_handler_params& params )
#line 340
{
	params.pRetVal->SetNumber( m_current_value = (float)params.pArgs[ 0 ].GetNumber( ) );
	m_parent_tab.get_game( ).active_scene( )->scene_renderer( ).set_gamma_correction_factor( m_current_value );
}

void options_gamma_selector::revert( )
#line 347
{
	options_item_float::revert( );
	m_parent_tab.get_game( ).active_scene( )->scene_renderer( ).set_gamma_correction_factor( m_current_value );
}

// claude@NOTE: base-init (r_resolution, id 1) matches; the find + fill_resolutions
// call (lines 357/359) is DCE'd because fill_resolutions is an empty render-blocked
// stub - the optimizer drops the whole call (and its find argument). Unblocks once
// fill_resolutions gets its real body (see its render-cap NOTE below).
 options_resolution_selector::options_resolution_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_resolution", 1, NULL, 0 )
#line 354
{
	fill_resolutions( ( u8 )( ( console_commands::cc_u32* )console_commands::find( "r_monitor_index" ) )->get_value( ) );
}

// STATE[STUB]
// claude@NOTE: the "render does not expose the monitor table" cause is stale -
// g_monitor_resolutions / g_num_monitors are declared at render/core/dx11/device.h:65-66
// and filled in device.cpp:103-105. Declare them locally as the ctor above does (that
// header cannot be included from a game TU: it drags d3d11/windows in and the winsdk
// ole2.h/ocidl.h then fail on MSG). What is left is the 27-statement body itself
// (locals: fixed_string<32> old_resolution, u32 old_resolution_index,
// flash_value new_resolution_data[4]) - not yet reconstructed.
void options_resolution_selector::fill_resolutions( u8 monitor_number )
#line 368
{
	VOSTOK_UNREFERENCED_PARAMETER( monitor_number );
}

void options_resolution_selector::initialize( )
#line 413
{
	pcstr current_resolution = ( ( console_commands::cc_string* )m_console_command )->get_value( );

	for ( u8 i = 0; i < m_values_count; ++i )
		if ( strings::equal( m_values[ i ], current_resolution ) )
		{
			m_source_value = i;
			m_current_value = m_source_value;
			return;
		}

	m_current_value = m_source_value;
}

void options_resolution_selector::apply( )
#line 427
{
	m_source_value = m_current_value;
	m_console_command->execute( m_values[ m_current_value ] );
}

 options_monitor_index_selector::options_monitor_index_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_monitor_index", 0, NULL, 0 )
#line 435
{
	for ( u8 i = 0; i < array_size( m_cached_monitors_names ); ++i )
		m_cached_monitors_names[ i ].assignf( "%d", i );

	m_values		= NEW_ARRAY( pcstr, render::g_num_monitors );
	m_values_count	= ( u8 )render::g_num_monitors;

	for ( u8 i = 0; i < m_values_count; ++i )
		m_values[ i ] = m_cached_monitors_names[ i ].c_str( );
}

void options_monitor_index_selector::call( flash_function_handler_params& params )
#line 451
{
	options_item_int::call( params );
	refill_resolutions_data( );
}

void options_monitor_index_selector::revert( )
#line 457
{
	options_item_int::revert( ); refill_resolutions_data( );
}

// claude@NOTE: refill_resolutions_data is STRUCTURE MATCH; revert/call inline it.
// Byte residual: fill_resolutions is still a STUB (render-blocked - it enumerates
// resolutions via render display-mode data we do not expose), and refill_item_data is
// tail-called with this passed in eax + the two u8 args elided by LTCG (the allowed
// argument-passing exception). refill_item_data itself is scaleform-capped in
// game_options.cpp.
void options_monitor_index_selector::refill_resolutions_data( )
#line 463
{
	( ( options_resolution_selector* )m_parent_tab.option_by_id( 1 ) )->fill_resolutions( m_current_value );
	m_parent_tab.get_game( ).get_game_options( ).refill_item_data( m_parent_tab.type( ), m_option_item_id );
}

// claude@NOTE: graphics_quality_data is a .rdata pcstr[6] table absent from the
// canonical dump; the exact preset strings are NOT recoverable from the available
// binary/PDB, so these placeholders are best-guesses. The ctor match only needs the
// table SYMBOL (it relocates the pointer); the table's own string bytes score as a
// separate data symbol and will not match until the real strings are recovered.
static pcstr graphics_quality_data[ 6 ] =
{
	"minimal",
	"low",
	"medium",
	"high",
	"max",
	"extreme"
};

 options_graphics_quality_selector::options_graphics_quality_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_graphics_quality", 8, graphics_quality_data, 6 )
#line 480
{
}

void options_graphics_quality_selector::call( flash_function_handler_params& params )
#line 485
{
	options_item_int::call( params );

	if ( m_current_value != m_values_count - 1 )
		for ( u8 i = 0; i < 10; ++i )
		{
			graphic_preset& preset = g_graphic_presets[ m_current_value ][ i ];

			flash_value new_resolution_data[ 4 ];
			new_resolution_data[ 0 ].SetUInt( video_options_type );
			new_resolution_data[ 1 ].SetUInt( preset.option_id );
			new_resolution_data[ 2 ].SetUInt( preset.option_value );
			new_resolution_data[ 3 ].SetUInt( 0 );

			m_parent_tab.get_movie( )->movie->Invoke( "root.set_value", NULL, new_resolution_data, 4 );
		}
}

// claude@NOTE: the per-option value tables (counts taken from the ctor's
// options_item_int args). Like graphics_quality_data, the .rdata string bytes are
// not recoverable from the available binary/PDB, so these are best-guess
// placeholders; the ctor match only needs each table SYMBOL (it relocates the
// pointer), the table's own string bytes score as separate data symbols.
static pcstr antialiasing_data[ 3 ]				= { "off", "fxaa", "msaa" };
static pcstr anisotrophic_filtering_data[ 5 ]	= { "off", "x2", "x4", "x8", "x16" };
static pcstr texture_quality_data[ 3 ]			= { "low", "medium", "high" };
static pcstr geometry_quality_data[ 2 ]			= { "low", "high" };
static pcstr shadow_quality_data[ 4 ]			= { "off", "low", "medium", "high" };
static pcstr lighting_quality_data[ 4 ]			= { "off", "low", "medium", "high" };
static pcstr shading_quality_data[ 4 ]			= { "low", "medium", "high", "max" };
static pcstr decorations_quality_data[ 3 ]		= { "off", "low", "high" };
static pcstr post_process_quality_data[ 4 ]		= { "off", "low", "medium", "high" };
static pcstr ambient_occlusion_data[ 4 ]		= { "off", "low", "medium", "high" };
static pcstr particles_quality_data[ 3 ]		= { "low", "medium", "high" };
static pcstr motion_blur_quality_data[ 4 ]		= { "off", "low", "medium", "high" };

// claude@NOTE: compiler-context wall: target retains strip_pointer( g_allocator )
// at all four NEW_ARRAY sites; base inlines the same template calls.
 options_tab::options_tab( game& g, flash_movie_resource_ptr& movie, options_enum type )
	: m_type( type )
	, m_game( g )
	, m_movie( movie )
#line 509
{
	switch ( type )
	{
		case gameplay_options_type:
		{

			m_options_count = 9;	m_options = NEW_ARRAY( options_item_base*, 9 );

			m_options[ 0 ] = NEW( options_item_bool )( *this, "g_invite_from_friends", 0 );
			m_options[ 1 ] = NEW( options_item_bool )( *this, "g_friends_signin_notification", 1 );
			m_options[ 2 ] = NEW( options_item_bool )( *this, "g_messages_censor", 2 );
			m_options[ 3 ] = NEW( options_item_bool )( *this, "g_messages_only_from_friends", 3 );
			m_options[ 4 ] = NEW( options_item_bool )( *this, "g_private_messages_in_game", 4 );
			m_options[ 5 ] = NEW( options_item_bool )( *this, "g_hide_spam", 5 );
			m_options[ 6 ] = NEW( options_item_int )( *this, "g_crosshair_type", 6, NULL, 0 );
			m_options[ 7 ] = NEW( options_item_bool )( *this, "g_crosshair_static", 7 );
			m_options[ 8 ] = NEW( options_item_bool )( *this, "is_ui_minimap_rotable", 8 );
			break;
		}
		case video_options_type:
		{
			m_options_count = 19;	m_options = NEW_ARRAY( options_item_base*, 19 );

			m_options[ 0 ]	= NEW( options_monitor_index_selector )( *this );
			m_options[ 1 ]	= NEW( options_resolution_selector )( *this );
			m_options[ 2 ]	= NEW( options_item_bool )( *this, "r_fullscreen", 2 );
			m_options[ 3 ]	= NEW( options_item_bool )( *this, "r_vsync", 3 );
			m_options[ 4 ]	= NEW( options_item_int )( *this, "r_antialiasing_method", 4, antialiasing_data, 3 );
			m_options[ 5 ]	= NEW( options_item_int )( *this, "r_max_anisotropic", 5, anisotrophic_filtering_data, 5 );
			m_options[ 6 ]	= NEW( options_gamma_selector )( *this );
			m_options[ 7 ]	= NEW( options_item_float )( *this, "fov", 7, 1.0f );
			m_options[ 8 ]	= NEW( options_graphics_quality_selector )( *this );
			m_options[ 9 ]	= NEW( options_item_int )( *this, "r_texture_quality", 9, texture_quality_data, 3 );
			m_options[ 10 ]	= NEW( options_item_int )( *this, "r_geometry_quality", 10, geometry_quality_data, 2 );
			m_options[ 11 ]	= NEW( options_item_int )( *this, "r_shadow_quality", 11, shadow_quality_data, 4 );
			m_options[ 12 ]	= NEW( options_item_int )( *this, "r_lighting_quality", 12, lighting_quality_data, 4 );
			m_options[ 13 ]	= NEW( options_item_int )( *this, "r_shading_quality", 13, shading_quality_data, 4 );
			m_options[ 14 ]	= NEW( options_item_int )( *this, "r_decorations_quality", 14, decorations_quality_data, 3 );
			m_options[ 15 ]	= NEW( options_item_int )( *this, "r_post_process_quality", 15, post_process_quality_data, 4 );
			m_options[ 16 ]	= NEW( options_item_int )( *this, "r_ambient_occlusion_quality", 16, ambient_occlusion_data, 4 );
			m_options[ 17 ]	= NEW( options_item_int )( *this, "r_particles_quality", 17, particles_quality_data, 3 );
			m_options[ 18 ]	= NEW( options_item_int )( *this, "r_motion_blur_quality", 18, motion_blur_quality_data, 4 );
			break;
		}
		case controllers_options_type:
		{
			m_options_count = 2;	m_options = NEW_ARRAY( options_item_base*, 2 );

			m_options[ 0 ]	= NEW( options_item_bool )( *this, "mouse_invertion", 0 );
			m_options[ 1 ]	= NEW( options_item_float )( *this, "sensitivity", 1, 0.05f );
			break;
		}


		case sound_options_type:
		{
			m_options_count = 7;	m_options = NEW_ARRAY( options_item_base*, 7 );

			m_options[ 0 ]	= NEW( options_item_float )( *this, "s_general_volume", 0, 1.0f );
			m_options[ 1 ]	= NEW( options_item_float )( *this, "s_ingame_volume", 1, 1.0f );
			m_options[ 2 ]	= NEW( options_item_float )( *this, "s_music_volume", 2, 1.0f );
			m_options[ 3 ]	= NEW( options_item_float )( *this, "s_chat_volume", 3, 1.0f );
			m_options[ 4 ]	= NEW( options_item_bool )( *this, "s_use_microphone", 4 );
			m_options[ 5 ]	= NEW( options_item_float )( *this, "s_mic_sens", 5, 1.0f );
			m_options[ 6 ]	= NEW( options_item_bool )( *this, "s_ptt_button", 6 );
			break;
		}
	}
}

// claude@NOTE: compiler-context wall: target retains strip_pointer( g_allocator )
// at DELETE_ARRAY; base inlines the same template call.
 options_tab::~options_tab( )
#line 579
{
	for ( u8 i = 0; i < m_options_count; ++i )
		DELETE( m_options[ i ] );

	DELETE_ARRAY( m_options );
}

void options_tab::apply( flash_movie_resource_ptr& movie )
#line 588
{
	for ( u8 i = 0; i < m_options_count; ++i )
		m_options[ i ]->apply( );

	if ( type( ) == video_options_type )
		m_game.active_scene( )->scene_renderer( ).end_render_options_changing(
			m_game.active_scene( )->render_scene( ),
			m_game.render_output_window( ),
			false,
			true,
			NULL
		);

	console_commands::find( "cfg_save_user" )->execute( "" );

	initialize_data( movie );
}

void options_tab::revert( flash_movie_resource_ptr& movie )
#line 608
{
	VOSTOK_UNREFERENCED_PARAMETER( movie );

	for ( u8 i = 0; i < m_options_count; ++i )
		m_options[ i ]->revert( );
}

void options_tab::initialize_data( flash_movie_resource_ptr& movie )
#line 615
{
	flash_value options_args[ 3 ];
	options_args[ 0 ].SetUInt( m_type );
	options_args[ 2 ].SetBoolean( true );

	movie->movie->CreateArray( &options_args[ 1 ] );

	for ( u8 i = 0; i < m_options_count; ++i )
	{
		m_options[ i ]->initialize( );

		flash_value option_item;
		movie->movie->CreateObject( &option_item );

		flash_value option_item_member;
		option_item_member.SetUInt( i );			option_item.SetMember( "id", option_item_member );

		m_options[ i ]->fill_value( option_item_member );
		option_item.SetMember( "value", option_item_member );

		options_args[ 1 ].SetElement( i, option_item );
	}

	movie->movie->Invoke( "root.set_values", NULL, options_args, 3 );
}

} // namespace survarium
