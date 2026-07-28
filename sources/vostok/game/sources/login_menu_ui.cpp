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
// claude@NOTE: parked. The flash /Od wall is now LIFTED (scaleform /Ox, /GL inlines
// SetVariable/GetVariable + the flash_value ctor/dtor), but this body is still empty -
// reconstructing it remains blocked on: (a) the login_client::sign_in call is a VTABLE
// dispatch (`[ebx+8]`) reading host/port members at login_client +0x134 / +0x174 plus a
// boost::function callback, not the non-virtual sign_in decl we have, so the exact
// vtable slot + member layout must be mapped first; (b) the exit branch calls
// `[edi+8]->[eax+7Ch]->vtbl[0]( 0 )` whose target is unconfirmed. Recovered body: if
// methodName == "sign_in_button_clicked" and !m_login_menu.action_blocked(): get the
// login_client via m_game.m_network_client->login_client(),
// SetVariable("root.sign_in_btn.enabled", flash_value(false)),
// SetVariable("root.status_str.text","Connecting..."), call
// login_client.sign_in(host,port,account,password,...), GetVariable(
// "root.save_checkbox.selected") -> s_store_user_pass, then
// store_user_password_in_settings / reset_user_password_in_settings +
// console_commands::save("user.cfg",...). else if "exit_button_clicked": exit.
// login_menu::action_blocked() is an empty inline stub here. Recover in a focused pass
// once the login_client vtable slots are mapped.
void login_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
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
