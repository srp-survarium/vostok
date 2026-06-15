////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_client.h"

#include "game_memory.h"	// g_allocator for the vectora<> buildability init

namespace survarium {

// STATE[STUB]
 lobby_client::lobby_client( game& g ) :
	// ref member + no-default-ctor packet client force the init-list; the world
	// placeholder is buildability only (NULL deref never runs in a stub) - a
	// matcher supplies the real source when this TU is enabled
	m_game( g ),
	m_packet_client( *( network::world* )NULL ),
	// vectora has a private default ctor; g_allocator is a buildability
	// placeholder - a matcher supplies the real allocator
	m_inventory_item_instances( g_allocator )
{
	// FUNCTION BODY[0x5c8a70]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5c8b6d>|0x0fd|+0x05e:'53'
	// <0x5c8bcb>|0x15b|+0x042:'54'
	// <0x5c8c0d>|0x19d|+0x042:'55'
	// <0x5c8c4f>|0x1df|+0x042:'56'
	// <0>
	// ******
}

// STATE[STUB]
 lobby_client::~lobby_client( )
{
	// FUNCTION BODY[0x5c8980]: 2
	// <0x5c8983>|0x003|+0x005:'62'
	// <0x5c8988>|0x008|+0x005:'63'
	// ******
}

// STATE[STUB]
void lobby_client::clear_initial_info( )
{
	// FUNCTION BODY[0x5c78d0]: 2
	// <0x5c78d0>|0x000|+0x02f:'68'
	// <0x5c78ff>|0x02f|+0x02e:'69'
	// ******
}

// STATE[STUB]
void lobby_client::clear_profile_info( )
{
	// FUNCTION BODY[0x5c7a60]: 13
	// <0x5c7a64>|0x004|+0x008:'74'
	// <0x5c7a6c>|0x00c|+0x02a:'75'
	// <0>
	// <1>
	// <0x5c7a96>|0x036|+0x050:'78'
	// <0>
	// <1>
	// <0x5c7ae6>|0x086|+0x032:'81'
	// <0>
	// <1>
	// <0x5c7b18>|0x0b8|+0x032:'84'
	// <0x5c7b4a>|0x0ea|+0x007:'85'
	// <0>
	// ******
}

// STATE[STUB]
void lobby_client::on_connected( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c8530]: 6
	// <0x5c853e>|0x00e|+0x04e:'91'
	// <0>
	// <0x5c858c>|0x05c|+0x00e:'93'
	// <0x5c859a>|0x06a|+0x015:'94'
	// <0x5c85af>|0x07f|+0x013:'95'
	// <0x5c85c2>|0x092|+0x00a:'96'
	// ******
}

// STATE[STUB]
void lobby_client::on_disconnected( )
{
	// FUNCTION BODY[0x5c7b60]: 3
	// <0x5c7b60>|0x000|+0x001:'100'	{
	// <0>
	// <0x5c7b61>|0x001|+0x018:'102'
	// <0x5c7b79>|0x019|+0x003:'103'
	// <0x5c7b7c>|0x01c|-0x001:'103'
	// <0x5c7b7b>|0x01b|+0x006:'104'
	// <0x5c7b81>|0x021|      :'104'	}
	// ******
}

// STATE[STUB]
// PDB: both params __formal (genuinely unused); two would collide - left unnamed
void lobby_client::on_error(
	network_core::client_error_codes_enum,
	boost::system::error_code
)
{
	// FUNCTION BODY[0x5c8440]: 4
	// <0x5c8449>|0x009|+0x0c3:'108'
	// <0x5c850c>|0x0cc|+0x007:'109'
	// <0>
	// <0x5c8513>|0x0d3|+0x00d:'111'
	// ******
}

// STATE[STUB]
void lobby_client::connect( server_connection_info const& lobby_connection_info )
{
	// FUNCTION BODY[0x5c8d10]: 4
	// <0x5c8d10>|0x000|+0x007:'116'
	// <0x5c8d17>|0x007|+0x00f:'117'
	// <0>
	// <0x5c8d26>|0x016|+0x01c:'119'
	// ******
}

// STATE[STUB]
void lobby_client::disconnect( )
{
	// FUNCTION BODY[0x5c8340]: 6
	// <0>
	// <0x5c834a>|0x00a|+0x01b:'125'
	// <0x5c8365>|0x025|+0x007:'126'
	// <0x5c836c>|0x02c|+0x0b8:'127'
	// <0x5c8424>|0x0e4|+0x005:'128'
	// <0x5c8429>|0x0e9|+0x00c:'129'
	// <0x5c8435>|0x0f5|-0x006:'129'
	// ******
}

// STATE[STUB]
void lobby_client::sign_in_on_packet_received( network_core::packet_reader& reader )
{
	// FUNCTION BODY[0x5c8120]: 23
	// <0x5c8120>|0x000|+0x00d:'133'	{
	// <0x5c812d>|0x00d|+0x0fb:'134'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c8228>|0x108|+0x018:'140'
	// <0x5c8240>|0x120|+0x0bf:'141'
	// <0>
	// <0x5c82ff>|0x1df|+0x010:'143'
	// <0>
	// <0x5c830f>|0x1ef|+0x005:'145'
	// <0x5c8314>|0x1f4|+0x007:'146'
	// <0>
	// <0x5c831b>|0x1fb|+0x011:'148'
	// <0x5c832c>|0x20c|-0x1f3:'149'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c8139>|0x019|+0x0e6:'154'
	// <0>
	// <1>
	// <0x5c821f>|0x0ff|+0x114:'157'
	// <0x5c8333>|0x213|      :'157'	}
	// ******
}

// STATE[STUB]
u32 lobby_client::session_id( ) const
{
	return 0;

	// FUNCTION BODY[0x5c7510]: 1
	// <0x5c7510>|0x000|+0x003:'161'
	// ******
}

// STATE[STUB]
void lobby_client::set_status_ready_for_match(
	const u32		arg_0 /* u32 profile_id */
)
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c80b0]: 5
	// <0>
	// <0x5c80b4>|0x004|+0x00e:'167'
	// <0x5c80c2>|0x012|+0x015:'168'
	// <0x5c80d7>|0x027|+0x014:'169'
	// <0x5c80eb>|0x03b|+0x012:'170'
	// ******
}

// STATE[STUB]
void lobby_client::query_client_status( lobby::query_info_types type )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c8040]: 5
	// <0>
	// <0x5c8044>|0x004|+0x00e:'176'
	// <0x5c8052>|0x012|+0x015:'177'
	// <0x5c8067>|0x027|+0x014:'178'
	// <0x5c807b>|0x03b|+0x012:'179'
	// ******
}

// STATE[STUB]
void lobby_client::query_prices(
	const u32		arg_0 /* u32 faction_id */
)
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7fc0]: 6
	// <0>
	// <0x5c7fc4>|0x004|+0x00e:'186'
	// <0x5c7fd2>|0x012|+0x015:'187'
	// <0x5c7fe7>|0x027|+0x011:'188'
	// <0x5c7ff8>|0x038|+0x014:'189'
	// <0x5c800c>|0x04c|+0x012:'190'
	// ******
}

// STATE[STUB]
void lobby_client::query_profile_contents(
	const u32		arg_0 /* u32 profile_id */
)
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7f40]: 6
	// <0>
	// <0x5c7f44>|0x004|+0x00e:'196'
	// <0x5c7f52>|0x012|+0x015:'197'
	// <0x5c7f67>|0x027|+0x011:'198'
	// <0x5c7f78>|0x038|+0x014:'199'
	// <0x5c7f8c>|0x04c|+0x012:'200'
	// ******
}

// STATE[STUB]
bool lobby_client::read_status_info( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c7810]: 32
	// <0x5c7810>|0x000|+0x000:'204'	{
	// <0x5c7810>|0x000|+0x00c:'205'
	// <0x5c781c>|0x00c|+0x006:'206'
	// <0>
	// <1>
	// <0x5c7822>|0x012|+0x00a:'209'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c782c>|0x01c|+0x011:'215'
	// <0x5c783d>|0x02d|+0x011:'216'
	// <0x5c784e>|0x03e|+0x012:'217'
	// <0x5c7860>|0x050|+0x002:'218'
	// <0>
	// <1>
	// <0x5c7862>|0x052|+0x009:'221'
	// <0x5c786b>|0x05b|+0x006:'222'
	// <0x5c7871>|0x061|+0x00a:'223'
	// <0>
	// <1>
	// <2>
	// <0x5c787b>|0x06b|+0x00f:'227'
	// <0>
	// <1>
	// <0x5c788a>|0x07a|+0x029:'230'
	// <0>
	// <1>
	// <0x5c78b3>|0x0a3|-0x004:'233'
	// <0>
	// <1>
	// <0x5c78af>|0x09f|+0x013:'236'
	// <0x5c78c2>|0x0b2|-0x010:'236'
	// <0x5c78b2>|0x0a2|+0x013:'237'
	// <0x5c78c5>|0x0b5|      :'237'	}
	// ******
}

// STATE[STUB]
bool lobby_client::read_enumerate_profiles_info( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c7990]: 13
	// <0x5c7990>|0x000|+0x00c:'241'
	// <0>
	// <1>
	// <0x5c799c>|0x00c|+0x014:'244'
	// <0>
	// <1>
	// <0x5c79b0>|0x020|+0x01e:'247'
	// <0x5c79ce>|0x03e|+0x034:'248'
	// <0>
	// <1>
	// <0x5c7a02>|0x072|+0x056:'251'
	// <0>
	// <0x5c7a58>|0x0c8|+0x003:'253'
	// ******
}

// STATE[STUB]
u8 lobby_client::read_profile_content_info( network_core::packet_reader& reader )
{
	// LOCALS
	// player_profile 					profile
	// ******

	return 0;

	// FUNCTION BODY[0x5c7650]: 19
	// <0x5c7650>|0x000|+0x010:'257'	{
	// <0x5c7660>|0x010|+0x009:'258'
	// <0x5c7669>|0x019|+0x013:'259'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c767c>|0x02c|+0x014:'265'
	// <0>
	// <0x5c7690>|0x040|+0x009:'267'
	// <0x5c7699>|0x049|+0x023:'268'
	// <0>
	// <0x5c76bc>|0x06c|+0x013:'270'
	// <0x5c76cf>|0x07f|-0x01f:'271'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c76b0>|0x060|+0x029:'277'
	// <0x5c76d9>|0x089|      :'277'	}
	// ******
}

// STATE[STUB]
bool lobby_client::read_enumerate_inventory_info( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c7760]: 7
	// <0x5c7763>|0x003|+0x007:'281'
	// <0x5c776a>|0x00a|+0x032:'282'
	// <0x5c779c>|0x03c|+0x005:'283'
	// <0x5c77a1>|0x041|+0x004:'284'
	// <0x5c77a5>|0x045|+0x014:'285'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
faction_price const& lobby_client::price(
	const u8	arg_0 /* u8 faction_id */
)
{
	// buildability return
	return m_prices[ 0 ];

	// FUNCTION BODY[0x5c7520]: 2
	// <0>
	// <0x5c7520>|0x000|+0x00a:'293'
	// ******
}

// STATE[STUB]
u8 lobby_client::read_price_items( network_core::packet_reader& reader )
{
	return 0;

	// FUNCTION BODY[0x5c8910]: 14
	// <0x5c8910>|0x000|+0x00a:'298'
	// <0x5c891a>|0x00a|+0x00b:'299'
	// <0x5c8925>|0x015|+0x003:'300'
	// <0x5c8928>|0x018|+0x011:'301'
	// <0>
	// <0x5c8939>|0x029|+0x005:'303'
	// <0>
	// <0x5c893e>|0x02e|+0x01f:'305'
	// <0>
	// <0x5c895d>|0x04d|+0x00a:'307'
	// <0x5c8967>|0x057|+0x013:'308'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
bool lobby_client::read_profile_slots_restrictions( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c8870]: 10
	// <0x5c887a>|0x00a|+0x030:'316'
	// <0>
	// <0x5c88aa>|0x03a|+0x011:'318'
	// <0x5c88bb>|0x04b|+0x020:'319'
	// <0>
	// <0x5c88db>|0x06b|+0x009:'321'
	// <0>
	// <0x5c88e4>|0x074|+0x004:'323'
	// <0x5c88e8>|0x078|+0x011:'324'
	// <0>
	// ******
}

// STATE[STUB]
bool lobby_client::read_items_compatibility( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c87d0]: 10
	// <0x5c87da>|0x00a|+0x030:'330'
	// <0>
	// <0x5c880a>|0x03a|+0x011:'332'
	// <0x5c881b>|0x04b|+0x024:'333'
	// <0>
	// <0x5c883f>|0x06f|+0x00d:'335'
	// <0>
	// <0x5c884c>|0x07c|+0x004:'337'
	// <0x5c8850>|0x080|+0x011:'338'
	// <0>
	// ******
}

// STATE[STUB]
bool lobby_client::read_player_skills( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c8690]: 24
	// <0x5c8691>|0x001|+0x034:'345'
	// <0>
	// <0x5c86c5>|0x035|+0x011:'347'
	// <0x5c86d6>|0x046|+0x011:'348'
	// <0x5c86e7>|0x057|+0x011:'349'
	// <0>
	// <0x5c86f8>|0x068|+0x009:'351'
	// <0x5c8701>|0x071|+0x022:'352'
	// <0>
	// <0x5c8723>|0x093|+0x011:'354'
	// <0>
	// <0x5c8734>|0x0a4|+0x004:'356'
	// <0x5c8738>|0x0a8|+0x011:'357'
	// <0>
	// <0x5c8749>|0x0b9|+0x02c:'359'
	// <0>
	// <0x5c8775>|0x0e5|+0x009:'361'
	// <0x5c877e>|0x0ee|+0x01f:'362'
	// <0>
	// <0x5c879d>|0x10d|+0x00f:'364'
	// <0x5c87ac>|0x11c|+0x004:'365'
	// <0x5c87b0>|0x120|+0x011:'366'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
bool lobby_client::read_player_skills_tree( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c8ca0]: 2
	// <0x5c8ca3>|0x003|+0x05e:'373'
	// <0x5c8d01>|0x061|+0x002:'374'
	// ******
}

// STATE[STUB]
lobby::client_state_enum lobby_client::status( fixed_string< 128 >& dest ) const
{
	// buildability return
	return m_status;

	// STATICS
	// static < NoType > 				 = <0x5c763c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b65a6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b65c2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b65e5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b660f }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5c7590]: 20
	// <0x5c7590>|0x000|+0x000:'378'	{
	// <0x5c7590>|0x000|+0x016:'379'
	// <0>
	// <1>
	// <2>
	// <0x5c75a6>|0x016|+0x01c:'383'
	// <0>
	// <1>
	// <2>
	// <0x5c75c2>|0x032|+0x023:'387'
	// <0>
	// <1>
	// <2>
	// <0x5c75e5>|0x055|+0x01a:'391'
	// <0>
	// <1>
	// <2>
	// <0x5c75ff>|0x06f|+0x010:'395'
	// <0x5c760f>|0x07f|-0x057:'395'
	// <0>
	// <1>
	// <0x5c75b8>|0x028|+0x023:'398'
	// <0x5c75db>|0x04b|+0x02a:'398'
	// <0x5c7605>|0x075|+0x02d:'398'
	// <0x5c7632>|0x0a2|-0x071:'398'
	// <0x5c75c1>|0x031|+0x023:'399'
	// <0x5c75e4>|0x054|+0x02a:'399'
	// <0x5c760e>|0x07e|+0x02a:'399'
	// <0x5c7638>|0x0a8|      :'399'	}
	// ******
}

// STATE[STUB]
bool lobby_client::can_move_item( const u32 item_category_id, const u32 target_slot_id )
{
	return false;

	// FUNCTION BODY[0x5c74d0]: 10
	// <0x5c74d0>|0x000|+0x000:'405'	{
	// <0x5c74d0>|0x000|+0x005:'406'
	// <0x5c74d5>|0x005|+0x009:'407'
	// <0>
	// <0x5c74de>|0x00e|+0x021:'409'
	// <0x5c74ff>|0x02f|-0x00f:'409'
	// <0>
	// <0x5c74f0>|0x020|+0x01b:'411'
	// <0x5c750b>|0x03b|-0x004:'412'
	// <0>
	// <1>
	// <0x5c7507>|0x037|-0x030:'415'
	// <0x5c74d7>|0x007|+0x033:'416'
	// <0x5c750a>|0x03a|+0x004:'416'
	// <0x5c750e>|0x03e|      :'416'	}
	// ******
}

// STATE[STUB]
void lobby_client::move_item( vector< relocate_item_descr >& items )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7e80]: 11
	// <0>
	// <1>
	// <0x5c7e8a>|0x00a|+0x00e:'422'
	// <0x5c7e98>|0x018|+0x015:'423'
	// <0x5c7ead>|0x02d|+0x011:'424'
	// <0x5c7ebe>|0x03e|+0x026:'425'
	// <0x5c7ee4>|0x064|+0x00c:'426'
	// <0>
	// <0x5c7ef0>|0x070|+0x011:'428'
	// <0>
	// <0x5c7f01>|0x081|+0x014:'430'
	// ******
}

// STATE[STUB]
void lobby_client::buy_item(
	const u16		item_dict_id,
	const u32		count,
	const u8		faction_id,
	const bool		use_premium_money
)
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7dc0]: 9
	// <0>
	// <0x5c7dc4>|0x004|+0x00e:'436'
	// <0x5c7dd2>|0x012|+0x015:'437'
	// <0x5c7de7>|0x027|+0x011:'438'
	// <0x5c7df8>|0x038|+0x015:'439'
	// <0x5c7e0d>|0x04d|+0x014:'440'
	// <0x5c7e21>|0x061|+0x014:'441'
	// <0x5c7e35>|0x075|+0x011:'442'
	// <0x5c7e46>|0x086|+0x012:'443'
	// ******
}

// STATE[STUB]
// the player_skill( u8 ) getter hides the type name; qualify (header does too)
void lobby_client::set_player_skills( vectora< survarium::player_skill >& skills, vectora< u8 >& perks )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7cf0]: 14
	// <0>
	// <0x5c7cfa>|0x00a|+0x010:'449'
	// <0x5c7d0a>|0x01a|+0x015:'450'
	// <0x5c7d1f>|0x02f|+0x011:'451'
	// <0>
	// <0x5c7d30>|0x040|+0x017:'453'
	// <0x5c7d47>|0x057|+0x009:'454'
	// <0x5c7d50>|0x060|+0x00d:'455'
	// <0>
	// <0x5c7d5d>|0x06d|+0x019:'457'
	// <0x5c7d76>|0x086|+0x009:'458'
	// <0x5c7d7f>|0x08f|+0x009:'459'
	// <0>
	// <0x5c7d88>|0x098|+0x014:'461'
	// ******
}

// STATE[STUB]
void lobby_client::reroll_player_skills( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7c80]: 5
	// <0>
	// <0x5c7c84>|0x004|+0x00e:'467'
	// <0x5c7c92>|0x012|+0x015:'468'
	// <0x5c7ca7>|0x027|+0x011:'469'
	// <0x5c7cb8>|0x038|+0x012:'470'
	// ******
}

// STATE[STUB]
bool lobby_client::check_compatibility( const u32 first_item_id, const u32 second_item_id )
{
	return false;

	// FUNCTION BODY[0x5c7530]: 9
	// <0x5c7530>|0x000|+0x00d:'474'	{
	// <0x5c753d>|0x00d|+0x02e:'475'
	// <0x5c756b>|0x03b|-0x01b:'475'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c7550>|0x020|+0x02d:'480'
	// <0x5c757d>|0x04d|-0x008:'481'
	// <0>
	// <0x5c7575>|0x045|+0x003:'483'
	// <0x5c7578>|0x048|+0x008:'484'
	// <0x5c7580>|0x050|      :'484'	}
	// ******
}

// STATE[STUB]
bool lobby_client::read_account_money( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c7930]: 5
	// <0x5c7930>|0x000|+0x011:'488'
	// <0x5c7941>|0x011|+0x011:'489'
	// <0x5c7952>|0x022|+0x00f:'490'
	// <0x5c7961>|0x031|+0x01f:'491'
	// <0x5c7980>|0x050|+0x003:'492'
	// ******
}

// STATE[STUB]
bool lobby_client::read_service_prices( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c77d0]: 2
	// <0x5c77d0>|0x000|+0x034:'497'
	// <0x5c7804>|0x034|+0x003:'498'
	// ******
}

// STATE[STUB]
bool lobby_client::read_player_reputations( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c85f0]: 11
	// <0x5c85f0>|0x000|+0x036:'503'
	// <0>
	// <0x5c8626>|0x036|+0x006:'505'
	// <0x5c862c>|0x03c|+0x027:'506'
	// <0>
	// <0x5c8653>|0x063|+0x013:'508'
	// <0>
	// <0x5c8666>|0x076|+0x004:'510'
	// <0x5c866a>|0x07a|+0x011:'511'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void lobby_client::discard_playing_order_on_connected( )
{
	// FUNCTION BODY[0x5c74c0]: 1
	// <0x5c74c0>|0x000|+0x007:'528'
	// ******
}

// STATE[STUB]
void lobby_client::discard_playing_order( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7c10]: 5
	// <0>
	// <0x5c7c14>|0x004|+0x00e:'534'
	// <0x5c7c22>|0x012|+0x015:'535'
	// <0x5c7c37>|0x027|+0x016:'536'
	// <0x5c7c4d>|0x03d|+0x00e:'537'
	// ******
}

// STATE[STUB]
void lobby_client::ping_server( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// FUNCTION BODY[0x5c7b90]: 7
	// <0x5c7b93>|0x003|+0x00a:'542'
	// <0>
	// <1>
	// <0x5c7b9d>|0x00d|+0x00e:'545'
	// <0x5c7bab>|0x01b|+0x015:'546'
	// <0x5c7bc0>|0x030|+0x019:'547'
	// <0x5c7bd9>|0x049|+0x00e:'548'
	// ******
}

// STATE[STUB]
bool lobby_client::read_ping_server_answer( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x5c76f0]: 11
	// <0x5c76f3>|0x003|+0x00b:'553'
	// <0>
	// <0x5c76fe>|0x00e|+0x021:'555'
	// <0>
	// <1>
	// <2>
	// <0x5c771f>|0x02f|-0x012:'559'
	// <0>
	// <0x5c770d>|0x01d|+0x018:'561'
	// <0x5c7725>|0x035|+0x029:'561'
	// <0>
	// <0x5c774e>|0x05e|+0x002:'563'
	// ******
}

} // namespace survarium
