////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/tcp_packet.h>
#include "network_core_memory.h"

namespace vostok {
namespace network_core {

// STATE[STUB]
tcp_packet_client::tcp_packet_client( boost::asio::io_service& io_service ) :
	m_socket		( io_service ),
	// *g_allocator is a buildability placeholder - a matcher supplies the real wiring
	m_packet_socket	( m_socket, *g_allocator ),
	m_io_service	( io_service ),
	m_first_packet	( NULL )
{
	// FUNCTION BODY[0x78cc70]: 1
	// <0x78cd34>|0x0c4|+0x05e:'20'
	// ******
}

// STATE[STUB]
tcp_packet_client::~tcp_packet_client( )
{
	// FUNCTION BODY[0x78cbe0]: 4
	// <0x78cbea>|0x00a|+0x016:'25'
	// <0x78cc00>|0x020|+0x002:'26'
	// <0x78cc02>|0x022|+0x008:'28'
	// ******
}

// STATE[STUB]
void tcp_packet_client::start_reading( )
{
	// FUNCTION BODY[0x78cde0]: 1
	// <0x78cdef>|0x00f|+0x00e:'33'
	// ******
}

// STATE[STUB]
void tcp_packet_client::on_connected( )
{
	// FUNCTION BODY[0x78ce10]: 4
	// <0x78ce1f>|0x00f|+0x021:'38'
	// <0x78ce40>|0x030|+0x011:'39'
	// <0x78ce51>|0x041|+0x00b:'41'
	// ******
}

// STATE[STUB]
void tcp_packet_client::connect( pcstr host, u16 port )
{
	// FUNCTION BODY[0x78ce60]: 1
	// <0x78ce69>|0x009|+0x09e:'46'
	// ******
}

// STATE[STUB]
void tcp_packet_client::disconnect( )
{
	// FUNCTION BODY[0x78cb90]: 3
	// <0x78cb9f>|0x00f|+0x00e:'51'
	// <0x78cbad>|0x01d|+0x024:'52'
	// <0x78cbd1>|0x041|+0x00b:'53'
	// ******
}

// STATE[STUB]
void tcp_packet_client::close_connection( )
{
	// LOCALS
	// boost::system::error_code 		ec
	// ******

	// FUNCTION BODY[0x78cb10]: 5
	// <0x78cb1f>|0x00f|+0x00c:'58'
	// <0x78cb2b>|0x01b|+0x00f:'59'
	// <0x78cb3a>|0x02a|+0x02f:'60'
	// <0x78cb69>|0x059|+0x00b:'61'
	// <0x78cb74>|0x064|+0x011:'62'
	// ******
}

// STATE[STUB]
void tcp_packet_client::send( tcp_packet const& packet )
{
	// FUNCTION BODY[0x78cda0]: 1
	// <0x78cdb9>|0x019|+0x012:'67'
	// ******
}

// STATE[STUB]
void tcp_packet_client::on_error( client_error_codes_enum client_error_code, boost::system::error_code error_code )
{
	// FUNCTION BODY[0x78caa0]: 3
	// <0x78caaf>|0x00f|+0x011:'72'
	// <0x78cac0>|0x020|+0x021:'73'
	// <0x78cae1>|0x041|+0x01d:'74'
	// ******
}

} // namespace network_core
} // namespace vostok
