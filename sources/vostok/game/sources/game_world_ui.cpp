////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_world_ui.h"
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor

namespace survarium {

// TU statics 'is_ui_minimap_rotable_old' / 's_is_ui_minimap_rotable'
// (compiler-generated dynamic initializer + atexit destructor); a matcher
// recovers their types/initializers from the init asm.
/*
// STATE[STUB]
void `dynamic initializer for 'is_ui_minimap_rotable_old''( )
{
	// FUNCTION BODY[0x7d8a60]
	// <0x7d8a60>|0x000|      :'28'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_is_ui_minimap_rotable''( )
{
	// FUNCTION BODY[0x7f0600]
	// <0x7d8a70>|0x000|      :'35'	{
	// ******
}
*/

// STATE[STUB]
 game_world_ui::game_world_ui( game_world& w ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_world( w )
{
	// FUNCTION BODY[0x5d2a10]: 1
	// <0x5d2a5a>|0x04a|+0x005:'51'
	// ******
}

// STATE[STUB]
void game_world_ui::initialize_resources( resources::unmanaged_resource_ptr const& game_hud )
{
	// FUNCTION BODY[0x5d2a70]: 9
	// <0x5d2a71>|0x001|+0x06f:'56'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d2ae0>|0x070|+0x01c:'62'
	// <0x5d2afc>|0x08c|+0x015:'63'
	// <0x5d2b11>|0x0a1|+0x015:'64'
	// ******
}

// STATE[STUB]
void game_world_ui::initialize( match_options& options )
{
	// LOCALS
	// flash_value 						victory_items_count_val
	// flash_value 						game_mode_val
	// game_team_id 					local_player_team
	// flash_value 						players_array
	// wchar_t[512] 					team_name
	// flash_value 						v
	// wchar_t[32] 						profile_name_w
	// flash_value 						player_item_property
	// flash_value 						player_item
	// ******

	// FUNCTION BODY[0x5d2320]: 76
	// <0>
	// <1>
	// <2>
	// <0x5d2326>|0x006|+0x03c:'79'
	// <0x5d2362>|0x042|+0x010:'80'
	// <0>
	// <0x5d2372>|0x052|+0x022:'82'
	// <0>
	// <0x5d2394>|0x074|+0x01b:'84'
	// <0x5d23af>|0x08f|+0x010:'85'
	// <0>
	// <0x5d23bf>|0x09f|+0x022:'87'
	// <0>
	// <0x5d23e1>|0x0c1|+0x023:'89'
	// <0x5d2404>|0x0e4|+0x02d:'90'
	// <0>
	// <1>
	// <0x5d2431>|0x111|+0x009:'93'
	// <0x5d243a>|0x11a|+0x00b:'94'
	// <0x5d2445>|0x125|+0x00f:'95'
	// <0>
	// <0x5d2454>|0x134|+0x00f:'97'
	// <0>
	// <1>
	// <2>
	// <0x5d2463>|0x143|+0x005:'101'
	// <0x5d2468>|0x148|+0x010:'102'
	// <0>
	// <1>
	// <2>
	// <0x5d2478>|0x158|+0x003:'106'
	// <0x5d247b>|0x15b|+0x03b:'107'
	// <0>
	// <1>
	// <0x5d24b6>|0x196|+0x003:'110'
	// <0x5d24b9>|0x199|+0x036:'111'
	// <0>
	// <1>
	// <2>
	// <0x5d24ef>|0x1cf|+0x031:'115'
	// <0>
	// <1>
	// <2>
	// <0x5d2520>|0x200|+0x00a:'119'
	// <0>
	// <1>
	// <2>
	// <0x5d252a>|0x20a|+0x021:'123'
	// <0>
	// <1>
	// <0x5d254b>|0x22b|+0x021:'126'
	// <0>
	// <0x5d256c>|0x24c|+0x028:'128'
	// <0x5d2594>|0x274|+0x038:'129'
	// <0>
	// <0x5d25cc>|0x2ac|+0x034:'131'
	// <0x5d2600>|0x2e0|+0x038:'132'
	// <0>
	// <1>
	// <0x5d2638>|0x318|+0x021:'135'
	// <0x5d2659>|0x339|+0x086:'136'
	// <0x5d26df>|0x3bf|+0x02e:'137'
	// <0>
	// <0x5d270d>|0x3ed|+0x025:'139'
	// <0x5d2732>|0x412|+0x03f:'140'
	// <0>
	// <0x5d2771>|0x451|+0x025:'142'
	// <0x5d2796>|0x476|+0x036:'143'
	// <0>
	// <0x5d27cc>|0x4ac|+0x025:'145'
	// <0x5d27f1>|0x4d1|+0x036:'146'
	// <0>
	// <0x5d2827>|0x507|+0x01a:'148'
	// <0x5d2841>|0x521|+0x061:'149'
	// <0>
	// <0x5d28a2>|0x582|+0x01d:'151'
	// ******
}

// STATE[STUB]
void game_world_ui::initialize_base_points( network_core::packet_reader& packet )
{
	// LOCALS
	// u32 								point_id
	// base_point_stats 				stats
	// ******

	// FUNCTION BODY[0x5d19d0]: 20
	// <0x5d19d0>|0x000|+0x011:'156'
	// <0>
	// <0x5d19e1>|0x011|+0x002:'158'
	// <0>
	// <1>
	// <0x5d19e3>|0x013|+0x070:'161'
	// <0x5d1a53>|0x083|-0x063:'161'
	// <0>
	// <1>
	// <0x5d19f0>|0x020|+0x00b:'164'
	// <0x5d19fb>|0x02b|+0x008:'165'
	// <0x5d1a03>|0x033|+0x008:'166'
	// <0x5d1a0b>|0x03b|+0x008:'167'
	// <0>
	// <0x5d1a13>|0x043|+0x01b:'169'
	// <0>
	// <1>
	// <0x5d1a2e>|0x05e|+0x016:'172'
	// <0>
	// <0x5d1a44>|0x074|+0x016:'174'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::add_victory_points( s8 team_1_points, s8 team_2_points )
{
	// LOCALS
	// flash_value[2] 					args
	// ******

	// CALL SITE INFO
	// <0x5d4f4f> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x5d4f10]: 15
	// <0x5d4f18>|0x008|+0x058:'180'
	// <0>
	// <0x5d4f70>|0x060|+0x007:'182'
	// <0>
	// <0x5d4f77>|0x067|+0x01a:'184'
	// <0x5d4f91>|0x081|+0x025:'185'
	// <0x5d4fb6>|0x0a6|+0x041:'186'
	// <0x5d4ff7>|0x0e7|+0x027:'187'
	// <0>
	// <1>
	// <2>
	// <0x5d501e>|0x10e|+0x030:'191'
	// <0x5d504e>|0x13e|+0x03d:'192'
	// <0x5d508b>|0x17b|+0x027:'193'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::set_victory_points( s8 team_1_points, s8 team_2_points )
{
	// LOCALS
	// flash_value[2] 					args
	// game_team_id 					local_player_team
	// ******

	// CALL SITE INFO
	// <0x5d4d83> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x5d4d40]: 12
	// <0x5d4d4d>|0x00d|+0x059:'199'
	// <0>
	// <0x5d4da6>|0x066|+0x003:'201'
	// <0x5d4da9>|0x069|+0x01a:'202'
	// <0x5d4dc3>|0x083|+0x024:'203'
	// <0x5d4de7>|0x0a7|+0x03b:'204'
	// <0x5d4e22>|0x0e2|+0x029:'205'
	// <0>
	// <1>
	// <0x5d4e4b>|0x10b|+0x02c:'208'
	// <0x5d4e77>|0x137|+0x037:'209'
	// <0x5d4eae>|0x16e|+0x029:'210'
	// ******
}

// STATE[STUB]
 game_world_ui::~game_world_ui( )
{
	// FUNCTION BODY[0x5d2990]: 3
	// <0x5d2990>|0x000|+0x00b:'214'	{
	// <0>
	// <1>
	// <2>
	// <0x5d299b>|0x00b|      :'218'	}
	// ******
}

// STATE[STUB]
void game_world_ui::set_base_capture_progress( u32 progress, u32 point_id )
{
	// LOCALS
	// char[64] 						buff
	// flash_value[3] 					args
	// flash_value[3] 					args
	// ******

	// FUNCTION BODY[0x5d1800]: 27
	// <0>
	// <1>
	// <2>
	// <0x5d1817>|0x017|+0x011:'225'
	// <0>
	// <0x5d1828>|0x028|+0x002:'227'
	// <0x5d182a>|0x02a|+0x01d:'228'
	// <0x5d1847>|0x047|+0x002:'229'
	// <0x5d1849>|0x049|+0x00f:'230'
	// <0>
	// <0x5d1858>|0x058|+0x022:'232'
	// <0>
	// <0x5d187a>|0x07a|+0x038:'234'
	// <0x5d18b2>|0x0b2|+0x013:'235'
	// <0x5d18c5>|0x0c5|+0x02c:'236'
	// <0x5d18f1>|0x0f1|+0x004:'237'
	// <0>
	// <1>
	// <0x5d18f5>|0x0f5|+0x002:'240'
	// <0x5d18f7>|0x0f7|+0x009:'241'
	// <0>
	// <0x5d1900>|0x100|+0x013:'243'
	// <0x5d1913>|0x113|+0x02c:'244'
	// <0x5d193f>|0x13f|+0x016:'245'
	// <0x5d1955>|0x155|+0x02f:'246'
	// <0x5d1984>|0x184|+0x02b:'247'
	// <0x5d19af>|0x1af|+0x013:'248'
	// ******
}

// STATE[STUB]
void game_world_ui::set_match_time( u32 time_left_ms )
{
	// LOCALS
	// flash_value 						match_time_str
	// char[64] 						buff
	// ******

	// FUNCTION BODY[0x5d12f0]: 15
	// <0>
	// <1>
	// <2>
	// <0x5d12f5>|0x005|+0x004:'256'
	// <0x5d12f9>|0x009|+0x011:'257'
	// <0x5d130a>|0x01a|+0x002:'258'
	// <0>
	// <0x5d130c>|0x01c|+0x027:'260'
	// <0x5d1333>|0x043|+0x018:'261'
	// <0x5d134b>|0x05b|-0x012:'261'
	// <0x5d1339>|0x049|+0x027:'262'
	// <0x5d1360>|0x070|+0x013:'262'
	// <0>
	// <1>
	// <2>
	// <0x5d1373>|0x083|+0x01d:'266'
	// <0x5d1390>|0x0a0|+0x021:'267'
	// ******
}

// STATE[STUB]
void game_world_ui::show_pregame( bool b_show )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d1290]: 3
	// <0>
	// <1>
	// <0x5d1293>|0x003|+0x033:'274'
	// ******
}

// STATE[STUB]
void game_world_ui::set_pregame( pcstr str, u32 time_left )
{
	// LOCALS
	// wchar_t[512] 					message
	// flash_value 						message_val
	// wchar_t[512] 					buff
	// ******

	// FUNCTION BODY[0x5d2240]: 12
	// <0x5d224e>|0x00e|+0x00f:'279'
	// <0>
	// <1>
	// <2>
	// <0x5d225d>|0x01d|+0x033:'283'
	// <0>
	// <1>
	// <0x5d2290>|0x050|+0x01e:'286'
	// <0>
	// <0x5d22ae>|0x06e|+0x005:'288'
	// <0x5d22b3>|0x073|+0x018:'289'
	// <0x5d22cb>|0x08b|+0x01c:'290'
	// ******
}

// STATE[STUB]
void game_world_ui::set_respawn_time( u32 time_left )
{
	// LOCALS
	// char[64] 						buff
	// flash_value 						respawn_time_str
	// ******

	// FUNCTION BODY[0x5d11e0]: 13
	// <0x5d11e0>|0x000|+0x026:'295'
	// <0x5d1206>|0x026|-0x017:'296'
	// <0>
	// <1>
	// <2>
	// <0x5d11ef>|0x00f|+0x004:'300'
	// <0x5d11f3>|0x013|+0x011:'301'
	// <0x5d1204>|0x024|+0x00f:'302'
	// <0x5d1213>|0x033|+0x013:'303'
	// <0>
	// <1>
	// <0x5d1226>|0x046|+0x01d:'306'
	// <0x5d1243>|0x063|+0x021:'307'
	// ******
}

// STATE[STUB]
void game_world_ui::set_player_kills_deaths( u8 player_id, u32 kills, u32 deaths )
{
	// LOCALS
	// flash_value 						out_event
	// flash_value 						out_event_property
	// ******

	// FUNCTION BODY[0x5d0ef0]: 27
	// <0>
	// <1>
	// <0x5d0ef8>|0x008|+0x025:'317'
	// <0>
	// <1>
	// <0x5d0f1d>|0x02d|+0x021:'320'
	// <0>
	// <0x5d0f3e>|0x04e|+0x029:'322'
	// <0x5d0f67>|0x077|+0x039:'323'
	// <0>
	// <0x5d0fa0>|0x0b0|+0x028:'325'
	// <0x5d0fc8>|0x0d8|+0x034:'326'
	// <0>
	// <0x5d0ffc>|0x10c|+0x028:'328'
	// <0x5d1024>|0x134|+0x034:'329'
	// <0>
	// <0x5d1058>|0x168|+0x024:'331'
	// <0x5d107c>|0x18c|+0x03a:'332'
	// <0>
	// <0x5d10b6>|0x1c6|+0x025:'334'
	// <0x5d10db>|0x1eb|+0x036:'335'
	// <0>
	// <0x5d1111>|0x221|+0x025:'337'
	// <0x5d1136>|0x246|+0x036:'338'
	// <0>
	// <0x5d116c>|0x27c|+0x01e:'340'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::update_ui( const u32 frame_delta_ms, const u32 __formal )
{
	// LOCALS
	// u8 								i
	// ******

	// CALL SITE INFO
	// <0x5d4c1a> -> match_options& < unknown >()
	// <0x5d4c35> -> player_ptr < unknown >( const u8 ) const
	// <0x5d4c99> -> match_options& < unknown >()
	// <0x5d4d02> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5d4bb0]: 38
	// <0x5d4bd0>|0x020|-0x017:'346'
	// <0>
	// <1>
	// <0x5d4bb9>|0x009|+0x01d:'349'
	// <0x5d4bd6>|0x026|+0x01c:'349'
	// <0>
	// <0x5d4bf2>|0x042|+0x00c:'351'
	// <0>
	// <0x5d4bfe>|0x04e|+0x00f:'353'
	// <0x5d4c0d>|0x05d|+0x017:'354'
	// <0>
	// <0x5d4c24>|0x074|+0x059:'356'
	// <0x5d4c7d>|0x0cd|+0x026:'357'
	// <0>
	// <1>
	// <2>
	// <0x5d4ca3>|0x0f3|+0x02e:'361'
	// <0>
	// <0x5d4cd1>|0x121|+0x010:'363'
	// <0x5d4ce1>|0x131|+0x007:'364'
	// <0>
	// <0x5d4ce8>|0x138|+0x006:'366'
	// <0>
	// <0x5d4cee>|0x13e|+0x01a:'368'
	// <0x5d4d08>|0x158|+0x006:'369'
	// <0>
	// <0x5d4d0e>|0x15e|+0x00d:'371'
	// <0>
	// <0x5d4d1b>|0x16b|+0x003:'373'
	// <0x5d4d1e>|0x16e|+0x003:'374'
	// <0>
	// <0x5d4d21>|0x171|+0x004:'376'
	// <0>
	// <0x5d4d25>|0x175|+0x00f:'378'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void game_world_ui::on_unload( )
{
	// FUNCTION BODY[0x5d0eb0]: 7
	// <0x5d0eb0>|0x000|+0x000:'387'	{
	// <0x5d0eb0>|0x000|+0x010:'388'
	// <0x5d0ec0>|0x010|+0x012:'389'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d0ed2>|0x022|+0x007:'394'
	// <0x5d0ed9>|0x029|+0x007:'394'
	// <0x5d0ee0>|0x030|-0x008:'394'
	// <0x5d0ed8>|0x028|+0x007:'395'
	// <0x5d0edf>|0x02f|+0x007:'395'
	// <0x5d0ee6>|0x036|      :'395'	}
	// ******
}

// STATE[STUB]
void game_world_ui::set_health( u8 health_in_percentage )
{
	// LOCALS
	// flash_value 						value
	// ******

	// FUNCTION BODY[0x5d0e50]: 4
	// <0>
	// <0x5d0e53>|0x003|+0x003:'400'
	// <0x5d0e56>|0x006|+0x033:'401'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::on_hit_from_pos( float3 position )
{
	// LOCALS
	// float4x4 						actor_camera_matrix
	// float4x4 						initiator_matrix
	// float 							angle
	// float3 							direction_vector
	// flash_value[2] 					args
	// ******

	// FUNCTION BODY[0x5d1510]: 15
	// <0>
	// <0x5d151c>|0x00c|+0x00c:'408'
	// <0>
	// <0x5d1528>|0x018|+0x04c:'410'
	// <0x5d1574>|0x064|+0x041:'411'
	// <0>
	// <0x5d15b5>|0x0a5|+0x0e8:'413'
	// <0>
	// <0x5d169d>|0x18d|+0x02f:'415'
	// <0>
	// <0x5d16cc>|0x1bc|+0x01c:'417'
	// <0x5d16e8>|0x1d8|+0x02a:'418'
	// <0x5d1712>|0x202|+0x045:'419'
	// <0x5d1757>|0x247|+0x033:'420'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::show_parametrized_message(
	pcstr		message_id,
	u8			font_size,
	u8			y_pos_in_percents,
	u32			timeout_in_ms
)
{
	// LOCALS
	// wchar_t[512] 					message
	// flash_value[4] 					message_val
	// ******

	// FUNCTION BODY[0x5d20f0]: 10
	// <0>
	// <0x5d20ff>|0x00f|+0x01c:'427'
	// <0>
	// <0x5d211b>|0x02b|+0x01c:'429'
	// <0x5d2137>|0x047|+0x00d:'430'
	// <0x5d2144>|0x054|+0x025:'431'
	// <0x5d2169>|0x079|+0x036:'432'
	// <0x5d219f>|0x0af|+0x030:'433'
	// <0>
	// <0x5d21cf>|0x0df|+0x029:'435'
	// ******
}

// STATE[STUB]
void game_world_ui::on_victory_item_put_take( u8 player_id, bool is_taken, bool is_base )
{
	// LOCALS
	// flash_value 						out_event
	// flash_value 						out_event_property
	// player_ptr 						player
	// u8 								i
	// ******

	// CALL SITE INFO
	// <0x5d46d2> -> player_ptr < unknown >( const u8 ) const
	// <0x5d46ed> -> bool < unknown >( const u8 ) const
	// <0x5d4708> -> match_options& < unknown >()
	// <0x5d4723> -> game_team_id < unknown >() const
	// <0x5d47e6> -> bool < unknown >( const u8 ) const
	// <0x5d48c5> -> game_team_id < unknown >() const
	// <0x5d4988> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x5d46a0]: 71
	// <0x5d46a0>|0x000|+0x006:'483'	{
	// <0x5d46a6>|0x006|+0x00b:'484'
	// <0>
	// <0x5d46b1>|0x011|+0x00f:'486'
	// <0>
	// <0x5d46c0>|0x020|+0x014:'488'
	// <0>
	// <0x5d46d4>|0x034|+0x002:'490'
	// <0>
	// <0x5d46d6>|0x036|+0x00a:'492'
	// <0>
	// <0x5d46e0>|0x040|+0x020:'494'
	// <0x5d4700>|0x060|+0x01a:'495'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d471a>|0x07a|+0x00f:'500'
	// <0x5d4729>|0x089|+0x00e:'501'
	// <0x5d4737>|0x097|+0x00b:'502'
	// <0x5d4742>|0x0a2|+0x042:'503'
	// <0x5d4784>|0x0e4|+0x018:'503'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d479c>|0x0fc|+0x02d:'509'
	// <0>
	// <0x5d47c9>|0x129|+0x008:'511'
	// <0>
	// <1>
	// <2>
	// <0x5d47d1>|0x131|+0x006:'515'
	// <0>
	// <1>
	// <0x5d47d7>|0x137|+0x015:'518'
	// <0x5d47ec>|0x14c|+0x00b:'519'
	// <0>
	// <1>
	// <0x5d47f7>|0x157|+0x002:'522'
	// <0>
	// <0x5d47f9>|0x159|+0x00b:'524'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d4804>|0x164|+0x028:'530'
	// <0x5d482c>|0x18c|+0x03b:'531'
	// <0>
	// <0x5d4867>|0x1c7|+0x009:'533'
	// <0x5d4870>|0x1d0|+0x02c:'534'
	// <0>
	// <0x5d489c>|0x1fc|+0x055:'536'
	// <0x5d48f1>|0x251|+0x03b:'537'
	// <0>
	// <0x5d492c>|0x28c|+0x009:'539'
	// <0x5d4935>|0x295|+0x02a:'540'
	// <0>
	// <0x5d495f>|0x2bf|+0x052:'542'
	// <0x5d49b1>|0x311|+0x036:'543'
	// <0>
	// <0x5d49e7>|0x347|+0x025:'545'
	// <0x5d4a0c>|0x36c|+0x036:'546'
	// <0>
	// <0x5d4a42>|0x3a2|+0x025:'548'
	// <0x5d4a67>|0x3c7|+0x036:'549'
	// <0>
	// <0x5d4a9d>|0x3fd|+0x025:'551'
	// <0x5d4ac2>|0x422|+0x036:'552'
	// <0>
	// <0x5d4af8>|0x458|-0x383:'554'
	// <0x5d4775>|0x0d5|+0x018:'555'
	// <0x5d478d>|0x0ed|+0x38d:'555'
	// <0x5d4b1a>|0x47a|      :'555'	}
	// ******
}

// STATE[STUB]
void game_world_ui::on_player_killed(
	u8		victim_id,
	u8		killer_id,
	bool	is_headshot,
	u32		item_dict_id
)
{
	// LOCALS
	// wchar_t const* 					victim_name
	// flash_value 						out_event
	// player_ptr 						killer
	// player_ptr 						victim
	// flash_value 						out_event_property
	// u8 								combat_log_icon
	// ******

	// CALL SITE INFO
	// <0x5d4159> -> player_ptr < unknown >( const u8 ) const
	// <0x5d416d> -> player_ptr < unknown >( const u8 ) const
	// <0x5d42ef> -> game_team_id < unknown >() const
	// <0x5d42fa> -> game_team_id < unknown >() const
	// <0x5d4401> -> game_team_id < unknown >() const
	// <0x5d440c> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x5d40f0]: 54
	// <0>
	// <0x5d40f6>|0x006|+0x01b:'560'
	// <0>
	// <0x5d4111>|0x021|+0x01a:'562'
	// <0>
	// <0x5d412b>|0x03b|+0x01c:'564'
	// <0>
	// <1>
	// <0x5d4147>|0x057|+0x014:'567'
	// <0x5d415b>|0x06b|+0x014:'568'
	// <0>
	// <0x5d416f>|0x07f|+0x004:'570'
	// <0x5d4173>|0x083|+0x004:'571'
	// <0>
	// <0x5d4177>|0x087|+0x043:'573'
	// <0>
	// <1>
	// <0x5d41ba>|0x0ca|+0x023:'576'
	// <0>
	// <1>
	// <0x5d41dd>|0x0ed|+0x023:'579'
	// <0>
	// <0x5d4200>|0x110|+0x02e:'581'
	// <0x5d422e>|0x13e|+0x03c:'582'
	// <0>
	// <0x5d426a>|0x17a|+0x009:'584'
	// <0x5d4273>|0x183|+0x02c:'585'
	// <0>
	// <0x5d429f>|0x1af|+0x0ac:'587'
	// <0x5d434b>|0x25b|+0x02c:'588'
	// <0>
	// <0x5d4377>|0x287|+0x00d:'590'
	// <0x5d4384>|0x294|+0x02c:'591'
	// <0>
	// <0x5d43b0>|0x2c0|+0x0a7:'593'
	// <0x5d4457>|0x367|+0x02c:'594'
	// <0>
	// <0x5d4483>|0x393|+0x029:'596'
	// <0x5d44ac>|0x3bc|+0x034:'597'
	// <0>
	// <1>
	// <0x5d44e0>|0x3f0|+0x00c:'600'
	// <0x5d44ec>|0x3fc|+0x004:'601'
	// <0x5d44f0>|0x400|+0x007:'602'
	// <0x5d44f7>|0x407|+0x002:'603'
	// <0>
	// <0x5d44f9>|0x409|+0x02b:'605'
	// <0x5d4524>|0x434|+0x039:'606'
	// <0>
	// <0x5d455d>|0x46d|+0x024:'608'
	// <0x5d4581>|0x491|+0x036:'609'
	// <0>
	// <0x5d45b7>|0x4c7|+0x022:'611'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::set_crosshair_size( float size )
{
	// LOCALS
	// flash_value 						value
	// ******

	// FUNCTION BODY[0x5d0de0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d0de9>|0x009|+0x039:'621'
	// ******
}

// STATE[STUB]
void game_world_ui::on_enemy_hitted( )
{
	// FUNCTION BODY[0x5d0dc0]: 1
	// <0x5d0dc0>|0x000|+0x01c:'626'
	// ******
}

// STATE[STUB]
void game_world_ui::show_crosshair( bool b_show )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d0d60]: 3
	// <0>
	// <1>
	// <0x5d0d63>|0x003|+0x033:'633'
	// ******
}

// STATE[STUB]
void game_world_ui::show_ammo_indicator( bool b_show )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d0d00]: 3
	// <0>
	// <1>
	// <0x5d0d03>|0x003|+0x033:'640'
	// ******
}

// STATE[STUB]
void game_world_ui::show_capture_progress( bool b_show )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d0ca0]: 3
	// <0>
	// <1>
	// <0x5d0ca3>|0x003|+0x034:'647'
	// ******
}

// STATE[STUB]
void game_world_ui::set_fire_queue_size( const u32 fire_queue_size )
{
	// LOCALS
	// flash_value 						value
	// ******

	// FUNCTION BODY[0x5d0c40]: 5
	// <0>
	// <1>
	// <2>
	// <0x5d0c49>|0x009|+0x033:'655'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::set_ammo_in_magazine( const u32 count )
{
	// LOCALS
	// flash_value 						value
	// ******

	// FUNCTION BODY[0x5d0be0]: 3
	// <0>
	// <1>
	// <0x5d0be9>|0x009|+0x033:'662'
	// ******
}

// STATE[STUB]
void game_world_ui::set_ammo_type( const u8 ammo_type )
{
	// LOCALS
	// flash_value 						value
	// ******

	// FUNCTION BODY[0x5d0b70]: 3
	// <0>
	// <0x5d0b79>|0x009|+0x003:'668'
	// <0x5d0b7c>|0x00c|+0x033:'669'
	// ******
}

// STATE[STUB]
void game_world_ui::show_players_list( bool b_show )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d0b00]: 7
	// <0x5d0b08>|0x008|+0x005:'675'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d0b0d>|0x00d|+0x033:'680'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::on_damage_affect_applying(
	pcstr							bodypart,
	const hit_affects_type_enum		arg_1 /* hit_affects_type_enum affect */,
	const affect_event_type_enum	arg_2 /* affect_event_type_enum event_type */
)
{
	// LOCALS
	// flash_value 						value
	// ******

	// FUNCTION BODY[0x5d1400]: 20
	// <0>
	// <1>
	// <0x5d1403>|0x003|+0x005:'688'
	// <0x5d1408>|0x008|+0x03d:'689'
	// <0x5d1445>|0x045|+0x009:'690'
	// <0x5d144e>|0x04e|+0x031:'691'
	// <0>
	// <1>
	// <0x5d147f>|0x07f|+0x009:'694'
	// <0x5d1488>|0x088|+0x004:'695'
	// <0>
	// <0x5d148c>|0x08c|+0x003:'697'
	// <0x5d148f>|0x08f|+0x027:'698'
	// <0>
	// <0x5d14b6>|0x0b6|+0x00a:'700'
	// <0>
	// <0x5d14c0>|0x0c0|+0x003:'702'
	// <0x5d14c3>|0x0c3|+0x02a:'703'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void game_world_ui::update_minimap_objects( )
{
	// LOCALS
	// player_ptr 						current_player
	// bool 							is_carrying_victory_item
	// game_team_id 					local_player_team
	// flash_value 						level_objects
	// u8 								bases_count
	// flash_value 						level_object_val_prop
	// u8 								i
	// victory_items_container** 		it
	// float 							position_y
	// flash_value 						level_object_val
	// float 							position_x
	// float 							position_y
	// flash_value 						level_object_val
	// float 							position_x
	// ******

	// CALL SITE INFO
	// <0x5d39ec> -> bool < unknown >( const u8 ) const
	// <0x5d3a06> -> match_options& < unknown >()
	// <0x5d3b19> -> game_team_id < unknown >()
	// <0x5d3e04> -> float4x4 < unknown >()
	// <0x5d3e23> -> float4x4 < unknown >()
	// ******

	// FUNCTION BODY[0x5d39b0]: 78
	// <0x5d39be>|0x00e|+0x00c:'710'
	// <0>
	// <1>
	// <2>
	// <0x5d39ca>|0x01a|+0x016:'714'
	// <0>
	// <0x5d39e0>|0x030|+0x01f:'716'
	// <0x5d39ff>|0x04f|+0x01d:'717'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d3a1c>|0x06c|+0x020:'723'
	// <0>
	// <0x5d3a3c>|0x08c|+0x008:'725'
	// <0>
	// <1>
	// <2>
	// <0x5d3a44>|0x094|+0x022:'729'
	// <0>
	// <0x5d3a66>|0x0b6|+0x015:'731'
	// <0>
	// <0x5d3a7b>|0x0cb|+0x31d:'733'
	// <0x5d3d98>|0x3e8|-0x28a:'733'
	// <0>
	// <0x5d3b0e>|0x15e|+0x017:'735'
	// <0>
	// <1>
	// <2>
	// <0x5d3b25>|0x175|+0x023:'739'
	// <0>
	// <0x5d3b48>|0x198|+0x014:'741'
	// <0x5d3b5c>|0x1ac|+0x01d:'742'
	// <0>
	// <0x5d3b79>|0x1c9|+0x039:'744'
	// <0x5d3bb2>|0x202|+0x03b:'745'
	// <0>
	// <0x5d3bed>|0x23d|+0x075:'747'
	// <0x5d3c62>|0x2b2|+0x02e:'748'
	// <0>
	// <0x5d3c90>|0x2e0|+0x025:'750'
	// <0x5d3cb5>|0x305|+0x043:'751'
	// <0>
	// <0x5d3cf8>|0x348|+0x025:'753'
	// <0x5d3d1d>|0x36d|+0x03e:'754'
	// <0>
	// <0x5d3d5b>|0x3ab|+0x015:'756'
	// <0>
	// <1>
	// <0x5d3d70>|0x3c0|+0x036:'759'
	// <0>
	// <0x5d3da6>|0x3f6|+0x29f:'761'
	// <0x5d4045>|0x695|-0x285:'761'
	// <0>
	// <0x5d3dc0>|0x410|+0x012:'763'
	// <0>
	// <0x5d3dd2>|0x422|+0x002:'765'
	// <0x5d3dd4>|0x424|+0x021:'766'
	// <0>
	// <0x5d3df5>|0x445|+0x016:'768'
	// <0x5d3e0b>|0x45b|+0x01f:'769'
	// <0>
	// <0x5d3e2a>|0x47a|+0x04a:'771'
	// <0x5d3e74>|0x4c4|+0x03a:'772'
	// <0>
	// <0x5d3eae>|0x4fe|+0x069:'774'
	// <0x5d3f17>|0x567|+0x02c:'775'
	// <0>
	// <0x5d3f43>|0x593|+0x024:'777'
	// <0x5d3f67>|0x5b7|+0x043:'778'
	// <0>
	// <0x5d3faa>|0x5fa|+0x025:'780'
	// <0x5d3fcf>|0x61f|+0x03e:'781'
	// <0>
	// <0x5d400d>|0x65d|+0x015:'783'
	// <0x5d4022>|0x672|+0x035:'784'
	// <0>
	// <0x5d4057>|0x6a7|+0x020:'786'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::initialize_minimap( )
{
	// LOCALS
	// flash_value 						minimap_props_value_property
	// flash_value 						minimap_props_value
	// ******

	// FUNCTION BODY[0x5d5250]: 15
	// <0x5d5258>|0x008|+0x076:'792'
	// <0>
	// <1>
	// <0x5d52ce>|0x07e|+0x021:'795'
	// <0>
	// <0x5d52ef>|0x09f|+0x021:'797'
	// <0>
	// <0x5d5310>|0x0c0|+0x009:'799'
	// <0x5d5319>|0x0c9|+0x02a:'800'
	// <0>
	// <0x5d5343>|0x0f3|+0x01e:'802'
	// <0>
	// <0x5d5361>|0x111|+0x006:'804'
	// <0>
	// <0x5d5367>|0x117|+0x007:'806'
	// ******
}

// STATE[STUB]
void game_world_ui::update_minimap_players( )
{
	// LOCALS
	// base_network_client* 			client
	// player_ptr 						local_player
	// flash_value 						players_array
	// u32 								in_array_index
	// u8 								i
	// float 							position_y
	// player_ptr 						current_player
	// flash_value 						player_descr_value
	// flash_value 						player_descr_value_property
	// float 							position_x
	// bool 							is_carrying_item
	// ******

	// CALL SITE INFO
	// <0x5d3531> -> player_ptr < unknown >( const u8 ) const
	// <0x5d3550> -> game_team_id < unknown >() const
	// <0x5d355b> -> game_team_id < unknown >() const
	// <0x5d3798> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x5d34b0]: 51
	// <0x5d34b0>|0x000|+0x009:'810'	{
	// <0x5d34b9>|0x009|+0x015:'811'
	// <0>
	// <0x5d34ce>|0x01e|+0x411:'813'
	// <0>
	// <1>
	// <0x5d38df>|0x42f|-0x3e9:'816'
	// <0>
	// <1>
	// <0x5d34f6>|0x046|+0x01e:'819'
	// <0x5d3514>|0x064|+0x004:'820'
	// <0x5d3518>|0x068|+0x008:'821'
	// <0>
	// <0x5d3520>|0x070|+0x013:'823'
	// <0>
	// <0x5d3533>|0x083|+0x3c6:'825'
	// <0x5d38f9>|0x449|-0x394:'826'
	// <0>
	// <0x5d3565>|0x0b5|+0x004:'828'
	// <0>
	// <0x5d3569>|0x0b9|+0x011:'830'
	// <0>
	// <1>
	// <2>
	// <0x5d357a>|0x0ca|+0x044:'834'
	// <0>
	// <1>
	// <0x5d35be>|0x10e|+0x021:'837'
	// <0>
	// <0x5d35df>|0x12f|+0x025:'839'
	// <0x5d3604>|0x154|+0x03c:'840'
	// <0>
	// <0x5d3640>|0x190|+0x024:'842'
	// <0x5d3664>|0x1b4|+0x043:'843'
	// <0>
	// <0x5d36a7>|0x1f7|+0x025:'845'
	// <0x5d36cc>|0x21c|+0x03e:'846'
	// <0>
	// <0x5d370a>|0x25a|+0x043:'848'
	// <0x5d374d>|0x29d|+0x042:'849'
	// <0>
	// <0x5d378f>|0x2df|+0x032:'851'
	// <0x5d37c1>|0x311|+0x03a:'852'
	// <0>
	// <0x5d37fb>|0x34b|+0x029:'854'
	// <0x5d3824>|0x374|+0x038:'855'
	// <0>
	// <0x5d385c>|0x3ac|+0x01a:'857'
	// <0>
	// <0x5d3876>|0x3c6|+0x07b:'859'
	// <0x5d38f1>|0x441|+0x04f:'859'
	// <0x5d3940>|0x490|-0x058:'860'
	// <0>
	// <0x5d38e8>|0x438|+0x079:'862'
	// <0x5d3961>|0x4b1|      :'862'	}
	// ******
}

// STATE[STUB]
void game_world_ui::update_minimap_local_player( )
{
	// LOCALS
	// player_ptr 						current_player
	// flash_value[4] 					player_descr_value
	// float4x4 						current_player_transform
	// ******

	// FUNCTION BODY[0x5d3300]: 16
	// <0x5d3300>|0x000|+0x006:'865'	{
	// <0x5d3306>|0x006|+0x019:'866'
	// <0x5d331f>|0x01f|+0x022:'867'
	// <0x5d3341>|0x041|+0x148:'868'
	// <0x5d3489>|0x189|-0x13b:'869'
	// <0>
	// <1>
	// <2>
	// <0x5d334e>|0x04e|+0x027:'873'
	// <0>
	// <0x5d3375>|0x075|+0x03b:'875'
	// <0x5d33b0>|0x0b0|+0x027:'876'
	// <0x5d33d7>|0x0d7|+0x020:'877'
	// <0x5d33f7>|0x0f7|+0x014:'878'
	// <0x5d340b>|0x10b|+0x028:'879'
	// <0>
	// <0x5d3433>|0x133|+0x031:'881'
	// <0x5d3464>|0x164|+0x039:'882'
	// <0x5d349d>|0x19d|      :'882'	}
	// ******
}

// STATE[STUB]
void game_world_ui::reset_map_rotatable( )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d0a90]: 4
	// <0>
	// <0x5d0a93>|0x003|+0x005:'887'
	// <0x5d0a98>|0x008|+0x033:'888'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::set_ammo_total_count( u32 first_type_count, u32 second_type_count )
{
	// LOCALS
	// flash_value 						count
	// ******

	// FUNCTION BODY[0x5d09d0]: 6
	// <0>
	// <1>
	// <0x5d09d9>|0x009|+0x033:'896'
	// <0>
	// <0x5d0a0c>|0x03c|+0x02a:'898'
	// <0x5d0a36>|0x066|+0x02b:'899'
	// ******
}

// STATE[STUB]
void game_world_ui::show_quick_slots( bool b_show )
{
	// LOCALS
	// flash_value 						b_val
	// ******

	// FUNCTION BODY[0x5d0940]: 6
	// <0>
	// <1>
	// <0x5d0943>|0x003|+0x039:'906'
	// <0>
	// <0x5d097c>|0x03c|+0x004:'908'
	// <0x5d0980>|0x040|+0x027:'909'
	// ******
}

// STATE[STUB]
void game_world_ui::create_slot_value(
	profile_slot_enum			slot,
	inventory_item_props&		item_props,
	flash_value&				slot_descr_value
)
{
	// LOCALS
	// flash_value 						slot_descr_valuec_property
	// bool 							enabled
	// u8 								item_icon
	// ******

	// STATICS
	// static < NoType > 				 = <0x5d20d8>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c0ea4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c0eab }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c0eb2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c0eb9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c0ec0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c0ec7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5d1e20]: 36
	// <0x5d1e23>|0x003|+0x048:'914'
	// <0>
	// <0x5d1e6b>|0x04b|+0x004:'916'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d1e6f>|0x04f|+0x013:'921'
	// <0>
	// <0x5d1e82>|0x062|+0x022:'923'
	// <0>
	// <0x5d1ea4>|0x084|+0x007:'925'
	// <0x5d1eab>|0x08b|+0x007:'926'
	// <0x5d1eb2>|0x092|+0x007:'927'
	// <0x5d1eb9>|0x099|+0x007:'928'
	// <0x5d1ec0>|0x0a0|+0x007:'929'
	// <0x5d1ec7>|0x0a7|+0x01b:'930'
	// <0>
	// <1>
	// <2>
	// <0x5d1ee2>|0x0c2|+0x025:'934'
	// <0>
	// <0x5d1f07>|0x0e7|+0x029:'936'
	// <0x5d1f30>|0x110|+0x036:'937'
	// <0>
	// <0x5d1f66>|0x146|+0x028:'939'
	// <0x5d1f8e>|0x16e|+0x033:'940'
	// <0>
	// <0x5d1fc1>|0x1a1|+0x02b:'942'
	// <0x5d1fec>|0x1cc|+0x033:'943'
	// <0>
	// <0x5d201f>|0x1ff|+0x029:'945'
	// <0x5d2048>|0x228|+0x035:'946'
	// <0>
	// <0x5d207d>|0x25d|+0x009:'948'
	// <0x5d2086>|0x266|+0x029:'949'
	// ******
}

// STATE[STUB]
void game_world_ui::fill_quick_slots( )
{
	// LOCALS
	// inventory_item_ptr 				item_in_back_slot
	// u32 								in_array_index
	// flash_value 						slots_array
	// flash_value 						slot_descr_value
	// inventory_item_props 			current_item_props
	// dictionary_item 					dict_item
	// inventory_item_props 			item_in_back_slot_props
	// dictionary_item 					dict_item
	// flash_value 						b_val
	// flash_value[2] 					oxygene_props
	// ******

	// CALL SITE INFO
	// <0x5d2f29> -> bool < unknown >( inventory_item_props& )
	// <0x5d30ef> -> bool < unknown >( inventory_item_props& )
	// ******

	// FUNCTION BODY[0x5d2e00]: 49
	// <0x5d2e50>|0x050|-0x042:'954'
	// <0x5d2e0e>|0x00e|+0x024:'955'
	// <0x5d2e32>|0x032|+0x1eb:'956'
	// <0x5d301d>|0x21d|-0x1cb:'957'
	// <0>
	// <1>
	// <0x5d2e52>|0x052|+0x021:'960'
	// <0>
	// <0x5d2e73>|0x073|+0x066:'962'
	// <0x5d2ed9>|0x0d9|+0x004:'963'
	// <0x5d2edd>|0x0dd|+0x02a:'964'
	// <0x5d2f07>|0x107|+0x016:'965'
	// <0>
	// <0x5d2f1d>|0x11d|+0x00e:'967'
	// <0>
	// <0x5d2f2b>|0x12b|+0x054:'969'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d2f7f>|0x17f|+0x018:'975'
	// <0x5d2f97>|0x197|+0x01a:'976'
	// <0x5d2fb1>|0x1b1|+0x084:'977'
	// <0>
	// <0x5d3035>|0x235|+0x01e:'979'
	// <0>
	// <0x5d3053>|0x253|+0x072:'981'
	// <0x5d30c5>|0x2c5|+0x008:'982'
	// <0>
	// <1>
	// <2>
	// <0x5d30cd>|0x2cd|+0x024:'986'
	// <0>
	// <0x5d30f1>|0x2f1|+0x054:'988'
	// <0>
	// <1>
	// <2>
	// <0x5d3145>|0x345|+0x010:'992'
	// <0>
	// <0x5d3155>|0x355|+0x032:'994'
	// <0>
	// <0x5d3187>|0x387|+0x01a:'996'
	// <0x5d31a1>|0x3a1|+0x029:'997'
	// <0x5d31ca>|0x3ca|+0x035:'998'
	// <0>
	// <0x5d31ff>|0x3ff|+0x02f:'1000'
	// <0x5d322e>|0x42e|+0x058:'1001'
	// <0x5d3286>|0x486|+0x02c:'1002'
	// ******
}

// STATE[STUB]
void game_world_ui::update_quick_slot( profile_slot_enum slot )
{
	// LOCALS
	// flash_value[2] 					slot_descr_value
	// inventory_item_props 			current_item_props
	// dictionary_item 					dict_item
	// flash_value[2] 					oxygene_props
	// ******

	// CALL SITE INFO
	// <0x5d2c03> -> bool < unknown >( inventory_item_props& )
	// ******

	// FUNCTION BODY[0x5d2b30]: 27
	// <0x5d2b3c>|0x00c|+0x01c:'1007'
	// <0x5d2b58>|0x028|+0x020:'1008'
	// <0>
	// <0x5d2b78>|0x048|+0x068:'1010'
	// <0>
	// <0x5d2be0>|0x0b0|+0x016:'1012'
	// <0x5d2bf6>|0x0c6|+0x013:'1013'
	// <0x5d2c09>|0x0d9|+0x006:'1014'
	// <0>
	// <0x5d2c0f>|0x0df|+0x009:'1016'
	// <0>
	// <0x5d2c18>|0x0e8|+0x029:'1018'
	// <0>
	// <0x5d2c41>|0x111|+0x012:'1020'
	// <0>
	// <0x5d2c53>|0x123|+0x013:'1022'
	// <0x5d2c66>|0x136|+0x02d:'1023'
	// <0x5d2c93>|0x163|+0x034:'1024'
	// <0x5d2cc7>|0x197|+0x02e:'1025'
	// <0x5d2cf5>|0x1c5|+0x013:'1026'
	// <0x5d2d08>|0x1d8|+0x030:'1027'
	// <0x5d2d38>|0x208|+0x002:'1028'
	// <0>
	// <0x5d2d3a>|0x20a|+0x011:'1030'
	// <0x5d2d4b>|0x21b|+0x029:'1031'
	// <0x5d2d74>|0x244|+0x02e:'1032'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::show_screen_message( pcstr message_id )
{
	// LOCALS
	// wchar_t[512] 					message
	// flash_value 						message_val
	// ******

	// FUNCTION BODY[0x5d1d90]: 5
	// <0>
	// <0x5d1da0>|0x010|+0x01c:'1039'
	// <0x5d1dbc>|0x02c|+0x002:'1040'
	// <0x5d1dbe>|0x02e|+0x015:'1041'
	// <0x5d1dd3>|0x043|+0x01d:'1042'
	// ******
}

// STATE[STUB]
void game_world_ui::set_using_progress_message( u32 progress_value )
{
	// LOCALS
	// wchar_t[512] 					message
	// flash_value[3] 					message_val
	// ******

	// FUNCTION BODY[0x5d1c60]: 7
	// <0>
	// <0x5d1c6f>|0x00f|+0x01f:'1048'
	// <0x5d1c8e>|0x02e|+0x023:'1049'
	// <0x5d1cb1>|0x051|+0x00d:'1050'
	// <0x5d1cbe>|0x05e|+0x025:'1051'
	// <0x5d1ce3>|0x083|+0x039:'1052'
	// <0x5d1d1c>|0x0bc|+0x029:'1053'
	// ******
}

// STATE[STUB]
void game_world_ui::set_using_info_message( pcstr str )
{
	// LOCALS
	// wchar_t[512] 					message
	// flash_value[3] 					message_val
	// ******

	// FUNCTION BODY[0x5d1b40]: 7
	// <0>
	// <0x5d1b4f>|0x00f|+0x01c:'1059'
	// <0x5d1b6b>|0x02b|+0x01c:'1060'
	// <0x5d1b87>|0x047|+0x00d:'1061'
	// <0x5d1b94>|0x054|+0x025:'1062'
	// <0x5d1bb9>|0x079|+0x032:'1063'
	// <0x5d1beb>|0x0ab|+0x029:'1064'
	// ******
}

// STATE[STUB]
void game_world_ui::add_quick_slot_to_update( profile_slot_enum slot )
{
	// FUNCTION BODY[0x5d17d0]: 1
	// <0x5d17d0>|0x000|+0x001:'1068'	{
	// <0x5d17d1>|0x001|+0x01f:'1069'
	// <0x5d17f0>|0x020|-0x003:'1069'
	// <0x5d17ed>|0x01d|+0x00e:'1070'
	// <0x5d17fb>|0x02b|      :'1070'	}
	// ******
}

// STATE[STUB]
void game_world_ui::disactivate_quick_slot( profile_slot_enum slot )
{
	// FUNCTION BODY[0x5d0740]: 8
	// <0x5d0740>|0x000|+0x004:'1074'
	// <0x5d0744>|0x004|+0x003:'1075'
	// <0>
	// <0x5d0747>|0x007|+0x03a:'1077'
	// <0x5d0781>|0x041|-0x031:'1077'
	// <0>
	// <0x5d0750>|0x010|+0x008:'1079'
	// <0x5d0758>|0x018|+0x036:'1080'
	// <0>
	// ******
}

// STATE[STUB]
void game_world_ui::show_chat( bool b_show )
{
	// FUNCTION BODY[0x5d0730]: 3
	// <0x5d0730>|0x000|+0x000:'1104'	{
	// <0>
	// <1>
	// <2>
	// <0x5d0730>|0x000|      :'1108'	}
	// ******
}

// STATE[STUB]
void game_world_ui::set_broken_connection_message( pcstr str )
{
	// LOCALS
	// wchar_t[512] 					w_message
	// flash_value[2] 					message_val
	// ******

	// FUNCTION BODY[0x5d1a70]: 9
	// <0x5d1a7e>|0x00e|+0x00c:'1112'
	// <0>
	// <1>
	// <2>
	// <0x5d1a8a>|0x01a|+0x01d:'1116'
	// <0x5d1aa7>|0x037|+0x013:'1117'
	// <0x5d1aba>|0x04a|+0x00d:'1118'
	// <0x5d1ac7>|0x057|+0x028:'1119'
	// <0x5d1aef>|0x07f|+0x02f:'1120'
	// ******
}

// STATE[STUB]
void game_world_ui::on_attached_to_player( player_ptr player )
{
	// LOCALS
	// weapon_ammo_info 				info
	// ******

	// CALL SITE INFO
	// <0x5d51b5> -> weapon_core* < unknown >()
	// ******

	// FUNCTION BODY[0x5d5100]: 27
	// <0x5d5100>|0x000|+0x00d:'1124'	{
	// <0x5d510d>|0x00d|+0x00d:'1125'
	// <0>
	// <0x5d511a>|0x01a|+0x009:'1127'
	// <0x5d5123>|0x023|+0x018:'1128'
	// <0>
	// <1>
	// <2>
	// <0x5d513b>|0x03b|+0x052:'1132'
	// <0x5d518d>|0x08d|+0x006:'1133'
	// <0>
	// <0x5d5193>|0x093|+0x03d:'1135'
	// <0>
	// <0x5d51d0>|0x0d0|+0x004:'1137'
	// <0>
	// <0x5d51d4>|0x0d4|+0x01a:'1139'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d51ee>|0x0ee|+0x00c:'1144'
	// <0>
	// <0x5d51fa>|0x0fa|+0x009:'1146'
	// <0>
	// <0x5d5203>|0x103|+0x00e:'1148'
	// <0x5d5211>|0x111|+0x014:'1149'
	// <0x5d5225>|0x125|+0x00c:'1150'
	// <0x5d5231>|0x131|-0x107:'1151'
	// <0x5d512a>|0x02a|+0x0b3:'1152'
	// <0x5d51dd>|0x0dd|+0x05f:'1152'
	// <0x5d523c>|0x13c|      :'1152'	}
	// ******
}

// STATE[STUB]
void game_world_ui::on_detached_from_player( )
{
	// FUNCTION BODY[0x5d13e0]: 2
	// <0x5d13ea>|0x00a|+0x009:'1156'
	// <0x5d13f3>|0x013|+0x007:'1157'
	// ******
}

// STATE[STUB]
void game_world_ui::show_item_container( u8 visual_id )
{
	// LOCALS
	// flash_value 						visual_id_val
	// ******

	// FUNCTION BODY[0x5d08e0]: 3
	// <0>
	// <1>
	// <0x5d08e3>|0x003|+0x030:'1164'
	// ******
}

// STATE[STUB]
void game_world_ui::hide_item_container( )
{
	// FUNCTION BODY[0x5d08c0]: 1
	// <0x5d08c0>|0x000|+0x01c:'1168'
	// ******
}

// STATE[STUB]
void game_world_ui::set_player_online_status( u32 player_id, bool is_online )
{
	// LOCALS
	// wchar_t[512] 					w_player_name
	// flash_value[2] 					player_online_value
	// ******

	// CALL SITE INFO
	// <0x5d07c7> -> match_options& < unknown >()
	// ******

	// FUNCTION BODY[0x5d07a0]: 12
	// <0x5d07b2>|0x012|+0x01d:'1173'
	// <0>
	// <1>
	// <2>
	// <0x5d07cf>|0x02f|+0x024:'1177'
	// <0>
	// <0x5d07f3>|0x053|+0x01e:'1179'
	// <0>
	// <0x5d0811>|0x071|+0x00d:'1181'
	// <0x5d081e>|0x07e|+0x02b:'1182'
	// <0>
	// <0x5d0849>|0x0a9|+0x02a:'1184'
	// ******
}

} // namespace survarium
