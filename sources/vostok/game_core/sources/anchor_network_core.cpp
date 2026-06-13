#include "pch.h"
#include "anchor.h"

// #include <boost/asio.hpp>
#include <boost/asio/error.hpp>
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

namespace vostok
{
	// shared opaque sink, declared in anchor.h and used by every anchor TU.
	void example_callback(const char *name)
	{
		printf("%s\n", name);
	}

	void use_network_core_tcp_packet()
	{
		memory::stack_allocator stack_allocator;
		network_core::tcp_packet packet( stack_allocator );

		network_core::buffer_to_send( packet );
		network_core::buffer_to_receive_into( packet );
	}


	void use_network_core_entry_point()
	{
		boost::asio::io_service io_service( 10 );
		network_core::get_ip_address( io_service );

		char dest_host[ 64 ];
		u16 dest_port;
		network_core::get_connection_info_from_string( "host:port", dest_host, dest_port );

		memory::stack_allocator stack_allocator;
		network_core::memory_allocator( stack_allocator );
		network_core::initialize( );
		network_core::finalize( );

		network_core::udp_match_items_stats items_a, items_b;
		network_core::udp_match_stream_stats stream_a, stream_b;
		network_core::udp_match_stats stats_a, stats_b;
		bool r = ( items_a >= items_b ) | ( stream_a >= stream_b ) | ( stats_a >= stats_b );
		printf( "%d", r );

		// address-takes keep the standalone COMDATs the target carries (a direct call
		// would let LTCG inline them into this anchor).
		network_core::udp_match_stats ( *stats_difference )( network_core::udp_match_stats const&, network_core::udp_match_stats const& ) = &network_core::operator-;
		example_callback( ( pcstr )&stats_difference );

		u8 ( network_core::udp_match_packet::*header_size_ptr )( ) const = &network_core::udp_match_packet::header_size;
		example_callback( ( pcstr )&header_size_ptr );
	}

	void use_network_core_tcp_packet_client()
	{
		boost::asio::io_service io_service( 10 );
		network_core::tcp_packet_client c( io_service );

		c.connect( "host", 80 );
		c.disconnect();

		memory::stack_allocator stack_allocator;
		network_core::tcp_packet packet( stack_allocator );
		c.send( packet );

		c.io_service();
	}

	void use_network_core_http_client()
	{
		boost::asio::io_service io_service( 10 );
		network_core::http_client http_client( io_service );
		http_client.set_on_error( boost::bind( use_network_core_http_client ) );
		http_client.get( "server", "path", boost::bind(&example_callback, "hello" ) );

		boost::asio::streambuf buff;
		network_core::read_lines_from_stream( "prefix", buff );
	}

	struct test_udp_match_packets_orderer : network_core::udp_match_packets_orderer
	{
		virtual network_core::udp_match_message_type_info get_sending_message_info( u8 ) 	{ return network_core::udp_match_message_type_info( false, false, 0 ); }
		virtual network_core::udp_match_message_type_info get_received_message_info( u8 ) 	{ return network_core::udp_match_message_type_info( false, false, 0 ); }
	};

	void use_network_core_udp_match_connection()
	{
		boost::asio::io_service io_service( 10 );
		boost::asio::ip::udp::socket socket( io_service );
		boost::asio::ip::udp::endpoint remote_endpoint;
		memory::single_size_buffer_allocator< 300, threading::single_threading_policy > packets_allocator( NULL, 0 );
		test_udp_match_packets_orderer packets_orderer;

		network_core::udp_match_connection connection(
			socket, remote_endpoint, packets_allocator, packets_orderer,
			10, 20, 30, "id"
		);

		connection.connect( NULL );
		connection.enqueue( NULL );
		connection.send_queued_packets( 10 );
		connection.disconnect( );
		connection.instant_disconnect( network_core::disconnected_by_timeout );
		connection.packets_count( );

		network_core::udp_match_connection::is_low_level_packet( *(network_core::base_packet const*)NULL );
		network_core::udp_match_connection::construct_packet( packets_orderer, *(network_core::udp_match_packet*)NULL, 0 );
	}

	void use_network_core_udp_network_flow_emulator_tick_functor( network_core::packet_reader&, boost::asio::ip::udp::endpoint const& )
	{
	}

	void use_network_core_udp_network_flow_emulator()
	{
		memory::stack_allocator stack_allocator;
		static char arena[ 4096 ];
		memory::single_size_buffer_allocator< 300, threading::single_threading_policy > packets_allocator( arena, sizeof( arena ) );
		network_core::udp_network_flow_emulator_options options;
		network_core::udp_network_flow_emulator emulator( stack_allocator, packets_allocator, options );

		emulator.tick( 10, boost::bind( &use_network_core_udp_network_flow_emulator_tick_functor, _1, _2 ) );
		emulator.on_packet_received( NULL, 10, boost::asio::ip::udp::endpoint( ), 10, 10 );
	}

	void use_network_core_udp_match_client()
	{
		boost::asio::io_service io_service( 10 );
		memory::single_size_buffer_allocator< 300, threading::single_threading_policy > packets_allocator( NULL, 0 );
		test_udp_match_packets_orderer packets_orderer;

		network_core::udp_match_client client( io_service, packets_allocator, packets_orderer, NULL );

		client.connect( "host", 80, NULL, 10 );
		client.start_receiving( );
		client.enqueue( NULL );
		client.send_queued_packets( 10 );
		client.disconnect( );
		client.check_consistency( );

		// private on_error is called by handle_receive; the address-take also keeps it standalone
		void ( network_core::udp_match_client::*on_error_ptr )( network_core::client_error_codes_enum, boost::system::error_code ) = &network_core::udp_match_client::on_error;
		example_callback( ( pcstr )&on_error_ptr );

		// private handle_receive's real caller (start_receiving's async_receive_from bind) is still STUB; address-take keeps it
		void ( network_core::udp_match_client::*handle_receive_ptr )( boost::system::error_code const&, u32 ) = &network_core::udp_match_client::handle_receive;
		example_callback( ( pcstr )&handle_receive_ptr );
	}

	void anchor_network_core( )
	{
		use_network_core_http_client();
		use_network_core_tcp_packet();
		use_network_core_entry_point();
		use_network_core_tcp_packet_client();
		use_network_core_udp_match_connection();
		use_network_core_udp_network_flow_emulator();
		use_network_core_udp_match_client();
	}
}
