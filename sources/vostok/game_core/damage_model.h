////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_MODEL_H_INCLUDED
#define DAMAGE_MODEL_H_INCLUDED

#include <boost/array.hpp>

#include <vostok/game_core/damage_protector.h>
#include <vostok/game_core/body_part_parameters.h>
#include <vostok/game_core/hit_affects_type_enum.h>
#include <vostok/game_core/affect_event_type_enum.h>

namespace survarium {
	enum affects_applying_type_enum;
	class bullet;
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

struct affect_subscriber: private boost::noncopyable {
public:
	typedef boost::function<void (
		pcstr,
		hit_affects_type_enum,
		affect_event_type_enum
	)> affect_event_callback_type;

public:
	inline	explicit affect_subscriber	( affect_event_callback_type const& subscription_callback ) : subscription_callback( subscription_callback ), next( NULL ) { }
	inline	         affect_subscriber	( ) : next( NULL ) { }

public:
	affect_event_callback_type	subscription_callback;
	affect_subscriber*		next;
};

STATIC_SIZE_ASSERT(affect_subscriber, 0x28);

struct booster_damage_protector : public damage_protector {
public:
	explicit			booster_damage_protector	( pcstr damage_type, float reduce, float absorb ) :
							m_reduce	( reduce ),
							m_absorb	( absorb )
	{
		reduce_damage_functor = boost::bind( &booster_damage_protector::reduce_damage, this, _1, _2, _3, _4 );
		strings::copy( (pstr)&m_hit_type, 16, damage_type ); // sushi@TODO: damage_type can be less then 16, is this safe?
	}

	inline	float		reduce_damage				(
							pcstr		body_part_name,
							pcstr		damage_type,
							const float	amount,
							const float	armor_piercing
						)
	{
		return strings::equal( damage_type, m_hit_type ) ? math::max( 0.0f, amount * m_reduce - m_absorb ) : amount;
	}


public:
	/* 0x0000 */	/* damage_protector */
	/* 0x0050 */	char						m_hit_type[16];
	/* 0x0060 */	float						m_reduce;
	/* 0x0064 */	float						m_absorb;
	/* 0x0068 */	booster_damage_protector*	next;
}; // struct booster_damage_protector

STATIC_SIZE_ASSERT(booster_damage_protector, 0x70);


class damage_model : public resources::unmanaged_resource, private boost::noncopyable {
public:
									damage_model					( const affects_applying_type_enum affects_applying_type );
	virtual							~damage_model					( );

			void					add_body_part					( body_part_parameters* const new_body_part );
			bool					hit_body_part					(
										const u8			initiator,
										pcstr		part_name,
										pcstr		damage_type,
										const float		amount,
										const float		armor_piercing,
										const u32			time_in_ms,
										bullet*		const bullet
									);

			void					apply_med_kit					( pcstr part_name, const float amount );

			void					tick							( const u32 time_delta_ms, const u32 current_time_in_ms );

			void					fill_stats						( ai::npc_statistics& stats, const u32 current_time_in_ms ) const;
	inline	void					fill_stats						( damage_info_type& arg_0, const u32 arg_1 ) const { /* no source */ }
			void					dump_stats						( boost::function<void( u32, float, float, pcstr )> callback );

	inline	bool					is_healthy						( ) const { /* no source */ }

			void					reset							( );

			void					apply_affect					( pcstr part_name, const hit_affects_type_enum affect, const affect_event_type_enum event_type );
			void					cancel_affect					( pcstr part_name, const hit_affects_type_enum affect );

			void					subscribe_on_affect				( const hit_affects_type_enum affect_type, affect_subscriber* const subscriber );
			void					unsubscribe_from_affect			( const hit_affects_type_enum affect_type, affect_subscriber* const subscriber );

			void					notify_on_affect_event			( pcstr body_part_name, const hit_affects_type_enum affect_type, const affect_event_type_enum event_type );
			void					add_damage_protector			( pcstr damage_type, float reduce, float absorb );

			void					register_body_part_damage_protector		( pcstr part_name, damage_protector* protector );
			void					unregister_body_part_damage_protector	( pcstr part_name, damage_protector* protector );

	inline	u8						broken_legs_count				( ) const { return m_broken_legs_count[0] + m_broken_legs_count[1]; }
	inline	u8						broken_hands_count				( ) const { return m_broken_hands_count[0] + m_broken_hands_count[1]; }
	inline	u32						get_parts_count					( ) const { /* no source */ }
	inline	u8						get_last_aggressor_id			( ) const { /* no source */ }

	inline	affects_applying_type_enum		get_affects_applying_type	( ) const { return m_affects_applying_type; }

			body_part_parameters*	get_body_part					( pcstr part_name ) const;
	inline	u8						get_body_part_index				( pcstr arg_0 )		const { /* no source */ }
	inline	pcstr					get_body_part_name				( const u8 arg_0 )	const { /* no source */ }

			body_part_parameters*	pop_body_part					( );

			u8						get_total_health				( );

	inline	body_part_parameters*	get_body_part_with_min_health	( ) const { /* no source */ }

			void					serialize				( network_core::udp_match_packet&, s32 ) const { /* no source */ }
			void					deserialize				( network_core::packet_reader& reader );

public:
	typedef vostok::intrusive_list< body_part_parameters,
		body_part_parameters *,
		&body_part_parameters::next,
		vostok::threading::single_threading_policy,
		vostok::size_policy,
		vostok::no_debug_policy > body_parts_type;

	typedef vostok::intrusive_list< affect_subscriber,
		affect_subscriber *,
		&affect_subscriber::next,
		vostok::threading::mutex,
		vostok::size_policy,
		vostok::no_debug_policy > affect_subscribers_type;

	typedef vostok::intrusive_list< booster_damage_protector,
		booster_damage_protector *,
		&booster_damage_protector::next,
		vostok::threading::single_threading_policy,
		vostok::size_policy,
		vostok::no_debug_policy > booster_damage_protectors_type;

	typedef boost::array< affect_subscribers_type, affect_types_count > affect_subscriptions_type;

private:
			void					on_broken_limb_affect	( pcstr bodypart, const hit_affects_type_enum affect, const affect_event_type_enum type );

private:
	/* 0x0108 */	body_parts_type					m_body_parts;
	/* 0x0118 */	affect_subscriptions_type		m_affect_subscriptions;
	/* 0x02c8 */	affects_applying_type_enum		m_affects_applying_type;
	/* 0x02cc */	booster_damage_protectors_type	m_damage_protectors;
	/* 0x02dc */	u32								m_last_tick_time_in_ms;
	/* 0x02e0 */	u8								m_last_hit_initiator;
	/* 0x02e8 */	affect_subscriber				m_leg_damaged_subscriber;
	/* 0x0310 */	affect_subscriber				m_hand_damaged_subscriber;
	/* 0x0338 */	u8								m_broken_legs_count[2];
	/* 0x033a */	u8								m_broken_hands_count[2];

	friend struct player_parameters_modifyer;

}; // class damage_model

STATIC_SIZE_ASSERT(damage_model, 0x340);


typedef resources::resource_ptr< damage_model, resources::unmanaged_intrusive_base > damage_model_ptr;

} // namespace survarium

#endif // #ifndef DAMAGE_MODEL_H_INCLUDED
