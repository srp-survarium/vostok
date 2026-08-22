////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game.h"
#include "login_menu.h"
#include "login_menu_external_handler.h"
#include "base_network_client.h"
#include "ui_label.h"

#include <vostok/network/login_client.h>
#include <vostok/resources_queries_result.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>
#include <vostok/memory_extensions.h>

#include <GFx/GFx_Player.h>

namespace survarium {

static bool s_store_user_pass	= true;
static vostok::console_commands::cc_bool s_store_user_pass_cc	( "store_user_password", s_store_user_pass, true, vostok::console_commands::command_type_user_specific );

void login_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	if ( strings::equal( methodName, "sign_in_button_clicked" ) )
	{
		if ( m_login_menu.action_blocked( ) )
			return;

		network::login_client& login_client = m_game.network_client( ).login_client( );

		flash_value sign_in_button_enable;
		sign_in_button_enable.SetBoolean( false );
		pmovieView->SetVariable( "root.sign_in_btn.enabled", sign_in_button_enable );
		pmovieView->SetVariable( "root.status_str.text", "Connecting..." );

		pcstr account_name = args[ 0 ].GetString( );
		pcstr account_password = args[ 1 ].GetString( );
		m_game.network_client( ).connect_to_login(
			login_client.m_server_host,
			login_client.m_server_port,
			account_name,
			account_password
		);

		flash_value need_to_save_password;
		pmovieView->GetVariable( &need_to_save_password, "root.save_checkbox.selected" );
		s_store_user_pass = need_to_save_password.GetBool( );

		if ( s_store_user_pass )
			login_client.store_user_password_in_settings( );
		else
			login_client.reset_user_password_in_settings( );

		console_commands::save( "user.cfg", console_commands::command_type_user_specific, memory::g_mt_allocator );
	}
	else if ( strings::equal( methodName, "exit_button_clicked" ) )
	{
		m_game.engine( ).exit( 0 );
	}
}

void login_menu::on_resources_ready( resources::queries_result& data )
{
	m_render_scene = static_cast_resource_ptr< render::scene_ptr >( data[0].get_unmanaged_resource( ) );
	m_render_scene_view = static_cast_resource_ptr< render::scene_view_ptr >( data[1].get_unmanaged_resource( ) );

	m_login_menu_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[2].get_unmanaged_resource( ) );
	m_login_menu_ui->movie->SetBackgroundAlpha( 0.f );

	m_cursor_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( data[3].get_unmanaged_resource( ) );
	m_cursor_ui->movie->SetBackgroundAlpha( 0.f );

	m_login_menu_ui->movie->SetExternalInterface( NEW( login_menu_external_handler )( get_game( ), *this ) );

	m_login_menu_ui->movie->SetViewAlignment( flash_movie::Align_Center );
	m_login_menu_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );

	m_cursor_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_cursor_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );

	show_movie( m_login_menu_ui );
	show_movie( m_cursor_ui );

	// The target bypasses flash_movie's deliberately-NoScale wrapper here.
	m_login_menu_ui->movie->m_movie->SetViewScaleMode( Scaleform::GFx::Movie::SM_ExactFit );

	fill_labels( );

	flash_value v;
	v.SetBoolean( s_store_user_pass );
	m_login_menu_ui->movie->SetVariable( "root.save_checkbox.selected", v );

	get_game( ).on_queried_by_network_client_scene_ready( lobby_scene_ready );
}

// STATE[STUB]
// claude@NOTE: parked. Flash /Od wall now LIFTED (scaleform /Ox), but this body is
// still blocked on DATA, not glue: it loops 5x over a file-scope ui_label
// login_labels[] array (login_labels symbol not in our tree and the per-label id/text
// strings are not recoverable from the binary without fabrication), translate_text
// each into label_w, builds a flash_value per label and Movie::Invoke(
// "root.set_localization_data"). Unblock needs the login_labels string table (data
// table, unrecoverable - same wall as lobby_labels).
void login_menu::fill_labels( )
{
}

} // namespace survarium
