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

// STATE[100%|DONE]
// claude@MATCH: GLOBAL-scope, extern-linkage buffers (target mangles
// ?s_net_client_account_name@@3PADA - no namespace); note the trailing
// underscore on the password buffer (the original's exact spelling)
char	s_net_client_account_name[128];
char	s_net_client_account_password_[128];

// STATE[0%|DONE]: dynamic-initializer thunks pair as None by design (objdiff
// cannot pair the base ??__E mangling with the demangled target name - the
// cc-initializer pattern, assembly_patterns.md); ctor args read off the target
// initializer bytes (name/buffer/0x80/true/command_type_user_specific)
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

// STATE[100%|DONE]
pcstr login_client::account_name( ) const
{
	return				s_net_client_account_name;
}

// STATE[100%|DONE]
pcstr login_client::account_password( ) const
{
	return				m_net_client_account_password;
}

// STATE[99.77%|PARTIAL]: the out-of-line strings::copy<128> COMDAT takes its args via esi/eax with a 3-byte reg-shuffle delta - LTCG call-boundary arg passing
void login_client::store_user_password_in_settings( )
{
	strings::copy		( s_net_client_account_password_, m_net_client_account_password );
}

// STATE[100%|DONE]
void login_client::reset_user_password_in_settings( )
{
	s_net_client_account_password_[0]	= 0;
}

// STATE[99.85%|PARTIAL]: 1-instruction reg rename at the inlined ip_address.c_str( ) read - LTCG slot choice after the get_ip_address sret
void login_client::create_client( )
{
	ASSERT				( UNKNOWN_EXPRESSION_T( !m_client ) );
	m_client			= NEW( login_client_impl ) ( m_world.io_service( ) );
	std::string ip_address	= network_core::get_ip_address( m_world.io_service( ) );
	strings::copy		( m_local_host_ip, ip_address.c_str( ) );
}

// STATE[76.77%|PARTIAL]: structure 3/3; residual is one statement - base lowers the functor_order bind_t -> boost::function<void()> copy via function-ctor + assign_to with extra esp temps where the target calls the templated ctor direct (the boost::function-ctor inline-vs-call wall, match_client ctor precedent)
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

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - the functor_order bind_t -> function0 copy lowering (function-ctor + assign_to vs direct templated ctor); non-steerable LTCG.
}

} // namespace network
} // namespace vostok

// STATE[90.83%|PARTIAL]: base keeps the LTCG-promoted strip_pointer fold call
// before delete_helper where the target inlines it - byte-identical for either
// allocator spelling (tcp_packet_client destroy_client precedent, same 90.83%)
// claude@MATCH: GLOBAL-scope static - the target symbol is the unmangled
// PDB-private name `destroy_client` (no namespaces), the tcp_packet_client idiom
static void destroy_client( vostok::network::login_client_impl* client_to_destroy )
{
	VOSTOK_DELETE_IMPL	( vostok::network::g_allocator, client_to_destroy );
}

namespace vostok {
namespace network {

// STATE[97.38%|PARTIAL]: same single-statement residual as the ctor (function0(bind_t) copy lowering inside the destroy_client functor_order)
login_client::~login_client( )
{
	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &destroy_client, m_client )
		)
	);
}

// STATE[89.29%|PARTIAL]: structure 4/4; residual = base inlines the
// functor_response bind argument copies (m_on_sign_up function5 + sign_up_info
// by-value) through extra esp temps where the target schedules them through the
// folded ctor COMDATs - the boost::function copy inline-vs-call wall
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - functor_response bind-copy lowering (function5 + info by-value); non-steerable LTCG.
}

// STATE[89.56%|PARTIAL]: structure clean (cmp 8 = servers_connection_info,
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

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - same functor_response bind-copy lowering as on_signed_up; non-steerable LTCG.
}

// STATE[96.88%|PARTIAL]: structure 1/1; residual = 2 bytes of bind-temp
// slot scheduling in the function4 conversion at the impl::sign_in boundary
void login_client::sign_in_impl(
		pcstr const		host,
		const u16		port,
		pcstr const		account_name,
		pcstr const		password
	)
{
	m_client->sign_in	( host, port, account_name, password, boost::bind( &login_client::on_signed_in, this, _1, _2, _3, _4 ) );
}

// STATE[INLINED]: no standalone target symbol - LTCG fully inlined it into its
// only real caller (the optimized game module); body reconstructed: the impl's
// sign-up chain binds boost::ref over the info, so the original must pass the
// long-lived m_sign_up_info member, and the callback routes through
// on_signed_up (which forwards to m_on_sign_up, assigned here); defining it
// here also keeps the whole impl sign-up chain reachable
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

// STATE[75.16%|PARTIAL]: structure 6/6; residuals = `m_on_sign_in = callback`
// inlined as copy-swap-clear (target calls the folded operator=, edi-promoted)
// plus the bind_t -> function3 conversion temp lowered via the converting-ctor
// call where the target inlines assign_to with the late EH-guard `or` - both
// the documented boost::function-assign inline-vs-call wall
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

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - operator= copy-swap-clear inline + bind_t -> function3 conversion temp lowering; non-steerable LTCG.
}

// STATE[88.19%|PARTIAL]: structure 4/4; same functor_response bind-copy
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - same functor_response bind-copy lowering as on_signed_up; non-steerable LTCG.
}

// STATE[80.60%|PARTIAL]: structure 3/3 (const& lifetime-extended on_signed_out
// local verified); residuals = `m_on_sign_out = callback` copy-swap-clear inline
// + the function4 temp conversion lowering - the boost::function-assign wall
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

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (SIZE-only)
	// VERDICT: STRUCTURE MATCH - operator= copy-swap-clear inline + function4 temp conversion lowering; non-steerable LTCG.
}

// STATE[100%|DONE]
u32 login_client::session_id( ) const
{
	return				m_client->session_id( );
}

// STATE[100%|DONE]
pcstr login_client::host_ip_address( ) const
{
	return				m_client->host_ip( );
}

// STATE[100%|DONE]
pcstr login_client::server_browser_address( ) const
{
	return				m_client->server_browser_address( );
}

// STATE[100%|DONE]
pcstr login_client::server_browser_initial_query( ) const
{
	return				m_client->server_browser_initial_query( );
}

} // namespace network
} // namespace vostok
