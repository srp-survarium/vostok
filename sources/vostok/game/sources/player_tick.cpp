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

namespace survarium {

// claude@NOTE: backing storage for the smooth() config commands. The cc_float
// command objects themselves (and the dynamic init / atexit pairing of the first
// TU-statics block) are PARKED: the dynamic-init asm stores the s_console_command_root
// link, the backing-var pointer, default/min/max and the vtable, but NEVER a
// command-NAME string pointer, so the pcstr name arg to each cc_float/cc_bool ctor is
// not recoverable - declaring the objects would fabricate a .data string (forbidden), so
// they stay unpaired (same convention as game.cpp's s_draw_snd_stats/s_show_profiler).
// Recovered values: smooth_linear_speed min 0 max 10.f serializable engine_internal;
// smooth_angular_speed/smooth_pitch_speed min 0 max 720.f.
static float s_smooth_linear_speed;
static float s_smooth_angular_speed;
static float s_smooth_pitch_speed;

// claude@NOTE: backing storage for tick's cc_bool / cc_float config commands - PARKED for
// the same unrecoverable-name reason as the s_smooth_* block above. Recovered: the random
// input cc_bools default false, serializable true; the player_name font cc_floats feed the
// camera-name distance/font clamp in tick().
static bool s_is_local_player_random_input_enabled;
static bool s_is_test_players_random_input_enabled;
static float s_player_name_min_font_size;
static float s_player_name_max_font_size;
static float s_player_name_decrease_koef;

// TU statics (compiler-generated dynamic initializers / atexit destructors). PARKED: the
// command-NAME string each cc_* ctor takes is absent from the dynamic-init asm (only the
// root link / backing-var ptr / default / vtable are stored), so they cannot be declared
// without fabricating a .data string.
/*
// STATE[STUB]
void `dynamic initializer for 's_net_max_position_discrepancy_command''( )
{
	// FUNCTION BODY[0x7d8620]
	// <0x7d8620>|0x000|      :'31'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_test_players_random_input_cc''( )
{
	// FUNCTION BODY[0x7d8690]
	// <0x7d8690>|0x000|      :'35'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_local_player_random_input_cc''( )
{
	// FUNCTION BODY[0x7d86e0]
	// <0x7d86e0>|0x000|      :'38'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_print_animations_cc''( )
{
	// FUNCTION BODY[0x7d8730]
	// <0x7d8730>|0x000|      :'42'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_sf_animation_states_cc''( )
{
	// FUNCTION BODY[0x7d8780]
	// <0x7d8780>|0x000|      :'45'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_smooth_linear_speed_command''( )
{
	// FUNCTION BODY[0x7d87d0]
	// <0x7d87d0>|0x000|      :'48'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_smooth_angular_speed_command''( )
{
	// FUNCTION BODY[0x7d8830]
	// <0x7d8830>|0x000|      :'51'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_smooth_pitch_speed_command''( )
{
	// FUNCTION BODY[0x7d8890]
	// <0x7d8890>|0x000|      :'54'	{
	// ******
}
*/

u32 player::history_lower_bound_index( const u32 time_in_ms ) const
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
{
	m_history.pop_tail( );
}

void player::remove_oldest_history_items( const u32 new_oldest_time_in_ms )
{
	while( !m_history.empty( ) && m_history.oldest( ).time_in_ms < new_oldest_time_in_ms )
		m_history.pop_tail( );
}

void player::restore_history_item( client_player_history_item& item )
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

// claude@NOTE: byte residual is the inlined quaternion-product schedule (the two
// math::operator*(quaternion,quaternion) chains) + the physics get_transform/from_bullet
// tail (cross-module COMDAT). Structure is faithful: target_rotation = product of three
// get_angles_xyz quaternions, create_matrix into m_target.transform with the
// translation-preserving c.xyz() blend, look_pitch copy, walk-vector push, then the
// physics controller update_action + get_transform roundtrip back into the item/transform.
void player::update_history_item_from_previous(
	client_player_history_item const&		previous_item,
	client_player_history_item&				item_to_update,
	float4x4&								previous_transform
)
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
{
	u32 index = from_index;
	while( index != m_history.head( ) )
	{
		update_history_item_from_previous( m_history[ m_history.previous( index ) ], m_history[ index ], previous_transform );
		inventory( ).action( (profile_slot_enum)m_history[ index ].action.weapon_state.slot_id, true );	index = m_history.next( index );
	}

	m_target.animation_player.set_object_transform( m_history[ m_history.previous( m_history.head( ) ) ].action.state.transform, this );
}

// claude@NOTE: Reconstructed and PAIRED (~72%), capped below a full structure match by
// replay_history's inner update_history_item_from_previous / set_object_transform calls
// dropping (those callees are still empty STUBs, so they inline to nothing - see
// replay_history note). The tick/restore/clamp/min/lower-bound/replay control flow and the
// transform-translation-preserving tail all match (process_quick_slots_for_proxy_player is
// now bodied, so the proxy path no longer DCE-collapses). Residual also includes the
// dropped unused bool& __formal arg to update_history_item (LTCG arg-drop). Next step:
// body update_history_item_from_previous (parked on the quaternion-product wall).
void player::time_warp( server_player_update const& action, u32 time_in_ms )
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

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
// STATE[STUB]
void `dynamic initializer for 'cc_player_name_min_font_size''( )
{
	// FUNCTION BODY[0x7d88f0]
	// <0x7d88f0>|0x000|      :'538'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 'cc_player_name_max_font_size''( )
{
	// FUNCTION BODY[0x7d8960]
	// <0x7d8960>|0x000|      :'539'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_net_max_position_discrepancy_command''( )
{
	// FUNCTION BODY[0x7f0330]
	// <0x7d89d0>|0x000|      :'540'	{
	// ******
}
*/

void player::smooth( const float time_delta )
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
			result = math::create_matrix( math::slerp_optimized( left_rotation, right_rotation, math::min( ( s_smooth_angular_speed / 180.f * math::pi ) * time_delta, 0.f ) ), float3( 0.f, 0.f, 0.f ) );
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

void player::tick( const u32 current_time_in_ms )
{
	if( m_is_first_tick )
	{
		m_is_first_tick = false;
		m_current_time_in_ms = current_time_in_ms;
	}

	const u32 previous_time_in_ms = is_local ? m_current_time_in_ms
		: ( m_history.empty( ) ? m_current_time_in_ms : math::max( m_current_time_in_ms, m_history.newest( ).time_in_ms ) );
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
