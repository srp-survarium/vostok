////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SERVICE_PRICES_H_INCLUDED
#define SERVICE_PRICES_H_INCLUDED

namespace vostok {
namespace network_core {
	class packet_reader;
	class tcp_packet;
} // namespace network_core
} // namespace vostok

namespace survarium {

struct service_prices {
public:
	/* 0x0000 */	u32		reroll_cost;
	/* 0x0004 */	u32		add_profile_cost;
	/* 0x0008 */	u32		rename_account_cost;

	inline	void	serialize	( network_core::tcp_packet& arg_0 ) const { /* no source */ }
	inline	void	deserialize	( network_core::packet_reader& arg_0 ) { /* no source */ }
}; // struct service_prices

STATIC_SIZE_ASSERT(service_prices, 0xC);

} // namespace survarium

#endif // #ifndef SERVICE_PRICES_H_INCLUDED
