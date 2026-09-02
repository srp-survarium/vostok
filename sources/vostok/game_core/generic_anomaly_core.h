// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GENERIC_ANOMALY_CORE_H_INCLUDED
#define GENERIC_ANOMALY_CORE_H_INCLUDED

#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/player_actions_subscriber.h>
#include <vostok/game_core/link_resolver.h>
#include <vostok/game_core/base_game_object.h>

namespace vostok {
namespace physics {
	struct world;
}
}

namespace survarium {

struct anomaly_state;
class artefact_container_core;
class damage_zone_core;

// sushi@TODO

class generic_anomaly_core : public base_game_object , public link_resolver , public player_actions_subscriber {
public:
								generic_anomaly_core		( );
	virtual						~generic_anomaly_core		( );

			void				load						( configs::binary_config_value const& config );
	virtual	void				resolve_links				( base_project* p, configs::binary_config_value config ) override;

	virtual	void				activate					( physics::world* world, scheduler& scheduler );
	virtual	void				deactivate					( );
	virtual	void				tick						( const u32 time_delta_ms, const u32 current_time_ms );

	inline	physics::world*		physics_world				( ) { return m_physics_world; }
	inline	scheduler&			scheduler					( ) { return *m_scheduler; }

	virtual	void				on_player_action			( hit_receiver const* receiver, player_actions_subscriber::action action, float param ) override;

	inline	void				on_hit_anomaly				( ) { /* no source */ }
	inline	void				on_explosion_in_anomaly		( ) { /* no source */ }
			void				on_zone_act					( damage_zone_core* zone, hit_receiver* receiver );
			void				on_hit_receiver_enter		( hit_receiver* receiver, damage_zone_core* zone );
			void				on_hit_receiver_leave		( hit_receiver* receiver, damage_zone_core* zone );
			void				on_artefact_container_use	( artefact_container_core* container );

	inline	u32					current_time				( ) const { return m_current_time; }
	inline	float				current_energy				( ) const { return m_energy_current; }
	inline	void				set_current_energy			( float energy ) { m_energy_current = energy; }

protected:
	virtual	void				state_changed				( ) { /* no source */ }
	/* 0x0009 */	/* base_game_object */
	/* 0x0000 */	/* link_resolver */
	/* 0x0004 */	/* player_actions_subscriber */
	/* 0x000c */	u32									m_current_time;
	/* 0x0010 */	bool								artefacts_enabled;
	/* 0x0014 */	u32									artefacts_max_count;
	/* 0x0018 */	u32									artefacts_respawn_time_sec;
	/* 0x001c */	u32									m_artefact_grab_time_ms;
	/* 0x0020 */	vector<artefact_container_core *>	m_artefact_containers;
	/* 0x002c */	bool								energy_enabled;
	/* 0x0030 */	float								m_energy_current;
	/* 0x0034 */	u32									energy_decrease_speed;
	/* 0x0038 */	u32									energy_af_container_use;
	/* 0x003c */	u32									energy_on_walk;
	/* 0x0040 */	u32									energy_on_run;
	/* 0x0044 */	u32									energy_on_sprint;
	/* 0x0048 */	u32									energy_on_jump;
	/* 0x004c */	u32									energy_on_shoot;
	/* 0x0050 */	u32									energy_on_character_hit;
	/* 0x0054 */	u32									energy_on_character_kill;
	/* 0x0058 */	u32									energy_on_explosion;
	/* 0x005c */	vector<anomaly_state *>				m_states;
	/* 0x0068 */	anomaly_state*						m_current_state;
	/* 0x006c */	bool								m_was_zone_trigger_event;
	/* 0x006d */	bool								m_was_shoot_trigger_event;

			anomaly_state*		select_state				( );

			void				inc_energy					( float amount );
			void				dec_energy					( float amount );

			void				spawn_artefacts				( );

private:
	/* 0x0070 */	physics::world*						m_physics_world;
	/* 0x0074 */	survarium::scheduler*				m_scheduler;
	/* 0x0078 */	scheduler::identifier				m_scheduler_identifier;
}; // class generic_anomaly_core

STATIC_SIZE_ASSERT(generic_anomaly_core, 0x7C);

} // namespace survarium

#endif // #ifndef GENERIC_ANOMALY_CORE_H_INCLUDED
