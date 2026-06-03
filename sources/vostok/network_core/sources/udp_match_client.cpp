////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "udp_match_client.h"

namespace vostok {
namespace network_core {

// STATE[STUB]
// vostok::network_core::udp_match_client::udp_match_client(boost::asio::io_service&, vostok::memory::single_size_buffer_allocator<300,vostok::threading::single_threading_policy>&, vostok::network_core::udp_match_packets_orderer&, vostok::network_core::udp_network_flow_emulator*)
udp_match_client::udp_match_client(
	boost::asio::io_service&			io_service,
	memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	packets_allocator,
	udp_match_packets_orderer&			packets_orderer,
	udp_network_flow_emulator*			network_flow_emulator)
{
	// FUNCTION BODY[0x758370]: 1
	// <0x75851e>|0x000|0x000:'40'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::on_error(const vostok::network_core::client_error_codes_enum, const boost::system::error_code)
void udp_match_client::on_error( client_error_codes_enum __formal, boost::system::error_code __formal )
{
	// FUNCTION BODY[0x7582c0]: 2
	// 1
	// <0x7582c7>|0x000|0x000:'49'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::process_incoming_packet(vostok::network_core::packet_reader&, boost::asio::ip::basic_endpoint<boost::asio::ip::udp> const&)
void udp_match_client::process_incoming_packet( packet_reader& reader, boost::asio::ip::basic_endpoint<boost::asio::ip::udp> const& endpoint )
{
	// FUNCTION BODY[0x758670]: 6
	// <0x75867f>|0x000|0x000:'78'
	// <0x7586b9>|0x03a|0x03a:'79'
	// <0x7586e1>|0x062|0x028:'80'
	// 1
	// <0x7586e3>|0x064|0x002:'82'
	// <0x7586ef>|0x070|0x00c:'83'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::handle_receive(boost::system::error_code const&, const unsigned int)
void udp_match_client::handle_receive( boost::system::error_code const& error_code, u32 bytes_transferred )
{
	// LOCALS
	// packet_reader 				reader<1>
	// ******

	// FUNCTION BODY[0x758730]: 38
	// <0x75874a>|0x000|0x000:'88'
	// 1
	// <0x758755>|0x00b|0x00b:'90'
	// <0x758761>|0x017|0x00c:'91'
	// 1
	// <0x75876e>|0x024|0x00d:'93'
	// 1
	// 2
	// 3
	// <0x758782>|0x038|0x014:'97'
	// <0x75885c>|0x112|0x0da:'98'
	// <0x758873>|0x129|0x017:'99'
	// 1
	// 2
	// <0x758878>|0x12e|0x005:'102'
	// <0x758882>|0x138|0x00a:'103'
	// <0x758902>|0x1b8|0x080:'104'
	// <0x758919>|0x1cf|0x017:'105'
	// 1
	// 2
	// <0x75891e>|0x1d4|0x005:'108'
	// <0x758954>|0x20a|0x036:'109'
	// <0x7589dd>|0x293|0x089:'110'
	// <0x7589f4>|0x2aa|0x017:'111'
	// 1
	// 2
	// <0x7589f9>|0x2af|0x005:'114'
	// <0x758a08>|0x2be|0x00f:'115'
	// 1
	// <0x758a60>|0x316|0x058:'117'
	// <0x758a62>|0x318|0x002|[1]:'118'
	// <0x758a94>|0x34a|0x032:'119'
	// 1
	// 2
	// <0x758ab0>|0x366|0x01c:'122'
	// 1
	// <0x758abb>|0x371|0x00b:'124'
	// <0x758ad4>|0x38a|0x019:'125'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::start_receiving()
void udp_match_client::start_receiving( )
{
	// FUNCTION BODY[0x758bf0]: 11
	// <0x758bff>|0x000|0x000:'130'
	// <0x758c0b>|0x00c|0x00c:'131'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x758c18>|0x019|0x00d:'140'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::connect(char const*, unsigned short, vostok::network_core::udp_match_packet*, unsigned int)
void udp_match_client::connect(
	pcstr					host,
	u16						port,
	udp_match_packet*		packet,
	u32						current_time_in_ms)
{
	// FUNCTION BODY[0x758cf0]: 14
	// <0x758d01>|0x000|0x000:'145'
	// <0x758d2a>|0x029|0x029:'146'
	// <0x758d3b>|0x03a|0x011:'147'
	// <0x758d57>|0x056|0x01c:'148'
	// 1
	// <0x758d99>|0x098|0x042:'150'
	// 1
	// <0x758de4>|0x0e3|0x04b:'152'
	// 1
	// <0x758df3>|0x0f2|0x00f:'154'
	// <0x758dfe>|0x0fd|0x00b:'155'
	// 1
	// <0x758e09>|0x108|0x00b:'157'
	// <0x758e18>|0x117|0x00f:'158'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::disconnect()
void udp_match_client::disconnect( )
{
	// FUNCTION BODY[0x7582e0]: 1
	// <0x7582e7>|0x000|0x000:'163'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::enqueue(vostok::network_core::udp_match_packet*)
void udp_match_client::enqueue( udp_match_packet* packet )
{
	// FUNCTION BODY[0x758590]: 8
	// <0x7585a1>|0x000|0x000:'168'
	// <0x7585b7>|0x016|0x016:'169'
	// 1
	// <0x7585c3>|0x022|0x00c:'171'
	// <0x7585c8>|0x027|0x005:'172'
	// <0x758642>|0x0a1|0x07a:'173'
	// 1
	// <0x758658>|0x0b7|0x016:'175'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::send_queued_packets(unsigned int)
void udp_match_client::send_queued_packets( u32 current_time_in_ms )
{
	// FUNCTION BODY[0x758af0]: 10
	// <0x758af9>|0x000|0x000:'180'
	// <0x758b09>|0x010|0x010:'181'
	// <0x758baa>|0x0b1|0x0a1:'182'
	// <0x758bc0>|0x0c7|0x016:'183'
	// 1
	// 2
	// <0x758bc2>|0x0c9|0x002:'186'
	// <0x758bce>|0x0d5|0x00c:'187'
	// <0x758bd6>|0x0dd|0x008:'188'
	// <0x758be2>|0x0e9|0x00c:'189'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::check_consistency() const
void udp_match_client::check_consistency( ) const
{
	// LOCALS
	// u32 							registered_packets_count
	// u32 							allocated_count
	// ******

	// FUNCTION BODY[0x758240]: 3
	// <0x758249>|0x000|0x000:'194'
	// <0x758267>|0x01e|0x01e:'195'
	// <0x7582a7>|0x05e|0x040:'196'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_client::on_disconnect(const vostok::network_core::disconnect_event_types_enum)
void udp_match_client::on_disconnect( disconnect_event_types_enum disconnect_type )
{
	// FUNCTION BODY[0x758300]: 6
	// <0x75830f>|0x000|0x000:'201'
	// <0x758320>|0x011|0x011:'202'
	// 1
	// 2
	// <0x758331>|0x022|0x011:'205'
	// <0x758353>|0x044|0x022:'206'
	// ******
}

	// TYPEDEFS
	typedef
		boost::asio::basic_socket<boost::asio::ip::udp,boost::asio::datagram_socket_service<boost::asio::ip::udp> >
		lowest_layer_type;

	typedef
		boost::asio::datagram_socket_service<boost::asio::ip::udp>
		service_type;

	typedef
		boost::asio::ip::basic_endpoint<boost::asio::ip::udp>
		endpoint_type;

	typedef
		boost::asio::ip::udp
		protocol_type;

	typedef
		boost::function<void __cdecl(u8,packet_reader &)>
		client_on_packet_received_type;

	typedef
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_packet,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,8>,udp_match_connection::comparer,u32,1> >
		tree_type;

	typedef
		sockaddr
		data_type;

	// ******

} // namespace network_core
} // namespace vostok
