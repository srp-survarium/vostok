////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// the original compiland really is messaging_client_process_messagess.cpp
// (sic, PDB-attested typo) - keep the filename

#include "pch.h"
#include "messaging_client.h"

namespace survarium {

// STATE[STUB]
void messaging_client::on_packet_received( network_core::packet_reader& reader )
{
	// FUNCTION BODY[0x5c9780]: 77
	// <0x5c9780>|0x000|+0x00e:'20'	{
	// <0x5c978e>|0x00e|+0x00b:'21'
	// <0x5c9799>|0x019|+0x452:'22'
	// <0>
	// <1>
	// <2>
	// <0x5c9beb>|0x46b|-0x355:'26'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c9896>|0x116|+0x009:'31'
	// <0>
	// <0x5c989f>|0x11f|+0x005:'33'
	// <0>
	// <0x5c98a4>|0x124|+0x006:'35'
	// <0x5c98aa>|0x12a|+0x005:'36'
	// <0x5c98af>|0x12f|+0x005:'37'
	// <0>
	// <0x5c98b4>|0x134|+0x007:'39'
	// <0x5c98bb>|0x13b|+0x005:'40'
	// <0x5c98c0>|0x140|+0x005:'41'
	// <0>
	// <0x5c98c5>|0x145|+0x006:'43'
	// <0x5c98cb>|0x14b|+0x005:'44'
	// <0x5c98d0>|0x150|+0x005:'45'
	// <0>
	// <0x5c98d5>|0x155|+0x006:'47'
	// <0x5c98db>|0x15b|+0x005:'48'
	// <0>
	// <1>
	// <0x5c98e0>|0x160|+0x00e:'51'
	// <0x5c98ee>|0x16e|+0x005:'52'
	// <0>
	// <0x5c98f3>|0x173|+0x007:'54'
	// <0x5c98fa>|0x17a|+0x005:'55'
	// <0x5c98ff>|0x17f|+0x09c:'56'
	// <0>
	// <0x5c999b>|0x21b|+0x005:'58'
	// <0x5c99a0>|0x220|+0x009:'59'
	// <0>
	// <1>
	// <0x5c99a9>|0x229|+0x003:'62'
	// <0>
	// <1>
	// <0x5c99ac>|0x22c|+0x006:'65'
	// <0x5c99b2>|0x232|+0x09c:'66'
	// <0>
	// <0x5c9a4e>|0x2ce|+0x005:'68'
	// <0x5c9a53>|0x2d3|+0x009:'69'
	// <0>
	// <1>
	// <0x5c9a5c>|0x2dc|+0x003:'72'
	// <0>
	// <1>
	// <0x5c9a5f>|0x2df|+0x006:'75'
	// <0x5c9a65>|0x2e5|+0x09e:'76'
	// <0>
	// <0x5c9b03>|0x383|+0x005:'78'
	// <0>
	// <1>
	// <2>
	// <0x5c9b08>|0x388|+0x005:'82'
	// <0x5c9b0d>|0x38d|+0x007:'83'
	// <0x5c9b14>|0x394|+0x005:'84'
	// <0x5c9b19>|0x399|+0x0b6:'85'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c9bcf>|0x44f|-0x40b:'91'
	// <0>
	// <1>
	// <2>
	// <0x5c97c4>|0x044|+0x0c9:'95'
	// <0>
	// <1>
	// <0x5c988d>|0x10d|+0x355:'98'
	// <0x5c9be2>|0x462|+0x010:'98'
	// <0x5c9bf2>|0x472|      :'98'	}
	// ******
}

// STATE[STUB]
messaging::message_channel_enum messaging_client::parse_receiver_channel( wchar_t const* w_receiver_name, const bool in_match )
{
	// buildability return
	return messaging::server_message_channel;

	// FUNCTION BODY[0x5c8d50]: 16
	// <0x5c8d50>|0x000|+0x000:'101'	{
	// <0>
	// <0x5c8d50>|0x000|+0x104:'103'
	// <0x5c8e54>|0x104|-0x0d8:'104'
	// <0>
	// <0x5c8d7c>|0x02c|+0x0d0:'106'
	// <0x5c8e4c>|0x0fc|-0x0a4:'107'
	// <0>
	// <0x5c8da8>|0x058|+0x09c:'109'
	// <0x5c8e44>|0x0f4|-0x074:'110'
	// <0>
	// <0x5c8dd0>|0x080|+0x02b:'112'
	// <0x5c8dfb>|0x0ab|+0x015:'113'
	// <0x5c8e10>|0x0c0|+0x024:'114'
	// <0x5c8e34>|0x0e4|+0x008:'115'
	// <0>
	// <0x5c8e3c>|0x0ec|-0x02f:'117'
	// <0x5c8e0d>|0x0bd|+0x02c:'118'
	// <0x5c8e39>|0x0e9|+0x008:'118'
	// <0x5c8e41>|0x0f1|+0x008:'118'
	// <0x5c8e49>|0x0f9|+0x008:'118'
	// <0x5c8e51>|0x101|+0x008:'118'
	// <0x5c8e59>|0x109|      :'118'	}
	// ******
}

// STATE[STUB]
void messaging_client::on_message_typed( wchar_t const* input_text, messaging::message_channel_enum message_chanel )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// wchar_t[32] 						w_receiver_name
	// bool 							has_direct_receiver
	// char[32] 						receiver_name
	// char[256] 						message_body
	// wchar_t[32] 						w_sender_name
	// ******

	// STATICS
	// static < NoType > 				 = <0x5c95b0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b85a0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN102") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b847c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b8487 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b8496 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5c92f0]: 92
	// <0x5c92f0>|0x000|+0x007:'121'	{
	// <0x5c92f7>|0x007|+0x00b:'122'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c9302>|0x012|+0x005:'127'
	// <0x5c9307>|0x017|+0x015:'128'
	// <0>
	// <0x5c931c>|0x02c|+0x00b:'130'
	// <0>
	// <0x5c9327>|0x037|+0x003:'132'
	// <0x5c932a>|0x03a|+0x016:'133'
	// <0x5c9340>|0x050|+0x02e:'134'
	// <0>
	// <1>
	// <0x5c936e>|0x07e|+0x005:'137'
	// <0x5c9373>|0x083|+0x011:'138'
	// <0>
	// <0x5c9384>|0x094|+0x00d:'140'
	// <0>
	// <1>
	// <0x5c9391>|0x0a1|+0x025:'143'
	// <0>
	// <0x5c93b6>|0x0c6|+0x016:'145'
	// <0x5c93cc>|0x0dc|+0x005:'146'
	// <0x5c93d1>|0x0e1|+0x1ba:'147'
	// <0>
	// <1>
	// <0x5c958b>|0x29b|-0x1a6:'150'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5c93e5>|0x0f5|+0x02c:'159'
	// <0x5c9411>|0x121|+0x01a:'160'
	// <0>
	// <0x5c942b>|0x13b|+0x028:'162'
	// <0x5c9453>|0x163|+0x01a:'163'
	// <0>
	// <1>
	// <0x5c946d>|0x17d|+0x00f:'166'
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <0x5c947c>|0x18c|+0x006:'184'
	// <0x5c9482>|0x192|+0x003:'185'
	// <0>
	// <0x5c9485>|0x195|+0x002:'187'
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
	// <0x5c9487>|0x197|+0x003:'198'
	// <0x5c948a>|0x19a|+0x00c:'199'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c9496>|0x1a6|+0x00e:'204'
	// <0>
	// <0x5c94a4>|0x1b4|+0x015:'206'
	// <0>
	// <0x5c94b9>|0x1c9|+0x010:'208'
	// <0x5c94c9>|0x1d9|+0x038:'209'
	// <0>
	// <0x5c9501>|0x211|+0x010:'211'
	// <0x5c9511>|0x221|+0x03f:'212'
	// <0x5c9550>|0x260|+0x015:'213'
	// <0x5c9565>|0x275|+0x03b:'214'
	// <0x5c95a0>|0x2b0|      :'214'	}
	// ******
}

// STATE[STUB]
void messaging_client::query_for_friend_list( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c9280]: 7
	// <0x5c9283>|0x003|+0x00a:'218'
	// <0>
	// <1>
	// <0x5c928d>|0x00d|+0x00e:'221'
	// <0x5c929b>|0x01b|+0x015:'222'
	// <0x5c92b0>|0x030|+0x011:'223'
	// <0x5c92c1>|0x041|+0x00e:'224'
	// ******
}

// STATE[STUB]
void messaging_client::query_for_friends_status( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c9200]: 7
	// <0x5c9209>|0x009|+0x00a:'229'
	// <0>
	// <1>
	// <0x5c9213>|0x013|+0x00e:'232'
	// <0x5c9221>|0x021|+0x015:'233'
	// <0x5c9236>|0x036|+0x011:'234'
	// <0x5c9247>|0x047|+0x00e:'235'
	// ******
}

// STATE[STUB]
void messaging_client::add_to_friend_list( const u32 account_id )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c9170]: 8
	// <0x5c9173>|0x003|+0x00a:'240'
	// <0>
	// <1>
	// <0x5c917d>|0x00d|+0x00e:'243'
	// <0x5c918b>|0x01b|+0x015:'244'
	// <0x5c91a0>|0x030|+0x011:'245'
	// <0x5c91b1>|0x041|+0x014:'246'
	// <0x5c91c5>|0x055|+0x00e:'247'
	// ******
}

// STATE[STUB]
void messaging_client::remove_from_friend_list( const u32 account_id )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c90e0]: 8
	// <0x5c90e3>|0x003|+0x00a:'252'
	// <0>
	// <1>
	// <0x5c90ed>|0x00d|+0x00e:'255'
	// <0x5c90fb>|0x01b|+0x015:'256'
	// <0x5c9110>|0x030|+0x011:'257'
	// <0x5c9121>|0x041|+0x014:'258'
	// <0x5c9135>|0x055|+0x00e:'259'
	// ******
}

// STATE[STUB]
void messaging_client::query_for_ignore_list( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c9070]: 7
	// <0x5c9073>|0x003|+0x00a:'264'
	// <0>
	// <1>
	// <0x5c907d>|0x00d|+0x00e:'267'
	// <0x5c908b>|0x01b|+0x015:'268'
	// <0x5c90a0>|0x030|+0x011:'269'
	// <0x5c90b1>|0x041|+0x00e:'270'
	// ******
}

// STATE[STUB]
void messaging_client::add_to_ignore_list( const u32 account_id )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c8fe0]: 8
	// <0x5c8fe3>|0x003|+0x00a:'275'
	// <0>
	// <1>
	// <0x5c8fed>|0x00d|+0x00e:'278'
	// <0x5c8ffb>|0x01b|+0x015:'279'
	// <0x5c9010>|0x030|+0x011:'280'
	// <0x5c9021>|0x041|+0x014:'281'
	// <0x5c9035>|0x055|+0x00e:'282'
	// ******
}

// STATE[STUB]
void messaging_client::remove_from_ignore_list( const u32 account_id )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c8f50]: 8
	// <0x5c8f53>|0x003|+0x00a:'287'
	// <0>
	// <1>
	// <0x5c8f5d>|0x00d|+0x00e:'290'
	// <0x5c8f6b>|0x01b|+0x015:'291'
	// <0x5c8f80>|0x030|+0x011:'292'
	// <0x5c8f91>|0x041|+0x014:'293'
	// <0x5c8fa5>|0x055|+0x00e:'294'
	// ******
}

// STATE[STUB]
void messaging_client::find_players_by_name( pcstr player_name )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c8ea0]: 8
	// <0x5c8ea9>|0x009|+0x00e:'299'
	// <0>
	// <1>
	// <0x5c8eb7>|0x017|+0x00e:'302'
	// <0x5c8ec5>|0x025|+0x015:'303'
	// <0x5c8eda>|0x03a|+0x011:'304'
	// <0x5c8eeb>|0x04b|+0x02e:'305'
	// <0x5c8f19>|0x079|+0x00e:'306'
	// ******
}

// STATE[STUB]
bool messaging_client::accept_message_from( const u32 sender_account_id, messaging::client_type_enum sender_type )
{
	return false;

	// FUNCTION BODY[0x5c8e60]: 3
	// <0x5c8e60>|0x000|+0x000:'310'	{
	// <0>
	// <0x5c8e60>|0x000|+0x032:'312'
	// <0x5c8e92>|0x032|-0x00c:'312'
	// <0x5c8e86>|0x026|+0x00e:'313'
	// <0x5c8e94>|0x034|-0x005:'313'
	// <0x5c8e8f>|0x02f|+0x00e:'314'
	// <0x5c8e9d>|0x03d|      :'314'	}
	// ******
}

// STATE[STUB]
void messaging_client::process_incoming_text_message( network_core::packet_reader& reader )
{
	// LOCALS
	// char[256] 						body
	// messaging::send_message_params 	params
	// wchar_t[1024] 					w_text
	// wchar_t[32] 						w_sender_name
	// ******

	// FUNCTION BODY[0x5c95d0]: 32
	// <0x5c95d0>|0x000|+0x00f:'317'	{
	// <0>
	// <1>
	// <0x5c95df>|0x00f|+0x00b:'320'
	// <0>
	// <0x5c95ea>|0x01a|+0x010:'322'
	// <0x5c95fa>|0x02a|+0x00e:'323'
	// <0>
	// <0x5c9608>|0x038|+0x02d:'325'
	// <0>
	// <0x5c9635>|0x065|+0x023:'327'
	// <0x5c9658>|0x088|+0x00d:'328'
	// <0x5c9665>|0x095|+0x020:'329'
	// <0>
	// <1>
	// <0x5c9685>|0x0b5|+0x028:'332'
	// <0>
	// <1>
	// <0x5c96ad>|0x0dd|+0x025:'335'
	// <0>
	// <0x5c96d2>|0x102|+0x00c:'337'
	// <0>
	// <0x5c96de>|0x10e|+0x029:'339'
	// <0>
	// <0x5c9707>|0x137|+0x005:'341'
	// <0>
	// <0x5c970c>|0x13c|+0x034:'343'
	// <0>
	// <1>
	// <0x5c9740>|0x170|+0x022:'346'
	// <0x5c9762>|0x192|-0x068:'347'
	// <0>
	// <1>
	// <0x5c96fa>|0x12a|+0x039:'350'
	// <0x5c9733>|0x163|+0x03c:'350'
	// <0x5c976f>|0x19f|      :'350'	}
	// ******
}

} // namespace survarium
