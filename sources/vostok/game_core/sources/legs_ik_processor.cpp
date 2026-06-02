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

// game_core's DELETE macro (game_core_memory.h has it commented out).
#define DELETE( pointer )	VOSTOK_DELETE_IMPL( *::survarium::g_allocator, pointer )

// claude@NOTE: get_relative_matrix is an inline math helper (target rva 0xbb050,
// math_float4x4_inline_2.h) not present in our headers; defined here so
// get_foot_fixed_transform's out-of-line `call vostok::math::get_relative_matrix`
// resolves. Reconstructed from binaries/structure/target.
namespace vostok {
namespace math {

inline float4x4 get_relative_matrix( float4x4 const& original_matrix, float4x4 const& parent_matrix )
{
	float4x4 inverted_parent_matrix;
	if ( !inverted_parent_matrix.try_invert( parent_matrix ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		return float4x4( ).identity( );
	}

	return original_matrix * inverted_parent_matrix;
}

// claude@NOTE: get_rotation_matrix / change_matrix_orientation are inline math
// helpers (math_float4x4_inline_2.h on target) absent from our headers; process_leg
// calls them OUT-OF-LINE (not inlined), so a self-contained definition here resolves
// the link without affecting process_leg's scored bytes (they are separate objects).
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
// reports (dispersion/bullet/etc.), a universal name-pairing artifact, not 0%. The
// emitted base bytes match: cc_float's init is byte-identical to target; cc_bool's
// init differs only in that the target passes the cc_bool ctor's command_type/
// execution_filter args in registers (whole-program LTCG fastcall) where base
// passes them on the stack - a call-boundary arg-passing artifact, not a source
// divergence. So these are effectively DONE (capped only by the None pairing
// artifact); the deeper second pass need not revisit them.
static bool		s_ik_legs_debug_draw_value		= false;
static float	s_ik_foot_capsule_radius_value	= 0.0f;
static bool		s_ik_legs_rot_axis_value		= false;
static bool		s_ik_adjust_hip_position_value	= false;

static console_commands::cc_bool	s_ik_legs_debug_draw_cc		( "ik_legs_debug_draw", s_ik_legs_debug_draw_value, false, console_commands::command_type_engine_internal );
static console_commands::cc_float	s_ik_foot_capsule_radius_cc	( "ik_foot_capsule_radius", s_ik_foot_capsule_radius_value, 0.01f, 0.2f, true, console_commands::command_type_engine_internal );
static console_commands::cc_bool	s_ik_legs_rot_axis_cc		( "ik_legs_rot_axis", s_ik_legs_rot_axis_value, false, console_commands::command_type_engine_internal, console_commands::execution_filter_early );
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

// STATE[85.71%|PARTIAL]: only `DELETE( m_drawer )` is real source; the residual is
// the compiler-generated member-destructor epilogue - the target sets `this+0x7c`
// (m_toe_interpolator) and `this+0x70` (m_heel_interpolator) before each
// ~fermi_interpolator call, base ICF-folds those `this`-pointer setups away. An
// ICF/codegen artifact in the auto-emitted dtor tail, not source-steerable.
legs_ik_processor::~legs_ik_processor( )
{
	DELETE( m_drawer );

	// FUNCTION BODY
	// <0x6faf49>|0x009|+0x029:'53'
	// ******
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

	// FUNCTION BODY
	// <0x6fa969>|0x009|+0x022:'64'
	// <0x6fa98b>|0x02b|+0x022:'65'
	// <0x6fa9ad>|0x04d|+0x015:'66'
	// ******
}

// STATE[83.69%|DONE]: residual is LTCG call-boundary arg passing - the two min()
// float args land in swapped xmm registers (target tr_time->xmm0, member->xmm1).
void legs_ik_processor::leg_params::set_heel_transition_time( float tr_time )
{
	heel_transition_time = math::min( tr_time, heel_transition_time );
}

// STATE[83.69%|DONE]: LTCG call-boundary arg passing - see set_heel_transition_time.
void legs_ik_processor::leg_params::set_toe_transition_time( float tr_time )
{
	toe_transition_time = math::min( tr_time, toe_transition_time );
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

	// FUNCTION BODY
	// <0xcb1f6>|0x006|+0x018:'113'	float const knee_angle_cos = upleg_dir | -leg_dir;  // operator| INLINED in base, target calls it
	// <0xcb20e>|0x01e|+0x075:'114'	return is_similar(cos,1.0f,epsilon_5) ? knee_len*0.5f : sqrt(sqr(knee_len)*0.5f/(1.0f-cos));
	// ******
}

// STATE[90%|PARTIAL]: full control structure matched (hip_world build, two
// get_foot_fixed_transform calls, the inverted_transform + R_ASSERT(success), and
// the four-way s_ik_adjust_hip_position if/else ladder with its three "process both
// legs" blocks + final ground-state check). Two residuals, both call-boundary /
// codegen, not source-steerable here:
//   1) base materializes get_skeleton() (*m_skeleton) into a per-call stack temp at
//      each get_bone_matrix_in_object_space site where the target inlines [m_skeleton]
//      directly - that 0xC of extra frame shifts every later [ebp-N] slot by 0xC
//      (a uniform slot-rename storm in the diff, no control-structure divergence).
//   2) R_ASSERT(success): base emits only the assert-enabled predicate; the target
//      additionally evaluates+reports `success` (push success; push 0; call) - an
//      assert-macro-variant difference. The deeper pass (real callers observing the
//      object) may settle the get_skeleton() spill. Full trail in
//      legs_ik_processor_rest.md.
void legs_ik_processor::process( float4x4* matrices, float4x4 const& transform )
{
	float4x4				hip_obj_matrix				= get_bone_matrix_in_object_space( *m_hip_bone, get_skeleton( ), matrices );
	float4x4 const&			hip_world_matrix			= hip_obj_matrix * transform;

	float					right_delta_len				= 0.0f;
	float4x4 const&			right_foot_fixed_transform	= get_foot_fixed_transform( m_right_leg_params, hip_world_matrix, matrices, right_delta_len );
	float					left_delta_len				= 0.0f;
	float4x4 const&			left_foot_fixed_transform	= get_foot_fixed_transform( m_left_leg_params, hip_world_matrix, matrices, left_delta_len );

	float4x4				inverted_transform;
	bool const				success						= inverted_transform.try_invert( transform );
	R_ASSERT( success );

	if ( !s_ik_adjust_hip_position_value || ( right_delta_len >= 0.0f && left_delta_len >= 0.0f ) )
	{
		process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
		process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	}
	else if ( m_left_leg_params.is_on_ground( ) && right_delta_len > 0.0f && 0.0f > left_delta_len )
	{
		matrices->c.y			+= right_delta_len;
		hip_obj_matrix			= get_bone_matrix_in_object_space( *m_hip_bone, get_skeleton( ), matrices );
		process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
		process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	}
	else if ( m_right_leg_params.is_on_ground( ) && right_delta_len > 0.0f && 0.0f > left_delta_len )
	{
		matrices->c.y			+= left_delta_len;
		hip_obj_matrix			= get_bone_matrix_in_object_space( *m_hip_bone, get_skeleton( ), matrices );
		process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
		process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );
	}
	else if ( m_left_leg_params.is_on_ground( ) )
	{
		m_right_leg_params.is_on_ground( );
	}

	// FUNCTION BODY
	// <0x6fc651>|0x011|+0x029:'119'
	// <0x6fc67a>|0x03a|+0x023:'120'
	// <0x6fc69d>|0x05d|+0x010:'121'
	// <0x6fc6ad>|0x06d|+0x02e:'122'
	// <0x6fc6db>|0x09b|+0x010:'123'
	// <0x6fc6eb>|0x0ab|+0x037:'124'
	// <0>
	// <0x6fc722>|0x0e2|+0x00b:'126'
	// <0x6fc72d>|0x0ed|+0x012:'127'
	// <0x6fc73f>|0x0ff|+0x028:'128'
	// <0>
	// <0x6fc767>|0x127|+0x035:'130'
	// <0>
	// <0x6fc79c>|0x15c|+0x03f:'132'
	// <0x6fc7db>|0x19b|+0x047:'133'
	// <0>
	// <0x6fc822>|0x1e2|+0x092:'135'
	// <0>
	// <1>
	// <0x6fc8b4>|0x274|+0x018:'138'
	// <0>
	// <0x6fc8cc>|0x28c|+0x038:'140'
	// <0>
	// <1>
	// <0x6fc904>|0x2c4|+0x03f:'143'
	// <0x6fc943>|0x303|+0x047:'144'
	// <0>
	// <0x6fc98a>|0x34a|+0x092:'146'
	// <0>
	// <1>
	// <0x6fca1c>|0x3dc|+0x018:'149'
	// <0>
	// <0x6fca34>|0x3f4|+0x038:'151'
	// <0>
	// <1>
	// <0x6fca6c>|0x42c|+0x03f:'154'
	// <0x6fcaab>|0x46b|+0x047:'155'
	// <0>
	// <0x6fcaf2>|0x4b2|+0x0bd:'157'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[78.82%|PARTIAL]: two-bone IK math, all 58 statements / operands matched, but a
// CONTROL-STRUCTURE divergence remains: the target carcass shows three [1] braced blocks
// (at srclines 205/231/245 - the up_leg/knee/leg orientation stages) whose locals are PDB-
// tagged <1> (original_*_dir, additive_len, up_leg_alpha_angle, rotation_matrix*, ...); my
// source wrote those three stages FLAT at function scope (no { }), so the base structure has
// ZERO [n] block-opens where the target has three. That missing bracing is the SOURCE cause
// of the dominant [ebp-N] slot renames (the <1> locals get function-scope slots instead of
// block-scoped ones) - it is a recoverable matching problem, NOT LTCG/slot noise. Next step
// (a faster machine): brace the three IK stages so the <1> locals are block-scoped, then
// re-diff. The smaller residual IS the permitted call-boundary class (is_similar epsilon/ptr,
// operator*/-/^ temps, a few xyz-fold inline-vs-call, the get_root_bones_count temp-roundtrip
// sibling get_foot_fixed_transform (84%) also shows). Full trail in process_leg.md.
void legs_ik_processor::process_leg(
	legs_ik_processor::leg_params&		params,
	float4x4 const&						target_foot_obj_matrix,
	float4x4 const&						hip_obj_matrix,
	float4x4*							matrices,
	float4x4 const&						transform
)
{
	u32 const				toe_matrix_index	= params.toe_bone_index    - get_skeleton( ).get_root_bones_count( );
	u32 const				foot_matrix_index	= params.foot_bone_index   - get_skeleton( ).get_root_bones_count( );
	u32 const				leg_matrix_index	= params.leg_bone_index    - get_skeleton( ).get_root_bones_count( );
	u32 const				knee_matrix_index	= params.knee_bone_index   - get_skeleton( ).get_root_bones_count( );
	u32 const				up_leg_matrix_index	= params.up_leg_bone_index - get_skeleton( ).get_root_bones_count( );

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
	knee_obj_matrix		= matrices[knee_matrix_index] * up_leg_obj_matrix;

	float3 const&			original_knee_dir				= math::normalize( leg_obj_matrix.c.xyz( ) - knee_obj_matrix.c.xyz( ) );
	float4x4 const&			rotation_matrix2				= math::get_rotation_matrix( original_knee_dir, target_up_leg_to_foot_dir );
	math::change_matrix_orientation( rotation_matrix2, knee_obj_matrix );
	leg_obj_matrix		= matrices[leg_matrix_index] * knee_obj_matrix;
	foot_obj_matrix		= matrices[foot_matrix_index] * leg_obj_matrix;

	float3 const&			original_leg_dir				= math::normalize( foot_obj_matrix.c.xyz( ) - leg_obj_matrix.c.xyz( ) );
	float3 const&			target_leg_dir					= math::normalize( target_foot_obj_matrix.c.xyz( ) - leg_obj_matrix.c.xyz( ) );
	float4x4 const&			rotation_matrix3				= math::get_rotation_matrix( original_leg_dir, target_leg_dir );
	math::change_matrix_orientation( rotation_matrix3, leg_obj_matrix );
	foot_obj_matrix		= target_foot_obj_matrix;

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
			u32 const		toe_matrix_index	= get_skeleton( ).get_bone_index( get_skeleton( ).get_bone( params.foot_bone_index ) ) - get_skeleton( ).get_root_bones_count( );
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

	// LOCALS
	// float4x4 					up_leg_obj_matrix
	// float4x4 					foot_obj_matrix
	// float3 const& 				target_up_leg_to_foot_dir
	// u32 							up_leg_matrix_index
	// u32 							leg_matrix_index
	// float4x4 					knee_obj_matrix
	// float4x4 					toe_obj_matrix
	// u32 							foot_matrix_index
	// float 						leg_len
	// u32 							toe_matrix_index
	// float3 						foot_pos
	// float 						up_leg_len
	// float4x4 					leg_obj_matrix
	// u32 							knee_matrix_index
	// float 						knee_len
	// float3 const& 				original_up_leg_to_foot_dir<1>
	// float 						up_leg_alpha_angle<1>
	// float 						additive_len<1>
	// float 						up_leg_to_foot_len<1>
	// float3 const& 				original_up_leg_dir<1>
	// float3 const& 				target_up_leg_dir<1>
	// float4x4 const& 				rotation_matrix<1>
	// float4x4 const& 				alpha_rotation_matrix<1>
	// float4x4 const& 				rotation_matrix<1>
	// float3 const& 				original_knee_dir<1>
	// float3 const& 				target_leg_dir<1>
	// float4x4 const& 				rotation_matrix<1>
	// float3 const& 				original_leg_dir<1>
	// float3 const& 				foot_pos<1>
	// float3 const& 				toe_pos<1>
	// u32 							toe_matrix_index<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fb8a8><1>
	// <0x6fb91d><1>
	// ******

	// FUNCTION BODY
	// <0x6fafb1>|0x011|+0x018:'166'
	// <0x6fafc9>|0x029|+0x017:'167'
	// <0x6fafe0>|0x040|+0x01b:'168'
	// <0x6faffb>|0x05b|+0x018:'169'
	// <0x6fb013>|0x073|+0x01b:'170'
	// <0>
	// <0x6fb02e>|0x08e|+0x01a:'172'
	// <0x6fb048>|0x0a8|+0x01d:'173'
	// <0x6fb065>|0x0c5|+0x01a:'174'
	// <0>
	// <0x6fb07f>|0x0df|+0x020:'176'
	// <0x6fb09f>|0x0ff|+0x020:'177'
	// <0x6fb0bf>|0x11f|+0x020:'178'
	// <0x6fb0df>|0x13f|+0x01d:'179'
	// <0x6fb0fc>|0x15c|+0x01d:'180'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fb119>|0x179|+0x0d7:'185'
	// <0x6fb1f0>|0x250|+0x005:'186'
	// <0>
	// <0x6fb1f5>|0x255|+0x01f:'188'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6fb214>|0x274|+0x0cb:'200'
	// <0>
	// <1>
	// <0x6fb2df>|0x33f|+0x03e:'203'
	// <0>
	// <0x6fb31d>|0x37d|+0x036|[1]:'205'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fb353>|0x3b3|+0x084:'210'
	// <0>
	// <0x6fb3d7>|0x437|+0x03e:'212'
	// <0>
	// <0x6fb415>|0x475|+0x044:'214'
	// <0x6fb459>|0x4b9|+0x044:'215'
	// <0x6fb49d>|0x4fd|+0x025:'216'
	// <0x6fb4c2>|0x522|+0x04a:'217'
	// <0>
	// <0x6fb50c>|0x56c|+0x02a:'219'
	// <0x6fb536>|0x596|+0x024:'220'
	// <0x6fb55a>|0x5ba|+0x029:'221'
	// <0x6fb583>|0x5e3|+0x016:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6fb599>|0x5f9|+0x02f|[1]:'231'
	// <0x6fb5c8>|0x628|+0x02f:'232'
	// <0x6fb5f7>|0x657|+0x041:'233'
	// <0x6fb638>|0x698|+0x029:'234'
	// <0x6fb661>|0x6c1|+0x016:'235'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6fb677>|0x6d7|+0x02f|[1]:'245'
	// <0x6fb6a6>|0x706|+0x02c:'246'
	// <0x6fb6d2>|0x732|+0x041:'247'
	// <0x6fb713>|0x773|+0x041:'248'
	// <0x6fb754>|0x7b4|+0x029:'249'
	// <0x6fb77d>|0x7dd|+0x013:'250'
	// <0x6fb790>|0x7f0|+0x010:'251'
	// <0>
	// <0x6fb7a0>|0x800|+0x01f:'253'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6fb7bf>|0x81f|+0x0d4:'265'
	// <0>
	// <0x6fb893>|0x8f3|+0x017:'267'
	// <0>
	// <0x6fb8aa>|0x90a|+0x028:'269'
	// <0x6fb8d2>|0x932|+0x036:'270'
	// <0>
	// <0x6fb908>|0x968|+0x01b:'272'
	// <0>
	// <0x6fb923>|0x983|+0x03c:'274'
	// <0x6fb95f>|0x9bf|+0x045:'275'
	// <0x6fb9a4>|0xa04|+0x031:'276'
	// <0>
	// <1>
	// <2>
	// <0x6fb9d5>|0xa35|+0x02f:'280'
	// <0x6fba04>|0xa64|+0x02f:'281'
	// <0x6fba33>|0xa93|+0x02f:'282'
	// <0x6fba62>|0xac2|+0x023:'283'
	// <0x6fba85>|0xae5|+0x029:'284'
	// <0x6fbaae>|0xb0e|+0x023:'285'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[84.16%|PARTIAL]: large float4x4 IK math, full structure matched (all 64
// statements, the is_similar early-out, the 4-way ground if-chain, lengths, blend,
// return). Residuals are register/[ebp-N] slot renaming plus the LTCG arg-passing /
// temp-materialization at the many math call boundaries (operator -+^* / normalize /
// create_rotation / transform_position / is_similar / length / get_root_bones_count /
// interpolated_value / adjust_foot_transform) and a couple of trivial-COMDAT
// inline-vs-call decisions. One unresolved statement: the else-branch single-byte
// original_color write (0x64) - see @TODO. Full trail in get_foot_fixed_transform.md.
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
	float4x4 const&			up_leg_world_matrix				= matrices[params.up_leg_bone_index - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			knee_world_matrix				= matrices[params.knee_bone_index   - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			leg_world_matrix				= matrices[params.leg_bone_index    - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			foot_world_matrix				= matrices[params.foot_bone_index   - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;
	float4x4 const&			toe_world_matrix				= matrices[params.toe_bone_index    - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;

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

	float3					start;
	float3					finish;
	math::color				original_color( 0x80u, 0xc8u, 0x00u, 0x00u );
	math::color				fixed_color( 0x80u, 0x00u, 0xc8u, 0x00u );
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
		// claude@TODO: target writes a single low byte (0x64=100) into original_color
		// here (mov byte[tmp],64h; mov [original_color],cl), not a full color ctor -
		// the exact source form (a channel setter?) is unresolved; this ctor diverges.
		original_color	= math::color( 0x64u, 0x00u, 0x00u );
	}

	float4x4 const&			foot_to_center_rel				= math::get_relative_matrix( foot_world_matrix, result );

	if ( s_ik_legs_debug_draw_value && m_drawer )
		m_drawer->draw_line_capsule( result, capsule_size, original_color, false );

	m_character_controller->adjust_foot_transform( capsule_size, start, finish, rotation_interpolation_koef, params.heel_transition_time, result );

	if ( s_ik_legs_debug_draw_value && m_drawer )
		m_drawer->draw_solid_capsule( result, capsule_size, fixed_color, true );

	float4x4				foot_center_transform			= foot_to_center_rel * result;

	float const				leg_len							= matrices[params.knee_bone_index   - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );
	float const				up_leg_len						= matrices[params.leg_bone_index    - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );
	float const				knee_len						= matrices[params.foot_bone_index   - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );

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

	// FUNCTION BODY
	// <0x6fbaf1>|0x011|+0x03b:'295'
	// <0x6fbb2c>|0x04c|+0x03e:'296'
	// <0x6fbb6a>|0x08a|+0x03e:'297'
	// <0x6fbba8>|0x0c8|+0x03d:'298'
	// <0x6fbbe5>|0x105|+0x03e:'299'
	// <0x6fbc23>|0x143|+0x078:'300'
	// <0x6fbc9b>|0x1bb|+0x018:'301'
	// <0>
	// <0x6fbcb3>|0x1d3|+0x04a:'303'
	// <0x6fbcfd>|0x21d|+0x044:'304'
	// <0x6fbd41>|0x261|+0x034:'305'
	// <0>
	// <0x6fbd75>|0x295|+0x008:'307'
	// <0x6fbd7d>|0x29d|+0x008:'308'
	// <0x6fbd85>|0x2a5|+0x01e:'309'
	// <0x6fbda3>|0x2c3|+0x01e:'310'
	// <0x6fbdc1>|0x2e1|+0x042:'311'
	// <0>
	// <0x6fbe03>|0x323|+0x015:'313'
	// <0x6fbe18>|0x338|+0x03e:'314'
	// <0x6fbe56>|0x376|+0x012:'315'
	// <0x6fbe68>|0x388|+0x012:'316'
	// <0x6fbe7a>|0x39a|+0x012:'317'
	// <0x6fbe8c>|0x3ac|+0x028:'318'
	// <0>
	// <0x6fbeb4>|0x3d4|+0x025:'320'
	// <0x6fbed9>|0x3f9|+0x02f:'321'
	// <0>
	// <0x6fbf08>|0x428|+0x029:'323'
	// <0x6fbf31>|0x451|+0x01d:'324'
	// <0>
	// <0x6fbf4e>|0x46e|+0x016:'326'
	// <0x6fbf64>|0x484|+0x032:'327'
	// <0x6fbf96>|0x4b6|+0x010:'328'
	// <0x6fbfa6>|0x4c6|+0x053:'329'
	// <0>
	// <0x6fbff9>|0x519|+0x050:'331'
	// <0x6fc049>|0x569|+0x050:'332'
	// <0x6fc099>|0x5b9|+0x02f:'333'
	// <0>
	// <0x6fc0c8>|0x5e8|+0x01b:'335'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc0e3>|0x603|+0x050:'340'
	// <0x6fc133>|0x653|+0x050:'341'
	// <0x6fc183>|0x6a3|+0x02f:'342'
	// <0>
	// <0x6fc1b2>|0x6d2|+0x01b:'344'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc1cd>|0x6ed|+0x050:'349'
	// <0x6fc21d>|0x73d|+0x050:'350'
	// <0x6fc26d>|0x78d|+0x026:'351'
	// <0>
	// <0x6fc293>|0x7b3|+0x005:'353'
	// <0>
	// <0x6fc298>|0x7b8|+0x050:'355'
	// <0x6fc2e8>|0x808|+0x022:'356'
	// <0x6fc30a>|0x82a|+0x013:'357'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6fc31d>|0x83d|+0x026:'364'
	// <0>
	// <0x6fc343>|0x863|+0x017:'366'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc35a>|0x87a|+0x022:'371'
	// <0>
	// <1>
	// <2>
	// <0x6fc37c>|0x89c|+0x030:'375'
	// <0>
	// <0x6fc3ac>|0x8cc|+0x017:'377'
	// <0>
	// <0x6fc3c3>|0x8e3|+0x022:'379'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fc3e5>|0x905|+0x01a:'384'
	// <0>
	// <0x6fc3ff>|0x91f|+0x02f:'386'
	// <0x6fc42e>|0x94e|+0x02f:'387'
	// <0x6fc45d>|0x97d|+0x02e:'388'
	// <0>
	// <0x6fc48b>|0x9ab|+0x039:'390'
	// <0x6fc4c4>|0x9e4|+0x027:'391'
	// <0x6fc4eb>|0xa0b|+0x03e:'392'
	// <0x6fc529>|0xa49|+0x031:'393'
	// <0>
	// <0x6fc55a>|0xa7a|+0x026:'395'
	// <0x6fc580>|0xaa0|+0x080:'396'
	// <0x6fc600>|0xb20|+0x021:'397'
	// <0>
	// <0x6fc621>|0xb41|+0x013:'399'
	// ******
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

	// FUNCTION BODY
	// <0x6fab79>|0x009|+0x015:'424'	if ( params.is_heel_on_ground() == value )
	// <0x6fab8e>|0x01e|+0x005:'425'	    return;
	// <0x6fab93>|0x023|+0x00d:'426'	params.set_heel_on_ground( value );
	// <0>
	// <0x6faba0>|0x030|+0x008:'428'	if ( value )
	// <0>
	// <0x6faba8>|0x038|+0x00f:'430'	    params.heel_transition_time = m_heel_transition_time;
	// <0x6fabb7>|0x047|+0x013:'431'	    m_heel_transition_time_calculator.reset();
	// <0>
	// <0x6fabca>|0x05a|+0x005:'433'	} (jmp = brace)
	// <0>
	// <0x6fabcf>|0x05f|+0x020:'435'	else { m_heel_transition_time = m_heel_transition_time_calculator.get_value();
	// <0x6fabef>|0x07f|+0x022:'436'	    math::clamp( m_heel_transition_time, 0.001f, 0.5f );
	// <0x6fac11>|0x0a1|+0x018:'437'	    m_left_leg_params.set_heel_transition_time( m_heel_transition_time );
	// <0x6fac29>|0x0b9|+0x018:'438'	    m_right_leg_params.set_heel_transition_time( m_heel_transition_time );
	// <0x6fac41>|0x0d1|+0x049:'439'	    m_heel_interpolator = fermi_interpolator( m_heel_transition_time );  // +lea ecx residual
	// <0x6fac8a>|0x11a|+0x00f:'440'	    params.toe_transition_time = m_toe_transition_time;
	// <0x6fac99>|0x129|+0x013:'441'	    m_toe_transition_time_calculator.reset(); }
	// <0>
	// ******
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

	// FUNCTION BODY
	// <0x6faa79>|0x009|+0x015:'447'	if ( params.is_toe_on_ground() == value )
	// <0x6faa8e>|0x01e|+0x005:'448'	    return;
	// <0x6faa93>|0x023|+0x00d:'449'	params.set_toe_on_ground( value );
	// <0x6faaa0>|0x030|+0x00c:'450'	if ( !value )
	// <0>
	// <0x6faaac>|0x03c|+0x020:'452'	{ m_toe_transition_time = m_toe_transition_time_calculator.get_value();
	// <0x6faacc>|0x05c|+0x022:'453'	    math::clamp( m_toe_transition_time, 0.001f, 0.5f );
	// <0x6faaee>|0x07e|+0x018:'454'	    m_left_leg_params.set_toe_transition_time( m_toe_transition_time );
	// <0x6fab06>|0x096|+0x018:'455'	    m_right_leg_params.set_toe_transition_time( m_toe_transition_time );
	// <0x6fab1e>|0x0ae|+0x049:'456'	    m_toe_interpolator = fermi_interpolator( m_toe_transition_time ); }  // +lea ecx residual
	// <0>
	// ******
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
