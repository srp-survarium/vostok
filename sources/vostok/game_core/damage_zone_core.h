////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_ZONE_CORE_H_INCLUDED
#define DAMAGE_ZONE_CORE_H_INCLUDED

#include <vostok/math_curve.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/collision_sensor.h>
#include <vostok/game_core/player_actions_subscriber.h>
#include <vostok/game_core/hit_initiator.h>

namespace vostok {
namespace physics {
	struct world;
	class base_physics_object;
}
}

namespace survarium {
	struct hit_receiver_info;
	struct zone_group;
	class game_scene;
}

namespace survarium {

enum apply_hit_type {
	on_enter			= 0x0000,
	on_inside			= 0x0001,
	on_motion_inside	= 0x0002,
};


class damage_zone_core : public collision_sensor , public hit_initiator , public player_actions_subscriber {
public:
							damage_zone_core			( );
	virtual					~damage_zone_core			( );

	virtual	void			tick						( const u32 frame_delta, const u32 current_time ) override;

	virtual	bool			is_filter_passed			( physics::base_physics_object* object ) const override;

	virtual	void			load						( configs::binary_config_value const& t ) override;

	virtual	void			activate					( zone_group* owner, physics::world* p_world, scheduler& scheduler );
	virtual	void			deactivate					( );

	virtual	void			on_player_action			( hit_receiver const* receiver, player_actions_subscriber::action action, float param ) override;

	inline	void			set_resources				( resources::queries_result& arg_0, u32 arg_1 ) { /* no source */ }

	inline	void			set_standalone				( bool standalone ) { m_standalone = standalone; }
	inline	bool			is_standalone				( ) const { return m_standalone; }

	inline	zone_group*		owner						( ) { /* no source */ }

private:
	virtual	void			on_inside					( buffer_vector<physics::base_physics_object *> const& objects ) override;
	virtual	void			on_leave					( buffer_vector<physics::base_physics_object *> const& objects ) override;
	virtual	void			on_enter					( buffer_vector<physics::base_physics_object *> const& objects ) override;

	inline	void			remove_null_receivers		( ) { /* no source */ }
	inline	void			remove_left_receivers		( buffer_vector<physics::base_physics_object *> const& arg_0 ) { /* no source */ }

	inline	float			calc_armor_piercing			( float arg_0 ) const { /* no source */ }

	inline	void			hit							( float arg_0 ) { /* no source */ }
			void			hit_on_enter				( const u32 frame_delta, const u32 current_time );
			void			hit_on_inside				( const u32 frame_delta, const u32 current_time );
			void			hit_on_motion_inside		( const u32 frame_delta, const u32 current_time );

	struct damage_zone_user_data {
		/* 0x0000 */	configs::binary_config_value const*	config;
		/* 0x0004 */	physics::world*						physics_world;
		/* 0x0008 */	game_scene*							game_scene;
	}; // struct damage_zone_user_data

private:
	/* 0x0000 */	/* collision_sensor */
	/* 0x0024 */	/* hit_initiator */
	/* 0x0030 */	/* player_actions_subscriber */
	/* 0x0038 */	math::curve_line_float				m_hit_curve;
	/* 0x0058 */	math::curve_line_float				m_motion_on_bound_curve;
	/* 0x0078 */	math::curve_line_float				m_motion_on_center_curve;
	/* 0x0098 */	physics::world*						m_physics_world;
	/* 0x009c */	zone_group*							m_owner;
	/* 0x00a0 */	vector< hit_receiver_info >			m_receivers;
	/* 0x00ac */	vector< fixed_string<16> >			m_body_parts_filter;
	/* 0x00b8 */	fixed_string<32>					m_damage_type;
	/* 0x00e4 */	apply_hit_type						m_apply_hit_type;
	/* 0x00e8 */	float								m_max_hit;
	/* 0x00ec */	float								m_min_hit;
	/* 0x00f0 */	float								m_max_armor_piercing;
	/* 0x00f4 */	float								m_min_armor_piercing;
	/* 0x00f8 */	u32									m_accumulated_hit_time_ms;
	/* 0x00fc */	u32									m_hit_interval_ms;
	/* 0x0100 */	scheduler::identifier				m_scheduler_identifier;
	/* 0x0104 */	scheduler*							m_scheduler;
	/* 0x0108 */	bool								m_standalone;
}; // class damage_zone_core

STATIC_SIZE_ASSERT(damage_zone_core, 0x110);

typedef resources::resource_ptr< damage_zone_core, resources::unmanaged_intrusive_base > damage_zone_core_ptr;

} // namespace survarium

#endif // #ifndef DAMAGE_ZONE_CORE_H_INCLUDED
