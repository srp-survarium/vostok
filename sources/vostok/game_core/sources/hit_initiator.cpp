// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
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

// claude@NOTE: structure matches the target 1:1 (9 stmts, same source lines), but each
// statement is byte-larger because the base INLINES packet_reader::r<bool>/r<float>/
// r_string<16> while the target keeps them OUT-OF-LINE (standalone target symbols
// ??$r@_N@, ??$r@M@, ??$r_string@$0BA@@ exist; the base has no such instantiations - they
// inline at every site). Steering that is a cross-unit LTCG inliner decision in the already
// matched packet_reader_inline.h, off-limits from here; the bigger base frame (sub esp 98h
// vs 58h) and the per-statement +0xN sizes all follow from it. objdiff leaves it unpaired
// purely on the size gap (0x117 vs 0x88 bytes).
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
}

} // namespace survarium
