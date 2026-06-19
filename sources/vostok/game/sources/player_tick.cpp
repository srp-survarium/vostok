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

// claude@NOTE: PARKED. Reconstructs an interpolated history item from server_action
// (transform/look_pitch blend between item and next_item over the action time delta),
// writing item.action.state and previous_transform. Math-heavy (transform blend +
// quaternion interpolation) over client_player_state internals; not yet reconstructed.
// Next step: decode the per-statement blend; restore_history_item / replay_history are
// the matched siblings that establish the m_target/state access idiom.
// STATE[STUB]
void player::update_history_item(
	client_player_history_item&		item,
	client_player_history_item const* const	next_item,
	server_player_update const&		server_action,
	const u32						server_action_time_in_ms,
	float4x4&						previous_transform,
	bool&							__formal
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( item, next_item, server_action, server_action_time_in_ms, previous_transform, __formal );
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

// claude@NOTE: PARKED. Walks the history ring from from_index to head, calling
// update_history_item_from_previous (PARKED) + inventory::action per item, then
// set_object_transform on the animation n_ary_tree. Structure is decoded (the ring
// walk uses the same previous()/% idiom as the matched history helpers) but it depends
// on update_history_item_from_previous and the cross-module inventory::action /
// animation::mixing::n_ary_tree::set_object_transform calls. Next step: match
// update_history_item_from_previous, then wire the walk.
// STATE[STUB]
void player::replay_history( const u32 from_index, float4x4& previous_transform )
{
	VOSTOK_UNREFERENCED_PARAMETERS( from_index, previous_transform );
}

// claude@NOTE: PARKED. Public entry: clamps server_action time, locates the history
// lower-bound (history_lower_bound_index, MATCHED), restores that item
// (restore_history_item, MATCHED) and replays forward (replay_history, PARKED) into
// previous_transform/transform. 127-statement body with 3 locals; capped by the parked
// replay_history / update_history_item_from_previous it drives. Next step: match those
// helpers, then assemble the clamp + lower-bound + restore + replay sequence.
// STATE[STUB]
void player::time_warp( server_player_update const& action, u32 time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS( action, time_in_ms );
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
