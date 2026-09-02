// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef HIT_INFO_H_INCLUDED
#define HIT_INFO_H_INCLUDED

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}

namespace survarium {

class bullet;

struct hit_info {
					hit_info	( );

					hit_info	(
						const u8		hit_initiator,
						const u8		being_hit,
						pcstr const	body_part_name,
						pcstr const	damage_type,
						const float	amount,
						const float	armor_piercing,
						bullet* const	bullet
					);

			void	deserialize	( network_core::packet_reader& packet );
			void	serialize	( network_core::udp_match_packet& packet ) const { /* no source */ }

public:
	/* 0x0000 */	fixed_string<16>	body_part_name;
	/* 0x001c */	fixed_string<16>	damage_type;
	/* 0x0038 */	bullet*				bullet;
	/* 0x003c */	float				amount;
	/* 0x0040 */	float				armor_piercing;
	/* 0x0044 */	u8					hit_initiator;
	/* 0x0045 */	u8					being_hit;
}; // struct hit_info

STATIC_SIZE_ASSERT(hit_info, 0x48);

} // namespace survarium

#endif // #ifndef HIT_INFO_H_INCLUDED
