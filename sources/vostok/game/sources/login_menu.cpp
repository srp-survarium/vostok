////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_menu.h"

#include "game.h"
#include "game_memory.h"
#include "base_network_client.h"
#include "camera_director.h"

#include <vostok/network/login_client.h>
#include <vostok/input/world.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/math_constants.h>
#include <vostok/resources.h>
#include <vostok/render/facade/common_types.h>

#include <boost/bind.hpp>

namespace survarium {

// claude@NOTE: structures match; the byte residual on on_activate/on_deactivate/tick/
// clear_resources/enable_button is a cross-module cap - base_game_scene::on_activate/
// on_deactivate/tick/hide_movie are still STUBs (so the inherited calls inline to nothing
// here instead of staying real calls), scaleform flash_movie::Advance/SetVariable +
// flash_value ctor/dtor are still STUBs (so the inlined movie ops differ), and the game
// vtable layout shifts input_world()'s slot. They lift when those siblings match.

 login_menu::login_menu( game& g )
	: base_game_scene( g )
	, m_block_btn_time( 0 )
	, m_status( login_menu_status_connected )
{
	query_resources( );
}

 login_menu::~login_menu( )
{
}

void login_menu::on_activate( )
{
	base_game_scene::on_activate( );
	get_game( ).input_world( ).add_handler( *this );
}

void login_menu::on_deactivate( )
{
	base_game_scene::on_deactivate( );
	get_game( ).input_world( ).remove_handler( *this );
}

void login_menu::tick(
	const u32		frame_delta_in_ms,
	const u32		current_time_in_ms,
	const bool		is_game_paused
)
{
	base_game_scene::tick( frame_delta_in_ms, current_time_in_ms, is_game_paused );

	if ( m_block_btn_time && ( m_block_btn_time < current_time_in_ms ) ) {
		m_block_btn_time = 0;
		enable_button( true );
	}

	float deltaTime = frame_delta_in_ms * math::epsilon_3;
	m_login_menu_ui->movie->Advance( deltaTime, 0 );
	m_cursor_ui->movie->Advance( deltaTime, 0 );
}








void login_menu::query_resources( )
{
	render::scene_configuration	render_configuration;

	resources::user_data_variant temp_data;
	temp_data.set										( render_configuration );

	resources::user_data_variant const* data[]			=
	{
		&temp_data,
		NULL,
		NULL,
		NULL
	};

	resources::request requests[] =
	{
		{ "game_scene",										resources::scene_class },
		{ "game_scene_view",								resources::scene_view_class },
		{ "resources/flash_movies/login_menu.swf",			resources::flash_movie_class },
		{ "resources/flash_movies/cursor.swf",				resources::flash_movie_class }
	};

	resources::query_resources							(
		requests,
		boost::bind( &login_menu::on_resources_ready, this, _1 ),
		g_allocator,
		data
	);
}

void login_menu::clear_resources( )
{
	hide_movie( m_login_menu_ui );
	hide_movie( m_cursor_ui );
}

void login_menu::set_status( login_menu_status_enum status )
{
	m_status				= status;

	network::login_client&	login_client	= get_game( )

		.network_client( ).login_client( );

	fixed_string< 512 >		status_str;
	status_str.assignf( "Login Server: " );

	switch ( m_status ) {
		case login_menu_status_connected:
			status_str.append( "Connected" );
			break;

		case login_menu_status_disconnected:
			status_str.append( "Disconnected" );
			m_block_btn_time	= get_game( ).game_time_ms( ) + 5000;
			break;

		case login_menu_status_error_connection:
			status_str.append( "Connection error" );
			m_block_btn_time	= get_game( ).game_time_ms( ) + 5000;
			break;

		case login_menu_status_invalid_user_or_password:
			status_str.append( "Invalid user name or password" );
			m_block_btn_time	= get_game( ).game_time_ms( ) + 5000;
			break;

		case login_menu_status_sign_in_attempt_interval_violated:
			status_str.append( "Sign in attempt interval violated" );
			m_block_btn_time	= get_game( ).game_time_ms( ) + 10000;
			break;

		case login_menu_status_user_banned:
			status_str.append( "User banned" );
			break;

		case login_menu_status_access_level_restriction:
			status_str.append( "Access level restriction" );
			break;

		case login_menu_status_sign_in_already_online:
			status_str.append( "Connecting..." );
			m_block_btn_time	= get_game( ).game_time_ms( ) + 5000;
			break;

		case login_menu_status_invalid_version:
			status_str.append( "Invalid version" );
			break;
		default:
			NODEFAULT( );
	}

	enable_button( false );

	m_login_menu_ui->movie->SetVariable( "root.status_str.text",		status_str.c_str( ) );
	m_login_menu_ui->movie->SetVariable( "root.login_input.text",		login_client.account_name( ) );
	m_login_menu_ui->movie->SetVariable( "root.password_input.text",	login_client.account_password( ) );
}

void login_menu::enable_button( bool value )
{
	flash_value	sign_in_button_enable;
	sign_in_button_enable.SetBoolean( value );
	m_login_menu_ui->movie->SetVariable( "root.sign_in_btn.enabled", sign_in_button_enable );
}

} // namespace survarium
