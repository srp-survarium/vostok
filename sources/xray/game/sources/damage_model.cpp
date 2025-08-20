////////////////////////////////////////////////////////////////////////////
//	Created 	: 14.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_model.h"

#include "body_part_parameters.h"
#include "bullet.h"

#include <xray/ai/npc_statistics.h>


//
// booster_damage_protector sushi@TODO: Most likely was defined in header
//

// STATE[UNVERIFIED]
stalker2::booster_damage_protector::booster_damage_protector(
	pcstr									damage_type,
	float									reduce,
	float									absorb):
	m_reduce								(reduce),
	m_absorb								(absorb)
{
	reduce_damage_functor = boost::bind(&booster_damage_protector::reduce_damage, this, _1, _2, _3);
	strings::copy((pstr)&m_hit_type, 16, damage_type); // damage_type can be less then 16, is this safe?
}

// STATE[UNVERIFIED]	
float stalker2::booster_damage_protector::reduce_damage(
	pcstr									__formal,
	pcstr									damage_type,
	float									amount)
{
	if (strings::equal(damage_type, m_hit_type))
		return math::max(0.0f, amount * m_reduce - m_absorb);
	else
		return amount;
}


namespace stalker2 {

//
// predicates
//
 
struct find_by_damage_type_predicate  {
public:
	find_by_damage_type_predicate(pcstr damage_type) {
		strings::copy(m_damage_type, 16, damage_type);
	} 

	// STATE[UNVERIFIED]: sushi@TODO: m_hit_type is getter
	bool operator()(booster_damage_protector * protector) const { 
		return strings::equal(protector->m_hit_type, m_damage_type);
	}

private:
	/* offset 0x0000 */ char                                m_damage_type[16];
}; // struct find_by_damage_type_predicate


struct regenerate_body_parts_predicate : boost::noncopyable {
public:
	regenerate_body_parts_predicate(u32 time_delta_ms, u32 current_time_in_ms) :
		time_delta_ms(time_delta_ms), current_time_in_ms(current_time_in_ms) {}

	void operator()(body_part_parameters * body_part) const {
		body_part->regenerate(time_delta_ms, current_time_in_ms);
	}

private:
	/* offset 0x0000 */ u32                                 time_delta_ms;
	/* offset 0x0004 */ u32                                 current_time_in_ms;
}; // struct regenerate_body_parts_predicate


struct dump_npc_body_part_state_predicate : boost::noncopyable {
public:
	dump_npc_body_part_state_predicate(
		xray::ai::npc_statistics&		npc_stats,
		u32								current_time) :
		npc_stats						(npc_stats),
		current_time					(current_time) {}

	void operator()(body_part_parameters* params) const { // sushi@TODO: <type>* <name> or <type> * <name>? Decide once and for all
		params->dump_state(npc_stats, current_time);
	}

private:
	/* offset 0x0000 */ xray::ai::npc_statistics&           npc_stats;
	/* offset 0x0004 */ u32                                 current_time;
}; // struct dump_npc_body_part_state_predicate


struct find_body_part_by_name_predicate : public boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	find_body_part_by_name_predicate(pcstr body_part_name) : body_part_name(body_part_name) {}
	
	// STATE[UNVERIFIED]
	bool operator()(body_part_parameters* params) const { return strings::equal(body_part_name, params->get_name()); };

private:
	/* offset 0x0000 */ pcstr                               body_part_name;
}; // struct find_body_part_by_name_predicate


struct affect_event_predicate : boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	affect_event_predicate(
		pcstr						body_part_name,
		hit_affects_type_enum		affect_type,
		affect_event_type_enum		event_type) :
		body_part_name				(body_part_name),
		affect_type					(affect_type),
		event_type					(event_type) {}

	// STATE[UNVERIFIED]
	void operator()(affect_subscriber * subscriber) const {
		subscriber->subscription_callback(body_part_name, affect_type, event_type);
	}

private:
	/* offset 0x0000 */ pcstr                               body_part_name;
	/* offset 0x0004 */ hit_affects_type_enum               affect_type;
	/* offset 0x0008 */ affect_event_type_enum              event_type;
}; // struct affect_event_predicate


struct reset_predicate : boost::noncopyable {
public:
	reset_predicate() {}

	// STATE[UNVERIFIED]
	void operator()(body_part_parameters* params) const {
		params->reset();
	}
}; // struct reset_predicate


//
// damage_model
// 

// STATE[UNVERIFIED]
damage_model::damage_model(
	affects_applying_type_enum         affects_applying_type) :
	xray::resources::unmanaged_resource		(1),
	m_affect_subscriptions					( ),
	m_affects_applying_type					(affects_applying_type),
	// m_damage_protectors need to be initialized somehow?
	m_last_tick_time_in_ms					(0),
	m_last_hit_initiator					(255)
{
	m_broken_legs_count[0] = 0;					// <0x6ffff4>
	m_broken_legs_count[1] = 0;					// <0x700001>

	m_broken_hands_count[0] = 0;				// <0x70000e>
	m_broken_hands_count[1] = 0;				// <0x70001b>	

	m_leg_damaged_subscriber.subscription_callback = boost::bind(&damage_model::on_broken_limb_affect, this, _1, _2, _3); // <0x700028>
	m_hand_damaged_subscriber.subscription_callback = boost::bind(&damage_model::on_broken_limb_affect, this, _1, _2, _3); // <0x7000e6>

	subscribe_on_affect(affects_type_leg_damage, &m_leg_damaged_subscriber);   // <0x7001ad>
	subscribe_on_affect(affects_type_hand_damage, &m_hand_damaged_subscriber); // <0x7001c7>
}

// STATE[UNVERIFIED]
damage_model::~damage_model( )
{
	while (booster_damage_protector* p = m_damage_protectors.pop_front())	// <0x6ffde3> <block><1>
		DELETE(p);															// <0x6ffdfa>

	unsubscribe_from_affect(affects_type_leg_damage, &m_leg_damaged_subscriber); // <0x6ffe22>
	unsubscribe_from_affect(affects_type_hand_damage, &m_hand_damaged_subscriber); // <0x6ffe36>
}

// STATE[UNVERIFIED]: sushi@TODO: Note that there is if inside decomp
body_part_parameters* damage_model::pop_body_part( )
{
	return m_body_parts.pop_front(); // <0x6ff629>
}

// STATE[UNVERIFIED]
void damage_model::tick(
	u32                                time_delta_ms,
	u32                                current_time_in_ms)
{
	if (!m_last_tick_time_in_ms) // <0x6ffc69>
	{
		m_last_tick_time_in_ms = current_time_in_ms; // <0x6ffc75>
		return; // <0x6ffc81>
	}

	regenerate_body_parts_predicate regeneration_predicate(time_delta_ms, current_time_in_ms); // <0x6ffc83>
	m_body_parts.for_each(regeneration_predicate); // <0x6ffc97>
	m_last_tick_time_in_ms = current_time_in_ms; // <0x6ffcbf>
}

// STATE[UNVERIFIED]
void damage_model::add_body_part(
	body_part_parameters*              new_body_part)
{
	m_body_parts.push_back(new_body_part); // <0x6ff5f9>
}

// STATE[UNVERIFIED]
body_part_parameters* damage_model::get_body_part(
	pcstr                              part_name)
{
	find_body_part_by_name_predicate find_predicate(part_name); // <0x6ff739>
	return m_body_parts.find_if(find_predicate);				// <0x6ff747>
}

// STATE[PARTIAL]
bool damage_model::hit_body_part(
	u8                                 initiator,
	pcstr                              part_name,
	pcstr                              damage_type,
	float                              amount,
	float                              armor_piercing,
	u32                                time_in_ms,
	bullet*                            bullet)
{

	body_part_parameters*           last_hitted_body_part =															// <0x6ffafa>
		bullet ? (body_part_parameters*)NULL /* m_body_parts.find(bullet->m_last_hitted_body_part) */ : NULL; // sushi@TODO: m_last_hitted_body_part is not member of bullet ; find is not a method of intrusive_list(?)
	body_part_parameters*           part = get_body_part(part_name);												// <0x6ffb2d>
	if (last_hitted_body_part																						// <0x6ffb3c>
		&& (last_hitted_body_part == part
			|| last_hitted_body_part->m_damage_group != 255
			&& last_hitted_body_part->m_damage_group == part->m_damage_group))
																													// <0x6ffb42> sushi@TODO
		return false;																								// <0x6ffb86>

	// ASSERT																										// <0x6ffb8a>
	pcstr                           hit_type = damage_type;															// <0x6ffb96>
	m_last_hit_initiator = initiator;																				// <0x6ffb9c>
	damage_protector*               prot = m_damage_protectors.find_if(find_by_damage_type_predicate(damage_type));	// <0x6ffba8>
	part->hit_by_type(hit_type, time_in_ms, amount, armor_piercing, true, prot);									// <0x6ffbcd>

	if (bullet)																										// <0x6ffbf1>
		{ ; } // bullet->m_last_hitted_body_part = part;																// <0x6ffbf7>

	return true;																									// <0x6ffc00>
}

// STATE[UNVERIFIED] TODO: CURRENT
void damage_model::apply_med_kit(
	pcstr                              part_name,
	float                              amount)
{
	body_part_parameters * part = get_body_part(part_name);	// <0x6ff8e9>
	ASSERT(part);											// <0x6ff8f8>
	part->increase_health(amount);							// <0x6ff904>
}

// STATE[UNVERIFIED]
void damage_model::fill_stats(
	xray::ai::npc_statistics&          stats,
	u32                                current_time_in_ms)
{
	typedef ai::statistics_item<46, 16> content_type;
	content_type new_stats_item = content_type();									// <0x6ff800>
	new_stats_item.caption = "right_leg";											// <0x6ff80b> sushi@TODO: Looks stupid, why 'right_leg' specifically.
	stats.selectors_state.push_back(new_stats_item);								// <0x6ff81b> sushi@TODO: This is wrong, npc_statistics has changed, this should be `body_state`.

	dump_npc_body_part_state_predicate dump_predicate(stats, current_time_in_ms);	// <0x6ff86b>
	m_body_parts.for_each(dump_predicate);											// <0x6ff888>
}

// STATE[UNVERIFIED]
void damage_model::dump_stats(
	boost::function<void __cdecl(u32,float,float,pcstr)> callback)
{
	body_part_parameters *		body_part = m_body_parts.front();		// <0x6ff92a>
	u32							body_part_index = 0;					// <0x6ff936>
	while ( body_part )													// <0x6ff93d>
	{
		body_part->dump_state(callback, body_part_index++);				// <0x6ff943>
		body_part = m_body_parts.get_next_of_object(body_part);			// <0x6ff96b>
	}																	// <0x6ff973>
}

// STATE[UNVERIFIED]
void damage_model::subscribe_on_affect(
	hit_affects_type_enum              affect_type,
	affect_subscriber*                 subscriber)
{
	m_affect_subscriptions[affect_type].push_back(subscriber); // <0x6ffa39>
}

// STATE[PARTIAL]
void damage_model::unsubscribe_from_affect(
	hit_affects_type_enum              affect_type,
	affect_subscriber*                 subscriber)
{
	affect_subscriptions * subscribers = &m_affect_subscriptions[affect_type];	// <0x6ff9f9>
	// ASSERT (maybe that such sub exists )										// <0x6ffa0c>
	subscribers->erase(subscriber);												// <0x6ffa18>
}

// STATE[UNVERIFIED]
void damage_model::notify_on_affect_event(
	pcstr                              body_part_name,
	hit_affects_type_enum              affect_type,
	affect_event_type_enum             event_type)
{
	//
	//
	//
	//
	affect_subscriptions * subscribers = &m_affect_subscriptions[affect_type];				// <0x6ff99f> <block><1>
	subscribers->for_each(affect_event_predicate(body_part_name, affect_type, event_type)); // <0x6ff9b5>
	//
}

// STATE[UNVERIFIED]
void damage_model::reset( )
{
	m_last_hit_initiator = 255;					// <0x6ff589>

	m_broken_legs_count[0] = 0;					// <0x6ff593>
	m_broken_legs_count[1] = 0;					// <0x6ff59d>

	m_broken_hands_count[0] = 0;				// <0x6ff5a7>
	m_broken_hands_count[1] = 0;				// <0x6ff5b1>

	m_body_parts.for_each(reset_predicate());	// <0x6ff5bb>
}

// STATE[PARTIAL] sushi@TODO: Why apply by last tick time?
void damage_model::apply_affect(
	pcstr                              part_name,
	hit_affects_type_enum              affect,
	affect_event_type_enum             event_type)
{
	body_part_parameters * part = get_body_part(part_name);						// <0x6ffaa9>
	// ASSERT (that part exists?)												// <0x6ffab8>
	part->apply_affect_by_force(affect, event_type, m_last_tick_time_in_ms);	// <0x6ffac4>
}

// STATE[UNVERIFIED]
u8 damage_model::get_total_health( )
{
	u8 result_value = 100;																																// <0x6ff529>

	for ( body_part_parameters * it_body_part = m_body_parts.front() ; it_body_part ; it_body_part = m_body_parts.get_next_of_object(it_body_part) )	// <0x6ff52d> <block><1>
	{
		if ( it_body_part->can_affect_death() && it_body_part->get_health_in_percentage() < result_value )												// <0x6ff549>
			result_value =  it_body_part->get_health_in_percentage();																					// <0x6ff56b>
	}																																					// <0x6ff576>
	return result_value;																																// <0x6ff578>
}

// STATE[PARTIAL]
void damage_model::cancel_affect(
	pcstr                              part_name,
	hit_affects_type_enum              affect)
{
	body_part_parameters * part = get_body_part(part_name); // <0x6ffa69>
	// ASSERT (that part exists?)							// <0x6ffa78>
	part->cancel_affect_by_force(affect);					// <0x6ffa84>
}

// STATE[PARTIAL]
void damage_model::register_body_part_damage_protector(
	pcstr                              part_name,
	damage_protector*                  protector)
{
	body_part_parameters * part = get_body_part(part_name); // <0x6ff7b9>
	// ASSERT (that part exists?)							// <0x6ff7c8>
	part->add_damage_protector(protector);					// <0x6ff7d4>
}

// STATE[UNVERIFIED]
void damage_model::unregister_body_part_damage_protector(
	pcstr                              part_name,
	damage_protector*                  protector)
{
	body_part_parameters * part = get_body_part(part_name); // <0x6ff779>
	// ASSERT (that part exists?)							// <0x6ff788>
	part->remove_damage_protector(protector);				// <0x6ff794>
}

// STATE[UNVERIFIED]: sushi@TODO: Don't know who calls DELETE? Maybe it is not needed, since the arena is used for an allocator
//								  There are calls to DELETE however in the xray source code
void damage_model::add_damage_protector(
	pcstr                              damage_type,
	float                              reduce,
	float                              absorb)
{
	booster_damage_protector * protector = m_damage_protectors.find_if(			// <0x6ffcf0>
		find_by_damage_type_predicate(damage_type));
	if ( protector ) // <0x6ffd18>
	{
		protector->m_reduce *= reduce; // 0.5 * 0.5								// <0x6ffd1e>
		protector->m_absorb += absorb;											// <0x6ffd33>
	}
	else																		// <0x6ffd48>
	{	// Maybe different alloc is needed? vostok::memory::doug_lea_allocator::malloc_impl(survarium::g_allocator)
		protector = NEW(booster_damage_protector)(damage_type, reduce, absorb); // <0x6ffd4a>
		m_damage_protectors.push_back(protector);								// <0x6ffdb0>
	}
}

// STATE[UNVERIFIED]: sushi@TODO: better understand how affects work and when this is called
void damage_model::on_broken_limb_affect(
	pcstr                              bodypart,
	hit_affects_type_enum              affect,
	affect_event_type_enum             type)
{
	if ( affect == affects_type_leg_damage && strings::equal("left_leg", bodypart) )			// <0x6ff669>
	{
		m_broken_legs_count[0] = type == affect_applying;										// <0x6ff683>
	}
	else if ( affect == affects_type_leg_damage && strings::equal("right_leg", bodypart) )		// <0x6ff69a>
	{
		m_broken_legs_count[1] = type == affect_applying;										// <0x6ff6b4>
	}
	else if ( affect == affects_type_hand_damage && strings::equal("left_hand", bodypart) )		// <0x6ff6c8>
	{
		m_broken_hands_count[0] = type == affect_applying;										// <0x6ff6e2>
	}
	else if ( affect == affects_type_hand_damage && strings::equal("right_hand", bodypart) )	// <0x6ff6f6>
	{
		m_broken_hands_count[1] = type == affect_applying;										// <0x6ff710>
	}
	//
	//
}

#if 0
// STATE[PENDING]
// void survarium::damage_model::deserialize(vostok::network_core::packet_reader&)
void damage_model::deserialize(
	xray::network_core::packet_reader& reader)
{
	// FUNCTION BODY
	// <0x6ffc19>
	// ******
}
#endif
} // namespace stalker2