////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "body_part_parameters.h"

#include "affects_applying_type_enum.h"
#include "damage_model.h"


#include <xray/ai/npc_statistics.h>

namespace stalker2 {

//
// predicates
//

struct find_hit_parameters_by_type_predicate : boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	find_hit_parameters_by_type_predicate(pcstr hit_t) : hit_type(hit_t) {}

	// STATE[PARTIAL] sushi@TODO: hit_type_params most likely uses getter
	bool operator()(hit_type_parameters* hit_type_params) const {
		return strings::equal(hit_type, hit_type_params->m_type.c_str());
	}

private:
	/* offset 0x0000 */ pcstr                               hit_type;
}; // struct find_hit_parameters_by_type_predicate

struct protect_affect_predicate : boost::noncopyable {
public:
	protect_affect_predicate(
		pcstr								body_type_name,
		hit_affects_type_enum				affect_type):
		m_body_type_name					(body_type_name),
		m_affect_type						(affect_type),
		m_result							(0)	{ }

	void operator()(
		damage_protector*                  protector);

public:
	/* offset 0x0000 */ pcstr                               m_body_type_name;
	/* offset 0x0004 */ hit_affects_type_enum               m_affect_type;
	/* offset 0x0008 */ bool                                m_result;
}; // struct protect_affect_predicate

// STATE[UNVERIFIED]
void protect_affect_predicate::operator()(
	damage_protector*                  protector)
{
	if (!m_result && protector->protect_affect_functor) // <0xc9f8f>
		m_result = protector->protect_affect_functor(m_body_type_name, m_affect_type); // <0xc9fb9>
}


struct protect_damage_predicate : boost::noncopyable {
public:
	protect_damage_predicate(
		float							   armor_piercing,
		pcstr							   damage_type,
		pcstr							   body_type_name,
		float							   amount) :
		m_body_type_name				   (body_type_name),
		m_damage_type					   (damage_type),
		m_armor_piercing				   (armor_piercing),
		m_amount					       (amount) {}

	void operator()(
		damage_protector*                  protector);

private:
	/* offset 0x0000 */ pcstr                               m_body_type_name;
	/* offset 0x0004 */ pcstr                               m_damage_type;
	/* offset 0x0008 */ float                               m_armor_piercing;
	/* offset 0x000c */ float                               m_amount;
}; // struct protect_damage_predicate

// STATE[UNVERIFIED]
void protect_damage_predicate::operator()(
	damage_protector*                  protector)
{
	if (m_amount > 0.0f && protector->reduce_damage_functor) // <0xc9eff>
		m_amount = protector->reduce_damage_functor(m_body_type_name, m_damage_type, m_amount, m_armor_piercing); // <0xc9f2f>

}


//
// body_part_parameters
//

// STATE[PARTIAL]: inlining was done completely differently
body_part_parameters::body_part_parameters(
	pcstr                              name,
	float                              health,
	float                              regeneration_speed,
	float                              regeneration_timeout,
	bool                               can_be_assigned,
	damage_model&                      owner,
	u8                                 damage_group) :

	next								(NULL),
	m_damage_model						(owner),
	m_max_health						(health),
	m_health							(health),
	m_regeneration_speed				(regeneration_speed),
	m_last_hit_time						(0),
	m_last_hit_health					(health),
	m_assignable						(can_be_assigned),
	m_damage_group						(damage_group)
{
	m_regeneration_timeout = math::floor(1000.0f * regeneration_timeout); // <0x597105>
}

// STATE[UNVERIFIED]
hit_type_parameters* body_part_parameters::pop_hit_type( )
{
	return m_hit_types.pop_front(); // <0x596f99>
}

// STATE[UNVERIFIED]
affects_threshold* body_part_parameters::pop_threshold( )
{
	return m_thresholds.pop_front(); // <0x596f59>
}

// STATE[UNVERIFIED]
void body_part_parameters::add_hit_type(
	hit_type_parameters*               new_hit_type)
{
	m_hit_types.push_back(new_hit_type); // <0x596f39>
}

// STATE[UNVERIFIED]
void body_part_parameters::add_threshold(
	affects_threshold*                 new_threshold)
{
	m_thresholds.push_back(new_threshold); // <0x596f19>
}

// STATE[UNVERIFIED]
hit_type_parameters* body_part_parameters::get_hit_parameters(
	pcstr                              hit_type)
{
	find_hit_parameters_by_type_predicate find_predicate = find_hit_parameters_by_type_predicate(hit_type); // <0x5971b9>
	return m_hit_types.find_if(find_predicate); // <0x5971c7>
}


// STATE[PENDING]: sushi@TODO: Requires get_hit_parameters
// void survarium::body_part_parameters::hit_by_type(char const*, const unsigned int, const float, const float, const bool, survarium::damage_protector*)
void body_part_parameters::hit_by_type(
	pcstr                              hit_type,
	u32                                time_in_ms,
	float                              amount,
	float                              armor_piercing,
	bool                               __formal,
	damage_protector*                  prot)
{
	// LOCALS
	// float                           delta
	// float                           e_wnd
	// float                           arp_arm_coeff
	// hit_type_parameters*            params
	// protect_damage_predicate        p
	// ******

	// FUNCTION BODY
	// <0x59791f>
	// <0x597931>

	// <0x59793d>



	// <0x597947>
	// <0x597966>
	// <0x597973>
	// <0x597975>

	// <0x5979a6>

	// <0x5979bd>

	// <0x597a2f>
	// <0x597a68>
	// <0x597a93>

	// <0x597aaa>
	// <0x597ab0>






	// <0x597afc>
	// <0x597b0e>
	// <0x597b26>

	// <0x597b35>
	// <0x597b53>

	// <0x597b62>
	// ******
}

// STATE[UNVERIFIED]
void body_part_parameters::increase_health(
	float                              amount)
{
	m_health = math::clamp_r<float>(0.0f, m_health + amount, m_max_health); // <0x596ec9>
}

// STATE[UNVERIFIED]
void body_part_parameters::decrease_health(
	float                              amount)
{
	m_health = math::clamp_r<float>(0.0f, m_health - amount, m_max_health); // <0x596e79>
}

// STATE[PENDING] sushi@TODO: Requires update_affects
// `time_delta_ms` frame duration.
// void survarium::body_part_parameters::regenerate(const unsigned int, const unsigned int)
void body_part_parameters::regenerate(
	u32                                time_delta_ms,
	u32                                current_time_in_ms)
{
	// LOCALS
	// u32                             regenerate_delta
	// float                           amount
	// u32                             next_regen_time<1>
	// u32                             regen_allowed<1>
	// ******

	// SKIPPED BLOCKS
	// <0x597879><1>
	// ******

	u32 regenerate_delta = time_delta_ms;
	if (m_regeneration_timeout)
	{
		u32 next_regen_time = m_regeneration_timeout + m_last_hit_time;
		if (current_time_in_ms <= next_regen_time )
			return;
		regenerate_delta = math::min(current_time_in_ms - next_regen_time, time_delta_ms);
	}
	float amount = regenerate_delta * m_regeneration_speed / 1000.0f;
	increase_health(amount);

	if (m_damage_model.m_affects_applying_type == affects_applying_type_enum::type_apply_directly) // sushi@TODO: Needs getter
		{ ; } // update_affects(current_time_in_ms);

	// FUNCTION BODY





	// <0x597869>

	// <0x59786f>

	// <0x59787b>

	// <0x597890>
	// <0x597898>

	// <0x59789a>
	// <0x5978a3>





	// <0x5978b1>
	// <0x5978d3>

	// <0x5978e2>
	// <0x5978f7>
	// ******
}

// STATE[PENDING]
// void survarium::body_part_parameters::update_affects(const unsigned int)
void body_part_parameters::update_affects(
	u32                                current_time_in_ms)
{
	// LOCALS
	// s32                             i<1>
	// stlp_std::pair<enum hit_affects_type_enum,u32>* it_affect<2>
	// ******

	// FUNCTION BODY
	for (s32 i = m_affects.size(); i >= 0; --i) // <0x5977a9> <block><1>
	{
	// <0x5977d7> <block><2>
	// <0x5977e6>

	// <0x597804>
	// <0x597810>
	// <0x59782f>
	// <0x59784b>
	}
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::cancel_affect_by_force(const survarium::hit_affects_type_enum)
void body_part_parameters::cancel_affect_by_force(
	hit_affects_type_enum              affect)
{
	// LOCALS
	// s32                             i<1>
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<2>
	// ******

	// FUNCTION BODY
	// <0x5976e9> <block><1>

	// <0x597717> <block><2>
	// <0x597726>

	// <0x597743>
	// <0x59774f>
	// <0x59776e>
	// <0x59778a>

	// ******
}

// STATE[UNVERIFIED]: sushi@NOTE: check_affects doesn't checks them but applies them .
void body_part_parameters::check_affects(
	u32                                current_time_in_ms)
{
	for ( affects_threshold* it_threshold = m_thresholds.front() ; it_threshold ; m_thresholds.get_next_of_object(it_threshold) )	// <0x597679> <block><1>
		if ( m_health <= m_max_health * it_threshold->value() )																		// <0x597692>
		{
			it_threshold->bodypart()->apply_affects(it_threshold, current_time_in_ms);												// <0x5976bb>
		}																															// <0x5976d4>
}

// STATE[STUB]
// bool survarium::body_part_parameters::is_affect_applied(const survarium::hit_affects_type_enum)
bool body_part_parameters::is_affect_applied(
	hit_affects_type_enum              affect)
{
	// LOCALS
	// u32                             i<1>
	// ******

	// FUNCTION BODY
	// <0x596da9> <block><1>
	// <0x596dd7>
	// <0x596df4>

	return true; // <0x596df8>
	// ******
}

// STATE[UNVERIFIED]
bool body_part_parameters::has_affect_protector(
	hit_affects_type_enum              affect)
{
	protect_affect_predicate p(m_name.c_str(), affect); // <0x5973e9>
	m_damage_protectors.for_each(p);					// <0x59740f>
	return p.m_result;									// <0x597437> sushi@TODO: Maybe getter?
}

// STATE[UNVERIFIED]
void body_part_parameters::apply_affects(
	affects_threshold const*           threshold_reached,
	u32                                current_time_in_ms)
{
	hit_affects_type_enum const*    it_begin = threshold_reached->get_affects();							// <0x597599>
	hit_affects_type_enum const*    it_end = it_begin + threshold_reached->get_affects_count();				// <0x5975ae>

	for ( hit_affects_type_enum const* it ; it != it_end ; ++it )											// <0x5975c3> <block><1>
		if ( !is_affect_applied(*it) && !has_affect_protector(*it) )										// <0x5975e0>
		{
			m_damage_model.notify_on_affect_event(m_name.c_str(), *it, affect_applying);					// <0x59760a>
			m_affects.push_back(std::make_pair(*it, current_time_in_ms + 1000 * affects_durations[*it]));	// <0x597629>
		}																									// <0x597664>
}

// STATE[STUB]
// void survarium::body_part_parameters::apply_affect_by_force(const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum, const unsigned int)
void body_part_parameters::apply_affect_by_force(
	hit_affects_type_enum              affect,
	affect_event_type_enum             event_type,
	u32                                current_time_in_ms)
{
	// LOCALS
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<1>
	// u32                             i<2>
	// ******

	// FUNCTION BODY
	// <0x597459>

	// <0x597472>
	// <0x597491>


	// <0x5974cd>

	// <0x5974d7> <block><1>
	// <0x5974e6> <block><2>

	// <0x597514>

	// <0x597531>
	// <0x59753d>
	// <0x59755a>

	// <0x597576>

	// <0x597578>

	// ******
}

// sushi@TODO: This one is very strange, since there is no such function in the header?
// It does exists in IDA. Maybe it was generated by something? The code seems completely different from what is generally used.
// Does that mean I need to implement this template also? Sigh..
#if 0
// STATE[STUB]
// void survarium::body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(vostok::ai::statistics_item<46,16>&, const unsigned int) const
void body_part_parameters::fill_new_stats_item<xray::ai::statistics_item<46,16> >(
	xray::ai::statistics_item<46,16>&  new_stats_item,
	u32                                current_time_in_ms)
{
	// LOCALS
	// xray::fixed_string<46>          new_item
	// u32                             i<1>
	// u32                             remaining_time_in_ms<2>
	// ******

	// SKIPPED BLOCKS
	// <0xca49d><2>
	// ******

	// FUNCTION BODY
	// <0xca3d0>
	// <0xca3e1>

	// <0xca3ee>
	// <0xca3f6>
	// <0xca432>

	// <0xca441>
	// <0xca449>
	// <0xca456>

	// <0xca465> <block><1>

	// <0xca4a3>
	// <0xca4ab>
	// <0xca512>
	// <0xca570>
	// <0xca57f>

	// <0xca584>
	// <0xca5b0>
	// ******
}
#endif

// STATE[PARTIAL] sushi@TODO: Requires fill_new_stats_item, which seems to be a m
void body_part_parameters::dump_state(
	xray::ai::npc_statistics&          stats,
	u32                                current_time_in_ms)
{
	typedef xray::ai::statistics_item<46,16> content_type;
	content_type new_stats_item = content_type();					// <0x59714f>
	// fill_new_stats_item(&new_stats_item, current_time_in_ms);	// <0x597165>
	// stats->body_state.push_back(new_stats_item);					// <0x59717b>
}

// STATE[PARTIAL]: sushi@TODO: We weren't able to fit the structure in the stub.
// Inlining happened differently.
void body_part_parameters::dump_state(
	boost::function<void __cdecl(u32,float,float,pcstr)> callback,
	u32                                index)
{
	xray::fixed_string<512>         affects_str;								// <0x597300>
	for (u32 i = 0 ; i < m_affects.size() ; ++i)								// <0x59730b> <block><1>
	{
		// ASSERT																// <0x597351> // ?
		affects_str.appendf("%s ", affects_captions[m_affects[i].first]);		// <0x597390>
	}
	callback(index, m_health, m_max_health, affects_str.c_str());				// <0x597392>
}

// STATE[UNVERIFIED]
void body_part_parameters::reset( )
{
	m_health = m_max_health;	// <0x596d49>
	m_last_hit_time = 0;		// <0x596d5b>
	m_affects.clear();			// <0x596d68> sushi@TODO:  for ( i = this->m_affects.m_begin; i != this->m_affects.m_end; ++i ) { ; } this->m_affects.m_end = this->m_affects.m_begin;
}

// STATE[VERIFIED]
bool body_part_parameters::can_affect_death( )
{
	for ( affects_threshold * it_threshold = m_thresholds.front() ;  it_threshold ; it_threshold = m_thresholds.get_next_of_object(it_threshold) ) // <0x596cc9> <block><1>
	{
		hit_affects_type_enum const* threshold_affects = it_threshold->get_affects();	// <0x596ce2> <block><2>
		u32 threshold_affects_count = it_threshold->get_affects_count();				// <0x596cf7>
		for ( u32 i = 0 ; i < threshold_affects_count ; ++i )							// <0x596d00> <block><3>
		{
			if (threshold_affects[i] == affects_type_death)								// <0x596d1a>
				return true;															// <0x596d26>
		}																				// <0x596d2a>
	}																					// <0x596d2c>
	return false;																		// <0x596d2e>
}

// STATE[UNVERIFIED]
u8 body_part_parameters::get_health_in_percentage( )
{
	return (u8)(100 * (m_health / m_max_health)); // <0x596c97>
}

// STATE[UNVERIFIED]
void body_part_parameters::add_damage_protector(
	damage_protector*                  protector)
{
	// ASSERT?									// <0x596e49>
	m_damage_protectors.push_back(protector);	// <0x596e55>
}

// STATE[UNVERIFIED]
void body_part_parameters::remove_damage_protector(
	damage_protector*                  protector)
{
	// ASSERT								// <0x596e19>
	m_damage_protectors.erase(protector);	// <0x596e25>
}

// STATE[UNVERIFIED]
void body_part_parameters::set_parameters(
	float                              max_health,
	float                              regeneration_speed)
{
	m_max_health = max_health;					// <0x596c67>
	m_regeneration_speed = regeneration_speed ; // <0x596c77>
}

#if 0
// STATE[STUB]
// void survarium::serialize_affect(vostok::network_core::udp_match_packet&, stlp_std::pair<enum survarium::hit_affects_type_enum,unsigned int> const&, int)
void serialize_affect(
	xray::network_core::udp_match_packet& packet,
	std::pair<enum hit_affects_type_enum,u32> const& affect,
	s32                                client_offset)
{
	// FUNCTION BODY
	// <0xc9ff6>
	// <0xca005>
	// ******
}

// STATE[STUB]
// void survarium::deserialize_affect(vostok::network_core::packet_reader&, stlp_std::pair<enum survarium::hit_affects_type_enum,unsigned int>&)
void deserialize_affect(
	xray::network_core::packet_reader& reader,
	std::pair<enum hit_affects_type_enum,u32>& affect)
{
	// FUNCTION BODY
	// <0xca026>
	// <0xca036>

	// <0xca044>
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::serialize(vostok::network_core::udp_match_packet&, int) const
void body_part_parameters::serialize(
	xray::network_core::udp_match_packet& packet,
	s32                                client_offset)
{
	// FUNCTION BODY
	// <0x5971f9>
	// <0x59720e>
	// <0x59723e>

	// <0x59725e>
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::deserialize(vostok::network_core::packet_reader&)
void body_part_parameters::deserialize(
	xray::network_core::packet_reader& reader)
{
	// LOCALS
	// u8                              affects_count
	// std::pair<enum hit_affects_type_enum,u32> affect<1>
	// ******

	// SKIPPED BLOCKS
	// <0x597bf1><1>
	// ******

	// FUNCTION BODY
	// <0x597b99>
	// <0x597bac>
	// <0x597bbd>

	// <0x597bc8>
	// <0x597bd4>

	// <0x597be0>

	// <0x597bf3>
	// <0x597c01>

	// <0x597c11>

	// ******
}
#endif

} // namespace stalker2
