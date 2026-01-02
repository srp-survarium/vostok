////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/bullet.h>

#include <vostok/game_core/bullet_manager.h>
#include <vostok/game_core/game_material.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/game_core/weapon_ammunition.h>
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/ray_result.h>
#include <vostok/physics/world.h>
#include <vostok/console_command.h>

namespace survarium {

float g_bullet_tracer_exposition = 0.5f;
static console_commands::cc_float bullet_tracer_exposition( "bullet_tracer_exposition", g_bullet_tracer_exposition, 0.01f, 5.0f, true, console_commands::command_type_engine_internal );

// STATE[UNCHECKED]
bullet::bullet(
	bullet_manager&					bullet_manager,
	float3 const&					position,
	float3 const&					velocity,
	u32								born_time_in_ms,
	float							air_resistance,
	weapon_ammunition_ptr const&	wa,
	weapon_core const&				wc,
	hit_initiator const* const		initiator,
	hit_receiver const*	const		ignorable_object
) :
	m_position				( position ),
	m_velocity				( velocity ),
	m_start_position		( position ),
	m_start_velocity		( velocity ),
	m_bullet_manager		( &bullet_manager ),
	m_bullet_material		( NULL ),
	m_collided_material		( NULL ),
	m_initiator				( initiator ),
	m_ignorable_object		( ignorable_object ), // sushi@NOTE: What does that mean
	m_last_hitted_body_part	( NULL ),
	m_born_time_in_ms		( born_time_in_ms ),
	m_current_time_in_ms	( 0 ),
	m_life_time				( 0.0f ),
	m_air_resistance		( wa->air_resistance( ) * air_resistance ),
	m_current_resistance	( air_resistance ),
	m_max_distance			( wa->distance( ) * 1000.f ),
	m_flown_distance		( 0.0f ),
	m_damage_factor			( wa->damage( ) ),
	m_ricochet_angle		( wa->ricochet_angle( ) * math::pi / 180.f ), // sushi@NOTE: Where should this impl be?
	m_pierce_factor			( wa->pierce( ) ),
	m_weapon_bullet_damage	( wc.get_bullet_damage( ) ),
	m_weapon_bullet_pierce	( wc.get_bullet_pierce( ) ),
	m_change_trajectory_count	( 0 ),
	m_ricochet_count			( 0 ),
	m_tracer_idx				( u16( -1 ) )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	m_bullet_material = m_bullet_manager->get_material_manager( ).get_material( wa->game_material_id( ) );
	ASSERT( UNKNOWN_EXPRESSION );

	// FUNCTION BODY
	// <0x5916fa>|0x24a|+0x00c:'85'
	// <0x591706>|0x256|+0x00c:'86'
	// <0x591712>|0x262|+0x041:'87'
	// <0x591753>|0x2a3|+0x00c:'88'
	// ******
}

// STATE[100%|DONE]
bullet::bullet( bullet const& other )
{
	m_position					= other.m_position;
	m_velocity					= other.m_velocity;
	m_start_position			= other.m_position;
	m_start_velocity			= other.m_velocity;
	m_bullet_manager			= other.m_bullet_manager;

	m_born_time_in_ms			= other.m_born_time_in_ms;
	m_life_time					= other.m_life_time;
	m_air_resistance			= other.m_air_resistance;
	m_current_resistance		= other.m_current_resistance;
	m_max_distance				= other.m_max_distance;
	m_flown_distance			= other.m_flown_distance;
	m_change_trajectory_count	= other.m_change_trajectory_count;
	ASSERT( UNKNOWN_EXPRESSION );
	m_bullet_material			= other.m_bullet_material;
	m_collided_material			= other.m_collided_material;
	m_initiator					= other.m_initiator;
	m_ignorable_object			= other.m_ignorable_object;
	m_last_hitted_body_part		= other.m_last_hitted_body_part;
	m_tracer_idx				= other.m_tracer_idx;

	ASSERT( UNKNOWN_EXPRESSION );

	// FUNCTION BODY
	// <0x590e3a>|0x03a|+0x016:'96'
	// <0x590e50>|0x050|+0x01c:'97'
	// <0x590e6c>|0x06c|+0x019:'98'
	// <0x590e85>|0x085|+0x01c:'99'
	// <0x590ea1>|0x0a1|+0x00c:'100'
	// <0>
	// <0x590ead>|0x0ad|+0x00c:'102'
	// <0x590eb9>|0x0b9|+0x00c:'103'
	// <0x590ec5>|0x0c5|+0x00c:'104'
	// <0x590ed1>|0x0d1|+0x00c:'105'
	// <0x590edd>|0x0dd|+0x00c:'106'
	// <0x590ee9>|0x0e9|+0x00c:'107'
	// <0>
	// <0x590ef5>|0x0f5|+0x00c:'109'
	// <0x590f01>|0x101|+0x00c:'110'
	// <0x590f0d>|0x10d|+0x00c:'111'
	// <0x590f19>|0x119|+0x00c:'112'
	// <0x590f25>|0x125|+0x00c:'113'
	// <0x590f31>|0x131|+0x00c:'114'
	// <0x590f3d>|0x13d|+0x00c:'115'
	// <0x590f49>|0x149|+0x00e:'116'
	// <0>
	// <0x590f57>|0x157|+0x00c:'118'
	// ******
}

// STATE[100%|DONE]
bullet::~bullet( )
{
}

// STATE[UNCHECKED]
void bullet::tick( u32 current_time_in_ms )
{
	m_current_time_in_ms				= current_time_in_ms;
	const float3	zero_velocity		= float3( 0, 0, 0 );
	u16				invalid_tracer_idx	= u16(-1);
	float			low_time			= m_life_time;
	float			high_time			= ( current_time_in_ms - m_born_time_in_ms ) / 1000.0f * m_bullet_manager->get_bullet_time_factor( );
	ASSERT( UNKNOWN_EXPRESSION );

	float3 const&	gravity				= m_bullet_manager->get_gravity( );

	for ( ;; )
	{
		// sushi@NOTE: Added some ifdefs here

		if ( m_velocity.squared_length( ) < 1.f )
		{
			m_start_velocity = zero_velocity;
			return;
		}

		if ( m_change_trajectory_count >= 32 )
		{
			m_start_velocity = zero_velocity;
			return;
		}

		if( low_time == high_time )
			return;

		float	time					= pick_next_permissible_time( low_time, high_time, gravity );

		if( low_time == time )
			return;

		collision_result	result	= check_collision( m_position, low_time, time );
		if ( result == collision_result_collide )
		{
			m_start_velocity = zero_velocity;
			return;
		}

		if ( result == collision_result_pierced || result == collision_result_reflected )
		{
			low_time	= m_life_time;
			high_time	-= time;
			time		= m_life_time;
		}
		if	( time != 0 )
			if ( !update_bullet_position( time, gravity ) )
			{
				m_start_velocity = zero_velocity;
				return;
			}

		if ( math::is_similar( time, high_time ) ) // <0x592c61>|0x241|+0x023:'188'
		{
			if ( m_tracer_idx != 0xFFFF /* sushi@NOTE: invalid_tracer_idx */ )
			{
				float3 d = m_position - m_start_position;
				float d_len = d.length( );
				d.normalize_safe( float3( 0.0f, 0.0f, 1.0f ) );
				float speed = m_velocity.length( );
				float length = math::min( g_bullet_tracer_exposition, m_life_time ) * speed;

				if ( m_initiator->is_local && m_change_trajectory_count == 0 && m_life_time < g_bullet_tracer_exposition )
					length -= 5.0f;

				if ( m_change_trajectory_count && length > d_len )
					length = d_len;

				if ( length > 0.0f )
					m_bullet_manager->update_tracer( this, m_position, d, length );
			}
			return;
		}

		R_ASSERT			( low_time < high_time, "start_low[%f] high_time[%f]", low_time, high_time );
		low_time			= time;
		R_ASSERT			( low_time < high_time, "start_low[%f] high[%f]", low_time, high_time );
	}



	// FUNCTION BODY
	// ...
	// <0x592c13>|0x1f3|+0x012:'179'
	// <0>
	// <0x592c25>|0x205|+0x01d:'181'		if ( !update_bullet_position( time, gravity ) )
	// <0>									{
	// <0x592c42>|0x222|+0x01a:'183'			m_start_velocity = zero_velocity;
	// <0x592c5c>|0x23c|+0x005:'184'			return;
	// <0>									}
	// <1>
	// <2>
	// <0x592c61>|0x241|+0x023:'188'	if ( math::is_similar( time, high_time ) )
	// <0>
	// <0x592c84>|0x264|+0x016:'190'
	// <0>
	// <0x592c9a>|0x27a|+0x01b:'192'
	// <0x592cb5>|0x295|+0x00b:'193'
	// <0x592cc0>|0x2a0|+0x023:'194'
	// <0x592ce3>|0x2c3|+0x011:'195'
	// <0>
	// <0x592cf4>|0x2d4|+0x022:'197'
	// <0>
	// <1>
	// <0x592d16>|0x2f6|+0x031:'200'
	// <0x592d47>|0x327|+0x012:'201'
	// <0>
	// <0x592d59>|0x339|+0x017:'203'
	// <0x592d70>|0x350|+0x00a:'204'
	// <0>
	// <0x592d7a>|0x35a|+0x00e:'206'
	// <0x592d88>|0x368|+0x027:'207'
	// <0>
	// <0x592daf>|0x38f|+0x002:'209'
	// <0>
	// <1>
	// <0x592db1>|0x391|+0x00c:'212'	R_ASSERT			( low_time < high_time, "start_low[%f] high_time[%f]", low_time, high_time );
	// <0x592dbd>|0x39d|+0x00a:'213'	low_time			= time;
	// <0x592dc7>|0x3a7|+0x011:'214'	R_ASSERT			( low_time < high_time, "start_low[%f] high[%f]", low_time, high_time );
	// <0>
	// ******
}

bool bullet::is_finish_flying ( ) const
{
	const float3 zero_velocity = float3( 0, 0, 0 );

	return m_start_velocity == zero_velocity;
}

float3 const& bullet::get_start_position ( ) const
{
	return m_start_position;
}
float3 const& bullet::get_start_velocity ( ) const
{
	return m_start_velocity;
}

float3 const& bullet::get_position ( ) const
{
	return m_position;
}
float3 const& bullet::get_velocity ( ) const
{
	return m_velocity;
}

// STATE[UNCHECKED]
float3 bullet::compute_parabolic_velocity( float time, float3 const& gravity )
{
	float3 const xz_velocity	= float3( m_start_velocity.x, 0.f, m_start_velocity.z );
	// this could be since we could fire in different directions
	// for example, vertically into the ground
	if ( math::is_zero( xz_velocity.squared_length( ) ) )
		return					m_start_velocity + gravity * time;

	return						m_start_velocity * math::max( 0.f, 1.f - m_current_resistance * time ) + ( gravity * time );
}

// STATE[UNCHECKED]
float3 bullet::compute_trajectory_velocity( float time, float3 const& gravity )
{
	float const parabolic_time	= get_parabolic_time( );
	float const	fall_down_time	= time - parabolic_time;

	if ( fall_down_time < 0.f )
		return					compute_parabolic_velocity ( time, gravity );

	float3 const& parabolic_vel	= compute_parabolic_velocity( parabolic_time, gravity );
	return						parabolic_vel + gravity * fall_down_time;
}

// STATE[UNCHECKED]
float3 bullet::compute_parabolic_position( float time, float3 const& gravity )
{
	float3 const xz_velocity	= float3( m_start_velocity.x, 0.f, m_start_velocity.z );

	if ( math::is_zero( xz_velocity.squared_length( ) ) )
		return				m_start_position + m_start_velocity * time + gravity * ( math::sqr( time ) * .5f );

	float const sqr_t_div_2	= math::sqr( time ) * .5f;
	return					m_start_position + m_start_velocity * time + m_start_velocity * -m_current_resistance * sqr_t_div_2 + gravity * sqr_t_div_2;
}

// STATE[UNCHECKED]
float3 bullet::compute_trajectory_position( float time, float3 const& gravity )
{
	float const parabolic_time	= get_parabolic_time( );
	float const	fall_down_time	= time - parabolic_time;

	if ( fall_down_time < 0.f )
		return					compute_parabolic_position( time, gravity );

	float3 const& parabolic_pos	= compute_parabolic_position( parabolic_time, gravity );
	float3 const& parabolic_vel	= compute_parabolic_velocity( parabolic_time, gravity );

	return parabolic_pos + parabolic_vel * fall_down_time + gravity * ( math::sqr( fall_down_time ) * .5f );
}

// STATE[UNCHECKED]
float bullet::get_parabolic_time( )
{
	static const float air_resistance_epsilon = 1.1f;
	return math::max( 0.f, 1.f / ( m_current_resistance * air_resistance_epsilon ) );
}

// STATE[100%|DONE]
float bullet::pick_next_permissible_time( float low_time, float high_time, float3 const& gravity )
{
	R_ASSERT	( low_time < high_time, "low_time[%f] high_time[%f]", low_time, high_time );
	float const start_high_time	= high_time;

	if ( pick_permissible_range( high_time, low_time, high_time, gravity ) )
	{
		if ( high_time <= start_high_time )
			return			high_time;

		return				start_high_time;
	}

	float					low			= low_time;
	float					check_time	= high_time;
	float const				epsilon		= .05f;
	while ( !math::is_similar( low, high_time, math::epsilon_5 ) )
	{
		float				distance = compute_max_error( low_time, check_time, gravity );

		if ( distance < epsilon )
			low				= check_time;
		else
			high_time		= check_time;

		check_time			= ( low + high_time ) * .5f;
	}

	R_ASSERT				( low <= start_high_time, "low[%f], high[%f]", low, start_high_time );
	return					( low );
}

// STATE[100%|DONE]
bool bullet::pick_permissible_range(
	float&				result,
	float				low_time,
	float				high_time,
	float3 const&		gravity
)
{
//	R_ASSERT				( m_max_distance - m_flown_distance > 0.0f );

	float const	parabolic_time	= get_parabolic_time( );

	if ( low_time > parabolic_time )
	{
		result				= get_check_time_in_vacuum( low_time, high_time, gravity );
		return				true;
	}

	if ( high_time  < parabolic_time )
	{
		result				= get_check_time( low_time, high_time, gravity );
		return				false;
	}

	if ( !math::is_similar( parabolic_time, low_time ) )
	{
		result				= get_check_time( low_time, parabolic_time, gravity );
		return				false;
	}

	result					= get_check_time_in_vacuum( parabolic_time, high_time, gravity );
	return					false;
}

// STATE[100%|DONE]
float bullet::get_check_time_in_vacuum( float start_low, float high, float3 const& gravity )
{
	float const max_test_distance	= m_max_distance - m_flown_distance;
	float const time_delta	= high - start_low;
	float const time_to_fly	= ( m_start_velocity * time_delta + gravity * ( math::sqr( time_delta ) * .5f ) ).length( );
	if ( time_to_fly <= max_test_distance )
		return				high;

	float const fall_down_velocity_magnitude = compute_trajectory_velocity( start_low, gravity ).length( );
	float const positive_gravity	= -gravity.y;
	float time				=	( math::sqrt(
									math::sqr( fall_down_velocity_magnitude ) + 2.f * max_test_distance * positive_gravity )
									- fall_down_velocity_magnitude )
								/ positive_gravity;
	R_ASSERT				( time >= 0.f );

	R_ASSERT				( high >= start_low );
	float result			= start_low + time;
	math::clamp				( result, start_low, high );
	R_ASSERT				( result <= high, "result[%f], high[%f], start_low[%f], air_resistance[%f]", result, high, start_low, m_current_resistance );

	return					result;
}

// STATE[UNCHECKED]
float get_max_error_time( float t0, float t1 )
{
	return					( ( t1 + t0 ) * .5f );
	// this is correct even in our case
	// y(t) = V0y*t - V0y*ar*t^2/2 - g*t^2/2
	// x(t) = V0x*t - V0x*ar*t^2/2
}

// STATE[UNCHECKED]
float bullet::get_check_time( float const start_low, float high, float3 const& gravity )
{
	float const max_test_distance	= m_max_distance - m_flown_distance;
	float3 const start		= compute_trajectory_position( start_low, gravity );
#ifndef MASTER_GOLD
	float const start_high	= high;
#endif // #ifndef MASTER_GOLD
	float					low = start_low;
	float					check_time = high;
	while ( !math::is_similar( low, high ) ) {
		float3 const intermediate	= compute_trajectory_position( get_max_error_time( start_low, check_time ), gravity );
		float3 const target			= compute_trajectory_position( check_time, gravity );
		float const distance		= ( intermediate - start ).length( ) + ( intermediate - target ).length( );
		if ( distance < max_test_distance )
			low				= check_time;
		else
			high			= check_time;

		check_time			= ( low + high ) * .5f;
	}

	R_ASSERT				( low <= start_high );
	return					low;
}

// STATE[UNCHECKED]
float bullet::compute_max_error( float low, float high, float3 const& gravity )
{
	float					max_error_time = get_max_error_time( low, high );

	float3 const start		= compute_trajectory_position( low, gravity );
	float3 const target		= compute_trajectory_position( high, gravity );
	float3 const max_error	= compute_trajectory_position( max_error_time, gravity );

	float3	start_to_max_error	= max_error - start;
	float	magnitude			= start_to_max_error.length( );
	start_to_max_error			*= 1.f / magnitude;
	float3	start_to_target		= ( target - start ).normalize( );
	float	cosine_alpha		= math::max( -1.0f, math::min( start_to_max_error | start_to_target, 1.0f ) );
	float	sine_alpha			= math::sqrt( 1.f - math::sqr( cosine_alpha ) );

	return	magnitude * sine_alpha;
}

// STATE[UNCHECKED]
collision_result bullet::check_collision( float3 start_position, float start_time, float current_time )
{
	collision_result const result	= collision_result_no_collision;

	float3 const& target_position	= compute_trajectory_position( current_time, m_bullet_manager->get_gravity( ) );
	R_ASSERT						( start_time < current_time, "start time can not be greater than current time" );

	float3 direction				= target_position - start_position;
	float distance					= direction.length( );
	if ( math::is_zero( distance ) )
		return						result;

	direction						*= 1.f / distance;

	physics::world& p_world			= m_bullet_manager->get_physics_world( );

	float3 new_start_position		= start_position;

	for ( ;; ) {
		physics::closest_ray_result ray_result = p_world.ray_test( new_start_position, direction, distance, 16, 8 ); // sushi@TODO: Give name to those constants based on the usage.
		if ( !ray_result.object )
			return					result;

		float3 triangle_normal		= ray_result.hit_normal_world;
		float cos_alpha				= triangle_normal | direction;
		triangle_orientation orientation	= (triangle_orientation)( cos_alpha >= 0.0f );
		bool ignorable_object_was_hit		= m_ignorable_object && ray_result.object->user_data && ray_result.object->user_data == m_ignorable_object;

		if ( ignorable_object_was_hit || orientation == triangle_orientation_back_face )
		{
			distance -= ( ray_result.hit_point_world - new_start_position ).length( );
			new_start_position = direction * math::epsilon_5 * new_start_position.length( ) + ray_result.hit_point_world;
			continue;
		}

		return process_ray_query( ray_result, distance, start_position, direction, start_time, current_time );
	}

	// FUNCTION BODY
	// <0x5927df>|0x00f|+0x007:'431'
	// <0>
	// <0x5927e6>|0x016|+0x029:'433'
	// <0x59280f>|0x03f|+0x00c:'434'
	// <0>
	// <0x59281b>|0x04b|+0x012:'436'
	// <0x59282d>|0x05d|+0x00b:'437'
	// <0x592838>|0x068|+0x014:'438'
	// <0x59284c>|0x07c|+0x008:'439'
	// <0>
	// <0x592854>|0x084|+0x015:'441'
	// <0>
	// <0x592869>|0x099|+0x00f:'443'	physics::world& p_world			= m_bullet_manager->get_physics_world( );
	// <0>
	// <0x592878>|0x0a8|+0x012:'445'	float3 new_start_position		= start_position;
	// <0>
	// <1>								for ( ;; ) {
	// <0x59288a>|0x0ba|+0x024|[1]:'448'
	// <0x5928ae>|0x0de|+0x006:'449'
	// <0x5928b4>|0x0e4|+0x008:'450'
	// <0>
	// <0x5928bc>|0x0ec|+0x012:'452'
	// <0x5928ce>|0x0fe|+0x010:'453'
	// <0x5928de>|0x10e|+0x02d:'454'
	// <0>
	// <1>
	// <2>
	// <0x59290b>|0x13b|+0x04b:'458'		bool ignorable_object_was_hit		= m_ignorable_ob
	// <0>
	// <0x592956>|0x186|+0x012:'460'
	// <0>
	// <0x592968>|0x198|+0x01d:'462'		distance -= ( ray_result.hit_point_world - new_start_position ).length( );
	// <0x592985>|0x1b5|+0x060:'463'		new_start_position = direction * math::epsilon_5 * new_start_position.length( ) + ray_result.hit_point_world;
	// <0x5929e5>|0x215|+0x005:'464'		continue
	// <0>
	// <1>
	// <0x5929ea>|0x21a|+0x028:'467'		return process_ray_query( ray_result, distance, start_position, direction, start_time, current_time );
	// <0x592a12>|0x242|+0x005:'468'	}
	// ******
}

// STATE[UNCHECKED]
collision_result bullet::process_ray_query(
	physics::closest_ray_result const&	ray_result,
	float								distance,
	float3&								start_position,
	float3&								fly_direction,
	float&								start_time,
	float&								current_time
)
{
	float3 triangle_normal				= ray_result.hit_normal_world;
	const float distance_to_hit_point	= ( start_position - ray_result.hit_point_world ).length( );
	float3 collide_point				= start_position + fly_direction * distance_to_hit_point;
	float collision_time				= start_time + ( current_time - start_time ) * distance_to_hit_point / distance;

	float cos_alpha						= triangle_normal | fly_direction;
	triangle_orientation orientation	= ( triangle_orientation )( cos_alpha >= 0.0f );

	if ( orientation == triangle_orientation_back_face )
		return collision_result_no_collision;

	if ( math::is_zero( cos_alpha ) )
		return collision_result_no_collision;

	physics::bt_rigid_body_base* body	= static_cast<physics::bt_rigid_body_base*>( ray_result.object );
	u16 game_material_id				= body->get_triangle_material( ray_result.triangle_index, ray_result.is_shape_index );

	m_collided_material					= m_bullet_manager->get_material_manager( ).get_material( game_material_id );
	ASSERT( UNKNOWN_EXPRESSION_T( m_collided_material ) );

	fix_collision_point_and_time( collide_point, collision_time, start_time, current_time, orientation, triangle_normal, m_bullet_manager->get_gravity( ) );

	fly_direction						= compute_trajectory_velocity( collision_time, m_bullet_manager->get_gravity( ) );
	float speed							= fly_direction.length( );
	fly_direction						/= speed;

	float angle_alpha					= math::acos( cos_alpha );

	if ( m_ricochet_count >= 2 || m_collided_material->ricochet_koef( ) * m_ricochet_angle < angle_alpha - math::pi_d2 )
		return collide_front_face( collide_point, fly_direction, triangle_normal, speed, collision_time, start_position, start_time, current_time, ray_result );

	return try_reflect( collide_point, fly_direction, triangle_normal, speed, collision_time, start_position, start_time, current_time, cos_alpha );
}

// STATE[100%|DONE]
void bullet::fix_collision_point_and_time(
	float3&					collide_point,
	float&					collision_time,
	float					start_time,
	float					current_time,
	triangle_orientation	orientation,
	float3 const&			triangle_normal,
	float3 const&			gravity
)
{
#ifndef MASTER_GOLD
	float3		dbg_new_collide_point	= compute_trajectory_position( start_time, gravity );
	float		dbg_delta				= ( ( dbg_new_collide_point - collide_point ) | triangle_normal );
	R_ASSERT	( ( orientation == triangle_orientation_front_face && dbg_delta >= 0 ) ||
					( orientation == triangle_orientation_back_face && dbg_delta <= 0 ) );
#endif // #ifndef MASTER_GOLD

	float3		new_collide_point	= compute_trajectory_position( collision_time, gravity );
	float		delta				= ( ( new_collide_point - collide_point ) | triangle_normal );

	if( !math::is_zero( delta ) )
	{
		float		low_time		= start_time;
		float		high_time		= current_time;

		while ( !math::is_zero( delta ) )
		{
			if (	( orientation == triangle_orientation_front_face && delta < 0 ) ||
					( orientation == triangle_orientation_back_face && delta > 0 ) )
				high_time			= collision_time;
			else
				low_time			= collision_time;

			collision_time		= ( low_time + high_time ) * .5f;

			new_collide_point	= compute_trajectory_position( collision_time, gravity );
			delta				= ( ( new_collide_point - collide_point ) | triangle_normal );
		}

		collide_point			= new_collide_point;
	}
}

// STATE[UNCHECKED]
collision_result bullet::collide_front_face(
	float3 const&						collide_point,
	float3 const&						bullet_direction,
	float3 const&						triangle_normal,
	float								speed,
	float								collision_time,
	float3&								start_position,
	float&								start_time,
	float&								current_time,
	physics::closest_ray_result const&	ray_result
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( m_bullet_manager->m_engine )
	{
		material_pair const* mtl_pair = m_bullet_manager->get_material_manager( ).get_pair( m_bullet_material->id( ), m_collided_material->id( ) );
		if ( mtl_pair )
		{
			if ( mtl_pair->decal1( ) )
				m_bullet_manager->add_decal( mtl_pair->decal1( ), mtl_pair->decal1_size( ), collide_point, bullet_direction, triangle_normal, true );

			if ( mtl_pair->has_particle( ) )
				m_bullet_manager->play_particle( mtl_pair->particle( ), collide_point, bullet_direction, triangle_normal );
		}
		else
		{
			LOG_WARNING( "material pair not exists [%s]-[%s]", m_bullet_material, m_collided_material );
		}
	}

	physics::bt_rigid_body_base* target = static_cast<physics::bt_rigid_body_base*>( ray_result.object ); // sushi@NOTE: Why do we do this cast.
	if ( target->user_data )
	{
		hit_receiver* hit_target = target->user_data->cast_to_hit_receiver( );
		if ( hit_target && hit_target != m_ignorable_object )
			hit_target->hit( m_initiator, ray_result.triangle_index, "injury", m_weapon_bullet_damage * m_damage_factor, m_weapon_bullet_pierce * m_pierce_factor, this ); // sushi@TODO: Functions for damage
	}

	if ( m_collided_material->resistance( ) > m_weapon_bullet_pierce * m_pierce_factor )
		return collision_result_collide;

	float new_speed = math::min(	// sushi@NOTE: Use clamp instead
		math::max( 0.0, m_weapon_bullet_pierce * m_pierce_factor / m_collided_material->resistance( ) - 1.0f ),
		1.0f
	) * speed;

	m_current_resistance = m_collided_material->resistance( ); // sushi@NOTE: Gets overwritten by `change_trajectory` :)
	change_trajectory			( collide_point + bullet_direction * 0.001f, bullet_direction * new_speed, collision_time ); // Ensures collision is not triggered immediately?

	start_position	= m_start_position;
	start_time		= m_life_time;
	current_time	-= collision_time;
}

// STATE[UNCHECKED]
collision_result bullet::try_reflect(
	float3 const&		collide_point,
	float3				direction,
	float3 const&		triangle_normal,
	float				speed,
	float				collision_time,
	float3&				start_position,
	float&				start_time,
	float&				current_time,
	float				cos_alpha
)
{
	float fin_ricochet_angle = m_collided_material->ricochet_koef( ) * m_ricochet_angle;
	float angle_alpha		= math::acos( cos_alpha ) - math::pi_d2;

	float calculated_koeff	= 1.0f - angle_alpha / fin_ricochet_angle;

	speed					*= math::linear_interpolation( calculated_koeff, 1.0f, m_collided_material->reflection_speed_down( ) );
	if( speed < 0 )
	{
#ifndef MASTER_GOLD
		bullet_manager.add_collision_point	( collide_point, math::color( 255, 0, 0, 128 ) );
#endif // #ifndef MASTER_GOLD
		return collision_result_collide;
	}

	direction				= 2 * triangle_normal * -cos_alpha + direction;
	change_trajectory		( collide_point, direction * speed, collision_time );

#ifndef MASTER_GOLD
	bullet_manager.store_bullet_trajectory	( this );
	bullet_manager.add_collision_point		( collide_point, math::color( 0, 255, 0, 128 ) );
#endif // #ifndef MASTER_GOLD

	start_position	= m_start_position;
	start_time		= m_life_time;
	current_time	-= collision_time;

	m_ricochet_count += 1;

	return collision_result_reflected;

	// FUNCTION BODY
	// <0x591059>|0x009|+0x022:'674'
	// <0x59107b>|0x02b|+0x018:'675'
	// <0>
	// <1>
	// <0x591093>|0x043|+0x01b:'678'
	// <0>
	// <0x5910ae>|0x05e|+0x041:'680'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5910ef>|0x09f|+0x00e:'689'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5910fd>|0x0ad|+0x00a:'695'
	// <0>
	// <1>
	// <0x591107>|0x0b7|+0x063:'698'
	// <0>
	// <0x59116a>|0x11a|+0x026:'700'
	// <0>
	// <0x591190>|0x140|+0x019:'702'
	// <0x5911a9>|0x159|+0x00b:'703'
	// <0x5911b4>|0x164|+0x013:'704'
	// <0>
	// <0x5911c7>|0x177|+0x00f:'706'
	// <0>
	// <0x5911d6>|0x186|+0x005:'708'
	// ******
}

// STATE[UNCHECKED]
void bullet::change_trajectory( float3 const& new_position, float3 const& new_velocity, float collision_time )
{
	++m_change_trajectory_count;

	m_start_position		= new_position;
	m_start_velocity		= new_velocity;
	m_position				= m_start_position;
	m_velocity				= m_start_velocity;
	m_current_resistance	= m_air_resistance;
	m_born_time_in_ms		+= math::floor( 1000.f * collision_time / m_bullet_manager->get_bullet_time_factor( ) ); // s_bm_bullet_time_factor
	m_life_time				= 0;


	// FUNCTION BODY
	// <0x590f79>|0x009|+0x00f:'714'
	// <0>
	// <0x590f88>|0x018|+0x019:'716'
	// <0x590fa1>|0x031|+0x019:'717'
	// <0x590fba>|0x04a|+0x019:'718'
	// <0x590fd3>|0x063|+0x01c:'719'
	// <0x590fef>|0x07f|+0x00c:'720'
	// <0x590ffb>|0x08b|+0x039:'721'
	// <0x591034>|0x0c4|+0x010:'722'
	// ******
}

// STATE[UNCHECKED]
bool bullet::update_bullet_position( const float time, float3 const& gravity )
{
	float3 const new_position	= compute_trajectory_position( time, gravity );
	if ( !m_bullet_manager->is_inside_collision_db( new_position ) )
		return					false;

	m_flown_distance			+= ( new_position - m_position ).length( );

	if ( m_flown_distance >= m_max_distance )
	{
		m_flown_distance =		m_max_distance;
		return					false;
	}

	m_velocity					= compute_trajectory_velocity( m_life_time, gravity );
	if ( math::is_zero( m_velocity.squared_length( ) ) )
		return					false;

	m_position					= new_position;
	m_life_time					= time;

	return						true;


	// FUNCTION BODY
	// <0x591919>|0x009|+0x01d:'727'
	// <0x591936>|0x026|+0x016:'728'
	// <0x59194c>|0x03c|+0x007:'729'
	// <0>
	// <0x591953>|0x043|+0x023:'731'
	// <0>
	// <0x591976>|0x066|+0x011:'733'	if ( m_flown_distance >= m_max_distance )
	// <0>
	// <0x591987>|0x077|+0x00c:'735'
	// <0x591993>|0x083|+0x007:'736'
	// <0>
	// <1>
	// <0x59199a>|0x08a|+0x030:'739'	m_velocity
	// <0x5919ca>|0x0ba|+0x022:'740'
	// <0x5919ec>|0x0dc|+0x004:'741'
	// <0>
	// <0x5919f0>|0x0e0|+0x016:'743'
	// <0x591a06>|0x0f6|+0x00c:'744'
	// <0>
	// <0x591a12>|0x102|+0x00d:'746'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x591a1f>|0x10f|+0x002:'752'
	// ******
}

} // namespace survarium
