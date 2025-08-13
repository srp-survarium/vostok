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

namespace stalker2 {

// TODO

struct affect_subscriber: public boost::noncopyable {
public:
	affect_subscriber* next;
};

// FORWARD DECLARATIONS

enum affects_applying_type_enum;
class damage_protector;

// DEFINITIONS

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
	char m_hit_type[16];
	float m_reduce;
	float m_absorb;
	booster_damage_protector* next;
};



class damage_model : public xray::resources::unmanaged_resource, public boost::noncopyable {
public:

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
	unsigned __int8							m_last_hit_initator;
	affect_subscriber						m_leg_damaged_subscriber;
	affect_subscriber						m_hand_damaged_subscriber;
	unsigned __int8							m_broken_legs_count[2];
	unsigned __int8							m_broken_hands_count[2];

}; // class damage_model

} // namespace stalker2

#endif // #ifndef DAMAGE_MODEL_H_INCLUDED