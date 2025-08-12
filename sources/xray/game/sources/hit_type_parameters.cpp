////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "hit_type_parameters.h"
#include "body_part_parameters.h"

namespace stalker2 {

//
//
//
//
hit_type_parameters::hit_type_parameters(const char* type, float absorption, float armor, float reduce, unsigned int bdb_count):
	next(NULL),
	m_type(type),
	m_absorption_amount(absorption),
	m_armor(armor),
	m_reduce(reduce),
	m_bdb_count(bdb_count)
{
}

void hit_type_parameters::apply_damage(float delta, unsigned int time_in_ms)
{
	std::pair<body_part_parameters*, float> const* it_begin = get_body_parts();
	std::pair<body_part_parameters*, float> const* it_end = it_begin + m_bdb_count;

	for ( std::pair<body_part_parameters*, float> const* it = it_begin; it_begin != it_end; ++it )
		if ( it->second > 0.0f )
			it->first->hit_by_type( m_type.c_str(), time_in_ms, it->second * delta, 0.0f, 0, 0 );
}

//
//
//
//
//
//
//
//
//
//
void hit_type_parameters::set_parameters(float armor, float reduce, float absorbtion)
{
	m_armor = armor;
	m_reduce = reduce;
	m_absorption_amount = absorbtion;
}

} // namespace stalker2