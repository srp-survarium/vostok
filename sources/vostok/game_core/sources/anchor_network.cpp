#include "pch.h"
#include "anchor.h"

// Network-module /OPT:REF reachability anchor. Ported verbatim from the pre-split
// temp_include_all.cpp use_network_clients(): every public network-module method
// is CALLED here so the linker keeps the carcass objects in the base EXE for the
// delinker to score (a stub body references nothing on its own). Retire once the
// real call graph (engine -> network) reaches them.
#include <vostok/network_core/http_client.h>
#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/tcp_packet_client.h>
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/udp_match_client.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/network_core/udp_network_flow_emulator.h>
#include <vostok/network_core/udp_network_flow_emulator_options.h>
#include <vostok/network_core/sources/network_core_entry_point.h>
#include <vostok/network_core/udp_match_packet.h>

#include <vostok/network/world.h>
#include <vostok/network/engine.h>
#include <vostok/network/login_client.h>
#include <vostok/network/match_client.h>
#include <vostok/network/tcp_packet_client.h>
#include <vostok/network/http_client.h>
#include <vostok/network/sources/string_order.h>
#include <vostok/network/sources/connect_order.h>
#include <vostok/network/sources/string_response.h>
#include <vostok/network/sources/receive_udp_response.h>
#include <vostok/network/sources/send_queued_order.h>
#include <vostok/network/sources/enqueue_order.h>

namespace vostok
{
	// declared in network/sources/network_entry_point.h, off the public include path
	namespace network {
		void initialize	( );
		void finalize	( );
	} // namespace network

	// concrete orderer so match_client can be constructed (anchor-only; uniquely
	// named to avoid an ODR clash with anchor_network_core.cpp's same-shaped helper)
	struct net_anchor_packets_orderer : network_core::udp_match_packets_orderer
	{
		virtual network_core::udp_match_message_type_info get_sending_message_info( u8 ) 	{ return network_core::udp_match_message_type_info( false, false, 0 ); }
		virtual network_core::udp_match_message_type_info get_received_message_info( u8 ) 	{ return network_core::udp_match_message_type_info( false, false, 0 ); }
	};

	// every public network-module method is CALLED here so /OPT:REF keeps the
	// carcass objects in the exe (a stub body references nothing on its own)
	void use_network_clients()
	{
		network::initialize( );
		network::finalize( );

		network::engine engine;
		memory::stack_allocator stack_allocator;
		network::world* world = network::create_world( engine, stack_allocator );

		{
			network::tcp_packet_client client( *world );
			client.set_on_packet_received( boost::function< void ( network_core::packet_reader& ) >() );
			client.set_on_connected( boost::function< void ( ) >() );
			client.set_on_disconnected( boost::function< void ( ) >() );
			client.set_on_error( boost::function< void ( network_core::client_error_codes_enum, boost::system::error_code ) >() );
			client.connect( "host", 80 );
			network_core::tcp_packet packet( stack_allocator );
			client.send( packet );
			network_core::packet_reader reader( packet );
			client.on_packet_received_impl( reader );
			client.on_packet_received( packet );
			client.on_connected( );
			client.on_connected_impl( );
			client.on_disconnected( );
			client.on_disconnected_impl( );
			client.on_error( ( network_core::client_error_codes_enum )0, boost::system::error_code() );
			client.on_error_impl( ( network_core::client_error_codes_enum )0, boost::system::error_code() );
			client.disconnect( );
		}

		{
			// string_order's 1-string ctor is reached via tcp_packet_client::connect;
			// the 2- and 3-string ctors have standalone target symbols but no
			// reachable caller yet (the login flow is still stubbed) - construct them
			network::string_order order2( stack_allocator, boost::function< void ( pcstr, pcstr ) >( ), "0", "1" );
			network::string_order order3( stack_allocator, boost::function< void ( pcstr, pcstr, pcstr ) >( ), "0", "1", "2" );
			order2.execute( );
			order3.execute( );

			// connect_order's ctor is inlined into its only construction site
			// (match_client::connect, still a stub) - construct one so the
			// vtable emits its dtor/execute/scalar-deleting-dtor
			network_core::udp_match_packets_allocator_ptr packets_allocator;
			network::connect_order order4( stack_allocator, "host", packets_allocator, NULL, boost::function< void ( pcstr, network_core::udp_match_packet const* ) >( ) );
			order4.execute( );
		}

		{
			network::http_client http_client( *world );
			http_client.set_on_error( boost::function< void ( boost::system::error_code ) >() );
			http_client.get( "server", "path", boost::bind( &example_callback, "hello" ) );
			http_client.busy( );
			http_client.get_impl( "server", "path" );
			http_client.create_client_impl( );
			http_client.on_content_downloaded( );
			http_client.on_content_downloaded_impl( "content" );
			http_client.on_error( boost::system::error_code() );
			http_client.on_error_impl( boost::system::error_code() );
		}

		{
			boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum ) > login_callback;
			boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, login_server_message_types_enum, sign_up_info const& ) > sign_up_callback;
			sign_up_info info;

			network::login_client client( *world );
			client.sign_up( "host", 80, info, sign_up_callback );
			client.sign_in( "host", 80, "account", "password", login_callback );
			client.sign_in_impl( "host", 80, "account", "password" );
			client.sign_out( login_callback );
			client.is_signed_in( );
			client.is_signed_out( );
			client.client_state( );
			client.account_name( );
			client.account_password( );
			client.session_id( );
			client.server_browser_address( );
			client.server_browser_initial_query( );
			client.local_ip_address( );
			client.host_ip_address( );
			client.store_user_password_in_settings( );
			client.reset_user_password_in_settings( );
			client.create_client( );
			client.on_signed_up( ( connection_error_types_enum )0, ( handshaking_error_types_enum )0, ( socket_error_types_enum )0, ( login_server_message_types_enum )0, info );
			client.on_signed_in( ( connection_error_types_enum )0, ( handshaking_error_types_enum )0, ( socket_error_types_enum )0, ( login_server_message_types_enum )0 );
			client.on_signed_out( ( connection_error_types_enum )0, ( handshaking_error_types_enum )0, ( socket_error_types_enum )0, ( login_server_message_types_enum )0 );
		}

		{
			net_anchor_packets_orderer packets_orderer;
			boost::function< void ( connection_error_types_enum, handshaking_error_types_enum, socket_error_types_enum, lobby_server_message_types_enum ) > match_callback;

			network::match_client client( *world, packets_orderer, NULL );
			client.connect( "host", 80, 10, NULL, match_callback );
			client.disconnect( );
			client.enqueue( NULL );
			client.send_queued_packets( 10 );
			network_core::udp_match_packet* packet = client.new_packet( 0 );
			client.delete_packet( packet );
			client.set_on_packet_received( boost::function< void ( u8, network_core::packet_reader& ) >() );
			client.set_on_disconnect( boost::function< void ( network_core::disconnect_event_types_enum ) >() );
			client.get_stats( );
			client.is_connected( );
			client.is_disconnected( );
			client.last_receive_time_in_ms( );
			packet = client.new_response_packet( );
			client.delete_response_packet( packet );
			client.create_client( NULL );
			client.create_responses_packets_allocator( );
			client.on_connected( ( connection_error_types_enum )0, ( handshaking_error_types_enum )0, ( socket_error_types_enum )0, ( lobby_server_message_types_enum )0 );
			network_core::tcp_packet tcp_packet( stack_allocator );
			network_core::packet_reader reader( tcp_packet );
			client.on_packet_received_impl( 0, reader );
			client.on_packet_received( 0, reader );
			client.on_disconnect_impl( ( network_core::disconnect_event_types_enum )0 );
			client.on_disconnect( ( network_core::disconnect_event_types_enum )0 );
		}

		// two call sites so LTCG keeps the string_response ctor out-of-line, as the
		// target does (standalone symbol at rva 0x49340)
		{
			network::string_response response( stack_allocator, boost::function< void ( pcstr ) >( ), "string0" );
			response.execute( );
		}
		{
			network::string_response response( stack_allocator, boost::function< void ( pcstr ) >( ), "string1" );
			response.execute( );
		}

		{
			network_core::udp_match_stats stats;
			network::receive_udp_response response(
				boost::function< void ( network_core::packet_reader& ) >( ),
				network_core::udp_match_packets_allocator_ptr( ),
				*static_cast< network_core::udp_match_packet* >( NULL ),
				stats,
				stats
			);
			response.execute( );
		}

		// send_queued_order/enqueue_order ctors are inlined into their only
		// construction sites (match_client::send_queued_packets / enqueue, still
		// stubs) - construct one of each so the vtables emit dtor/execute/
		// scalar-deleting-dtor. NOTE: heap new + scalar delete here was tried and
		// does NOT flip the vtable dtor slot from ??_E (vector) to the target's
		// ??_G (scalar) - it only churns unrelated ICF folds; keep stack objects
		{
			network_core::udp_match_stats stats;
			network::match_client_impl* impl = NULL;
			network::send_queued_order order(
				boost::function< void ( ) >( ),
				impl,
				stats
			);
			order.execute( );
		}

		{
			network_core::udp_match_stats stats;
			network::enqueue_order order(
				boost::function< void ( network_core::udp_match_packet& ) >( ),
				*static_cast< network_core::udp_match_packet* >( NULL ),
				network_core::udp_match_packets_allocator_ptr( ),
				stats,
				stats
			);
			order.execute( );
		}

		network::destroy_world( world );
	}

	void anchor_network( )
	{
		use_network_clients( );
	}
}
