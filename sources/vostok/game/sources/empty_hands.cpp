////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "empty_hands.h"
#include <vostok/game_core/base_player.h>

namespace survarium {

 empty_hands::empty_hands( resources::managed_resource_ptr* animations, const u32 animations_count ) :
	m_user( 0 ),
	m_animations( animations ),
	m_animations_count( animations_count )
{
	m_transform.identity( );
}

void empty_hands::activate( base_player& user, engine& engine )
{
	m_user		= &user;
	m_transform	= user.get_transform( );
}

// STATE[STUB]
// claude@NOTE: parked - walled by the animation-lexeme subsystem (the
// create_animation_interval / mixing_animation_lexeme_parameters wall). Target builds
// an animation_lexeme over m_animations (animation_lexeme_parameters::
// animation_intervals_count / create_animation_intervals), wraps it in a
// binary_tree_animation_node + a linear_interpolator, and returns
// expression( lexeme ) cloned_in_buffer( buffer ). NEXT: unblock once the lexeme
// builders are implemented (see create-animation-interval-lexeme-wall memory note).
animation::mixing::expression empty_hands::selected_animations( mutable_buffer& buffer, const bool is_third_view ) const
{
	VOSTOK_UNREACHABLE_CODE( );	// buildability device - expression has no default ctor (the booby_trap_set_core precedent)
}

// STATE[STUB]
// claude@NOTE: parked - target computes the user-hand bones: __find_if the bone by id,
// n_ary_tree::compute_bones_local_matrices + convert_to_object_matrices over the
// animation_player tree, then mul4x3 / calculated_head_matrix to place the result into
// character_head_transform / character_transform using user_transform (a float4x4 const&
// accessor on the skeleton at 0x5bcc83). NEXT: recover the n_ary_tree accessors + the
// bone-search predicate (lines 78-87).
void empty_hands::update_bones_matrices(
	animation::skeleton_ptr const&			user_skeleton,
	float4x4* const							user_matrices,
	const u32								user_matrices_count,
	const u32								current_time_in_ms,
	float4x4&								character_head_transform,
	float4x4&								character_transform,
	animation::animation_player const&		animation_player
)
{
}

} // namespace survarium
