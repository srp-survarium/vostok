////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SERVICE_PRICES_H_INCLUDED
#define SERVICE_PRICES_H_INCLUDED

/* FORWARD REFS */
class vostok::network_core::packet_reader;
class vostok::network_core::tcp_packet;

namespace survarium {

struct service_prices {
	inline	void	serialize	( network_core::tcp_packet& arg_0 ) const { /* no source */ }
	inline	void	deserialize	( network_core::packet_reader& arg_0 ) { /* no source */ }

public:
	/* 0x0000 */	u32		reroll_cost;
	/* 0x0004 */	u32		add_profile_cost;
	/* 0x0008 */	u32		rename_account_cost;
}; // struct service_prices

STATIC_SIZE_ASSERT(service_prices, 0xC);

} // namespace survarium

#endif // #ifndef SERVICE_PRICES_H_INCLUDED
