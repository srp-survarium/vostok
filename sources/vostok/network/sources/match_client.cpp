////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network/match_client.h>
#include "match_client_impl.h"
#include "network_world.h"
#include "connect_order.h"
#include "enqueue_order.h"
#include "send_queued_order.h"
#include "receive_udp_response.h"
#include "functor_order.h"
#include "functor_response.h"
#include "udp_match_fixed_packets_allocator.h"
#include <vostok/network_core/packet_reader.h>

using vostok::network::match_client;
using vostok::network::match_client_impl;

// TU-local order living at GLOBAL namespace (per the PDB): deferred destruction
// of the match client + its allocators on the network thread
class client_destroyer : public vostok::network::order {
public:
	// STATE[INLINED]: no standalone symbol; init list verified against the inline
	// expansion in match_client's dtor (target rva 0x74c6c0, +0x8d..+0xca)
	inline			client_destroyer	(
						vostok::network_core::udp_match_packets_allocator_ptr const&	responses_allocator,
						vostok::memory::base_allocator&			orders_allocator,
						vostok::network::match_client_impl**	client
					) :
		m_responses_allocator	( responses_allocator ),
		m_orders_allocator		( orders_allocator ),
		m_client				( client )
	{
	}

	virtual			~client_destroyer	( );

	virtual	void	execute				( );

private:
	vostok::network_core::udp_match_packets_allocator_ptr	m_responses_allocator;
	vostok::memory::base_allocator&			m_orders_allocator;
	vostok::network::match_client_impl**	m_client;
}; // class client_destroyer

STATIC_SIZE_ASSERT(client_destroyer, 0x14);

// STATE[89.00%|PARTIAL]: structure 5/5; residual = set_on_disconnect's inlined
// function1::operator= (base copy-swap-clear vs target's folded call) - the
// boost::function-assign inline-vs-call wall (assembly_patterns.md)
void match_client::create_client( vostok::network_core::udp_network_flow_emulator_options const* options )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !*m_client ) );

	match_client_impl* const temp	= VOSTOK_NEW_IMPL( m_world.responses_allocator( ), match_client_impl ) (
		m_world.io_service( ),
		m_packets_orderer,
		options
	);
	vostok::threading::interlocked_exchange_pointer( (pvoid&)*m_client, temp );
	( *m_client )->set_on_packet_received	( boost::bind( &match_client::on_packet_received, this, _1, _2 ) );
	( *m_client )->set_on_disconnect		( boost::bind( &match_client::on_disconnect, this, _1 ) );
}

// STATE[54.18%|PARTIAL]: structure 2/2; residual = base INLINES the intrusive_ptr
// operator=(T*) body (temp/swap/interlocked_decrement) where the target calls the
// esi-promoted COMDAT - the intrusive_ptr inline-vs-call wall (~connect_order class)
void match_client::create_responses_packets_allocator( )
{
	ASSERT					( UNKNOWN_EXPRESSION_T( !m_response_packets_allocator ) );

	m_response_packets_allocator	= VOSTOK_NEW_IMPL( m_world.responses_allocator( ), udp_match_fixed_packets_allocator< 8192 > ) (
		m_world.responses_allocator( )
	);
}

// STATE[94.58%|PARTIAL]: structure 3/3 clean; residual is in the init list -
// base inlines the m_order_packets_allocator intrusive_ptr ctor (zero + set call)
// where the target calls the ctor COMDAT, plus +0x38 ghost frame dwords
 match_client::match_client(
	vostok::network::world&		world,
	vostok::network_core::udp_match_packets_orderer&	packets_orderer,
	vostok::network_core::udp_network_flow_emulator_options const*	options
) :
	m_order_packets_allocator	(
		VOSTOK_NEW_IMPL( static_cast_checked<network_world&>(world).orders_allocator( ), udp_match_fixed_packets_allocator< 8192 > ) (
			static_cast_checked<network_world&>(world).orders_allocator( )
		)
	),
	m_packets_orderer	( packets_orderer ),
	m_world				( static_cast_checked<network_world&>(world) ),
	m_client			( VOSTOK_NEW_IMPL( static_cast_checked<network_world&>(world).orders_allocator( ), match_client_impl* ) )
{
	*m_client			= 0;

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &match_client::create_responses_packets_allocator, this )
		)
	);

	m_world.add_order	(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &match_client::create_client, this, options )
		)
	);
}

// STATE[75.17%|PARTIAL]: the inlined delete_helper body is byte-aligned; residual
// = the compiler-emitted m_responses_allocator dtor (base inlines ~intrusive_ptr,
// target calls it) - the intrusive_ptr inline-vs-call wall
client_destroyer::~client_destroyer( )
{
	VOSTOK_DELETE_IMPL		( m_orders_allocator, m_client );
}

// STATE[0%|PARTIAL]: objdiff scores None (body too divergent to pair, not a
// mangling issue - both sides carry ?execute@client_destroyer@@UAEXXZ); structure
// 2/2 verified via pdb_fetch structure-diff (STRUCTURE MATCH); residual = base
// inlines intrusive_ptr::operator=(0) where the target calls the esi-promoted
// COMDAT, plus the strip_pointer fold call our base keeps before delete_helper
// (destroy_client precedent: byte-identical for g_allocator/*g_allocator)
void client_destroyer::execute( )
{
	VOSTOK_DELETE_IMPL		( *vostok::network::g_allocator, *m_client );
	m_responses_allocator	= 0;
}

// STATE[50.09%|PARTIAL]: structure 3/3; residual = base inlines the
// client_destroyer-ctor's intrusive_ptr copy-ctor, the operator=(0) body and the
// member ~intrusive_ptr where the target keeps all three out-of-line (the
// per-call-site whole-program intrusive_ptr inline-vs-call wall)
 match_client::~match_client( )
{
	order* const order		= VOSTOK_NEW_IMPL( m_world.orders_allocator( ), client_destroyer ) (
		m_response_packets_allocator,
		m_world.orders_allocator( ),
		m_client
	);
	m_response_packets_allocator	= 0;
	m_world.add_order		( order );
}

// STATE[88.33%|PARTIAL]: structure 2/2; residual = the by-value bind copy of
// m_on_connected (base lowers the function0(bind_t) ctor via function-ctor +
// assign_to_own with extra esp-temp slots, target calls the templated ctor direct)
void match_client::on_connected(
	vostok::connection_error_types_enum			connection_error,
	vostok::handshaking_error_types_enum		handshaking_error,
	vostok::socket_error_types_enum				socket_error,
	vostok::lobby_server_message_types_enum		lobby_error
)
{
	if ( m_on_connected )
		m_world.add_response	(
			VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
				boost::bind( m_on_connected, connection_error, handshaking_error, socket_error, lobby_error )
			)
		);
}

// STATE[67.55%|PARTIAL]: structure 3/3; residuals = `m_on_connected = callback`
// inlined as copy-swap-clear (target calls the folded operator=, edi-promoted)
// plus the function-copy folds inside the connect_order-ctor expansion - both the
// documented boost::function-assign inline-vs-call wall
void match_client::connect(
	pcstr			host,
	u16				port,
	u32				current_time_in_ms,
	vostok::network_core::udp_match_packet*	packet,
	boost::function< void( enum vostok::connection_error_types_enum, enum vostok::handshaking_error_types_enum, enum vostok::socket_error_types_enum, enum vostok::lobby_server_message_types_enum ) > const&	callback
)
{
	m_on_connected			= callback;
	boost::function< void( enum vostok::connection_error_types_enum, enum vostok::handshaking_error_types_enum, enum vostok::socket_error_types_enum, enum vostok::lobby_server_message_types_enum ) > const&	on_connected	= boost::bind( &match_client::on_connected, this, _1, _2, _3, _4 );

	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), connect_order ) (
			m_world.orders_allocator( ),
			host,
			m_order_packets_allocator,
			packet,
			boost::bind( &match_client_impl::connect, boost::ref( *m_client ), _1, port, current_time_in_ms, _2, on_connected )
		)
	);
}

// STATE[100%|DONE]
void match_client::disconnect( )
{
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), functor_order ) (
			boost::bind( &match_client_impl::disconnect, boost::ref( *m_client ) )
		)
	);
}

// STATE[100%|DONE]
// claude@MATCH: GLOBAL-scope static - the target symbol is the unmangled
// PDB-private name `enqueue_impl` (no namespaces)
static void enqueue_impl(
		match_client_impl** const					client,
		vostok::network_core::udp_match_packet&		packet
	)
{
	( *client )->enqueue	( ( *client )->clone_packet( packet ) );
}

// STATE[97.89%|PARTIAL]: structure 1/1, +0x2 stmt size; residual = +8 ghost frame
// dwords (LTCG inline-consideration context, string_order-ctor class) + esi spill
void match_client::enqueue( vostok::network_core::udp_match_packet* packet )
{
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), enqueue_order ) (
			boost::bind( &enqueue_impl, m_client, _1 ),
			boost::ref( *packet ),
			m_order_packets_allocator,
			( *m_client )->get_stats( ),
			m_stats
		)
	);
}

// STATE[100%|DONE]
void match_client::send_queued_packets( u32 current_time_in_ms )
{
	m_world.add_order		(
		VOSTOK_NEW_IMPL( m_world.orders_allocator( ), send_queued_order ) (
			boost::bind( &match_client_impl::send_queued_packets, boost::ref( *m_client ), current_time_in_ms ),
			boost::ref( *m_client ),
			m_stats
		)
	);
}

// STATE[100%|DONE]
void match_client::on_packet_received_impl( u8 message_type, vostok::network_core::packet_reader& reader )
{
	if ( m_on_packet_received )
		m_on_packet_received( message_type, reader );
}

// STATE[63.86%|PARTIAL]: structure 4/4; residuals = base inlines the intrusive_ptr
// operator* (with its compiled-out-ASSERT byte) in the new_udp_match_packet arg,
// inlines packet_reader::pointer() at the append site, and lowers the function1
// temp via extra slots - all the per-call-site inline-vs-call wall
void match_client::on_packet_received( u8 message_type, vostok::network_core::packet_reader& reader )
{
	if ( m_on_packet_received ) {
		network_core::udp_match_packet* const packet	= network_core::new_udp_match_packet( *m_response_packets_allocator );
		packet->append		( reader.pointer( ), reader.size_to_eof( ) );

		m_world.add_response	(
			VOSTOK_NEW_IMPL( m_world.responses_allocator( ), receive_udp_response ) (
				boost::bind( &match_client::on_packet_received_impl, this, message_type, _1 ),
				m_response_packets_allocator,
				boost::ref( *packet ),
				( *m_client )->get_stats( ),
				m_stats
			)
		);
	}
}

// STATE[100%|DONE]
void match_client::on_disconnect_impl( vostok::network_core::disconnect_event_types_enum type )
{
	if ( m_on_disconnected )
		m_on_disconnected	( type );
}

// STATE[100%|DONE]
void match_client::on_disconnect( vostok::network_core::disconnect_event_types_enum type )
{
	m_world.add_response	(
		VOSTOK_NEW_IMPL( m_world.responses_allocator( ), functor_response ) (
			boost::bind( &match_client::on_disconnect_impl, this, type )
		)
	);
}

// STATE[75.40%|PARTIAL]: structure 3/3; residual = base inlines the intrusive_ptr
// operator* body (with its compiled-out-ASSERT byte) where the target calls the
// eax-promoted COMDAT - the intrusive_ptr inline-vs-call wall
vostok::network_core::udp_match_packet* match_client::new_packet( u8 message_type )
{
	network_core::udp_match_packet* const result	= network_core::new_udp_match_packet( *m_order_packets_allocator );

	match_client_impl::construct_packet	( m_packets_orderer, *result, message_type );
	return					result;
}

// STATE[100%|DONE]
bool match_client::is_connected( ) const
{
	return					*m_client && ( *m_client )->is_connected( );
}

// STATE[100%|DONE]
bool match_client::is_disconnected( ) const
{
	return					!*m_client || ( *m_client )->is_disconnected( );
}

// STATE[100%|DONE]
u32 match_client::last_receive_time_in_ms( ) const
{
	ASSERT					( UNKNOWN_EXPRESSION_T( *m_client ) );
	return					( *m_client )->last_receive_time_in_ms( );
}
