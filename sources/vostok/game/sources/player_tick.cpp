////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player.h"
#include <vostok/game_core/server_player_update.h>
#include <vostok/game_core/player_state.h>
#include <vostok/physics/character_controller.h>

namespace survarium {

// claude@NOTE: backing storage for the smooth() config commands. The cc_float
// command objects themselves (and the dynamic init / atexit pairing of the first
// TU-statics block) are a separate config-machinery unit; recovered values are:
//   smooth_linear_speed : min 0, max 10.f, serializable, engine_internal
//   smooth_angular_speed: min 0, max 720.f
//   smooth_pitch_speed  : min 0, max 720.f
static float s_smooth_linear_speed;
static float s_smooth_angular_speed;
static float s_smooth_pitch_speed;

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

// claude@NOTE: PARKED on the manual quaternion-product expansion (math-inline wall).
// 15 statements (lines 186-255), single named local target_rotation (quaternion).
// Args (delinker frame): ebx=this, [ebp+0Ch]=previous_item, [ebp+10h]=item_to_update,
// [ebp+14h]=previous_transform. Structure decoded:
//   L186: quaternion( previous_transform.get_angles_xyz() )      // get_angles + quaternion(float3)
//   L187: quaternion( item_to_update.action.state.transform.get_angles_xyz() ) // [ebp+10h]+0x14 = action.state.transform
//   L188: the 0xf3-byte w/x/y/z quaternion product (math::operator*(quaternion,quaternion) inlined)
//   L189: quaternion( previous_item.action.state.transform.get_angles_xyz() ) // [ebp+0Ch]+0x14 ; another product -> target_rotation
//   L198: target_rotation = create_matrix( target_rotation, float3(0,0,0) ); write -> m_target.transform ([this+0x10D44])
//   L199-207: m_target.look_pitch = item_to_update...; set_physics_controller_walk_vector( m_target ); ([this+0x10DCC] pitch)
//   L240-255: a physics get_transform / from_bullet roundtrip (bullet_character_controller::get_transform
//             -> from_bullet -> previous_transform = <result>; a [this+0x10DC8] controller call via vtable [edx])
// Walls: byte-exact product schedule + the from_bullet/get_transform physics tail
// (cross-module callee set). Next step: write the get_angles/operator*/create_matrix
// chain and the physics roundtrip, accept the product residual.
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

// claude@NOTE: PARKED on the manual quaternion-product expansion (math-inline wall).
// 12 statements (lines 512-529), locals: previous_rotation, angle, new_rotation,
// new_transform, axe. Structure decoded:
//   L512: float4x4 src = player_state.animation_player.are_there_any_animations()
//           ? player_state.animation_player.get_object_transform( this )
//           : player_state.transform;          // cmp [tree+0x1C]=m_animations_count,0; jbe
//   L513: quaternion previous_rotation( src ); // quaternion(float4x4 const&)
//   L514: quaternion new_rotation( <float3 input> ); // quaternion(float3)
//   L515: new_rotation = previous_rotation * new_rotation; new_rotation.get_axis_and_angle( axe, angle );
//           (the 0x100-byte w/x/y/z product is math::operator*(quaternion,quaternion) inlined)
//   L518: player_state.animation_player.set_object_transform( <node>, this );
//   L519: if( is_local ) {                     // cmp [this+0x35]=hit_initiator.is_local
//   L521-523:  apply_input( player_state, float2( m_input.angular_acceleration.* * dt*0.5 + previous_input.angular_velocity.*, ... ) * dt );
//   L525: if( fabs(angle) >= epsilon ) {        // and ecx,0x7FFFFFFF; comiss <eps>
//   L528:   new_transform = math::mul4x3( create_matrix( <rot>, float3(0,0,0) ), <node> );
//   L529:   player_state.previous_transform = new_transform; set_object_transform( <node>, new_transform ); }
// Walls: (1) the float3 input to new_rotation at L514 (esp+14 origin not statically
// resolvable through the sub-esp/push stack juggling); (2) the byte-exact w/x/y/z
// product schedule. Next step: simulate the stack to pin esp+14, then write the
// operator* form and accept the residual.
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

// claude@NOTE: structure reconstructed and paired (~71%). All statements/calls present:
// the != angle/position guards, slerp_optimized + create_matrix rotation, per-component
// position lerp and pitch blend. Residual is optimizer-schedule, not structure:
//  - the angular factor is deg2rad( s_smooth_angular_speed ) inlined (the target inlines
//    it; our LTCG emits a `call` to the standalone deg2rad, costing ~7%, so it is written
//    as deg2rad's body s/180*pi to force the inline). The base still folds 1/180*pi and
//    reassociates *time_delta earlier than the target's (1/180),(pi),(dt) order.
//  - the two get_angles_xyz() results live as named locals here but as anonymous temps in
//    the target (PDB records only 3 named locals: result/right/left_rotation), shifting the
//    frame +8 and hoisting the `this` load out of the time_delta guard.
void player::smooth( const float time_delta )
{
	if( time_delta > 0.f )
	{
		float4x4 result;

		const float3 current_angles	= m_current.transform.get_angles_xyz( );
		const float3 target_angles	= m_target.transform.get_angles_xyz( );
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
