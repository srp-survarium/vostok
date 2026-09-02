// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/hit_type_parameters.h>

#include <vostok/game_core/body_part_parameters.h>

namespace survarium {

hit_type_parameters::hit_type_parameters(
	pcstr		type,
	const float	absorption,
	const float	armor,
	float		reduce,
	const u32	bdb_count
) :
	next					( NULL ),
	m_type				  	( type ),
	m_absorption_amount		( absorption ),
	m_armor					( armor ),
	m_reduce				( reduce ),
	m_bdb_count				( bdb_count )
{
}

void hit_type_parameters::apply_damage( const float delta, const u32 time_in_ms )
{
	std::pair<body_part_parameters*, float>* const it_begin = get_bdb_coefficients();
	std::pair<body_part_parameters*, float>* const it_end = it_begin + m_bdb_count;

	for ( std::pair<body_part_parameters*, float>* it = it_begin; it != it_end; ++it )
		if ( it->second > 0.0f )
			it->first->hit_by_type( get_type( ), time_in_ms, it->second * delta, 0.0f, false, NULL ); // sushi@MATCH: In target `c_str` is called in a different order.
}

void hit_type_parameters::set_parameters( float armor, float reduce, float absorbtion )
{
	m_armor = armor;
	m_reduce = reduce;
	m_absorption_amount = absorbtion;
}

} // namespace survarium
