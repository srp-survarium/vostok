////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_model.h"
#include "bullet.h"

#include <xray/strings_functions.h>
#include <xray/math_functions_inline.h>
#include <xray/resources.h>

namespace stalker2 {

// damage_model

damage_model::damage_model(
	affects_applying_type_enum affects_applying_type
	) :
	xray::resources::unmanaged_resource				(1),
	m_affect_subscriptions							( ),
	m_affects_applying_type							(affects_applying_type),
	// ???
	m_last_tick_time_in_ms							(0),
	m_last_hit_initiator							(255), // -1?
	// ???
	m_broken_legs_count								( ),
	m_broken_hands_count							( )
{
	// 11
	subscribe_on_affect( affects_type_leg_damage, &m_leg_damage_subscriber );
	subscribe_on_affect( affects_type_hand_damage, &m_hand_damage_subscriber );
}

damage_model::~damage_model()
{
	// 5
}

body_part* damage_model::pop_body_part()
{
	// 1
	return NULL;
}

void damage_model::tick(
	unsigned int time_delta_in_ms,
	unsigned int current_time_in_ms
	)
{
	// 9
}

void damage_model::add_body_part(
	body_part* new_body_part
	)
{
	// 1
}

body_part_parameters* damage_model::get_body_part(
	const char* part_name
	)
{
	// 2
	return NULL;
}

void damage_model::hit_body_part(
	unsigned __int8 initiator,
    const char* part_name,
	const char* damage_type,
	float amount,
	float armor_piercing,
	unsigned int time_in_ms,
	bullet const* bullet
	)
{
	// 21
}

void damage_model::apply_med_kit(
	const char *part_name,
	float amount
	)
{
	// 3
}

void damage_model::fill_stats(
	options_tab* stats,
	unsigned int current_time_in_ms
	)
{
	// 7
}

void damage_model::dump_stats(
	boost::function< void ( unsigned int, float, float, char const* ) > callback
	)
{
	// 7
}

void damage_model::subscribe_on_affect(
	hit_affects_type_enum affect_type,
	affect_subscriber const* subscriber
	)
{
	// 1
}

void damage_model::unsubscribe_from_affect(
	hit_affects_type_enum affect_type,
	affect_subscriber const* subscriber
	)
{
	// 3
}

void damage_model::notify_on_affect_event(
	const char*					body_part_name,
	hit_affects_type_enum		affect_type,
	affect_event_type_enum		event_type
	)
{
	// 9
}

void damage_model::reset( )
{
	// 9
}

void damage_model::apply_affect(
	const char*					part_name,
	hit_affects_type_enum		affect,
	affect_event_type_enum		event_type
	)
{
	// 3
}

unsigned __int8 damage_model::get_total_health(	)
{
	// 8
	return 0;
}

void damage_model::cancel_affect(
	const char*					part_name,
	hit_affects_type_enum		affect
	)
{
	// 3
}

void damage_model::register_body_part_damage_protector(
	const char*					part_name,
	damage_protector*			protector
	)
{
	// 3
}

void damage_model::unregister_body_part_damage_protector(
	const char*					part_name,
	damage_protector*			protector
	)
{
	// 3
}

void damage_model::add_damage_protector(
	const char*					damage_type,
	float						reduce,
	float						absorb
	)
{
	// 12
}

void damage_model::on_broken_limb_affect(
	const char*					bodypart,
	hit_affects_type_enum		affect,
	affect_event_type_enum		type
	)
{
	// 20
}

void damage_model::deserialize(
	xray::network_core::packet_reader* reader
	)
{
	// 1
}

// booster_damage_protector
	

booster_damage_protector::booster_damage_protector(const char* damage_type, float reduce, float absorb):
	m_reduce(reduce),
	m_absorb(absorb)
{
	reduce_damage_functor = boost::bind( &booster_damage_protector::reduce_damage, this, _1, _2, _3 );
	strings::copy( (pstr)&m_hit_type, 16, damage_type); // damage_type can be less then 16, is this safe?
}
	
float booster_damage_protector::reduce_damage(const char* __formal, const char* damage_type, float amount)
{
	if (strings::equal(damage_type, m_hit_type))
		return math::max(0.0f, amount * m_reduce - m_absorb);
	else
		return amount;
}


} // namespace stalker2 