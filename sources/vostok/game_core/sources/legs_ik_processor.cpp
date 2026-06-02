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

// claude@NOTE: the s_ik_*_cc console-command machinery is still STUB; the dynamic
// initializers above name s_ik_*_cc. get_foot_fixed_transform only READS the backing
// values, so declare them as plain file statics to reproduce the byte/float loads.
// (The cc-registration bytes are a separate, unmatched concern.)
static bool		s_ik_legs_debug_draw_value		= false;
static float	s_ik_foot_capsule_radius_value	= 0.0f;

/*
// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_legs_debug_draw_cc''()
void `dynamic initializer for 's_ik_legs_debug_draw_cc''( )
{
	// FUNCTION BODY
	// <0x7db1c0>|0x000|      :'20'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_foot_capsule_radius_cc''()
void `dynamic initializer for 's_ik_foot_capsule_radius_cc''( )
{
	// FUNCTION BODY
	// <0x7db200>|0x000|      :'23'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_legs_rot_axis_cc''()
void `dynamic initializer for 's_ik_legs_rot_axis_cc''( )
{
	// FUNCTION BODY
	// <0x7db250>|0x000|      :'26'	{
	// ******
}

// STATE[STUB]
// void survarium::`dynamic atexit destructor for 's_ik_legs_debug_draw_cc''()
void `dynamic atexit destructor for 's_ik_legs_debug_draw_cc''( )
{
	// FUNCTION BODY
	// <0x7db290>|0x000|      :'29'	{
	// ******
}
*/

// STATE[STUB]
// survarium::legs_ik_processor::leg_params::leg_params()
legs_ik_processor::leg_params::leg_params( )
{
	// FUNCTION BODY
	// <0x6fa6c0>|0x000|+0x062:'38'	{
	// <0x6fa722>|0x062|      :'39'	}
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::activate(vostok::animation::skeleton const&, char const*)
void legs_ik_processor::leg_params::activate( animation::skeleton const& skeleton, pcstr foot_bone_name )
{
	// LOCALS
	// animation::skeleton_bone const& foot_bone
	// ******

	// FUNCTION BODY
	// <0x6fad89>|0x009|+0x016:'43'
	// <0x6fad9f>|0x01f|+0x02e:'44'
	// <0x6fadcd>|0x04d|+0x03a:'45'
	// <0x6fae07>|0x087|+0x038:'46'
	// <0x6fae3f>|0x0bf|+0x041:'47'
	// <0x6fae80>|0x100|+0x04a:'48'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::~legs_ik_processor()
legs_ik_processor::~legs_ik_processor( )
{
	// FUNCTION BODY
	// <0x6faf49>|0x009|+0x029:'53'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::leg_params::tick(float)
void legs_ik_processor::leg_params::tick( float dt )
{
	// FUNCTION BODY
	// <0x6fa969>|0x009|+0x022:'64'
	// <0x6fa98b>|0x02b|+0x022:'65'
	// <0x6fa9ad>|0x04d|+0x015:'66'
	// ******
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

// STATE[STUB]
// survarium::legs_ik_processor::legs_ik_processor()
// this->m_heel_transition_time = this->m_heel_interpolator.transition_time(&this->m_heel_interpolator);
// this->m_toe_transition_time = this->m_toe_interpolator.transition_time(&this->m_toe_interpolator);
legs_ik_processor::legs_ik_processor( ) : m_heel_interpolator( 0.1f ), m_toe_interpolator( 0.1f )
{
	// CALL SITE INFO
	// <0x6fa7ea> -> float <unknown>() const
	// <0x6fa804> -> float <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x6fa730>|0x000|+0x0df:'100'	{
	// <0x6fa80f>|0x0df|      :'101'	}
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_processor::activate(vostok::animation::skeleton const&)
void legs_ik_processor::activate( animation::skeleton const& skeleton )
{
	// FUNCTION BODY
	// <0x6faed9>|0x009|+0x00c:'105'
	// <0x6faee5>|0x015|+0x014:'106'
	// <0x6faef9>|0x029|+0x014:'107'
	// <0x6faf0d>|0x03d|+0x01e:'108'
	// ******
}

// STATE[65.38%|PARTIAL]: sole residual is the dot-product - operator| did NOT inline in
// the target (out-of-line call) but our /Ob2 build inlines it; not steerable from the
// caller. Trail: docs/binary_matching/game_core/get_additional_length.md.
float get_additional_length( float3 const& upleg_dir, float3 const& leg_dir, float knee_len )
{
	float const knee_angle_cos	= upleg_dir | -leg_dir;
	return math::is_similar( knee_angle_cos, 1.0f, math::epsilon_5 )
		? knee_len * 0.5f
		: math::sqrt( math::sqr( knee_len ) * 0.5f / ( 1.0f - knee_angle_cos ) );

	// STRUCTURE DIFF:
	// target: 0xbb1f0            base: 0x513fa0
	// ; float survarium::get_additional_length(vostok::math::float3 const&, vostok::math::float3 const&, float) ; target 2 stmts / base 3 stmts
	// 0x006 <0x18> | 0x006 <0x49> | float const knee_angle_cos	= upleg_dir | -leg_dir;   SIZE
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// ; aligned 1, size-diffs 1, quantity-diffs 1
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is operator| out-of-line call vs our /Ob2-inlined COMDAT; proven NON-steerable on #144 (3 source forms all inlined)  trail: get_additional_length.md
}

// STATE[STUB]
// void survarium::legs_ik_processor::process(vostok::math::float4x4*, vostok::math::float4x4 const&)
void legs_ik_processor::process( float4x4* matrices, float4x4 const& transform )
{
	// LOCALS
	// float4x4 					inverted_transform
	// float 						right_delta_len
	// float 						left_delta_len
	// float4x4 					hip_obj_matrix
	// float4x4 const& 				right_foot_fixed_transform
	// float4x4 const& 				hip_world_matrix
	// bool 						success
	// float4x4 const& 				left_foot_fixed_transform
	// ******

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

// STATE[78.82%|PARTIAL]: large two-bone IK math, full structure matched (all 58
// statements, the 4-row is_similar early-out + return, both debug-draw blocks with
// draw_leg/draw_cross/draw_origin, the alpha-angle chain
// get_additional_length/get_angle/create_rotation/transform_direction/get_rotation_matrix/
// change_matrix_orientation rebuilding up_leg/knee/leg orientations, and the
// get_relative_matrix writeback tail with foot-row save/restore). All IK operands
// verified against the asm (up_leg_to_foot_len has NO normalize and uses target_foot;
// additive_len's two dir args ARE normalized; get_angle = (leg_len+add, up_leg_to_foot_len,
// knee_len+add); params.rotation_axis (leg_params+0x1C) is the rotation axis, not a
// matrix row; toe debug uses params.foot_bone_index). Residual is the LTCG/slot class:
// (a) the dominant 447 register/[ebp-N] slot renames - my local-declaration order yields
// a different /Od stack layout than the target's PDB order (reordering to match is the
// unconverged deep tail); (b) call-boundary arg passing (is_similar epsilon/ptr,
// operator*/-/^ temps) and a few xyz-fold inline-vs-call. The get_skeleton()->
// get_root_bones_count temp-roundtrip is the same artifact sibling get_foot_fixed_transform
// (84%) shows. Full trail in process_leg.md.
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

// STATE[84.66%|PARTIAL]: large float4x4 IK math, full structure matched (all 64
// statements, the is_similar early-out, the 4-way ground if-chain, lengths, blend,
// return). Residuals are register/[ebp-N] slot renaming plus the LTCG arg-passing /
// temp-materialization at the many math call boundaries (operator -+^* / normalize /
// create_rotation / transform_position / is_similar / length / get_root_bones_count /
// interpolated_value / adjust_foot_transform) and a couple of trivial-COMDAT
// inline-vs-call decisions. Full trail in get_foot_fixed_transform.md.
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
	// target: 0x6ebae0            base: 0x514fb0
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
	// 0x7b8 <0x50> | 0x833 <0x4a> | start			= foot_to_cube_center_offset * dist_to_test + finish;   SIZE
	// 0x808 <0x22> | 0x87d <0x24> | finish			= start;   SIZE
	// --          | <0>         |    EMPTY only base
	// 0x82a <0x13> | 0x8a1 <0x1c> | original_color	= math::color( 0x64u, 0x00u, 0x00u );   SIZE
	// .. same ..
	// 0x83d <0x26> | 0x8bd <0x23> | float4x4 const&			foot_to_center_rel				= math::get_relative_matrix( foot_world_matrix, result );   SIZE
	// .. same ..
	// --          | 0x8f7 <0x25> | m_drawer->draw_line_capsule( result, capsule_size, original_color, false );   ONLY base
	// .. same ..
	// 0x87a <0x22> | 0x91c <0x33> | m_character_controller->adjust_foot_transform( capsule_size, start, finish, rotation_interpolation_koef, params.heel_transition_time, result );   SIZE
	// .. same ..
	// 0x89c <0x30> | --          | L375   ONLY target
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// --          | 0x966 <0x25> | m_drawer->draw_solid_capsule( result, capsule_size, fixed_color, true );   ONLY base
	// .. same ..
	// 0x8e3 <0x22> | --          | L379   ONLY target
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// 0x91f <0x2f> | 0x9a5 <0x3b> | float const				leg_len							= matrices[params.knee_bone_index   - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );   SIZE
	// 0x94e <0x2f> | 0x9e0 <0x3b> | float const				up_leg_len						= matrices[params.leg_bone_index    - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );   SIZE
	// 0x97d <0x2e> | 0xa1b <0x3a> | float const				knee_len						= matrices[params.foot_bone_index   - get_skeleton( ).get_root_bones_count( )].c.xyz( ).length( );   SIZE
	// .. same ..
	// 0x9ab <0x39> | 0xa55 <0x36> | float const				up_leg_to_fixed_foot_dist		= ( up_leg_world_matrix.c.xyz( ) - foot_center_transform.c.xyz( ) ).length( );   SIZE
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// 0xa7a <0x26> | 0xb21 <0x2a> | float const			position_iterpolation_koef		= 1.0f - m_heel_interpolator.interpolated_value( params.heel_transition_time );   SIZE
	// 0xaa0 <0x80> | 0xb4b <0x7d> | float3 const&		position						= foot_world_matrix.c.xyz( ) * position_iterpolation_koef + foot_center_transform.c.xyz( ) * ( 1.0f - position_iterpolation_koef );   SIZE
	// 0xb20 <0x21> | 0xbc8 <0x1e> | foot_center_transform.c.xyz( )	= position;   SIZE
	// .. same ..
	// 0xb41 <0x13> | 0xbe6 <0x10> | return foot_center_transform;   SIZE
	// ; aligned 44, size-diffs 38, quantity-diffs 15
	// VERDICT: STRUCTURE MATCH - 86/93 stmts, all aligned rows SIZE-only (LTCG temp-materialization at math call boundaries); quantity diffs are draw_*capsule/return codegen block placement, not source shape  trail: get_foot_fixed_transform.md
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

// STATE[STUB]
// void survarium::legs_ik_processor::tick(const unsigned int)
void legs_ik_processor::tick( u32 current_time_in_ms )
{
	// LOCALS
	// float 						dt_sec<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fa9e0><1>
	// ******

	// FUNCTION BODY
	// <0x6fa9d9>|0x009|+0x009:'462'
	// <0>
	// <0x6fa9e2>|0x012|+0x01f:'464'
	// <0x6faa01>|0x031|+0x012:'465'
	// <0x6faa13>|0x043|+0x012:'466'
	// <0x6faa25>|0x055|+0x01e:'467'
	// <0x6faa43>|0x073|+0x01e:'468'
	// <0>
	// <0x6faa61>|0x091|+0x009:'470'
	// ******
}

} // namespace survarium
