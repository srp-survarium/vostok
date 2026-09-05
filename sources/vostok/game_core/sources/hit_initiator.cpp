// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_info.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

 hit_info::hit_info( )
{
}

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
}

void hit_info::deserialize( network_core::packet_reader& packet )
{
	hit_initiator	= packet.r< u8 >( );
	being_hit		= packet.r< u8 >( );

	char c_body_part_name[ 16 ];
	packet.r_string	( c_body_part_name );
	body_part_name	= c_body_part_name;

	char damage_type_info[ 16 ];
	packet.r_string	( damage_type_info );
	damage_type		= damage_type_info;

	amount			= packet.r< float >( );
	armor_piercing	= packet.r< float >( );

	bullet			= NULL;
}

} // namespace survarium
