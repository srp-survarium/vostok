////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/async_connector.h>
#include "network_core_memory.h"
namespace vostok {
namespace network_core {

// STATE[91.79%|PARTIAL]: body byte-correct; LTCG ICF fold reps + extra temp slot (frame 10h vs 0Ch)
async_connector::async_connector( ) :
	m_host				( ),
	m_socket			( NULL ),
	m_connection_state	( host_name_is_unresolved )
{
}

// STATE[96.25%|PARTIAL]: LTCG ICF-folded boost::function reps + register alloc; body byte-correct
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

// STATE[91.89%|PARTIAL]: structurally byte-correct; anon-ns placeholder hash + register alloc noise
void async_connector::connect( boost::asio::ip::tcp::resolver::iterator const& iterator )
{
	m_connection_state	= connection_is_being_established;
	boost::asio::async_connect(
		*m_socket,
		iterator,
		boost::bind( &async_connector::on_connected, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator ) );
}

// STATE[93.25%|PARTIAL]: 20/20 after reconstructing the error/not-exhausted arm as a
// RETRY re-resolve (target bytes at 0x5454b7: deref *iterator -> endpoint temp,
// bind(&on_resolved, this, resolver, _1, _2), resolver_service::async_resolve), NOT the
// old m_host/connect guess; ++iterator is its own statement, the inner if tests `!=`
// with flat returns. Residual is LOG_INFO __LINE__/lowering + DELETE call-form noise.
void async_connector::on_resolved(
	// the header declares this `* const` (target mangles QAV); the definition drops the
	// top-level const so DELETE( resolver ) can take the pointer by T*& and null it.
	boost::asio::ip::tcp::resolver*	resolver,
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_connection_state == host_name_is_being_resolved ) );

	if ( error_code ) {
		LOG_INFO( "NOT host_name_has_been_resolved!" );
		LOG_INFO( "error during host_name_is_being_resolved: %s", error_code.message( ).c_str( ) );
		++iterator;
		if ( iterator != boost::asio::ip::tcp::resolver::iterator( ) ) {
			resolver->async_resolve(
				*iterator,
				boost::bind(
					&async_connector::on_resolved,
					this,
					resolver,
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator
				)
			);
			return;
		}

		DELETE( resolver );
		m_connection_state	= host_name_is_unresolved;
		LOG_INFO( "can't resolve endpoints: %s", error_code.message( ).c_str( ) );
		LOG_INFO( "please, try again later" );
		if ( m_on_error )
			m_on_error( host_cannot_be_resolved, error_code );
		return;
	}

	DELETE( resolver );
	LOG_INFO( "host name has been resolved!" );
	m_connection_state	= host_name_has_been_resolved;
	m_host				= iterator;
	connect( m_host );

	// STRUCTURE DIFF: target 20 stmts / base 20 stmts (all t.ln == b.ln, 1:1)
	// SIZE -0x3|+3 | LOG_INFO( "NOT host_name_has_been_resolved!" );
	// SIZE -0x3|+4 | LOG_INFO( "error during host_name_is_being_resolved: %s", error_code.message( ).c_str( ) );
	// SIZE -0x4|+20| DELETE( resolver );
	// SIZE -0x6|+22| LOG_INFO( "can't resolve endpoints: %s", error_code.message( ).c_str( ) );
	// SIZE -0x6|+23| LOG_INFO( "please, try again later" );
	// SIZE +0x1|+24| if ( m_on_error )
	// SIZE -0x5|+29| DELETE( resolver );
	// SIZE -0x7|+30| LOG_INFO( "host name has been resolved!" );
	// SIZE +0x3|+33| connect( m_host );
	// VERDICT: STRUCTURE MATCH (shape ok) - 20/20 aligned 1:1; rows are LOG_INFO
	// __LINE__/inline lowering, DELETE helper call form, and connect tail-form noise.
}

// STATE[88.25%|PARTIAL]: logging-heavy Boost.Asio resolver NEW + query + boost::bind.
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

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ----------+--------+--------+----+----+------+------
	// SIZE +0x26|0x5458ec|0x46134b|0x11|0x37|101   |	m_on_error			= on_error;
	// SIZE -0x3 |0x5458fd|0x461382|0x80|0x7d|102   |	LOG_INFO( "host name is being resolved..." );
	// BASE_ONLY |--      |0x461421|--  |0x95|105   |	boost::asio::ip::tcp::resolver*	resolver	= NEW( boost::asio::ip::tcp::resolver )( m_socket->get_io_service( ) );
	// TRGT_ONLY |0x54599f|--      |0x99|--  |--    |--
	// BASE_ONLY |--      |0x4614cb|--  |0x7d|108   |	boost::asio::ip::tcp::resolver::query	query( host, port );
	// SIZE -0x16|0x545a4d|0x461548|0x8e|0x78|111   |		boost::bind( &async_connector::on_resolved, this, resolver, boost::asio::placeholders::error, boost::asio::placeholders::iterator ) );
	// TRGT_ONLY |0x545adb|--      |0x77|--  |--    |--
	// VERDICT: STRUCTURE MATCH (shape ok) - 11/11; the ONLY rows are the SAME resolver-NEW (0x95 vs 0x99) and query-ctor (0x7d vs 0x77) statements in the same ordinal slots, mis-paired by small size drift; rest is boost::function assign + bind marshalling residual, non-steerable.
}

// STATE[100%|DONE]
void async_connector::reset( )
{
	m_connection_state	= host_name_is_unresolved;
}

} // namespace network_core
} // namespace vostok
