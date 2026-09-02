// SPDX-License-Identifier: GPL-3.0-or-later

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

namespace vostok {
namespace render {
	extern int g_num_monitors;
	extern math::int2 g_monitor_resolutions[ 6 ][ 512 ];
} // namespace render
} // namespace vostok

namespace survarium {

graphic_preset g_graphic_presets[ 5 ][ 10 ] =
{
	{
		{ texture_quality_option, 0 }, { geometry_quality_option, 1 },
		{ shadow_quality_option, 0 }, { lightning_quality_option, 0 },
		{ shading_quality_option, 0 }, { decorations_option, 0 },
		{ post_process_option, 0 }, { ambient_occlusion_option, 0 },
		{ particles_quality_option, 0 }, { motion_blur_amount_option, 0 },
	},
	{
		{ texture_quality_option, 1 }, { geometry_quality_option, 1 },
		{ shadow_quality_option, 1 }, { lightning_quality_option, 1 },
		{ shading_quality_option, 1 }, { decorations_option, 1 },
		{ post_process_option, 1 }, { ambient_occlusion_option, 1 },
		{ particles_quality_option, 1 }, { motion_blur_amount_option, 1 },
	},
	{
		{ texture_quality_option, 2 }, { geometry_quality_option, 1 },
		{ shadow_quality_option, 2 }, { lightning_quality_option, 2 },
		{ shading_quality_option, 2 }, { decorations_option, 2 },
		{ post_process_option, 2 }, { ambient_occlusion_option, 2 },
		{ particles_quality_option, 2 }, { motion_blur_amount_option, 2 },
	},
	{
		{ texture_quality_option, 3 }, { geometry_quality_option, 1 },
		{ shadow_quality_option, 3 }, { lightning_quality_option, 3 },
		{ shading_quality_option, 3 }, { decorations_option, 3 },
		{ post_process_option, 3 }, { ambient_occlusion_option, 3 },
		{ particles_quality_option, 3 }, { motion_blur_amount_option, 3 },
	},
	{
		{ texture_quality_option, 4 }, { geometry_quality_option, 1 },
		{ shadow_quality_option, 4 }, { lightning_quality_option, 4 },
		{ shading_quality_option, 4 }, { decorations_option, 4 },
		{ post_process_option, 4 }, { ambient_occlusion_option, 4 },
		{ particles_quality_option, 4 }, { motion_blur_amount_option, 4 },
	},
};

 options_item_base::options_item_base(
	options_tab&				parent_tab,
	pcstr						console_command,
	u8							option_item_id,
	option_item_type_enum		type
)
	: m_type( type )
	, m_parent_tab( parent_tab )
	, m_option_item_id( option_item_id )
{
	m_console_command = console_commands::find( console_command );

	if ( m_console_command == NULL )
		LOG_ERROR( "Console command [%s] not found for options_item [%d]", console_command, option_item_id );
}

void options_item_base::revert( )
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
{
}

void options_item_int::initialize( )
{
	if ( m_console_command )
		m_source_value = ( u8 )( ( console_commands::cc_u32* )m_console_command )->get_value( );
	else
		m_source_value = 0;

	m_current_value	= m_source_value;
}

void options_item_int::fill_data( flash_value& val )
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

void options_item_int::fill_value( flash_value& val )
{
	val.SetUInt( m_current_value );
}

void options_item_int::apply( )
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
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_int::call( flash_function_handler_params& params )
{
	m_current_value = ( u8 )params.pArgs[ 0 ].GetUInt( );

	if ( m_parent_tab.type( ) == video_options_type )
	{
		u8 graphics_quality = ( ( options_graphics_quality_selector* )m_parent_tab.option_by_id( 8 ) )->value( );

		if ( graphics_quality < 5 )
			for ( u8 i = 0; i < 10; ++i )
			{
				graphic_preset& preset = g_graphic_presets[ graphics_quality ][ i ];

				if ( preset.option_id == m_option_item_id && m_values_count > preset.option_value && m_current_value != preset.option_value )
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

	if ( m_current_value >= m_values_count && m_values_count > 0 )
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
{
}

void options_item_float::initialize( )
{
	if ( m_console_command )
		m_source_value = ( ( console_commands::cc_value< float >* )m_console_command )->get_value( );
	else
		m_source_value = 0.0f;

	m_current_value	= m_source_value;
}

void options_item_float::fill_data( flash_value& val )
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
{
	val.SetNumber( m_current_value );
}

void options_item_float::apply( )
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
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_float::call( flash_function_handler_params& params )
{
	m_current_value = ( float )params.pArgs[ 0 ].GetNumber( );
	params.pRetVal->SetNumber( m_current_value );
}

 options_item_bool::options_item_bool( options_tab& parent_tab, pcstr console_command, u8 option_item_id )
	: options_item_base( parent_tab, console_command, option_item_id, bool_selector )
{
}

void options_item_bool::initialize( )
{
	if ( m_console_command )
		m_source_value = ( ( console_commands::cc_value< bool >* )m_console_command )->get_value( );
	else
		m_source_value = false;

	m_current_value	= m_source_value;
}

void options_item_bool::fill_data( flash_value& __formal )
{
	VOSTOK_UNREFERENCED_PARAMETER( __formal );
}

void options_item_bool::fill_value( flash_value& val )
{
	val.SetBoolean( m_current_value );
}

void options_item_bool::apply( )
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
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_bool::call( flash_function_handler_params& params )
{
	params.pRetVal->SetBoolean( m_current_value = params.pArgs[ 0 ].GetBool( ) );
}

void options_gamma_selector::call( flash_function_handler_params& params )
{
	params.pRetVal->SetNumber( m_current_value = (float)params.pArgs[ 0 ].GetNumber( ) );
	m_parent_tab.get_game( ).active_scene( )->scene_renderer( ).set_gamma_correction_factor( m_current_value );
}

void options_gamma_selector::revert( )
{
	options_item_float::revert( );
	m_parent_tab.get_game( ).active_scene( )->scene_renderer( ).set_gamma_correction_factor( m_current_value );
}

 options_resolution_selector::options_resolution_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_resolution", 1, NULL, 0 )
{
	fill_resolutions( ( u8 )( ( console_commands::cc_u32* )console_commands::find( "r_monitor_index" ) )->get_value( ) );
}

void options_resolution_selector::fill_resolutions( u8 monitor_number )
{
	fixed_string< 32 > old_resolution;
	u32 old_resolution_index = u32( -1 );

	if ( m_values )
	{
		old_resolution = m_values[ m_current_value ];
		VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, m_values );
	}

	u8 resolutions_count = 0;

	for ( u32 i = 0; i < array_size( render::g_monitor_resolutions[ monitor_number ] ); ++i )
		if ( render::g_monitor_resolutions[ monitor_number ][ i ].y >= 720 && render::g_monitor_resolutions[ monitor_number ][ i ].x >= 1280 )
			m_cached_resolutions[ resolutions_count++ ].assignf(
				"%dx%d",
				render::g_monitor_resolutions[ monitor_number ][ i ].x,
				render::g_monitor_resolutions[ monitor_number ][ i ].y
			);

	m_values = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, pcstr, resolutions_count );
	m_values_count = resolutions_count;

	for ( u32 i = 0; i < resolutions_count; ++i )
	{
		m_values[ i ] = m_cached_resolutions[ i ].c_str( );

		if ( strings::equal( old_resolution.c_str( ), m_values[ i ] ) )
			old_resolution_index = i;
	}

	if ( !strings::equal( old_resolution.c_str( ), "" ) )
	{
		if ( old_resolution_index == u32( -1 ) )
		{
			m_current_value = m_values_count - 1;

			flash_value new_resolution_data[ 4 ];
			new_resolution_data[ 0 ].SetUInt( video_options_type );
			new_resolution_data[ 1 ].SetUInt( 1 );
			new_resolution_data[ 2 ].SetUInt( m_current_value );
			new_resolution_data[ 3 ].SetUInt( 0 );

			m_parent_tab.get_movie( )->movie->Invoke( "root.set_value", NULL, new_resolution_data, 4 );
		}
		else
			m_current_value = ( u8 )old_resolution_index;
	}
}

void options_resolution_selector::initialize( )
{
	pcstr current_resolution = ( ( console_commands::cc_string* )m_console_command )->get_value( );

	for ( u8 i = 0; i < m_values_count; ++i )
		if ( strings::equal( current_resolution, m_values[ i ] ) )
		{
			m_source_value = i;
			m_current_value = m_source_value;
			return;
		}

	m_current_value = m_source_value;
}

void options_resolution_selector::apply( )
{
	m_source_value = m_current_value;
	m_console_command->execute( m_values[ m_current_value ] );
}

 options_monitor_index_selector::options_monitor_index_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_monitor_index", 0, NULL, 0 )
{
	for ( u8 i = 0; i < array_size( m_cached_monitors_names ); ++i )
		m_cached_monitors_names[ i ].assignf( "%d", i );

	m_values		= VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, pcstr, render::g_num_monitors );
	m_values_count	= ( u8 )render::g_num_monitors;

	for ( u8 i = 0; i < render::g_num_monitors; ++i )
		m_values[ i ] = m_cached_monitors_names[ i ].c_str( );
}

void options_monitor_index_selector::call( flash_function_handler_params& params )
{
	options_item_int::call( params );
	refill_resolutions_data( );
}

void options_monitor_index_selector::revert( )
{
	options_item_int::revert( ); refill_resolutions_data( );
}

void options_monitor_index_selector::refill_resolutions_data( )
{
	( ( options_resolution_selector* )m_parent_tab.option_by_id( 1 ) )->fill_resolutions( m_current_value );
	m_parent_tab.get_game( ).get_game_options( ).refill_item_data( m_parent_tab.type( ), m_option_item_id );
}

// The PDB identifies this table but not its initializer strings.
static pcstr graphics_quality_data[ 6 ] =
{
	"st_options_minimum",
	"st_options_low",
	"st_options_medium",
	"st_options_high",
	"st_options_ultra",
	"st_options_custom"
};

 options_graphics_quality_selector::options_graphics_quality_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_graphics_quality", 8, graphics_quality_data, 6 )
{
}

void options_graphics_quality_selector::call( flash_function_handler_params& params )
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

static pcstr antialiasing_data[ 3 ]				= { "st_options_off", "st_options_fxaa", "st_options_2taa" };
static pcstr anisotrophic_filtering_data[ 5 ]	= { "st_options_off", "2x", "4x", "8x", "16x" };
static pcstr texture_quality_data[ 3 ]			= { "st_options_low", "st_options_medium", "st_options_high" };
static pcstr geometry_quality_data[ 2 ]			= { "st_options_medium", "st_options_high" };
static pcstr shadow_quality_data[ 4 ]			= { "st_options_low", "st_options_medium", "st_options_high", "st_options_ultra" };
static pcstr lighting_quality_data[ 4 ]			= { "st_options_low", "st_options_medium", "st_options_high", "st_options_ultra" };
static pcstr shading_quality_data[ 4 ]			= { "st_options_low", "st_options_medium", "st_options_high", "st_options_ultra" };
static pcstr decorations_quality_data[ 3 ]		= { "st_options_low", "st_options_medium", "st_options_high" };
static pcstr post_process_quality_data[ 4 ]		= { "st_options_minimum", "st_options_low", "st_options_medium", "st_options_high" };
static pcstr ambient_occlusion_data[ 4 ]		= { "st_options_off", "st_options_ssao", "st_options_hbao", "st_options_ssdo" };
static pcstr particles_quality_data[ 3 ]		= { "st_options_low", "st_options_medium", "st_options_high" };
static pcstr motion_blur_quality_data[ 4 ]		= { "st_options_off", "st_options_low", "st_options_medium", "st_options_high" };

 options_tab::options_tab( game& g, flash_movie_resource_ptr& movie, options_enum type )
	: m_type( type )
	, m_game( g )
	, m_movie( movie )
{
	switch ( type )
	{
		case gameplay_options_type:
		{

			m_options_count = 9;
			m_options = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, options_item_base*, 9 );

			m_options[ 0 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_invite_from_friends", 0 );
			m_options[ 1 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_friends_signin_notification", 1 );
			m_options[ 2 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_messages_censor", 2 );
			m_options[ 3 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_messages_only_from_friends", 3 );
			m_options[ 4 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_private_messages_in_game", 4 );
			m_options[ 5 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_hide_spam", 5 );
			m_options[ 6 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "g_crosshair_type", 6, NULL, 0 );
			m_options[ 7 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "g_crosshair_static", 7 );
			m_options[ 8 ] = VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "is_ui_minimap_rotable", 8 );
			break;
		}
		case video_options_type:
		{
			m_options_count = 19;
			m_options = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, options_item_base*, 19 );

			m_options[ 0 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_monitor_index_selector )( *this );
			m_options[ 1 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_resolution_selector )( *this );
			m_options[ 2 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "r_fullscreen", 2 );
			m_options[ 3 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "r_vsync", 3 );
			m_options[ 4 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_antialiasing_method", 4, antialiasing_data, 3 );
			m_options[ 5 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_max_anisotropic", 5, anisotrophic_filtering_data, 5 );
			m_options[ 6 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_gamma_selector )( *this );
			m_options[ 7 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "fov", 7, 1.0f );
			m_options[ 8 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_graphics_quality_selector )( *this );
			m_options[ 9 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_texture_quality", 9, texture_quality_data, 3 );
			m_options[ 10 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_geometry_quality", 10, geometry_quality_data, 2 );
			m_options[ 11 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_shadow_quality", 11, shadow_quality_data, 4 );
			m_options[ 12 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_lighting_quality", 12, lighting_quality_data, 4 );
			m_options[ 13 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_shading_quality", 13, shading_quality_data, 4 );
			m_options[ 14 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_decorations_quality", 14, decorations_quality_data, 3 );
			m_options[ 15 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_post_process_quality", 15, post_process_quality_data, 4 );
			m_options[ 16 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_ambient_occlusion_quality", 16, ambient_occlusion_data, 4 );
			m_options[ 17 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_particles_quality", 17, particles_quality_data, 3 );
			m_options[ 18 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_int )( *this, "r_motion_blur_quality", 18, motion_blur_quality_data, 4 );
			break;
		}
		case controllers_options_type:
		{
			m_options_count = 2;
			m_options = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, options_item_base*, 2 );

			m_options[ 0 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "mouse_invertion", 0 );
			m_options[ 1 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "sensitivity", 1, 0.05f );
			break;
		}


		case sound_options_type:
		{
			m_options_count = 7;
			m_options = VOSTOK_NEW_ARRAY_IMPL( ::survarium::g_allocator, options_item_base*, 7 );

			m_options[ 0 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "s_general_volume", 0, 1.0f );
			m_options[ 1 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "s_ingame_volume", 1, 1.0f );
			m_options[ 2 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "s_music_volume", 2, 1.0f );
			m_options[ 3 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "s_chat_volume", 3, 1.0f );
			m_options[ 4 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "s_use_microphone", 4 );
			m_options[ 5 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_float )( *this, "s_mic_sens", 5, 1.0f );
			m_options[ 6 ]	= VOSTOK_NEW_IMPL( ::survarium::g_allocator, options_item_bool )( *this, "s_ptt_button", 6 );
			break;
		}
	}
}

 options_tab::~options_tab( )
{
	for ( u8 i = 0; i < m_options_count; ++i )
		VOSTOK_DELETE_IMPL( ::survarium::g_allocator, m_options[ i ] );

	VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, m_options );
}

void options_tab::apply( flash_movie_resource_ptr& movie )
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
{
	VOSTOK_UNREFERENCED_PARAMETER( movie );

	for ( u8 i = 0; i < m_options_count; ++i )
		m_options[ i ]->revert( );
}

void options_tab::initialize_data( flash_movie_resource_ptr& movie )
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
		option_item_member.SetUInt( i );
		option_item.SetMember( "id", option_item_member );

		m_options[ i ]->fill_value( option_item_member );
		option_item.SetMember( "value", option_item_member );

		options_args[ 1 ].SetElement( i, option_item );
	}

	movie->movie->Invoke( "root.set_values", NULL, options_args, 3 );
}

} // namespace survarium
