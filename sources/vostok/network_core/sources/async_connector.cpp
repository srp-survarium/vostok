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

// STATE[63.37%|PARTIAL]: logging-heavy Boost.Asio wall (1520/1510 bytes). The old join
// failure (target `* const`/QAV vs base `*`/PAV) is FIXED: the header now declares the
// resolver param `* const` (the definition drops the top-level const so DELETE() can
// null it; MSVC mangles from the declaration -> QAV) and the symbol pairs.
void async_connector::on_resolved(
	// the header declares this `* const` (target mangles QAV); the definition drops the
	// top-level const so DELETE( resolver ) can take the pointer by T*& and null it.
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

	// STRUCTURE DIFF: target 20 stmts / base 22 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ----------+--------+--------+----+----+------+------
	// BASE_ONLY |--      |0x460d36|--  |0x15|62    |	if ( error_code )
	// SIZE +0x68|0x5452d7|0x460d4b|0x15|0x7d|64    |		LOG_INFO( "NOT host_name_has_been_resolved!" );
	// SIZE +0x57|0x5452ec|0x460dc8|0x80|0xd7|65    |		LOG_INFO( "error during host_name_is_being_resolved: %s", error_code.message( ).c_str( ) );
	// SIZE -0x73|0x54536c|0x460e9f|0xda|0x67|66    |		if ( ++iterator == boost::asio::ip::tcp::resolver::iterator( ) )
	// TRGT_ONLY |0x545446|--      |0x8 |--  |--    |--
	// TRGT_ONLY |0x54544e|--      |0x69|--  |--    |--
	// SIZE -0x94|0x5454b7|0x460f06|0xc2|0x2e|68    |			DELETE( resolver );
	// BASE_ONLY |--      |0x460f34|--  |0x10|69    |			m_connection_state	= host_name_is_unresolved;
	// BASE_ONLY |--      |0x460f44|--  |0xe6|70    |			LOG_INFO( "can't resolve endpoints: %s", error_code.message( ).c_str( ) );
	// BASE_ONLY |--      |0x46102a|--  |0x83|71    |			LOG_INFO( "please, try again later" );
	// BASE_ONLY |--      |0x4610ad|--  |0x1f|72    |			if ( m_on_error )
	// BASE_ONLY |--      |0x4610cc|--  |0x1a|73    |				m_on_error( host_cannot_be_resolved, error_code );
	// SIZE +0x6d|0x54559f|0x46110c|0x17|0x84|76    |		LOG_INFO( "host name has been resolved!" );
	// SIZE -0xd4|0x5455c6|0x4611a0|0xef|0x1b|78    |		m_host				= iterator;
	// SIZE -0x77|0x5456b5|0x4611bb|0x89|0x12|79    |		connect( m_host );
	// TRGT_ONLY |0x54573e|--      |0x1e|--  |--    |--
	// TRGT_ONLY |0x54575c|--      |0x1a|--  |--    |--
	// SIZE -0x5 |0x54579c|0x4611f3|0x17|0x12|82    |	DELETE( resolver );
	// SIZE +0x2 |0x5457b3|0x461205|0x8a|0x8c|83    |	LOG_INFO( "host name has been resolved!" );
	// SIZE +0x3 |0x545868|0x4612bc|0xf |0x12|86    |	connect( m_host );
	// VERDICT: STRUCTURE MISMATCH (quantity) - 20 vs 22; the LOG_INFO inline-vs-call wall flips every row size (target calls a logging helper at 0x15-0x17, base inlines 0x7d-0xe6 formatting blobs) and drags the aligner; the 2 extra base stmts are most likely the two `return;`s (the target's 9-line gap after the inner block suggests if/else shape instead) - left for a follow-up matcher, the wall dominates the score either way.
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
