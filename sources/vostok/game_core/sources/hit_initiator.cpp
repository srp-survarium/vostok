////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_info.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[UNCHECKED]
 hit_info::hit_info( )
{
	// FUNCTION BODY[0x73ea50]: 0
	// <0x73ea50>|0x000|+0x01c:'20'	{
	// <0x73ea6c>|0x01c|      :'21'	}
	// ******
}

// STATE[UNCHECKED]
 hit_info::hit_info(
	const u8					hit_initiator,
	const u8					being_hit,
	pcstr const					body_part_name,
	pcstr const					damage_type,
	const float					amount,
	const float					armor_piercing,
	survarium::bullet* const	bullet
) :
	body_part_name	( body_part_name ),
	damage_type		( damage_type ),
	bullet			( bullet ),
	amount			( amount ),
	armor_piercing	( armor_piercing ),
	hit_initiator	( hit_initiator ),
	being_hit		( being_hit )
{
	// FUNCTION BODY[0x73ea80]: 0
	// <0x73ea80>|0x000|+0x059:'39'	{
	// <0x73ead9>|0x059|      :'40'	}
	// ******
}

// STATE[PARTIAL]: r<bool>x2, r_string into temp + fixed_string assign x2,
// r<float>x2, bullet=NULL - shape matches target.
void hit_info::deserialize( network_core::packet_reader& packet )
{
	hit_initiator	= packet.r< bool >( );
	being_hit		= packet.r< bool >( );

	char c_body_part_name[ 16 ];
	packet.r_string	( c_body_part_name );
	body_part_name	= c_body_part_name;

	char damage_type_info[ 16 ];
	packet.r_string	( damage_type_info );
	damage_type		= damage_type_info;

	amount			= packet.r< float >( );
	armor_piercing	= packet.r< float >( );

	bullet			= NULL;

	// STRUCTURE DIFF[target 0x72eaf0 | base 0x56d640]: target 9 / base 9 stmts
	//   1-2: hit_initiator/being_hit r<bool>   ONLY base (SIZE-drift, target rows L45/L49/L52)
	//   9: damage_type = damage_type_info;   ONLY base (SIZE-drift)
	//  10-11: amount/armor_piercing r<float>   SIZE
	// ; aligned 4, size-diffs 2, quantity-diffs 6, blank-gaps 2
	// VERDICT: STRUCTURE MATCH (shape ok) - identical 9-stmt order (2 r<bool>, r_string+assign x2, 2 r<float>, bullet=NULL); ALL divergences are LTCG inline-vs-call SIZE residual + its alignment drift (target inlines r<bool>/r_string/r<float>), non-steerable.
}

} // namespace survarium
