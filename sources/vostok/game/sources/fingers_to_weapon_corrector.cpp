////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "fingers_to_weapon_corrector.h"

#include <vostok/animation/skeleton.h>
#include <vostok/render/facade/model.h>
#include <vostok/game_core/ik_utils.h>	// mix_transformations
#include <vostok/fixed_string.h>
#include <vostok/math_float4x4.h>
#include <string.h>

namespace survarium {

// The phalange bone identifiers per hand (left/right). The arm fingers correspond
// to the skeleton bone ids; the locator pass appends "_loc" and looks the matching
// locator up in the weapon model. The trailing 16th slot per row pads the stride to
// the float4x4 array the locator pass fills (only the first 15 feed the bone indices).
static pcstr s_arm_fingers_phalanges[fingers_to_weapon_corrector::hands_count][16] =
{
	{
		"LeftHandThumb1",  "LeftHandThumb2",  "LeftHandThumb3",
		"LeftHandIndex1",  "LeftHandIndex2",  "LeftHandIndex3",
		"LeftHandMiddle1", "LeftHandMiddle2", "LeftHandMiddle3",
		"LeftHandRing1",   "LeftHandRing2",   "LeftHandRing3",
		"LeftHandPinky1",  "LeftHandPinky2",  "LeftHandPinky3",
		"LeftHand",
	},
	{
		"RightHandThumb1",  "RightHandThumb2",  "RightHandThumb3",
		"RightHandIndex1",  "RightHandIndex2",  "RightHandIndex3",
		"RightHandMiddle1", "RightHandMiddle2", "RightHandMiddle3",
		"RightHandRing1",   "RightHandRing2",   "RightHandRing3",
		"RightHandPinky1",  "RightHandPinky2",  "RightHandPinky3",
		"RightHand",
	},
};

// Parent matrix index of each phalange in the locator chain (15 phalanges).
static u32 s_index_of_parent[15] =
{
	15, 0, 1, 15, 3, 4, 15, 6, 7, 15, 9, 10, 15, 12, 13,
};

// claude@NOTE: init-list ctor, 0 body statements - structurally correct. Residual is an
// inline-vs-call wall on linear_interpolator::linear_interpolator(float): the target inlines
// it (writes the vtable + m_total_transition_time directly), our base emits a call. The
// decision is LTCG-driven and flips per call site (the sibling hand_to_weapon_ik_processor
// ctor shows the opposite: target calls, base inlines). Not source-steerable without
// __forceinline on the shared interpolator header (off-limits - other units' call sites).
 fingers_to_weapon_corrector::fingers_to_weapon_corrector( ) :
	m_interpolator( 0.1f )
{
}

void fingers_to_weapon_corrector::activate(
	animation::skeleton const&				character_skeleton,
	render::render_model_instance_ptr		weapon_model,
	const bool								first_person_view
)
{
	initialize_bones_indices							( character_skeleton );
	initialize_locators									( *weapon_model, first_person_view );
}

// claude@NOTE: structure matches (3 stmts, nested for + get_bone_index-get_root_bones_count).
// Residual is register allocation: the target keeps character_skeleton in a callee-saved
// reg (ebp, surviving the inlined __find_if call) and the bone-index / root-count divides
// are scheduled in the opposite order to our base. Not source-steerable.
void fingers_to_weapon_corrector::initialize_bones_indices( animation::skeleton const& character_skeleton )
{
	for ( u32 hand = 0; hand < hands_count; ++hand )
		for ( u32 i = 0; i < 15; ++i )
			m_hands[hand].phalanges_bones_indices[i] = character_skeleton.get_bone_index( s_arm_fingers_phalanges[hand][i] ) - character_skeleton.get_root_bones_count( );
}

// claude@NOTE: structure matches (10 stmts). Residual is stack-slot coloring: the target
// places the mul4x3 return temporary at the lowest local slot (pushing current_item +0x40
// up), while our base colors current_item lowest and the temp high. Same locals, same
// statements - the slot assignment order is a compiler choice, not source-steerable.
void fingers_to_weapon_corrector::initialize_locators( render::render_model_instance const& weapon_model, const bool first_person_view )
{
	typedef fixed_string< 256 >		locator_name_string_type;

	m_first_person_view									= first_person_view;

	float4x4							matrices[16];
	float4x4							inverted_matrices[16];
	render::model_locator_item			current_item;

	for ( u32 hand = 0; hand < hands_count; ++hand )
	{
		for ( u32 i = 0; i < 16; ++i )
		{
			locator_name_string_type const	locator_name	= locator_name_string_type::createf( "%s_loc", s_arm_fingers_phalanges[hand][i] );
			weapon_model.get_locator		( locator_name.c_str(), current_item );

			matrices[i]						= current_item.m_offset;
			matrices[i].try_invert			( inverted_matrices[i] );
		}

		for ( u32 i = 0; i < 15; ++i )
			matrices[i]						= math::mul4x3( matrices[i], inverted_matrices[s_index_of_parent[i]] );

		memmove								( m_hands[hand].phalanges_matrices, matrices, sizeof( m_hands[hand].phalanges_matrices ) );
	}
}

void interpolate_hand_matrices(
	float4x4 const*		locator_matrices,
	u32 const*			bone_indices,
	const u32			phalanges_count,
	const float			iterpolation_coeff,
	float4x4*			result_matrices
)
{
	for ( u32 i = 0; i < phalanges_count; ++i )
		result_matrices[bone_indices[i]] = mix_transformations( result_matrices[bone_indices[i]], locator_matrices[i], iterpolation_coeff );
}

// claude@NOTE: structure matches (7 stmts, if/else over the per-hand transition). Residual
// is the is_active fast-path copy loop: our base unrolls it 5x (15 = 5*3) while the target
// keeps it rolled (15 iterations, one rep movsd each). The unroll factor is an optimizer
// heuristic on the constant trip count, not source-steerable.
void fingers_to_weapon_corrector::process( const u32 current_time_in_ms, float4x4* matrices ) const
{
	for ( hand const* current_hand = m_hands; current_hand != m_hands + hands_count; ++current_hand )
	{
		if ( current_hand->start_transition_time_in_ms + 100 <= current_time_in_ms )
		{
			if ( current_hand->is_active )
				for ( u32 i = 0; i != 15; ++i ) matrices[current_hand->phalanges_bones_indices[i]] = current_hand->phalanges_matrices[i];
		}
		else
		{
			const float interpolation_coeff = get_hand_coefficient( ( current_time_in_ms - current_hand->start_transition_time_in_ms ) * math::epsilon_3, current_hand->is_active );
			interpolate_hand_matrices( current_hand->phalanges_matrices, current_hand->phalanges_bones_indices, 15, interpolation_coeff, matrices );
		}
	}
}

float fingers_to_weapon_corrector::get_hand_coefficient( const float hand_transition_time, const bool hand_active ) const
{
	return hand_active
		? 1.0f - m_interpolator.interpolated_value( hand_transition_time )
		: m_interpolator.interpolated_value( hand_transition_time );
}

void fingers_to_weapon_corrector::activate_hand(
	const fingers_to_weapon_corrector::hands_enum	hand,
	const bool		is_active,
	const u32		current_time_in_ms
)
{
	if ( m_hands[ hand ].is_active != is_active )
	{
		m_hands[ hand ].is_active = is_active;
		m_hands[ hand ].start_transition_time_in_ms = current_time_in_ms;
	}
}

} // namespace survarium
