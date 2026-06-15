////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_menu.h"
#include "login_menu_external_handler.h"

namespace survarium {

// TU static 's_store_user_pass_cc' (compiler-generated atexit destructor); a
// matcher recovers its type/initializer from the asm.
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_store_user_pass_cc''( )
{
	// FUNCTION BODY[0x7f0900]
	// <0x7d8f20>|0x000|      :'17'	{
	// ******
}
*/

// STATE[STUB]
void login_menu_external_handler::callback(
	flash_movie*			pmovieView,
	pcstr					methodName,
	flash_value const*		args,
	u32						argCount
)
{
	// LOCALS
	// flash_value 						sign_in_button_enable
	// flash_value 						need_to_save_password
	// ******

	// CALL SITE INFO
	// <0xa942a> -> network::login_client& < unknown >()
	// <0xa94b0> -> void < unknown >( pcstr, const u16, pcstr, pcstr )
	// <0xa9594> -> void < unknown >( int )
	// ******

	// FUNCTION BODY[0xa93c0]: 39
	// <0xa93c0>|0x000|+0x00b:'27'	{
	// <0>
	// <0xa93cb>|0x00b|+0x042:'29'
	// <0>
	// <0xa940d>|0x04d|+0x00f:'31'
	// <0>
	// <1>
	// <0xa941c>|0x05c|+0x010:'34'
	// <0>
	// <1>
	// <2>
	// <0xa942c>|0x06c|+0x029:'38'
	// <0>
	// <0xa9455>|0x095|+0x010:'40'
	// <0>
	// <0xa9465>|0x0a5|+0x018:'42'
	// <0xa947d>|0x0bd|+0x010:'43'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0xa948d>|0x0cd|+0x025:'49'
	// <0>
	// <1>
	// <0xa94b2>|0x0f2|+0x01c:'52'
	// <0xa94ce>|0x10e|+0x009:'53'
	// <0>
	// <1>
	// <0xa94d7>|0x117|+0x00b:'56'
	// <0xa94e2>|0x122|+0x002:'57'
	// <0xa94e4>|0x124|+0x005:'58'
	// <0>
	// <1>
	// <0xa94e9>|0x129|+0x019:'61'
	// <0xa9502>|0x142|+0x052:'62'
	// <0xa9554>|0x194|+0x035:'63'
	// <0>
	// <0xa9589>|0x1c9|-0x03e:'65'
	// <0>
	// <0xa954b>|0x18b|+0x04b:'67'
	// <0xa9596>|0x1d6|      :'67'	}
	// ******
}

// STATE[STUB]
void login_menu::on_resources_ready( resources::queries_result& data )
{
	// LOCALS
	// flash_value 						v
	// ******

	// FUNCTION BODY[0x7a4a20]: 29
	// <0x7a4a29>|0x009|+0x07c:'72'
	// <0x7a4aa5>|0x085|+0x06f:'73'
	// <0>
	// <0x7a4b14>|0x0f4|+0x0ad:'75'
	// <0x7a4bc1>|0x1a1|+0x01b:'76'
	// <0>
	// <0x7a4bdc>|0x1bc|+0x0aa:'78'
	// <0x7a4c86>|0x266|+0x023:'79'
	// <0>
	// <0x7a4ca9>|0x289|+0x052:'81'
	// <0>
	// <0x7a4cfb>|0x2db|+0x014:'83'
	// <0x7a4d0f>|0x2ef|+0x014:'84'
	// <0>
	// <0x7a4d23>|0x303|+0x018:'86'
	// <0x7a4d3b>|0x31b|+0x018:'87'
	// <0>
	// <0x7a4d53>|0x333|+0x008:'89'
	// <0x7a4d5b>|0x33b|+0x010:'90'
	// <0>
	// <0x7a4d6b>|0x34b|+0x017:'92'
	// <0>
	// <0x7a4d82>|0x362|+0x006:'94'
	// <0>
	// <1>
	// <0x7a4d88>|0x368|+0x006:'97'
	// <0x7a4d8e>|0x36e|+0x030:'98'
	// <0>
	// <0x7a4dbe>|0x39e|+0x01e:'100'
	// ******
}

// STATE[STUB]
void login_menu::fill_labels( )
{
	// LOCALS
	// flash_value 						labels
	// wchar_t[512] 					label_w
	// flash_value 						label_translate
	// ******

	// FUNCTION BODY[0x7a48a0]: 19
	// <0>
	// <0x7a48af>|0x00f|+0x035:'122'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x7a48e4>|0x044|+0x020:'132'
	// <0>
	// <0x7a4904>|0x064|+0x068:'134'
	// <0>
	// <0x7a496c>|0x0cc|+0x02a:'136'
	// <0x7a4996>|0x0f6|+0x02e:'137'
	// <0>
	// <0x7a49c4>|0x124|+0x021:'139'
	// ******
}

} // namespace survarium
