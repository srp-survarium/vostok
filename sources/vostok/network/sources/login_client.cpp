////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network/login_client.h>
#include "network_world.h"
#include "login_client_impl.h"
#include "string_order.h"
#include "functor_order.h"
#include "functor_response.h"
#include <vostok/console_command.h>
#include <vostok/network_core/sources/network_core_entry_point.h>

// Target linkage keeps these buffers at global scope.
char	s_net_client_account_name[128];
char	s_net_client_account_password_[128];

// The target initializes both commands with a 128-byte user-specific buffer.
static vostok::console_commands::cc_string s_net_client_account_name_cc(
	"account_name", s_net_client_account_name,
	sizeof( s_net_client_account_name ), true,
	vostok::console_commands::command_type_user_specific );
static vostok::console_commands::cc_string s_net_client_account_password_cc(
	"account_password", s_net_client_account_password_,
	sizeof( s_net_client_account_password_ ), true,
	vostok::console_commands::command_type_user_specific );

namespace vostok {
namespace network {

pcstr login_client::account_name( ) const
{
	return				s_net_client_account_name;
}

pcstr login_client::account_password( ) const
{
	return				m_net_client_account_password;
}

// claude@NOTE: residual = the out-of-line strings::copy<128> COMDAT takes its
// args via esi/eax with a 3-byte reg-shuffle delta (LTCG call-boundary arg passing)
void login_client::store_user_password_in_settings( )
{
	strings::copy		( s_net_client_account_password_, m_net_client_account_password );
}

void login_client::reset_user_password_in_settings( )
{
	s_net_client_account_password_[0]	= 0;
}

// claude@NOTE: residual = 1-instruction reg rename at the inlined
// ip_address.c_str( ) read (LTCG slot choice after the get_ip_address sret)
void login_client::create_client( )
{
	ASSERT				( UNKNOWN_EXPRESSION_T( !m_client ) );
	m_client			= NEW( login_client_impl ) ( m_world.io_service( ) );
	std::string ip_address	= network_core::get_ip_address( m_world.io_service( ) );
	strings::copy		( m_local_host_ip, ip_address.c_str( ) );
}

// claude@NOTE: structure + locals match; residual = the three boost::function
// member default-ctors (m_on_sign_*) are inlined with extra [ebp-XX] temp-slot
// spills where the target calls the folded default-ctor with this+offset direct
// (boost::function ctor inline-vs-call wall; match_client ctor precedent)
login_client::login_client( world& world ) :
	m_world				( static_cast_checked<network_world&>(world) ),
	m_client			( 0 ),
	m_client_state		( signed_out )
{
	strings::copy		( m_net_client_account_password, s_net_client_account_password_ );

	m_local_host_ip[0]	= 0;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &login_client::create_client, this )
		)
	);
}

} // namespace network
} // namespace vostok

// claude@NOTE: base keeps the LTCG-promoted strip_pointer fold call before
// delete_helper where the target inlines it - byte-identical for either allocator
// spelling (tcp_packet_client destroy_client precedent)
// claude@MATCH: GLOBAL-scope static - the target symbol is the unmangled
// PDB-private name `destroy_client` (no namespaces), the tcp_packet_client idiom
static void destroy_client( vostok::network::login_client_impl* client_to_destroy )
{
	VOSTOK_DELETE_IMPL	( vostok::network::g_allocator, client_to_destroy );
}

namespace vostok {
namespace network {

// claude@NOTE: residual = the function0(bind_t) copy lowering inside the
// destroy_client functor_order (boost::function ctor inline-vs-call wall)
login_client::~login_client( )
{
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &destroy_client, m_client )
		)
	);
}

// claude@NOTE: structure + locals match; residual = base inlines the
// functor_response bind argument copies (m_on_sign_up function5 + sign_up_info
// by-value - target bind list5 confirms value<sign_up_info>, NOT cref) through
// extra esp temps where the target schedules them through folded ctor COMDATs
// (boost::function copy inline-vs-call wall)
void login_client::on_signed_up(
		connection_error_types_enum			connection_error,
		handshaking_error_types_enum		handshaking_error,
		socket_error_types_enum				socket_error,
		login_server_message_types_enum		login_error,
		sign_up_info const&					sign_up_info
	)
{
	m_client_state		= signed_out;

	if ( !m_on_sign_up )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( m_on_sign_up, connection_error, handshaking_error, socket_error, login_error, sign_up_info )
		)
	);
}

// claude@NOTE: structure + locals match (cmp 8 = servers_connection_info,
// brace-less if/else pair); residual = the same functor_response bind-copy
// lowering as on_signed_up
void login_client::on_signed_in(
		connection_error_types_enum			connection_error,
		handshaking_error_types_enum		handshaking_error,
		socket_error_types_enum				socket_error,
		login_server_message_types_enum		login_error
	)
{
	if ( !connection_error && !handshaking_error && !socket_error && ( login_error == servers_connection_info_message_type ) )
		m_client_state	= signed_in;
	else
		m_client_state	= signed_out;

	if ( !m_on_sign_in )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( m_on_sign_in, connection_error, handshaking_error, socket_error, login_error )
		)
	);
}

// claude@NOTE: residual = 2 bytes of bind-temp slot scheduling in the function4
// conversion at the impl::sign_in boundary (boost::function conversion wall)
void login_client::sign_in_impl(
		pcstr const		host,
		const u16		port,
		pcstr const		account_name,
		pcstr const		password
	)
{
	m_client->sign_in	( host, port, account_name, password, boost::bind( &login_client::on_signed_in, this, _1, _2, _3, _4 ) );
}

// Target emits this only inline; m_sign_up_info must outlive the asynchronous
// boost::ref chain.
void login_client::sign_up(
		pcstr					host,
		u16						port,
		sign_up_info const&		info,
		boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum, sign_up_info const& ) > const&	callback
	)
{
	m_sign_up_info		= info;
	m_on_sign_up		= callback;
	m_client->sign_up	( host, port, m_sign_up_info, boost::bind( &login_client::on_signed_up, this, _1, _2, _3, _4, _5 ) );
}

// claude@NOTE: structure + locals match; residuals = `m_on_sign_in = callback`
// inlined as copy-swap-clear where the target calls the folded operator=
// (edi-promoted), plus the bind_t -> function3 conversion temp lowering in the
// string_order ctor build (boost-function-assign-inline.md wall, both sites)
void login_client::sign_in(
		pcstr			host,
		u16				port,
		pcstr			account_name,
		pcstr			password,
		boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
	)
{
	strings::copy		( m_server_host, host );
	m_server_port		= port;

	strings::copy		( s_net_client_account_name, account_name );
	strings::copy		( m_net_client_account_password, password );

	m_on_sign_in		= callback;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), string_order ) (
			m_world.orders_allocator( ),
			boost::bind(
				&login_client::sign_in_impl,
				this,
				_1,
				port,
				_2,
				_3
			),
			m_server_host,
			s_net_client_account_name,
			m_net_client_account_password
		)
	);
}

// claude@NOTE: structure + locals match; same functor_response bind-copy
// lowering residual as on_signed_up
void login_client::on_signed_out(
		connection_error_types_enum			connection_error,
		handshaking_error_types_enum		handshaking_error,
		socket_error_types_enum				socket_error,
		login_server_message_types_enum		login_error
	)
{
	m_client_state		= signed_out;

	if ( !m_on_sign_out )
		return;

	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( m_on_sign_out, connection_error, handshaking_error, socket_error, login_error )
		)
	);
}

// claude@NOTE: structure + locals match (const& lifetime-extended on_signed_out
// local verified - its bind->function4 assign_to matches the target); residual =
// `m_on_sign_out = callback` copy-swap-clear inline where the target calls the
// folded operator= (boost-function-assign-inline.md wall)
void login_client::sign_out(
		boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum login_server_message_types_enum ) > const&	callback
	)
{
	m_on_sign_out		= callback;
	boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > const& on_signed_out	= boost::bind( &login_client::on_signed_out, this, _1, _2, _3, _4 );

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind(
				&login_client_impl::sign_out,
				m_client,
				on_signed_out
			)
		)
	);
}

u32 login_client::session_id( ) const
{
	return				m_client->session_id( );
}

pcstr login_client::host_ip_address( ) const
{
	return				m_client->host_ip( );
}

pcstr login_client::server_browser_address( ) const
{
	return				m_client->server_browser_address( );
}

pcstr login_client::server_browser_initial_query( ) const
{
	return				m_client->server_browser_initial_query( );
}

} // namespace network
} // namespace vostok
