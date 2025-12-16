////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hit_type_parameters.h>

#include <vostok/game_core/body_part_parameters.h>

namespace survarium {

// STATE[78.14%|DONE]: LTCG for `m_type` constructor.
hit_type_parameters::hit_type_parameters(
	pcstr		type,
	float		absorption,
	float		armor,
	float		reduce,
	u32			bdb_count
) :
	next					( NULL ),
	m_type				  	( type ),
	m_absorption_amount		( absorption ),
	m_armor					( armor ),
	m_reduce				( reduce ),
	m_bdb_count				( bdb_count )
{
}

// STATE[88.44%|PARTIAL]
void hit_type_parameters::apply_damage( float delta, u32 time_in_ms )
{
	std::pair<body_part_parameters*, float>* it_begin = get_bdb_coefficients();
	std::pair<body_part_parameters*, float>* it_end = it_begin + m_bdb_count;

	for ( std::pair<body_part_parameters*, float>* it = it_begin; it != it_end; ++it )
		if ( it->second > 0.0f )
			it->first->hit_by_type( m_type.c_str(), time_in_ms, it->second * delta, 0.0f, 0, 0 ); // In target `c_str` is called in a different order.
}

// STATE[100%|DONE]
void hit_type_parameters::set_parameters( float armor, float reduce, float absorbtion )
{
	m_armor = armor;
	m_reduce = reduce;
	m_absorption_amount = absorbtion;
}

} // namespace survarium
