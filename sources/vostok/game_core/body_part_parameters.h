// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BODY_PART_PARAMETERS_H_INCLUDED
#define BODY_PART_PARAMETERS_H_INCLUDED

#include <vostok/game_core/hit_affects_type_enum.h>
#include <vostok/game_core/affect_event_type_enum.h>
#include <vostok/game_core/hit_type_parameters.h>
#include <vostok/game_core/affects_threshold.h>
#include <vostok/game_core/damage_protector.h>

namespace survarium {
	class damage_model;
	struct damage_info_type;
}

namespace vostok {
namespace ai {
	struct npc_statistics;
}
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}

namespace survarium {

struct remove_vertex_from_hit_parameters_predicate : private boost::noncopyable {
	inline explicit remove_vertex_from_hit_parameters_predicate( body_part_parameters* vertex ) :
		vertex( vertex )
	{
	}

	inline void operator()( hit_type_parameters* const hit_parameters ) const
	{
		hit_parameters->remove_vertex( vertex );
	}

public:
	/* 0x0000 */ body_part_parameters*	vertex;
};

STATIC_SIZE_ASSERT(remove_vertex_from_hit_parameters_predicate, 0x4);

typedef vostok::intrusive_list<
	hit_type_parameters,
	hit_type_parameters*,
	&hit_type_parameters::next,
	vostok::threading::single_threading_policy,
	vostok::size_policy,
	vostok::no_debug_policy > hit_type_parameters_list;

typedef vostok::intrusive_list<
	affects_threshold,
	affects_threshold*,
	&affects_threshold::next,
	vostok::threading::single_threading_policy,
	vostok::size_policy,
	vostok::no_debug_policy > affects_threshold_list;

typedef vostok::intrusive_list<
	damage_protector,
	damage_protector*,
	&damage_protector::next,
	vostok::threading::single_threading_policy,
	vostok::size_policy,
	vostok::no_debug_policy > damage_protector_list;

class body_part_parameters : private boost::noncopyable {
	typedef boost::function< void( u32, float, float, pcstr ) > player_damage_model_callback_type;

public:
									body_part_parameters	(
										pcstr				name,
										const float				health,
										const float				regeneration_speed,
										const float				regeneration_timeout,
										const bool				can_be_assigned,
										damage_model&		owner,
										const u8					damage_group
									);

			void					add_hit_type			( hit_type_parameters* const new_hit_type );
			void					add_threshold			( affects_threshold* const new_threshold );

			void					hit_by_type				(
										pcstr					hit_type,
										const u32						time_in_ms,
										const float					amount,
										const float					armor_piercing,
										const bool					__formal,
										damage_protector*		prot
									);

			void					increase_health			( const float amount );
			void					decrease_health			( const float amount );

			void					regenerate				( const u32 time_delta_ms, const u32 current_time_in_ms );

			void					dump_state				( ai::npc_statistics& stats, const u32 current_time_in_ms ) const;
	inline	void					dump_state				( damage_info_type& arg_0, const u32 arg_1 ) const { /* no source */ }
			void					dump_state				( boost::function< void( u32, float, float, pcstr ) > callback, const u32 index ) const;

	inline	void					remove_edges			( body_part_parameters* arg_0 ) { /* no source */ }

			void					reset					( );

			void					apply_affect_by_force	( const hit_affects_type_enum affect, const affect_event_type_enum event_type, const u32 current_time_in_ms );

			bool					can_affect_death		( );
			bool					has_affect_protector	( const hit_affects_type_enum affect );
			u8						get_health_in_percentage( );
			void					cancel_affect_by_force	( const hit_affects_type_enum affect );

			void					add_damage_protector	( damage_protector* protector );
			void					remove_damage_protector	( damage_protector* protector );

	inline	pcstr					get_name				( ) const { return m_name.c_str( ); }

	inline	bool					is_healthy				( ) const { return m_health == m_max_health; } // sushi@TODO: This is an assumption before we find actual usage
	inline	float					relative_health			( ) const { return m_health; }
	inline	float					get_max_health			( ) const { return m_max_health; }

	inline	u8						damage_group			( ) const { return m_damage_group; }

	inline	float					get_regeneration_speed	( ) const { return m_regeneration_speed; }

			hit_type_parameters*	pop_hit_type			( );
			affects_threshold*		pop_threshold			( );

	inline	math::color				get_health_level_color	( ) const { /* no source */ }

			bool					is_affect_applied		( const hit_affects_type_enum affect );

			hit_type_parameters*	get_hit_parameters		( pcstr hit_type ) const;
			void					set_parameters			( float max_health, float regeneration_speed );

			void					serialize				( network_core::udp_match_packet& packet, s32 client_offset ) const;
			void					deserialize				( network_core::packet_reader& reader );

public:
	/* 0x0000 */	body_part_parameters*			next;

private:
			void					check_affects			( const u32 current_time_in_ms );
			void					update_affects			( const u32 current_time_in_ms );
			void					apply_affects			( affects_threshold const* threshold_reached, const u32 current_time_in_ms );

			template < class stats_item_type >
			void					fill_new_stats_item		( stats_item_type& new_stats_item, const u32 current_time_in_ms ) const;

	typedef hit_type_parameters_list hit_type;
	typedef affects_threshold_list thresholds_type;
	typedef std::pair< hit_affects_type_enum, u32 > affect_type;
	typedef vostok::fixed_vector< affect_type, 8 > affects_type;

	/* 0x0004 */	hit_type_parameters_list		m_hit_types;
	/* 0x0014 */	affects_threshold_list			m_thresholds;
	/* 0x0024 */	vostok::fixed_vector< std::pair< hit_affects_type_enum, u32 >, 8 >	m_affects;
	/* 0x006c */	damage_model&					m_damage_model;
	/* 0x0070 */	fixed_string<16>				m_name;
	/* 0x008c */	float							m_max_health;
	/* 0x0090 */	float							m_health;
	/* 0x0094 */	float							m_regeneration_speed; /* health per second */
	/* 0x0098 */	u32								m_regeneration_timeout;
	/* 0x009c */	u32								m_last_hit_time;
	/* 0x00a0 */	float							m_last_hit_health;
	/* 0x00a4 */	const bool						m_assignable;
	/* 0x00a5 */	const u8						m_damage_group;
	/* 0x00a8 */	damage_protector_list			m_damage_protectors;
}; // class body_part_parameters

STATIC_SIZE_ASSERT(body_part_parameters, 0xB8);

} // namespace survarium

#endif // #ifndef BODY_PART_PARAMETERS_H_INCLUDED
