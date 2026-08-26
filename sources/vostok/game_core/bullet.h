////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <vostok/game_core/weapon_ammunition.h>

namespace vostok {
namespace physics {
	struct closest_ray_result;
}
}

namespace survarium {

class weapon_core;

class body_part_parameters;
class bullet_manager;
class game_material;
struct hit_initiator;
struct hit_receiver;

enum triangle_orientation {
	triangle_orientation_front_face	= 0x0,
	triangle_orientation_back_face	= 0x1,
};

enum collision_result {
	collision_result_no_collision	= 0x0,
	collision_result_collide		= 0x1,
	collision_result_pierced		= 0x2,
	collision_result_reflected		= 0x3,
};

class bullet : private boost::noncopyable {
public:
	explicit							bullet						(
											bullet_manager&					bullet_manager,
											float3 const&					position,
											float3 const&					velocity,
											const u32						born_time_in_ms,
											float							air_resistance,
											weapon_ammunition_ptr const&	wa,
											weapon_core const&				wc,
											hit_initiator const* const		initiator,
											hit_receiver const*	const		ignorable_object
										);
	explicit							bullet						( bullet const& other );
										~bullet						( );

			void						tick						( u32 current_time_in_ms );
			bool						is_finish_flying			( ) const;

	inline	float3 const&				get_start_position			( ) const { return m_start_position;	}
	inline	float3 const&				get_start_velocity			( ) const { return m_start_velocity;	}
	inline	float3 const&				get_position				( ) const { return m_position;			}
	inline	float3 const&				get_velocity				( ) const { return m_velocity;			}

			float						pick_next_permissible_time	( float low_time, float high_time, float3 const& gravity );

	inline	u8							get_initiator_id			( ) const { return m_initiator->id; }

	inline	body_part_parameters*		last_hitted_body_part		( ) const { return m_last_hitted_body_part; }
	inline	void						last_hitted_body_part		( body_part_parameters* const last_hitted_body_part ) { m_last_hitted_body_part = last_hitted_body_part; }

private:
			float3						compute_parabolic_velocity	( float time, float3 const& gravity );
			float3						compute_trajectory_velocity	( float time, float3 const& gravity );
			float3						compute_parabolic_position	( float time, float3 const& gravity );
			float3						compute_trajectory_position	( float time, float3 const& gravity );

			float						get_parabolic_time			( );
			bool						pick_permissible_range		(
											float&				result,
											float				low_time,
											float				high_time,
											float3 const&		gravity
										);

			float						get_check_time				( const float start_low, float high, float3 const& gravity );
			float						get_check_time_in_vacuum	( float start_low, float high, float3 const& gravity );

			float						compute_max_error			( float low, float high, float3 const& gravity );

			collision_result			check_collision				( float3 start_position, float start_time, float current_time );

			collision_result			process_ray_query			(
											physics::closest_ray_result const&	ray_result,
											float								distance,
											float3&								start_position,
											float3&								fly_direction,
											float&								start_time,
											float&								current_time
										);

			void						fix_collision_point_and_time(
											float3&					collide_point,
											float&					collision_time,
											float					start_time,
											float					current_time,
											const triangle_orientation orientation,
											float3 const&			triangle_normal,
											float3 const&			gravity
										);

			collision_result			collide_front_face			(
											float3 const&						collide_point,
											float3 const&						bullet_direction,
											float3 const&						triangle_normal,
											float								speed,
											float								collision_time,
											float3&								start_position,
											float&								start_time,
											float&								current_time,
											physics::closest_ray_result const&	ray_result
										);

			collision_result			try_reflect					(
											float3 const&		collide_point,
											float3				direction,
											float3 const&		triangle_normal,
											float				speed,
											float				collision_time,
											float3&				start_position,
											float&				start_time,
											float&				current_time,
											float				cos_alpha
										);

			void						change_trajectory			( float3 const& new_position, float3 const& new_velocity, float collision_time );
			bool						update_bullet_position		( float time, float3 const& gravity );


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float3						m_position;
	/* 0x000c */	float3						m_velocity;
	/* 0x0018 */	float3						m_start_position;
	/* 0x0024 */	float3						m_start_velocity;
	/* 0x0030 */	bullet_manager*				m_bullet_manager;
	/* 0x0034 */	game_material const*		m_bullet_material;
	/* 0x0038 */	game_material const*		m_collided_material;
	/* 0x003c */	hit_initiator const*		m_initiator;
	/* 0x0040 */	hit_receiver const*			m_ignorable_object;
	/* 0x0044 */	body_part_parameters*		m_last_hitted_body_part;
	/* 0x0048 */	u32							m_born_time_in_ms;
	/* 0x004c */	u32							m_current_time_in_ms;
	/* 0x0050 */	float						m_life_time;
	/* 0x0054 */	float						m_air_resistance;
	/* 0x0058 */	float						m_current_resistance;
	/* 0x005c */	float						m_max_distance;
	/* 0x0060 */	float						m_flown_distance;
	/* 0x0064 */	float						m_damage_factor;
	/* 0x0068 */	float						m_ricochet_angle;
	/* 0x006c */	float						m_pierce_factor;
	/* 0x0070 */	float						m_weapon_bullet_damage;
	/* 0x0074 */	float						m_weapon_bullet_pierce;
	/* 0x0078 */	u32							m_change_trajectory_count;
	/* 0x007c */	u8							m_ricochet_count;
public:
	/* 0x007e */	u16							m_tracer_idx;
}; // class bullet

STATIC_SIZE_ASSERT(bullet, 0x80);

} // namespace survarium

#endif // #ifndef BULLET_H_INCLUDED
