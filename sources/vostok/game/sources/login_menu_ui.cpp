////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game.h"
#include "login_menu.h"
#include "login_menu_external_handler.h"
#include "base_network_client.h"

#include <vostok/network/login_client.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>
#include <vostok/memory_extensions.h>

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

// STATE[STUB]
// claude@NOTE: parked. Flash /Od wall now LIFTED (scaleform /Ox); body still empty -
// builds the login menu UI movie + external handler from the loaded resources,
// SetVariable population, fill_labels(), create_network_client(). Remaining blockers:
// the resource_ptr / movie-resource construction surface and the loaded-resource
// member layout, plus fill_labels' login_labels table (below). Recover with on_resources_ready's resource surface.
void login_menu::on_resources_ready( resources::queries_result& data )
{
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
