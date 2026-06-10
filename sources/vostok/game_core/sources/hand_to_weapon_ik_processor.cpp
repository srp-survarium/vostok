////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/math_float4x4.h>
#include <vostok/animation/skeleton.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/game_core/hand_to_weapon_ik_processor.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/ik_utils.h>
#include <vostok/console_command.h>

// get_rotation_matrix / change_matrix_orientation / get_bone_matrix_in_object_space are
// out-of-line IK helpers declared in <vostok/game_core/ik_utils.h> with VOSTOK_GAME_CORE_API
// (defined in legs_ik_processor.cpp / ik_processor.cpp). get_relative_matrix is the canonical
// inline in math_float4x4_inline.h.

namespace survarium {

static float	s_aim_transition_time			= 0.3f;

static bool		s_ik_hands_debug_draw_value		= false;

static console_commands::cc_bool	s_ik_hands_debug_draw_cc	( "ik_hands_debug_draw", s_ik_hands_debug_draw_value, false, console_commands::command_type_engine_internal );

// STATE[100%|DONE]
hand_to_weapon_ik_processor::hand_to_weapon_ik_processor( )
	: m_interpolator( s_aim_transition_time ), m_current_transition_time( 0.0f ), m_active( true )
{
}

// STATE[100%|DONE]
void hand_to_weapon_ik_processor::activate( animation::skeleton const& user_skeleton, animation::skeleton const& weapon_skeleton )
{
	m_skeleton								= &user_skeleton;
	m_weapon_bone_index						= user_skeleton.get_bone_index( "Weapon" );

	m_hands[left].is_active					= false;
	m_hands[left].hand_bone_index			= user_skeleton.get_bone_index( "LeftHand" );
	m_hands[left].hand_matrix_index			= m_hands[left].hand_bone_index - user_skeleton.get_root_bones_count( );
	m_hands[left].locator_matrix_index		= weapon_skeleton.get_bone_index( "left_hand_cont" ) - weapon_skeleton.get_root_bones_count( );

	m_hands[right].is_active				= false;
	m_hands[right].hand_bone_index			= user_skeleton.get_bone_index( "RightHand" );
	m_hands[right].hand_matrix_index		= m_hands[right].hand_bone_index - user_skeleton.get_root_bones_count( );
	m_hands[right].locator_matrix_index		= weapon_skeleton.get_bone_index( "right_hand_cont" ) - weapon_skeleton.get_root_bones_count( );
}

// STATE[100%|DONE]
void hand_to_weapon_ik_processor::activate_hand( hand_to_weapon_ik_processor::hands_enum hand, bool active, u32 current_time_in_ms )
{
	if ( m_hands[hand].is_active != active )
	{
		m_hands[hand].is_active						= active;
		m_hands[hand].start_transition_time_in_ms	= get_hand_new_start_transition_time( m_hands[hand], current_time_in_ms );
		ASSERT( UNKNOWN_EXPRESSION_T( m_hands[hand].is_active == active ) );
	}
}

// STATE[100%|DONE]
bool hand_to_weapon_ik_processor::hand_need_correction( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	return h.is_active || hand_need_interpolation( h, current_time_in_ms );
}

// STATE[100%|DONE]
bool hand_to_weapon_ik_processor::hand_need_interpolation( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	return current_time_in_ms - h.start_transition_time_in_ms < 300;
}

// STATE[100%|DONE]
u32 hand_to_weapon_ik_processor::get_hand_new_start_transition_time( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms )
{
	u32 const	current_transition_time	= current_time_in_ms - h.start_transition_time_in_ms;
	return current_transition_time < 300 ? current_time_in_ms - ( 300 - current_transition_time ) : current_time_in_ms;
}

// STATE[94.64%|PARTIAL]: was 81.51 with the inner if/else inverted - the target's line records
// (then-branch = the 0x1a process_hand(locator) call, 5-byte rel32 jmp over the big else block)
// prove the original wrote `if ( !hand_need_interpolation ) process_hand(locator); else {...}`.
// Residual is mix_transformations collapsing to an inline default-float4x4 in base: the target
// calls it OUT-OF-LINE (NRV); its 3-arg overload forwards to the 4-arg quaternion-slerp body
// which needs the file-local `slerp_optimized` from math_quaternion.cpp (no source in our tree),
// so the out-of-line call cannot be materialized.
void hand_to_weapon_ik_processor::process( u32 const current_time_in_ms, float4x4 const* weapon_matrices, float4x4* user_matrices ) const
{
	animation::skeleton_bone const&	weapon_bone			= m_skeleton->get_bone( m_weapon_bone_index );
	float4x4 const&					weapon_transform	= get_bone_matrix_in_object_space( weapon_bone, *m_skeleton, user_matrices );

	for ( hand const* h = m_hands; h != m_hands + hands_count; ++h )
	{
		if ( hand_need_correction( *h, current_time_in_ms ) )
		{
			float4x4 const&	locator_transform	= weapon_matrices[h->locator_matrix_index] * weapon_transform;

			if ( !hand_need_interpolation( *h, current_time_in_ms ) )
				process_hand( *h, locator_transform, user_matrices );
			else
			{
				float4x4 const&	target_transform	= get_bone_matrix_in_object_space( m_skeleton->get_bone( h->hand_bone_index ), *m_skeleton, user_matrices );
				float4x4 const&	hand_transform		= mix_transformations( target_transform, locator_transform, get_hand_coefficient( *h, current_time_in_ms ) );
				process_hand( *h, hand_transform, user_matrices );
			}
		}
	}

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts
	// SIZE -0xe | 103 | float4x4 const& hand_transform = mix_transformations( target_transform, locator_transform, get_hand_coefficient( *h, current_time_in_ms ) );
	// VERDICT: STRUCTURE MATCH (12/12) - sole SIZE is the mix_transformations out-of-line call (target) vs inlined default-float4x4 (base, slerp_optimized body unavailable); non-steerable.
}

// STATE[99.90%|PARTIAL]: structure fully matched (single-statement coeff + named return
// local). Sole residual is one /Od stack-slot offset: target puts the return local at
// [ebp-8] (its own fresh slot), base reuses the now-dead interpolation_coeff slot [ebp-4]
// for it - one mov/fld operand differs. Instruction stream + statement structure identical.
float hand_to_weapon_ik_processor::get_hand_coefficient( hand_to_weapon_ik_processor::hand const& h, u32 current_time_in_ms ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( true ) );

	float const	interpolation_coeff		= ( current_time_in_ms - h.start_transition_time_in_ms ) / 1000.0f;

	float const	hand_coefficient		= h.is_active ? 1.0f - m_interpolator.interpolated_value( interpolation_coeff ) : m_interpolator.interpolated_value( interpolation_coeff );
	return hand_coefficient;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts (0x8b both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - sole residual is one /Od stack-slot choice (target's return local at [ebp-8], base reuses the dead [ebp-4]); allocation noise, non-steerable.
}

// STATE[90.37%|PARTIAL]: structure matched - 37==37 statements, control flow identical (no
// jmp/je/cmp/call-target diffs remain). The reviewer's missing-L134 fix landed: hoisting the
// forearm matrix's .c.xyz() into a named float3 const& (original_forearm_pos) restored the
// statement the target had (89.54->90.37). Residual ~9.6% is entirely call-boundary ABI: the
// target calls the vector-math helpers (operator-, normalize, free math::length) OUT-OF-LINE
// with register-NRV (mov esi/edi) and cdecl-by-ref; our /GL LTCG inlines them to push/push +
// member-length, which cascades the frame size (target 0x54C vs base 0x4DC) and the operand
// slots. Per-call-site inline-vs-call LTCG class (same as get_angle/acos in this TU), not
// source-steerable from this body. NOTE: forearm matrix is the .c.xyz() REFERENCE, not a full
// float4x4 local (a full-matrix hoist was wrong - target only keeps the .c.xyz() ref); the
// free math::length(L190) form REGRESSED (LTCG inlines it worse than member .length()). See md.
void hand_to_weapon_ik_processor::process_hand( hand_to_weapon_ik_processor::hand const& h, float4x4 const& target_hand_obj_space_transform, float4x4* matrices ) const
{
	animation::skeleton_bone const&	hand_bone				= m_skeleton->get_bone( h.hand_bone_index );
	animation::skeleton_bone const&	forearm_bone			= *hand_bone.parent( );
	animation::skeleton_bone const&	arm_bone				= *forearm_bone.parent( );

	u32 const						forearm_matrix_index	= m_skeleton->get_bone_index( forearm_bone ) - m_skeleton->get_root_bones_count( );
	u32 const						arm_matrix_index		= m_skeleton->get_bone_index( arm_bone ) - m_skeleton->get_root_bones_count( );

	float const						forearm_len				= matrices[h.hand_matrix_index].c.xyz( ).length( );
	float const						arm_len					= matrices[forearm_matrix_index].c.xyz( ).length( );

	ASSERT( UNKNOWN_EXPRESSION_T( forearm_len ) );
	ASSERT( UNKNOWN_EXPRESSION_T( arm_len ) );

	float4x4						arm_obj_matrix			= get_bone_matrix_in_object_space( arm_bone, *m_skeleton, matrices );
	float3							arm_pos					= arm_obj_matrix.c.xyz( );

	float const						arm_to_hand_len			= ( arm_pos - target_hand_obj_space_transform.c.xyz( ) ).length( );
	if ( math::is_zero( arm_to_hand_len, math::epsilon_5 ) )
		return;

	float3 const&					arm_to_hand_dir			= math::normalize( arm_pos - target_hand_obj_space_transform.c.xyz( ) );

	float3 const&					original_forearm_pos	= get_bone_matrix_in_object_space( forearm_bone, *m_skeleton, matrices ).c.xyz( );
	float3 const&					original_forearm_dir	= math::normalize( arm_pos - original_forearm_pos );
	float3 const&					rotation_axis			= math::normalize( arm_to_hand_dir ^ original_forearm_dir );

	float const						arm_alpha_angle			= get_angle( arm_len, arm_to_hand_len, forearm_len );
	float4x4 const&					alpha_rotation_matrix	= math::create_rotation( rotation_axis, arm_alpha_angle );
	float3 const&					new_forearm_dir			= alpha_rotation_matrix.transform_direction( arm_to_hand_dir );
	float4x4 const&					arm_rotation_matrix		= math::get_rotation_matrix( original_forearm_dir, new_forearm_dir );
	math::change_matrix_orientation( arm_rotation_matrix, arm_obj_matrix );

	float4x4						forearm_obj_matrix		= matrices[forearm_matrix_index] * arm_obj_matrix;
	float3							forearm_pos				= forearm_obj_matrix.c.xyz( );

	float3 const&					initial_hand_pos		= forearm_obj_matrix.transform_position( matrices[h.hand_matrix_index].c.xyz( ) );
	float3 const&					original_arm_dir		= math::normalize( forearm_pos - initial_hand_pos );
	float3 const&					target_arm_dir			= math::normalize( forearm_pos - target_hand_obj_space_transform.c.xyz( ) );
	float4x4 const&					forearm_rotation_matrix	= math::get_rotation_matrix( original_arm_dir, target_arm_dir );
	math::change_matrix_orientation( forearm_rotation_matrix, forearm_obj_matrix );

	matrices[h.hand_matrix_index]	= math::get_relative_matrix( target_hand_obj_space_transform, forearm_obj_matrix );
	if ( !math::is_similar( matrices[h.hand_matrix_index].c.xyz( ).length( ), forearm_len, math::epsilon_5 ) )
		matrices[h.hand_matrix_index].c.xyz( )	= math::normalize( matrices[h.hand_matrix_index].c.xyz( ) ) * forearm_len;

	matrices[forearm_matrix_index]	= math::get_relative_matrix( forearm_obj_matrix, arm_obj_matrix );
	matrices[arm_matrix_index]		= math::get_relative_matrix( get_bone_matrix_in_object_space( *arm_bone.parent( ), *m_skeleton, matrices ), arm_obj_matrix );

	// STRUCTURE DIFF: target 35 stmts / base 35 stmts
	// SIZE -0x4 | 163 | float const arm_to_hand_len = ( arm_pos - target_hand_obj_space_transform.c.xyz( ) ).length( );
	// SIZE +0x3 | 167 | float3 const& arm_to_hand_dir = math::normalize( arm_pos - target_hand_obj_space_transform.c.xyz( ) );
	// SIZE +0x6 | 170 | float3 const& original_forearm_dir = math::normalize( arm_pos - original_forearm_pos );
	// SIZE +0x6 | 171 | float3 const& rotation_axis = math::normalize( arm_to_hand_dir ^ original_forearm_dir );
	// SIZE +0x3 | 176 | float4x4 const& arm_rotation_matrix = math::get_rotation_matrix( original_forearm_dir, new_forearm_dir );
	// SIZE -0x1 | 182 | float3 const& initial_hand_pos = forearm_obj_matrix.transform_position( matrices[h.hand_matrix_index].c.xyz( ) );
	// SIZE +0x3 | 184 | float3 const& target_arm_dir = math::normalize( forearm_pos - target_hand_obj_space_transform.c.xyz( ) );
	// SIZE -0x3 | 185 | float4x4 const& forearm_rotation_matrix = math::get_rotation_matrix( original_arm_dir, target_arm_dir );
	// SIZE +0x3 | 190 | matrices[h.hand_matrix_index].c.xyz( ) = math::normalize( matrices[h.hand_matrix_index].c.xyz( ) ) * forearm_len;
	// VERDICT: STRUCTURE MATCH (35/35, control flow identical) - all SIZE rows are +-6B inline-vs-call shifts of the vector-math helpers (operator-, normalize, get_rotation_matrix) at call boundaries; per-call-site LTCG, non-steerable.
}

// STATE[70.97%|PARTIAL]: packs both hands' is_active into a u8 bitfield (bit0=left, bit1=right),
// appends it, then appends each hand's start_transition_time_in_ms biased by client_offset.
void hand_to_weapon_ik_processor::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	u8 const active_hands = ( m_hands[ left ].is_active ? 1 : 0 ) | ( m_hands[ right ].is_active ? 2 : 0 );
	packet.append( active_hands );
	packet.append( m_hands[ left  ].start_transition_time_in_ms - client_offset );
	packet.append( m_hands[ right ].start_transition_time_in_ms - client_offset );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x7 | 241 | packet.append( active_hands );
	// SIZE +0x8 | 242 | packet.append( m_hands[ left  ].start_transition_time_in_ms - client_offset );
	// SIZE +0x8 | 243 | packet.append( m_hands[ right ].start_transition_time_in_ms - client_offset );
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE rows are packet<T>::append kept out-of-line in target vs LTCG-inlined in base, cross-module wall, non-steerable.
}

// STATE[PARTIAL]: reads the packed active-hands byte then both start times; bits 0/1 of
// active_hands drive each hand's is_active.
void hand_to_weapon_ik_processor::deserialize( network_core::packet_reader& reader )
{
	u8 const active_hands = reader.r< bool >( );
	m_hands[ left  ].start_transition_time_in_ms = reader.r< u32 >( );
	m_hands[ right ].start_transition_time_in_ms = reader.r< u32 >( );

	ASSERT( UNKNOWN_EXPRESSION );
	m_hands[ left  ].is_active = ( active_hands & 1 ) != 0;
	m_hands[ right ].is_active = ( active_hands & 2 ) != 0;

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts
	// SIZE +0x15 | 257 | u8 const active_hands = reader.r< bool >( );
	// SIZE +0x15 | 258 | m_hands[ left  ].start_transition_time_in_ms = reader.r< u32 >( );
	// SIZE +0x15 | 259 | m_hands[ right ].start_transition_time_in_ms = reader.r< u32 >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - the 0xc assert eater at target L208 is restored; remaining SIZE rows are r<bool>/r<u32> kept out-of-line in target vs LTCG-inlined in base, cross-module wall, non-steerable.
}

} // namespace survarium
