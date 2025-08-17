////////////////////////////////////////////////////////////////////////////
//	Created 	: 14.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_model.h"


//
// booster_damage_protector
//

stalker2::booster_damage_protector::booster_damage_protector(
	const char*								damage_type,
	float									reduce,
	float									absorb):
	m_reduce								(reduce),
	m_absorb								(absorb)
{
	reduce_damage_functor = boost::bind( &booster_damage_protector::reduce_damage, this, _1, _2, _3 );
	strings::copy( (pstr)&m_hit_type, 16, damage_type); // damage_type can be less then 16, is this safe?
}
	
float stalker2::booster_damage_protector::reduce_damage(
	const char*								__formal,
	const char*								damage_type,
	float									amount)
{
	if (strings::equal(damage_type, m_hit_type))
		return math::max(0.0f, amount * m_reduce - m_absorb);
	else
		return amount;
}

//
// damage_model
//

namespace stalker2 {

// survarium::damage_model::damage_model(const survarium::affects_applying_type_enum)
damage_model::damage_model(
	affects_applying_type_enum         affects_applying_type) :
	xray::resources::unmanaged_resource		(1),
	m_affect_subscriptions					( ),
	m_affects_applying_type					(affects_applying_type),
	// ???
	m_last_tick_time_in_ms					(0),
	m_last_hit_initiator					(255), // -1?
	// ???
	m_broken_legs_count						( ),
	m_broken_hands_count					( )
{
	// FUNCTION BODY
	// <0x6ffff4>
	// <0x700001>

	// <0x70000e>
	// <0x70001b>

	// <0x700028>
	// <0x7000e6>

	// subscribe_on_affect( affects_type_leg_damage, &m_leg_damage_subscriber );   // <0x7001ad>
	// subscribe_on_affect( affects_type_hand_damage, &m_hand_damage_subscriber ); // <0x7001c7>
	// ******
}

// void survarium::damage_model::~damage_model()
damage_model::~damage_model( )
{
	// LOCALS
	// booster_damage_protector*       p<1>
	// ******

	// FUNCTION BODY
	// <0x6ffde3> <block><1>
	// <0x6ffdfa>

	// <0x6ffe22>
	// <0x6ffe36>
	// ******
}

// STATE[UNVERIFIED]: sushi@TODO: Note that there is if inside decomp
// survarium::body_part_parameters* survarium::damage_model::pop_body_part()
body_part_parameters* damage_model::pop_body_part( )
{
	// FUNCTION BODY
	return m_body_parts.pop_front(); // <0x6ff629>
	// ******
}

// void survarium::damage_model::tick(const unsigned int, const unsigned int)
void damage_model::tick(
	u32                                time_delta_ms,
	u32                                current_time_in_ms)
{
	// LOCALS
	// regenerate_body_parts_predicate regeneration_predicate
	// ******

	// FUNCTION BODY
	// <0x6ffc69>

	// <0x6ffc75>
	// <0x6ffc81>


	// <0x6ffc83>
	// <0x6ffc97>
	// <0x6ffcbf>
	// ******
}

// void survarium::damage_model::add_body_part(survarium::body_part_parameters* const)
void damage_model::add_body_part(
	body_part_parameters*              new_body_part)
{
	// FUNCTION BODY
	m_body_parts.push_back(new_body_part); // <0x6ff5f9>
	// ******
}

struct find_body_part_by_name_predicate : public boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	find_body_part_by_name_predicate(pcstr body_part_name) : body_part_name(body_part_name) {}
	
	// STATE[UNVERIFIED]
	bool operator()(body_part_parameters* params) const { return strings::equal(body_part_name, params->get_name()); };

private:
	/* offset 0x0000 */ pcstr                               body_part_name;
}; // struct find_body_part_by_name_predicate


// STATE[UNVERIFIED]
// survarium::body_part_parameters* survarium::damage_model::get_body_part(char const*) const
body_part_parameters* damage_model::get_body_part(
	pcstr                              part_name)
{
	// LOCALS
	// find_body_part_by_name_predicate find_predicate
	// ******

	// FUNCTION BODY
	find_body_part_by_name_predicate find_predicate(part_name); // <0x6ff739>
	return m_body_parts.find_if(find_predicate); // <0x6ff747>
	// ******
}

// bool survarium::damage_model::hit_body_part(const unsigned char, char const*, char const*, const float, const float, const unsigned int, survarium::bullet* const)
bool damage_model::hit_body_part(
	u8                                 initiator,
	pcstr                              part_name,
	pcstr                              damage_type,
	float                              amount,
	float                              armor_piercing,
	u32                                time_in_ms,
	bullet*                            bullet)
{
	// LOCALS
	// damage_protector*               prot
	// body_part_parameters*           last_hitted_body_part
	// body_part_parameters*           part
	// pcstr                           hit_type
	// ******

	// FUNCTION BODY

	// <0x6ffafa>

	// <0x6ffb2d>
	// <0x6ffb3c>



	// <0x6ffb42>
	// <0x6ffb86>

	// <0x6ffb8a>
	// <0x6ffb96>
	// <0x6ffb9c>
	// <0x6ffba8>
	// <0x6ffbcd>

	// <0x6ffbf1>
	// <0x6ffbf7>

	return false; // <0x6ffc00>
	// ******
}

// void survarium::damage_model::apply_med_kit(char const*, const float)
void damage_model::apply_med_kit(
	pcstr                              part_name,
	float                              amount)
{
	// LOCALS
	// body_part_parameters*           part
	// ******

	// FUNCTION BODY
	// <0x6ff8e9>
	// <0x6ff8f8>
	// <0x6ff904>
	// ******
}

// void survarium::damage_model::fill_stats(vostok::ai::npc_statistics&, const unsigned int) const
void damage_model::fill_stats(
	xray::ai::npc_statistics&          stats,
	u32                                current_time_in_ms)
{
	// LOCALS
	// dump_npc_body_part_state_predicate dump_predicate
	// xray::ai::statistics_item<46,16> new_stats_item
	// ******

	// OTHER SYMBOLS
	// UserDefinedType(UserDefinedTypeSymbol { type_index: TypeIndex(0x143b9), name: RawString("content_type") })
	// ******

	// FUNCTION BODY

	// <0x6ff800>
	// <0x6ff80b>
	// <0x6ff81b>

	// <0x6ff86b>
	// <0x6ff888>
	// ******
}

// void survarium::damage_model::dump_stats(boost::function<void __cdecl(unsigned int,float,float,char const *)>)
void damage_model::dump_stats(
	boost::function<void __cdecl(u32,float,float,pcstr)> callback)
{
	// LOCALS
	// body_part_parameters const*     body_part
	// u32                             body_part_index
	// ******

	// FUNCTION BODY
	// <0x6ff92a>
	// <0x6ff936>
	// <0x6ff93d>

	// <0x6ff943>
	// <0x6ff96b>
	// <0x6ff973>
	// ******
}

// void survarium::damage_model::subscribe_on_affect(const survarium::hit_affects_type_enum, survarium::affect_subscriber* const)
void damage_model::subscribe_on_affect(
	hit_affects_type_enum              affect_type,
	affect_subscriber*                 subscriber)
{
	// FUNCTION BODY
	m_affect_subscriptions[affect_type].push_back(subscriber); // <0x6ffa39>
	// ******
}

// void survarium::damage_model::unsubscribe_from_affect(const survarium::hit_affects_type_enum, survarium::affect_subscriber* const)
void damage_model::unsubscribe_from_affect(
	hit_affects_type_enum              affect_type,
	affect_subscriber*                 subscriber)
{
	// LOCALS
	// xray::intrusive_list<affect_subscriber,affect_subscriber *,32,xray::threading::mutex,xray::size_policy,xray::no_debug_policy>& subscribers
	// ******

	// FUNCTION BODY
	// <0x6ff9f9>
	// <0x6ffa0c>
	// <0x6ffa18>
	// ******
}

// void survarium::damage_model::notify_on_affect_event(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void damage_model::notify_on_affect_event(
	pcstr                              body_part_name,
	hit_affects_type_enum              affect_type,
	affect_event_type_enum             event_type)
{
	// LOCALS
	// xray::intrusive_list<affect_subscriber,affect_subscriber *,32,xray::threading::mutex,xray::size_policy,xray::no_debug_policy>& subscribers<1>
	// ******

	// FUNCTION BODY






	// <0x6ff99f> <block><1>
	// <0x6ff9b5>

	// ******
}

// void survarium::damage_model::reset()
void damage_model::reset( )
{
	// FUNCTION BODY
	// <0x6ff589>

	// <0x6ff593>
	// <0x6ff59d>

	// <0x6ff5a7>
	// <0x6ff5b1>

	// <0x6ff5bb>
	// ******
}

// void survarium::damage_model::apply_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void damage_model::apply_affect(
	pcstr                              part_name,
	hit_affects_type_enum              affect,
	affect_event_type_enum             event_type)
{
	// LOCALS
	// body_part_parameters*           part
	// ******

	// FUNCTION BODY
	// <0x6ffaa9>
	// <0x6ffab8>
	// <0x6ffac4>
	// ******
}

// unsigned char survarium::damage_model::get_total_health()
u8 damage_model::get_total_health( )
{
	// LOCALS
	// u8                              result_value
	// body_part_parameters*           it_body_part<1>
	// ******

	// FUNCTION BODY
	// <0x6ff529>

	// <0x6ff52d> <block><1>

	// <0x6ff549>
	// <0x6ff56b>
	// <0x6ff576>
	return 0; // <0x6ff578>
	// ******
}

// void survarium::damage_model::cancel_affect(char const*, const survarium::hit_affects_type_enum)
void damage_model::cancel_affect(
	pcstr                              part_name,
	hit_affects_type_enum              affect)
{
	// LOCALS
	// body_part_parameters*           part
	// ******

	// FUNCTION BODY
	// <0x6ffa69>
	// <0x6ffa78>
	// <0x6ffa84>
	// ******
}

// void survarium::damage_model::register_body_part_damage_protector(char const*, survarium::damage_protector*)
void damage_model::register_body_part_damage_protector(
	pcstr                              part_name,
	damage_protector*                  protector)
{
	// LOCALS
	// body_part_parameters*           part
	// ******

	// FUNCTION BODY
	// <0x6ff7b9>
	// <0x6ff7c8>
	// <0x6ff7d4>
	// ******
}

// void survarium::damage_model::unregister_body_part_damage_protector(char const*, survarium::damage_protector*)
void damage_model::unregister_body_part_damage_protector(
	pcstr                              part_name,
	damage_protector*                  protector)
{
	// LOCALS
	// body_part_parameters*           part
	// ******

	// FUNCTION BODY
	// <0x6ff779>
	// <0x6ff788>
	// <0x6ff794>
	// ******
}

// void survarium::damage_model::add_damage_protector(char const*, float, float)
void damage_model::add_damage_protector(
	pcstr                              damage_type,
	float                              reduce,
	float                              absorb)
{
	// LOCALS
	// booster_damage_protector*       protector
	// ******

	// FUNCTION BODY
	// <0x6ffcf0>

	// <0x6ffd18>

	// <0x6ffd1e>
	// <0x6ffd33>

	// <0x6ffd48>

	// <0x6ffd4a>
	// <0x6ffdb0>

	// ******
}

// void survarium::damage_model::on_broken_limb_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void damage_model::on_broken_limb_affect(
	pcstr                              bodypart,
	hit_affects_type_enum              affect,
	affect_event_type_enum             type)
{
	// FUNCTION BODY
	// <0x6ff669>

	// <0x6ff683>

	// <0x6ff69a>

	// <0x6ff6b4>

	// <0x6ff6c8>

	// <0x6ff6e2>

	// <0x6ff6f6>

	// <0x6ff710>





	// ******
}

/*
// void survarium::damage_model::deserialize(vostok::network_core::packet_reader&)
void damage_model::deserialize(
	xray::network_core::packet_reader& reader)
{
	// FUNCTION BODY
	// <0x6ffc19>
	// ******
}
*/

	/* TYPEDEFS

	typedef
		xray::ai::statistics_item<46,16>
		part_info_type;

	*/

} // namespace stalker2


