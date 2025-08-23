////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_MODEL_H_INCLUDED
#define DAMAGE_MODEL_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>

#include <vostok/unmanaged_allocation_resource.h>
#include <vostok/intrusive_list.h>
#include <vostok/threading_policies.h>

#include "damage_protector.h"
#include "body_part_parameters.h"

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

namespace survarium {
	enum affects_applying_type_enum;
	class bullet;
}

namespace vostok {
	namespace ai {
		struct npc_statistics;
	}
}

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

namespace survarium {

struct affect_subscriber: public boost::noncopyable {
public:
	typedef boost::function<void ( char const *,
		enum hit_affects_type_enum,
		enum affect_event_type_enum )> subscription_functor;

public:
	subscription_functor	subscription_callback;
	affect_subscriber*		next;
};


struct booster_damage_protector: public damage_protector {
public:
	booster_damage_protector(
		pcstr									damage_type,
		float									reduce,
		float									absorb);
	
	float reduce_damage(
		pcstr									__formal,
		pcstr									damage_type,
		float									amount);


	char						m_hit_type[16];
	float						m_reduce;
	float						m_absorb;
	booster_damage_protector*	next;
};

//////////////////////////
//     DAMAGE_MODEL     //
//////////////////////////

class damage_model : public resources::unmanaged_resource, public boost::noncopyable {
public:
	damage_model(
		affects_applying_type_enum         affects_applying_type);

	virtual ~damage_model( );

	void add_body_part(
		body_part_parameters*              new_body_part);

	bool hit_body_part(
		u8                                 initiator,
		pcstr                              part_name,
		pcstr                              damage_type,
		float                              amount,
		float                              armor_piercing,
		u32                                time_in_ms,
		bullet*                            bullet);

	void apply_med_kit(
		pcstr                              part_name,
		float                              amount);

	void tick(
		u32                                time_delta_ms,
		u32                                current_time_in_ms);

	// sushi@TODO
	// void fill_stats(damage_info_type&, u32) const /* no source */;

	void fill_stats(
		vostok::ai::npc_statistics&          stats,
		u32                                current_time_in_ms);

	void dump_stats(
		boost::function<void __cdecl(u32,float,float,pcstr)> callback);

	// sushi@TODO
	// bool is_healthy() const /* no source */;

	void reset( );

	void apply_affect(
		pcstr                              part_name,
		hit_affects_type_enum              affect,
		affect_event_type_enum             event_type);

	void cancel_affect(
		pcstr                              part_name,
		hit_affects_type_enum              affect);

	void subscribe_on_affect(
		hit_affects_type_enum              affect_type,
		affect_subscriber*                 subscriber);

	void unsubscribe_from_affect(
		hit_affects_type_enum              affect_type,
		affect_subscriber*                 subscriber);

	void notify_on_affect_event(
		pcstr                              body_part_name,
		hit_affects_type_enum              affect_type,
		affect_event_type_enum             event_type);

	void add_damage_protector(
		pcstr                              damage_type,
		float                              reduce,
		float                              absorb);

	void register_body_part_damage_protector(
		pcstr                              part_name,
		damage_protector*                  protector);

	void unregister_body_part_damage_protector(
		pcstr                              part_name,
		damage_protector*                  protector);

	// sushi@TODO
	// u8 broken_legs_count() const /* no source */;

	// sushi@TODO
	// u8 broken_hands_count() const /* no source */;

	// sushi@TODO
	// u32 get_parts_count() const /* no source */;

	// sushi@TODO
	// u8 get_last_aggressor_id() const /* no source */;

	affects_applying_type_enum get_affects_applying_type() const {
		return m_affects_applying_type;
	}

	body_part_parameters* get_body_part(
		pcstr                              part_name) const;

	// sushi@TODO
	// u8 get_body_part_index(pcstr) const /* no source */;

	// sushi@TODO
	// pcstr get_body_part_name(u8) const /* no source */;

	body_part_parameters* pop_body_part( );

	u8 get_total_health( );

	// sushi@TODO
	// body_part_parameters* get_body_part_with_min_health() const /* no source */;

	// sushi@TODO: Networking
#if 0 
	void serialize(vostok::network_core::udp_match_packet&, s32) const /* no source */;

	void deserialize(
	 	vostok::network_core::packet_reader& reader);
#endif

	void on_broken_limb_affect(
		pcstr                              bodypart,
		hit_affects_type_enum              affect,
		affect_event_type_enum             type);

public:
	typedef vostok::intrusive_list< body_part_parameters,
		body_part_parameters *,
		&body_part_parameters::next,
		vostok::threading::single_threading_policy,
		vostok::size_policy,
		vostok::no_debug_policy > body_parts;

	typedef vostok::intrusive_list< affect_subscriber,
		affect_subscriber *,
		&affect_subscriber::next,
		vostok::threading::mutex,
		vostok::size_policy,
		vostok::no_debug_policy > affect_subscriptions;

	typedef vostok::intrusive_list< booster_damage_protector,
		booster_damage_protector *,
		&booster_damage_protector::next,
		vostok::threading::single_threading_policy,
		vostok::size_policy,
		vostok::no_debug_policy > damage_protectors;

public:
	// STATE_M[UNVERIFIED]
	/* offset 0x0000 */ /* fields for resources::unmanaged_resource */
	/* offset 0x0108 */ /* fields for boost::noncopyable */
	/* offset 0x0108 */ body_parts							m_body_parts;
	/* offset 0x0118 */ boost::array< 
							affect_subscriptions,
							affect_types_count>				m_affect_subscriptions;
	/* offset 0x02c8 */ affects_applying_type_enum          m_affects_applying_type;
	/* offset 0x02cc */ damage_protectors					m_damage_protectors;

	/* offset 0x02dc */ u32                                 m_last_tick_time_in_ms;
	/* offset 0x02e0 */ u8                                  m_last_hit_initiator;
	/* offset 0x02e8 */ affect_subscriber                   m_leg_damaged_subscriber;
	/* offset 0x0310 */ affect_subscriber                   m_hand_damaged_subscriber;
	/* offset 0x0338 */ u8									m_broken_legs_count[2];
	/* offset 0x033a */ u8									m_broken_hands_count[2];


}; // class damage_model

namespace { 
	typedef char size_assert[
		sizeof(damage_model) == 0x340 ? 1 : -1
	];
}

} // namespace survarium

#endif // #ifndef DAMAGE_MODEL_H_INCLUDED
