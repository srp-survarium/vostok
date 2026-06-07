////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/math_float4x4.h>
#include <vostok/animation/skeleton.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/game_core/hand_to_weapon_ik_processor.h>
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

// STATE[81.51%|PARTIAL]: logic/control-flow match; capped by mix_transformations
// collapsing to an inline default-float4x4 in base. The target calls it OUT-OF-LINE
// (NRV); its 3-arg overload forwards to the 4-arg quaternion-slerp body which needs the
// file-local `slerp_optimized` from math_quaternion.cpp - that function has NO source in
// our tree, so the 4-arg stub inlines away and the out-of-line call cannot be materialized.
// Residual is the resulting stack-slot offset cascade. See process.md.
void hand_to_weapon_ik_processor::process( u32 current_time_in_ms, float4x4 const* weapon_matrices, float4x4* user_matrices ) const
{
	animation::skeleton_bone const&	weapon_bone			= m_skeleton->get_bone( m_weapon_bone_index );
	float4x4 const&					weapon_transform	= get_bone_matrix_in_object_space( weapon_bone, *m_skeleton, user_matrices );

	for ( hand const* h = m_hands; h != m_hands + hands_count; ++h )
	{
		if ( hand_need_correction( *h, current_time_in_ms ) )
		{
			float4x4 const&	locator_transform	= weapon_matrices[h->locator_matrix_index] * weapon_transform;

			if ( hand_need_interpolation( *h, current_time_in_ms ) )
			{
				float4x4 const&	target_transform	= get_bone_matrix_in_object_space( m_skeleton->get_bone( h->hand_bone_index ), *m_skeleton, user_matrices );
				float4x4 const&	hand_transform		= mix_transformations( target_transform, locator_transform, get_hand_coefficient( *h, current_time_in_ms ) );
				process_hand( *h, hand_transform, user_matrices );
			}
			else
				process_hand( *h, locator_transform, user_matrices );
		}
	}
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

	// FUNCTION BODY (target: 6 statements, 0x8b bytes) - structure fully matched; only the
	// return-local slot differs (target [ebp-8] vs base reuses [ebp-4]).
	// <0x583e70>|0x000|     :'185'		{
	// <0x583e79>|0x009|+0x00c:'186'		ASSERT( UNKNOWN_EXPRESSION_T( true ) );
	// <0x583e85>|0x015|+0x01e:'187'		interpolation_coeff = (current_time - h.start) / 1000.0f;
	// <0x583ea3>|0x033|+0x04f:'190'		hand_coefficient = is_active ? 1 - interp(c) : interp(c);
	// <0x583ef2>|0x082|+0x003:'191'		return hand_coefficient;  (movss/movss/fld via [ebp-8])
	// <0x583ef5>|0x085|+0x006:'192'		}
	// ******
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

	// FUNCTION BODY
	// <0x5940b1>|0x011|+0x01a:'115'		hand_bone
	// <0x5940cb>|0x02b|+0x00c:'116'		forearm_bone
	// <0x5940d7>|0x037|+0x00c:'117'		arm_bone
	// <0x5940e3>|0x043|+0x05d:'119'		forearm_matrix_index
	// <0x594140>|0x0a0|+0x060:'120'		arm_matrix_index
	// <0x5941a0>|0x100|+0x01d:'122'		forearm_len
	// <0x5941bd>|0x11d|+0x020:'123'		arm_len
	// <0x5941dd>|0x13d|+0x012:'124'		ASSERT
	// <0x5941ef>|0x14f|+0x012:'125'		ASSERT
	// <0x594201>|0x161|+0x024:'127'		arm_obj_matrix
	// <0x594225>|0x185|+0x025:'128'		arm_pos
	// <0x59424a>|0x1aa|+0x02e:'129'		arm_to_hand_len
	// <0x594278>|0x1d8|+0x014:'130'		if ( is_zero )
	// <0x59428c>|0x1ec|+0x005:'131'		    return
	// <0x594291>|0x1f1|+0x03b:'133'		arm_to_hand_dir
	// <0x5942cc>|0x22c|+0x02c:'134'		(forearm get_bone_matrix temp)
	// <0x5942f8>|0x258|+0x02e:'135'		original_forearm_dir
	// <0x594326>|0x286|+0x02e:'136'		rotation_axis
	// <0x594354>|0x2b4|+0x026:'137'		arm_alpha_angle
	// <0x59437a>|0x2da|+0x024:'138'		alpha_rotation_matrix
	// <0x59439e>|0x2fe|+0x021:'139'		new_forearm_dir
	// <0x5943bf>|0x31f|+0x026:'140'		arm_rotation_matrix
	// <0x5943e5>|0x345|+0x016:'141'		change_matrix_orientation
	// <0x5943fb>|0x35b|+0x023:'143'		forearm_obj_matrix
	// <0x59441e>|0x37e|+0x025:'144'		forearm_pos
	// <0x594443>|0x3a3|+0x035:'145'		initial_hand_pos
	// <0x594478>|0x3d8|+0x034:'146'		original_arm_dir
	// <0x5944ac>|0x40c|+0x03b:'147'		target_arm_dir
	// <0x5944e7>|0x447|+0x023:'149'		forearm_rotation_matrix
	// <0x59450a>|0x46a|+0x016:'150'		change_matrix_orientation
	// (151-175: inlined normalize/rescale ops of the is_similar rescale block)
	// <0x594520>|0x480|+0x02f:'176'		matrices[hand_matrix_index] = get_relative_matrix(...)
	// <0x59454f>|0x4af|+0x042:'177'		(rescale len check)
	// <0x594591>|0x4f1|+0x05d:'178'		if ( !is_similar ) ...c.xyz() = normalize(...) * forearm_len
	// <0x5945ee>|0x54e|+0x032:'180'		matrices[forearm_matrix_index] = get_relative_matrix(...)
	// <0x594620>|0x580|+0x05f:'181'		matrices[arm_matrix_index]     = get_relative_matrix(...)
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
void hand_to_weapon_ik_processor::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// FUNCTION BODY
	// <0x594049>|0x009|+0x020:'196'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
void hand_to_weapon_ik_processor::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x593fd9>|0x009|+0x00b:'204'
	// ******
}

} // namespace survarium
