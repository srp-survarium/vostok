////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/async_connector.h>
#include "network_core_memory.h"
namespace vostok {
namespace network_core {

// STATE[27%|PARTIAL]: body byte-correct; LTCG ICF fold reps + extra temp slot (frame 10h vs 0Ch)
async_connector::async_connector( ) :
	m_host				( ),
	m_socket			( NULL ),
	m_connection_state	( host_name_is_unresolved )
{
}

// STATE[64%|PARTIAL]: LTCG ICF-folded boost::function reps + register alloc; body byte-correct
void async_connector::on_connected(
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_connection_state == connection_is_being_established ) );
	if ( error_code )
	{
		m_connection_state	= host_name_is_unresolved;
		if ( m_on_error )
			m_on_error( server_cannot_be_connected, error_code );
		return;
	}
	LOG_INFO( "connection_has_been_established!" );
	m_connection_state	= connection_has_been_established;

	if ( m_on_connected )
		m_on_connected( );
}

// STATE[24%|PARTIAL]: structurally byte-correct; anon-ns placeholder hash + register alloc noise
void async_connector::connect( boost::asio::ip::tcp::resolver::iterator const& iterator )
{
	m_connection_state	= connection_is_being_established;
	boost::asio::async_connect(
		*m_socket,
		iterator,
		boost::bind( &async_connector::on_connected, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator ) );
}

// STATE[PARTIAL]: logging-heavy Boost.Asio (~1510/1520 bytes). objdiff can't pair (target
// param `* const`/QAV vs base `*`/PAV; see async_connector.md). Error/end-of-iterator branch
// shape is best-effort, not byte-verified.
void async_connector::on_resolved(
	boost::asio::ip::tcp::resolver*	resolver,
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_connection_state == host_name_is_being_resolved ) );
	if ( error_code )
	{
		LOG_INFO( "NOT host_name_has_been_resolved!" );
		LOG_INFO( "error during host_name_is_being_resolved: %s", error_code.message( ).c_str( ) );
		if ( ++iterator == boost::asio::ip::tcp::resolver::iterator( ) )
		{
			DELETE( resolver );
			m_connection_state	= host_name_is_unresolved;
			LOG_INFO( "can't resolve endpoints: %s", error_code.message( ).c_str( ) );
			LOG_INFO( "please, try again later" );
			if ( m_on_error )
				m_on_error( host_cannot_be_resolved, error_code );
			return;
		}
		LOG_INFO( "host name has been resolved!" );
		m_connection_state	= host_name_has_been_resolved;
		m_host				= iterator;
		connect( m_host );
		return;
	}
	DELETE( resolver );
	LOG_INFO( "host name has been resolved!" );
	m_connection_state	= host_name_has_been_resolved;
	m_host				= iterator;
	connect( m_host );
}

// STATE[PARTIAL]: logging-heavy Boost.Asio resolver NEW + query + boost::bind (~179/177 instrs).
void async_connector::connect(
	boost::asio::ip::tcp::socket&		socket,
	pcstr								host,
	u16									host_port,
	boost::function< void() > const&	on_connected,
	boost::function< void( enum client_error_codes_enum, boost::system::error_code ) > const&	on_error
)
{
	m_socket			= &socket;
	m_connection_state	= host_name_is_unresolved;
	m_on_connected		= on_connected;
	m_on_error			= on_error;
	LOG_INFO( "host name is being resolved..." );
	ASSERT( UNKNOWN_EXPRESSION_T( host ) );
	m_connection_state	= host_name_is_being_resolved;
	boost::asio::ip::tcp::resolver*	resolver	= NEW( boost::asio::ip::tcp::resolver )( m_socket->get_io_service( ) );
	char	port[ 6 ];
	_itoa_s( host_port, port, 10 );
	boost::asio::ip::tcp::resolver::query	query( host, port );
	resolver->async_resolve(
		query,
		boost::bind( &async_connector::on_resolved, this, resolver, boost::asio::placeholders::error, boost::asio::placeholders::iterator ) );
}

// STATE[100%|DONE]
void async_connector::reset( )
{
	m_connection_state	= host_name_is_unresolved;
}

memory::base_allocator*	g_allocator	= 0;

} // namespace network_core
} // namespace vostok
