// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef LOGIN_MENU_STATUS_ENUM_H_INCLUDED
#define LOGIN_MENU_STATUS_ENUM_H_INCLUDED

namespace survarium {

enum login_menu_status_enum
{
	login_menu_status_error_connection		= 0x0,
	login_menu_status_invalid_user_or_password	= 0x1,
	login_menu_status_sign_in_attempt_interval_violated	= 0x2,
	login_menu_status_disconnected			= 0x3,
	login_menu_status_connected				= 0x4,
	login_menu_status_user_banned			= 0x5,
	login_menu_status_access_level_restriction	= 0x6,
	login_menu_status_sign_in_already_online	= 0x7,
	login_menu_status_invalid_version		= 0x8,
};

} // namespace survarium

#endif // #ifndef LOGIN_MENU_STATUS_ENUM_H_INCLUDED
