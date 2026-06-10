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

// STATE[0%|PARTIAL]: r<bool>x2, r_string into temp + fixed_string assign x2,
// r<float>x2, bullet=NULL - shape matches target 9/9. The 0% (report.json omits the
// 0.0 as null) is wholesale inline-vs-call: the target keeps r<>/r_string<16>/
// fixed_string::operator= out-of-line as LTCG custom-convention COMDATs (136 bytes)
// where our base inlines them all (279 bytes) - not source-steerable.
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

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ----------+--------+--------+----+----+------+------
	// SIZE +0xe |0x72eafa|0x58112f|0xe |0x1c|49    |	hit_initiator	= packet.r< bool >( );
	// SIZE +0xe |0x72eb08|0x58114b|0xe |0x1c|50    |	being_hit		= packet.r< bool >( );
	// SIZE +0x22|0x72eb16|0x581167|0xc |0x2e|53    |	packet.r_string	( c_body_part_name );
	// SIZE +0x4 |0x72eb22|0x581195|0xb |0xf |54    |	body_part_name	= c_body_part_name;
	// SIZE +0x22|0x72eb2d|0x5811a4|0xc |0x2e|57    |	packet.r_string	( damage_type_info );
	// SIZE +0x4 |0x72eb39|0x5811d2|0xe |0x12|58    |	damage_type		= damage_type_info;
	// SIZE +0x10|0x72eb47|0x5811e4|0x10|0x20|60    |	amount			= packet.r< float >( );
	// SIZE +0x10|0x72eb57|0x581204|0x10|0x20|61    |	armor_piercing	= packet.r< float >( );
	// SIZE +0x3 |0x72eb67|0x581224|0xa |0xd |63    |	bullet			= NULL;
	// VERDICT: STRUCTURE MATCH (shape ok) - 9/9 aligned, every row a SIZE flip from the same inline-vs-call wall (base inlines r<>/r_string/operator=, target calls), non-steerable.
}

} // namespace survarium
