////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_world.h"
#include "game.h"			// get_game().get_sound_world() needs game complete
#include "game_memory.h"	// g_allocator for allocator-taking vectora member
#include "camera_director.h"	// switch_to_camera (switch_to_free_fly_camera)
#include "free_fly_camera.h"	// free_fly_camera complete (game_camera* arg)

namespace survarium {

// TU statics 'draw_respawn_debug_cc' / 'draw_match_stats_cc' (compiler-
// generated dynamic initializers + atexit destructor); a matcher recovers
// their types/initializers from the init asm.
/*
// STATE[STUB]
void `dynamic initializer for 'draw_respawn_debug_cc''( )
{
	// FUNCTION BODY[0x7d8280]
	// <0x7d8280>|0x000|      :'67'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 'draw_match_stats_cc''( )
{
	// FUNCTION BODY[0x7d82d0]
	// <0x7d82d0>|0x000|      :'68'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 'draw_respawn_debug_cc''( )
{
	// FUNCTION BODY[0x7f0000]
	// <0x7d8320>|0x000|      :'69'	{
	// ******
}
*/

// STATE[STUB]
game_world::game_world( game& game ) :
	// base / value-member sources are the obvious ones (base_game_scene and
	// game_ui take the owner refs) - a matcher confirms when this TU is enabled
	base_game_scene( game ),
	game_ui( *this ),
	m_victory_items( g_allocator )	// allocator-taking vectora (no default ctor)
{
	// STATICS
	// static console_commands::cc_delegate clear_player_spawn_cc = <0x4c2b450>;
	// static console_commands::cc_delegate clear_enemies_position_cc = <0x4c2b4b0>;
	// static console_commands::cc_delegate add_enemy_position_cc = <0x4c2b510>;
	// ******

	// FUNCTION BODY[0x5e0350]: 16
	// <0x5e04a4>|0x154|+0x08c:'99'
	// <0x5e0530>|0x1e0|+0x08e:'100'
	// <0x5e05be>|0x26e|+0x08d:'101'
	// <0>
	// <0x5e064b>|0x2fb|+0x01d:'103'
	// <0>
	// <0x5e0668>|0x318|+0x031:'105'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e0699>|0x349|+0x02c:'111'
	// <0x5e06c5>|0x375|+0x005:'112'
	// <0x5e06ca>|0x37a|+0x006:'113'
	// <0x5e06d0>|0x380|+0x02c:'114'
	// ******
}

// STATE[STUB]
void game_world::register_cooks( )
{
	// STATICS
	// static booby_trap_cook 			s_booby_trap_cook = <0x4c276d8>;
	// static damage_zone_cook 			s_damage_zone_cook = <0x4c2732c>;
	// static animation_space_graph_cook s_animation_space_graph_cook = <0x4c276fc>;
	// static weapon_cook 				s_weapon_cook = <0x4c276b4>;
	// static human_npc_cook 			s_human_npc_cook = <0x4c27720>;
	// static rifle_scope_cook 			s_rifle_scope_cook = <0x4c2730c>;
	// static player_cook 				s_player_cook = <0x4c277f4>;
	// static booby_trap_set_cook 		s_booby_trap_set_cook = <0x4c27814>;
	// static sound_player_cook 		s_sound_player_cook = <0x4c27744>;
	// static empty_hands_cook 			s_empty_hands_cook = <0x4c272ec>;
	// ******

	// FUNCTION BODY[0x5dfb80]: 21
	// <0x5dfb80>|0x000|+0x032:'119'
	// <0x5dfbb2>|0x032|+0x026:'120'
	// <0x5dfbd8>|0x058|+0x02c:'121'
	// <0x5dfc04>|0x084|+0x025:'122'
	// <0x5dfc29>|0x0a9|+0x03a:'123'
	// <0x5dfc63>|0x0e3|+0x03a:'124'
	// <0>
	// <0x5dfc9d>|0x11d|+0x00a:'126'
	// <0x5dfca7>|0x127|+0x00a:'127'
	// <0x5dfcb1>|0x131|+0x00a:'128'
	// <0x5dfcbb>|0x13b|+0x00a:'129'
	// <0>
	// <1>
	// <0x5dfcc5>|0x145|+0x042:'132'
	// <0x5dfd07>|0x187|+0x005:'133'
	// <0x5dfd0c>|0x18c|+0x026:'134'
	// <0x5dfd32>|0x1b2|+0x025:'135'
	// <0>
	// <1>
	// <2>
	// <0x5dfd57>|0x1d7|+0x023:'139'
	// ******
}

// STATE[STUB]
 game_world::~game_world( )
{
	// CALL SITE INFO
	// <0x5e00e4> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5e0070]: 7
	// <0x5e0077>|0x007|+0x08c:'144'
	// <0x5e0103>|0x093|+0x036:'145'
	// <0x5e0139>|0x0c9|+0x021:'146'
	// <0x5e015a>|0x0ea|+0x030:'147'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void game_world::on_after_tick( )
{
	// CALL SITE INFO
	// <0x5e0e45> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e0e10]: 10
	// <0x5e0e14>|0x004|+0x00d:'155'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e0e21>|0x011|+0x02d:'162'
	// <0>
	// <0x5e0e4e>|0x03e|+0x013:'164'
	// ******
}

// STATE[STUB]
void game_world::tick( const u32 frame_delta_ms, const u32 current_time_in_ms, const bool is_game_paused )
{
	// CALL SITE INFO
	// <0x5e0ef6> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5e0e70]: 60
	// <0x5e0e73>|0x003|+0x043:'169'
	// <0>
	// <0x5e0eb6>|0x046|+0x020:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e0ed6>|0x066|+0x009:'178'
	// <0x5e0edf>|0x06f|+0x009:'179'
	// <0>
	// <0x5e0ee8>|0x078|+0x004:'181'
	// <0x5e0eec>|0x07c|+0x00c:'182'
	// <0>
	// <0x5e0ef8>|0x088|+0x007:'184'
	// <0>
	// <0x5e0eff>|0x08f|+0x009:'186'
	// <0x5e0f08>|0x098|+0x00f:'187'
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
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <0x5e0f17>|0x0a7|+0x01c:'228'
	// ******
}

// STATE[STUB]
void game_world::switch_camera_mode(
	const input_mode_type_enum		arg_0 /* input_mode_type_enum input_mode */
)
{
	// FUNCTION BODY[0x5dff20]: 19
	// <0x5dff20>|0x000|+0x001:'280'	{
	// <0x5dff21>|0x001|+0x099:'281'
	// <0>
	// <1>
	// <0x5dffba>|0x09a|-0x021:'284'
	// <0>
	// <1>
	// <2>
	// <0x5dff99>|0x079|-0x05a:'288'
	// <0>
	// <1>
	// <2>
	// <0x5dff3f>|0x01f|+0x056:'292'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5dff95>|0x075|+0x021:'300'
	// <0x5dffb6>|0x096|+0x00b:'300'
	// <0x5dffc1>|0x0a1|      :'300'	}
	// ******
}

// STATE[STUB]
void game_world::switch_to_player_camera( const bool first_person_view )
{
	// FUNCTION BODY[0x5dfe90]: 6
	// <0x5dfe90>|0x000|+0x000:'303'	{
	// <0x5dfe90>|0x000|+0x010:'304'
	// <0>
	// <0x5dfea0>|0x010|+0x011:'306'
	// <0x5dfeb1>|0x021|+0x026:'307'
	// <0x5dfed7>|0x047|+0x02d:'308'
	// <0x5dff04>|0x074|-0x005:'308'
	// <0>
	// <0x5dfeff>|0x06f|+0x014:'310'
	// <0x5dff13>|0x083|      :'310'	}
	// ******
}

// STATE[STUB]
void game_world::switch_to_free_fly_camera( )
{
	// legacy reached m_camera_director directly; it moved into base_game_scene
	// (private) - go through get_camera_director()
	get_camera_director( ).switch_to_camera	( m_free_fly_camera, "free fly camera" );

	// FUNCTION BODY[0x5dfe60]: 2
	// <0>
	// <0x5dfe60>|0x000|+0x022:'315'
	// ******
}

// STATE[STUB]
bool game_world::empty( )
{
	return false;

	// FUNCTION BODY[0x5df9f0]: 1
	// <0x5df9f0>|0x000|+0x00b:'328'
	// ******
}

// STATE[STUB]
void game_world::on_project_loaded(
	resources::queries_result&		data,
	const u32						results_offset,
	boost::function< void( resources::queries_result& ) > const&	callback
)
{
	// LOCALS
	// u32 								resource_index
	// variant< 32 > 					user_data
	// float3 							camera_position
	// float3 							camera_direction
	// u32 								i
	// u8 								i
	// ******

	// CALL SITE INFO
	// <0x5e162b> -> bool < unknown >() const
	// <0x5e1642> -> lobby_client& < unknown >()
	// <0x5e1695> -> void < unknown >( bool )
	// <0x5e16ad> -> match_options& < unknown >()
	// <0x5e17a7> -> match_options& < unknown >()
	// <0x5e18b8> -> sound::world_user& < unknown >() const
	// <0x5e18d2> -> bool < unknown >() const
	// <0x5e18e9> -> match_client& < unknown >()
	// <0x5e19d2> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5e0f40]: 109
	// <0x5e0f40>|0x000|+0x006:'336'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e0f46>|0x006|+0x00d:'341'
	// <0x5e0f53>|0x013|+0x010:'342'
	// <0x5e0f63>|0x023|+0x088:'343'
	// <0x5e0feb>|0x0ab|+0x07c:'344'
	// <0x5e1067>|0x127|+0x084:'345'
	// <0x5e10eb>|0x1ab|+0x057:'346'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e1142>|0x202|+0x02e:'352'
	// <0x5e1170>|0x230|+0x128:'353'
	// <0>
	// <0x5e1298>|0x358|+0x03c:'355'
	// <0x5e12d4>|0x394|+0x092:'356'
	// <0>
	// <0x5e1366>|0x426|+0x0f1:'358'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5e1457>|0x517|+0x00d:'368'
	// <0>
	// <0x5e1464>|0x524|+0x0bc:'370'
	// <0>
	// <0x5e1520>|0x5e0|+0x03e:'372'
	// <0>
	// <1>
	// <2>
	// <0x5e155e>|0x61e|+0x0bc:'376'
	// <0>
	// <0x5e161a>|0x6da|+0x033:'378'
	// <0x5e164d>|0x70d|+0x006:'379'
	// <0x5e1653>|0x713|+0x029:'380'
	// <0x5e167c>|0x73c|+0x010:'381'
	// <0>
	// <1>
	// <2>
	// <0x5e168c>|0x74c|+0x00b:'385'
	// <0>
	// <0x5e1697>|0x757|+0x03a:'387'
	// <0>
	// <0x5e16d1>|0x791|+0x063:'389'
	// <0x5e1734>|0x7f4|+0x03c:'390'
	// <0x5e1770>|0x830|+0x049:'391'
	// <0>
	// <0x5e17b9>|0x879|+0x018:'393'
	// <0>
	// <0x5e17d1>|0x891|+0x031:'395'
	// <0x5e1802>|0x8c2|+0x03b:'396'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5e183d>|0x8fd|+0x01e:'406'
	// <0x5e185b>|0x91b|+0x014:'407'
	// <0x5e186f>|0x92f|+0x021:'408'
	// <0>
	// <1>
	// <2>
	// <0x5e1890>|0x950|+0x009:'412'
	// <0x5e1899>|0x959|+0x028:'413'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e18c1>|0x981|+0x017:'420'
	// <0x5e18d8>|0x998|+0x025:'421'
	// <0>
	// <1>
	// <0x5e18fd>|0x9bd|+0x026:'424'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5e1923>|0x9e3|+0x092:'432'
	// <0>
	// <0x5e19b5>|0xa75|+0x00c:'434'
	// <0x5e19c1>|0xa81|+0x017:'435'
	// <0x5e19d8>|0xa98|+0x007:'436'
	// <0>
	// <1>
	// <2>
	// <0x5e19df>|0xa9f|+0x008:'440'
	// <0x5e19e7>|0xaa7|+0x009:'441'
	// <0>
	// <0x5e19f0>|0xab0|-0x36d:'443'
	// <0>
	// <1>
	// <0x5e1683>|0x743|+0x39c:'446'
	// <0x5e1a1f>|0xadf|      :'446'	}
	// ******
}

// STATE[STUB]
void game_world::unload( )
{
	// LOCALS
	// human_npc_ptr 					it_npc
	// ******

	// CALL SITE INFO
	// <0x5e0b70> -> void < unknown >()
	// <0x5e0d82> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5e0b30]: 27
	// <0x5e0b30>|0x000|+0x006:'449'	{
	// <0x5e0b36>|0x006|+0x023:'450'
	// <0>
	// <0x5e0b59>|0x029|+0x012:'452'
	// <0x5e0b6b>|0x03b|+0x007:'453'
	// <0>
	// <0x5e0b72>|0x042|+0x0ca:'455'
	// <0x5e0c3c>|0x10c|-0x097:'455'
	// <0x5e0ba5>|0x075|+0x0c4:'456'
	// <0x5e0c69>|0x139|+0x01a:'456'
	// <0>
	// <0x5e0c83>|0x153|+0x02a:'458'
	// <0x5e0cad>|0x17d|+0x006:'459'
	// <0x5e0cb3>|0x183|+0x054:'460'
	// <0>
	// <0x5e0d07>|0x1d7|+0x017:'462'
	// <0>
	// <0x5e0d1e>|0x1ee|+0x01b:'464'
	// <0x5e0d39>|0x209|+0x005:'465'
	// <0x5e0d3e>|0x20e|+0x027:'466'
	// <0>
	// <1>
	// <0x5e0d65>|0x235|+0x00e:'469'
	// <0>
	// <0x5e0d73>|0x243|+0x00a:'471'
	// <0x5e0d7d>|0x24d|+0x012:'472'
	// <0>
	// <0x5e0d8f>|0x25f|+0x02c:'474'
	// <0>
	// <0x5e0dbb>|0x28b|+0x035:'476'
	// <0x5e0df0>|0x2c0|-0x007:'476'
	// <0x5e0de9>|0x2b9|+0x010:'477'
	// <0x5e0df9>|0x2c9|      :'477'	}
	// ******
}

// STATE[STUB]
void game_world::load(
	pcstr						project_resource_name,
	resources::request*			requests_begin,
	resources::request*			requests_end,
	variant< 32 > const**		user_datas_begin,
	boost::function< void( resources::queries_result& ) > const&	callback
)
{
	// LOCALS
	// const u32 						requests_count
	// render::scene_configuration 		scene_configuration
	// buffer_vector< variant< 32 > const* > user_data_ptrs
	// const u8 						victory_items_count
	// buffer_vector< variant< 32 > > 	user_datas
	// buffer_vector< resources::request > requests
	// sound::sound_scene_creation_params sound_configuration
	// fixed_string< 8 > 				name
	// ******

	// CALL SITE INFO
	// <0x5e1a7d> -> match_options& < unknown >()
	// <0x5e1b52> -> ui::world& < unknown >()
	// <0x5e1b89> -> ui::world& < unknown >()
	// ******

	// FUNCTION BODY[0x5e1a50]: 101
	// <0>
	// <0x5e1a63>|0x013|+0x007:'488'
	// <0>
	// <0x5e1a6a>|0x01a|+0x01b:'490'
	// <0x5e1a85>|0x035|+0x00c:'491'
	// <0x5e1a91>|0x041|+0x01e:'492'
	// <0>
	// <0x5e1aaf>|0x05f|+0x00d:'494'
	// <0x5e1abc>|0x06c|+0x019:'495'
	// <0x5e1ad5>|0x085|+0x014:'496'
	// <0>
	// <0x5e1ae9>|0x099|+0x00d:'498'
	// <0x5e1af6>|0x0a6|+0x011:'499'
	// <0x5e1b07>|0x0b7|+0x019:'500'
	// <0>
	// <1>
	// <2>
	// <0x5e1b20>|0x0d0|+0x011:'504'
	// <0x5e1b31>|0x0e1|+0x037:'505'
	// <0x5e1b68>|0x118|+0x031:'506'
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
	// <0x5e1b99>|0x149|+0x05b:'520'
	// <0x5e1bf4>|0x1a4|+0x03b:'521'
	// <0>
	// <0x5e1c2f>|0x1df|+0x036:'523'
	// <0x5e1c65>|0x215|+0x045:'524'
	// <0>
	// <0x5e1caa>|0x25a|+0x013:'526'
	// <0x5e1cbd>|0x26d|+0x012:'527'
	// <0>
	// <0x5e1ccf>|0x27f|+0x016:'529'
	// <0x5e1ce5>|0x295|+0x00d:'530'
	// <0>
	// <0x5e1cf2>|0x2a2|+0x016:'532'
	// <0x5e1d08>|0x2b8|+0x00c:'533'
	// <0>
	// <0x5e1d14>|0x2c4|+0x016:'535'
	// <0x5e1d2a>|0x2da|+0x00a:'536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e1d34>|0x2e4|+0x00d:'543'
	// <0x5e1d41>|0x2f1|+0x016:'544'
	// <0x5e1d57>|0x307|+0x047:'545'
	// <0>
	// <1>
	// <0x5e1d9e>|0x34e|-0x031:'548'
	// <0x5e1d6d>|0x31d|+0x01f:'549'
	// <0x5e1d8c>|0x33c|+0x017:'550'
	// <0>
	// <1>
	// <2>
	// <0x5e1da3>|0x353|+0x00c:'554'
	// <0x5e1daf>|0x35f|+0x008:'555'
	// <0>
	// <0x5e1db7>|0x367|+0x0cb:'557'
	// <0>
	// <0x5e1e82>|0x432|+0x00d:'559'
	// <0>
	// <0x5e1e8f>|0x43f|+0x014:'561'
	// <0x5e1ea3>|0x453|+0x009:'562'
	// <0>
	// <1>
	// <0x5e1eac>|0x45c|+0x035:'565'
	// <0x5e1ee1>|0x491|+0x051:'566'
	// <0>
	// <0x5e1f32>|0x4e2|+0x017:'568'
	// <0x5e1f49>|0x4f9|+0x009:'569'
	// <0>
	// <1>
	// <2>
	// <0x5e1f52>|0x502|+0x014:'573'
	// <0>
	// <0x5e1f66>|0x516|+0x005:'575'
	// <0x5e1f6b>|0x51b|+0x01f:'576'
	// <0x5e1f8a>|0x53a|+0x031:'577'
	// <0x5e1fbb>|0x56b|-0x01d:'577'
	// <0x5e1f9e>|0x54e|+0x020:'578'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5e1fbe>|0x56e|+0x221:'587'
	// ******
}

// STATE[STUB]
void game_world::on_portal_system_loaded( resources::queries_result& data )
{
	// CALL SITE INFO
	// <0x5e0b1a> -> sound::world_user& < unknown >() const
	// ******

	// FUNCTION BODY[0x5e09a0]: 8
	// <0x5e09a0>|0x000|+0x019:'592'
	// <0>
	// <0x5e09b9>|0x019|+0x0aa:'594'
	// <0x5e0a63>|0x0c3|+0x084:'595'
	// <0>
	// <0x5e0ae7>|0x147|+0x018:'597'
	// <0x5e0aff>|0x15f|+0x024:'598'
	// <0>
	// ******
}

// STATE[STUB]
void game_world::on_activate( )
{
	// CALL SITE INFO
	// <0x5dfff2> -> input::world& < unknown >()
	// <0x5dfffc> -> void < unknown >( input::handler& )
	// <0x5e002c> -> sound::world_user& < unknown >() const
	// <0x5e0046> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5dffd0]: 9
	// <0x5dffd4>|0x004|+0x005:'604'
	// <0>
	// <0x5dffd9>|0x009|+0x025:'606'
	// <0>
	// <0x5dfffe>|0x02e|+0x018:'608'
	// <0x5e0016>|0x046|+0x01f:'609'
	// <0>
	// <0x5e0035>|0x065|+0x017:'611'
	// <0x5e004c>|0x07c|+0x016:'612'
	// ******
}

// STATE[STUB]
void game_world::on_deactivate( )
{
	// CALL SITE INFO
	// <0x5e096b> -> input::world& < unknown >()
	// <0x5e0975> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x5e0940]: 7
	// <0x5e0943>|0x003|+0x017:'617'
	// <0>
	// <0x5e095a>|0x01a|+0x01d:'619'
	// <0>
	// <0x5e0977>|0x037|+0x00b:'621'
	// <0>
	// <0x5e0982>|0x042|+0x01d:'623'
	// ******
}

// STATE[STUB]
void game_world::show_ui( bool b_show )
{
	// CALL SITE INFO
	// <0x5e0743> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5e0710]: 16
	// <0x5e0710>|0x000|+0x001:'627'	{
	// <0x5e0711>|0x001|+0x011:'628'
	// <0>
	// <1>
	// <0x5e0722>|0x012|+0x004:'631'
	// <0>
	// <0x5e0726>|0x016|+0x00c:'633'
	// <0x5e0732>|0x022|+0x017:'634'
	// <0x5e0749>|0x039|+0x026:'635'
	// <0>
	// <1>
	// <2>
	// <0x5e076f>|0x05f|+0x00d:'639'
	// <0x5e077c>|0x06c|+0x012:'640'
	// <0x5e078e>|0x07e|-0x02a:'641'
	// <0>
	// <0x5e0764>|0x054|+0x032:'643'
	// <0x5e0796>|0x086|-0x02a:'643'
	// <0x5e076c>|0x05c|+0x033:'644'
	// <0x5e079f>|0x08f|      :'644'	}
	// ******
}

// STATE[STUB]
void game_world::clear_resources( )
{
	// CALL SITE INFO
	// <0x5e0814> -> sound::world_user& < unknown >() const
	// <0x5e0890> -> void < unknown >()
	// <0x5e089d> -> void < unknown >()
	// <0x5e08a7> -> void < unknown >( bool )
	// ******

	// FUNCTION BODY[0x5e07b0]: 27
	// <0x5e07b6>|0x006|+0x035:'648'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e07eb>|0x03b|+0x017:'654'
	// <0x5e0802>|0x052|+0x01b:'655'
	// <0>
	// <0x5e081d>|0x06d|+0x034:'657'
	// <0x5e0851>|0x0a1|+0x034:'658'
	// <0>
	// <0x5e0885>|0x0d5|+0x00d:'660'
	// <0x5e0892>|0x0e2|+0x00d:'661'
	// <0>
	// <0x5e089f>|0x0ef|+0x00a:'663'
	// <0>
	// <0x5e08a9>|0x0f9|+0x00a:'665'
	// <0x5e08b3>|0x103|+0x017:'666'
	// <0>
	// <1>
	// <0x5e08ca>|0x11a|+0x013:'669'
	// <0>
	// <0x5e08dd>|0x12d|+0x005:'671'
	// <0x5e08e2>|0x132|+0x027:'672'
	// <0x5e0909>|0x159|+0x032:'673'
	// <0>
	// ******
}

// STATE[STUB]
sound::world& game_world::get_sound_world( ) const
{
	return get_game( ).get_sound_world( );	// buildability return (the obvious source)

	// FUNCTION BODY[0x5dfa90]: 1
	// <0x5dfa90>|0x000|+0x00c:'679'
	// ******
}

// STATE[STUB]
void game_world::add_enemy_position_for_team( pcstr const team_name )
{
	// CALL SITE INFO
	// <0x5dfd9d> -> float3 < unknown >() const
	// ******

	// FUNCTION BODY[0x5dfd80]: 6
	// <0x5dfd80>|0x000|+0x007:'684'	{
	// <0x5dfd87>|0x007|+0x018:'685'
	// <0>
	// <0x5dfd9f>|0x01f|+0x03a:'687'
	// <0x5dfdd9>|0x059|+0x008:'688'
	// <0x5dfde1>|0x061|+0x030:'689'
	// <0x5dfe11>|0x091|+0x02f:'690'
	// <0x5dfe40>|0x0c0|-0x006:'690'
	// <0x5dfe3a>|0x0ba|+0x012:'691'
	// <0x5dfe4c>|0x0cc|      :'691'	}
	// ******
}

// STATE[STUB]
void game_world::clear_enemies_positions_for_team( pcstr const team_name )
{
	// FUNCTION BODY[0x5dfab0]: 4
	// <0x5dfab0>|0x000|+0x001:'694'	{
	// <0x5dfab1>|0x001|+0x038:'695'
	// <0x5dfae9>|0x039|+0x02e:'696'
	// <0x5dfb17>|0x067|+0x032:'697'
	// <0x5dfb49>|0x099|-0x035:'698'
	// <0x5dfb14>|0x064|+0x060:'699'
	// <0x5dfb74>|0x0c4|      :'699'	}
	// ******
}

// STATE[STUB]
void game_world::clear_player_spawn_info( )
{
	// FUNCTION BODY[0x5dfa50]: 8
	// <0x5dfa50>|0x000|+0x00d:'703'
	// <0x5dfa5d>|0x00d|+0x006:'704'
	// <0>
	// <0x5dfa63>|0x013|+0x00d:'706'
	// <0>
	// <1>
	// <0x5dfa70>|0x020|+0x01a:'709'
	// <0>
	// ******
}

// STATE[STUB]
void game_world::on_player_killed( player_ptr& player, const u8 arg_1, const u8 arg_2 )
{
	// FUNCTION BODY[0x5dfaa0]: 11
	// <0x5dfaa0>|0x000|+0x000:'720'	{
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
	// <0x5dfaa0>|0x000|      :'732'	}
	// ******
}

// STATE[STUB]
u32 game_world::get_current_time_in_ms( ) const
{
	return 0;

	// FUNCTION BODY[0x5dfa40]: 1
	// <0x5dfa40>|0x000|+0x009:'741'
	// ******
}

// STATE[STUB]
void game_world::set_local_player_camera( player_input_handler* camera )
{
	// FUNCTION BODY[0x5df9e0]: 1
	// <0x5df9e0>|0x000|+0x006:'746'
	// ******
}

// STATE[STUB]
void game_world::put_victory_item( u8 item_id, float4x4 const& transform )
{
	// CALL SITE INFO
	// <0x5dfa2b> -> void < unknown >( physics::world*, float4x4 const&, scheduler& )
	// ******

	// FUNCTION BODY[0x5dfa00]: 1
	// <0x5dfa00>|0x000|+0x02e:'751'
	// ******
}


} // namespace survarium
