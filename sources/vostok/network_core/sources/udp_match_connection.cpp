////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "udp_match_connection.h"

namespace vostok {
namespace network_core {

// STATE[STUB]
// vostok::network_core::udp_match_connection::udp_match_connection(boost::asio::basic_datagram_socket<boost::asio::ip::udp,boost::asio::datagram_socket_service<boost::asio::ip::udp> >&, boost::asio::ip::basic_endpoint<boost::asio::ip::udp> const&, vostok::memory::single_size_buffer_allocator<300,vostok::threading::single_threading_policy>&, vostok::network_core::udp_match_packets_orderer&, unsigned int, unsigned int, unsigned int, char const*)
udp_match_connection::udp_match_connection(
	boost::asio::basic_datagram_socket<boost::asio::ip::udp,boost::asio::datagram_socket_service<boost::asio::ip::udp> >&	socket,
	boost::asio::ip::basic_endpoint<boost::asio::ip::udp> const&	remote_endpoint,
	memory::single_size_buffer_allocator<300,threading::single_threading_policy>&	packets_allocator,
	udp_match_packets_orderer&			packets_orderer,
	u32									disconnection_timeout_in_ms,
	u32									max_packet_wait_time_in_ms,
	u32									max_idle_time_in_ms,
	pcstr								logging_id)
{
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::~udp_match_connection()
void udp_match_connection::~udp_match_connection( )
{
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::on_error(vostok::network_core::client_error_codes_enum, boost::system::error_code)
void udp_match_connection::on_error( client_error_codes_enum __formal, boost::system::error_code __formal )
{
}

// STATE[STUB]
// static bool vostok::network_core::udp_match_connection::is_low_level_packet(vostok::network_core::base_packet const&)
static bool udp_match_connection::is_low_level_packet( base_packet const& packet )
{
	// LOCALS
	// packet_reader 				reader
	// u16 							bits
	// ******

	return false;
	// FUNCTION BODY
	// <0x5564d6>|0x000|0x000:'85'
	// 1
	// <0x5564e7>|0x011|0x011:'87'
	// <0x5564fb>|0x025|0x014:'88'
	// <0x55650f>|0x039|0x014:'89'
	// <0x55651b>|0x045|0x00c:'90'
	// <0x556524>|0x04e|0x009:'91'
	// 1
	// <0x556528>|0x052|0x004:'93'
	// <0x55653c>|0x066|0x014:'94'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::handle_send(vostok::network_core::udp_match_packet*, boost::system::error_code const&, unsigned int)
void udp_match_connection::handle_send( udp_match_packet* packet, boost::system::error_code const& error_code, u32 bytes_transferred )
{
	// LOCALS
	// bool 						success
	// u8* 							buffer<1>
	// ******

	// FUNCTION BODY
	// <0x556be7>|0x000|0x000:'99'
	// 1
	// <0x556c02>|0x01b|0x01b:'101'
	// <0x556c1a>|0x033|0x018:'102'
	// 1
	// <0x556c26>|0x03f|0x00c:'104'
	// <0x556c38>|0x051|0x012:'105'
	// 1
	// <0x556c51>|0x06a|0x019:'107'
	// <0x556c56>|0x06f|0x005:'108'
	// <0x556cc2>|0x0db|0x06c:'109'
	// 1
	// <0x556cdb>|0x0f4|0x019:'111'
	// <0x556cdd>|0x0f6|0x002|[1]:'112'
	// <0x556ce6>|0x0ff|0x009:'113'
	// <0x556d09>|0x122|0x023:'114'
	// 1
	// 2
	// 3
	// <0x556d20>|0x139|0x017:'118'
	// <0x556d35>|0x14e|0x015:'119'
	// <0x556dfe>|0x217|0x0c9:'120'
	// <0x556e15>|0x22e|0x017:'121'
	// 1
	// 2
	// <0x556e1a>|0x233|0x005:'124'
	// <0x556e24>|0x23d|0x00a:'125'
	// <0x556e9b>|0x2b4|0x077:'126'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::send(vostok::network_core::udp_match_packet*)
void udp_match_connection::send( udp_match_packet* packet )
{
	// FUNCTION BODY
	// <0x556fe0>|0x000|0x000:'133'
	// <0x556ff3>|0x013|0x013:'134'
	// <0x557022>|0x042|0x02f:'135'
	// 1
	// <0x557055>|0x075|0x033:'137'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// <0x557070>|0x090|0x01b:'151'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::fill_packet_header(vostok::network_core::udp_match_packet&)
void udp_match_connection::fill_packet_header( udp_match_packet& packet )
{
	// LOCALS
	// u8* 							buffer
	// udp_match_packets_count_enum packet_type
	// ******

	// FUNCTION BODY
	// <0x555bc9>|0x000|0x000:'156'
	// 1
	// <0x555bd2>|0x009|0x009:'158'
	// <0x555bdb>|0x012|0x009:'159'
	// 1
	// <0x555be7>|0x01e|0x00c:'161'
	// <0x555c09>|0x040|0x022:'162'
	// 1
	// <0x555c2e>|0x065|0x025:'164'
	// <0x555c51>|0x088|0x023:'165'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::send_packets_list(vostok::network_core::udp_match_packet*, unsigned int)
void udp_match_connection::send_packets_list( udp_match_packet* packets_list, u32 packets_count )
{
	// LOCALS
	// udp_match_packet* 			packet_to_send
	// buffer_vector<udp_match_packet *> packets
	// udp_match_packet* 			i<1>
	// udp_match_packet** 			e<2>
	// udp_match_packet** 			i<2>
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector<udp_match_packet *>
	// 	packets_type;

	// ******

	// FUNCTION BODY
	// <0x55718d>|0x000|0x000:'170'
	// 1
	// <0x55719c>|0x00f|0x00f:'172'
	// <0x5571a9>|0x01c|0x00d:'173'
	// <0x5571b5>|0x028|0x00c:'174'
	// 1
	// <0x5571c1>|0x034|0x00c:'176'
	// 1
	// <0x5571d5>|0x048|0x014:'178'
	// <0x5571e1>|0x054|0x00c:'179'
	// <0x5571f0>|0x063|0x00f:'180'
	// 1
	// <0x55721d>|0x090|0x02d:'182'
	// <0x55722c>|0x09f|0x00f:'183'
	// <0x557240>|0x0b3|0x014:'184'
	// 1
	// 2
	// <0x557254>|0x0c7|0x014:'187'
	// 1
	// <0x557268>|0x0db|0x014:'189'
	// <0x557274>|0x0e7|0x00c:'190'
	// 1
	// 2
	// 3
	// <0x557279>|0x0ec|0x005:'194'
	// <0x557285>|0x0f8|0x00c:'195'
	// <0x55729a>|0x10d|0x015:'196'
	// <0x5572a9>|0x11c|0x00f:'197'
	// <0x5572bf>|0x132|0x016:'198'
	// <0x5572c6>|0x139|0x007:'199'
	// 1
	// 2
	// <0x5572d2>|0x145|0x00c:'202'
	// 1
	// <0x557308>|0x17b|0x036|[1]:'204'
	// <0x557323>|0x196|0x01b:'205'
	// 1
	// 2
	// <0x55732f>|0x1a2|0x00c:'208'
	// 1
	// <0x557343>|0x1b6|0x014:'210'
	// <0x55734f>|0x1c2|0x00c:'211'
	// <0x55735e>|0x1d1|0x00f:'212'
	// 1
	// <0x557376>|0x1e9|0x018:'214'
	// <0x557385>|0x1f8|0x00f:'215'
	// <0x55739d>|0x210|0x018:'216'
	// 1
	// 2
	// 3
	// <0x5573b1>|0x224|0x014:'220'
	// <0x5573bd>|0x230|0x00c:'221'
	// <0x5573ce>|0x241|0x011:'222'
	// <0x5573ed>|0x260|0x01f:'223'
	// 1
	// <0x5573f2>|0x265|0x005:'225'
	// 1
	// <0x557406>|0x279|0x014:'227'
	// 1
	// <0x557412>|0x285|0x00c|[2]:'229'
	// <0x557431>|0x2a4|0x01f:'230'
	// <0x557446>|0x2b9|0x015:'231'
	// 1
	// <0x55745c>|0x2cf|0x016:'233'
	// <0x55745e>|0x2d1|0x002:'234'
	// <0x557474>|0x2e7|0x016:'235'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::dump(char const* const, const unsigned int)
void udp_match_connection::dump( pcstr caption, u32 current_time_in_ms )
{
	// FUNCTION BODY
	// <0x555b89>|0x000|0x000:'240'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// ******
}

// STATE[STUB]
// vostok::network_core::udp_match_packet* vostok::network_core::udp_match_connection::new_low_level_packet(unsigned char)
udp_match_packet* udp_match_connection::new_low_level_packet( u8 message_type )
{
	// LOCALS
	// udp_match_packet* 			packet
	// pcstr 						message_id_string
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x556eca>|0x000|0x000:'295'
	// <0x556edf>|0x015|0x015:'296'
	// 1
	// <0x556ee6>|0x01c|0x007:'298'
	// 1
	// <0x556ef0>|0x026|0x00a:'300'
	// 1
	// 2
	// <0x556efd>|0x033|0x00d:'303'
	// 1
	// <0x556f17>|0x04d|0x01a:'305'
	// <0x556f1e>|0x054|0x007:'306'
	// 1
	// 2
	// <0x556f20>|0x056|0x002:'309'
	// <0x556f27>|0x05d|0x007:'310'
	// 1
	// 2
	// <0x556f29>|0x05f|0x002:'313'
	// <0x556f30>|0x066|0x007:'314'
	// 1
	// 2
	// <0x556f32>|0x068|0x002:'317'
	// 1
	// 2
	// 3
	// 4
	// <0x556f39>|0x06f|0x007:'322'
	// <0x556f48>|0x07e|0x00f:'323'
	// <0x556f75>|0x0ab|0x02d:'324'
	// <0x556f84>|0x0ba|0x00f:'325'
	// <0x556fad>|0x0e3|0x029:'326'
	// 1
	// 2
	// <0x556fbc>|0x0f2|0x00f:'329'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::send_queued_packets(unsigned int)
void udp_match_connection::send_queued_packets( u32 current_time_in_ms )
{
	// LOCALS
	// buffer_vector<udp_match_packet *> packets
	// u32 							packets_count
	// udp_match_packet* 			packet<1>
	// udp_match_packet* 			packet<1>
	// u32 							size_left<1>
	// udp_match_packet* 			packet_list_tail<1>
	// sequence_number<u16> 		test<1>
	// u32 							packets_count<1>
	// udp_match_packet** 			e<2>
	// udp_match_packet** 			i<2>
	// std::reverse_iterator<udp_match_packet * *> e<2>
	// std::reverse_iterator<udp_match_packet * *> i<2>
	// ******

	// STATICS
	// static <NoType> 				 = <0x557c70>;
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector<udp_match_packet *>
	// 	packets_type;

	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5464f3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN36") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x546536 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN34") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x546571 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN29") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5465c3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN27") })
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x5574c0>|0x000|0x000:'336'
	// 1
	// <0x5574d4>|0x014|0x014:'338'
	// 1
	// <0x5574f3>|0x033|0x01f:'340'
	// <0x55751f>|0x05f|0x02c:'341'
	// <0x55752c>|0x06c|0x00d:'342'
	// 1
	// <0x557531>|0x071|0x005:'344'
	// 1
	// 2
	// <0x557536>|0x076|0x005:'347'
	// <0x557542>|0x082|0x00c:'348'
	// 1
	// <0x55754e>|0x08e|0x00c:'350'
	// <0x55756f>|0x0af|0x021:'351'
	// 1
	// 2
	// <0x557571>|0x0b1|0x002:'354'
	// 1
	// <0x55758e>|0x0ce|0x01d:'356'
	// <0x55759b>|0x0db|0x00d:'357'
	// 1
	// 2
	// <0x5575a0>|0x0e0|0x005:'360'
	// <0x5575c1>|0x101|0x021:'361'
	// 1
	// 2
	// <0x5575c3>|0x103|0x002:'364'
	// <0x5575cf>|0x10f|0x00c:'365'
	// 1
	// 2
	// 3
	// 4
	// <0x5575d1>|0x111|0x002:'370'
	// 1
	// <0x557635>|0x175|0x064:'372'
	// <0x557648>|0x188|0x013:'373'
	// <0x55764e>|0x18e|0x006:'374'
	// 1
	// <0x55766b>|0x1ab|0x01d:'376'
	// 1
	// 2
	// <0x557670>|0x1b0|0x005:'379'
	// <0x557691>|0x1d1|0x021:'380'
	// 1
	// 2
	// 3
	// <0x557698>|0x1d8|0x007:'384'
	// 1
	// <0x5576ac>|0x1ec|0x014:'386'
	// <0x5576f7>|0x237|0x04b:'387'
	// <0x557710>|0x250|0x019|[1]:'388'
	// <0x557724>|0x264|0x014:'389'
	// 1
	// 2
	// <0x55772e>|0x26e|0x00a:'392'
	// <0x557750>|0x290|0x022:'393'
	// <0x55777b>|0x2bb|0x02b:'394'
	// 1
	// <0x557780>|0x2c0|0x005:'396'
	// 1
	// <0x5577a6>|0x2e6|0x026:'398'
	// <0x5577bc>|0x2fc|0x016|[1]:'399'
	// <0x5577cd>|0x30d|0x011:'400'
	// 1
	// <0x5577f9>|0x339|0x02c|[2]:'402'
	// <0x557818>|0x358|0x01f:'403'
	// <0x55782c>|0x36c|0x014:'404'
	// <0x557845>|0x385|0x019:'405'
	// <0x557847>|0x387|0x002:'406'
	// <0x55788d>|0x3cd|0x046:'407'
	// 1
	// <0x557892>|0x3d2|0x005:'409'
	// 1
	// 2
	// <0x557897>|0x3d7|0x005:'412'
	// <0x5578b2>|0x3f2|0x01b:'413'
	// <0x5578cd>|0x40d|0x01b:'414'
	// 1
	// <0x5578d3>|0x413|0x006:'416'
	// <0x5578dc>|0x41c|0x009:'417'
	// <0x5578e8>|0x428|0x00c:'418'
	// <0x55792d>|0x46d|0x045:'419'
	// <0x557939>|0x479|0x00c:'420'
	// <0x557948>|0x488|0x00f:'421'
	// 1
	// <0x55794f>|0x48f|0x007:'423'
	// <0x55795b>|0x49b|0x00c:'424'
	// <0x55797d>|0x4bd|0x022:'425'
	// <0x557996>|0x4d6|0x019|[2]:'426'
	// <0x5579e5>|0x525|0x04f:'427'
	// <0x557a3f>|0x57f|0x05a:'428'
	// <0x557a6f>|0x5af|0x030:'429'
	// <0x557a95>|0x5d5|0x026:'430'
	// <0x557ab8>|0x5f8|0x023:'431'
	// <0x557adf>|0x61f|0x027:'432'
	// <0x557b05>|0x645|0x026:'433'
	// <0x557b3e>|0x67e|0x039:'434'
	// <0x557b7a>|0x6ba|0x03c:'435'
	// 1
	// <0x557b83>|0x6c3|0x009:'437'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x557b88>|0x6c8|0x005:'446'
	// 1
	// 2
	// <0x557bf8>|0x738|0x070:'449'
	// <0x557c07>|0x747|0x00f:'450'
	// <0x557c1a>|0x75a|0x013:'451'
	// 1
	// <0x557c1f>|0x75f|0x005:'453'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::connect(vostok::network_core::udp_match_packet*)
void udp_match_connection::connect( udp_match_packet* packet )
{
	// FUNCTION BODY
	// <0x555d39>|0x000|0x000:'458'
	// <0x555d45>|0x00c|0x00c:'459'
	// <0x555d52>|0x019|0x00d:'460'
	// <0x555d58>|0x01f|0x006:'461'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::enqueue_impl(vostok::network_core::udp_match_packet*)
void udp_match_connection::enqueue_impl( udp_match_packet* packet )
{
	// LOCALS
	// sequence_number<u16>& 		sent_order_id<1>
	// u8* 							stream<1>
	// ******

	// FUNCTION BODY
	// <0x555c69>|0x000|0x000:'466'
	// <0x555c7c>|0x013|0x013|[1]:'467'
	// <0x555c98>|0x02f|0x01c:'468'
	// <0x555cad>|0x044|0x015:'469'
	// <0x555cbe>|0x055|0x011:'470'
	// <0x555cdf>|0x076|0x021:'471'
	// 1
	// 2
	// <0x555cef>|0x086|0x010:'474'
	// <0x555d01>|0x098|0x012:'475'
	// 1
	// <0x555d10>|0x0a7|0x00f:'477'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::enqueue(vostok::network_core::udp_match_packet*)
void udp_match_connection::enqueue( udp_match_packet* packet )
{
	// FUNCTION BODY
	// <0x555e29>|0x000|0x000:'482'
	// <0x555e35>|0x00c|0x00c:'483'
	// <0x555e41>|0x018|0x00c:'484'
	// 1
	// 2
	// <0x555e43>|0x01a|0x002:'487'
	// <0x555e4f>|0x026|0x00c:'488'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::update_acknowledgements(vostok::network_core::sequence_number<unsigned short>, vostok::network_core::sequence_number<unsigned short>, unsigned short)
void udp_match_connection::update_acknowledgements( sequence_number<u16> remote_sequence_id, sequence_number<u16> local_sequence_id, u16 local_acknowledgement_bits )
{
	// LOCALS
	// u32 							local_sequence_difference
	// u32 							remote_sequence_difference
	// u16 							acknowledgement_bits
	// u16 							last_local_acknowledgement_bits
	// u32 							difference<1>
	// u32 							unacknowledged_packets_size<2>
	// sequence_number<u16> 		sequence_id<1>
	// u32 							unacknowledged_packets_size<2>
	// ******

	// FUNCTION BODY
	// <0x555e7f>|0x000|0x000:'521'
	// <0x555e8b>|0x00c|0x00c:'522'
	// <0x555ea7>|0x028|0x01c:'523'
	// <0x555ee5>|0x066|0x03e:'524'
	// <0x555f04>|0x085|0x01f:'525'
	// 1
	// <0x555f1d>|0x09e|0x019:'527'
	// 1
	// <0x555f39>|0x0ba|0x01c:'529'
	// 1
	// 2
	// <0x555f3e>|0x0bf|0x005:'532'
	// <0x555f4a>|0x0cb|0x00c:'533'
	// <0x555f6a>|0x0eb|0x020|[1]:'534'
	// <0x555f86>|0x107|0x01c:'535'
	// <0x555f90>|0x111|0x00a|[2]:'536'
	// <0x555f96>|0x117|0x006:'537'
	// 1
	// 2
	// 3
	// <0x555fc1>|0x142|0x02b:'541'
	// <0x555fd4>|0x155|0x013:'542'
	// <0x55604f>|0x1d0|0x07b:'543'
	// 1
	// 2
	// <0x556078>|0x1f9|0x029:'546'
	// 1
	// 2
	// <0x55607d>|0x1fe|0x005:'549'
	// <0x556099>|0x21a|0x01c:'550'
	// <0x5560ce>|0x24f|0x035:'551'
	// 1
	// <0x5560e0>|0x261|0x012:'553'
	// 1
	// 2
	// <0x5560e5>|0x266|0x005:'556'
	// <0x556119>|0x29a|0x034:'557'
	// 1
	// <0x55612a>|0x2ab|0x011:'559'
	// <0x556136>|0x2b7|0x00c:'560'
	// <0x556154>|0x2d5|0x01e:'561'
	// 1
	// <0x556160>|0x2e1|0x00c:'563'
	// <0x556177>|0x2f8|0x017|[1]:'564'
	// <0x5561a4>|0x325|0x02d:'565'
	// <0x5561b4>|0x335|0x010|[2]:'566'
	// <0x5561c7>|0x348|0x013:'567'
	// <0x556260>|0x3e1|0x099:'568'
	// <0x556289>|0x40a|0x029:'569'
	// 1
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::process_low_level_message(vostok::network_core::packet_reader&, const unsigned int)
void udp_match_connection::process_low_level_message( packet_reader& reader, u32 time_in_ms )
{
	// LOCALS
	// udp_match_connection::low_level_message_type_enum message_type<1>
	// ******

	// FUNCTION BODY
	// <0x556ac1>|0x000|0x000|[1]:'575'
	// 1
	// 2
	// <0x556aeb>|0x02a|0x02a:'578'
	// 1
	// <0x556af7>|0x036|0x00c:'580'
	// 1
	// 2
	// 3
	// <0x556afc>|0x03b|0x005:'584'
	// <0x556b09>|0x048|0x00d:'585'
	// <0x556b15>|0x054|0x00c:'586'
	// 1
	// 2
	// 3
	// <0x556b1a>|0x059|0x005:'590'
	// <0x556b26>|0x065|0x00c:'591'
	// <0x556ba0>|0x0df|0x07a:'592'
	// 1
	// 2
	// <0x556ba2>|0x0e1|0x002:'595'
	// <0x556bae>|0x0ed|0x00c:'596'
	// 1
	// <0x556bba>|0x0f9|0x00c:'598'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::instant_disconnect(vostok::network_core::disconnect_event_types_enum)
void udp_match_connection::instant_disconnect( disconnect_event_types_enum type )
{
	// LOCALS
	// u32 							i<1>
	// u32 							n<1>
	// udp_match_connection::channel& channel<2>
	// udp_match_packet* 			packet<3>
	// ******

	// FUNCTION BODY
	// <0x5565af>|0x000|0x000:'649'
	// 1
	// <0x5565bf>|0x010|0x010:'651'
	// <0x5565cf>|0x020|0x010:'652'
	// <0x5565df>|0x030|0x010:'653'
	// <0x5565ef>|0x040|0x010:'654'
	// <0x5565ff>|0x050|0x010:'655'
	// <0x55660e>|0x05f|0x00f:'656'
	// <0x55661d>|0x06e|0x00f:'657'
	// <0x55662d>|0x07e|0x010:'658'
	// <0x55664f>|0x0a0|0x022:'659'
	// <0x556670>|0x0c1|0x021:'660'
	// <0x556692>|0x0e3|0x022:'661'
	// 1
	// <0x5566b4>|0x105|0x022:'663'
	// <0x55670c>|0x15d|0x058:'664'
	// 1
	// <0x556764>|0x1b5|0x058|[1]:'666'
	// <0x556789>|0x1da|0x025|[2]:'667'
	// <0x55679f>|0x1f0|0x016:'668'
	// <0x5567b8>|0x209|0x019|[3]:'669'
	// <0x5567e5>|0x236|0x02d:'670'
	// <0x556806>|0x257|0x021:'671'
	// <0x55681f>|0x270|0x019:'672'
	// 1
	// <0x556824>|0x275|0x005:'674'
	// <0x55682c>|0x27d|0x008:'675'
	// 1
	// 2
	// <0x556831>|0x282|0x005:'678'
	// <0x556853>|0x2a4|0x022:'679'
	// ******
}

// STATE[STUB]
// void vostok::network_core::udp_match_connection::disconnect()
void udp_match_connection::disconnect( )
{
	// LOCALS
	// sequence_number<u16> 		test
	// u32 							i<1>
	// u32 							n<1>
	// udp_match_connection::channel& channel<2>
	// udp_match_packet* 			packet<3>
	// ******

	// FUNCTION BODY
	// <0x55687f>|0x000|0x000:'684'
	// 1
	// <0x55688b>|0x00c|0x00c:'686'
	// 1
	// <0x55689b>|0x01c|0x010:'688'
	// <0x5568ac>|0x02d|0x011:'689'
	// <0x5568d4>|0x055|0x028:'690'
	// <0x5568e1>|0x062|0x00d:'691'
	// 1
	// 2
	// <0x5568e6>|0x067|0x005:'694'
	// <0x55693e>|0x0bf|0x058:'695'
	// 1
	// <0x556996>|0x117|0x058|[1]:'697'
	// <0x5569bb>|0x13c|0x025|[2]:'698'
	// <0x5569d1>|0x152|0x016:'699'
	// <0x5569ea>|0x16b|0x019|[3]:'700'
	// <0x556a11>|0x192|0x027:'701'
	// <0x556a32>|0x1b3|0x021:'702'
	// <0x556a4b>|0x1cc|0x019:'703'
	// <0x556a4d>|0x1ce|0x002:'704'
	// 1
	// <0x556a52>|0x1d3|0x005:'706'
	// <0x556a7a>|0x1fb|0x028:'707'
	// ******
}

// STATE[STUB]
// unsigned int vostok::network_core::udp_match_connection::packets_count() const
u32 udp_match_connection::packets_count( ) const
{
	// LOCALS
	// u32 							result
	// u32 							i<1>
	// u32 							n<1>
	// ******

	return 0;
	// FUNCTION BODY
	// <0x555d7a>|0x000|0x000:'712'
	// 1
	// <0x555daa>|0x030|0x030|[1]:'714'
	// <0x555dcb>|0x051|0x021:'715'
	// 1
	// <0x555e16>|0x09c|0x04b:'717'
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
		boost::function<void __cdecl(udp_match_client_session &,u8,packet_reader &)>
		on_packet_received_type;

	typedef
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_client_session,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,1372>,udp_match_server::comparer,u32,1> >
		tree_type;

	typedef
		boost::intrusive::rbtree_impl<boost::intrusive::setopt<boost::intrusive::detail::member_hook_traits<udp_match_packet,boost::intrusive::set_member_hook<boost::intrusive::none,boost::intrusive::none,boost::intrusive::none,boost::intrusive::none>,8>,udp_match_connection::comparer,u32,1> >
		tree_type;

	typedef
		sockaddr
		data_type;

	typedef
		udp_match_packet**
		iterator_type;

	// ******

} // namespace network_core
} // namespace vostok
