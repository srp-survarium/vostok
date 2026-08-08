////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "empty_hands.h"
#include <vostok/game_core/base_player.h>

#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/mixing_expression.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/animation_player.h>
#include <vostok/animation/skeleton.h>
#include <vostok/math_float4x4.h>

namespace vostok { namespace animation {
math::float4x4 calculated_head_matrix( math::float4x4 const& head_matrix, math::float4x4 const& object_matrix );
} } // namespace vostok::animation

namespace survarium {

static float s_aim_transition_time = 0.3f;

// claude@NOTE: STRUCTURE MATCH (1 stmt). Capped ~82%: target inlines interactive_object's
// implicit default ctor and calls unmanaged_resource::unmanaged_resource(1) directly, while
// base emits interactive_object::interactive_object() out-of-line. Inline-vs-call of a
// different unit's (interactive_object) implicit ctor - not steerable from here.
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

// claude@NOTE: STRUCTURE MATCH (2 stmts). Capped by the animation-lexeme inline-vs-call
// wall shared with single_animation::emit - gold inlines animation_lexeme_parameters /
// animation_lexeme / expression construction that our /Od base emits as the same calls.
animation::mixing::expression empty_hands::selected_animations( mutable_buffer& buffer, const bool is_third_view ) const
{
	VOSTOK_UNREFERENCED_PARAMETER	( is_third_view );

	animation::linear_interpolator l_interpolator( s_aim_transition_time );
	animation::mixing::animation_lexeme lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"empty_hands",
			// sushi@TODO: const method mutates m_random - const_cast vs `mutable m_random` is
			// byte-identical; confirm the original spelling (header is 100% with m_random non-mutable).
			m_animations[ const_cast< empty_hands* >( this )->m_random.random( m_animations_count ) ],
			NULL, NULL
		)
		.weight_interpolator( l_interpolator )
		.time_scale_interpolator( l_interpolator )
		.animated_object( m_user )
		.unique_animation_id( u8( -1 ) )
	);

	return animation::mixing::expression( lexeme );
}

// claude@NOTE: STRUCTURE MATCH (7 stmts). Direct analog of weapon_core::update_bones_matrices.
// Capped by the animation inline-vs-call wall (compute/convert_*_matrices and skeleton bone-index
// accessors are inlined gold-side, called here).
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
	VOSTOK_UNREFERENCED_PARAMETER	( current_time_in_ms );

	animation_player.compute_bones_local_matrices	( *user_skeleton, user_matrices, user_matrices + user_matrices_count, m_user, NULL );

	animation_player.convert_to_object_matrices		( *user_skeleton, user_matrices, user_matrices + user_matrices_count, m_user );

	float4x4 const&	user_transform					= m_user->get_transform( );

	u32 const		head_bone_index					= ( *user_skeleton ).get_bone_index( "Head" ) - ( *user_skeleton ).get_root_bones_count( );
	character_head_transform = animation::calculated_head_matrix( user_matrices[head_bone_index], user_transform );

	u32 const		root_bone_index					= ( *user_skeleton ).get_bone_index( "Root" ) - ( *user_skeleton ).get_root_bones_count( );
	character_transform = math::mul4x3( user_matrices[root_bone_index], user_transform );
}

} // namespace survarium
