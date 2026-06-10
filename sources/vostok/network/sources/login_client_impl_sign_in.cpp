////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"
#include <vostok/login_server/login_structures.h>

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_sign_in_answer_received(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error_code, &bytes_transferred );

	// LOCALS
	// pbyte 							buffer
	// boost::asio::ip::basic_resolver_query< boost::asio::ip::udp > query<1>
	// boost::asio::ip::basic_resolver_iterator< boost::asio::ip::udp > iterator<1>
	// u8 								length1<1>
	// boost::asio::ip::basic_resolver< boost::asio::ip::udp, boost::asio::ip::resolver_service< boost::asio::ip::udp > > resolver<1>
	// u8 								length2<1>
	// char[6] 							port<1>
	// ******

	// FUNCTION BODY[0x7a2050]: 63
	// <0x7a206b>|0x01b|+0x012:'20'
	// <0>
	// <0x7a207d>|0x02d|+0x01b:'22'
	// <0x7a2098>|0x048|+0x010:'23'
	// <0x7a20a8>|0x058|+0x00d:'24'
	// <0x7a20b5>|0x065|+0x015:'25'
	// <0x7a20ca>|0x07a|+0x0f0:'26'
	// <0>
	// <0x7a21ba>|0x16a|+0x011:'28'
	// <0x7a21cb>|0x17b|+0x010:'29'
	// <0x7a21db>|0x18b|+0x005:'30'
	// <0>
	// <1>
	// <0x7a21e0>|0x190|+0x089:'33'
	// <0x7a2269>|0x219|+0x00f:'34'
	// <0x7a2278>|0x228|+0x019:'35'
	// <0>
	// <0x7a2291>|0x241|+0x009|[1]:'37'
	// <0>
	// <0x7a229a>|0x24a|+0x011:'39'
	// <0x7a22ab>|0x25b|+0x00d:'40'
	// <0x7a22b8>|0x268|+0x00d:'41'
	// <0>
	// <1>
	// <0x7a22c5>|0x275|+0x01e:'44'
	// <0x7a22e3>|0x293|+0x00a:'45'
	// <0x7a22ed>|0x29d|+0x012:'46'
	// <0>
	// <0x7a22ff>|0x2af|+0x011:'48'
	// <0x7a2310>|0x2c0|+0x01e:'49'
	// <0x7a232e>|0x2de|+0x00a:'50'
	// <0x7a2338>|0x2e8|+0x012:'51'
	// <0>
	// <0x7a234a>|0x2fa|+0x011:'53'
	// <0x7a235b>|0x30b|+0x009:'54'
	// <0>
	// <1>
	// <0x7a2364>|0x314|+0x015:'57'
	// <0>
	// <0x7a2379>|0x329|+0x021:'59'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7a239a>|0x34a|+0x146:'64'
	// <0x7a24e0>|0x490|+0x010:'65'
	// <0x7a24f0>|0x4a0|+0x054:'66'
	// <0>
	// <0x7a2544>|0x4f4|+0x010:'68'
	// <0x7a2554>|0x504|+0x00d:'69'
	// <0x7a2561>|0x511|+0x011:'70'
	// <0x7a2572>|0x522|+0x010:'71'
	// <0>
	// <0x7a2582>|0x532|+0x075:'73'
	// <0x7a25f7>|0x5a7|+0x03b:'74'
	// <0>
	// <1>
	// <0x7a2632>|0x5e2|+0x010:'77'
	// <0x7a2642>|0x5f2|+0x011:'78'
	// <0x7a2653>|0x603|+0x015:'79'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_in_password_written(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error_code, &bytes_transferred );

	// LOCALS
	// const u32 						buffer_size
	// ******

	// FUNCTION BODY[0x7a2670]: 33
	// <0x7a268a>|0x01a|+0x015:'91'
	// <0x7a269f>|0x02f|+0x010:'92'
	// <0x7a26af>|0x03f|+0x00d:'93'
	// <0x7a26bc>|0x04c|+0x011:'94'
	// <0x7a26cd>|0x05d|+0x010:'95'
	// <0x7a26dd>|0x06d|+0x0da:'96'
	// <0x7a27b7>|0x147|+0x005:'97'
	// <0>
	// <1>
	// <0x7a27bc>|0x14c|+0x006:'100'
	// <0x7a27c2>|0x152|+0x010:'101'
	// <0x7a27d2>|0x162|+0x00d:'102'
	// <0x7a27df>|0x16f|+0x011:'103'
	// <0x7a27f0>|0x180|+0x010:'104'
	// <0x7a2800>|0x190|+0x005:'105'
	// <0>
	// <1>
	// <0x7a2805>|0x195|+0x080:'108'
	// <0x7a2885>|0x215|+0x080:'109'
	// <0x7a2905>|0x295|+0x007:'110'
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
	// <0x7a290c>|0x29c|+0x0be:'123'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_in_handshaked(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	const handshaking_error_types_enum		error
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error );

	// LOCALS
	// pbyte 							buffer
	// const u32 						password_length
	// ******

	// FUNCTION BODY[0x7a29e0]: 26
	// <0x7a29f7>|0x017|+0x006:'128'
	// <0x7a29fd>|0x01d|+0x010:'129'
	// <0x7a2a0d>|0x02d|+0x00d:'130'
	// <0x7a2a1a>|0x03a|+0x011:'131'
	// <0x7a2a2b>|0x04b|+0x010:'132'
	// <0x7a2a3b>|0x05b|+0x005:'133'
	// <0>
	// <1>
	// <0x7a2a40>|0x060|+0x07a:'136'
	// <0x7a2aba>|0x0da|+0x05a:'137'
	// <0x7a2b14>|0x134|+0x00f:'138'
	// <0x7a2b23>|0x143|+0x011:'139'
	// <0x7a2b34>|0x154|+0x01d:'140'
	// <0x7a2b51>|0x171|+0x009:'141'
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
	// <0x7a2b5a>|0x17a|+0x0ce:'153'
	// ******
}

// STATE[STUB]
void login_client_impl::on_user_name_answer_received(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a2c30]: 47
	// <0x7a2c47>|0x017|+0x00c:'158'
	// <0>
	// <0x7a2c53>|0x023|+0x015:'160'
	// <0x7a2c68>|0x038|+0x010:'161'
	// <0x7a2c78>|0x048|+0x00d:'162'
	// <0x7a2c85>|0x055|+0x011:'163'
	// <0x7a2c96>|0x066|+0x010:'164'
	// <0x7a2ca6>|0x076|+0x0cc:'165'
	// <0x7a2d72>|0x142|+0x005:'166'
	// <0>
	// <1>
	// <0x7a2d77>|0x147|+0x006:'169'
	// <0x7a2d7d>|0x14d|+0x010:'170'
	// <0x7a2d8d>|0x15d|+0x00d:'171'
	// <0x7a2d9a>|0x16a|+0x011:'172'
	// <0x7a2dab>|0x17b|+0x010:'173'
	// <0x7a2dbb>|0x18b|+0x005:'174'
	// <0>
	// <1>
	// <0x7a2dc0>|0x190|+0x026:'177'
	// <0>
	// <0x7a2de6>|0x1b6|+0x010:'179'
	// <0x7a2df6>|0x1c6|+0x00d:'180'
	// <0x7a2e03>|0x1d3|+0x011:'181'
	// <0x7a2e14>|0x1e4|+0x01c:'182'
	// <0x7a2e30>|0x200|+0x005:'183'
	// <0>
	// <0x7a2e35>|0x205|+0x002:'185'
	// <0>
	// <0x7a2e37>|0x207|+0x010:'187'
	// <0x7a2e47>|0x217|+0x00d:'188'
	// <0x7a2e54>|0x224|+0x011:'189'
	// <0x7a2e65>|0x235|+0x01c:'190'
	// <0x7a2e81>|0x251|+0x002:'191'
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
	// <0x7a2e83>|0x253|+0x05a:'204'
	// ******
}

// STATE[STUB]
void login_client_impl::on_sign_in_written(
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &callback, &error_code, &bytes_transferred );

	// FUNCTION BODY[0x7a2ef0]: 30
	// <0x7a2f0a>|0x01a|+0x00c:'209'
	// <0>
	// <0x7a2f16>|0x026|+0x015:'211'
	// <0x7a2f2b>|0x03b|+0x010:'212'
	// <0x7a2f3b>|0x04b|+0x00d:'213'
	// <0x7a2f48>|0x058|+0x011:'214'
	// <0x7a2f59>|0x069|+0x010:'215'
	// <0x7a2f69>|0x079|+0x0e1:'216'
	// <0x7a304a>|0x15a|+0x005:'217'
	// <0>
	// <1>
	// <0x7a304f>|0x15f|+0x006:'220'
	// <0x7a3055>|0x165|+0x010:'221'
	// <0x7a3065>|0x175|+0x00d:'222'
	// <0x7a3072>|0x182|+0x011:'223'
	// <0x7a3083>|0x193|+0x010:'224'
	// <0x7a3093>|0x1a3|+0x005:'225'
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
	// <0x7a3098>|0x1a8|+0x0b0:'238'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_in_on_connected(
	connection_error_types_enum		connection_result,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &connection_result, &callback );

	// LOCALS
	// pbyte 							buffer
	// char[8] 							version
	// const u8 						account_name_length
	// ******

	// FUNCTION BODY[0x7a3150]: 36
	// <0x7a3160>|0x010|+0x006:'243'
	// <0x7a3166>|0x016|+0x010:'244'
	// <0x7a3176>|0x026|+0x011:'245'
	// <0x7a3187>|0x037|+0x010:'246'
	// <0x7a3197>|0x047|+0x005:'247'
	// <0>
	// <1>
	// <0x7a319c>|0x04c|+0x00e:'250'
	// <0>
	// <0x7a31aa>|0x05a|+0x00f:'252'
	// <0>
	// <0x7a31b9>|0x069|+0x059:'254'
	// <0x7a3212>|0x0c2|+0x00c:'255'
	// <0x7a321e>|0x0ce|+0x011:'256'
	// <0x7a322f>|0x0df|+0x01e:'257'
	// <0x7a324d>|0x0fd|+0x00a:'258'
	// <0>
	// <1>
	// <0x7a3257>|0x107|+0x008:'261'
	// <0x7a325f>|0x10f|+0x012:'262'
	// <0x7a3271>|0x121|+0x00e:'263'
	// <0x7a327f>|0x12f|+0x009:'264'
	// <0>
	// <0x7a3288>|0x138|+0x010:'266'
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
	// <0x7a3298>|0x148|+0x0cc:'278'
	// ******
}

// STATE[STUB]
void login_client_impl::sign_in(
	pcstr const		host,
	const u16		port,
	pcstr const		account_name,
	pcstr const		password,
	boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &host, &port, &account_name, &password, &callback );

	// LOCALS
	// boost::function< void( enum connection_error_types_enum ) > const& sign_in_functor
	// ******

	// FUNCTION BODY[0x7a3370]: 28
	// <0x7a338a>|0x01a|+0x093:'283'
	// <0>
	// <0x7a341d>|0x0ad|+0x011:'285'
	// <0x7a342e>|0x0be|+0x01c:'286'
	// <0x7a344a>|0x0da|+0x00d:'287'
	// <0>
	// <0x7a3457>|0x0e7|+0x01c:'289'
	// <0x7a3473>|0x103|+0x00d:'290'
	// <0>
	// <0x7a3480>|0x110|+0x01c:'292'
	// <0x7a349c>|0x12c|+0x00d:'293'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x7a34a9>|0x139|+0x03f:'301'
	// <0>
	// <0x7a34e8>|0x178|+0x084:'303'
	// <0>
	// <0x7a356c>|0x1fc|+0x00f:'305'
	// <0x7a357b>|0x20b|+0x018:'306'
	// <0x7a3593>|0x223|+0x00f:'307'
	// <0x7a35a2>|0x232|+0x00a:'308'
	// <0x7a35ac>|0x23c|+0x005:'309'
	// <0x7a35b1>|0x241|+0x08c:'310'
	// ******
}


} // namespace network
} // namespace vostok
