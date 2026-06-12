////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\network_client_lobby.h"

namespace survarium {

// STATE[STUB]
void network_client::on_lobby_packet_received( network_core::packet_reader& reader )
{
	// LOCALS
	// char[64] 						host
	// lobby::query_info_types 			type
	// u8 								faction_id
	// lobby_client_message_types_enum 	op_id
	// char[512] 						description
	// lobby_client_message_types_enum 	op_id
	// ******

	// STATICS
	// static < NoType > 				 = <0x5c732c>;
	// ******

	// CALL SITE INFO
	// <0x5c6bd2> -> lobby_client& < unknown >()
	// <0x5c6bed> -> lobby_client& < unknown >()
	// <0x5c6bfc> -> lobby_client& < unknown >()
	// <0x5c6df0> -> lobby_client& < unknown >()
	// <0x5c6dfb> -> lobby_client& < unknown >()
	// <0x5c6e10> -> messaging_client& < unknown >()
	// <0x5c6e61> -> lobby_client& < unknown >()
	// <0x5c6e71> -> lobby_client& < unknown >()
	// <0x5c6e80> -> lobby_client& < unknown >()
	// <0x5c6e90> -> messaging_client& < unknown >()
	// <0x5c6eac> -> lobby_client& < unknown >()
	// <0x5c6ec7> -> lobby_client& < unknown >()
	// <0x5c6eee> -> lobby_client& < unknown >()
	// <0x5c6f08> -> lobby_client& < unknown >()
	// <0x5c6f21> -> lobby_client& < unknown >()
	// <0x5c6f39> -> lobby_client& < unknown >()
	// <0x5c6f53> -> lobby_client& < unknown >()
	// <0x5c6f6d> -> lobby_client& < unknown >()
	// <0x5c6f88> -> lobby_client& < unknown >()
	// <0x5c6fbc> -> lobby_client& < unknown >()
	// <0x5c6fd8> -> lobby_client& < unknown >()
	// <0x5c71b1> -> lobby_client& < unknown >()
	// <0x5c71c2> -> lobby_client& < unknown >()
	// <0x5c7319> -> lobby_client& < unknown >()
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b5b82 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN41") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b5e49 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN40") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b60b1 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN15") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b61e5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5b6312 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x5c6b50]: 152
	// <0x5c6b50>|0x000|+0x00f:'19'	{
	// <0x5c6b5f>|0x00f|+0x00a:'20'
	// <0>
	// <0x5c6b69>|0x019|+0x019:'22'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c6b82>|0x032|+0x030:'27'
	// <0x5c6bb2>|0x062|+0x009:'28'
	// <0x5c6bbb>|0x06b|+0x01f:'29'
	// <0x5c6bda>|0x08a|+0x01b:'30'
	// <0x5c6bf5>|0x0a5|+0x013:'31'
	// <0>
	// <0x5c6c08>|0x0b8|+0x00f:'33'
	// <0x5c6c17>|0x0c7|+0x0c0:'34'
	// <0x5c6cd7>|0x187|+0x07e:'35'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c6d55>|0x205|+0x094:'41'
	// <0>
	// <0x5c6de9>|0x299|+0x049:'43'
	// <0>
	// <0x5c6e32>|0x2e2|+0x017:'45'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c6e49>|0x2f9|+0x00d:'50'
	// <0>
	// <0x5c6e56>|0x306|+0x004:'52'
	// <0>
	// <0x5c6e5a>|0x30a|+0x010:'54'
	// <0>
	// <0x5c6e6a>|0x31a|+0x009:'56'
	// <0x5c6e73>|0x323|+0x015:'57'
	// <0x5c6e88>|0x338|+0x013:'58'
	// <0x5c6e9b>|0x34b|+0x005:'59'
	// <0x5c6ea0>|0x350|+0x005:'60'
	// <0>
	// <0x5c6ea5>|0x355|+0x011:'62'
	// <0x5c6eb6>|0x366|+0x005:'63'
	// <0x5c6ebb>|0x36b|+0x005:'64'
	// <0>
	// <0x5c6ec0>|0x370|+0x00f:'66'
	// <0x5c6ecf>|0x37f|+0x00e:'67'
	// <0x5c6edd>|0x38d|+0x005:'68'
	// <0x5c6ee2>|0x392|+0x005:'69'
	// <0>
	// <0x5c6ee7>|0x397|+0x00f:'71'
	// <0x5c6ef6>|0x3a6|+0x005:'72'
	// <0x5c6efb>|0x3ab|+0x005:'73'
	// <0>
	// <0x5c6f00>|0x3b0|+0x00f:'75'
	// <0x5c6f0f>|0x3bf|+0x005:'76'
	// <0x5c6f14>|0x3c4|+0x005:'77'
	// <0>
	// <0x5c6f19>|0x3c9|+0x00f:'79'
	// <0x5c6f28>|0x3d8|+0x005:'80'
	// <0x5c6f2d>|0x3dd|+0x005:'81'
	// <0>
	// <0x5c6f32>|0x3e2|+0x00f:'83'
	// <0x5c6f41>|0x3f1|+0x005:'84'
	// <0x5c6f46>|0x3f6|+0x005:'85'
	// <0>
	// <0x5c6f4b>|0x3fb|+0x011:'87'
	// <0x5c6f5c>|0x40c|+0x005:'88'
	// <0x5c6f61>|0x411|+0x005:'89'
	// <0>
	// <0x5c6f66>|0x416|+0x011:'91'
	// <0x5c6f77>|0x427|+0x005:'92'
	// <0x5c6f7c>|0x42c|+0x005:'93'
	// <0>
	// <0x5c6f81>|0x431|+0x012:'95'
	// <0x5c6f93>|0x443|+0x018:'96'
	// <0x5c6fab>|0x45b|+0x005:'97'
	// <0x5c6fb0>|0x460|+0x005:'98'
	// <0>
	// <0x5c6fb5>|0x465|+0x012:'100'
	// <0x5c6fc7>|0x477|+0x005:'101'
	// <0x5c6fcc>|0x47c|+0x005:'102'
	// <0>
	// <0x5c6fd1>|0x481|+0x012:'104'
	// <0x5c6fe3>|0x493|+0x005:'105'
	// <0>
	// <0x5c6fe8>|0x498|+0x0af:'107'
	// <0>
	// <1>
	// <0x5c7097>|0x547|+0x01a:'110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c70b1>|0x561|+0x006:'116'
	// <0x5c70b7>|0x567|+0x0ba:'117'
	// <0x5c7171>|0x621|+0x062:'118'
	// <0>
	// <1>
	// <2>
	// <0x5c71d3>|0x683|-0x039:'122'
	// <0>
	// <1>
	// <2>
	// <0x5c719a>|0x64a|+0x009:'126'
	// <0x5c71a3>|0x653|+0x005:'127'
	// <0>
	// <0x5c71a8>|0x658|+0x011:'129'
	// <0>
	// <0x5c71b9>|0x669|-0x038:'131'
	// <0>
	// <0x5c7181>|0x631|+0x064:'133'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5c71e5>|0x695|+0x006:'142'
	// <0x5c71eb>|0x69b|+0x007:'143'
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
	// <0x5c71f2>|0x6a2|+0x034:'155'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c7226>|0x6d6|+0x0c8:'160'
	// <0x5c72ee>|0x79e|+0x024:'161'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c7312>|0x7c2|-0x4d2:'166'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c6e40>|0x2f0|+0x268:'172'
	// <0x5c70a8>|0x558|+0x0e9:'172'
	// <0x5c7191>|0x641|+0x039:'172'
	// <0x5c71ca>|0x67a|+0x012:'172'
	// <0x5c71dc>|0x68c|+0x12d:'172'
	// <0x5c7309>|0x7b9|+0x01a:'172'
	// <0x5c7323>|0x7d3|      :'172'	}
	// ******
}

// STATE[STUB]
void network_client::on_connected_to_lobby( )
{
	// FUNCTION BODY[0x5c6b20]: 2
	// <0>
	// <0x5c6b20>|0x000|+0x027:'177'
	// ******
}

// STATE[STUB]
void network_client::on_disconnected_from_lobby( )
{
	// FUNCTION BODY[0x5c6af0]: 2
	// <0>
	// <0x5c6af1>|0x001|+0x022:'183'
	// ******
}

// STATE[STUB]
void network_client::process_shop_action( network_core::packet_reader& packet )
{
	// LOCALS
	// inventory_item_instance 			new_item
	// ******

	// CALL SITE INFO
	// <0x5c6a3c> -> lobby_client& < unknown >()
	// <0x5c6a4b> -> lobby_client& < unknown >()
	// <0x5c6a82> -> lobby_client& < unknown >()
	// <0x5c6ae0> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5c69f0]: 30
	// <0x5c69f0>|0x000|+0x00f:'284'
	// <0>
	// <0x5c69ff>|0x00f|+0x008:'286'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c6a07>|0x017|+0x009:'292'
	// <0x5c6a10>|0x020|+0x008:'293'
	// <0x5c6a18>|0x028|+0x008:'294'
	// <0>
	// <0x5c6a20>|0x030|+0x024:'296'
	// <0x5c6a44>|0x054|+0x00f:'297'
	// <0>
	// <0x5c6a53>|0x063|+0x004:'299'
	// <0>
	// <0x5c6a57>|0x067|+0x05c:'301'
	// <0>
	// <0x5c6ab3>|0x0c3|+0x002:'303'
	// <0>
	// <1>
	// <2>
	// <0x5c6ab5>|0x0c5|-0x052:'307'
	// <0x5c6a63>|0x073|+0x054:'308'
	// <0x5c6ab7>|0x0c7|+0x011:'308'
	// <0>
	// <0x5c6ac8>|0x0d8|+0x00f:'310'
	// <0x5c6ad7>|0x0e7|+0x011:'311'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void network_client::close_current_match( bool user_initiate )
{
	// CALL SITE INFO
	// <0x5c7352> -> match_client& < unknown >()
	// <0x5c7366> -> match_client& < unknown >()
	// <0x5c738b> -> lobby_client& < unknown >()
	// <0x5c739b> -> lobby_client& < unknown >()
	// <0x5c73ad> -> lobby_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5c7340]: 18
	// <0>
	// <1>
	// <0x5c7343>|0x003|+0x01c:'320'
	// <0x5c735f>|0x01f|+0x010:'321'
	// <0>
	// <0x5c736f>|0x02f|+0x00e:'323'
	// <0>
	// <0x5c737d>|0x03d|+0x007:'325'
	// <0>
	// <0x5c7384>|0x044|+0x010:'327'
	// <0>
	// <1>
	// <0x5c7394>|0x054|+0x012:'330'
	// <0x5c73a6>|0x066|+0x010:'331'
	// <0>
	// <1>
	// <0x5c73b6>|0x076|+0x00d:'334'
	// <0x5c73c3>|0x083|+0x009:'335'
	// ******
}

// STATE[STUB]
void network_client::process_match_finished( network_core::packet_reader& __formal )
{
	// CALL SITE INFO
	// <0x5c69e7> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5c69e0]: 1
	// <0x5c69e0>|0x000|+0x009:'340'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	char[32]
	// 	account_name_type;

	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::inventory_item_instance*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium
