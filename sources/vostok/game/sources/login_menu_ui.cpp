////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_menu.h"
#include "login_menu_external_handler.h"

#include <vostok/console_command.h>

namespace survarium {

static bool s_store_user_pass	= true;
static vostok::console_commands::cc_bool s_store_user_pass_cc	( "store_user_password", s_store_user_pass, true, vostok::console_commands::command_type_user_specific );

// STATE[STUB]
// claude@NOTE: parked - flash-glue /Od wall. Recovered body: if methodName ==
// "sign_in_button_clicked" and !m_login_menu.action_blocked(): get the login_client
// via m_game.m_network_client->login_client(), SetVariable("root.sign_in_btn.enabled",
// flash_value(false)), SetVariable("root.status_str.text","Connecting..."), call
// login_client.sign_in(host,port,account,password,...), GetVariable(
// "root.save_checkbox.selected") -> s_store_user_pass, then store_user_password_in_settings
// / reset_user_password_in_settings + console_commands::save("user.cfg",...). else if
// "exit_button_clicked": exit. The flash_value ctor/dtor + Movie::SetVariable/GetVariable
// inline in the target (whole-program) but emit out-of-line calls in our /Od scaleform
// TU (same wall as scaleform/value.cpp glue, 14-34%), and login_menu::action_blocked()
// is an empty inline stub here. Unblock once the scaleform glue is matched in an
// optimized TU and the menu accessors are recovered.
void login_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
}

// STATE[STUB]
// claude@NOTE: parked - flash-glue /Od wall (see callback above). Builds the login
// menu UI movie + external handler from the loaded resources, SetVariable population,
// fill_labels(), create_network_client(). Heavy flash_value glue + resource_ptr/sound
// inlining; out-of-line in our /Od build vs whole-program-inlined target.
void login_menu::on_resources_ready( resources::queries_result& data )
{
}

// STATE[STUB]
// claude@NOTE: parked - flash-glue /Od wall (see callback above). Loops 5x over a
// file-scope ui_label login_labels[] array (login_labels symbol not in our tree and
// the per-label id/text strings are not recoverable from the binary without
// fabrication), translate_text each into label_w, builds a flash_value per label and
// Movie::Invoke("root.set_localization_data"). flash_value ctor/dtor + CreateObject/
// Invoke inline in target, out-of-line here. Unblock needs the login_labels data +
// optimized scaleform glue.
void login_menu::fill_labels( )
{
}

} // namespace survarium
