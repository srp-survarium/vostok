// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/bullet.h>

#include <vostok/game_core/bullet_manager.h>
#include <vostok/game_core/game_material.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/game_core/weapon_ammunition.h>
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_receiver.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/ray_result.h>
#include <vostok/physics/world.h>
#include <vostok/console_command.h>

namespace survarium {

static float g_bullet_tracer_exposition = 0.5f;
static console_commands::cc_float bullet_tracer_exposition( "bullet_tracer_exposition", g_bullet_tracer_exposition, 0.01f, 5.0f, true, console_commands::command_type_engine_internal );

bullet::bullet(
	bullet_manager&					bullet_manager,
	float3 const&					position,
	float3 const&					velocity,
	const u32						born_time_in_ms,
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
	m_ricochet_angle		( wa->ricochet_angle( ) * math::pi / 180.f ), // sushi@NOTE: wa->ricochet_angle( ) generates one more useless load
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
}

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
}

bullet::~bullet( )
{
}

void bullet::tick( u32 current_time_in_ms )
{
	m_current_time_in_ms				= current_time_in_ms;
	float3			zero_velocity		= float3( 0, 0, 0 );	// sushi@MATCH: <0x592a3b> Arguments are built slightly differently. claude@MATCH: target PDB records it NON-const.
	u16 const		invalid_tracer_idx	= u16(-1);
	float			low_time			= m_life_time;
	float			high_time			= ( current_time_in_ms - m_born_time_in_ms ) / 1000.0f * m_bullet_manager->get_bullet_time_factor( );
	ASSERT( UNKNOWN_EXPRESSION );

	float3 const&	gravity				= m_bullet_manager->get_gravity( );

	for ( ;; )
	{
		// sushi@NOTE: Added some ifdefs here

		if ( m_velocity.squared_length( ) < 1.f )	// sushi@MATCH: <0x592aba> fld1 is used instead of movss.
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
		{
			m_start_velocity = zero_velocity;
			return;
		}

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

		if ( math::is_similar( time, high_time ) )
		{
			if ( m_tracer_idx != 0xFFFF /* sushi@NOTE: invalid_tracer_idx */ )
			{
				float3 d = m_position - m_start_position;
				float const d_len = d.length( );
				d.normalize_safe( float3( 0.0f, 0.0f, 1.0f ) );
				float const speed = m_velocity.length( );
				float length = math::min( m_life_time, g_bullet_tracer_exposition ) * speed;

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
}

bool bullet::is_finish_flying ( ) const
{
	const float3 zero_velocity = float3( 0, 0, 0 );

	return m_start_velocity == zero_velocity;
}

float3 bullet::compute_parabolic_velocity( float time, float3 const& gravity )
{
	float3 xz_velocity			= float3( m_start_velocity.x, 0.f, m_start_velocity.z );	// claude@MATCH: target PDB records it NON-const
	// this could be since we could fire in different directions
	// for example, vertically into the ground
	if ( math::is_zero( xz_velocity.squared_length( ) ) )
		return					m_start_velocity + gravity * time;

	return						m_start_velocity * math::max( 0.f, 1.f - m_current_resistance * time ) + ( gravity * time );
}

float3 bullet::compute_trajectory_velocity( float time, float3 const& gravity )
{
	float const parabolic_time	= get_parabolic_time( );
	float const	fall_down_time	= time - parabolic_time;

	if ( fall_down_time < 0.f )
		return					compute_parabolic_velocity ( time, gravity );

	float3 const& parabolic_vel	= compute_parabolic_velocity( parabolic_time, gravity );
	return						parabolic_vel + gravity * fall_down_time;
}

float3 bullet::compute_parabolic_position( float time, float3 const& gravity )
{
	float3 xz_velocity			= float3( m_start_velocity.x, 0.f, m_start_velocity.z );	// claude@MATCH: target PDB records it NON-const

	if ( math::is_zero( xz_velocity.squared_length( ) ) )
		return				m_start_position + m_start_velocity * time + gravity * ( math::sqr( time ) * .5f );

	float const sqr_t_div_2	= math::sqr( time ) * .5f;
	return					m_start_position + m_start_velocity * time + m_start_velocity * -m_current_resistance * sqr_t_div_2 + gravity * sqr_t_div_2;
}

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

float bullet::get_parabolic_time( )
{
	static const float air_resistance_epsilon = 1.1f;
	return math::max( 0.f, 1.f / ( m_current_resistance * air_resistance_epsilon ) );
}

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

inline static float get_max_error_time( float t0, float t1 )
{
	return					( ( t1 + t0 ) * .5f );
	// this is correct even in our case
	// y(t) = V0y*t - V0y*ar*t^2/2 - g*t^2/2
	// x(t) = V0x*t - V0x*ar*t^2/2
}

float bullet::get_check_time( float const start_low, float high, float3 const& gravity )
{
	float const max_test_distance	= m_max_distance - m_flown_distance;
	float3 start			= compute_trajectory_position( start_low, gravity );	// claude@MATCH: target PDB records start/intermediate/target NON-const
#ifndef MASTER_GOLD
	float const start_high	= high;
#endif // #ifndef MASTER_GOLD
	float					low = start_low;
	float					check_time = high;
	while ( !math::is_similar( low, high ) ) {
		float3 intermediate			= compute_trajectory_position( get_max_error_time( start_low, check_time ), gravity );
		float3 target				= compute_trajectory_position( check_time, gravity );
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

float bullet::compute_max_error( float low, float high, float3 const& gravity )
{
	float					max_error_time = get_max_error_time( low, high );

	// claude@MATCH: target PDB records these three NON-const.
	float3 start			= compute_trajectory_position( low, gravity );
	float3 target			= compute_trajectory_position( high, gravity );
	float3 max_error		= compute_trajectory_position( max_error_time, gravity );

	float3	start_to_max_error	= max_error - start;
	float	magnitude			= start_to_max_error.length( );
	start_to_max_error			*= 1.f / magnitude;
	float3	start_to_target		= ( target - start ).normalize( );
	float	cosine_alpha		= math::max( -1.0f, math::min( start_to_max_error | start_to_target, 1.0f ) );
	float	sine_alpha			= math::sqrt( 1.f - math::sqr( cosine_alpha ) );

	return	magnitude * sine_alpha;
}

collision_result bullet::check_collision( float3 start_position, float start_time, float current_time )
{
	collision_result result			= collision_result_no_collision;	// claude@MATCH: target PDB records this local NON-const

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
		triangle_orientation orientation	= (triangle_orientation)( 0.0f <= cos_alpha ); // sushi@MATCH: <0x5928de> Somehow done in reverse. Maybe with some inlined function.
		bool const ignorable_object_was_hit	= m_ignorable_object && ray_result.object->user_data && ray_result.object->user_data->cast_to_hit_receiver( ) == m_ignorable_object;

		if ( ignorable_object_was_hit || orientation == triangle_orientation_back_face )
		{
			distance -= ( ray_result.hit_point_world - new_start_position ).length( );
			new_start_position = ray_result.hit_point_world + direction * math::epsilon_5 * new_start_position.length( );
			continue;
		}

		return process_ray_query( ray_result, distance, start_position, direction, start_time, current_time );
	}
}

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
	float collision_time				= start_time + distance_to_hit_point / distance * ( current_time - start_time );

	float cos_alpha						= triangle_normal | fly_direction;
	triangle_orientation orientation	= ( triangle_orientation )( cos_alpha >= 0.0f ); // sushi@MATCH: This again

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

	if ( m_ricochet_count < 2 && angle_alpha - math::pi_d2 <=  m_collided_material->ricochet_koef( ) * m_ricochet_angle )
		return try_reflect( collide_point, fly_direction, triangle_normal, speed, collision_time, start_position, start_time, current_time, cos_alpha );

	return collide_front_face( collide_point, fly_direction, triangle_normal, speed, collision_time, start_position, start_time, current_time, ray_result );
}

void bullet::fix_collision_point_and_time(
	float3&					collide_point,
	float&					collision_time,
	float					start_time,
	float					current_time,
	const triangle_orientation orientation,
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

	if( !math::is_zero( delta, math::epsilon_3 ) )
	{
		float		low_time		= start_time;
		float		high_time		= current_time;

		while ( !math::is_zero( delta, math::epsilon_3 ) )
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

	if ( m_bullet_manager->has_engine( ) )
	{
		material_pair const* mtl_pair = m_bullet_manager->get_material_manager( ).get_pair( m_bullet_material->id( ), m_collided_material->id( ) );
		if ( mtl_pair )
		{
			if ( mtl_pair->decal1( ) )	// sushi@MATCH: decal1 & decal1_size didn't inline normally
				m_bullet_manager->add_decal( mtl_pair->decal1( ), mtl_pair->decal1_size( ), collide_point, bullet_direction, triangle_normal, true );

			if ( mtl_pair->has_particle( ) )
				m_bullet_manager->play_particle( mtl_pair->particle( ), collide_point, bullet_direction, triangle_normal );
		}
		else
		{	// sushi@MATCH: Some kind of jump here.
			LOG_WARNING( "material pair not exists [%s]-[%s]", m_bullet_material->name( ), m_collided_material->name( ) );
		}
	}

	physics::bt_rigid_body_base* const target = static_cast<physics::bt_rigid_body_base*>( ray_result.object ); // sushi@NOTE: Why do we do this cast.
	if ( target->user_data )
	{
		hit_receiver* const hit_target = target->user_data->cast_to_hit_receiver( );
		if ( hit_target && hit_target != m_ignorable_object )
			hit_target->hit( m_initiator, ray_result.triangle_index, "injury", m_weapon_bullet_damage * m_damage_factor, m_weapon_bullet_pierce * m_pierce_factor, this ); // sushi@TODO: Functions for damage
	}

	if ( m_collided_material->resistance( ) > m_weapon_bullet_pierce * m_pierce_factor )
		return collision_result_collide;
	float new_speed = math::min(	// sushi@NOTE: Use clamp instead
		1.0f,
		math::max( 0.0, m_weapon_bullet_pierce * m_pierce_factor / m_collided_material->resistance( ) - 1.0f )
	) * speed;

	m_current_resistance = m_collided_material->resistance( ); // sushi@NOTE: Gets overwritten by `change_trajectory` :)
	change_trajectory			( collide_point + bullet_direction * math::epsilon_3, bullet_direction * new_speed, collision_time ); // Ensures collision is not triggered immediately?

	start_position	= m_start_position;
	start_time		= m_life_time;
	current_time	-= collision_time;
	return collision_result_pierced;
}

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

	speed					*= math::lerp( m_collided_material->reflection_speed_down( ), 1.0f, calculated_koeff );
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
}

void bullet::change_trajectory( float3 const& new_position, float3 const& new_velocity, float collision_time )
{
	++m_change_trajectory_count;

	m_start_position		= new_position;
	m_start_velocity		= new_velocity;
	m_position				= m_start_position;
	m_velocity				= m_start_velocity;
	m_current_resistance	= m_air_resistance;
	m_born_time_in_ms		+= math::floor( 1000.f * collision_time / m_bullet_manager->get_bullet_time_factor( ) );
	m_life_time				= 0;
}

bool bullet::update_bullet_position( const float time, float3 const& gravity )
{
	float3 const& new_position	= compute_trajectory_position( time, gravity );
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
	ASSERT( UNKNOWN_EXPRESSION );
	m_life_time					= time;

	return						true;
}

} // namespace survarium
