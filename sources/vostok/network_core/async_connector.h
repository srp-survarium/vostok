////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED
#define NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED

namespace vostok {
namespace network_core {

/* vostok::network_core::async_connector */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class handler_allocator;
class boost::function<void(enum client_error_codes_enum,boost::system::error_code)>;
class boost::asio::ip::tcp::socket;
class boost::asio::ip::tcp::resolver::iterator;
class boost::function<void()>;

enum vostok::network_core::async_connector::connection_state_enum /* stored as s32 */ {
	host_name_is_unresolved = 0x0000,
	host_name_is_being_resolved = 0x0001,
	host_name_has_been_resolved = 0x0002,
	connection_is_being_established = 0x0003,
	connection_has_been_established = 0x0004,
}

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

// claude@NOTE: this header is a non-compilable PDB stub; async_connector.cpp is
// ExcludedFromBuild in the vcproj, so the unit is BLOCKED (see
// docs/binary_matching/network_core/async_connector.md). Verified target access
// mangling for the future deep pass: reset/ctor/connect(socket,...) = QAE (public);
// on_connected/on_resolved/connect(iterator const&) = AAE (private).
class async_connector {
public:
					async_connector				( );

			void	connect						( boost::asio::ip::tcp::resolver::iterator const& iterator );

			void	connect						(
						boost::asio::ip::tcp::socket&		socket,
						pcstr								host,
						u16									host_port,
						boost::function<void()> const&		on_connected,
						boost::function<void(enum client_error_codes_enum,boost::system::error_code)> const&	on_error
					);

			void	reset						( );

	inline	bool	is_connected				( ) const { /* no source */ }

	inline	bool	has_connection_established	( ) const { /* no source */ }

			void	on_connected				( boost::system::error_code const& error_code, boost::asio::ip::tcp::resolver::iterator iterator );

			void	on_resolved					( boost::asio::ip::tcp::resolver* resolver, boost::system::error_code const& error_code, boost::asio::ip::tcp::resolver::iterator iterator );

	inline	void	resolve						( pcstr arg_0, u32 arg_1 ) { /* no source */ }

	inline	void	close_connection			( ) { /* no source */ }

	inline	void	on_error					( client_error_codes_enum arg_0, boost::system::error_code arg_1 ) { /* no source */ }

					~async_connector			( );


private:
	/* 0x0000 */	boost::asio::ip::tcp::resolver::iterator	m_host;
	/* 0x0010 */	boost::function<void()>				m_on_connected;
	/* 0x0030 */	boost::function<void(enum client_error_codes_enum,boost::system::error_code)>	m_on_error;
	/* 0x0050 */	handler_allocator					m_allocator;
	/* 0x0458 */	boost::asio::ip::tcp::socket*		m_socket;
	/* 0x045c */	async_connector::connection_state_enum	m_connection_state;
}; // class async_connector

STATIC_SIZE_ASSERT(async_connector, 0x460);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_ASYNC_CONNECTOR_H_INCLUDED