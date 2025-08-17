////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "find_hit_parameters_by_type_predicate.h"
#include "protect_damage_predicate.h"
#include "protect_affect_predicate.h"

#include "affects_applying_type_enum.h";

#include "damage_model.h"
#include "body_part_parameters.h"

namespace stalker2 {

// STATE[PARTIAL]: inlining was done completely differently
// survarium::body_part_parameters::body_part_parameters(char const*, const float, const float, const float, const bool, survarium::damage_model&, const unsigned char)
body_part_parameters::body_part_parameters(
	pcstr                              name,
	float                              health,
	float                              regeneration_speed,
	float                              regeneration_timeout,
	bool                               can_be_assigned,
	damage_model&                      owner,
	u8                                 damage_group) :
	
	next(NULL),
	m_damage_model(owner),
	m_max_health(health),
	m_health(health),
	m_regeneration_speed(regeneration_speed),
	m_last_hit_time(0),
	m_last_hit_health(health),
	m_assignable(can_be_assigned),
	m_damage_group(damage_group)
{
	m_regeneration_timeout = math::floor(1000.0f * regeneration_timeout); // <0x597105>
}

// STATE[UNVERIFIED]
// survarium::hit_type_parameters* survarium::body_part_parameters::pop_hit_type()
hit_type_parameters* body_part_parameters::pop_hit_type( )
{
	// FUNCTION BODY
	return m_hit_types.pop_front(); // <0x596f99>
	// ******
}

// STATE[UNVERIFIED]
// survarium::affects_threshold* survarium::body_part_parameters::pop_threshold()
affects_threshold* body_part_parameters::pop_threshold( )
{
	// FUNCTION BODY
	return m_thresholds.pop_front(); // <0x596f59>
	// ******
}

// STATE[UNVERIFIED]
// void survarium::body_part_parameters::add_hit_type(survarium::hit_type_parameters* const)
void body_part_parameters::add_hit_type(
	hit_type_parameters*               new_hit_type)
{
	// FUNCTION BODY
	m_hit_types.push_back(new_hit_type); // <0x596f39>
	// ******
}

// STATE[UNVERIFIED]
// void survarium::body_part_parameters::add_threshold(survarium::affects_threshold* const)
void body_part_parameters::add_threshold(
	affects_threshold*                 new_threshold)
{
	// FUNCTION BODY
	m_thresholds.push_back(new_threshold); // <0x596f19>
	// ******
}

// STATE[UNVERIFIED]
// survarium::hit_type_parameters* survarium::body_part_parameters::get_hit_parameters(char const*) const
hit_type_parameters* body_part_parameters::get_hit_parameters(
	pcstr                              hit_type)
{
	// LOCALS
	// find_hit_parameters_by_type_predicate find_predicate
	// ******

	// FUNCTION BODY
	find_hit_parameters_by_type_predicate find_predicate = find_hit_parameters_by_type_predicate(hit_type); // <0x5971b9>
	return m_hit_types.find_if(find_predicate); // <0x5971c7>
	// ******
}

// STATE[UNVERIFIED]: sushi@TODO: A bit strange that this .cpp file implements methods of multiple headers, sigh.
// void survarium::protect_damage_predicate::operator()(survarium::damage_protector* const)
void protect_damage_predicate::operator()(
	damage_protector*                  protector)
{
	// FUNCTION BODY
	if (m_amount > 0.0f && protector->reduce_damage_functor) // <0xc9eff>
		m_amount = protector->reduce_damage_functor(m_body_type_name, m_damage_type, m_amount, m_armor_piercing); // <0xc9f2f>

	// ******
}

// STATE[UNVERIFIED]: sushi@TODO: A bit strange that this .cpp file implements methods of multiple headers, sigh.
// void survarium::protect_affect_predicate::operator()(survarium::damage_protector* const)
void protect_affect_predicate::operator()(
	damage_protector*                  protector)
{
	// FUNCTION BODY
	if (!m_result && protector->protect_affect_functor) // <0xc9f8f>
		m_result = protector->protect_affect_functor(m_body_type_name, m_affect_type); // <0xc9fb9>
	// ******
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
// void survarium::body_part_parameters::increase_health(const float)
void body_part_parameters::increase_health(
	float                              amount)
{
	// FUNCTION BODY
	m_health = math::clamp_r<float>(0.0f, m_health + amount, m_max_health); // <0x596ec9>
	// ******
}

// STATE[UNVERIFIED]
// void survarium::body_part_parameters::decrease_health(const float)
void body_part_parameters::decrease_health(
	float                              amount)
{
	// FUNCTION BODY
	m_health = math::clamp_r<float>(0.0f, m_health - amount, m_max_health); // <0x596e79>
	// ******
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
		; // update_affects(current_time_in_ms);
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




} // namespace stalker2 