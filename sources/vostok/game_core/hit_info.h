////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_INFO_H_INCLUDED
#define HIT_INFO_H_INCLUDED

namespace vostok {
namespace network_core {
	class packet_reader {};		// sushi@TODO
	class udp_match_packet {};	// sushi@TODO
}
}

namespace survarium {

class bullet;

struct hit_info {
					hit_info	(
						u8			hit_initiator,
						u8			being_hit,
						pcstr		body_part_name,
						pcstr		damage_type,
						float		amount,
						float		armor_piercing,
						bullet*		bullet
					);

					hit_info	( );

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
