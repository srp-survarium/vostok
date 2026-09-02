// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// claude@NOTE: structure + locals match (else-if ladder + iterator end checks
// verified); residual = LOG-helper callback-ctor schedule x6
// (log-callback-ctor-schedule.md) + the functor/function2 bind-copy lowering at
// the resolve/async_resolve retry sites
void login_client_impl::on_resolved(
		boost::asio::ip::tcp::resolver* const	resolver,
		const u32								retry_count,
		boost::function< void ( resolve_error_types_enum, boost::asio::ip::tcp::resolver::iterator ) > const&	functor,
		boost::system::error_code const&		error_code,
		boost::asio::ip::tcp::resolver::iterator	iterator
	)
{

	ASSERT			( UNKNOWN_EXPRESSION_T( resolver ) );

	if ( error_code ) {
		LOG_INFO	( "[LOGIN] NOT resolved!\r\n" );
		LOG_INFO	( "[LOGIN] error during resolving: %s", error_code.message( ).c_str( ) );
		m_connection_state	= unresolved;
		if ( retry_count ) {
			LOG_INFO	( "[LOGIN] reconnecting...\r\n" );
			delete		resolver;
			resolve		( functor, retry_count - 1 );
			return;
		}
		else
			if ( iterator != boost::asio::ip::tcp::resolver::iterator( ) ) {
				++iterator;
				if ( iterator != boost::asio::ip::tcp::resolver::iterator( ) ) {
					resolver->async_resolve	(
						iterator->endpoint( ),
						boost::bind(
							&login_client_impl::on_resolved,
							this,
							resolver,
							retry_count,
							functor,
							boost::asio::placeholders::error,
							boost::asio::placeholders::iterator
						)
					);
					return;
				}
			}

		delete		resolver;
		functor		( cannot_resolve, iterator );
		LOG_INFO	( "[LOGIN] can't resolve endpoints: %s", error_code.message( ).c_str( ) );
		LOG_INFO	( "[LOGIN] please, try again later\r\n" );
		return;
	}

	delete			resolver;

	LOG_INFO		( "[LOGIN] resolved!\r\n" );

	m_connection_state	= resolved;
	functor			( successfully_resolved, iterator );
}
// claude@NOTE: structure + locals match; residual = LOG-helper callback-ctor
// schedule + the functor bind-copy lowering in the async_resolve bind; the
// _itoa_s line also hoists m_host_port to an unnamed temp slot the base loads
// inline (compiler temp-scheduling, not a source local)
void login_client_impl::resolve( boost::function< void ( resolve_error_types_enum, boost::asio::ip::tcp::resolver::iterator ) > const& functor, const u32 retry_count )
{
	LOG_INFO		( "[LOGIN] resolving...\r\n" );

	ASSERT			( UNKNOWN_EXPRESSION_T( m_connection_state == unresolved ) );
	m_connection_state	= resolving;

	boost::asio::ip::tcp::resolver* const resolver	= new boost::asio::ip::tcp::resolver( m_io_service );

	char port[6];
	_itoa_s			( m_host_port, port, sizeof( port ), 10 );

	boost::asio::ip::tcp::resolver::query query( boost::asio::ip::tcp::v4( ), m_host, port );
	resolver->async_resolve	(
		query,
		boost::bind(
			&login_client_impl::on_resolved,
			this,
			resolver,
			retry_count,
			functor,
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator
		)
	);
}

} // namespace network
} // namespace vostok
