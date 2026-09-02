// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "network_core_entry_point.h"

// carcass completeness: force-compile the type headers no other enabled TU includes yet
#include <vostok/network_core/udp_match_server.h>
#include <vostok/network_core/udp_match_packets_allocator.h>
#include <vostok/network_core/process_packet_predicate.h>
#include <vostok/network_core/move_to_list_predicate.h>
#include <vostok/network_core/custom_alloc_handler.h>

namespace vostok {
namespace network_core {

// module allocator storage (wired up by memory_allocator())
memory::base_allocator*	g_allocator	= NULL;

typedef boost::asio::ip::basic_resolver_query<boost::asio::ip::tcp>	query_type;

void memory_allocator( memory::base_allocator& allocator )
{
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call empty_stub`, delinker-misnamed finalize_impl)
	g_allocator = &allocator;
}

std::basic_string<char,std::char_traits<char>,std::allocator<char> > get_ip_address( boost::asio::io_service& io_service )
{
	boost::asio::ip::tcp::resolver				resolver( io_service );
	query_type									query( boost::asio::ip::host_name(), "" );
	boost::asio::ip::tcp::resolver::iterator	iter = resolver.resolve( query );
	boost::asio::ip::tcp::resolver::iterator	end;

	for ( ; iter != end; ++iter ) {
		boost::asio::ip::address	addr = iter->endpoint( ).address( );
		if ( !addr.is_loopback( ) && addr.is_v4( ) ) {
			return iter->endpoint( ).address( ).to_string( );
		}
	}

	return "unknown";
	// sushi@TODO: residual is the dead loop closing-brace `jmp short` anchor MSVC emits so
	// the `}` is breakpointable (0x216, unreachable) - our build omits it, shifting later
	// offsets by 2. Find the source/flag lever for it - see review_todos.md.
}

bool get_connection_info_from_string( pcstr buffer, char* const dest_host, u16& dest_port )
{
	pcstr	delim	= strchr( buffer, ':' );
	if ( delim ) {
		u32	port;
		strncpy_s( dest_host, 64, buffer, delim - buffer );
		s32	result	= sscanf_s( delim + 1, "%d", &port );
		if ( strings::length( dest_host ) && result == 1 ) {
			dest_port	= port & 0xffff;
			return true;
		}
	}
	return false;
}

void initialize( )
{
}

void finalize( )
{
}

} // namespace network_core
} // namespace vostok
