#include "pch.h"
#include "anchor.h"

// Keep the original include set: network_core_entry_point.h declares
// get_ip_address(io_service&) without including boost::asio itself, and these
// network_core headers pull asio in the engine-safe order (a direct
// <boost/asio.hpp> here drags windows.h in too early -> pvoid C2371). Unused
// includes emit no code; only the anchor FUNCTIONS were pruned.
#include <boost/asio/error.hpp>
#include <vostok/network_core/http_client.h>
#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/udp_match_client.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/sources/network_core_entry_point.h>
#include <vostok/network_core/udp_match_packet.h>

namespace vostok
{
	// shared opaque sink, declared in anchor.h and used by every anchor TU.
	void example_callback(const char *name)
	{
		printf("%s\n", name);
	}

	// The only network_core anchor still needed. Every other network_core symbol
	// is now reached through the network module's real call graph
	// (use_network_clients in anchor_network.cpp): tcp_packet(_client) via
	// network::tcp_packet_client, http_client via network::http_client,
	// udp_match_connection / udp_match_client / udp_network_flow_emulator via
	// match_client(_impl) - so their standalone anchors were dropped (cf. #316,
	// which did the same prune in the pre-split temp_include_all.cpp).
	// Still kept here: get_connection_info_from_string has no real caller anywhere
	// yet, and the two address-takes keep standalone COMDATs whose real callers
	// live in the still-unmatched game module (a direct call would let LTCG inline
	// them into this anchor).
	void use_network_core_entry_point()
	{
		char dest_host[ 64 ];
		u16 dest_port;
		network_core::get_connection_info_from_string( "host:port", dest_host, dest_port );

		network_core::udp_match_stats ( *stats_difference )( network_core::udp_match_stats const&, network_core::udp_match_stats const& ) = &network_core::operator-;
		example_callback( ( pcstr )&stats_difference );

		u8 ( network_core::udp_match_packet::*header_size_ptr )( ) const = &network_core::udp_match_packet::header_size;
		example_callback( ( pcstr )&header_size_ptr );
	}

	void anchor_network_core( )
	{
		use_network_core_entry_point();
	}
}
