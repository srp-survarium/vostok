// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERVICE_PRICES_H_INCLUDED
#define SERVICE_PRICES_H_INCLUDED

#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/tcp_packet.h>

namespace survarium {

struct service_prices {
public:
	/* 0x0000 */	u32		reroll_cost;
	/* 0x0004 */	u32		add_profile_cost;
	/* 0x0008 */	u32		rename_account_cost;

	// sushi@TODO: inverse-wire model; recover the original writer and append boundaries.
	inline	void	serialize	( network_core::tcp_packet& packet ) const
	{
		packet.append	( reroll_cost );
		packet.append	( add_profile_cost );
		packet.append	( rename_account_cost );
	}
	// sushi@TODO: reader operations are verified; confirm the original named inline boundary.
	inline	void	deserialize	( network_core::packet_reader& reader )
	{
		reroll_cost			= reader.r< u32 >( );
		add_profile_cost	= reader.r< u32 >( );
		rename_account_cost	= reader.r< u32 >( );
	}
}; // struct service_prices

STATIC_SIZE_ASSERT(service_prices, 0xC);

} // namespace survarium

#endif // #ifndef SERVICE_PRICES_H_INCLUDED
