////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_MODEL_H_INCLUDED
#define DAMAGE_MODEL_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>

#include <xray/unmanaged_allocation_resource.h>
#include <xray/intrusive_list.h>
#include <xray/threading_policies.h>

#include "damage_protector.h"
#include "body_part_parameters.h"

// TODO
namespace xray {
namespace network_core {
	class packet_reader {};
}
}

namespace stalker2 {

// TODO
typedef void options_tab;
typedef void body_part;


// FORWARD DECLARATIONS

enum affects_applying_type_enum;
class damage_protector;
class bullet;

// DEFINITIONS

struct affect_subscriber: public boost::noncopyable { // @TODO: Possibly move to its own header
public:
	typedef boost::function<void ( char const *,
		enum hit_affects_type_enum,
		enum affect_event_type_enum )> subscription_functor;

	subscription_functor	subscription_callback;
	affect_subscriber*		next;
};


struct booster_damage_protector: public damage_protector {
public:
	booster_damage_protector(
		const char* damage_type,
		float reduce,
		float absorb
		);
	
	float reduce_damage(
		const char* __formal,
		const char* damage_type,
		float amount
		);

public:
	char						m_hit_type[16];
	float						m_reduce;
	float						m_absorb;
	booster_damage_protector*	next;
};


class damage_model : public xray::resources::unmanaged_resource, public boost::noncopyable {
public:
	damage_model(
		affects_applying_type_enum affects_applying_type
		);

	~damage_model();

	body_part* pop_body_part();

	void tick(
		unsigned int time_delta_in_ms,
		unsigned int current_time_in_ms
		);

	void add_body_part(
		body_part* new_body_part
		);

	body_part_parameters* get_body_part(
		const char* part_name
		);

	void hit_body_part(
		unsigned __int8 initiator,
	    const char* part_name,
		const char* damage_type,
		float amount,
		float armor_piercing,
		unsigned int time_in_ms,
		bullet const* bullet
		);

	void apply_med_kit(
		const char* part_name,
		float amount
		);

	void fill_stats(
		options_tab* stats,
		unsigned int current_time_in_ms
		);

	void dump_stats(
		boost::function< void ( unsigned int, float, float, char const* ) > callback
		);

	void subscribe_on_affect(
		hit_affects_type_enum affect_type,
		affect_subscriber const* subscriber
		);

	void unsubscribe_from_affect(
		hit_affects_type_enum affect_type,
		affect_subscriber const* subscriber
		);

	void notify_on_affect_event(
		const char*					body_part_name,
		hit_affects_type_enum		affect_type,
		affect_event_type_enum		event_type
		);

	void reset( );

	void apply_affect(
		const char*					part_name,
		hit_affects_type_enum		affect,
		affect_event_type_enum		event_type
		);

	unsigned __int8 get_total_health(	);

	void cancel_affect(
		const char*					part_name,
		hit_affects_type_enum		affect
		);

	void register_body_part_damage_protector(
		const char*					part_name,
		damage_protector*			protector
		);

	void unregister_body_part_damage_protector(
		const char*					part_name,
		damage_protector*			protector
		);

	void add_damage_protector(
		const char*					damage_type,
		float						reduce,
		float						absorb
		);

	void on_broken_limb_affect(
		const char*					bodypart,
		hit_affects_type_enum		affect,
		affect_event_type_enum		type
		);

	void deserialize(
		xray::network_core::packet_reader* reader
		);

private:
	typedef xray::intrusive_list< body_part_parameters,
		body_part_parameters *,
		&body_part_parameters::next,
		xray::threading::single_threading_policy,
		xray::size_policy > body_parts;

	typedef xray::intrusive_list< affect_subscriber,
		affect_subscriber *,
		&affect_subscriber::next,
		xray::threading::mutex,
		xray::size_policy > affect_subscriptions;

	typedef xray::intrusive_list< booster_damage_protector,
		booster_damage_protector *,
		&booster_damage_protector::next,
		xray::threading::single_threading_policy,
		xray::size_policy > damage_protectors;
	
private:
	body_parts								m_body_parts;
	boost::array<affect_subscriptions, 9>	m_affect_subscriptions;
	affects_applying_type_enum				m_affects_applying_type;
	damage_protectors						m_damage_protectors;

	unsigned int							m_last_tick_time_in_ms;
	unsigned __int8							m_last_hit_initiator;
	affect_subscriber						m_leg_damage_subscriber;
	affect_subscriber						m_hand_damage_subscriber;
	unsigned __int8							m_broken_legs_count[2];
	unsigned __int8							m_broken_hands_count[2];

}; // class damage_model

} // namespace stalker2

#endif // #ifndef DAMAGE_MODEL_H_INCLUDED