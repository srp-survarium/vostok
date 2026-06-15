////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_client.h"

// m_local_player (player_ptr) / m_net_players (boost::array<player_desc>)
// instantiate resource_ptr<player> dtors here, needing the complete player type
#include "player.h"

namespace survarium {

// TU static console command (compiler-generated atexit destructor); a matcher
// recovers its type/initializer from the init asm when this TU is enabled.
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_show_network_statistics_comand''( )
{
	// FUNCTION BODY[0x7f0290]
	// <0x7d84f0>|0x000|      :'26'	{
	// ******
}
*/

// STATE[STUB]
 network_client::network_client( game& g, const bool is_spectator ) :
	base_network_client( g ),
	// the client members have no default ctors; the world placeholders are
	// buildability only - a matcher supplies the real network world when this
	// TU is enabled
	m_login_client( *( network::world* )NULL ),
	m_lobby_client( g ),
	m_match_client( *( network::world* )NULL ),
	m_messaging_client( g ),
	m_http_client( *( network::world* )NULL )
{
	// STATICS
	// static console_commands::cc_delegate s_attach_to_player = <0x4c2bad8>;
	// static console_commands::cc_delegate s_detach_to_player = <0x4c2ba78>;
	// ******

	// FUNCTION BODY[0x705000]: 21
	// <0x705186>|0x186|+0x079:'51'
	// <0x7051ff>|0x1ff|+0x067:'52'
	// <0x705266>|0x266|+0x066:'53'
	// <0x7052cc>|0x2cc|+0x066:'54'
	// <0x705332>|0x332|+0x049:'55'
	// <0x70537b>|0x37b|+0x00c:'56'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x705387>|0x387|+0x08e:'63'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x705415>|0x415|+0x08d:'70'
	// <0>
	// ******
}

// STATE[STUB]
 network_client::~network_client( )
{
	// FUNCTION BODY[0x704970]: 12
	// <0x704975>|0x005|+0x019:'76'
	// <0>
	// <1>
	// <2>
	// <0x70498e>|0x01e|+0x021:'80'
	// <0x7049af>|0x03f|+0x021:'81'
	// <0x7049d0>|0x060|+0x021:'82'
	// <0x7049f1>|0x081|+0x021:'83'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// claude@NOTE: body recovered (loop over m_net_players, cast each player_desc::player
// to player_ptr, player->set_use_physics_controller_for_current(m_use_physics_controller_for_current)).
// PARKED: player::set_use_physics_controller_for_current(bool) is declared in player.h
// but its definition is not linked (player .cpp stub) -> LNK2001. Restore the body once
// that player method is implemented.
// STATE[STUB]
void network_client::apply_use_physics_controller_for_current( )
{
	// FUNCTION BODY[0x7048f0]: 4
	// loop m_net_players -> player->set_use_physics_controller_for_current( m_use_physics_controller_for_current )
}

// STATE[STUB]
void network_client::on_connected_to_match(
	const connection_error_types_enum		arg_0 /* connection_error_types_enum connection_error */,
	const handshaking_error_types_enum		arg_1 /* handshaking_error_types_enum handshaking_error */,
	const socket_error_types_enum			arg_2 /* socket_error_types_enum socket_error */,
	const lobby_server_message_types_enum	arg_3 /* lobby_server_message_types_enum message_type */
)
{
	// CALL SITE INFO
	// <0x7048aa> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x704120]: 95
	// <0x704120>|0x000|+0x009:'104'	{
	// <0x704129>|0x009|+0x0f1:'105'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x70421a>|0x0fa|-0x0d6:'112'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x704144>|0x024|+0x1a2:'118'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7042e6>|0x1c6|+0x0ca:'123'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7043b0>|0x290|-0x0ad:'130'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x704303>|0x1e3|+0x15a:'139'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x70445d>|0x33d|+0x172:'144'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7045cf>|0x4af|-0x0a8:'151'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x704527>|0x407|-0x0b3:'157'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x704474>|0x354|+0x208:'163'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x70467c>|0x55c|+0x221:'168'
	// <0>
	// <1>
	// <2>
	// <0x70489d>|0x77d|+0x008:'172'
	// <0x7048a5>|0x785|+0x00c:'173'
	// <0x7048b1>|0x791|+0x00c:'174'
	// <0x7048bd>|0x79d|-0x0c8:'175'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7047f5>|0x6d5|-0x0ab:'181'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x70474a>|0x62a|-0x0b6:'188'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x704694>|0x574|-0x483:'195'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x704211>|0x0f1|+0x0cc:'200'
	// <0x7042dd>|0x1bd|+0x0ca:'200'
	// <0x7043a7>|0x287|+0x0ad:'200'
	// <0x704454>|0x334|+0x0ca:'200'
	// <0x70451e>|0x3fe|+0x0a8:'200'
	// <0x7045c6>|0x4a6|+0x0ad:'200'
	// <0x704673>|0x553|+0x0ce:'200'
	// <0x704741>|0x621|+0x0ab:'200'
	// <0x7047ec>|0x6cc|+0x0a8:'200'
	// <0x704894>|0x774|+0x047:'200'
	// <0x7048db>|0x7bb|      :'200'	}
	// ******
}

// STATE[STUB]
void network_client::on_connected_to_login(
	const connection_error_types_enum		arg_0 /* connection_error_types_enum connection_error */,
	const handshaking_error_types_enum		arg_1 /* handshaking_error_types_enum handshaking_error */,
	const socket_error_types_enum			arg_2 /* socket_error_types_enum socket_error */,
	const login_server_message_types_enum	arg_3 /* login_server_message_types_enum message_type */
)
{
	// STATICS
	// static < NoType > 				 = <0x703ba4>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f2573 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f2627 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f26e1 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f279b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f286e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f292b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f29fe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x703030]: 123
	// <0x703030>|0x000|+0x00c:'208'	{
	// <0x70303c>|0x00c|+0x0f1:'209'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x70312d>|0x0fd|+0x0c2:'216'
	// <0>
	// <0x7031ef>|0x1bf|-0x193:'218'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x70305c>|0x02c|+0x0cc:'223'
	// <0>
	// <0x703128>|0x0f8|+0x0cc:'225'
	// <0>
	// <1>
	// <2>
	// <0x7031f4>|0x1c4|+0x0c1:'229'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7032b5>|0x285|+0x09f:'236'
	// <0>
	// <0x703354>|0x324|-0x143:'238'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x703211>|0x1e1|+0x09f:'246'
	// <0>
	// <0x7032b0>|0x280|+0x0a9:'248'
	// <0>
	// <1>
	// <2>
	// <0x703359>|0x329|+0x160:'252'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7034b9>|0x489|+0x09f:'259'
	// <0>
	// <0x703558>|0x528|-0x13e:'261'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x70341a>|0x3ea|+0x09a:'266'
	// <0>
	// <0x7034b4>|0x484|-0x144:'268'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x703370>|0x340|+0x0a5:'273'
	// <0>
	// <0x703415>|0x3e5|+0x148:'275'
	// <0>
	// <1>
	// <2>
	// <0x70355d>|0x52d|+0x016:'279'
	// <0>
	// <1>
	// <2>
	// <0x703573>|0x543|+0x09f:'283'
	// <0x703612>|0x5e2|+0x015:'284'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x703627>|0x5f7|+0x0ba:'290'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7036e1>|0x6b1|+0x0ba:'296'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x70379b>|0x76b|+0x0d3:'302'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x70386e>|0x83e|+0x0bd:'308'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x70392b>|0x8fb|+0x0d3:'314'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7039fe>|0x9ce|+0x0d3:'320'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x703ad1>|0xaa1|-0x407:'327'
	// <0x7036ca>|0x69a|+0x0ba:'328'
	// <0x703784>|0x754|+0x0d3:'328'
	// <0x703857>|0x827|+0x0bd:'328'
	// <0x703914>|0x8e4|+0x0d3:'328'
	// <0x7039e7>|0x9b7|+0x0d3:'328'
	// <0x703aba>|0xa8a|+0x0d3:'328'
	// <0x703b8d>|0xb5d|-0x56f:'328'
	// <0>
	// <1>
	// <2>
	// <0x70361e>|0x5ee|+0x0ba:'332'
	// <0x7036d8>|0x6a8|+0x0ba:'332'
	// <0x703792>|0x762|+0x0d3:'332'
	// <0x703865>|0x835|+0x0bd:'332'
	// <0x703922>|0x8f2|+0x0d3:'332'
	// <0x7039f5>|0x9c5|+0x0d3:'332'
	// <0x703ac8>|0xa98|+0x0d2:'332'
	// <0x703b9a>|0xb6a|      :'332'	}
	// ******
}

// STATE[STUB]
bool network_client::http_query_server_connection_info( u8 type )
{
	// LOCALS
	// fixed_string< 512 > 				client_str
	// char[512] 						request_str
	// ******

	// CALL SITE INFO
	// <0x704f07> -> network::login_client& < unknown >()
	// <0x704f14> -> network::login_client& < unknown >()
	// ******

	return false;

	// FUNCTION BODY[0x704e90]: 29
	// <0x704e90>|0x000|+0x00e:'335'	{
	// <0x704e9e>|0x00e|+0x017:'336'
	// <0x704eb5>|0x025|+0x01f:'337'
	// <0>
	// <0x704ed4>|0x044|+0x009:'339'
	// <0x704edd>|0x04d|+0x00e:'340'
	// <0>
	// <0x704eeb>|0x05b|+0x00a:'342'
	// <0x704ef5>|0x065|+0x00b:'343'
	// <0x704f00>|0x070|+0x00d:'344'
	// <0x704f0d>|0x07d|+0x010:'345'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x704f1d>|0x08d|+0x022:'353'
	// <0x704f3f>|0x0af|+0x08b:'354'
	// <0>
	// <1>
	// <0x704fca>|0x13a|+0x005:'357'
	// <0x704fcf>|0x13f|+0x007:'358'
	// <0x704fd6>|0x146|+0x002:'359'
	// <0x704fd8>|0x148|+0x005:'360'
	// <0x704fdd>|0x14d|-0x021:'361'
	// <0>
	// <1>
	// <0x704fbc>|0x12c|-0x0dd:'364'
	// <0x704edf>|0x04f|+0x0df:'365'
	// <0x704fbe>|0x12e|+0x02d:'365'
	// <0x704feb>|0x15b|      :'365'	}
	// ******
}

// STATE[STUB]
void network_client::on_http_result_ready( pcstr content, u8 type )
{
	// LOCALS
	// server_connection_info 			connection_info
	// ******

	// CALL SITE INFO
	// <0x704da4> -> network::login_client& < unknown >()
	// <0x704dc0> -> network::login_client& < unknown >()
	// <0x704e06> -> lobby_client& < unknown >()
	// <0x704e1f> -> messaging_client& < unknown >()
	// <0x704e3d> -> lobby_client& < unknown >()
	// <0x704e4d> -> lobby_client& < unknown >()
	// <0x704e68> -> messaging_client& < unknown >()
	// <0x704e78> -> messaging_client& < unknown >()
	// ******

	// FUNCTION BODY[0x704d90]: 29
	// <0x704d90>|0x000|+0x00f:'368'	{
	// <0>
	// <0x704d9f>|0x00f|+0x012:'370'
	// <0>
	// <1>
	// <0x704db1>|0x021|+0x025:'373'
	// <0x704dd6>|0x046|+0x019:'374'
	// <0>
	// <0x704def>|0x05f|+0x002:'376'
	// <0>
	// <0x704df1>|0x061|+0x009:'378'
	// <0x704dfa>|0x06a|+0x01a:'379'
	// <0>
	// <0x704e14>|0x084|+0x004:'381'
	// <0x704e18>|0x088|+0x01a:'382'
	// <0>
	// <1>
	// <0x704e32>|0x0a2|+0x004:'385'
	// <0>
	// <0x704e36>|0x0a6|+0x010:'387'
	// <0x704e46>|0x0b6|+0x017:'388'
	// <0>
	// <1>
	// <0x704e5d>|0x0cd|+0x004:'391'
	// <0>
	// <0x704e61>|0x0d1|-0x00d:'393'
	// <0x704e54>|0x0c4|+0x01d:'394'
	// <0x704e71>|0x0e1|-0x064:'394'
	// <0>
	// <1>
	// <2>
	// <0x704e0d>|0x07d|+0x01e:'398'
	// <0x704e2b>|0x09b|+0x02b:'398'
	// <0x704e56>|0x0c6|+0x029:'398'
	// <0x704e7f>|0x0ef|      :'398'	}
	// ******
}

// claude@NOTE: body recovered (single LOG_ERROR). Unpaired only because /OPT:REF strips it:
// on_http_error is bound as an http callback inside the http-client setup path (e.g.
// connect_to_login / http_query_server_connection_info), and those bind sites are still stubs,
// so nothing references it. Pairs once a binder that captures &on_http_error emits.
void network_client::on_http_error( boost::system::error_code __formal )
{
	LOG_ERROR( "http client error!" );
}

// STATE[STUB]
void network_client::on_match_disconnected( network_core::disconnect_event_types_enum disconnect_event_type )
{
	// CALL SITE INFO
	// <0x702f13> -> < unknown >
	// <0x702f2f> -> void < unknown >( bool )
	// <0x702f42> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x702ed0]: 11
	// <0x702ed0>|0x000|+0x00d:'406'	{
	// <0x702edd>|0x00d|+0x03b:'407'
	// <0x702f18>|0x048|+0x00e:'408'
	// <0>
	// <1>
	// <0x702f26>|0x056|+0x013:'411'
	// <0x702f39>|0x069|-0x00e:'411'
	// <0>
	// <1>
	// <2>
	// <0x702f2b>|0x05b|+0x006:'415'
	// <0>
	// <1>
	// <0x702f31>|0x061|+0x013:'418'
	// <0x702f44>|0x074|      :'418'	}
	// ******
}

// STATE[STUB]
void network_client::connect_to_login(
	pcstr const		host,
	const u16		port,
	pcstr			account_name,
	pcstr			account_password
)
{
	// CALL SITE INFO
	// <0x704c53> -> < unknown >
	// ******

	// FUNCTION BODY[0x704af0]: 27
	// <0x704af0>|0x000|+0x012:'421'	{
	// <0x704b02>|0x012|+0x010:'422'
	// <0>
	// <1>
	// <0x704b12>|0x022|+0x00f:'425'
	// <0x704b21>|0x031|+0x0ce:'426'
	// <0>
	// <0x704bef>|0x0ff|+0x069:'428'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x704c58>|0x168|+0x07a:'434'
	// <0>
	// <0x704cd2>|0x1e2|+0x039:'436'
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
	// <0x704d0b>|0x21b|-0x009:'447'
	// <0>
	// <0x704d02>|0x212|+0x07e:'449'
	// <0x704d80>|0x290|      :'449'	}
	// ******
}

// STATE[STUB]
game_world& network_client::get_game_world( )
{
	// buildability return; the real body reaches the world through m_game
	return *( game_world* )NULL;

	// FUNCTION BODY[0x702e50]: 1
	// <0x702e50>|0x000|+0x008:'453'
	// ******
}

// STATE[STUB]
void network_client::draw_stats( const u32 current_time_in_ms )
{
	// LOCALS
	// network_core::udp_match_stats 	difference
	// char[256] 						text
	// ******

	// STATICS
	// static bool 						first_time = <0xa7d45f>;
	// ******

	// FUNCTION BODY[0x703be0]: 77
	// <0x703be6>|0x006|+0x01d:'458'
	// <0x703c03>|0x023|+0x011:'459'
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
	// <0x703c14>|0x034|+0x031:'477'
	// <0x703c45>|0x065|+0x02b:'478'
	// <0x703c70>|0x090|+0x02a:'479'
	// <0x703c9a>|0x0ba|+0x026:'480'
	// <0x703cc0>|0x0e0|+0x02a:'481'
	// <0x703cea>|0x10a|+0x026:'482'
	// <0>
	// <0x703d10>|0x130|+0x022:'484'
	// <0x703d32>|0x152|+0x01a:'485'
	// <0x703d4c>|0x16c|+0x01f:'486'
	// <0x703d6b>|0x18b|+0x021:'487'
	// <0>
	// <0x703d8c>|0x1ac|+0x022:'489'
	// <0x703dae>|0x1ce|+0x01a:'490'
	// <0x703dc8>|0x1e8|+0x01f:'491'
	// <0x703de7>|0x207|+0x021:'492'
	// <0>
	// <0x703e08>|0x228|+0x022:'494'
	// <0x703e2a>|0x24a|+0x01a:'495'
	// <0x703e44>|0x264|+0x01f:'496'
	// <0x703e63>|0x283|+0x021:'497'
	// <0>
	// <0x703e84>|0x2a4|+0x022:'499'
	// <0x703ea6>|0x2c6|+0x01a:'500'
	// <0x703ec0>|0x2e0|+0x01f:'501'
	// <0x703edf>|0x2ff|+0x021:'502'
	// <0>
	// <1>
	// <0x703f00>|0x320|+0x00d:'505'
	// <0x703f0d>|0x32d|+0x00b:'506'
	// <0x703f18>|0x338|+0x00b:'507'
	// <0x703f23>|0x343|+0x00b:'508'
	// <0x703f2e>|0x34e|+0x00b:'509'
	// <0x703f39>|0x359|+0x00b:'510'
	// <0>
	// <0x703f44>|0x364|+0x039:'512'
	// <0x703f7d>|0x39d|+0x034:'513'
	// <0x703fb1>|0x3d1|+0x034:'514'
	// <0x703fe5>|0x405|+0x02c:'515'
	// <0>
	// <1>
	// <0x704011>|0x431|+0x01c:'518'
	// <0x70402d>|0x44d|+0x018:'519'
	// <0x704045>|0x465|+0x018:'520'
	// <0x70405d>|0x47d|+0x018:'521'
	// <0x704075>|0x495|+0x018:'522'
	// <0x70408d>|0x4ad|+0x018:'523'
	// <0>
	// <0x7040a5>|0x4c5|+0x00f:'525'
	// <0x7040b4>|0x4d4|+0x015:'526'
	// <0>
	// <1>
	// <2>
	// <0x7040c9>|0x4e9|+0x01b:'530'
	// <0x7040e4>|0x504|+0x00f:'531'
	// <0>
	// <0x7040f3>|0x513|+0x013:'533'
	// <0x704106>|0x526|+0x00c:'534'
	// ******
}

// STATE[STUB]
game_team_id network_client::get_player_team( pcstr player_profile_name )
{
	// buildability return
	return team_invalid;

	// FUNCTION BODY[0x702e60]: 6
	// <0x702e60>|0x000|+0x001:'538'	{
	// <0x702e61>|0x001|+0x00f:'539'
	// <0>
	// <0x702e70>|0x010|+0x049:'541'
	// <0x702eb9>|0x059|-0x007:'542'
	// <0>
	// <0x702eb2>|0x052|+0x006:'544'
	// <0x702eb8>|0x058|+0x012:'545'
	// <0x702eca>|0x06a|      :'545'	}
	// ******
}

} // namespace survarium
