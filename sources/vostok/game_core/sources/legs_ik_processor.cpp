////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/math_float4x4.h>
#include <vostok/animation/skeleton.h>
#include <vostok/game_core/legs_ik_processor.h>
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/game_core/ik_utils.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>
#include <vostok/console_command.h>

// claude@NOTE: get_rotation_matrix / change_matrix_orientation are inline math
// helpers (math_float4x4_inline_2.h on target) absent from our headers; process_leg
// calls them OUT-OF-LINE (not inlined), so a self-contained definition here resolves
// the link without affecting process_leg's scored bytes (they are separate objects).
namespace vostok {
namespace math {

float4x4 get_rotation_matrix( float3 const& original_dir, float3 const& target_dir )
{
	float3 const&	axis	= original_dir ^ target_dir;
	float			axis_len	= axis.length( );
	float			cos_angle	= original_dir | target_dir;

	clamp( axis_len, -1.0f, 1.0f );
	clamp( cos_angle, -1.0f, 1.0f );

	float			angle	= atan2( axis_len, cos_angle );
	if ( is_zero( angle, epsilon_5 ) )
		return float4x4( ).identity( );

	float3 const&	normalized_axis	= normalize( axis );
	return create_rotation( normalized_axis, -angle );
}

void change_matrix_orientation( float4x4 const& rotation, float4x4& matrix )
{
	float3 const	position	= matrix.c.xyz( );
	matrix.c.xyz( )	= float3( 0.0f, 0.0f, 0.0f );
	matrix			= matrix * rotation;
	matrix.c.xyz( )	= position;
}

} // namespace math
} // namespace vostok

namespace survarium {

// claude@NOTE: defined out-of-line in ik_processor.cpp (same module); process()
// calls it to build the hip object-space matrix.
float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices );

// claude@MATCH: the s_ik_*_cc console-command static initializers. Each `static
// console_commands::cc_*` emits the `dynamic initializer for 's_ik_*_cc'` (ctor +
// atexit) and a matching `dynamic atexit destructor`. Backing value statics are
// bound by reference; get_foot_fixed_transform / process read them. Names taken from
// the mangled ??_C@ string constants; cc kinds/args from each initializer's target
// asm (cc_bool for debug_draw/rot_axis/adjust_hip, cc_float for foot_capsule). See
// dispersion_calculator.cpp / bullet.cpp for the identical-shape reference inits.
//
// claude@NOTE: report.json leaves every `dynamic initializer`/`dynamic atexit
// destructor` thunk UNSCORED (fuzzy_match_percent: None) - objdiff does not pair the
// base `??__E.../??__F...` mangled names with the target's demangled "dynamic
// initializer/destructor" names. This is the SAME None every cc init in the codebase
// reports (dispersion/bullet/etc.), a universal name-pairing artifact, not 0%.
//
// VERIFIED (base init asm vs target init asm, both pulled via pdb_fetch):
//   * cc_float (foot_capsule): base is BYTE-IDENTICAL to target - same push 1 / fld
//     [3e4ccccd]=0.2f / push &value / push name / mov eax,1 / xor ecx / movss
//     xmm0,[3c23d70a]=0.01f / mov esi,&cc / call. Fully matched.
//   * cc_bool (debug_draw/rot_axis/adjust_hip): same arg VALUES, only the passing
//     convention differs - base is plain thiscall (this in eax; serializable,
//     command_type, execution_filter pushed on the stack: push <filter>; push 0
//     <command_type=engine_internal>; push 0 <serializable=false>; push &value; push
//     name), the target uses a whole-program LTCG custom convention (this in esi, and
//     two args in registers: eax=1 plus ecx=<filter>). The base stack pushes carry the
//     correct values - the omitted explicit filter on debug_draw defaults to
//     execution_filter_general(=1), matching target ecx=1; rot_axis/adjust_hip pass
//     execution_filter_early(=0), matching target ecx=0. (eax=1 is a constant the cc_bool
//     convention loads in all three regardless of command_type, NOT command_type-in-eax.)
// So all four are effectively DONE - bytes correct, capped only by the None pairing
// artifact (cc_bool also by the register-vs-stack call-boundary LTCG choice).
static bool		s_ik_legs_debug_draw_value		= false;
static float	s_ik_foot_capsule_radius_value	= 0.0f;
static bool		s_ik_legs_rot_axis_value		= false;
static bool		s_ik_adjust_hip_position_value	= false;

// Each cc static below emits a compiler-generated dynamic initializer (+ a paired atexit
// destructor) thunk; report.json leaves both unscored (None, name-pairing artifact), never 0%.
// All four init bytes are VERIFIED correct against the target (see claude@NOTE above).

// STATE[100%|DONE]: cc_bool init verified vs target (arg values match; cc_bool register-vs-stack LTCG)
static console_commands::cc_bool	s_ik_legs_debug_draw_cc		( "ik_legs_debug_draw", s_ik_legs_debug_draw_value, false, console_commands::command_type_engine_internal );
// STATE[100%|DONE]: cc_float init is byte-identical to target
static console_commands::cc_float	s_ik_foot_capsule_radius_cc	( "ik_foot_capsule_radius", s_ik_foot_capsule_radius_value, 0.01f, 0.2f, true, console_commands::command_type_engine_internal );
// STATE[100%|DONE]: cc_bool init verified vs target (filter_early -> ecx=0; cc_bool register-vs-stack LTCG)
static console_commands::cc_bool	s_ik_legs_rot_axis_cc		( "ik_legs_rot_axis", s_ik_legs_rot_axis_value, false, console_commands::command_type_engine_internal, console_commands::execution_filter_early );
// STATE[100%|DONE]: cc_bool init verified vs target (filter_early -> ecx=0; cc_bool register-vs-stack LTCG)
static console_commands::cc_bool	s_ik_adjust_hip_position_cc	( "ik_adjust_hip_position", s_ik_adjust_hip_position_value, false, console_commands::command_type_engine_internal, console_commands::execution_filter_early );

// STATE[100%|DONE]
legs_ik_processor::leg_params::leg_params( ) :
	heel_transition_time	( 0.0f ),
	toe_transition_time		( 0.0f ),
	rotation_axis			( 1.0f, 0.0f, 0.0f ),
	m_time_since_stance		( 0.0f ),
	m_heel_on_ground		( false ),
	m_toe_on_ground			( false )
{
}

// STATE[100%|DONE]
void legs_ik_processor::leg_params::activate( animation::skeleton const& skeleton, pcstr foot_bone_name )
{
	animation::skeleton_bone const&	foot_bone	= skeleton.get_bone( skeleton.get_bone_index( foot_bone_name ) );

	foot_bone_index		= skeleton.get_bone_index( foot_bone );
	toe_bone_index		= skeleton.get_bone_index( *foot_bone.children_begin( ) );
	leg_bone_index		= skeleton.get_bone_index( *foot_bone.parent( ) );
	knee_bone_index		= skeleton.get_bone_index( *foot_bone.parent( )->parent( ) );
	up_leg_bone_index	= skeleton.get_bone_index( *foot_bone.parent( )->parent( )->parent( ) );
}

// STATE[85.71%|PARTIAL]: only the `VOSTOK_DELETE_IMPL( ..., m_drawer )` delete is real source; the residual is
// the compiler-generated member-destructor epilogue - the target sets `this+0x7c`
// (m_toe_interpolator) and `this+0x70` (m_heel_interpolator) before each
// ~fermi_interpolator call, base ICF-folds those `this`-pointer setups away. An
// ICF/codegen artifact in the auto-emitted dtor tail, not source-steerable.
legs_ik_processor::~legs_ik_processor( )
{
	VOSTOK_DELETE_IMPL( *::survarium::g_allocator, m_drawer );

	// STRUCTURE DIFF: target 1 / base 1 stmts (no diverging rows; 0x51 vs 0x45 bytes)
	// VERDICT: STRUCTURE MATCH - residual is the compiler-generated member-dtor epilogue (target sets this+0x7c/+0x70 before each ~fermi_interpolator call, base ICF-folds those setups away); non-steerable.
}

// STATE[97.42%|DONE]: residual is a single extra 4-byte frame slot (target sub
// esp,8 / this at [ebp-8]; base push ecx / this at [ebp-4]) - an LTCG frame-layout
// artifact (a phantom temp at the math::max call boundary, same family as the
// swapped-xmm set_*_transition_time residual). Every statement/operand byte-exact.
void legs_ik_processor::leg_params::tick( float dt )
{
	heel_transition_time	= math::max( heel_transition_time - dt, 0.0f );
	toe_transition_time		= math::max( toe_transition_time - dt, 0.0f );
	m_time_since_stance		+= dt;

	// STRUCTURE DIFF: target 3 / base 3 stmts (no diverging rows)
	// VERDICT: STRUCTURE MATCH - residual is one extra 4-byte frame slot (target sub esp,8; base push ecx), LTCG frame layout, non-steerable.
}

// STATE[100%|DONE]
// claude@MATCH: arg order is min( member, tr_time ) - verified 100% by build. The swapped
// min( tr_time, member ) regresses to 83.69% (an extra movss reorders the operand spills).
void legs_ik_processor::leg_params::set_heel_transition_time( float tr_time )
{
	heel_transition_time = math::min( heel_transition_time, tr_time );
}

// STATE[100%|DONE]
void legs_ik_processor::leg_params::set_toe_transition_time( float tr_time )
{
	toe_transition_time = math::min( toe_transition_time, tr_time );
}

// STATE[100%|DONE]
void legs_ik_processor::leg_params::set_heel_on_ground( bool value )
{
	m_heel_on_ground = value;
	if ( is_full_on_ground( ) )
		m_time_since_stance = 0.0f;
}

// STATE[100%|DONE]
void legs_ik_processor::leg_params::set_toe_on_ground( bool value )
{
	m_toe_on_ground = value;
	if ( is_full_on_ground( ) )
		m_time_since_stance = 0.0f;
}

// STATE[100%|DONE]
legs_ik_processor::legs_ik_processor( ) :
	m_drawer				( NULL ),
	m_character_controller	( NULL ),
	m_heel_interpolator		( 0.1f ),
	m_toe_interpolator		( 0.1f )
{
	m_heel_transition_time	= m_heel_interpolator.transition_time( );
	m_toe_transition_time	= m_toe_interpolator.transition_time( );
}

// STATE[100%|DONE]
void legs_ik_processor::activate( animation::skeleton const& skeleton )
{
	ik_processor::activate( skeleton );
	m_left_leg_params.activate( skeleton, "LeftFoot" );
	m_right_leg_params.activate( skeleton, "RightFoot" );
	m_hip_bone	= &skeleton.get_bone( skeleton.get_bone_index( "Hip" ) );
}

// STATE[65.38%|PARTIAL]: only residual is the dot product `operator|` being INLINED
// in base while the target emits a `call vostok::math::operator|` - a per-call-site
// whole-program LTCG inline-vs-call of a trivial COMDAT (both binaries keep the
// standalone operator|; base just inlined this site). Not steerable from this
// function's source. The cascading frame shift (sub esp,24h vs 20h) and the extra
// [ebp-18h] temp all follow from that one inline. Every other statement, all
// constants (1.0f/0.5f/epsilon_5) and the ternary control flow are byte-exact.
float get_additional_length( float3 const& upleg_dir, float3 const& leg_dir, float knee_len )
{
	float const knee_angle_cos	= upleg_dir | -leg_dir;
	return math::is_similar( knee_angle_cos, 1.0f, math::epsilon_5 )
		? knee_len * 0.5f
		: math::sqrt( math::sqr( knee_len ) * 0.5f / ( 1.0f - knee_angle_cos ) );

	// STRUCTURE DIFF: target 2 / base 2 stmts
	// SIZE +0x31 | 221 | float const knee_angle_cos	= upleg_dir | -leg_dir;
	// VERDICT: STRUCTURE MATCH (shape ok) - operator| (dot product) inlined in base vs out-of-line call in target; proven non-steerable on #144 (3 source forms all inlined).
}

// STATE[98.96%|PARTIAL]: 92.60 -> 98.96 by (1) direct m_skeleton access (the get_skeleton()
// accessor left a +0xc call-result temp per site the target lacks) and (2) merging the final
// `else if ( ... ) ...is_on_ground( );` onto ONE line - the target attributes test+body to a
// single statement (0xbd = 0x64 + 0x59), which was the one REAL quantity diff the old
// "aligner swap" verdict hid (review_todos row updated). 23/23 statements now.
void legs_ik_processor::process( float4x4* matrices, float4x4 const& transform )
{
	float4x4				hip_obj_matrix				= get_bone_matrix_in_object_space( *m_hip_bone, *m_skeleton, matrices );
	float4x4 const&			hip_world_matrix			= hip_obj_matrix * transform;

	float					right_delta_len				= 0.0f;
	float4x4 const&			right_foot_fixed_transform	= get_foot_fixed_transform( m_right_leg_params, hip_world_matrix, matrices, right_delta_len );
	float					left_delta_len				= 0.0f;
	float4x4 const&			left_foot_fixed_transform	= get_foot_fixed_transform( m_left_leg_params, hip_world_matrix, matrices, left_delta_len );

	float4x4				inverted_transform;
	bool const				success						= inverted_transform.try_invert( transform );
	R_ASSERT_U( success );

	if ( !s_ik_adjust_hip_position_value || ( right_delta_len >= 0.0f && left_delta_len >= 0.0f ) )
	{
		process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
		process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	}
	else if ( m_left_leg_params.is_on_ground( ) && right_delta_len > 0.0f && 0.0f > left_delta_len )
	{
		matrices->c.y			+= right_delta_len;
		hip_obj_matrix			= get_bone_matrix_in_object_space( *m_hip_bone, *m_skeleton, matrices );
		process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
		process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	}
	else if ( m_right_leg_params.is_on_ground( ) && right_delta_len > 0.0f && 0.0f > left_delta_len )
	{
		matrices->c.y			+= left_delta_len;
		hip_obj_matrix			= get_bone_matrix_in_object_space( *m_hip_bone, *m_skeleton, matrices );
		process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
		process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	}
	// claude@MATCH: the whole branch is ONE target statement (0xbd = test 0x64 + call 0x59):
	// condition and body sit on the SAME source line in the original.
	else if ( m_left_leg_params.is_on_ground( ) ) m_right_leg_params.is_on_ground( );

	// STRUCTURE DIFF: target 23 / base 23 stmts
	// SIZE -0x3 | 256 | float4x4 const&			left_foot_fixed_transform	= get_foot_fixed_transform( m_left_leg_params, hip_world_matrix, matrices, left_delta_len );
	// SIZE -0x3 | 265 | process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	// SIZE -0x3 | 272 | process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	// SIZE -0x3 | 279 | process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	// VERDICT: STRUCTURE MATCH (shape ok) - identical instruction streams; the -0x3s are the left_foot_fixed_transform ref slot landing at disp8 [ebp-4] in base vs disp32 [ebp-94h] in target (LTCG slot allocation), non-steerable.
}

// STATE[86.08%|PARTIAL]: 83.12 -> 86.08 via direct m_skeleton access (the get_skeleton()
// accessor temp, +0xc per matrix-index row, all five closed). Braced 3-stage IK blocks are
// CORRECT - do not unbrace. Residual: float3 is_similar inline-vs-call (x4 in the early-out
// if, +0x82), get_angle out-of-line in target (STUB here), call-boundary temps/slot disps.
void legs_ik_processor::process_leg(
	legs_ik_processor::leg_params&		params,
	float4x4 const&						target_foot_obj_matrix,
	float4x4 const&						hip_obj_matrix,
	float4x4*							matrices,
	float4x4 const&						transform
)
{
	u32 const				toe_matrix_index	= params.toe_bone_index    - m_skeleton->get_root_bones_count( );
	u32 const				foot_matrix_index	= params.foot_bone_index   - m_skeleton->get_root_bones_count( );
	u32 const				leg_matrix_index	= params.leg_bone_index    - m_skeleton->get_root_bones_count( );
	u32 const				knee_matrix_index	= params.knee_bone_index   - m_skeleton->get_root_bones_count( );
	u32 const				up_leg_matrix_index	= params.up_leg_bone_index - m_skeleton->get_root_bones_count( );

	float const				leg_len				= matrices[knee_matrix_index].c.xyz( ).length( );
	float const				up_leg_len			= matrices[leg_matrix_index].c.xyz( ).length( );
	float const				knee_len			= matrices[foot_matrix_index].c.xyz( ).length( );

	float4x4				up_leg_obj_matrix	= matrices[up_leg_matrix_index] * hip_obj_matrix;
	float4x4				knee_obj_matrix		= matrices[knee_matrix_index] * up_leg_obj_matrix;
	float4x4				leg_obj_matrix		= matrices[leg_matrix_index] * knee_obj_matrix;
	float4x4				foot_obj_matrix		= matrices[foot_matrix_index] * leg_obj_matrix;
	float4x4				toe_obj_matrix		= matrices[toe_matrix_index] * foot_obj_matrix;

	if ( math::is_similar( target_foot_obj_matrix.i.xyz( ), foot_obj_matrix.i.xyz( ), math::epsilon_3 ) &&
		 math::is_similar( target_foot_obj_matrix.j.xyz( ), foot_obj_matrix.j.xyz( ), math::epsilon_3 ) &&
		 math::is_similar( target_foot_obj_matrix.k.xyz( ), foot_obj_matrix.k.xyz( ), math::epsilon_3 ) &&
		 math::is_similar( target_foot_obj_matrix.c.xyz( ), foot_obj_matrix.c.xyz( ), math::epsilon_3 ) )
		return;

	if ( s_ik_legs_debug_draw_value && m_drawer )
		m_drawer->draw_leg(
			up_leg_obj_matrix * transform,
			knee_obj_matrix * transform,
			leg_obj_matrix * transform,
			foot_obj_matrix * transform,
			math::color( 0x64u, 0x64u, 0x64u ),
			math::color( 0x64u, 0x00u, 0x00u ),
			math::color( 0x00u, 0x00u, 0x64u ),
			math::color( 0x00u, 0x64u, 0x00u ),
			0.0f );

	float3 const&			target_up_leg_to_foot_dir		= math::normalize( up_leg_obj_matrix.c.xyz( ) - target_foot_obj_matrix.c.xyz( ) );

	{
		float const				up_leg_to_foot_len				= ( up_leg_obj_matrix.c.xyz( ) - target_foot_obj_matrix.c.xyz( ) ).length( );

		float const				additive_len					= get_additional_length( math::normalize( foot_obj_matrix.c.xyz( ) - up_leg_obj_matrix.c.xyz( ) ), math::normalize( leg_obj_matrix.c.xyz( ) - foot_obj_matrix.c.xyz( ) ), knee_len );
		float const				up_leg_alpha_angle				= get_angle( leg_len + additive_len, up_leg_to_foot_len, knee_len + additive_len );

		float3 const&			original_up_leg_dir				= math::normalize( foot_obj_matrix.c.xyz( ) - up_leg_obj_matrix.c.xyz( ) );
		float3 const&			target_up_leg_dir				= math::normalize( target_foot_obj_matrix.c.xyz( ) - up_leg_obj_matrix.c.xyz( ) );

		if ( !math::is_similar( target_up_leg_dir, original_up_leg_dir, math::epsilon_3 ) )
			params.rotation_axis	= math::normalize( target_up_leg_dir ^ original_up_leg_dir );

		float4x4 const&			alpha_rotation_matrix			= math::create_rotation( params.rotation_axis, up_leg_alpha_angle );
		float3 const&			rotated_dir						= alpha_rotation_matrix.transform_direction( target_up_leg_to_foot_dir );
		float4x4 const&			rotation_matrix					= math::get_rotation_matrix( original_up_leg_dir, rotated_dir );
		math::change_matrix_orientation( rotation_matrix, up_leg_obj_matrix );
	}

	{
		knee_obj_matrix		= matrices[knee_matrix_index] * up_leg_obj_matrix;
		leg_obj_matrix		= matrices[leg_matrix_index] * knee_obj_matrix;

		float3 const&			original_knee_dir				= math::normalize( leg_obj_matrix.c.xyz( ) - knee_obj_matrix.c.xyz( ) );
		float4x4 const&			rotation_matrix2				= math::get_rotation_matrix( original_knee_dir, target_up_leg_to_foot_dir );
		math::change_matrix_orientation( rotation_matrix2, knee_obj_matrix );
	}

	{
		leg_obj_matrix		= matrices[leg_matrix_index] * knee_obj_matrix;
		foot_obj_matrix		= matrices[foot_matrix_index] * leg_obj_matrix;

		float3 const&			original_leg_dir				= math::normalize( foot_obj_matrix.c.xyz( ) - leg_obj_matrix.c.xyz( ) );
		float3 const&			target_leg_dir					= math::normalize( target_foot_obj_matrix.c.xyz( ) - leg_obj_matrix.c.xyz( ) );
		float4x4 const&			rotation_matrix3				= math::get_rotation_matrix( original_leg_dir, target_leg_dir );
		math::change_matrix_orientation( rotation_matrix3, leg_obj_matrix );
		foot_obj_matrix		= target_foot_obj_matrix;
	}

	if ( s_ik_legs_debug_draw_value && m_drawer )
	{
		m_drawer->draw_leg(
			up_leg_obj_matrix * transform,
			knee_obj_matrix * transform,
			leg_obj_matrix * transform,
			foot_obj_matrix * transform,
			math::color( 0x96u, 0x96u, 0x96u ),
			math::color( 0xFFu, 0x00u, 0x00u ),
			math::color( 0x00u, 0x00u, 0xFFu ),
			math::color( 0x00u, 0xFFu, 0x00u ),
			0.0f );

		if ( params.is_heel_on_ground( ) )
		{
			float3 const&	foot_pos	= transform.transform_position( target_foot_obj_matrix.c.xyz( ) );
			m_drawer->draw_cross( foot_pos, s_ik_foot_capsule_radius_value, math::color( 0x00u, 0xFFu, 0x00u, 0x00u ), false );
		}

		if ( params.is_toe_on_ground( ) )
		{
			u32 const		toe_matrix_index	= m_skeleton->get_bone_index( m_skeleton->get_bone( params.foot_bone_index ) ) - m_skeleton->get_root_bones_count( );
			float3 const&	toe_pos	= transform.transform_position( ( matrices[toe_matrix_index] * target_foot_obj_matrix ).c.xyz( ) );
			m_drawer->draw_origin( math::create_translation( toe_pos ), s_ik_foot_capsule_radius_value, false );
		}
	}

	matrices[up_leg_matrix_index]	= math::get_relative_matrix( up_leg_obj_matrix, hip_obj_matrix );
	matrices[knee_matrix_index]		= math::get_relative_matrix( knee_obj_matrix, up_leg_obj_matrix );
	matrices[leg_matrix_index]		= math::get_relative_matrix( leg_obj_matrix, knee_obj_matrix );

	float3 const			foot_pos	= matrices[foot_matrix_index].c.xyz( );
	matrices[foot_matrix_index]		= math::get_relative_matrix( foot_obj_matrix, target_foot_obj_matrix );
	matrices[foot_matrix_index].c.xyz( )	= foot_pos;

	return;

	// STRUCTURE DIFF: target 56 / base 56 stmts
	// SIZE +0x82 | 370 | math::is_similar( ..., math::epsilon_3 ) ) (the 4x float3 is_similar early-out if)
	// SIZE +0x6  | 390 | float const additive_len = get_additional_length( ... );
	// BASE_ONLY  | 391 | up_leg_alpha_angle = get_angle( ... ) / TRGT_ONLY 0x3e at L212 - SAME size, aligner pairing refusal
	// BASE_ONLY  | 394 | target_up_leg_dir = normalize( ... ) / TRGT_ONLY 0x4a at L217 - aligner slide of the -0x3/-0x9 dir rows
	// (rest: +/-0x3..0x11 rows on normalize/draw/matrix statements)
	// VERDICT: STRUCTURE MATCH (shape ok) - hand-pairing aligns all 56 statements in order (target L212 get_angle is 0x3e exactly like base's); the big row is float3 is_similar INLINED in base vs called out-of-line in target (x4 sites, same wall family as operator|); the small rows are call-boundary temp/slot-disp cascade. Non-steerable.
}

// STATE[88.54%|PARTIAL]: 84.65 -> 88.54 via direct m_skeleton access (the accessor temp on
// all eight matrix-index rows) plus two line-merge recoveries: `float3 start, finish;` is ONE
// target statement (0x16 = 2*0xb) and the two color ctors are ONE (0x32 = 2*0x19). Residual:
// float3 is_similar inline-vs-call in the early-out if, math call-boundary temps, and the
// draw_*capsule/set_B aligner slides (sizes pair sequentially by hand).
float4x4 legs_ik_processor::get_foot_fixed_transform(
	legs_ik_processor::leg_params const&	params,
	float4x4 const&						hip_world_matrix,
	float4x4 const*						matrices,
	float&								delta_len
) const
{
	static float const		dist_to_test					= 0.082f; // s_ik_foot_capsule_radius_value-region const

	// claude@MATCH: get_root_bones_count() is re-called per matrix in the target
	// (the index helper is inlined fresh each time), not hoisted into one local.
	float4x4 const&			up_leg_world_matrix				= matrices[params.up_leg_bone_index - m_skeleton->get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			knee_world_matrix				= matrices[params.knee_bone_index   - m_skeleton->get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			leg_world_matrix				= matrices[params.leg_bone_index    - m_skeleton->get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			foot_world_matrix				= matrices[params.foot_bone_index   - m_skeleton->get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			toe_world_matrix				= matrices[params.toe_bone_index    - m_skeleton->get_root_bones_count( )] * hip_world_matrix;

	if ( math::is_similar( foot_world_matrix.c.xyz( ), toe_world_matrix.c.xyz( ) ) ||
		 math::is_similar( foot_world_matrix.c.xyz( ), leg_world_matrix.c.xyz( ) ) )
		return foot_world_matrix;

	float3 const&			foot_to_leg_dir					= math::normalize( leg_world_matrix.c.xyz( ) - foot_world_matrix.c.xyz( ) );
	float3 const&			foot_to_toe_dir					= math::normalize( toe_world_matrix.c.xyz( ) - foot_world_matrix.c.xyz( ) );
	float3 const&			left_dir						= math::normalize( foot_to_leg_dir ^ foot_to_toe_dir );

	float4x4				result;
	result.identity( );
	result.i.xyz( )		= left_dir;
	result.j.xyz( )		= foot_to_leg_dir;
	result.k.xyz( )		= math::normalize( foot_to_leg_dir ^ left_dir );

	float const				rotation_angle					= math::deg2rad( 30.0f );
	result				= math::create_rotation( left_dir, rotation_angle ) * result;

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	result.c.xyz( )		= foot_world_matrix.c.xyz( );

	float3					up_dir( 0.0f, 0.082f, 0.005f );
	result.c.xyz( )		= result.transform_position( up_dir );

	float3					capsule_size( s_ik_foot_capsule_radius_value, 0.12f, s_ik_foot_capsule_radius_value );
	float3					foot_to_cube_center_offset( 0.0f, 1.0f, 0.0f );

	// claude@MATCH: target line-table has ONE stmt for both float3 ctors (0x16 = 2*0xb) and
	// ONE for both color ctors (0x32 = 2*0x19) - each pair declared on a single source line.
	float3					start, finish;
	math::color				original_color( 0x80u, 0xc8u, 0x00u, 0x00u ), fixed_color( 0x80u, 0x00u, 0xc8u, 0x00u );
	float					rotation_interpolation_koef		= 0.0f;

	if ( params.is_full_on_ground( ) )
	{
		start			= foot_to_cube_center_offset * dist_to_test + finish;
		finish			= foot_to_cube_center_offset * dist_to_test - finish;
		rotation_interpolation_koef	= 1.0f - m_heel_interpolator.interpolated_value( params.heel_transition_time );
	}
	else if ( params.is_heel_on_ground( ) )
	{
		start			= foot_to_cube_center_offset * dist_to_test + finish;
		finish			= foot_to_cube_center_offset * dist_to_test - finish;
		rotation_interpolation_koef	= 1.0f - m_heel_interpolator.interpolated_value( params.heel_transition_time );
	}
	else if ( params.is_toe_on_ground( ) )
	{
		start			= foot_to_cube_center_offset * dist_to_test + finish;
		finish			= foot_to_cube_center_offset * dist_to_test - finish;
		rotation_interpolation_koef	= m_toe_interpolator.interpolated_value( params.toe_transition_time );
	}
	else
	{
		start			= foot_to_cube_center_offset * dist_to_test + finish;
		finish			= start;
		// claude@MATCH: target writes only the low (b) channel byte: mov byte[tmp],64h;
		// mov cl,[tmp]; mov [original_color],cl == set_B(0x64) (b = val&0xff at offset 0).
		original_color.set_B( 0x64u );
	}

	float4x4 const&			foot_to_center_rel				= math::get_relative_matrix( foot_world_matrix, result );

	if ( s_ik_legs_debug_draw_value && m_drawer )
		m_drawer->draw_line_capsule( result, capsule_size, original_color, false );

	m_character_controller->adjust_foot_transform( capsule_size, start, finish, rotation_interpolation_koef, params.heel_transition_time, result );

	if ( s_ik_legs_debug_draw_value && m_drawer )
		m_drawer->draw_solid_capsule( result, capsule_size, fixed_color, true );

	float4x4				foot_center_transform			= foot_to_center_rel * result;

	float const				leg_len							= matrices[params.knee_bone_index   - m_skeleton->get_root_bones_count( )].c.xyz( ).length( );
	float const				up_leg_len						= matrices[params.leg_bone_index    - m_skeleton->get_root_bones_count( )].c.xyz( ).length( );
	float const				knee_len						= matrices[params.foot_bone_index   - m_skeleton->get_root_bones_count( )].c.xyz( ).length( );

	float const				up_leg_to_fixed_foot_dist		= ( up_leg_world_matrix.c.xyz( ) - foot_center_transform.c.xyz( ) ).length( );
	delta_len			= leg_len + up_leg_len + knee_len - up_leg_to_fixed_foot_dist;

	float const				up_leg_to_original_foot_dist_sqr	= ( up_leg_world_matrix.c.xyz( ) - foot_world_matrix.c.xyz( ) ).squared_length( );

	if ( math::sqr( up_leg_to_fixed_foot_dist ) > up_leg_to_original_foot_dist_sqr && params.heel_transition_time != 0.0f )
	{
		float const			position_iterpolation_koef		= 1.0f - m_heel_interpolator.interpolated_value( params.heel_transition_time );
		float3 const&		position						= foot_world_matrix.c.xyz( ) * position_iterpolation_koef + foot_center_transform.c.xyz( ) * ( 1.0f - position_iterpolation_koef );
		foot_center_transform.c.xyz( )	= position;
	}

	return foot_center_transform;

	// STRUCTURE DIFF: target 62 / base 62 stmts
	// SIZE +0x3e | 658 | math::is_similar( foot_world_matrix.c.xyz( ), leg_world_matrix.c.xyz( ) ) ) (2x float3 is_similar, inlined in base vs called in target)
	// SIZE -0x6 x6 | 694-708 | start/finish = foot_to_cube_center_offset * dist_to_test +/- finish; (target materializes &finish via a folded helper call)
	// BASE_ONLY 712/713 + SIZE -0x40 | 716 | else-branch start/finish=start/set_B rows - aligner slide; sizes pair sequentially (0x50/0x22/0x13 vs 0x4a/0x24/0x10)
	// BASE_ONLY 722/727 + TRGT_ONLY 0x30/0x22 | draw_line/draw_solid_capsule - aligner slide (+0xb/-0x3 real deltas)
	// (rest: +/-0x3..0x11 call-boundary temp rows)
	// VERDICT: STRUCTURE MATCH (shape ok) - 62/62 statements pair sequentially by hand; all rows are the is_similar/accessor inline-vs-call walls and math call-boundary temp/slot-disp cascade. Non-steerable.
}

// STATE[100%|DONE]
void legs_ik_processor::set_left_heel_on_ground( bool value )
{
	set_heel_on_ground( m_left_leg_params, value );
}

// STATE[100%|DONE]
void legs_ik_processor::set_left_toe_on_ground( bool value )
{
	set_toe_on_ground( m_left_leg_params, value );
}

// STATE[100%|DONE]
void legs_ik_processor::set_right_heel_on_ground( bool value )
{
	set_heel_on_ground( m_right_leg_params, value );
}

// STATE[100%|DONE]
void legs_ik_processor::set_right_toe_on_ground( bool value )
{
	set_toe_on_ground( m_right_leg_params, value );
}

// STATE[98.84%|DONE]: residual is a single `lea ecx,[ebp-0Ch]` (the `this` arg
// of the COMDAT-folded trivial ~fermi_interpolator temp dtor) the target emits
// before the dtor call and base omits - an ICF/LTCG call-boundary arg-passing
// artifact; every statement, the control structure and all consts match.
void legs_ik_processor::set_heel_on_ground( legs_ik_processor::leg_params& params, bool value )
{
	if ( params.is_heel_on_ground( ) == value )
		return;

	params.set_heel_on_ground( value );

	if ( value )
	{
		params.heel_transition_time = m_heel_transition_time;
		m_heel_transition_time_calculator.reset( );
	}
	else
	{
		m_heel_transition_time = m_heel_transition_time_calculator.get_value( );
		math::clamp( m_heel_transition_time, 0.001f, 0.5f );
		m_left_leg_params.set_heel_transition_time( m_heel_transition_time );
		m_right_leg_params.set_heel_transition_time( m_heel_transition_time );
		m_heel_interpolator = animation::fermi_interpolator( m_heel_transition_time );
		params.toe_transition_time = m_toe_transition_time;
		m_toe_transition_time_calculator.reset( );
	}

	// STRUCTURE DIFF: target 14 / base 14 stmts
	// SIZE -0x3 | 574 | m_heel_interpolator = animation::fermi_interpolator( m_heel_transition_time );
	// VERDICT: STRUCTURE MATCH (shape ok) - target emits lea ecx,[ebp-0Ch] (this arg of the COMDAT-folded trivial ~fermi_interpolator temp dtor) that base omits; ICF/LTCG call-boundary artifact, non-steerable.
}

// STATE[98.59%|DONE]: same ICF/LTCG residual as set_heel_on_ground - the lone
// `lea ecx,[ebp-0Ch]` (this arg of the folded ~fermi_interpolator temp dtor).
void legs_ik_processor::set_toe_on_ground( legs_ik_processor::leg_params& params, bool value )
{
	if ( params.is_toe_on_ground( ) == value )
		return;

	params.set_toe_on_ground( value );

	if ( !value )
	{
		m_toe_transition_time = m_toe_transition_time_calculator.get_value( );
		math::clamp( m_toe_transition_time, 0.001f, 0.5f );
		m_left_leg_params.set_toe_transition_time( m_toe_transition_time );
		m_right_leg_params.set_toe_transition_time( m_toe_transition_time );
		m_toe_interpolator = animation::fermi_interpolator( m_toe_transition_time );
	}

	// STRUCTURE DIFF: target 9 / base 9 stmts
	// SIZE -0x3 | 606 | m_toe_interpolator = animation::fermi_interpolator( m_toe_transition_time );
	// VERDICT: STRUCTURE MATCH (shape ok) - same ICF/LTCG residual as set_heel_on_ground (the folded ~fermi_interpolator temp-dtor this arg), non-steerable.
}

// STATE[100%|DONE]
void legs_ik_processor::tick( u32 current_time_in_ms )
{
	if ( m_last_time_in_ms != 0 )
	{
		float const		dt_sec	= ( current_time_in_ms - m_last_time_in_ms ) * math::epsilon_3;
		m_left_leg_params.tick( dt_sec );
		m_right_leg_params.tick( dt_sec );
		m_heel_transition_time_calculator.tick( dt_sec );
		m_toe_transition_time_calculator.tick( dt_sec );
	}

	m_last_time_in_ms	= current_time_in_ms;
}

} // namespace survarium
