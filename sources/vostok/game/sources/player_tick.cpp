////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player.h"
#include "game.h"
#include "base_game_scene.h"
#include "base_network_client.h"
#include "game_world.h"
#include "camera_director.h"
#include "game_camera.h"
#include "game_world_ui.h"
#include "player_input_handler.h"
#include "weapon.h"
#include <vostok/game_core/server_player_update.h>
#include <vostok/game_core/player_state.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/physics/character_controller.h>
#include <vostok/console_command.h>

static float s_net_max_position_discrepancy = 0.1f;
#line 31
static vostok::console_commands::cc_float s_net_max_position_discrepancy_command( "net_max_position_discrepancy", s_net_max_position_discrepancy, 1e-7f, 0.1f, true, vostok::console_commands::command_type_engine_internal );

static bool s_is_test_players_random_input_enabled;
#line 35
static vostok::console_commands::cc_bool s_test_players_random_input_cc( "test_players_random_input", s_is_test_players_random_input_enabled, false, vostok::console_commands::command_type_user_specific );

static bool s_is_local_player_random_input_enabled;
#line 38
static vostok::console_commands::cc_bool s_local_player_random_input_cc( "local_player_random_input", s_is_local_player_random_input_enabled, false, vostok::console_commands::command_type_user_specific );

static bool s_print_animations_value;
#line 42
static vostok::console_commands::cc_bool s_print_animations_cc( "print_animations", s_print_animations_value, true, vostok::console_commands::command_type_user_specific );

static bool s_sf_animation_states_value;
#line 45
static vostok::console_commands::cc_bool s_sf_animation_states_cc( "sf_animation_states", s_sf_animation_states_value, true, vostok::console_commands::command_type_user_specific );

static float s_smooth_linear_speed = 3.f;
#line 48
static vostok::console_commands::cc_float s_smooth_linear_speed_command( "smooth_linear_speed", s_smooth_linear_speed, 0.f, 10.f, true, vostok::console_commands::command_type_engine_internal );

static float s_smooth_angular_speed = 180.f;
#line 51
static vostok::console_commands::cc_float s_smooth_angular_speed_command( "smooth_angular_speed", s_smooth_angular_speed, 0.f, 720.f, true, vostok::console_commands::command_type_engine_internal );

static float s_smooth_pitch_speed = 11.25f;
#line 54
static vostok::console_commands::cc_float s_smooth_pitch_speed_command( "smooth_pitch_speed", s_smooth_pitch_speed, 0.f, 720.f, true, vostok::console_commands::command_type_engine_internal );

static float s_player_name_min_font_size = 12.f;
#line 538
static vostok::console_commands::cc_float cc_player_name_min_font_size( "player_name_min_font_size", s_player_name_min_font_size, 1.f, 100.f, true, vostok::console_commands::command_type_engine_internal );

static float s_player_name_max_font_size = 32.f;
#line 539
static vostok::console_commands::cc_float cc_player_name_max_font_size( "player_name_max_font_size", s_player_name_max_font_size, 1.f, 100.f, true, vostok::console_commands::command_type_engine_internal );

static float s_player_name_decrease_koef = 0.3f;
#line 540
static vostok::console_commands::cc_float cc_player_name_decrease_koef( "player_name_decrease_koef", s_player_name_decrease_koef, 0.f, 1.f, true, vostok::console_commands::command_type_engine_internal );

namespace survarium {

u32 player::history_lower_bound_index( const u32 time_in_ms ) const
#line 66
{
	u32 index = m_history.head( );
	while( index != m_history.tail( ) )
	{
		index = m_history.previous( index );
		if( m_history[ index ].time_in_ms <= time_in_ms )
			return index;
	}

	return (u32)-1;
}

void player::remove_oldest_history_item( )
#line 79
{
	m_history.pop_tail( );
}

void player::remove_oldest_history_items( const u32 new_oldest_time_in_ms )
#line 84
{
	while( !m_history.empty( ) && m_history.oldest( ).time_in_ms < new_oldest_time_in_ms )
		m_history.pop_tail( );
}

void player::restore_history_item( client_player_history_item& item )
#line 98
{
	m_target.transform		= item.action.state.transform;
	m_target.look_pitch		= item.action.state.look_pitch;
	m_target.physics_controller->set_transform( m_target.transform );
}

void player::update_history_item(
	client_player_history_item&		item,
	client_player_history_item const* const	next_item,
	server_player_update const&		server_action,
	const u32						server_action_time_in_ms,
	float4x4&						previous_transform,
	bool&							__formal
)
#line 120
{
	restore_history_item( item );

	const u32 time_delta = server_action_time_in_ms - item.time_in_ms;
	previous_transform = math::lerp(
		item.action.state.transform,
		next_item->action.state.transform,
		float( time_delta ) / float( next_item->time_in_ms - item.time_in_ms )
	);

	item.action = server_action;

	m_target.transform		= server_action.state.transform;
	m_target.look_pitch		= server_action.state.look_pitch;
	m_target.physics_controller->set_transform( m_target.transform );

	if( item.time_in_ms != server_action_time_in_ms )
		item.time_in_ms = server_action_time_in_ms;
}

// claude@NOTE: target and base share the three-block flow. The remaining line
// attribution and byte differences are the two inlined quaternion products plus
// the physics get_transform/from_bullet tail; the full transform update is present.
void player::update_history_item_from_previous(
	client_player_history_item const&		previous_item,
	client_player_history_item&				item_to_update,
	float4x4&								previous_transform
)
#line 185
{
	math::quaternion target_rotation =
		math::quaternion( previous_transform.get_angles_xyz( ) )
		* math::quaternion( item_to_update.action.state.transform.get_angles_xyz( ) )
		* math::quaternion( previous_item.action.state.transform.get_angles_xyz( ) );

	const float3 position =
		previous_item.action.state.transform.c.xyz( )
		+ ( item_to_update.action.state.transform.c.xyz( ) - previous_transform.c.xyz( ) );

	previous_transform = item_to_update.action.state.transform;

	m_target.transform = math::create_matrix( target_rotation, float3( 0.f, 0.f, 0.f ) );
	m_target.transform.c.xyz( ) = position;
	m_target.look_pitch = item_to_update.action.state.look_pitch;
	set_physics_controller_walk_vector( m_target );

	m_target.physics_controller->update_action( item_to_update.time_in_ms - previous_item.time_in_ms );
	previous_transform = item_to_update.action.state.transform = m_target.physics_controller->get_transform( );
}

// claude@NOTE: STRUCTURE MATCH (target 4 stmts == base 4). Ring walk from_index->head,
// update_history_item_from_previous + inventory().action per item, then
// m_target.animation_player.set_object_transform. The loop line-table fusion is now matched:
// the target fuses inventory().action + index=next into one statement (line 276), so they are
// written on one source line here too (was 5 stmts -> 4). Byte residual (~69%, not structure):
// our LTCG out-lines inventory() to a `call` where the target inlines the [this+8] deref, and
// the target tail-duplicates the post-loop set_object_transform across the empty-loop edge.
void player::replay_history( const u32 from_index, float4x4& previous_transform )
#line 264
{
	u32 index = from_index;
	while( index != m_history.head( ) )
	{
		update_history_item_from_previous( m_history[ m_history.previous( index ) ], m_history[ index ], previous_transform );
		inventory( ).action( (profile_slot_enum)m_history[ index ].action.weapon_state.slot_id, true );	index = m_history.next( index );
	}

	m_target.animation_player.set_object_transform( m_history[ m_history.previous( m_history.head( ) ) ].action.state.transform, this );
}

// claude@NOTE: target and base share all fourteen CFG blocks and the complete
// clamp/lower-bound/update/replay path. Residual statement splits come from ring-index
// register selection, replay_history's call schedule, and the physics transform tail.
void player::time_warp( server_player_update const& action, u32 time_in_ms )
#line 284
{
	if( m_last_server_correction_time && time_in_ms < m_last_server_correction_time )
		return;

	if( m_history.empty( ) )
		return;

	if( !is_local )
	{
		time_in_ms = math::min( time_in_ms, m_current_time_in_ms );
		m_history[ m_history.previous( m_history.head( ) ) ].time_in_ms = time_in_ms;

		m_history[ m_history.previous( m_history.head( ) ) ].action = action;

		m_target.transform		= action.state.transform;
		m_target.look_pitch		= action.state.look_pitch;
		m_target.physics_controller->set_transform( m_target.transform );

		process_quick_slots_for_proxy_player( );

		m_last_server_correction_time = time_in_ms;
		return;
	}

	if( time_in_ms > m_history[ m_history.previous( m_history.head( ) ) ].time_in_ms )
		return;

	if( time_in_ms + 1000 < m_history[ m_history.previous( m_history.tail( ) ) ].time_in_ms )
		return;

	time_in_ms = math::min( time_in_ms, m_current_time_in_ms );

	const u32 lower_bound_index = history_lower_bound_index( time_in_ms );
	if( lower_bound_index == (u32)-1 )
		return;

	if( m_history.next( lower_bound_index ) == m_history.head( ) )
		return;

	float4x4 transform = m_target.transform;

	m_is_replaying_history = true;

	remove_oldest_history_items( m_history[ lower_bound_index ].time_in_ms );

	float4x4 previous_transform;
	update_history_item(
		m_history[ lower_bound_index ],
		is_local ? &m_history[ m_history.next( lower_bound_index ) ] : NULL,
		action,
		time_in_ms,
		previous_transform,
		m_is_replaying_history
	);

	replay_history( m_history.next( m_history.tail( ) ), previous_transform );

	const float3 position = m_target.transform.c.xyz( );
	m_target.transform = transform;
	m_target.transform.c.xyz( ) = position;
	m_target.physics_controller->set_transform( m_target.transform );

	m_is_replaying_history = false;
	m_last_server_correction_time = time_in_ms;
}

// claude@NOTE: Master Gold strips the LOG body to a bare `ret` (target size 1).
// The carcass shows ~11 statements at lines 415-427 (a LOG_* of the active object,
// gated header param) that all compile out; reproduced as the unreferenced-param
// eater so the base also emits just `ret`.
void player::log_active_object( pcstr const header ) const
#line 415
{
	VOSTOK_UNREFERENCED_PARAMETER( header );
}

// claude@NOTE: byte residual is the inlined math::operator*(quaternion,quaternion) product
// schedule (L515) + the create_matrix/mul4x3 inline tail (L528) + the LTCG custom call
// convention into the apply_input overload. Structure is faithful: previous_rotation from
// the animated/character transform, a per-frame angular-increment quaternion, the product
// driving get_axis_and_angle, a tree resync, the is_local apply_input path, and the
// angle-gated rotation re-application into the tree.
void player::apply_input_before_new_transform(
	client_player_state&	player_state,
	player_input const&		previous_input,
	const float				time_delta
)
#line 511
{
	math::quaternion previous_rotation(
		player_state.animation_player.are_there_any_animations( )
			? player_state.animation_player.get_object_transform( this )
			: player_state.transform );

	math::quaternion new_rotation( float3(
		0.f,
		( m_input.angular_acceleration.x * time_delta * 0.5f + previous_input.angular_velocity.x ) * time_delta,
		( m_input.angular_acceleration.y * time_delta * 0.5f + previous_input.angular_velocity.y ) * time_delta
	) );

	new_rotation = previous_rotation * new_rotation;

	float3 axe;
	float angle;
	new_rotation.get_axis_and_angle( axe, angle );

	player_state.animation_player.set_object_transform( player_state.transform, this );

	if( is_local )
		apply_input( player_state, previous_input.angular_velocity, m_input.angular_acceleration, time_delta );

	if( math::abs( angle ) >= math::epsilon_3 )
	{
		float4x4 new_transform = math::mul4x3(
			math::create_matrix( new_rotation, float3( 0.f, 0.f, 0.f ) ),
			player_state.transform );
		player_state.transform.c.xyz( ) = new_transform.c.xyz( );
		player_state.animation_player.set_object_transform( new_transform, this );
	}
}

void player::smooth( const float time_delta )
#line 543
{
	if( time_delta > 0.f )
	{
		float4x4 result;

		float3 const& current_angles = m_current.transform.get_angles_xyz( );
		float3 const& target_angles = m_target.transform.get_angles_xyz( );
		if( current_angles != target_angles )
		{
			math::quaternion left_rotation( current_angles );
			math::quaternion right_rotation( target_angles );
			result = math::create_matrix( ::slerp_optimized( left_rotation, right_rotation, math::min( ( s_smooth_angular_speed / 180.f * math::pi ) * time_delta, 0.f ) ), float3( 0.f, 0.f, 0.f ) );
		}
		else
			result = m_current.transform;

		if( m_current.transform.c.xyz( ) != m_target.transform.c.xyz( ) )
		{
			const float linear_factor = math::min( s_smooth_linear_speed * time_delta, 0.f );
			result.c.xyz( ) = ( m_target.transform.c.xyz( ) - m_current.transform.c.xyz( ) ) * linear_factor + m_current.transform.c.xyz( );
		}
		else
			result.c.xyz( ) = m_current.transform.c.xyz( );

		m_current.transform = result;
		m_current.look_pitch += math::min( s_smooth_pitch_speed * time_delta, 0.f ) * ( m_target.look_pitch - m_current.look_pitch );
	}
}

// claude@NOTE: std::max reproduces the target CFG through B63. The first remaining
// skeleton divergence is animation-owned animation_player::tick: target inlines
// skip_time_if_needed/n_ary_tree::tick/callback compaction, while base calls it.
void player::tick( const u32 current_time_in_ms )
#line 589
{
	if( m_is_first_tick )
	{
		m_is_first_tick = false;
		m_current_time_in_ms = current_time_in_ms;
	}

	const u32 previous_time_in_ms = is_local ? m_current_time_in_ms
		: ( m_history.empty( ) ? m_current_time_in_ms : std::max( m_current_time_in_ms, m_history.newest( ).time_in_ms ) );
	m_current_time_in_ms = current_time_in_ms;
	const u32 time_delta_in_ms = current_time_in_ms > previous_time_in_ms ? current_time_in_ms - previous_time_in_ms : 0;

	m_target.update_transform( );
	if( m_use_physics_controller_for_current )
		m_current.update_transform( );

	if( m_use_physics_controller_for_current && m_target.physics_controller->has_updates( )
		&& ( m_current.transform.c.xyz( ) - m_target.transform.c.xyz( ) ).length( ) > 1.f )
	{
		m_current.transform = m_target.transform;
		m_current.physics_controller->set_transform( m_current.transform );
	}

	if( m_target.physics_controller->has_updates( ) || m_current.physics_controller->has_updates( ) )
		smooth( time_delta_in_ms * 0.001f );

	const bool is_current = m_game.network_client( ).is_player_current( id );

	player_input previous_input = m_input;
	m_input = m_is_alive ? ( is_local ? local_input( ) : remote_input( ) ) : player_input( );

	if( is_local )
	{
		if( ( s_is_local_player_random_input_enabled && is_current ) || ( s_is_test_players_random_input_enabled && !is_current ) )
		{
			static math::random32 random;
			m_input.actions_mask = random.random( u32( -1 ) );
		}
	}

	if( m_is_alive )
		m_stamina.tick( current_time_in_ms, m_stamina.can_be_spent( ) && m_current_active_object->is_sprinting( ) );

	if( is_current )
	{
		process_quick_slots_for_current_player( );
		if( m_game_ui )
			m_game_ui->set_health( damage_model( )->get_total_health( ) );
		if( weapon* const w = static_cast< weapon* >( m_current_active_object->cast_weapon_core( ) ) )
			w->update_dispersion_visual_representation( );
	}

	u32 time_in_ms = previous_time_in_ms;
	bool are_there_any_callbacks = false;
	do
	{
		if( time_in_ms != previous_time_in_ms || m_input.actions_mask != previous_input.actions_mask || m_force_animation_selection )
		{
			m_force_animation_selection = false;
			select_animations( time_in_ms );
		}

		if( m_force_bones_recompute )
		{
			m_force_bones_recompute = false;
			compute_bones( time_in_ms );
		}

		if( m_current_active_object && m_current_active_object != m_empty_hands )
			m_current.animation_player.set_object_transform( float4x4( ).identity( ), m_current_active_object.c_ptr( ) );
		m_current.animation_player.set_object_transform( m_current.transform, this );
		m_target.animation_player.set_object_transform( m_target.transform, this );

		are_there_any_callbacks = m_target.animation_player.tick_to_nearest_user_handled_callback( current_time_in_ms );
		time_in_ms = m_current.animation_player.last_tick_time_in_ms( );

		const float time_delta = ( time_in_ms - previous_time_in_ms ) * 0.001f;
		apply_input_before_new_transform( m_current, previous_input, time_delta );
		m_target.animation_player.tick( time_in_ms );
		apply_input_before_new_transform( m_target, previous_input, time_delta );

		float2 const& angular_velocity = previous_input.angular_velocity + m_input.angular_acceleration * time_delta;
		previous_input = m_input;
		previous_input.angular_velocity = angular_velocity;
	}
	while( time_in_ms != current_time_in_ms );

	if( are_there_any_callbacks )
		select_animations( current_time_in_ms );

	if( ( is_local && m_is_alive ) || !m_history.empty( ) )
		serialize_current_state( current_time_in_ms );

	if( is_local && m_is_alive )
		m_game_scene.get_game( ).network_client( ).send_local_player_input( m_input, current_time_in_ms, m_target.transform, m_target.look_pitch );

	set_physics_controller_walk_vector( m_target );
	if( m_use_physics_controller_for_current )
		set_physics_controller_walk_vector( m_current );

	notify_actions_subscribers( );
	render( current_time_in_ms, current_time_in_ms );

	if( m_is_alive )
		damage_model( )->tick( time_delta_in_ms, current_time_in_ms );

	if( m_game_ui && is_current && m_is_alive )
	{
		update_speed_info( );
		detect_usable_objects( current_time_in_ms );
		if( usable_object_user_data( )->current_object && usable_object_user_data( )->current_progress != u32( -1 ) )
			m_game_ui->set_using_progress_message( usable_object_user_data( )->current_progress );
	}

	bool name_visible = !m_is_demo_player && ( !is_current || m_local_input_controller->input_mode( ) != first_person_mode );
	if( name_visible && m_game.network_client( ).current_player_team( ) == m_team_id && m_game.get_game_world( ).get_camera_director( ).get_active_camera( ) )
	{
		float2 screen_p( math::SNaN, math::SNaN );
		if( m_game_scene.point_to_screen( float3( m_character_head_transform.c.x, m_character_head_transform.c.y + 0.2f, m_character_head_transform.c.z ), screen_p ) )
		{
			const float near_plane = m_game.get_game_world( ).get_camera_director( ).get_active_camera( )->get_near_plane( );
			const float font_size = math::max(
				s_player_name_max_font_size -
					( ( m_character_head_transform.c.xyz( ) - m_game.get_game_world( ).get_camera_director( ).get_inverted_view_matrix( ).c.xyz( ) ).length( ) - near_plane ) /
					( m_game.get_game_world( ).get_camera_director( ).get_active_camera( )->get_far_plane( ) - near_plane ) * 1000.f * s_player_name_decrease_koef,
				s_player_name_min_font_size );
			if( !m_text.visible )
				m_text.set_visible( true );
			m_text.set_font_size( font_size );
			const float width = m_text.get_width( );
			float2 p( screen_p.x - width * 0.5f, screen_p.y - m_text.get_height( ) );
			m_text.set_position( p.x, p.y );
			return;
		}
	}

	if( m_text.visible )
		m_text.set_visible( false );
}

} // namespace survarium
#line 0
