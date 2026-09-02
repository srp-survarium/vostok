// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_OPTIONS_H_INCLUDED
#define NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_OPTIONS_H_INCLUDED

namespace vostok {
namespace network_core {

struct udp_network_flow_emulator_options {
	// STATE[REMOVED]: the options struct is filled/constructed by its game-module owner
	// and passed to udp_network_flow_emulator by const& - no in-scope TU default-constructs
	// it, so this ctor is never instantiated here. Both sides.
	inline		udp_network_flow_emulator_options	( ) { /* no source */ } // STATE[REMOVED]

public:
	/* 0x0000 */	pcstr		logging_id;
	/* 0x0004 */	float		lost_packet_probability;
	/* 0x0008 */	u32			min_ping_time_in_ms;
	/* 0x000c */	u32			max_ping_time_in_ms;
}; // struct udp_network_flow_emulator_options

STATIC_SIZE_ASSERT(udp_network_flow_emulator_options, 0x10);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_NETWORK_FLOW_EMULATOR_OPTIONS_H_INCLUDED
