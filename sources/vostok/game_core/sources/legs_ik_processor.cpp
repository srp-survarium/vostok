// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/math_float4x4.h>
#include <vostok/animation/skeleton.h>
#include <vostok/game_core/legs_ik_processor.h>
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/game_core/ik_utils.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>
#include <vostok/console_command.h>

namespace survarium {

float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices );

static bool		s_ik_legs_debug_draw_value		= false;
static float	s_ik_foot_capsule_radius_value	= 0.065f;
static bool		s_ik_legs_rot_axis_value		= false;
static bool		s_ik_adjust_hip_position_value	= false;

static console_commands::cc_bool	s_ik_legs_debug_draw_cc		( "ik_legs_debug_draw", s_ik_legs_debug_draw_value, false, console_commands::command_type_engine_internal );
static console_commands::cc_float	s_ik_foot_capsule_radius_cc	( "ik_foot_capsule_radius", s_ik_foot_capsule_radius_value, 0.01f, 0.2f, true, console_commands::command_type_engine_internal );
static console_commands::cc_bool	s_ik_legs_rot_axis_cc		( "ik_legs_rot_axis", s_ik_legs_rot_axis_value, false, console_commands::command_type_engine_internal, console_commands::execution_filter_early );
static console_commands::cc_bool	s_ik_adjust_hip_position_cc	( "ik_adjust_hip_position", s_ik_adjust_hip_position_value, false, console_commands::command_type_engine_internal, console_commands::execution_filter_early );

legs_ik_processor::leg_params::leg_params( ) :
	heel_transition_time	( 0.0f ),
	toe_transition_time		( 0.0f ),
	rotation_axis			( 1.0f, 0.0f, 0.0f ),
	m_time_since_stance		( 0.0f ),
	m_heel_on_ground		( false ),
	m_toe_on_ground			( false )
{
}

void legs_ik_processor::leg_params::activate( animation::skeleton const& skeleton, pcstr foot_bone_name )
{
	animation::skeleton_bone const&	foot_bone	= skeleton.get_bone( skeleton.get_bone_index( foot_bone_name ) );

	foot_bone_index		= skeleton.get_bone_index( foot_bone );
	toe_bone_index		= skeleton.get_bone_index( *foot_bone.children_begin( ) );
	leg_bone_index		= skeleton.get_bone_index( *foot_bone.parent( ) );
	knee_bone_index		= skeleton.get_bone_index( *foot_bone.parent( )->parent( ) );
	up_leg_bone_index	= skeleton.get_bone_index( *foot_bone.parent( )->parent( )->parent( ) );
}

legs_ik_processor::~legs_ik_processor( )
{
	VOSTOK_DELETE_IMPL( *::survarium::g_allocator, m_drawer );
}

void legs_ik_processor::leg_params::tick( float dt )
{
	heel_transition_time	= math::max( heel_transition_time - dt, 0.0f );
	toe_transition_time		= math::max( toe_transition_time - dt, 0.0f );
	m_time_since_stance		+= dt;
}

void legs_ik_processor::leg_params::set_heel_transition_time( float tr_time )
{
	heel_transition_time = math::min( heel_transition_time, tr_time );
}

void legs_ik_processor::leg_params::set_toe_transition_time( float tr_time )
{
	toe_transition_time = math::min( toe_transition_time, tr_time );
}

void legs_ik_processor::leg_params::set_heel_on_ground( const bool value )
{
	m_heel_on_ground = value;
	if ( is_full_on_ground( ) )
		m_time_since_stance = 0.0f;
}

void legs_ik_processor::leg_params::set_toe_on_ground( const bool value )
{
	m_toe_on_ground = value;
	if ( is_full_on_ground( ) )
		m_time_since_stance = 0.0f;
}

legs_ik_processor::legs_ik_processor( ) :
	m_drawer				( NULL ),
	m_character_controller	( NULL ),
	m_heel_interpolator		( 0.1f ),
	m_toe_interpolator		( 0.1f ),
	m_heel_transition_time	( m_heel_interpolator.transition_time( ) ),
	m_toe_transition_time	( m_toe_interpolator.transition_time( ) )
{
}

void legs_ik_processor::activate( animation::skeleton const& skeleton )
{
	ik_processor::activate( skeleton );
	m_left_leg_params.activate( skeleton, "LeftFoot" );
	m_right_leg_params.activate( skeleton, "RightFoot" );
	m_hip_bone	= &skeleton.get_bone( skeleton.get_bone_index( "Hip" ) );
}

float get_additional_length( float3 const& upleg_dir, float3 const& leg_dir, float knee_len )
{
	float const knee_angle_cos	= upleg_dir | -leg_dir;
	return math::is_similar( knee_angle_cos, 1.0f, math::epsilon_5 )
		? knee_len * 0.5f
		: math::sqrt( math::sqr( knee_len ) * 0.5f / ( 1.0f - knee_angle_cos ) );
}

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
	else if ( m_left_leg_params.is_on_ground( ) ) m_right_leg_params.is_on_ground( );
}

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
}

float4x4 legs_ik_processor::get_foot_fixed_transform(
	legs_ik_processor::leg_params const&	params,
	float4x4 const&						hip_world_matrix,
	float4x4 const*						matrices,
	float&								delta_len
) const
{
	static float				dist_to_test					= 0.5f;

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
}

void legs_ik_processor::set_left_heel_on_ground( bool value )
{
	set_heel_on_ground( m_left_leg_params, value );
}

void legs_ik_processor::set_left_toe_on_ground( bool value )
{
	set_toe_on_ground( m_left_leg_params, value );
}

void legs_ik_processor::set_right_heel_on_ground( bool value )
{
	set_heel_on_ground( m_right_leg_params, value );
}

void legs_ik_processor::set_right_toe_on_ground( bool value )
{
	set_toe_on_ground( m_right_leg_params, value );
}

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
}

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
}

void legs_ik_processor::tick( const u32 current_time_in_ms )
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
