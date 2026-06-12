////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\network_client_processing.h"

namespace survarium {

// STATE[STUB]
player_ptr network_client::get_player( const u8 id ) const
{
	// FUNCTION BODY[0x5c51c0]: 2
	// <0>
	// <0x5c51c0>|0x000|+0x046:'62'
	// ******
}

// STATE[STUB]
void network_client::destroy_player_impl( const u8 id )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c6017> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c6000]: 16
	// <0x5c600a>|0x00a|+0x00f:'73'
	// <0>
	// <0x5c6019>|0x019|+0x032:'75'
	// <0>
	// <0x5c604b>|0x04b|+0x01a:'77'
	// <0x5c6065>|0x065|+0x005:'78'
	// <0>
	// <0x5c606a>|0x06a|+0x018:'80'
	// <0x5c6082>|0x082|+0x02a:'81'
	// <0>
	// <0x5c60ac>|0x0ac|+0x015:'83'
	// <0>
	// <0x5c60c1>|0x0c1|+0x027:'85'
	// <0x5c60e8>|0x0e8|+0x003:'86'
	// <0x5c60eb>|0x0eb|+0x00d:'87'
	// <0>
	// ******
}

// STATE[STUB]
void network_client::process_match_info( network_core::packet_reader& reader )
{
	// CALL SITE INFO
	// <0x5c46fd> -> match_client& < unknown >()
	// <0x5c4711> -> match_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5c46f0]: 2
	// <0x5c46f5>|0x005|+0x015:'93'
	// <0x5c470a>|0x01a|+0x012:'94'
	// ******
}

// STATE[STUB]
void network_client::on_players_ready( resources::queries_result& data, const u32 players_count )
{
	// FUNCTION BODY[0x5c4e30]: 26
	// <0x5c4e39>|0x009|+0x0d1:'99'
	// <0>
	// <1>
	// <2>
	// <0x5c4f0a>|0x0da|+0x01c:'103'
	// <0>
	// <0x5c4f26>|0x0f6|+0x059:'105'
	// <0>
	// <0x5c4f7f>|0x14f|+0x082:'107'
	// <0>
	// <0x5c5001>|0x1d1|+0x0e1:'109'
	// <0x5c50e2>|0x2b2|+0x006:'110'
	// <0>
	// <0x5c50e8>|0x2b8|+0x045:'112'
	// <0>
	// <0x5c512d>|0x2fd|+0x030:'114'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5c515d>|0x32d|+0x017:'122'
	// <0x5c5174>|0x344|+0x01f:'123'
	// <0x5c5193>|0x363|+0x01b:'124'
	// ******
}

// STATE[STUB]
void network_client::query_players( )
{
	// LOCALS
	// buffer_vector< variant< 32 > const* > user_data_ptrs
	// const u32 						players_count
	// buffer_vector< variant< 32 > > 	user_datas
	// buffer_vector< resources::request > requests
	// u8 								i
	// player_initial_info 				info
	// variant< 32 > 					ud
	// ******

	// CALL SITE INFO
	// <0x5c66e4> -> match_client& < unknown >()
	// <0x5c6746> -> match_client& < unknown >()
	// <0x5c68b5> -> match_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5c66d0]: 43
	// <0x5c66dc>|0x00c|+0x00a:'129'
	// <0>
	// <1>
	// <0x5c66e6>|0x016|+0x007:'132'
	// <0x5c66ed>|0x01d|+0x017:'133'
	// <0x5c6704>|0x034|+0x013:'134'
	// <0x5c6717>|0x047|+0x014:'135'
	// <0>
	// <0x5c672b>|0x05b|+0x121:'137'
	// <0x5c684c>|0x17c|-0x113:'137'
	// <0>
	// <0x5c6739>|0x069|+0x003:'139'
	// <0x5c673c>|0x06c|+0x01b:'140'
	// <0>
	// <0x5c6757>|0x087|+0x014:'142'
	// <0>
	// <1>
	// <2>
	// <0x5c676b>|0x09b|+0x006:'146'
	// <0x5c6771>|0x0a1|+0x00a:'147'
	// <0x5c677b>|0x0ab|+0x041:'148'
	// <0x5c67bc>|0x0ec|-0x085:'149'
	// <0>
	// <1>
	// <2>
	// <0x5c6737>|0x067|+0x09d:'153'
	// <0x5c67d4>|0x104|+0x01c:'154'
	// <0>
	// <1>
	// <0x5c67f0>|0x120|+0x02e:'157'
	// <0x5c681e>|0x14e|+0x031:'158'
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
	// <0x5c684f>|0x17f|+0x104:'171'
	// ******
}

// STATE[STUB]
void network_client::process_player_profile( network_core::packet_reader& reader )
{
	// CALL SITE INFO
	// <0x5c699d> -> match_client& < unknown >()
	// ******

	// FUNCTION BODY[0x5c6990]: 5
	// <0x5c6995>|0x005|+0x00c:'176'
	// <0x5c69a1>|0x011|+0x025:'177'
	// <0>
	// <0x5c69c6>|0x036|+0x00e:'179'
	// <0x5c69d4>|0x044|+0x007:'180'
	// ******
}

// STATE[STUB]
void network_client::process_team_bases( network_core::packet_reader& reader )
{
	// FUNCTION BODY[0x5c4750]: 1
	// <0x5c4751>|0x001|+0x010:'185'
	// ******
}

// STATE[STUB]
void on_signed_out(
	connection_error_types_enum			__formal,
	handshaking_error_types_enum		__formal,
	socket_error_types_enum				__formal,
	login_server_message_types_enum		__formal
)
{
	// FUNCTION BODY[0x5c4210]: 0
	// <0x5c4210>|0x000|+0x000:'189'	{
	// <0x5c4210>|0x000|      :'190'	}
	// ******
}

// STATE[STUB]
void network_client::disconnect( )
{
	// CALL SITE INFO
	// <0x5c4a23> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5c4a00]: 19
	// <0x5c4a0d>|0x00d|+0x00b:'194'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c4a18>|0x018|+0x00d:'200'
	// <0>
	// <0x5c4a25>|0x025|+0x03b:'202'
	// <0x5c4a60>|0x060|+0x00b:'203'
	// <0>
	// <1>
	// <0x5c4a6b>|0x06b|+0x00b:'206'
	// <0x5c4a76>|0x076|+0x00b:'207'
	// <0x5c4a81>|0x081|+0x070:'208'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void network_client::process_player_kill( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c6632> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c65f0]: 12
	// <0x5c65f3>|0x003|+0x00d:'217'
	// <0x5c6600>|0x010|+0x007:'218'
	// <0x5c6607>|0x017|+0x002:'219'
	// <0>
	// <0x5c6609>|0x019|+0x02b:'221'
	// <0>
	// <1>
	// <2>
	// <0x5c6634>|0x044|+0x01b:'225'
	// <0>
	// <0x5c664f>|0x05f|+0x023:'227'
	// <0x5c6672>|0x082|+0x010:'228'
	// ******
}

// STATE[STUB]
void network_client::process_player_hit( network_core::packet_reader& packet )
{
	// LOCALS
	// hit_info 						info
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c4dbb> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c4d90]: 5
	// <0x5c4d93>|0x003|+0x009:'233'
	// <0x5c4d9c>|0x00c|+0x00e:'234'
	// <0x5c4daa>|0x01a|+0x013:'235'
	// <0x5c4dbd>|0x02d|+0x011:'236'
	// <0x5c4dce>|0x03e|+0x016:'237'
	// ******
}

// STATE[STUB]
void network_client::process_affect_damage_model( network_core::packet_reader& packet )
{
	// LOCALS
	// char[16] 						body_part_name
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c4954> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c4930]: 8
	// <0x5c4937>|0x007|+0x009:'242'
	// <0x5c4940>|0x010|+0x016:'243'
	// <0>
	// <0x5c4956>|0x026|+0x01d:'245'
	// <0x5c4973>|0x043|+0x00d:'246'
	// <0x5c4980>|0x050|+0x008:'247'
	// <0x5c4988>|0x058|+0x014:'248'
	// <0x5c499c>|0x06c|+0x010:'249'
	// ******
}

// STATE[STUB]
void network_client::process_player_respawn( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c5c19> -> player_ptr < unknown >( const u8 ) const
	// <0x5c5c25> -> void < unknown >( network_core::packet_reader& )
	// ******

	// FUNCTION BODY[0x5c5bf0]: 12
	// <0x5c5bf9>|0x009|+0x00b:'254'
	// <0x5c5c04>|0x014|+0x017:'255'
	// <0>
	// <0x5c5c1b>|0x02b|+0x00c:'257'
	// <0>
	// <1>
	// <2>
	// <0x5c5c27>|0x037|+0x014:'261'
	// <0x5c5c3b>|0x04b|+0x003:'262'
	// <0>
	// <0x5c5c3e>|0x04e|+0x01c:'264'
	// <0x5c5c5a>|0x06a|+0x017:'265'
	// ******
}

// STATE[STUB]
void network_client::process_initialize_victory_items( network_core::packet_reader& packet )
{
	// LOCALS
	// float3 							position
	// float4x4 						transform
	// ******

	// CALL SITE INFO
	// <0x5c5881> -> player_ptr < unknown >( const u8 ) const
	// <0x5c59f1> -> void < unknown >()
	// <0x5c59fb> -> void < unknown >( victory_item_core* )
	// ******

	// FUNCTION BODY[0x5c56c0]: 44
	// <0x5c56cf>|0x00f|+0x00a:'270'
	// <0x5c56d9>|0x019|+0x008:'271'
	// <0>
	// <0x5c56e1>|0x021|+0x026:'273'
	// <0>
	// <0x5c5707>|0x047|+0x009:'275'
	// <0x5c5710>|0x050|+0x1f7:'276'
	// <0x5c5907>|0x247|-0x1e7:'276'
	// <0>
	// <0x5c5720>|0x060|+0x009:'278'
	// <0x5c5729>|0x069|+0x006:'279'
	// <0x5c572f>|0x06f|+0x01b:'280'
	// <0>
	// <1>
	// <0x5c574a>|0x08a|+0x009:'283'
	// <0>
	// <1>
	// <0x5c5753>|0x093|+0x0ab:'286'
	// <0x5c57fe>|0x13e|+0x03e:'287'
	// <0>
	// <0x5c583c>|0x17c|+0x00c:'289'
	// <0>
	// <0x5c5848>|0x188|+0x028:'291'
	// <0x5c5870>|0x1b0|+0x052:'292'
	// <0x5c58c2>|0x202|+0x019:'293'
	// <0x5c58db>|0x21b|+0x00e:'294'
	// <0x5c58e9>|0x229|+0x029:'295'
	// <0>
	// <1>
	// <0x5c5912>|0x252|+0x009:'298'
	// <0>
	// <0x5c591b>|0x25b|+0x103:'300'
	// <0x5c5a1e>|0x35e|-0x0ee:'300'
	// <0>
	// <0x5c5930>|0x270|+0x009:'302'
	// <0x5c5939>|0x279|+0x060:'303'
	// <0x5c5999>|0x2d9|+0x009:'304'
	// <0x5c59a2>|0x2e2|+0x075:'305'
	// <0x5c5a17>|0x357|-0x067:'305'
	// <0>
	// <0x5c59b0>|0x2f0|+0x009:'307'
	// <0x5c59b9>|0x2f9|+0x028:'308'
	// <0x5c59e1>|0x321|+0x009:'309'
	// <0x5c59ea>|0x32a|+0x009:'310'
	// <0x5c59f3>|0x333|+0x00a:'311'
	// <0x5c59fd>|0x33d|+0x02c:'312'
	// <0>
	// ******
}

// STATE[STUB]
void network_client::process_base_capture_progress( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c4720]: 3
	// <0x5c4721>|0x001|+0x00c:'318'
	// <0x5c472d>|0x00d|+0x008:'319'
	// <0x5c4735>|0x015|+0x015:'320'
	// ******
}

// STATE[STUB]
void network_client::process_match_time( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c43c0]: 1
	// <0x5c43c0>|0x000|+0x00b:'325'
	// ******
}

// STATE[STUB]
void network_client::process_respawn_timer( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c4390]: 2
	// <0x5c4390>|0x000|+0x00b:'331'
	// <0x5c439b>|0x00b|+0x00d:'332'
	// ******
}

// STATE[STUB]
void network_client::process_match_wait_timer( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c4770]: 3
	// <0x5c4770>|0x000|+0x00d:'338'
	// <0x5c477d>|0x00d|+0x008:'339'
	// <0x5c4785>|0x015|+0x01b:'340'
	// ******
}

// STATE[STUB]
void `dynamic initializer for 'cc_warmup_camera_position''( )
{
	// FUNCTION BODY[0x7d8d60]
	// <0x7d8d60>|0x000|      :'346'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 'cc_warmup_camera_position''( )
{
	// FUNCTION BODY[0x7f0880]
	// <0x7d8e10>|0x000|      :'347'	{
	// ******
}

// STATE[STUB]
void network_client::setup_camera_for_warmup( )
{
	// LOCALS
	// float3 							target
	// physics::closest_ray_result 		ray_result
	// float3 							direction
	// float3 							position
	// const float 						length
	// ******

	// CALL SITE INFO
	// <0x5c4661> -> physics::closest_ray_result < unknown >( float3 const&, float3 const&, const float, u16, u16 )
	// ******

	// FUNCTION BODY[0x5c43f0]: 21
	// <0>
	// <1>
	// <0x5c43f3>|0x003|+0x042:'353'
	// <0x5c4435>|0x045|+0x13f:'354'
	// <0>
	// <0x5c4574>|0x184|+0x018:'356'
	// <0x5c458c>|0x19c|+0x04a:'357'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5c45d6>|0x1e6|+0x08d:'366'
	// <0>
	// <0x5c4663>|0x273|+0x007:'368'
	// <0x5c466a>|0x27a|+0x066:'369'
	// <0>
	// <0x5c46d0>|0x2e0|+0x017:'371'
	// ******
}

// STATE[STUB]
void network_client::process_game_status( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c5b30]: 24
	// <0x5c5b30>|0x000|+0x000:'375'	{
	// <0x5c5b30>|0x000|+0x011:'376'
	// <0>
	// <0x5c5b41>|0x011|+0x010:'378'
	// <0>
	// <1>
	// <0x5c5b51>|0x021|+0x009:'381'
	// <0>
	// <0x5c5b5a>|0x02a|+0x005:'383'
	// <0>
	// <0x5c5b5f>|0x02f|+0x009:'385'
	// <0x5c5b68>|0x038|+0x021:'386'
	// <0>
	// <0x5c5b89>|0x059|+0x00b:'388'
	// <0x5c5b94>|0x064|+0x01c:'389'
	// <0>
	// <1>
	// <0x5c5bb0>|0x080|+0x004:'392'
	// <0>
	// <0x5c5bb4>|0x084|+0x009:'394'
	// <0x5c5bbd>|0x08d|+0x012:'395'
	// <0x5c5bcf>|0x09f|-0x02c:'396'
	// <0>
	// <1>
	// <0x5c5ba3>|0x073|+0x033:'399'
	// <0x5c5bd6>|0x0a6|-0x02d:'399'
	// <0x5c5ba9>|0x079|+0x033:'400'
	// <0x5c5bdc>|0x0ac|      :'400'	}
	// ******
}

// STATE[STUB]
void network_client::process_player_kd_stats( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c4350]: 5
	// <0x5c4351>|0x001|+0x00c:'404'
	// <0x5c435d>|0x00d|+0x009:'405'
	// <0x5c4366>|0x016|+0x008:'406'
	// <0x5c436e>|0x01e|+0x004:'407'
	// <0x5c4372>|0x022|+0x016:'408'
	// ******
}

// STATE[STUB]
void network_client::process_victory_item_take_or_put( network_core::packet_reader& packet )
{
	// LOCALS
	// s8 								team_1_points
	// s8 								team_2_points
	// player_ptr 						current_player
	// float4x4 						item_transform
	// victory_item_ptr 				item
	// ******

	// CALL SITE INFO
	// <0x5c54bc> -> game_team_id < unknown >()
	// <0x5c54d1> -> game_team_id < unknown >()
	// <0x5c5510> -> player_ptr < unknown >( const u8 ) const
	// <0x5c555c> -> void < unknown >()
	// <0x5c5574> -> victory_item_core* < unknown >()
	// <0x5c558d> -> player_ptr < unknown >( const u8 ) const
	// <0x5c55e5> -> game_team_id < unknown >() const
	// <0x5c5657> -> void < unknown >( victory_item_core* )
	// ******

	// FUNCTION BODY[0x5c53d0]: 58
	// <0x5c53d3>|0x003|+0x00a:'413'
	// <0x5c53dd>|0x00d|+0x00c:'414'
	// <0x5c53e9>|0x019|+0x010:'415'
	// <0x5c53f9>|0x029|+0x004:'416'
	// <0>
	// <1>
	// <0x5c53fd>|0x02d|+0x015:'419'
	// <0x5c5412>|0x042|+0x006:'420'
	// <0>
	// <0x5c5418>|0x048|+0x027:'422'
	// <0>
	// <0x5c543f>|0x06f|+0x066:'424'
	// <0>
	// <0x5c54a5>|0x0d5|+0x004:'426'
	// <0x5c54a9>|0x0d9|+0x005:'427'
	// <0x5c54ae>|0x0de|+0x010:'428'
	// <0x5c54be>|0x0ee|+0x015:'429'
	// <0x5c54d3>|0x103|+0x024:'430'
	// <0>
	// <1>
	// <0x5c54f7>|0x127|+0x007:'433'
	// <0x5c54fe>|0x12e|+0x02c:'434'
	// <0>
	// <0x5c552a>|0x15a|+0x019:'436'
	// <0x5c5543>|0x173|+0x00e:'437'
	// <0>
	// <0x5c5551>|0x181|+0x004:'439'
	// <0>
	// <0x5c5555>|0x185|+0x009:'441'
	// <0x5c555e>|0x18e|+0x00a:'442'
	// <0>
	// <0x5c5568>|0x198|+0x005:'444'
	// <0>
	// <0x5c556d>|0x19d|+0x009:'446'
	// <0>
	// <1>
	// <0x5c5576>|0x1a6|+0x005:'449'
	// <0>
	// <0x5c557b>|0x1ab|+0x014:'451'
	// <0x5c558f>|0x1bf|+0x00e:'452'
	// <0>
	// <0x5c559d>|0x1cd|+0x019:'454'
	// <0x5c55b6>|0x1e6|+0x022:'455'
	// <0>
	// <0x5c55d8>|0x208|+0x004:'457'
	// <0x5c55dc>|0x20c|+0x00f:'458'
	// <0>
	// <1>
	// <0x5c55eb>|0x21b|+0x050:'461'
	// <0>
	// <0x5c563b>|0x26b|+0x006:'463'
	// <0>
	// <0x5c5641>|0x271|+0x00e:'465'
	// <0x5c564f>|0x27f|+0x00a:'466'
	// <0>
	// <0x5c5659>|0x289|+0x009:'468'
	// <0>
	// <0x5c5662>|0x292|+0x022:'470'
	// ******
}

// STATE[STUB]
void network_client::send_sync_request( )
{
	// FUNCTION BODY[0x5c4840]: 6
	// <0x5c4845>|0x005|+0x02f:'475'
	// <0>
	// <1>
	// <0x5c4874>|0x034|+0x015:'478'
	// <0x5c4889>|0x049|+0x01c:'479'
	// <0x5c48a5>|0x065|+0x012:'480'
	// ******
}

// STATE[STUB]
void network_client::process_sync_response( network_core::packet_reader& packet )
{
	// FUNCTION BODY[0x5c5a40]: 12
	// <0x5c5a4b>|0x00b|+0x036:'485'
	// <0>
	// <1>
	// <0x5c5a81>|0x041|+0x026:'488'
	// <0>
	// <1>
	// <0x5c5aa7>|0x067|+0x038:'491'
	// <0x5c5adf>|0x09f|+0x00f:'492'
	// <0>
	// <0x5c5aee>|0x0ae|+0x016:'494'
	// <0>
	// <0x5c5b04>|0x0c4|+0x017:'496'
	// ******
}

// STATE[STUB]
void network_client::send_local_player_input(
	player_input const&		input,
	const u32				time_in_ms,
	float4x4 const&			transform,
	const float				look_pitch
)
{
	// FUNCTION BODY[0x5c4250]: 12
	// <0>
	// <1>
	// <2>
	// <0x5c4257>|0x007|+0x02c:'509'
	// <0x5c4283>|0x033|+0x01d:'510'
	// <0>
	// <0x5c42a0>|0x050|+0x05b:'512'
	// <0x5c42fb>|0x0ab|+0x006:'513'
	// <0x5c4301>|0x0b1|+0x008:'514'
	// <0x5c4309>|0x0b9|+0x007:'515'
	// <0>
	// <0x5c4310>|0x0c0|+0x03a:'517'
	// ******
}

// STATE[STUB]
void network_client::process_player_action( network_core::packet_reader& packet, const u32 time_in_ms )
{
	// LOCALS
	// server_player_update 			action
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c523d> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c5210]: 56
	// <0x5c5210>|0x000|+0x00c:'521'	{
	// <0x5c521c>|0x00c|+0x00d:'522'
	// <0x5c5229>|0x019|+0x016:'523'
	// <0>
	// <0x5c523f>|0x02f|+0x015:'525'
	// <0x5c5254>|0x044|+0x00a:'526'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5c525e>|0x04e|+0x00e:'533'
	// <0>
	// <0x5c526c>|0x05c|+0x0e6:'535'
	// <0>
	// <1>
	// <2>
	// <0x5c5352>|0x142|+0x008:'539'
	// <0>
	// <0x5c535a>|0x14a|+0x045:'541'
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
	// <0x5c539f>|0x18f|-0x062:'559'
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
	// <17>
	// <0x5c533d>|0x12d|+0x04d:'578'
	// <0x5c538a>|0x17a|+0x028:'578'
	// <0x5c53b2>|0x1a2|      :'578'	}
	// ******
}

// STATE[STUB]
void network_client::send_player_inputs( )
{
	// FUNCTION BODY[0x5c47e0]: 9
	// <0x5c47e3>|0x003|+0x01d:'582'
	// <0>
	// <0x5c4800>|0x020|+0x00b:'584'
	// <0x5c480b>|0x02b|+0x008:'585'
	// <0>
	// <0x5c4813>|0x033|+0x01b:'587'
	// <0>
	// <1>
	// <0x5c482e>|0x04e|+0x00f:'590'
	// ******
}

// STATE[STUB]
void network_client::tick( const u32 current_time_in_ms, const bool is_game_paused )
{
	// LOCALS
	// u8 								id
	// player_ptr 						player
	// ******

	// STATICS
	// static u32 						lobby_resolve_time = <0x4c26630>;
	// static u32 						messaging_resolve_time = <0x4c26634>;
	// static const u32 				min_time_delta_in_ms = <0x10000>;
	// ******

	// CALL SITE INFO
	// <0x5c620b> -> network::login_client& < unknown >()
	// <0x5c6221> -> lobby_client& < unknown >()
	// <0x5c6237> -> lobby_client& < unknown >()
	// <0x5c6249> -> lobby_client& < unknown >()
	// <0x5c6261> -> network::login_client& < unknown >()
	// <0x5c629d> -> lobby_client& < unknown >()
	// <0x5c6370> -> match_client& < unknown >()
	// <0x5c6395> -> messaging_client& < unknown >()
	// <0x5c63b6> -> messaging_client& < unknown >()
	// <0x5c63e3> -> match_client& < unknown >()
	// <0x5c641a> -> match_client& < unknown >()
	// <0x5c6441> -> match_client& < unknown >()
	// <0x5c64d5> -> match_client& < unknown >()
	// <0x5c6531> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c61e0]: 122
	// <0x5c61e0>|0x000|+0x00a:'594'	{
	// <0x5c61ea>|0x00a|+0x007:'595'
	// <0>
	// <0x5c61f1>|0x011|+0x013:'597'
	// <0>
	// <0x5c6204>|0x024|+0x016:'599'
	// <0>
	// <1>
	// <0x5c621a>|0x03a|+0x010:'602'
	// <0>
	// <0x5c622a>|0x04a|+0x018:'604'
	// <0>
	// <0x5c6242>|0x062|+0x00f:'606'
	// <0x5c6251>|0x071|+0x022:'607'
	// <0x5c6273>|0x093|+0x00a:'608'
	// <0x5c627d>|0x09d|+0x005:'609'
	// <0>
	// <1>
	// <0x5c6282>|0x0a2|+0x014:'612'
	// <0>
	// <0x5c6296>|0x0b6|+0x014:'614'
	// <0x5c62aa>|0x0ca|+0x00e:'615'
	// <0x5c62b8>|0x0d8|+0x0ae:'616'
	// <0>
	// <1>
	// <0x5c6366>|0x186|+0x02c:'619'
	// <0x5c6392>|0x1b2|+0x00e:'620'
	// <0>
	// <1>
	// <0x5c63a0>|0x1c0|+0x00f:'623'
	// <0>
	// <0x5c63af>|0x1cf|+0x025:'625'
	// <0x5c63d4>|0x1f4|-0x06b:'626'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c6369>|0x189|+0x014:'631'
	// <0x5c637d>|0x19d|+0x05f:'632'
	// <0>
	// <1>
	// <2>
	// <0x5c63dc>|0x1fc|+0x014:'636'
	// <0x5c63f0>|0x210|+0x00c:'637'
	// <0>
	// <0x5c63fc>|0x21c|+0x011:'639'
	// <0>
	// <0x5c640d>|0x22d|+0x02d:'641'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c643a>|0x25a|+0x019:'646'
	// <0x5c6453>|0x273|+0x00c:'647'
	// <0x5c645f>|0x27f|+0x00f:'648'
	// <0>
	// <1>
	// <2>
	// <0x5c646e>|0x28e|+0x00d:'652'
	// <0x5c647b>|0x29b|+0x011:'653'
	// <0x5c648c>|0x2ac|+0x00c:'654'
	// <0>
	// <1>
	// <0x5c6498>|0x2b8|+0x019:'657'
	// <0x5c64b1>|0x2d1|+0x007:'658'
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
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <0x5c64b8>|0x2d8|+0x016:'686'
	// <0x5c64ce>|0x2ee|+0x01e:'687'
	// <0>
	// <0x5c64ec>|0x30c|+0x01f:'689'
	// <0>
	// <0x5c650b>|0x32b|+0x00f:'691'
	// <0>
	// <1>
	// <2>
	// <0x5c651a>|0x33a|+0x006:'695'
	// <0>
	// <0x5c6520>|0x340|+0x013:'697'
	// <0x5c6533>|0x353|+0x053:'698'
	// <0x5c6586>|0x3a6|-0x042:'699'
	// <0>
	// <1>
	// <0x5c6544>|0x364|+0x00f:'702'
	// <0>
	// <0x5c6553>|0x373|+0x012:'704'
	// <0>
	// <1>
	// <0x5c6565>|0x385|+0x00f:'707'
	// <0x5c6574>|0x394|+0x007:'708'
	// <0x5c657b>|0x39b|+0x049:'709'
	// <0>
	// <0x5c65c4>|0x3e4|-0x0b8:'711'
	// <0x5c650c>|0x32c|+0x0d1:'712'
	// <0x5c65dd>|0x3fd|-0x254:'712'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c6389>|0x1a9|+0x0a8:'717'
	// <0x5c6431>|0x251|+0x0e0:'717'
	// <0x5c6511>|0x331|+0x0d2:'717'
	// <0x5c65e3>|0x403|      :'717'	}
	// ******
}

// STATE[STUB]
void network_client::initiate_kill_current_player( )
{
	// FUNCTION BODY[0x5c47b0]: 6
	// <0x5c47b0>|0x000|+0x010:'721'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c47c0>|0x010|+0x01f:'726'
	// ******
}

// STATE[STUB]
void network_client::unload( )
{
	// LOCALS
	// u8 								id
	// ******

	// CALL SITE INFO
	// <0x5c6174> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c6150]: 8
	// <0x5c615e>|0x00e|+0x005:'731'
	// <0x5c6163>|0x013|+0x059:'732'
	// <0x5c61bc>|0x06c|+0x016:'733'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
bool network_client::is_player_local( const u8 player_id ) const
{
	return false;

	// FUNCTION BODY[0x5c4220]: 1
	// <0x5c4220>|0x000|+0x000:'742'	{
	// <0x5c4220>|0x000|+0x024:'743'
	// <0x5c4244>|0x024|-0x003:'743'
	// <0x5c4241>|0x021|+0x005:'744'
	// <0x5c4246>|0x026|      :'744'	}
	// ******
}

// STATE[STUB]
void network_client::player_visibility_change( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c5f68> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c5f40]: 14
	// <0x5c5f40>|0x000|+0x003:'917'	{
	// <0x5c5f43>|0x003|+0x00e:'918'
	// <0x5c5f51>|0x011|+0x006:'919'
	// <0>
	// <0x5c5f57>|0x017|+0x013:'921'
	// <0x5c5f6a>|0x02a|+0x04c:'922'
	// <0x5c5fb6>|0x076|+0x031:'923'
	// <0x5c5fe7>|0x0a7|-0x06c:'923'
	// <0>
	// <1>
	// <0x5c5f7b>|0x03b|+0x011:'926'
	// <0x5c5f8c>|0x04c|+0x004:'927'
	// <0x5c5f90>|0x050|+0x013:'928'
	// <0>
	// <0x5c5fa3>|0x063|-0x00e:'930'
	// <0>
	// <0x5c5f95>|0x055|+0x013:'932'
	// <0x5c5fa8>|0x068|+0x03a:'932'
	// <0x5c5fe2>|0x0a2|+0x013:'932'
	// <0x5c5ff5>|0x0b5|      :'932'	}
	// ******
}

// STATE[STUB]
void network_client::on_trap_placed( network_core::packet_reader& packet )
{
	// LOCALS
	// float3 							angles
	// float3 							position
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c4d13> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c4cb0]: 10
	// <0x5c4cb3>|0x003|+0x00e:'936'
	// <0x5c4cc1>|0x011|+0x008:'937'
	// <0x5c4cc9>|0x019|+0x007:'938'
	// <0x5c4cd0>|0x020|+0x015:'939'
	// <0x5c4ce5>|0x035|+0x013:'940'
	// <0>
	// <0x5c4cf8>|0x048|+0x01d:'942'
	// <0>
	// <1>
	// <0x5c4d15>|0x065|+0x024:'945'
	// ******
}

// STATE[STUB]
void network_client::on_trap_removed( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c4c44> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c4c10]: 8
	// <0x5c4c13>|0x003|+0x00f:'950'
	// <0x5c4c22>|0x012|+0x006:'951'
	// <0x5c4c28>|0x018|+0x007:'952'
	// <0>
	// <0x5c4c2f>|0x01f|+0x017:'954'
	// <0>
	// <1>
	// <0x5c4c46>|0x036|+0x01a:'957'
	// ******
}

// STATE[STUB]
void network_client::on_trap_fired( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c5cf4> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c5cc0]: 8
	// <0x5c5cc3>|0x003|+0x00f:'962'
	// <0x5c5cd2>|0x012|+0x006:'963'
	// <0x5c5cd8>|0x018|+0x007:'964'
	// <0>
	// <0x5c5cdf>|0x01f|+0x017:'966'
	// <0>
	// <1>
	// <0x5c5cf6>|0x036|+0x01a:'969'
	// ******
}

// STATE[STUB]
void network_client::on_trap_disarmed( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c4ba4> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c4b70]: 8
	// <0x5c4b73>|0x003|+0x00f:'974'
	// <0x5c4b82>|0x012|+0x006:'975'
	// <0x5c4b88>|0x018|+0x007:'976'
	// <0>
	// <0x5c4b8f>|0x01f|+0x017:'978'
	// <0>
	// <1>
	// <0x5c4ba6>|0x036|+0x01a:'981'
	// ******
}

// STATE[STUB]
void network_client::game_world_object_state_arrived( network_core::packet_reader& reader )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c4b1b> -> player_ptr < unknown >( const u8 ) const
	// ******

	// FUNCTION BODY[0x5c4b00]: 4
	// <0x5c4b01>|0x001|+0x008:'986'
	// <0x5c4b09>|0x009|+0x014:'987'
	// <0>
	// <0x5c4b1d>|0x01d|+0x009:'989'
	// ******
}

// STATE[STUB]
void network_client::on_world_sync_request( )
{
	// LOCALS
	// u8 								id
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c5d91> -> player_ptr < unknown >( const u8 ) const
	// <0x5c5f00> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5c5d60]: 30
	// <0>
	// <1>
	// <0x5c5d67>|0x007|+0x00a:'996'
	// <0>
	// <0x5c5d71>|0x011|+0x00f:'998'
	// <0>
	// <0x5c5d80>|0x020|+0x013:'1000'
	// <0x5c5d93>|0x033|+0x043:'1001'
	// <0x5c5dd6>|0x076|-0x032:'1002'
	// <0>
	// <0x5c5da4>|0x044|+0x005:'1004'
	// <0x5c5da9>|0x049|+0x010:'1005'
	// <0x5c5db9>|0x059|+0x00b:'1006'
	// <0x5c5dc4>|0x064|+0x04d:'1007'
	// <0>
	// <0x5c5e11>|0x0b1|+0x03f:'1009'
	// <0x5c5e50>|0x0f0|+0x07e:'1010'
	// <0x5c5ece>|0x16e|-0x038:'1010'
	// <0>
	// <0x5c5e96>|0x136|+0x040:'1012'
	// <0>
	// <1>
	// <0x5c5ed6>|0x176|+0x009:'1015'
	// <0x5c5edf>|0x17f|+0x011:'1016'
	// <0>
	// <0x5c5ef0>|0x190|+0x00b:'1018'
	// <0x5c5efb>|0x19b|+0x015:'1019'
	// <0>
	// <1>
	// <2>
	// <0x5c5f10>|0x1b0|+0x017:'1023'
	// ******
}

// STATE[STUB]
void network_client::damage_model_state_arrived( network_core::packet_reader& packet )
{
	// LOCALS
	// player_ptr 						player
	// ******

	// CALL SITE INFO
	// <0x5c48db> -> player_ptr < unknown >( const u8 ) const
	// <0x5c48e5> -> damage_model_ptr const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5c48c0]: 4
	// <0x5c48c1>|0x001|+0x008:'1028'
	// <0x5c48c9>|0x009|+0x014:'1029'
	// <0>
	// <0x5c48dd>|0x01d|+0x012:'1031'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	char[32]
	// 	account_name_type;

	// typedef
	// 	long
	// 	counter_type;

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

	// typedef
	// 	vostok::memory::multi_threading_single_size_allocator_policy< vostok::memory::single_size_buffer_allocator< 128, vostok::threading::simple_lock >::node >::free_list_type
	// 	free_list_type;

	// ******

} // namespace survarium
