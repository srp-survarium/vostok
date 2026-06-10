////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"
#include <vostok/login_server/login_structures.h>

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_sign_out_password_written(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a1500]: 21
	// <0x7a1517>|0x017|+0x015:'19'
	// <0x7a152c>|0x02c|+0x010:'20'
	// <0x7a153c>|0x03c|+0x00d:'21'
	// <0x7a1549>|0x049|+0x010:'22'
	// <0x7a1559>|0x059|+0x0c9:'23'
	// <0x7a1622>|0x122|+0x005:'24'
	// <0>
	// <1>
	// <0x7a1627>|0x127|+0x006:'27'
	// <0x7a162d>|0x12d|+0x010:'28'
	// <0x7a163d>|0x13d|+0x00d:'29'
	// <0x7a164a>|0x14a|+0x010:'30'
	// <0x7a165a>|0x15a|+0x005:'31'
	// <0>
	// <1>
	// <0x7a165f>|0x15f|+0x078:'34'
	// <0>
	// <0x7a16d7>|0x1d7|+0x010:'36'
	// <0x7a16e7>|0x1e7|+0x00d:'37'
	// <0x7a16f4>|0x1f4|+0x011:'38'
	// <0x7a1705>|0x205|+0x010:'39'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_out_handshaked(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	const handshaking_error_types_enum		error
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error );

	// LOCALS
	// pbyte 							buffer
	// const u32 						password_length
	// ******

	// FUNCTION BODY[0x7a1720]: 27
	// <0x7a1737>|0x017|+0x006:'44'
	// <0x7a173d>|0x01d|+0x010:'45'
	// <0x7a174d>|0x02d|+0x00d:'46'
	// <0x7a175a>|0x03a|+0x011:'47'
	// <0x7a176b>|0x04b|+0x010:'48'
	// <0x7a177b>|0x05b|+0x005:'49'
	// <0>
	// <1>
	// <0x7a1780>|0x060|+0x077:'52'
	// <0>
	// <0x7a17f7>|0x0d7|+0x05a:'54'
	// <0x7a1851>|0x131|+0x00f:'55'
	// <0x7a1860>|0x140|+0x011:'56'
	// <0x7a1871>|0x151|+0x01d:'57'
	// <0x7a188e>|0x16e|+0x009:'58'
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
	// <0x7a1897>|0x177|+0x0ce:'70'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_out_written(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a1970]: 31
	// <0x7a198a>|0x01a|+0x00c:'75'
	// <0>
	// <0x7a1996>|0x026|+0x015:'77'
	// <0x7a19ab>|0x03b|+0x010:'78'
	// <0x7a19bb>|0x04b|+0x00d:'79'
	// <0x7a19c8>|0x058|+0x011:'80'
	// <0x7a19d9>|0x069|+0x010:'81'
	// <0x7a19e9>|0x079|+0x0de:'82'
	// <0x7a1ac7>|0x157|+0x005:'83'
	// <0>
	// <1>
	// <0x7a1acc>|0x15c|+0x006:'86'
	// <0x7a1ad2>|0x162|+0x010:'87'
	// <0x7a1ae2>|0x172|+0x00d:'88'
	// <0x7a1aef>|0x17f|+0x011:'89'
	// <0x7a1b00>|0x190|+0x010:'90'
	// <0x7a1b10>|0x1a0|+0x005:'91'
	// <0>
	// <1>
	// <0x7a1b15>|0x1a5|+0x080:'94'
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
	// <0x7a1b95>|0x225|+0x05a:'105'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_out_on_connected(
	connection_error_types_enum		connection_result,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &connection_result, &callback );

	// LOCALS
	// pbyte 							buffer
	// ******

	// FUNCTION BODY[0x7a1c00]: 29
	// <0x7a1c17>|0x017|+0x006:'110'
	// <0x7a1c1d>|0x01d|+0x010:'111'
	// <0x7a1c2d>|0x02d|+0x011:'112'
	// <0x7a1c3e>|0x03e|+0x010:'113'
	// <0x7a1c4e>|0x04e|+0x005:'114'
	// <0>
	// <1>
	// <0x7a1c53>|0x053|+0x077:'117'
	// <0>
	// <0x7a1cca>|0x0ca|+0x00f:'119'
	// <0>
	// <0x7a1cd9>|0x0d9|+0x00f:'121'
	// <0>
	// <0x7a1ce8>|0x0e8|+0x011:'123'
	// <0x7a1cf9>|0x0f9|+0x009:'124'
	// <0>
	// <0x7a1d02>|0x102|+0x010:'126'
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
	// <0x7a1d12>|0x112|+0x0cb:'138'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_out(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback );

	// LOCALS
	// boost::function< void( enum connection_error_types_enum ) > const& sign_out_functor
	// ******

	// FUNCTION BODY[0x7a1df0]: 20
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7a1e0a>|0x01a|+0x03f:'149'
	// <0>
	// <0x7a1e49>|0x059|+0x083:'151'
	// <0>
	// <0x7a1ecc>|0x0dc|+0x013:'153'
	// <0x7a1edf>|0x0ef|+0x083:'154'
	// <0x7a1f62>|0x172|+0x014:'155'
	// <0x7a1f76>|0x186|+0x00f:'156'
	// <0x7a1f85>|0x195|+0x013:'157'
	// <0x7a1f98>|0x1a8|+0x002:'158'
	// <0x7a1f9a>|0x1aa|+0x00a:'159'
	// <0>
	// <0x7a1fa4>|0x1b4|+0x005:'161'
	// <0x7a1fa9>|0x1b9|+0x08d:'162'
	// ******
}


} // namespace network
} // namespace vostok
