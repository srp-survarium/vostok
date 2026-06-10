////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"
#include <vostok/login_server/login_structures.h>

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_sign_up_answer_received(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
	sign_up_info const&					sign_up_info,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &sign_up_info, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a3650]: 21
	// <0x7a3667>|0x017|+0x015:'21'
	// <0x7a367c>|0x02c|+0x010:'22'
	// <0x7a368c>|0x03c|+0x00d:'23'
	// <0x7a3699>|0x049|+0x011:'24'
	// <0x7a36aa>|0x05a|+0x051:'25'
	// <0x7a36fb>|0x0ab|+0x0c9:'26'
	// <0x7a37c4>|0x174|+0x005:'27'
	// <0>
	// <1>
	// <0x7a37c9>|0x179|+0x00a:'30'
	// <0x7a37d3>|0x183|+0x010:'31'
	// <0x7a37e3>|0x193|+0x00d:'32'
	// <0x7a37f0>|0x1a0|+0x011:'33'
	// <0x7a3801>|0x1b1|+0x051:'34'
	// <0x7a3852>|0x202|+0x005:'35'
	// <0>
	// <1>
	// <0x7a3857>|0x207|+0x010:'38'
	// <0x7a3867>|0x217|+0x00d:'39'
	// <0x7a3874>|0x224|+0x011:'40'
	// <0x7a3885>|0x235|+0x069:'41'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_up_info_written(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
	sign_up_info const&					sign_up_info,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &sign_up_info, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a3900]: 30
	// <0x7a391a>|0x01a|+0x00c:'51'
	// <0>
	// <0x7a3926>|0x026|+0x015:'53'
	// <0x7a393b>|0x03b|+0x010:'54'
	// <0x7a394b>|0x04b|+0x00d:'55'
	// <0x7a3958>|0x058|+0x011:'56'
	// <0x7a3969>|0x069|+0x051:'57'
	// <0x7a39ba>|0x0ba|+0x0da:'58'
	// <0x7a3a94>|0x194|+0x005:'59'
	// <0>
	// <1>
	// <0x7a3a99>|0x199|+0x00a:'62'
	// <0x7a3aa3>|0x1a3|+0x010:'63'
	// <0x7a3ab3>|0x1b3|+0x00d:'64'
	// <0x7a3ac0>|0x1c0|+0x011:'65'
	// <0x7a3ad1>|0x1d1|+0x051:'66'
	// <0x7a3b22>|0x222|+0x005:'67'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x7a3b27>|0x227|+0x0cb:'80'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_up_on_handshaked(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
	sign_up_info const&						sign_up_info,
	const handshaking_error_types_enum		handshaking_result
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &sign_up_info, &handshaking_result );

	// LOCALS
	// pbyte 							buffer
	// const u8 						email_length
	// const u8 						password_length
	// ******

	// FUNCTION BODY[0x7a3c00]: 34
	// <0x7a3c10>|0x010|+0x00a:'89'
	// <0x7a3c1a>|0x01a|+0x010:'90'
	// <0x7a3c2a>|0x02a|+0x00d:'91'
	// <0x7a3c37>|0x037|+0x011:'92'
	// <0x7a3c48>|0x048|+0x056:'93'
	// <0x7a3c9e>|0x09e|+0x005:'94'
	// <0>
	// <1>
	// <0x7a3ca3>|0x0a3|+0x00f:'97'
	// <0>
	// <0x7a3cb2>|0x0b2|+0x054:'99'
	// <0x7a3d06>|0x106|+0x00c:'100'
	// <0x7a3d12>|0x112|+0x011:'101'
	// <0x7a3d23>|0x123|+0x018:'102'
	// <0x7a3d3b>|0x13b|+0x00a:'103'
	// <0>
	// <0x7a3d45>|0x145|+0x054:'105'
	// <0x7a3d99>|0x199|+0x00c:'106'
	// <0x7a3da5>|0x1a5|+0x011:'107'
	// <0x7a3db6>|0x1b6|+0x018:'108'
	// <0x7a3dce>|0x1ce|+0x00a:'109'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x7a3dd8>|0x1d8|+0x0e0:'122'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_up_account_answer_received(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
	sign_up_info const&					sign_up_info,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &sign_up_info, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a3ec0]: 48
	// <0x7a3ed7>|0x017|+0x015:'132'
	// <0x7a3eec>|0x02c|+0x010:'133'
	// <0x7a3efc>|0x03c|+0x00d:'134'
	// <0x7a3f09>|0x049|+0x011:'135'
	// <0x7a3f1a>|0x05a|+0x051:'136'
	// <0x7a3f6b>|0x0ab|+0x0cc:'137'
	// <0x7a4037>|0x177|+0x005:'138'
	// <0>
	// <1>
	// <0x7a403c>|0x17c|+0x00a:'141'
	// <0x7a4046>|0x186|+0x010:'142'
	// <0x7a4056>|0x196|+0x00d:'143'
	// <0x7a4063>|0x1a3|+0x011:'144'
	// <0x7a4074>|0x1b4|+0x051:'145'
	// <0x7a40c5>|0x205|+0x005:'146'
	// <0>
	// <1>
	// <0x7a40ca>|0x20a|+0x02d:'149'
	// <0>
	// <0x7a40f7>|0x237|+0x010:'151'
	// <0x7a4107>|0x247|+0x00d:'152'
	// <0x7a4114>|0x254|+0x011:'153'
	// <0x7a4125>|0x265|+0x069:'154'
	// <0x7a418e>|0x2ce|+0x005:'155'
	// <0>
	// <1>
	// <0x7a4193>|0x2d3|+0x005:'158'
	// <0>
	// <1>
	// <0x7a4198>|0x2d8|+0x010:'161'
	// <0x7a41a8>|0x2e8|+0x00d:'162'
	// <0x7a41b5>|0x2f5|+0x011:'163'
	// <0x7a41c6>|0x306|+0x069:'164'
	// <0x7a422f>|0x36f|+0x002:'165'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x7a4231>|0x371|+0x06d:'179'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_up_written(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
	sign_up_info const&					sign_up_info,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &sign_up_info, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a42b0]: 30
	// <0x7a42ca>|0x01a|+0x00c:'189'
	// <0>
	// <0x7a42d6>|0x026|+0x015:'191'
	// <0x7a42eb>|0x03b|+0x010:'192'
	// <0x7a42fb>|0x04b|+0x00d:'193'
	// <0x7a4308>|0x058|+0x011:'194'
	// <0x7a4319>|0x069|+0x051:'195'
	// <0x7a436a>|0x0ba|+0x0e0:'196'
	// <0x7a444a>|0x19a|+0x005:'197'
	// <0>
	// <1>
	// <0x7a444f>|0x19f|+0x00a:'200'
	// <0x7a4459>|0x1a9|+0x010:'201'
	// <0x7a4469>|0x1b9|+0x00d:'202'
	// <0x7a4476>|0x1c6|+0x011:'203'
	// <0x7a4487>|0x1d7|+0x051:'204'
	// <0x7a44d8>|0x228|+0x005:'205'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x7a44dd>|0x22d|+0x0c8:'218'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_up_on_connected(
	const connection_error_types_enum		connection_result,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback,
	sign_up_info const&						sign_up_info
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &connection_result, &callback, &sign_up_info );

	// LOCALS
	// pbyte 							buffer
	// const u8 						account_name_length
	// ******

	// FUNCTION BODY[0x7a45b0]: 31
	// <0x7a45c0>|0x010|+0x006:'227'
	// <0x7a45c6>|0x016|+0x010:'228'
	// <0x7a45d6>|0x026|+0x011:'229'
	// <0x7a45e7>|0x037|+0x057:'230'
	// <0x7a463e>|0x08e|+0x005:'231'
	// <0>
	// <1>
	// <0x7a4643>|0x093|+0x010:'234'
	// <0>
	// <0x7a4653>|0x0a3|+0x00f:'236'
	// <0>
	// <0x7a4662>|0x0b2|+0x00f:'238'
	// <0>
	// <0x7a4671>|0x0c1|+0x051:'240'
	// <0x7a46c2>|0x112|+0x00c:'241'
	// <0x7a46ce>|0x11e|+0x011:'242'
	// <0x7a46df>|0x12f|+0x015:'243'
	// <0x7a46f4>|0x144|+0x00a:'244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x7a46fe>|0x14e|+0x0dd:'257'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_up(
	pcstr const				host,
	const u16				port,
	sign_up_info const&		sign_up_info,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, vostok::sign_up_info const& ) > const&	callback
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &host, &port, &sign_up_info, &callback );

	// FUNCTION BODY[0x7a47f0]: 15
	// <0x7a47fa>|0x00a|+0x00e:'262'
	// <0x7a4808>|0x018|+0x019:'263'
	// <0x7a4821>|0x031|+0x00a:'264'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x7a482b>|0x03b|+0x064:'276'
	// ******
}


} // namespace network
} // namespace vostok
