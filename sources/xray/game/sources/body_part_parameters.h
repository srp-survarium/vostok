////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BODY_PART_PARAMETERS_H_INCLUDED
#define BODY_PART_PARAMETERS_H_INCLUDED

#include "hit_affects_type_enum.h"
#include "affect_event_type_enum.h"

#include "hit_type_parameters.h" // intrusive_list
#include "affects_threshold.h"	 // intrusive_list
#include "damage_protector.h"	 // intrusive_list

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

namespace stalker2 {
	class damage_model;
}

namespace xray {
	namespace ai {
		struct npc_statistics;
	}
}

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

namespace stalker2 {

class body_part_parameters : public boost::noncopyable {
public:
	body_part_parameters(
		pcstr                              name,
		float                              health,
		float                              regeneration_speed,
		float                              regeneration_timeout,
		bool                               can_be_assigned,
		damage_model&                      owner,
		u8                                 damage_group);

	void add_hit_type(
		hit_type_parameters*               new_hit_type);

	void add_threshold(
		affects_threshold*                 new_threshold);

	void hit_by_type(
		pcstr                              hit_type,
		u32                                time_in_ms,
		float                              amount,
		float                              armor_piercing,
		bool                               __formal,
		damage_protector*                  prot);

	void increase_health(
		float                              amount);

	void decrease_health(
		float                              amount);

	void regenerate(
		u32                                time_delta_ms,
		u32                                current_time_in_ms);

	void dump_state(
		boost::function<void __cdecl(u32,float,float,pcstr)> callback,
		u32                                index);
#if 0
	void dump_state(damage_info_type&, u32) const /* no source */;
#endif
	void dump_state(
		xray::ai::npc_statistics&          stats,
		u32                                current_time_in_ms);
#if 0
	void remove_edges(body_part_parameters*) /* no source */;
#endif
	void reset( );

	void apply_affect_by_force(
		hit_affects_type_enum              affect,
		affect_event_type_enum             event_type,
		u32                                current_time_in_ms);

	bool can_affect_death( );

	bool has_affect_protector(
		hit_affects_type_enum              affect);

	u8 get_health_in_percentage( );

	void cancel_affect_by_force(
		hit_affects_type_enum              affect);

	void add_damage_protector(
		damage_protector*                  protector);

	void remove_damage_protector(
		damage_protector*                  protector);

	pcstr get_name() const { return m_name.c_str(); } /* sushi@TODO: Is this impl buggy for cases where name is 16 chars? */


	bool is_healthy() const { return m_health == m_max_health; } /* sushi@TODO: Maybe? Try to find usage, since this is inlined */

	float relative_health() const { return m_health; } /* sushi@TODO: I don't know what this might mean */;

	float get_max_health() const { return m_max_health; }

	u8 damage_group() const { return m_damage_group; }

	float get_regeneration_speed() const { return m_regeneration_speed; }

	hit_type_parameters* pop_hit_type( );

	affects_threshold* pop_threshold( );

	// color get_health_level_color() const /* sushi@TODO: Checkout color */;

	bool is_affect_applied(
		hit_affects_type_enum              affect);

	hit_type_parameters* get_hit_parameters(
		pcstr                              hit_type);

	void set_parameters(
		float                              max_health,
		float                              regeneration_speed);

	/* sushi@TODO: Serialization is skipped for now
	void serialize(
		xray::network_core::udp_match_packet& packet,
		s32                                client_offset);

	void deserialize(
		xray::network_core::packet_reader& reader);
	*/

	void check_affects(
		u32                                current_time_in_ms);

	void update_affects(
		u32                                current_time_in_ms);

	void apply_affects(
		affects_threshold const*           threshold_reached,
		u32                                current_time_in_ms);

public:
	typedef xray::intrusive_list<
		hit_type_parameters,
		hit_type_parameters*,
		&hit_type_parameters::next,
		xray::threading::single_threading_policy,
		xray::size_policy,
		xray::no_debug_policy >  hit_type_parameters_type;

	typedef xray::intrusive_list<
		affects_threshold,
		affects_threshold*,
		&affects_threshold::next,
		xray::threading::single_threading_policy,
		xray::size_policy,
		xray::no_debug_policy >  affects_threshold_type;

	typedef xray::fixed_vector<
		std::pair< hit_affects_type_enum, u32 >,
		8 >											hit_affects_types;

	typedef xray::intrusive_list<
		damage_protector,
		damage_protector*,
		&damage_protector::next,
		xray::threading::single_threading_policy,
		xray::size_policy,
		xray::no_debug_policy >  damage_protector_type;

public:
	// STATE_M[VERIFIED]
	body_part_parameters*				next;
	hit_type_parameters_type			m_hit_types;
	affects_threshold_type				m_thresholds;
	hit_affects_types					m_affects;
	damage_model&                       m_damage_model;
	xray::fixed_string<16>				m_name;
	float                               m_max_health;
	float                               m_health;
	float                               m_regeneration_speed;	/* health per second */
	u32                                 m_regeneration_timeout;
	u32                                 m_last_hit_time;
	float                               m_last_hit_health;
	bool                                m_assignable;
	u8									m_damage_group;
	damage_protector_type				m_damage_protectors;

}; // class body_part_parameters 

namespace {
	typedef char size_assert[
		sizeof(body_part_parameters) == 0xB8 ? 1 : -1
	];
}

} // namespace stalker2

#endif // #ifndef BODY_PART_PARAMETERS_H_INCLUDED