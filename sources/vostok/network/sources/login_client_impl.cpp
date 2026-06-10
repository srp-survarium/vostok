////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"
#include <vostok/login_server/login_structures.h>

namespace vostok {
namespace network {

// STATE[STUB]: init-list only; the ssl method (sslv23) is a buildability guess
login_client_impl::login_client_impl( boost::asio::io_service& io_service ) :
	m_socket			( io_service ),
	m_ssl_context		( io_service, boost::asio::ssl::context::sslv23 ),
	m_ssl_stream		( m_socket, m_ssl_context ),
	m_ping_socket		( io_service ),
	m_ping_timer		( io_service ),
	m_io_service		( io_service ),
	m_client_state		( signed_out ),
	m_connection_state	( unresolved ),
	m_session_id		( 0 ),
	m_host_port			( 0 ),
	m_in_destructor		( false )
{

	// FUNCTION BODY[0x5796a0]: 17
	// <0x5797b5>|0x115|+0x00d:'30'
	// <0x5797c2>|0x122|+0x00d:'31'
	// <0>
	// <1>
	// <0x5797cf>|0x12f|+0x038:'34'
	// <0x579807>|0x167|+0x010:'35'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x579817>|0x177|+0x03d:'43'
	// <0>
	// <0x579854>|0x1b4|+0x00d:'45'
	// <0x579861>|0x1c1|+0x00d:'46'
	// ******
}

// STATE[STUB]
 login_client_impl::~login_client_impl( )
{
	// FUNCTION BODY[0x579a80]: 39
	// <0x579a90>|0x010|+0x00d:'51'
	// <0>
	// <1>
	// <0x579a9d>|0x01d|+0x032:'54'
	// <0x579acf>|0x04f|+0x005:'55'
	// <0x579ad4>|0x054|+0x005:'56'
	// <0>
	// <0x579ad9>|0x059|+0x026:'58'
	// <0x579aff>|0x07f|+0x002:'59'
	// <0>
	// <0x579b01>|0x081|+0x00d:'61'
	// <0x579b0e>|0x08e|+0x002:'62'
	// <0>
	// <0x579b10>|0x090|+0x002:'64'
	// <0>
	// <0x579b12>|0x092|+0x01c:'66'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x579b2e>|0x0ae|+0x002:'71'
	// <0>
	// <0x579b30>|0x0b0|+0x026:'73'
	// <0x579b56>|0x0d6|+0x002:'74'
	// <0>
	// <0x579b58>|0x0d8|+0x00d:'76'
	// <0x579b65>|0x0e5|+0x002:'77'
	// <0>
	// <0x579b67>|0x0e7|+0x002:'79'
	// <0>
	// <0x579b69>|0x0e9|+0x01c:'81'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x579b85>|0x105|+0x011:'88'
	// <0x579b96>|0x116|+0x013:'89'
	// ******
}

// STATE[STUB]
bool login_client_impl::verify_ssl_certificate( const bool preverified, boost::asio::ssl::verify_context& verify_context )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &preverified, &verify_context );

	return false;

	// FUNCTION BODY[0x579690]: 15
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x579697>|0x007|+0x003:'109'
	// ******
}

// STATE[STUB]
void login_client_impl::establish_connection(
	boost::function< void( enum connection_error_types_enum ) > const&	functor,
	const u32		resolve_retry_count,
	const u32		reconnect_retry_count
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &functor, &resolve_retry_count, &reconnect_retry_count );

	// FUNCTION BODY[0x5799f0]: 14
	// <0x5799f9>|0x009|+0x00c:'115'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x579a05>|0x015|+0x06e:'126'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void login_client_impl::close_connection( const bool stop_ping_timer )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &stop_ping_timer );

	// FUNCTION BODY[0x579880]: 14
	// <0x579897>|0x017|+0x07a:'133'
	// <0>
	// <1>
	// <2>
	// <0x579911>|0x091|+0x008:'137'
	// <0x579919>|0x099|+0x011:'138'
	// <0>
	// <1>
	// <0x57992a>|0x0aa|+0x041:'141'
	// <0x57996b>|0x0eb|+0x04b:'142'
	// <0>
	// <0x5799b6>|0x136|+0x00d:'144'
	// <0x5799c3>|0x143|+0x00b:'145'
	// <0x5799ce>|0x14e|+0x010:'146'
	// ******
}


} // namespace network
} // namespace vostok
