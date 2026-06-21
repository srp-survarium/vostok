////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player.h"
#include <vostok/game_core/server_player_update.h>
#include <vostok/game_core/player_state.h>
#include <vostok/physics/character_controller.h>

namespace survarium {

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
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

// claude@NOTE: PARKED. Quaternion-interpolation reconstruction of item_to_update's
// transform from previous_item (target_rotation local is a math::quaternion built from
// the blended angles, then create_matrix). 75-statement math body over the history
// item / server_player_update state; not yet reconstructed. Called by replay_history
// (matched). Next step: decode the slerp/create_matrix blend statement-by-statement.
// STATE[STUB]
void player::update_history_item_from_previous(
	client_player_history_item const&		previous_item,
	client_player_history_item&				item_to_update,
	float4x4&								previous_transform
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( previous_item, item_to_update, previous_transform );
}

// claude@NOTE: PARKED on a stub-inlining wall. Structure is reconstructed (ring walk
// from_index->head, update_history_item_from_previous + inventory().action per item,
// then m_target.animation_player.set_object_transform). But the
// update_history_item_from_previous CALL and the final set_object_transform statement
// both vanish at /Od because their callees are still empty STUBs (an empty body inlines
// to nothing): update_history_item_from_previous is the parked 15-stmt quaternion-blend,
// and m_target.animation_player.set_object_transform forwards into a stub tree method.
// Next step: give update_history_item_from_previous a real body, then re-score - the two
// dropped statements should reappear. (inventory() also out-lines here vs the target's
// inlined [this+8] deref - a secondary /Od inlining diff to revisit after.)
// STATE[STUB]
void player::replay_history( const u32 from_index, float4x4& previous_transform )
{
	u32 index = from_index;
	while( index != m_history.head( ) )
	{
		update_history_item_from_previous( m_history[ m_history.previous( index ) ], m_history[ index ], previous_transform );
		inventory( ).action( (profile_slot_enum)m_history[ index ].action.weapon_state.slot_id, true );
		index = m_history.next( index );
	}

	m_target.animation_player.set_object_transform( m_history[ m_history.previous( m_history.head( ) ) ].action.state.transform, this );
}

// claude@NOTE: Reconstructed and PAIRED, but capped below a full structure match by
// two stub-inlining walls in sibling units: player::process_quick_slots_for_proxy_player
// (player.cpp) is still an empty STUB, so its call in the !is_local proxy path inlines to
// nothing; and replay_history's inner update_history_item_from_previous / set_object_transform
// calls drop for the same reason (see replay_history note). The tick/restore/clamp/min/
// lower-bound/replay control flow and the transform-translation-preserving tail all match.
// Residual also includes the dropped unused bool& __formal arg to update_history_item
// (LTCG arg-drop). Next step: give process_quick_slots_for_proxy_player +
// update_history_item_from_previous real bodies, then re-score.
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

// claude@NOTE: PARKED. Applies previous_input angular velocity to player_state's
// transform: builds previous_rotation (quaternion), an angle, a new_rotation around
// axe (float3), composes new_transform. 18-statement quaternion/float4x4 math over
// client_player_state. Next step: decode the rotation compose statement-by-statement
// (the matched smooth/restore siblings establish the math::quaternion/create_matrix idiom).
// STATE[STUB]
void player::apply_input_before_new_transform(
	client_player_state&	player_state,
	player_input const&		previous_input,
	const float				time_delta
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( player_state, previous_input, time_delta );
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

// claude@NOTE: PARKED. Slerps m_target toward m_current over time_delta: builds
// left/right_rotation quaternions from each transform's angles, slerp_optimized
// between them gated by per-axis equality, and a linear blend of position into
// result, written via create_matrix. 15-statement quaternion/matrix body with its
// own TU statics (s_smooth_linear/angular/pitch_speed config). Next step: reconstruct
// the slerp + create_matrix sequence (callees: get_angles_xyz, slerp_optimized,
// create_matrix - all decoded in the asm).
// STATE[STUB]
void player::smooth( const float time_delta )
{
	VOSTOK_UNREFERENCED_PARAMETER( time_delta );
}

// claude@NOTE: PARKED. The 265-statement per-frame tick: name-visibility/font sizing,
// input integration, time_warp / serialize cone, weapon_core + damage_model queries,
// and the SF animation-state plumbing. Depends on the parked time_warp + many player.cpp
// cone members and cross-module callees (weapon_core, damage_model_ptr). Matched LAST
// alongside player.cpp's tick cone. Next step: after player.cpp + time_warp + the input
// helpers land, assemble the frame sequence.
// STATE[STUB]
void player::tick( const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETER( current_time_in_ms );
}

} // namespace survarium
