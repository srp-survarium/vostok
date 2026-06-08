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

	// STRUCTURE DIFF:
	// target: 0x6eaf40            base: 0x522c50
	// ; void survarium::legs_ik_processor::~legs_ik_processor() ; target 1 stmts / base 1 stmts
	// .. same ..
	// ; aligned 1, size-diffs 0, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - the sole source stmt (the m_drawer delete) matches; the <100% residual is the compiler-generated member-dtor epilogue: target sets this+0x7Ch (m_toe_interpolator) and this+0x70h (m_heel_interpolator) before each ~fermi_interpolator call, base ICF-folds those this-pointer setups away. Codegen in the auto-emitted dtor tail, non-steerable. trail: legs_ik_processor_rest.md
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

	// STRUCTURE DIFF:
	// target: 0x6ea960            base: 0x522150
	// ; void survarium::legs_ik_processor::leg_params::tick(float) ; target 3 stmts / base 3 stmts
	// .. same ..
	// ; aligned 3, size-diffs 0, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - all 3 stmts/operands byte-exact; residual is a single extra 4-byte frame slot (target sub esp,8 / this at [ebp-8]; base push ecx / this at [ebp-4]), an LTCG frame-layout artifact (a phantom temp at the math::max call boundary). trail: legs_ik_processor_rest.md
}

// STATE[100%|DONE]
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

	// STRUCTURE DIFF:
	// target: 0xbb1f0            base: 0x5228a0
	// ; float survarium::get_additional_length(vostok::math::float3 const&, vostok::math::float3 const&, float) ; target 2 stmts / base 3 stmts
	// 0x006 <0x18> | 0x006 <0x49> | float const knee_angle_cos	= upleg_dir | -leg_dir;   SIZE
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// ; aligned 1, size-diffs 1, quantity-diffs 1
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is operator| out-of-line call vs our /Ob2-inlined COMDAT; proven NON-steerable on #144 (3 source forms all inlined)  trail: get_additional_length.md
}

// STATE[90%|PARTIAL]: control flow VERIFIED one-to-one against the target asm (the
// four-branch s_ik_adjust_hip_position ladder: both-legs / +=right_delta when left
// is_on_ground / +=left_delta when right is_on_ground / left.is_on_ground()?
// right.is_on_ground()); the operand-aware byte diff is 100% `~` register/slot renames
// with ZERO missing-or-extra instructions, so there is NO structure bug (the
// structure-diff's ONLY-base rows are an LCS aligner mis-pairing of the four
// textually-identical process_leg blocks - see claude@NOTE on the diff). Sole residual
// is the m_hip_bone pointer spill: base materializes `*m_hip_bone` into a stack temp
// before each get_bone_matrix_in_object_space call where the target uses it inline,
// giving a 0x4-bigger frame (sub esp 3C0h vs 3BCh) that renames every later [ebp-N]
// slot. A call-boundary LTCG spill, not source-steerable. Trail in
// legs_ik_processor_rest.md.
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
	R_ASSERT_U( success );

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

	// STRUCTURE DIFF:
	// target: 0x6ec640            base: 0x525ca0
	// ; void survarium::legs_ik_processor::process(vostok::math::float4x4*, vostok::math::float4x4 const&) ; target 35 stmts / base 34 stmts
	// 0x011 <0x29> | 0x011 <0x35> | float4x4				hip_obj_matrix				= get_bone_matrix_in_object_space( *m_hip_bone, get_skeleton( ), matrices );   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x0ab <0x37> | 0x0b7 <0x34> | float4x4 const&			left_foot_fixed_transform	= get_foot_fixed_transform( m_left_leg_params, hip_world_matrix, matrices, left_delta_len );   SIZE
	// .. same ..
	// 0x0ff <0x28> | 0x108 <0x12> | R_ASSERT( success );   SIZE
	// .. same ..
	// 0x19b <0x47> | 0x18e <0x44> | process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );   SIZE
	// .. same ..
	// 0x28c <0x38> | 0x27c <0x44> | hip_obj_matrix			= get_bone_matrix_in_object_space( *m_hip_bone, get_skeleton( ), matrices );   SIZE
	// <0>         | --          |    EMPTY only target
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// 0x303 <0x47> | 0x2ff <0x44> | process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );   SIZE
	// .. same ..
	// --          | 0x3ed <0x44> | hip_obj_matrix			= get_bone_matrix_in_object_space( *m_hip_bone, get_skeleton( ), matrices );   ONLY base
	// --          | 0x431 <0x3f> | process_leg( m_left_leg_params, right_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );   ONLY base
	// --          | 0x470 <0x44> | process_leg( m_right_leg_params, left_foot_fixed_transform * inverted_transform, hip_obj_matrix, matrices, transform );   ONLY base
	// .. same ..
	// 0x3f4 <0x38> | 0x4b4 <0x64> | else if ( m_left_leg_params.is_on_ground( ) )   SIZE
	// .. same ..
	// 0x42c <0x3f> | 0x518 <0x59> | m_right_leg_params.is_on_ground( );   SIZE
	// 0x46b <0x47> | --          | L155   ONLY target
	// <0>         | --          |    EMPTY only target
	// 0x4b2 <0xbd> | --          | L157   ONLY target
	// ; aligned 22, size-diffs 8, quantity-diffs 9
	// claude@NOTE: the ONLY-base/L155/L157 rows are a structure-diff ALIGNER artifact, NOT a
	// real extra branch (investigated: process()'s four ground branches each emit a textually
	// IDENTICAL process_leg(L)/process_leg(R)/hip_obj_matrix string, so the LCS row-aligner
	// slides them and mis-pairs base's +=right body against target's +=left body). The
	// operand-aware byte diff (pdb_fetch --view diff) shows ZERO `<`/`>` instructions - every
	// diff line is `~` (a register/[ebp-N] slot rename); base and target have the SAME
	// instruction sequence in all four branches. Target/base control flow verified one-to-one
	// from the target asm: .2=both-legs, .5=+=right_delta(L is_on_ground), .8=+=left_delta(R
	// is_on_ground), .9=left.is_on_ground()?right.is_on_ground(). No source change needed.
	// VERDICT: STRUCTURE MATCH (shape ok) - the SIZE/ONLY-base rows are the get_skeleton()/m_hip_bone
	// spill (base spills the pointer to a stack temp where target uses it inline; 0x4 bigger frame,
	// sub esp 3C0h vs 3BCh, renames every later slot) plus the aligner mis-pairing of the three
	// identical-text ground branches. A call-boundary LTCG spill, non-steerable. trail: legs_ik_processor_rest.md
}

// STATE[80.96%|PARTIAL]: two-bone IK math, all 58 statements / operands matched AND the
// control structure now matches: the three target [1] braced IK-stage blocks (srclines
// 205/231/245 - up_leg/knee/leg orientation) are braced here, the knee/leg recompute lands
// as the first statement INSIDE the next block, and the block-scoped <1> locals collapse the
// former [ebp-N] slot-rename storm (78.81 -> 80.96, three [1] block-opens now present in the
// base structure dump). The remaining residual is the permitted non-bracing class: (1) the
// up_leg_obj_matrix dir-math reads a separate [ebp-150h] working slot the target keeps while
// change_matrix_orientation mutates [ebp-180h] - a compiler in/out lowering copy not visible
// as a named local (no second float4x4 in the carcass LOCALS); (2) get_angle inline-vs-call
// (STUB here, the target calls it out-of-line, so the leg_len+additive_len adds inline into
// the call); (3) the get_skeleton()->get_root_bones_count temp-roundtrip sibling
// get_foot_fixed_transform also shows; (4) is_similar epsilon/ptr + operator*/-/^ temps.
// Full trail in process_leg.md.
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

	// STRUCTURE DIFF:
	// target: 0x6eafa0            base: 0x514e30
	// ; void survarium::legs_ik_processor::process_leg(survarium::legs_ik_processor::leg_params&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4*, vostok::math::float4x4 const&) ; target 76 stmts / base 79 stmts
	// 0x011 <0x18> | 0x011 <0x24> | u32 const				toe_matrix_index	= params.toe_bone_index    - get_skeleton( ).get_root_bones_count( );   SIZE
	// 0x029 <0x17> | 0x035 <0x23> | u32 const				foot_matrix_index	= params.foot_bone_index   - get_skeleton( ).get_root_bones_count( );   SIZE
	// 0x040 <0x1b> | 0x058 <0x27> | u32 const				leg_matrix_index	= params.leg_bone_index    - get_skeleton( ).get_root_bones_count( );   SIZE
	// 0x05b <0x18> | 0x07f <0x24> | u32 const				knee_matrix_index	= params.knee_bone_index   - get_skeleton( ).get_root_bones_count( );   SIZE
	// 0x073 <0x1b> | 0x0a3 <0x27> | u32 const				up_leg_matrix_index	= params.up_leg_bone_index - get_skeleton( ).get_root_bones_count( );   SIZE
	// .. same ..
	// 0x15c <0x1d> | 0x198 <0x20> | float4x4				toe_obj_matrix		= matrices[toe_matrix_index] * foot_obj_matrix;   SIZE
	// .. same ..
	// 0x179 <0xd7> | 0x1b8 <0x159> | math::is_similar( target_foot_obj_matrix.c.xyz( ), foot_obj_matrix.c.xyz( ), math::epsilon_3 ) )   SIZE
	// .. same ..
	// 0x33f <0x3e> | 0x400 <0x41> | float3 const&			target_up_leg_to_foot_dir		= math::normalize( up_leg_obj_matrix.c.xyz( ) - target_foot_obj_matrix.c.xyz( ) );   SIZE
	// .. same ..
	// 0x3b3 <0x84> | 0x477 <0x8a> | float const				additive_len					= get_additional_length( math::normalize( foot_obj_matrix.c.xyz( ) - up_leg_obj_matrix.c.xyz( ) ), math::normalize( leg_obj_matrix.c.xyz( ) - foot_obj_matrix.c.xyz( ) ), knee_len );   SIZE
	// --          | 0x501 <0x10> | float const				up_leg_alpha_angle				= get_angle( leg_len + additive_len, up_leg_to_foot_len, knee_len + additive_len );   ONLY base
	// .. same ..
	// 0x437 <0x3e> | 0x511 <0x47> | float3 const&			original_up_leg_dir				= math::normalize( foot_obj_matrix.c.xyz( ) - up_leg_obj_matrix.c.xyz( ) );   SIZE
	// --          | 0x558 <0x47> | float3 const&			target_up_leg_dir				= math::normalize( target_foot_obj_matrix.c.xyz( ) - up_leg_obj_matrix.c.xyz( ) );   ONLY base
	// .. same ..
	// 0x475 <0x44> | 0x59f <0x33> | if ( !math::is_similar( target_up_leg_dir, original_up_leg_dir, math::epsilon_3 ) )   SIZE
	// 0x4b9 <0x44> | 0x5d2 <0x41> | params.rotation_axis	= math::normalize( target_up_leg_dir ^ original_up_leg_dir );   SIZE
	// 0x4fd <0x25> | --          | L216   ONLY target
	// 0x522 <0x4a> | --          | L217   ONLY target
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x657 <0x41> | 0x6fe <0x44> | float3 const&			original_knee_dir				= math::normalize( leg_obj_matrix.c.xyz( ) - knee_obj_matrix.c.xyz( ) );   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x732 <0x41> | 0x7dc <0x44> | float3 const&			original_leg_dir				= math::normalize( foot_obj_matrix.c.xyz( ) - leg_obj_matrix.c.xyz( ) );   SIZE
	// 0x773 <0x41> | 0x820 <0x44> | float3 const&			target_leg_dir					= math::normalize( target_foot_obj_matrix.c.xyz( ) - leg_obj_matrix.c.xyz( ) );   SIZE
	// .. same ..
	// 0x81f <0xd4> | 0x8cf <0xd7> | 0.0f );   SIZE
	// .. same ..
	// 0x90a <0x28> | 0x9bd <0x27> | float3 const&	foot_pos	= transform.transform_position( target_foot_obj_matrix.c.xyz( ) );   SIZE
	// 0x932 <0x36> | 0x9e4 <0x38> | m_drawer->draw_cross( foot_pos, s_ik_foot_capsule_radius_value, math::color( 0x00u, 0xFFu, 0x00u, 0x00u ), false );   SIZE
	// .. same ..
	// 0x983 <0x3c> | 0xa37 <0x5b> | u32 const		toe_matrix_index	= get_skeleton( ).get_bone_index( get_skeleton( ).get_bone( params.foot_bone_index ) ) - get_skeleton( ).get_root_bones_count( );   SIZE
	// 0x9bf <0x45> | 0xa92 <0x44> | float3 const&	toe_pos	= transform.transform_position( ( matrices[toe_matrix_index] * target_foot_obj_matrix ).c.xyz( ) );   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0xae5 <0x29> | 0xbb7 <0x2c> | matrices[foot_matrix_index]		= math::get_relative_matrix( foot_obj_matrix, target_foot_obj_matrix );   SIZE
	// .. same ..
	// ; aligned 53, size-diffs 21, quantity-diffs 7
	// VERDICT: STRUCTURE MATCH (shape ok) - the three braced IK-stage blocks are present; all SIZE rows are the get_root_bones_count()/is_similar/operator call-boundary temp-materialization and the get_root_bones_count spill (0xC per matrix index, target inlines it). The up_leg_alpha_angle/target_up_leg_dir ONLY base + L216/L217 ONLY target rows are an aligner swap around the get_angle call (both sides call it out-of-line), not a missing statement. trail: process_leg.md
}

// STATE[84.65%|PARTIAL]: large float4x4 IK math, full structure matched (all 64
// statements, the is_similar early-out, the 4-way ground if-chain, lengths, blend,
// return). The else-branch single-byte original_color write is now `set_B( 0x64u )`
// (target writes only the low b channel: mov byte[tmp],64h; mov cl,[tmp]; mov
// [original_color],cl) - that closed the prior @TODO (84.23->84.65). Remaining
// residuals are register/[ebp-N] slot renaming plus the LTCG arg-passing /
// temp-materialization at the many math call boundaries (operator -+^* / normalize /
// create_rotation / transform_position / is_similar / length / get_root_bones_count /
// interpolated_value / adjust_foot_transform), the get_root_bones_count() spill (0xC per
// matrix index, target inlines it) and a couple of trivial-COMDAT inline-vs-call
// decisions. Full trail in get_foot_fixed_transform.md.
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

	// STRUCTURE DIFF:
	// target: 0x6ebae0            base: 0x515a40
	// ; vostok::math::float4x4 survarium::legs_ik_processor::get_foot_fixed_transform(survarium::legs_ik_processor::leg_params const&, vostok::math::float4x4 const&, vostok::math::float4x4 const*, float&) const ; target 86 stmts / base 93 stmts
	// 0x011 <0x3b> | 0x011 <0x47> | float4x4 const&			up_leg_world_matrix				= matrices[params.up_leg_bone_index - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;   SIZE
	// 0x04c <0x3e> | 0x058 <0x47> | float4x4 const&			knee_world_matrix				= matrices[params.knee_bone_index   - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;   SIZE
	// 0x08a <0x3e> | 0x09f <0x47> | float4x4 const&			leg_world_matrix				= matrices[params.leg_bone_index    - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;   SIZE
	// 0x0c8 <0x3d> | 0x0e6 <0x46> | float4x4 const&			foot_world_matrix				= matrices[params.foot_bone_index   - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;   SIZE
	// 0x105 <0x3e> | 0x12c <0x47> | float4x4 const&			toe_world_matrix				= matrices[params.toe_bone_index    - get_skeleton( ).get_root_bones_count( )] * hip_world_matrix;   SIZE
	// --          | <0>         |    EMPTY only base
	// 0x143 <0x78> | 0x173 <0xb6> | math::is_similar( foot_world_matrix.c.xyz( ), leg_world_matrix.c.xyz( ) ) )   SIZE
	// .. same ..
	// 0x1d3 <0x4a> | 0x241 <0x4d> | float3 const&			foot_to_leg_dir					= math::normalize( leg_world_matrix.c.xyz( ) - foot_world_matrix.c.xyz( ) );   SIZE
	// 0x21d <0x44> | 0x28e <0x4d> | float3 const&			foot_to_toe_dir					= math::normalize( toe_world_matrix.c.xyz( ) - foot_world_matrix.c.xyz( ) );   SIZE
	// 0x261 <0x34> | 0x2db <0x37> | float3 const&			left_dir						= math::normalize( foot_to_leg_dir ^ foot_to_toe_dir );   SIZE
	// .. same ..
	// 0x295 <0x8> | 0x312 <0xb> | float4x4				result;   SIZE
	// 0x29d <0x8> | 0x31d <0xb> | result.identity( );   SIZE
	// 0x2a5 <0x1e> | 0x328 <0x21> | result.i.xyz( )		= left_dir;   SIZE
	// 0x2c3 <0x1e> | 0x349 <0x21> | result.j.xyz( )		= foot_to_leg_dir;   SIZE
	// 0x2e1 <0x42> | 0x36a <0x48> | result.k.xyz( )		= math::normalize( foot_to_leg_dir ^ left_dir );   SIZE
	// .. same ..
	// 0x338 <0x3e> | 0x3c7 <0x44> | result				= math::create_rotation( left_dir, rotation_angle ) * result;   SIZE
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x3ac <0x28> | 0x441 <0x2b> | result.c.xyz( )		= foot_world_matrix.c.xyz( );   SIZE
	// .. same ..
	// 0x3f9 <0x2f> | 0x491 <0x36> | result.c.xyz( )		= result.transform_position( up_dir );   SIZE
	// .. same ..
	// 0x46e <0x16> | 0x50d <0xb> | float3					start;   SIZE
	// 0x484 <0x32> | 0x518 <0xb> | float3					finish;   SIZE
	// --          | 0x523 <0x19> | math::color				original_color( 0x80u, 0xc8u, 0x00u, 0x00u );   ONLY base
	// --          | 0x53c <0x19> | math::color				fixed_color( 0x80u, 0x00u, 0xc8u, 0x00u );   ONLY base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x519 <0x50> | 0x5b8 <0x4a> | start			= foot_to_cube_center_offset * dist_to_test + finish;   SIZE
	// 0x569 <0x50> | 0x602 <0x4a> | finish			= foot_to_cube_center_offset * dist_to_test - finish;   SIZE
	// .. same ..
	// 0x603 <0x50> | 0x696 <0x4a> | start			= foot_to_cube_center_offset * dist_to_test + finish;   SIZE
	// 0x653 <0x50> | 0x6e0 <0x4a> | finish			= foot_to_cube_center_offset * dist_to_test - finish;   SIZE
	// .. same ..
	// 0x6ed <0x50> | 0x774 <0x4a> | start			= foot_to_cube_center_offset * dist_to_test + finish;   SIZE
	// 0x73d <0x50> | 0x7be <0x4a> | finish			= foot_to_cube_center_offset * dist_to_test - finish;   SIZE
	// .. same ..
	// 0x7b3 <0x5> | 0x82e <0x2> | else   SIZE
	// --          | <0>         |    EMPTY only base
	// --          | 0x830 <0x4a> | start			= foot_to_cube_center_offset * dist_to_test + finish;   ONLY base
	// --          | 0x87a <0x24> | finish			= start;   ONLY base
	// .. same ..
	// 0x7b8 <0x50> | 0x89e <0x10> | original_color.set_B( 0x64u );   SIZE
	// 0x808 <0x22> | --          | L356   ONLY target
	// 0x82a <0x13> | --          | L357   ONLY target
	// .. same ..
	// 0x83d <0x26> | 0x8ae <0x23> | float4x4 const&			foot_to_center_rel				= math::get_relative_matrix( foot_world_matrix, result );   SIZE
	// .. same ..
	// --          | 0x8e8 <0x25> | m_drawer->draw_line_capsule( result, capsule_size, original_color, false );   ONLY base
	// .. same ..
	// 0x87a <0x22> | 0x90d <0x33> | m_character_controller->adjust_foot_transform( capsule_size, start, finish, rotation_interpolation_koef, params.heel_transition_time, result );   SIZE
	// .. same ..
	// 0x89c <0x30> | --          | L375   ONLY target
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// --          | 0x957 <0x25> | m_drawer->draw_solid_capsule( result, capsule_size, fixed_color, true );   ONLY base
	// .. same ..
	// 0x8e3 <0x22> | --          | L379   ONLY target
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// 0x91f <0x2f> | 0x996 <0x3b> | float const				leg_len							= matrices[params.knee_bone_index   - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );   SIZE
	// 0x94e <0x2f> | 0x9d1 <0x3b> | float const				up_leg_len						= matrices[params.leg_bone_index    - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );   SIZE
	// 0x97d <0x2e> | 0xa0c <0x3a> | float const				knee_len						= matrices[params.foot_bone_index   - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );   SIZE
	// .. same ..
	// 0x9ab <0x39> | 0xa46 <0x36> | float const				up_leg_to_fixed_foot_dist		= ( up_leg_world_matrix.c.xyz( ) - foot_center_transform.c.xyz( ) ).length( );   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0xa7a <0x26> | 0xb12 <0x2a> | float const			position_iterpolation_koef		= 1.0f - m_heel_interpolator.interpolated_value( params.heel_transition_time );   SIZE
	// 0xaa0 <0x80> | 0xb3c <0x7d> | float3 const&		position						= foot_world_matrix.c.xyz( ) * position_iterpolation_koef + foot_center_transform.c.xyz( ) * ( 1.0f - position_iterpolation_koef );   SIZE
	// 0xb20 <0x21> | 0xbb9 <0x1e> | foot_center_transform.c.xyz( )	= position;   SIZE
	// .. same ..
	// 0xb41 <0x13> | 0xbd7 <0x10> | return foot_center_transform;   SIZE
	// ; aligned 43, size-diffs 37, quantity-diffs 19
	// VERDICT: STRUCTURE MATCH (shape ok) - all aligned rows are SIZE-only LTCG temp-materialization at math call boundaries plus the get_root_bones_count() spill (0xC per matrix index; target inlines it); the ONLY base/L356/L357 rows are codegen block placement of the original_color/draw_*capsule statements, not a source-shape divergence. else-branch is now set_B( 0x64u ) (single low-byte write, matches target). trail: get_foot_fixed_transform.md
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

	// STRUCTURE DIFF:
	// target: 0x6eab70            base: 0x5226a0
	// ; void survarium::legs_ik_processor::set_heel_on_ground(survarium::legs_ik_processor::leg_params&, bool) ; target 18 stmts / base 19 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x0d1 <0x49> | 0x0d1 <0x46> | m_heel_interpolator = animation::fermi_interpolator( m_heel_transition_time );   SIZE
	// .. same ..
	// ; aligned 17, size-diffs 1, quantity-diffs 1
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the m_heel_interpolator = fermi_interpolator(...) row: target emits lea ecx,[ebp-0Ch] (the this arg of the COMDAT-folded trivial ~fermi_interpolator temp dtor) that base omits - an ICF/LTCG call-boundary arg-passing artifact. trail: legs_ik_processor_rest.md
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

	// STRUCTURE DIFF:
	// target: 0x6eaa70            base: 0x5225a0
	// ; void survarium::legs_ik_processor::set_toe_on_ground(survarium::legs_ik_processor::leg_params&, bool) ; target 10 stmts / base 12 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0x0ae <0x49> | 0x0ae <0x46> | m_toe_interpolator = animation::fermi_interpolator( m_toe_transition_time );   SIZE
	// ; aligned 9, size-diffs 1, quantity-diffs 2
	// VERDICT: STRUCTURE MATCH (shape ok) - same ICF/LTCG residual as set_heel_on_ground: the lone lea ecx,[ebp-0Ch] (this arg of the folded ~fermi_interpolator temp dtor) on the m_toe_interpolator = fermi_interpolator(...) row. trail: legs_ik_processor_rest.md
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
