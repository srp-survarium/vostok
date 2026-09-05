// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_world_ui.h"
#include "game_world.h"	// m_game_world.get_game()
#include "game.h"		// game::text_translator() / get_network_client()
#include "game_project.h"	// simple_game_project::project_name
#include "camera_director.h"	// get_camera_director().get_inverted_view_matrix()
#include "chat_handler.h"	// get_chat_handler().get_movie() (update_ui minimap advance)
#include "base_network_client.h"	// match_options() / get_player() / current_player_team()
#include "network_client.h"
#include "player.h" // complete type for player_ptr (intrusive_ptr<player>) dtor
#include <vostok/network_core/packet_reader.h>	// packet.r<u32>() (initialize_base_points)
#include <vostok/game_core/game_net_defines.h>	// complete match_options / player_profile
#include <vostok/game_core/weapon_core.h>		// get_ammo_info() / cast_weapon_core()
#include <vostok/game_core/weapon_ammo_info.h>
#include <vostok/game_core/inventory_item_props.h>	// create_slot_value item props
#include <vostok/game_core/inventory.h>			// update_minimap_objects: get_victory_item
#include "victory_items_container.h"			// update_minimap_objects: team/get_transform
#include "victory_item.h"						// update_minimap_objects: spotted/get_transform
#include <vostok/game_core/dictionary_item.h>		// item_by_id().item_cfg
#include "key_binder.h"			// get_action_dik / dik_to_ptr (create_slot_value hotkey)
#include "keyboard_key_descr.h"	// keyboard_key_descr::key_name
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/console_command.h>

static bool is_ui_minimap_rotable		= false;
static bool is_ui_minimap_rotable_old	= is_ui_minimap_rotable;
// The retained previous value detects user-specific console changes.


static console_commands::cc_bool s_is_ui_minimap_rotable( "is_ui_minimap_rotable", is_ui_minimap_rotable, true, console_commands::command_type_user_specific );
namespace survarium {
game_world_ui::game_world_ui( game_world& w ) :
	m_game_world( w ),
	m_players_list_visible( false ),
	m_game_mode( capture_enemy_base ),
	m_match_time( 0 )
{
	memory::zero( m_victory_points );
}

void game_world_ui::initialize_resources( resources::unmanaged_resource_ptr const& game_hud )
{
	m_game_hud_ui = static_cast_resource_ptr< flash_movie_resource_ptr >( game_hud );

	m_game_hud_ui->movie->SetBackgroundAlpha( 0.0f );
	m_game_hud_ui->movie->SetViewAlignment( flash_movie::Align_TopLeft );
	m_game_hud_ui->movie->SetViewScaleMode( flash_movie::SM_NoScale );
}

void game_world_ui::initialize( match_options& options )
{
	flash_value victory_items_count_val;
	flash_value game_mode_val;

	wchar_t team_name[512];
	m_game_world.get_game( ).text_translator( ).translate_text( "st_label_teamA", team_name );
	victory_items_count_val.SetStringW( team_name );

	get_ui( )->movie->SetVariable( "root.player_list.players.team1.text", victory_items_count_val );

	m_game_world.get_game( ).text_translator( ).translate_text( "st_label_teamB", team_name );
	victory_items_count_val.SetStringW( team_name );

	get_ui( )->movie->SetVariable( "root.player_list.players.team2.text", victory_items_count_val );

	flash_value v;
	v.SetUInt( 0xff0000 );
	get_ui( )->movie->SetVariable( "root.player_list.players.team2.textColor", v );

	m_game_mode				= options.match_mode_;
	m_match_time			= options.match_time;
	m_victory_items_count	= options.victory_items_count;

	game_team_id local_player_team = team_undefined;

	for ( u8 i = 0; i < 20; ++i )
		if ( options.player_profiles[i].is_local )
			local_player_team = options.player_profiles[i].team;

	victory_items_count_val.SetUInt( m_victory_items_count );
	get_ui( )->movie->Invoke( "root.set_artifacts_required", NULL, &victory_items_count_val, 1 );

	game_mode_val.SetUInt( m_game_mode );
	get_ui( )->movie->Invoke( "root.set_game_type", NULL, &game_mode_val, 1 );

	flash_value players_array;
	get_ui( )->movie->CreateArray( &players_array );
	u32 in_array_index = 0;
	for ( u8 i = 0; i < 20; ++i )
	{
		if ( options.player_profiles[i].team == team_undefined )
			continue;

		flash_value player_item;
		get_ui( )->movie->CreateObject( &player_item );

		flash_value player_item_property;
		get_ui( )->movie->CreateObject( &player_item_property );

		player_item_property.SetInt( in_array_index );
		player_item.SetMember( "id", player_item_property );

		player_item_property.SetInt( options.player_profiles[i].team != local_player_team ? 2 : 1 );
		player_item.SetMember( "team", player_item_property );

		wchar_t profile_name_w[32];
		mbstowcs_s( NULL, profile_name_w, 32, options.player_profiles[i].profile_name, _TRUNCATE );
		player_item_property.SetStringW( profile_name_w );
		player_item.SetMember( "name", player_item_property );

		player_item_property.SetInt( 66 );
		player_item.SetMember( "ping", player_item_property );

		player_item_property.SetInt( 0 );
		player_item.SetMember( "rank", player_item_property );

		player_item_property.SetInt( 0 );
		player_item.SetMember( "artifacts", player_item_property );

		players_array.SetElement( in_array_index, player_item );
		++in_array_index;
	}

	get_ui( )->movie->Invoke( "root.list_set_players", NULL, &players_array, 1 );
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
	game_team_id const local_player_team = m_game_world.get_game( ).get_network_client( )->get_current_player( )->team( );

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
	game_team_id const local_player_team = m_game_world.get_game( ).get_network_client( )->get_current_player( )->team( );

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

void game_world_ui::set_base_capture_progress( u32 progress, u32 point_id )
{
	base_point_stats& stats = m_base_points[point_id];

	char buff[64];
	if ( progress == stats.team_points_amount )
		vostok::sprintf( buff, "captured!" );
	else
		vostok::sprintf( buff, "(%d/%d)", progress, stats.team_points_amount );

	u32 const percent = (u32)( ( (float)progress / (float)stats.team_points_amount ) * 100.0f );

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
	b_val.SetBoolean( b_show );
	get_ui( )->movie->Invoke( "root.show_pregame", NULL, &b_val, 1 );
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
	flash_value out_event;
	get_ui( )->movie->CreateObject( &out_event );
	flash_value out_event_property;
	get_ui( )->movie->CreateObject( &out_event_property );

	out_event_property.SetInt( player_id );
	out_event.SetMember( "id", out_event_property );
	out_event_property.SetInt( kills );
	out_event.SetMember( "kills", out_event_property );
	out_event_property.SetInt( deaths );
	out_event.SetMember( "deaths", out_event_property );
	out_event_property.SetInt( 66 );
	out_event.SetMember( "ping", out_event_property );
	out_event_property.SetInt( 0 );
	out_event.SetMember( "rank", out_event_property );
	out_event_property.SetInt( 0 );
	out_event.SetMember( "artifacts", out_event_property );

	get_ui( )->movie->Invoke( "root.list_update_player", NULL, &out_event, 1 );
}

// claude@NOTE: PDB locals and CFG match. Retail LTCG passes this in edi and
// frame_delta_ms in ecx, drops __formal, and devirtualizes the byte-exact
// network_client::is_player_local; base keeps thiscall arguments and the vcall.
// Reopen after game_world::tick/network_client compiler-context changes.
void game_world_ui::update_ui( const u32 frame_delta_ms, const u32 __formal )
{
	VOSTOK_UNREFERENCED_PARAMETER( __formal );

	get_ui( )->movie->Advance( frame_delta_ms * 0.001f, 0 );

	if ( m_players_list_visible )
	{
		base_network_client* const client = m_game_world.get_game( ).get_network_client( );
		for ( u8 i = 0; i < client->match_options( ).players_count; ++i )
		{
			if ( client->get_player( i ) )
				set_player_online_status( i, client->is_player_local( i ) );
		}
	}

	m_game_world.get_game( ).get_chat_handler( ).get_movie( )->movie->Advance( frame_delta_ms * 0.001f, 0 );

	if ( is_ui_minimap_rotable_old != is_ui_minimap_rotable )
		reset_map_rotatable( );

	update_minimap_local_player( );

	if ( m_game_world.get_game( ).get_network_client( )->has_bandwidth( ) )
		update_minimap_players( );

	if ( m_slots_to_update.size( ) )
	{
		vector< profile_slot_enum >::iterator it	= m_slots_to_update.begin( );
		vector< profile_slot_enum >::iterator end	= m_slots_to_update.end( );
		for ( ; it != end; ++it )
			update_quick_slot( *it );
	}
}

void game_world_ui::on_unload( )
{
	if ( m_game_hud_ui )
		m_game_hud_ui->movie->Restart( );

	memory::zero( m_victory_points );
}

void game_world_ui::set_health( u8 health_in_percentage )
{
	flash_value value;
	value.SetUInt( health_in_percentage );
	get_ui( )->movie->Invoke( "root.set_player_hp", NULL, &value, 1 );
}

// claude@NOTE: Body and PDB structure match. Retail's sole
// player::apply_hit_directly caller stack-passes this; base has no real caller
// while that player function remains stubbed. Reopen with that caller.
void game_world_ui::on_hit_from_pos( float3 position )
{
	float4x4 const actor_camera_matrix = m_game_world.get_camera_director( ).get_inverted_view_matrix( );

	float3 direction_vector = -( position - actor_camera_matrix.c.xyz( ) );
	direction_vector.normalize( );

	float4x4 const initiator_matrix = math::invert4x3( math::create_camera_direction( position, direction_vector, float3( 0.0f, 1.0f, 0.0f ) ) );

	float const angle = initiator_matrix.get_angles( math::rotation_zxy ).y - actor_camera_matrix.get_angles( math::rotation_zxy ).y;

	flash_value args[2];
	args[0].SetNumber( -angle - math::pi_d2 );
	args[1].SetNumber( 50.f );
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

void game_world_ui::on_victory_item_put_take( u8 player_id, bool is_taken, bool is_base )
{
	update_minimap_objects( );

	base_network_client* const client = m_game_world.get_game( ).get_network_client( );
	player_ptr player = client->get_player( player_id );

	game_team_id local_player_team = team_1;
	u8 i = 0;
	for ( ; i < 20; ++i )
	{
		if ( client->is_player_local( i ) )
		{
			local_player_team = client->match_options( ).player_profiles[i].team;
			break;
		}
	}

	if ( player->team( ) != local_player_team )
	{
		if ( is_taken && is_base )
			show_parametrized_message( "st_on_enemy_theft_item", 0, 0, 0 );
		return;
	}

	flash_value out_event;
	get_ui( )->movie->CreateObject( &out_event );
	flash_value out_event_property;

	u8 action_id;
	if ( is_taken )
	{
		action_id = 3;
		if ( client->is_player_local( player_id ) )
			show_parametrized_message( "st_bring_item_to_base", 0, 0, 0 );
	}
	else
		action_id = is_base ? 5 : 4;

	out_event_property.SetUInt( action_id );
	out_event.SetMember( "action_id", out_event_property );

	out_event_property.SetStringW( player->get_profile_name( ) );
	out_event.SetMember( "who_name", out_event_property );

	out_event_property.SetUInt( client->is_player_current( player_id ) ? 2 : player->team( ) );
	out_event.SetMember( "who_team", out_event_property );

	out_event_property.SetStringW( player->get_profile_name( ) );
	out_event.SetMember( "victim_name", out_event_property );

	out_event_property.SetUInt( client->is_player_current( player_id ) ? 2 : player->team( ) );
	out_event.SetMember( "victim_team", out_event_property );

	out_event_property.SetUInt( 0 );
	out_event.SetMember( "object_icon", out_event_property );
	out_event_property.SetUInt( 0 );
	out_event.SetMember( "extra_icon", out_event_property );
	out_event_property.SetUInt( 0 );
	out_event.SetMember( "mastery_icon", out_event_property );

	get_ui( )->movie->Invoke( "root.add_log_message", NULL, &out_event, 1 );
}

void game_world_ui::on_player_killed(
	u8		victim_id,
	u8		killer_id,
	bool	is_headshot,
	u32		item_dict_id
)
{
	network_client* const client = static_cast< network_client* >( m_game_world.get_game( ).get_network_client( ) );

	if ( client->is_player_current( victim_id ) )
	{
		get_ui( )->movie->Invoke( "root.reset_damage_indicator", NULL, NULL, 0 );
	}

	player_ptr killer = client->get_player( killer_id );
	player_ptr victim = client->get_player( victim_id );

	wchar_t const* victim_name = victim->get_profile_name( );
	wchar_t const* killer_name = killer->get_profile_name( );

	u8 const combat_log_icon = item_dict_id ? m_game_world.get_game( ).items_dictionary( ).item_by_id( item_dict_id ).combat_log_icon : 0;


	flash_value out_event; get_ui( )->movie->CreateObject( &out_event );


	flash_value out_event_property; get_ui( )->movie->CreateObject( &out_event_property );

	out_event_property.SetUInt( victim_id == killer_id ? 2 : 1 );
	out_event.SetMember( "action_id", out_event_property );

	out_event_property.SetStringW( killer_name );
	out_event.SetMember( "who_name", out_event_property );

	out_event_property.SetUInt( client->is_player_current( killer_id ) ? 2 : client->get_local_player( )->team( ) != killer->team( ) );
	out_event.SetMember( "who_team", out_event_property );

	out_event_property.SetStringW( victim_name );
	out_event.SetMember( "victim_name", out_event_property );

	out_event_property.SetUInt( client->is_player_current( victim_id ) ? 2 : client->get_local_player( )->team( ) != victim->team( ) );
	out_event.SetMember( "victim_team", out_event_property );

	out_event_property.SetUInt( combat_log_icon );
	out_event.SetMember( "object_icon", out_event_property );

	u8 extra_icon = 0;
	if ( victim_id == killer_id )
		extra_icon = 4;
	else if ( is_headshot )
		extra_icon = 1;

	out_event_property.SetUInt( extra_icon );
	out_event.SetMember( "extra_icon", out_event_property );

	out_event_property.SetUInt( 0 );
	out_event.SetMember( "mastery_icon", out_event_property );

	get_ui( )->movie->Invoke( "root.add_log_message", NULL, &out_event, 1 );
}

void game_world_ui::set_crosshair_size( float size )
{
	flash_value value;
	value.SetNumber( size );
	get_ui( )->movie->Invoke( "root.set_crosshair_size", NULL, &value, 1 );
}

void game_world_ui::on_enemy_hitted( )
{
	get_ui( )->movie->Invoke( "root.crosshair_enemy_hit", NULL, NULL, 0 );
}

void game_world_ui::show_crosshair( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );
	get_ui( )->movie->Invoke( "root.show_crosshair", NULL, &b_val, 1 );
}

void game_world_ui::show_ammo_indicator( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );
	get_ui( )->movie->Invoke( "root.show_ammo", NULL, &b_val, 1 );
}

void game_world_ui::show_capture_progress( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );
	get_ui( )->movie->Invoke( "root.show_capture_progress", NULL, &b_val, 1 );
}

void game_world_ui::set_fire_queue_size( const u32 fire_queue_size )
{
	flash_value value;
	value.SetUInt( fire_queue_size );
	get_ui( )->movie->Invoke( "root.set_weapon_fire_queue_size", NULL, &value, 1 );
}

void game_world_ui::set_ammo_in_magazine( const u32 count )
{
	flash_value value;
	value.SetUInt( count );
	get_ui( )->movie->Invoke( "root.set_weapon_ammo_size", NULL, &value, 1 );
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
	b_val.SetBoolean( b_show );
	get_ui( )->movie->Invoke( "root.show_player_list", NULL, &b_val, 1 );
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

// sushi@NOTE: Retail keeps the reusable GFx property's cleanup inside each object-loop
// latch; base emits two extra cleanup branches despite the target local/statement shape.
void game_world_ui::update_minimap_objects( )
{
	base_network_client* const client = m_game_world.get_game( ).get_network_client( );

	game_team_id local_player_team = team_1;
	for ( u8 i = 0; i < 20; ++i )
		if ( client->is_player_local( i ) )
		{
			local_player_team = client->match_options( ).player_profiles[i].team;
			break;
		}

	flash_value level_objects;
	get_ui( )->movie->CreateArray( &level_objects );
	flash_value level_object_val_prop;

	player_ptr current_player = client->get_current_player( );
	bool is_carrying_victory_item = current_player.c_ptr( ) && current_player->inventory( ).get_victory_item( ) != NULL;

	u8 bases_count = 0;
	for ( victory_items_container** it = m_game_world.get_project( )->m_victory_items_containers.begin( );
		it != m_game_world.get_project( )->m_victory_items_containers.end( );
		++it )
	{
		if ( (*it)->team( ) != local_player_team )
			continue;

		flash_value level_object_val;
		get_ui( )->movie->CreateObject( &level_object_val );

		float position_x = (*it)->get_transform( ).c.x;
		float position_y = -(*it)->get_transform( ).c.z;

		level_object_val_prop.SetUInt( (*it)->id( ) );
		level_object_val.SetMember( "id", level_object_val_prop );

		level_object_val_prop.SetString( is_carrying_victory_item ? "base_highlighted" : "base" );
		level_object_val.SetMember( "type", level_object_val_prop );

		level_object_val_prop.SetNumber( position_x );
		level_object_val.SetMember( "pos_x", level_object_val_prop );

		level_object_val_prop.SetNumber( position_y );
		level_object_val.SetMember( "pos_y", level_object_val_prop );

		level_objects.PushBack( level_object_val );
		++bases_count;
	}

	for ( victory_item_ptr* it = m_game_world.get_victory_items( ).begin( ); it != m_game_world.get_victory_items( ).end( ); ++it )
	{
		if ( (*it)->get_spotted_to_team( ) != local_player_team )
			continue;

		flash_value level_object_val;
		get_ui( )->movie->CreateObject( &level_object_val );

		float position_x = (*it)->get_transform( ).c.x;
		float position_y = -(*it)->get_transform( ).c.z;

		level_object_val_prop.SetUInt( (*it)->id + bases_count );
		level_object_val.SetMember( "id", level_object_val_prop );

		level_object_val_prop.SetString( "artifact" );
		level_object_val.SetMember( "type", level_object_val_prop );

		level_object_val_prop.SetNumber( position_x );
		level_object_val.SetMember( "pos_x", level_object_val_prop );

		level_object_val_prop.SetNumber( position_y );
		level_object_val.SetMember( "pos_y", level_object_val_prop );

		level_objects.PushBack( level_object_val );
	}

	get_ui( )->movie->Invoke( "root.update_objects", NULL, &level_objects, 1 );
}


void game_world_ui::initialize_minimap( )
{
	pcstr const project_name = m_game_world.get_project( )->project_name( );

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

// sushi@NOTE: Retail hoists the inventory victory-item read above both GFx object creations.
// Moving it earlier in source would contradict the target line records.
void game_world_ui::update_minimap_players( )
{
	base_network_client* client = m_game_world.get_game( ).get_network_client( );
	player_ptr local_player = client->get_current_player( );
	if ( !local_player )
		return;

	flash_value players_array;
	get_ui( )->movie->CreateArray( &players_array );

	u32 in_array_index = 0;
	for ( u8 i = 0; i < 20; ++i )
	{
		player_ptr current_player = client->get_player( i );
		if ( !current_player || !current_player->has_been_inserted( ) || current_player->team( ) != local_player->team( ) )
			continue;

		float position_x = current_player->get_current( ).transform.c.x;
		float position_y = -current_player->get_current( ).transform.c.z;

		bool is_carrying_item = current_player->inventory( ).get_victory_item( ) != NULL;

		flash_value player_descr_value;
		get_ui( )->movie->CreateObject( &player_descr_value );
		flash_value player_descr_value_property;
		get_ui( )->movie->CreateObject( &player_descr_value_property );

		player_descr_value_property.SetUInt( i );
		player_descr_value.SetMember( "player_id", player_descr_value_property );

		player_descr_value_property.SetNumber( position_x );
		player_descr_value.SetMember( "player_pos_x", player_descr_value_property );

		player_descr_value_property.SetNumber( position_y );
		player_descr_value.SetMember( "player_pos_y", player_descr_value_property );

		player_descr_value_property.SetNumber( current_player->get_current( ).transform.get_angles( math::rotation_zxy ).y );
		player_descr_value.SetMember( "player_rotation_in_rad", player_descr_value_property );

		player_descr_value_property.SetUInt( current_player->team( ) );
		player_descr_value.SetMember( "team", player_descr_value_property );

		player_descr_value_property.SetBoolean( is_carrying_item );
		player_descr_value.SetMember( "is_carrying_item", player_descr_value_property );

		players_array.SetElement( in_array_index, player_descr_value );
		++in_array_index;
	}

	get_ui( )->movie->Invoke( "root.update_players", NULL, &players_array, 1 );
}

// claude@NOTE: PDB locals and predicates match. Retail LTCG inlines the
// implicit player_ptr destructor; base calls its byte-exact COMDAT body.
// Reopen after resource_ptr/player compiler-context changes.
void game_world_ui::update_minimap_local_player( )
{
	player_ptr current_player = m_game_world.get_game( ).get_network_client( )->get_current_player( );
	if ( !current_player )
		return;
	if ( !current_player->has_been_inserted( ) )
		return;

	float4x4 const current_player_transform = current_player->get_current( ).transform;
	flash_value player_descr_value[4];

	player_descr_value[0].SetNumber( current_player_transform.c.x );
	player_descr_value[1].SetNumber( -current_player_transform.c.z );
	player_descr_value[2].SetNumber( current_player_transform.get_angles( math::rotation_zxy ).y );
	player_descr_value[3].SetUInt( current_player ? current_player->team( ) : team_neutral );

	get_ui( )->movie->Invoke( "root.update_local_player", NULL, player_descr_value, 4 );
}

void game_world_ui::reset_map_rotatable( )
{
	flash_value b_val;
	b_val.SetBoolean( is_ui_minimap_rotable );
	get_ui( )->movie->Invoke( "root.set_rotable", NULL, &b_val, 1 );
	is_ui_minimap_rotable_old = is_ui_minimap_rotable;
}

void game_world_ui::set_ammo_total_count( u32 first_type_count, u32 second_type_count )
{
	flash_value count;
	count.SetUInt( first_type_count );
	get_ui( )->movie->Invoke( "root.set_primary_ammo", NULL, &count, 1 );
	count.SetUInt( second_type_count );
	get_ui( )->movie->Invoke( "root.set_secondary_ammo", NULL, &count, 1 );
}

void game_world_ui::show_quick_slots( bool b_show )
{
	flash_value b_val;
	b_val.SetBoolean( b_show );
	get_ui( )->movie->Invoke( "root.show_slots", NULL, &b_val, 1 );
	if ( !b_show )
		m_slots_to_update.clear( );
}

void game_world_ui::create_slot_value(
	profile_slot_enum			slot,
	inventory_item_props&		item_props,
	flash_value&				slot_descr_value
)
{
	u8 const item_icon	= m_game_world.get_game( ).items_dictionary( ).item_by_id( item_props.m_dict_id ).item_cfg->get_root( )[ "ui_desc" ][ "icon" ];

	bool const enabled	= item_props.m_amount > 0;

	pcstr hotkey = "";
	game_action_id action = game_action_id( -1 );
	switch ( slot )
	{
		case quick_slot1:	action = kQUICK_USE_1;	break;
		case quick_slot2:	action = kQUICK_USE_2;	break;
		case quick_slot3:	action = kQUICK_USE_3;	break;
		case quick_slot4:	action = kQUICK_USE_4;	break;
		case quick_slot5:	action = kQUICK_USE_5;	break;
		case quick_slot6:	action = kQUICK_USE_6;	break;
		default:			action = game_action_id( -1 );
	}

	if ( action != game_action_id( -1 ) )
	{
		keyboard_key_descr const* const key = m_game_world.get_game( ).get_key_binder( ).dik_to_ptr( m_game_world.get_game( ).get_key_binder( ).get_action_dik( action, 0 ), false );
		hotkey = key ? key->key_name : 0;
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

// sushi@NOTE: Keep the counter loop: it matches retail through the slot-fill cone.
// The enum-iterator form diverges at loop entry.
void game_world_ui::fill_quick_slots( )
{
	flash_value slots_array;
	get_ui( )->movie->CreateArray( &slots_array );
	u32 in_array_index = 0;
	for ( u32 i = 0; i < 6; ++i )
	{
		flash_value slot_descr_value;
		get_ui( )->movie->CreateObject( &slot_descr_value );

		inventory_item_ptr item = m_game_world.get_game( ).get_network_client( )->get_current_player( )->inventory( ).item_in_slot( (profile_slot_enum)( quick_slot1 + i ) );
		if ( !item )
			continue;

		inventory_item_props current_item_props;
		item->get_item_props( current_item_props );

		dictionary_item dict_item = m_game_world.get_game( ).items_dictionary( ).item_by_id( item->get_dict_id( ) );

		create_slot_value( (profile_slot_enum)( quick_slot1 + i ), current_item_props, slot_descr_value );
		slots_array.SetElement( in_array_index, slot_descr_value );
		++in_array_index;
	}

	get_ui( )->movie->Invoke( "root.fill_slots", NULL, &slots_array, 1 );

	inventory_item_ptr item_in_back_slot = m_game_world.get_game( ).get_network_client( )->get_current_player( )->inventory( ).item_in_slot( back_slot );
	if ( item_in_back_slot )
	{
		inventory_item_props item_in_back_slot_props;
		item_in_back_slot->get_item_props( item_in_back_slot_props );

		dictionary_item dict_item = m_game_world.get_game( ).items_dictionary( ).item_by_id( item_in_back_slot->get_dict_id( ) );

		if ( dict_item.item_category == 4 )
		{
			flash_value b_val;
			b_val.SetBoolean( true );
			get_ui( )->movie->Invoke( "root.show_oxygen", NULL, &b_val, 1 );

			flash_value oxygene_props[2];
			oxygene_props[0].SetUInt( item_in_back_slot_props.use_in_percents );
			oxygene_props[1].SetUInt( item_in_back_slot_props.m_amount_ms );
			get_ui( )->movie->Invoke( "root.set_oxygen", NULL, oxygene_props, 2 );
		}
	}
}

void game_world_ui::update_quick_slot( profile_slot_enum slot )
{
	flash_value slot_descr_value[2];
	get_ui( )->movie->CreateObject( &slot_descr_value[0] );

	inventory_item_ptr item = m_game_world.get_game( ).get_network_client( )->get_current_player( )->inventory( ).item_in_slot( slot );

	inventory_item_props current_item_props;
	if ( !item->get_item_props( current_item_props ) )
		disactivate_quick_slot( slot );

	if ( slot == back_slot )
	{
		dictionary_item dict_item = m_game_world.get_game( ).items_dictionary( ).item_by_id( item->get_dict_id( ) );
		if ( dict_item.item_category == 4 )
		{
			flash_value oxygene_props[2];
			oxygene_props[0].SetUInt( current_item_props.use_in_percents );
			oxygene_props[1].SetUInt( current_item_props.m_amount_ms );
			get_ui( )->movie->Invoke( "root.set_oxygen", NULL, oxygene_props, 2 );
		}
	}
	else
	{
		create_slot_value( slot, current_item_props, slot_descr_value[0] );
		slot_descr_value[1].SetUInt( slot - quick_slot1 );
		get_ui( )->movie->Invoke( "root.fill_slot", NULL, slot_descr_value, 2 );
	}
}

// claude@NOTE: Source locals and operations match. Retail callers push this
// and pass message_id in edx, separating the inlined flash_value constructor;
// base caller context keeps this in esi. Reopen after player/weapon callers.
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
	message_val[1].SetInt( progress_value );
	message_val[2].SetInt( 20 );
	get_ui( )->movie->Invoke( "root.set_context", NULL, message_val, 3 );
}

void game_world_ui::set_using_info_message( pcstr str )
{
	wchar_t message[512];
	m_game_world.get_game( ).text_translator( ).translate_text( str, message );

	flash_value message_val[3];
	message_val[0].SetStringW( message );
	message_val[1].SetInt( 0 );
	message_val[2].SetInt( 1000 );
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
	message_val[1].SetInt( 1000 );
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
	VOSTOK_UNREFERENCED_PARAMETER( visual_id );

	flash_value visual_id_val;
	visual_id_val.SetUInt( 0 );
	get_ui( )->movie->Invoke( "root.show_container_icon", NULL, &visual_id_val, 1 );
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
