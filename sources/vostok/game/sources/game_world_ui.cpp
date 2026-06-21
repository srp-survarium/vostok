////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_world_ui.h"
#include "game_world.h"	// m_game_world.get_game()
#include "game.h"		// game::text_translator() / get_network_client()
#include "game_project.h"	// complete simple_game_project for get_project()->m_config
#include "camera_director.h"	// get_camera_director().get_inverted_view_matrix()
#include "base_network_client.h"	// match_options() / get_player() / current_player_team()
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor
#include <vostok/configs_binary_config.h>	// binary_config::get_root() (project_name lookup)
#include <vostok/network_core/packet_reader.h>	// packet.r<u32>() (initialize_base_points)
#include <vostok/game_core/game_net_defines.h>	// complete match_options / player_profile
#include <vostok/game_core/weapon_core.h>		// get_ammo_info() / cast_weapon_core()
#include <vostok/game_core/weapon_ammo_info.h>
#include <vostok/game_core/inventory_item_props.h>	// create_slot_value item props
#include <vostok/game_core/dictionary_item.h>		// item_by_id().item_cfg
#include "key_binder.h"			// get_action_dik / dik_to_ptr (create_slot_value hotkey)
#include "keyboard_key_descr.h"	// keyboard_key_descr::key_name
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/console_command.h>

namespace survarium {

static bool is_ui_minimap_rotable		= true;
static bool is_ui_minimap_rotable_old	= is_ui_minimap_rotable;
// claude@NOTE: cc_bool console-command registration is /Od + cross-module-walled
// (the ctor inlines into a dynamic initializer + atexit destructor; the cc_bool
// vtable/console-command-root linkage is the known cross-module cc_bool park).
static console_commands::cc_bool s_is_ui_minimap_rotable( "is_ui_minimap_rotable", is_ui_minimap_rotable, true, console_commands::command_type_user_specific );

game_world_ui::game_world_ui( game_world& w ) :
	m_game_world( w ),
	m_players_list_visible( false ),
	m_game_mode( capture_enemy_base ),
	m_match_time( 0 )
{
	m_victory_points[0] = m_victory_points[1] = 0;
}

void game_world_ui::initialize_resources( resources::unmanaged_resource_ptr const& game_hud )
{
	m_game_hud_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( game_hud );

	m_game_hud_ui->movie->SetBackgroundAlpha( 0.0f );
	m_game_hud_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_game_hud_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );
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

void game_world_ui::initialize_base_points( network_core::packet_reader& packet )
{
	u32 const points_count = packet.r< u32 >( );
	for ( u32 i = 0; i < points_count; ++i )
	{
		u32 point_id				= packet.r< u32 >( );
		base_point_stats stats;
		stats.point_owner			= (game_team_id)packet.r< u32 >( );
		stats.team_points_amount	= packet.r< u32 >( );
		stats.capture_progress		= packet.r< u32 >( );

		if ( m_game_mode != capture_neutral_base || stats.point_owner == team_neutral )
		{
			m_base_points[point_id] = stats;
			set_base_capture_progress( stats.capture_progress, point_id );
		}
	}
}

void game_world_ui::add_victory_points( s8 team_1_points, s8 team_2_points )
{
	game_team_id const local_player_team = m_game_world.get_game( ).get_network_client( )->current_player_team( );

	m_victory_points[0] += team_1_points;
	flash_value args[2];
	args[0].SetBoolean( local_player_team != team_1 );
	args[1].SetUInt( m_victory_points[0] );
	get_ui( )->movie->Invoke( "root.set_artifacts_progress", NULL, args, 2 );

	m_victory_points[1] += team_2_points;
	args[0].SetBoolean( local_player_team != team_2 );
	args[1].SetUInt( m_victory_points[1] );
	get_ui( )->movie->Invoke( "root.set_artifacts_progress", NULL, args, 2 );
}

void game_world_ui::set_victory_points( s8 team_1_points, s8 team_2_points )
{
	game_team_id const local_player_team = m_game_world.get_game( ).get_network_client( )->current_player_team( );

	m_victory_points[0] = team_1_points;
	flash_value args[2];
	args[0].SetBoolean( local_player_team != team_1 );
	args[1].SetUInt( team_1_points );
	get_ui( )->movie->Invoke( "root.set_artifacts_progress", NULL, args, 2 );

	m_victory_points[1] = team_2_points;
	args[0].SetBoolean( local_player_team != team_2 );
	args[1].SetUInt( team_2_points );
	get_ui( )->movie->Invoke( "root.set_artifacts_progress", NULL, args, 2 );
}

game_world_ui::~game_world_ui( )
{
}

// claude@NOTE: flash /Od wall - the two flash_value[3] branches share a folded
// SetString/Invoke tail; the percent fistp + flash glue is /Od-shaped.
void game_world_ui::set_base_capture_progress( u32 progress, u32 point_id )
{
	base_point_stats& stats = m_base_points[point_id];

	char buff[64];
	if ( progress == stats.team_points_amount )
		vostok::sprintf( buff, "captured!" );
	else
		vostok::sprintf( buff, "(%d/%d)", progress, stats.team_points_amount );

	u32 const percent = (u32)math::floor( ( (float)progress / (float)stats.team_points_amount ) * 100.0f );

	if ( m_game_mode == capture_enemy_base )
	{
		flash_value args[3];
		args[0].SetUInt( stats.point_owner );
		args[1].SetString( buff );
		args[2].SetUInt( percent );
		get_ui( )->movie->Invoke( "root.set_capture_progress", NULL, args, 3 );
	}
	else if ( m_game_mode == capture_neutral_base )
	{
		flash_value args[3];
		args[0].SetUInt( 3 );
		args[1].SetString( "" );
		args[2].SetUInt( percent );
		get_ui( )->movie->Invoke( "root.set_capture_progress", NULL, args, 3 );
	}
}

void game_world_ui::set_match_time( u32 time_left_ms )
{
	char buff[64];
	if ( time_left_ms == 0 )
		vostok::sprintf( buff, "MATCH TIME IS UP!!!!" );
	else
		vostok::sprintf(
			buff,
			"MATCH TIME: %02d : %02d",
			math::floor( time_left_ms * ( 1.0f / 60000.0f ) ),
			math::floor( ( time_left_ms - math::floor( time_left_ms * ( 1.0f / 60000.0f ) ) * 60000 ) * 0.001f )
		);

	flash_value match_time_str;
	match_time_str.SetString( buff );
	get_ui( )->movie->Invoke( "root.set_match_time", NULL, &match_time_str, 1 );
}

void game_world_ui::show_pregame( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );	get_ui( )->movie->Invoke( "root.show_pregame", NULL, &b_val, 1 );
}

void game_world_ui::set_pregame( pcstr str, u32 time_left )
{
	wchar_t message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( str, message );

	flash_value message_val;
	wchar_t buff[512];
	swprintf( buff, 512, L"%s %02d : %02d", message, time_left / 60, time_left % 60 );

	message_val.SetStringW( buff );
	get_ui( )->movie->Invoke( "root.set_pregame", NULL, &message_val, 1 );
}

void game_world_ui::set_respawn_time( u32 time_left )
{
	char buff[64];
	if ( time_left == 0 )
		vostok::sprintf( buff, "" );
	else
		vostok::sprintf( buff, "WILL RESPAWN IN  %02d : %02d", time_left / 60, time_left % 60 );

	flash_value respawn_time_str;
	respawn_time_str.SetString( buff );
	get_ui( )->movie->Invoke( "root.set_respawn_time", NULL, &respawn_time_str, 1 );
}

void game_world_ui::set_player_kills_deaths( u8 player_id, u32 kills, u32 deaths )
{
	flash_value out_event;				get_ui( )->movie->CreateObject( &out_event );
	flash_value out_event_property;		get_ui( )->movie->CreateObject( &out_event_property );

	out_event_property.SetUInt( player_id );
	out_event.SetMember( "id", out_event_property );
	out_event_property.SetUInt( kills );
	out_event.SetMember( "kills", out_event_property );
	out_event_property.SetUInt( deaths );
	out_event.SetMember( "deaths", out_event_property );
	out_event_property.SetUInt( 0 );
	out_event.SetMember( "ping", out_event_property );
	out_event_property.SetUInt( 0 );
	out_event.SetMember( "rank", out_event_property );
	out_event_property.SetUInt( 0 );
	out_event.SetMember( "artifacts", out_event_property );

	get_ui( )->movie->Invoke( "root.list_update_player", NULL, &out_event, 1 );
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

void game_world_ui::on_unload( )
{
	// claude@NOTE: capped by scaleform flash_movie - the inlined movie reset at
	// vtable+0x58 forwards to a GFx::Movie virtual our flash_movie wrapper stubs out.
	if ( m_game_hud_ui && m_game_hud_ui->movie )
		m_game_hud_ui->movie->Restart( );

	m_victory_points[0] = m_victory_points[1] = 0;
}

void game_world_ui::set_health( u8 health_in_percentage )
{
	flash_value value;
	value.SetUInt( health_in_percentage );
	get_ui( )->movie->Invoke( "root.set_player_hp", NULL, &value, 1 );
}

// claude@NOTE: flash /Od wall - the trailing flash_value[2] SetNumber/Invoke glue
// and the angle-conversion constant are /Od-shaped (structure faithful, bytes parked).
void game_world_ui::on_hit_from_pos( float3 position )
{
	float4x4 const actor_camera_matrix = m_game_world.get_camera_director( ).get_inverted_view_matrix( );

	float3 direction_vector = -( position - actor_camera_matrix.c.xyz( ) );
	direction_vector.normalize( );

	float4x4 const initiator_matrix = math::invert4x3( math::create_camera_direction( position, direction_vector, float3( 0.0f, 1.0f, 0.0f ) ) );

	float const angle = initiator_matrix.get_angles( math::rotation_zxy ).y - actor_camera_matrix.get_angles( math::rotation_zxy ).y;

	flash_value args[2];
	args[0].SetNumber( angle );
	args[1].SetNumber( -angle - math::pi_d2 );
	get_ui( )->movie->Invoke( "root.hit_player", NULL, args, 2 );
}

void game_world_ui::show_parametrized_message(
	pcstr		message_id,
	u8			font_size,
	u8			y_pos_in_percents,
	u32			timeout_in_ms
)
{
	wchar_t message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( message_id, message );

	flash_value message_val[4];
	message_val[0].SetStringW( message );
	message_val[1].SetUInt( 25 );
	message_val[2].SetUInt( 20 );
	message_val[3].SetUInt( 3000 );
	get_ui( )->movie->Invoke( "root.set_parameterized_message", NULL, message_val, 4 );
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

void game_world_ui::set_crosshair_size( float size )
{
	flash_value value;
	value.SetNumber( size );	get_ui( )->movie->Invoke( "root.set_crosshair_size", NULL, &value, 1 );
}

void game_world_ui::on_enemy_hitted( )
{
	get_ui( )->movie->Invoke( "root.crosshair_enemy_hit", NULL, NULL, 0 );
}

void game_world_ui::show_crosshair( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );	get_ui( )->movie->Invoke( "root.show_crosshair", NULL, &b_val, 1 );
}

void game_world_ui::show_ammo_indicator( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );	get_ui( )->movie->Invoke( "root.show_ammo", NULL, &b_val, 1 );
}

void game_world_ui::show_capture_progress( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );	get_ui( )->movie->Invoke( "root.show_capture_progress", NULL, &b_val, 1 );
}

void game_world_ui::set_fire_queue_size( const u32 fire_queue_size )
{
	flash_value value;
	value.SetUInt( fire_queue_size );	get_ui( )->movie->Invoke( "root.set_weapon_fire_queue_size", NULL, &value, 1 );
}

void game_world_ui::set_ammo_in_magazine( const u32 count )
{
	flash_value value;
	value.SetUInt( count );	get_ui( )->movie->Invoke( "root.set_weapon_ammo_size", NULL, &value, 1 );
}

void game_world_ui::set_ammo_type( const u8 ammo_type )
{
	flash_value value;
	value.SetUInt( ammo_type );
	get_ui( )->movie->Invoke( "root.set_weapon_ammo_type", NULL, &value, 1 );
}

void game_world_ui::show_players_list( bool b_show )
{
	if ( m_players_list_visible == b_show )
		return;

	flash_value b_val;
	b_val.SetBoolean( b_show );	get_ui( )->movie->Invoke( "root.show_player_list", NULL, &b_val, 1 );
	m_players_list_visible = b_show;
}

void game_world_ui::on_damage_affect_applying(
	pcstr							bodypart,
	const hit_affects_type_enum		affect,
	const affect_event_type_enum	event_type
)
{
	u8 body_part_index = 0;
	if ( affect == affects_type_hand_damage )
		body_part_index = ( strcmp( bodypart, "left_hand" ) == 0 ) ? 1 : 2;
	else if ( affect == affects_type_leg_damage )
		body_part_index = ( strcmp( bodypart, "left_leg" ) == 0 ) ? 3 : 4;

	flash_value value;
	if ( event_type == affect_applying )
	{
		value.SetUInt( body_part_index );
		get_ui( )->movie->Invoke( "root.crit_player_body_part", NULL, &value, 1 );
	}
	else if ( event_type == affect_recalling || event_type == affect_canceling )
	{
		value.SetUInt( body_part_index );
		get_ui( )->movie->Invoke( "root.heal_player_body_part", NULL, &value, 1 );
	}
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

// claude@NOTE: flash /Od wall - project_name fetch reuses one get_project() temp in
// the target (register-only, unrecorded local); the value_exists/operator[] ternary +
// CreateObject/SetMember/Invoke glue is /Od-shaped. Structure faithful, bytes parked.
void game_world_ui::initialize_minimap( )
{
	pcstr const project_name = m_game_world.get_project( )->m_config->get_root( ).value_exists( "project_name" )
		? m_game_world.get_project( )->m_config->get_root( )[ "project_name" ]
		: "";

	flash_value minimap_props_value;
	get_ui( )->movie->CreateObject( &minimap_props_value );
	flash_value minimap_props_value_property;
	get_ui( )->movie->CreateObject( &minimap_props_value_property );

	minimap_props_value_property.SetString( project_name );
	minimap_props_value.SetMember( "map", minimap_props_value_property );

	get_ui( )->movie->Invoke( "root.set_minimap_props", NULL, &minimap_props_value, 1 );

	update_minimap_objects( );
	reset_map_rotatable( );
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

void game_world_ui::reset_map_rotatable( )
{
	flash_value b_val;
	b_val.SetBoolean( is_ui_minimap_rotable );
	get_ui( )->movie->Invoke( "root.set_rotable", NULL, &b_val, 1 );	is_ui_minimap_rotable_old = is_ui_minimap_rotable;
}

void game_world_ui::set_ammo_total_count( u32 first_type_count, u32 second_type_count )
{
	flash_value count;
	count.SetUInt( first_type_count );	get_ui( )->movie->Invoke( "root.set_primary_ammo", NULL, &count, 1 );
	count.SetUInt( second_type_count );
	get_ui( )->movie->Invoke( "root.set_secondary_ammo", NULL, &count, 1 );
}

void game_world_ui::show_quick_slots( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );	get_ui( )->movie->Invoke( "root.show_slots", NULL, &b_val, 1 );
	m_slots_to_update.clear( );
}

// claude@NOTE: flash /Od wall - the SetMember/CreateObject glue and the hotkey
// switch -> key_binder lookup are /Od-shaped; structure faithful, bytes parked.
void game_world_ui::create_slot_value(
	profile_slot_enum			slot,
	inventory_item_props&		item_props,
	flash_value&				slot_descr_value
)
{
	u8 const item_icon	= m_game_world.get_game( ).items_dictionary( ).item_by_id( item_props.m_dict_id ).item_cfg->get_root( )[ "ui_desc" ][ "icon" ];

	bool const enabled	= item_props.m_amount > 0;

	pcstr hotkey = "";
	switch ( slot )
	{
		case quick_slot1:	hotkey = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( kQUICK_USE_1, 0 ), false )->key_name;	break;
		case quick_slot2:	hotkey = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( kQUICK_USE_2, 0 ), false )->key_name;	break;
		case quick_slot3:	hotkey = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( kQUICK_USE_3, 0 ), false )->key_name;	break;
		case quick_slot4:	hotkey = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( kQUICK_USE_4, 0 ), false )->key_name;	break;
		case quick_slot5:	hotkey = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( kQUICK_USE_5, 0 ), false )->key_name;	break;
		case quick_slot6:	hotkey = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( kQUICK_USE_6, 0 ), false )->key_name;	break;
	}

	flash_value slot_descr_valuec_property;
	get_ui( )->movie->CreateObject( &slot_descr_valuec_property );

	slot_descr_valuec_property.SetUInt( item_icon );
	slot_descr_value.SetMember( "icon", slot_descr_valuec_property );

	slot_descr_valuec_property.SetUInt( item_props.m_amount );
	slot_descr_value.SetMember( "count", slot_descr_valuec_property );

	slot_descr_valuec_property.SetUInt( 0 );
	slot_descr_value.SetMember( "cooldown", slot_descr_valuec_property );

	slot_descr_valuec_property.SetBoolean( enabled );
	slot_descr_value.SetMember( "enabled", slot_descr_valuec_property );

	slot_descr_valuec_property.SetString( hotkey );
	slot_descr_value.SetMember( "hotkey", slot_descr_valuec_property );
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

void game_world_ui::show_screen_message( pcstr message_id )
{
	wchar_t message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( message_id, message );

	flash_value message_val;
	message_val.SetStringW( message );
	get_ui( )->movie->Invoke( "root.set_context_message", NULL, &message_val, 1 );
}

void game_world_ui::set_using_progress_message( u32 progress_value )
{
	wchar_t message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( "st_using_progress_message", message );

	flash_value message_val[3];
	message_val[0].SetStringW( message );
	message_val[1].SetUInt( progress_value );
	message_val[2].SetUInt( 20 );
	get_ui( )->movie->Invoke( "root.set_context", NULL, message_val, 3 );
}

void game_world_ui::set_using_info_message( pcstr str )
{
	wchar_t message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( str, message );

	flash_value message_val[3];
	message_val[0].SetStringW( message );
	message_val[1].SetUInt( 0 );
	message_val[2].SetUInt( 1000 );
	get_ui( )->movie->Invoke( "root.set_context", NULL, message_val, 3 );
}

void game_world_ui::add_quick_slot_to_update( profile_slot_enum slot )
{
	m_slots_to_update.push_back( slot );
}

void game_world_ui::disactivate_quick_slot( profile_slot_enum slot )
{
	vector< profile_slot_enum >::iterator it	= m_slots_to_update.begin( );
	vector< profile_slot_enum >::iterator end	= m_slots_to_update.end( );
	for ( ; it != end; ++it )
		if ( *it == slot )
			m_slots_to_update.erase( it );
}

void game_world_ui::show_chat( bool b_show )
{
	VOSTOK_UNREFERENCED_PARAMETER( b_show );
}

void game_world_ui::set_broken_connection_message( pcstr str )
{
	VOSTOK_UNREFERENCED_PARAMETER( str );

	if ( !get_ui( ) )
		return;

	wchar_t w_message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( "match server connection lost", w_message );

	flash_value message_val[2];
	message_val[0].SetStringW( w_message );
	message_val[1].SetUInt( 1000 );
	get_ui( )->movie->Invoke( "root.set_warning_message", NULL, message_val, 2 );
}

void game_world_ui::on_attached_to_player( player_ptr player )
{
	if ( !player->is_alive( ) )
	{
		show_ammo_indicator( false );
		show_quick_slots( false );
		return;
	}

	show_quick_slots( true );
	fill_quick_slots( );

	weapon_core* const weapon = player->get_current_active_object( )->cast_weapon_core( );
	if ( !weapon )
	{
		show_ammo_indicator( false );
		return;
	}

	weapon_ammo_info info;
	weapon->get_ammo_info( info );
	show_ammo_indicator( true );
	set_ammo_total_count( info.ammo1_total, info.ammo2_total );
	set_ammo_in_magazine( info.ammo_in_magazine + ( info.round_is_chambered ? 1 : 0 ) );
	set_fire_queue_size( info.fire_queue_size );
	set_ammo_type( info.current_ammo_type );
}

void game_world_ui::on_detached_from_player( )
{
	show_ammo_indicator( false );
	show_quick_slots( false );
}

void game_world_ui::show_item_container( u8 visual_id )
{
	flash_value visual_id_val;
	visual_id_val.SetUInt( visual_id );	get_ui( )->movie->Invoke( "root.show_container_icon", NULL, &visual_id_val, 1 );
}

void game_world_ui::hide_item_container( )
{
	get_ui( )->movie->Invoke( "root.hide_container_icon", NULL, NULL, 0 );
}

void game_world_ui::set_player_online_status( u32 player_id, bool is_online )
{
	wchar_t w_player_name[512];
	mbstowcs_s(
		NULL,
		w_player_name,
		512,
		m_game_world.get_game( ).get_network_client( )->match_options( ).player_profiles[player_id].profile_name,
		_TRUNCATE
	);

	flash_value player_online_value[2];
	player_online_value[0].SetStringW( w_player_name );
	player_online_value[1].SetBoolean( is_online );
	get_ui( )->movie->Invoke( "root.set_online_player", NULL, player_online_value, 2 );
}

} // namespace survarium
