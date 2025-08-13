////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_model.h"

#include <xray/strings_functions.h>
#include <xray/math_functions_inline.h>

namespace stalker2 {

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