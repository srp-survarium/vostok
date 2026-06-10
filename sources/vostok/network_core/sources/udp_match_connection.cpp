////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/udp_match_connection.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace vostok {
namespace network_core {

// STATE[86%|DONE]: full init list matches; residual is ICF-folded call-name noise + handler_allocator stub's trailing byte init (sibling unit)
 udp_match_connection::udp_match_connection(
	boost::asio::ip::udp::socket&		socket,
	boost::asio::ip::udp::endpoint const&	remote_endpoint,
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	packets_allocator,
	udp_match_packets_orderer&			packets_orderer,
	const u32							disconnection_timeout_in_ms,
	const u32							max_packet_wait_time_in_ms,
	const u32							max_idle_time_in_ms,
	pcstr const							logging_id
) :
	m_socket			( socket ),
	m_remote_endpoint	( remote_endpoint ),
	m_packets_allocator	( packets_allocator ),
	m_packets_orderer	( packets_orderer ),
	m_logging_id		( logging_id ),
	m_last_receive_time_in_ms		( 0 ),
	m_disconnection_timeout_in_ms	( disconnection_timeout_in_ms ),
	m_last_send_time_in_ms			( 0 ),
	m_last_send_attempt_time_in_ms	( 0 ),
	m_max_packet_wait_time_in_ms	( max_packet_wait_time_in_ms ),
	m_max_idle_time_in_ms			( max_idle_time_in_ms ),
	m_disconnection_receive_time_in_ms	( 0 ),
	m_pending_operations_count		( 0 ),
	m_state				( disconnected ),
	m_remote_acknowledgement_bits		( 0 ),
	m_received_local_acknowledgement_bits	( 0 ),
	m_local_sequence_id				( 0xFFFF ),
	m_remote_sequence_id			( 0xFFFF ),
	m_received_local_sequence_id	( 0xFFFF ),
	m_disconnection_local_sequence_id	( 0xFFFF )
{
}

// STATE[PARTIAL]: auto-generated member-dtor cleanup matches; target caches `this` in esi vs base ebp-slot - /Od register-alloc residual + ICF call-name noise
 udp_match_connection::~udp_match_connection( )
{
}

// STATE[100%|DONE]
void udp_match_connection::on_error( client_error_codes_enum, boost::system::error_code )
{
}

// STATE[PARTIAL]: shape exact; the single-TU anchor inlines packet_reader::r<> where the
// target keeps the template out-of-line (the documented anchor/inline-vs-call wall).
bool udp_match_connection::is_low_level_packet( base_packet const& packet )
{
	packet_reader	reader( packet );

	reader.r< u16 >( );
	reader.r< u16 >( );
	const u16	bits	= reader.r< u16 >( );

	if ( ( bits & 1 ) == 0 )
		return false;

	reader.advance( reader.r< bool >( ) );
	return reader.eof( );

	// STRUCTURE DIFF: target 8 stmts / base 8 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|t.ln|b.ln|b.code
	// ----------+--------+--------+----+----+----+----+------
	// SIZE +0x3 |0x5464d6|0x461926|0x11|0x14|0   |0   |packet_reader reader( packet );
	// BASE_ONLY |--      |0x46193a|--  |0x13|--  |+2  |reader.r< u16 >( );
	// BASE_ONLY |--      |0x46194d|--  |0x13|--  |+3  |reader.r< u16 >( );
	// BASE_ONLY |--      |0x461960|--  |0x1b|--  |+4  |const u16 bits = reader.r< u16 >( );
	// TRGT_ONLY |0x5464e7|--      |0x14|--  |+2  |--  |--
	// TRGT_ONLY |0x5464fb|--      |0x14|--  |+3  |--  |--
	// TRGT_ONLY |0x54650f|--      |0xc |--  |+4  |--  |--
	// SIZE +0xa |0x546528|0x461988|0x14|0x1e|+8  |+9  |reader.advance( reader.r< bool >( ) );
	// SIZE +0x17|0x54653c|0x4619a6|0x8 |0x1f|+9  |+10 |return reader.eof( );
	// VERDICT: STRUCTURE MATCH (shape ok) - 8/8; the ONLY rows are the SAME three r<u16>
	// statements in the same ordinal slots (#2-#4) the aligner cannot pair because the
	// sizes flip (base inlines r<> at 0x13-0x1b, target calls the kept COMDATs at 0xc-0x14
	// with LTCG custom regs); byte overlap is near zero (report 0.0, serialized as None)
	// over a verified-identical skeleton - the documented call-boundary wall, not a shape miss.
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::handle_send(
	udp_match_packet*					packet,
	boost::system::error_code const&	error_code,
	const u32							bytes_transferred
)
{
	// LOCALS
	// const bool 						success
	// pbyte 							buffer<1>
	// ******

	// FUNCTION BODY[0x556bd0]: 30
	// <0x556be7>|0x017|+0x01b:'99'
	// <0>
	// <0x556c02>|0x032|+0x018:'101'
	// <0x556c1a>|0x04a|+0x00c:'102'
	// <0>
	// <0x556c26>|0x056|+0x012:'104'
	// <0x556c38>|0x068|+0x019:'105'
	// <0>
	// <0x556c51>|0x081|+0x005:'107'
	// <0x556c56>|0x086|+0x06c:'108'
	// <0x556cc2>|0x0f2|+0x019:'109'
	// <0>
	// <0x556cdb>|0x10b|+0x002:'111'
	// <0x556cdd>|0x10d|+0x009|[1]:'112'
	// <0x556ce6>|0x116|+0x023:'113'
	// <0x556d09>|0x139|+0x017:'114'
	// <0>
	// <1>
	// <2>
	// <0x556d20>|0x150|+0x015:'118'
	// <0x556d35>|0x165|+0x0c9:'119'
	// <0x556dfe>|0x22e|+0x017:'120'
	// <0x556e15>|0x245|+0x005:'121'
	// <0>
	// <1>
	// <0x556e1a>|0x24a|+0x00a:'124'
	// <0x556e24>|0x254|+0x077:'125'
	// <0x556e9b>|0x2cb|+0x017:'126'
	// <0>
	// <1>
	// ******
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::send( udp_match_packet* const packet )
{
	// FUNCTION BODY[0x556fd0]: 19
	// <0x556fe0>|0x010|+0x013:'133'
	// <0x556ff3>|0x023|+0x02f:'134'
	// <0x557022>|0x052|+0x033:'135'
	// <0>
	// <0x557055>|0x085|+0x01b:'137'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x557070>|0x0a0|+0x104:'151'
	// ******
}

// STATE[INPROGRESS]: faithful body written (serialize header into packet.m_buffer: 3x
// sequence_number<u16>::serialize(pbyte&)-style writes). NOT YET SCORED - this private
// method is unreferenced (its callers send/new_low_level_packet are still carcasses), so
// no COMDAT emits; next step: implement a caller (or anchor it via a friend) to verify.
void udp_match_connection::fill_packet_header( udp_match_packet& packet )
{
	pbyte	buffer	= packet.m_buffer.data( );

	const udp_match_packets_count_enum	packet_type	= udp_match_packets_count_enum( *buffer );
	ASSERT( UNKNOWN_EXPRESSION_T( packet_type < 2 ) );

	reinterpret_cast< sequence_number< u16 >& >( packet.sequence_id ).serialize( buffer );
	m_remote_sequence_id.serialize( buffer );

	*reinterpret_cast< u16* >( buffer )	= u16( ( u32( m_remote_acknowledgement_bits ) << 1 ) | ( packet_type == udp_match_low_level_packet ) );
	buffer	+= 2;

	// VERDICT: STRUCTURE UNVERIFIED - no base symbol (private + unreferenced; callers send/new_low_level_packet are still carcasses, so no COMDAT emits). Next: anchor via a caller/friend to score.
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::send_packets_list( udp_match_packet* const packets_list, const u32 packets_count )
{
	// LOCALS
	// udp_match_packet* const 			packet_to_send
	// buffer_vector< udp_match_packet* > packets
	// udp_match_packet* 				i<1>
	// udp_match_packet** 				e<2>
	// udp_match_packet** 				i<2>
	// ******


	// FUNCTION BODY[0x557180]: 66
	// <0x55718d>|0x00d|+0x00f:'170'
	// <0>
	// <0x55719c>|0x01c|+0x00d:'172'
	// <0x5571a9>|0x029|+0x00c:'173'
	// <0x5571b5>|0x035|+0x00c:'174'
	// <0>
	// <0x5571c1>|0x041|+0x014:'176'
	// <0>
	// <0x5571d5>|0x055|+0x00c:'178'
	// <0x5571e1>|0x061|+0x00f:'179'
	// <0x5571f0>|0x070|+0x02d:'180'
	// <0>
	// <0x55721d>|0x09d|+0x00f:'182'
	// <0x55722c>|0x0ac|+0x014:'183'
	// <0x557240>|0x0c0|+0x014:'184'
	// <0>
	// <1>
	// <0x557254>|0x0d4|+0x014:'187'
	// <0>
	// <0x557268>|0x0e8|+0x00c:'189'
	// <0x557274>|0x0f4|+0x005:'190'
	// <0>
	// <1>
	// <2>
	// <0x557279>|0x0f9|+0x00c:'194'
	// <0x557285>|0x105|+0x015:'195'
	// <0x55729a>|0x11a|+0x00f:'196'
	// <0x5572a9>|0x129|+0x016:'197'
	// <0x5572bf>|0x13f|+0x007:'198'
	// <0x5572c6>|0x146|+0x00c:'199'
	// <0>
	// <1>
	// <0x5572d2>|0x152|+0x036:'202'
	// <0>
	// <0x557308>|0x188|+0x01b|[1]:'204'
	// <0x557323>|0x1a3|+0x00c:'205'
	// <0>
	// <1>
	// <0x55732f>|0x1af|+0x014:'208'
	// <0>
	// <0x557343>|0x1c3|+0x00c:'210'
	// <0x55734f>|0x1cf|+0x00f:'211'
	// <0x55735e>|0x1de|+0x018:'212'
	// <0>
	// <0x557376>|0x1f6|+0x00f:'214'
	// <0x557385>|0x205|+0x018:'215'
	// <0x55739d>|0x21d|+0x014:'216'
	// <0>
	// <1>
	// <2>
	// <0x5573b1>|0x231|+0x00c:'220'
	// <0x5573bd>|0x23d|+0x011:'221'
	// <0x5573ce>|0x24e|+0x01f:'222'
	// <0x5573ed>|0x26d|+0x005:'223'
	// <0>
	// <0x5573f2>|0x272|+0x014:'225'
	// <0>
	// <0x557406>|0x286|+0x00c:'227'
	// <0>
	// <0x557412>|0x292|+0x01f|[2]:'229'
	// <0x557431>|0x2b1|+0x015:'230'
	// <0x557446>|0x2c6|+0x016:'231'
	// <0>
	// <0x55745c>|0x2dc|+0x002:'233'
	// <0x55745e>|0x2de|+0x016:'234'
	// <0x557474>|0x2f4|+0x002:'235'
	// ******
}

// STATE[INPROGRESS]: single guarded debug-dump statement (bool guard call + 2-arg log call),
// both fold to empty_stub; exact logging helper API (sibling unit) unresolved.
void udp_match_connection::dump( pcstr const caption, const u32 current_time_in_ms )
{
	VOSTOK_UNREFERENCED_PARAMETERS( caption, current_time_in_ms );

	// VERDICT: STRUCTURE UNVERIFIED - no base symbol (current empty body folds to empty_stub via ICF; target's guarded debug-dump stmts unresolved - sibling logging unit API).
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
udp_match_packet* udp_match_connection::new_low_level_packet( const u8 message_type )
{
	return NULL;

	// LOCALS
	// udp_match_packet* const 			packet
	// pcstr 							message_id_string
	// ******

	return NULL;

	// FUNCTION BODY[0x556ec0]: 35
	// <0x556eca>|0x00a|+0x015:'295'
	// <0x556edf>|0x01f|+0x007:'296'
	// <0>
	// <0x556ee6>|0x026|+0x00a:'298'
	// <0>
	// <0x556ef0>|0x030|+0x00d:'300'
	// <0>
	// <1>
	// <0x556efd>|0x03d|+0x01a:'303'
	// <0>
	// <0x556f17>|0x057|+0x007:'305'
	// <0x556f1e>|0x05e|+0x002:'306'
	// <0>
	// <1>
	// <0x556f20>|0x060|+0x007:'309'
	// <0x556f27>|0x067|+0x002:'310'
	// <0>
	// <1>
	// <0x556f29>|0x069|+0x007:'313'
	// <0x556f30>|0x070|+0x002:'314'
	// <0>
	// <1>
	// <0x556f32>|0x072|+0x007:'317'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x556f39>|0x079|+0x00f:'322'
	// <0x556f48>|0x088|+0x02d:'323'
	// <0x556f75>|0x0b5|+0x00f:'324'
	// <0x556f84>|0x0c4|+0x029:'325'
	// <0x556fad>|0x0ed|+0x00f:'326'
	// <0>
	// <1>
	// <0x556fbc>|0x0fc|+0x003:'329'
	// ******
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::send_queued_packets( const u32 current_time_in_ms )
{
	// LOCALS
	// buffer_vector< udp_match_packet* > packets
	// u32 								packets_count
	// udp_match_packet* const 			packet<1>
	// udp_match_packet* const 			packet<1>
	// u32 								size_left<1>
	// udp_match_packet* 				packet_list_tail<1>
	// sequence_number< u16 > 			test<1>
	// u32 								packets_count<1>
	// udp_match_packet** 				e<2>
	// udp_match_packet** 				i<2>
	// std::reverse_iterator< udp_match_packet** > e<2>
	// std::reverse_iterator< udp_match_packet** > i<2>
	// ******

	// STATICS
	// static < NoType > 				 = <0x557c70>;
	// ******


	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5464f3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN36") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x546536 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN34") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x546571 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN29") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5465c3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN27") })
	// ******

	// FUNCTION BODY[0x5574b0]: 120
	// <0>
	// <1>
	// <0x5574c0>|0x010|+0x014:'336'
	// <0>
	// <0x5574d4>|0x024|+0x01f:'338'
	// <0>
	// <0x5574f3>|0x043|+0x02c:'340'
	// <0x55751f>|0x06f|+0x00d:'341'
	// <0x55752c>|0x07c|+0x005:'342'
	// <0>
	// <0x557531>|0x081|+0x005:'344'
	// <0>
	// <1>
	// <0x557536>|0x086|+0x00c:'347'
	// <0x557542>|0x092|+0x00c:'348'
	// <0>
	// <0x55754e>|0x09e|+0x021:'350'
	// <0x55756f>|0x0bf|+0x002:'351'
	// <0>
	// <1>
	// <0x557571>|0x0c1|+0x01d:'354'
	// <0>
	// <0x55758e>|0x0de|+0x00d:'356'
	// <0x55759b>|0x0eb|+0x005:'357'
	// <0>
	// <1>
	// <0x5575a0>|0x0f0|+0x021:'360'
	// <0x5575c1>|0x111|+0x002:'361'
	// <0>
	// <1>
	// <0x5575c3>|0x113|+0x00c:'364'
	// <0x5575cf>|0x11f|+0x002:'365'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5575d1>|0x121|+0x064:'370'
	// <0>
	// <0x557635>|0x185|+0x013:'372'
	// <0x557648>|0x198|+0x006:'373'
	// <0x55764e>|0x19e|+0x01d:'374'
	// <0>
	// <0x55766b>|0x1bb|+0x005:'376'
	// <0>
	// <1>
	// <0x557670>|0x1c0|+0x021:'379'
	// <0x557691>|0x1e1|+0x007:'380'
	// <0>
	// <1>
	// <2>
	// <0x557698>|0x1e8|+0x014:'384'
	// <0>
	// <0x5576ac>|0x1fc|+0x04b:'386'
	// <0x5576f7>|0x247|+0x019:'387'
	// <0x557710>|0x260|+0x014|[1]:'388'
	// <0x557724>|0x274|+0x00a:'389'
	// <0>
	// <1>
	// <0x55772e>|0x27e|+0x022:'392'
	// <0x557750>|0x2a0|+0x02b:'393'
	// <0x55777b>|0x2cb|+0x005:'394'
	// <0>
	// <0x557780>|0x2d0|+0x026:'396'
	// <0>
	// <0x5577a6>|0x2f6|+0x016:'398'
	// <0x5577bc>|0x30c|+0x011|[1]:'399'
	// <0x5577cd>|0x31d|+0x02c:'400'
	// <0>
	// <0x5577f9>|0x349|+0x01f|[2]:'402'
	// <0x557818>|0x368|+0x014:'403'
	// <0x55782c>|0x37c|+0x019:'404'
	// <0x557845>|0x395|+0x002:'405'
	// <0x557847>|0x397|+0x046:'406'
	// <0x55788d>|0x3dd|+0x005:'407'
	// <0>
	// <0x557892>|0x3e2|+0x005:'409'
	// <0>
	// <1>
	// <0x557897>|0x3e7|+0x01b:'412'
	// <0x5578b2>|0x402|+0x01b:'413'
	// <0x5578cd>|0x41d|+0x006:'414'
	// <0>
	// <0x5578d3>|0x423|+0x009:'416'
	// <0x5578dc>|0x42c|+0x00c:'417'
	// <0x5578e8>|0x438|+0x045:'418'
	// <0x55792d>|0x47d|+0x00c:'419'
	// <0x557939>|0x489|+0x00f:'420'
	// <0x557948>|0x498|+0x007:'421'
	// <0>
	// <0x55794f>|0x49f|+0x00c:'423'
	// <0x55795b>|0x4ab|+0x022:'424'
	// <0x55797d>|0x4cd|+0x019:'425'
	// <0x557996>|0x4e6|+0x04f|[2]:'426'
	// <0x5579e5>|0x535|+0x05a:'427'
	// <0x557a3f>|0x58f|+0x030:'428'
	// <0x557a6f>|0x5bf|+0x026:'429'
	// <0x557a95>|0x5e5|+0x023:'430'
	// <0x557ab8>|0x608|+0x027:'431'
	// <0x557adf>|0x62f|+0x026:'432'
	// <0x557b05>|0x655|+0x039:'433'
	// <0x557b3e>|0x68e|+0x03c:'434'
	// <0x557b7a>|0x6ca|+0x009:'435'
	// <0>
	// <0x557b83>|0x6d3|+0x005:'437'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x557b88>|0x6d8|+0x070:'446'
	// <0>
	// <1>
	// <0x557bf8>|0x748|+0x00f:'449'
	// <0x557c07>|0x757|+0x013:'450'
	// <0x557c1a>|0x76a|+0x005:'451'
	// <0>
	// <0x557c1f>|0x76f|+0x014:'453'
	// ******
}

// STATE[DONE]: bytes match (only diff is the ICF-folded empty_stub representative name objdiff prints)
void udp_match_connection::connect( udp_match_packet* const packet )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_state == disconnected ) );

	m_state	= connected;

	if ( packet )
		enqueue_impl( packet );

	// STRUCTURE DIFF[target 0x545d30 | base 0x44e8d0]: target 4 / base 4 stmts
	// .. same ..
	// ; aligned 4, size-diffs 0, quantity-diffs 0, blank-gaps 2
	// VERDICT: STRUCTURE MATCH - all 4 statements aligned, no size/quantity divergence.
}

// STATE[PARTIAL]: is_ordered stamps the packet's order_id from the channel's running
// sent_order_id (post-increment), then the is_reliable / push_back tail. Shape matches
// target; the sequence_number post-increment temp accounts for the residual.
void udp_match_connection::enqueue_impl( udp_match_packet* packet )
{
	if ( packet->is_ordered )
	{
		channel&	channel	= m_channels[ packet->channel_id ];
		packet->order_id	= channel.sent_order_id++;
	}

	if ( packet->is_reliable )
		++m_stats.unacknowledged_packets;

	m_packets_to_send.push_back( packet );

	// STRUCTURE DIFF[target 0x545c60 | base 0x44e820]: target 9 / base 6 stmts
	//   3: 0x038 <0x15> | 0x038 <0x36> | packet->order_id	= channel.sent_order_id++;   SIZE
	//   4: 0x04d <0x11> | --          | L469   ONLY target
	//   5: 0x05e <0x21> | --          | L470   ONLY target
	//   6: 0x07f <0x10> | --          | L471   ONLY target
	// .. same ..
	// ; aligned 5, size-diffs 1, quantity-diffs 3, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - the base inlines sequence_number<u16>::operator++(s32) (post-increment copy+inc+assign) as one statement where the target's /Od keeps the operator's steps as separate statements (L469-471); same op set, inline-vs-call/template line-attribution wall, not caller-steerable.
}

// STATE[PARTIAL]: 4/5 statements aligned; the else-branch inlines delete_udp_match_packet
// (call_destructor+deallocate+decrement) where the target emits a single out-of-line call -
// the single-TU anchor cannot keep the inline free function standalone (inline-vs-call wall).
void udp_match_connection::enqueue( udp_match_packet* packet )
{
	if ( m_state == connected )
		enqueue_impl( packet );
	else
	{
		ASSERT( UNKNOWN_EXPRESSION_T( m_state != connected ) );
		delete_udp_match_packet( m_packets_allocator, packet );
	}

	// STRUCTURE DIFF[target 0x545e20 | base 0x44ea70]: target 5 / base 5 stmts
	//   5: 0x02f <0x16> | 0x02f <0x44> | delete_udp_match_packet( m_packets_allocator, packet );   SIZE
	// ; aligned 4, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the base inlining delete_udp_match_packet (call_destructor+deallocate) where the target emits one out-of-line call (single-TU anchor inline-vs-call wall).
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::update_acknowledgements(
	sequence_number< u16 >		remote_sequence_id,
	sequence_number< u16 >		local_sequence_id,
	const u16					local_acknowledgement_bits
)
{
	// LOCALS
	// const u32 						local_sequence_difference
	// const u32 						remote_sequence_difference
	// u16 								acknowledgement_bits
	// const u16 						last_local_acknowledgement_bits
	// const u32 						difference<1>
	// const u32 						unacknowledged_packets_size<2>
	// sequence_number< u16 > 			sequence_id<1>
	// const u32 						unacknowledged_packets_size<2>
	// ******

	// FUNCTION BODY[0x555e70]: 50
	// <0x555e7f>|0x00f|+0x00c:'521'
	// <0x555e8b>|0x01b|+0x01c:'522'
	// <0x555ea7>|0x037|+0x03e:'523'
	// <0x555ee5>|0x075|+0x01f:'524'
	// <0x555f04>|0x094|+0x019:'525'
	// <0>
	// <0x555f1d>|0x0ad|+0x01c:'527'
	// <0>
	// <0x555f39>|0x0c9|+0x005:'529'
	// <0>
	// <1>
	// <0x555f3e>|0x0ce|+0x00c:'532'
	// <0x555f4a>|0x0da|+0x020:'533'
	// <0x555f6a>|0x0fa|+0x01c|[1]:'534'
	// <0x555f86>|0x116|+0x00a:'535'
	// <0x555f90>|0x120|+0x006|[2]:'536'
	// <0x555f96>|0x126|+0x02b:'537'
	// <0>
	// <1>
	// <2>
	// <0x555fc1>|0x151|+0x013:'541'
	// <0x555fd4>|0x164|+0x07b:'542'
	// <0x55604f>|0x1df|+0x029:'543'
	// <0>
	// <1>
	// <0x556078>|0x208|+0x005:'546'
	// <0>
	// <1>
	// <0x55607d>|0x20d|+0x01c:'549'
	// <0x556099>|0x229|+0x035:'550'
	// <0x5560ce>|0x25e|+0x012:'551'
	// <0>
	// <0x5560e0>|0x270|+0x005:'553'
	// <0>
	// <1>
	// <0x5560e5>|0x275|+0x034:'556'
	// <0x556119>|0x2a9|+0x011:'557'
	// <0>
	// <0x55612a>|0x2ba|+0x00c:'559'
	// <0x556136>|0x2c6|+0x01e:'560'
	// <0x556154>|0x2e4|+0x00c:'561'
	// <0>
	// <0x556160>|0x2f0|+0x017:'563'
	// <0x556177>|0x307|+0x02d|[1]:'564'
	// <0x5561a4>|0x334|+0x010:'565'
	// <0x5561b4>|0x344|+0x013|[2]:'566'
	// <0x5561c7>|0x357|+0x099:'567'
	// <0x556260>|0x3f0|+0x029:'568'
	// <0x556289>|0x419|+0x005:'569'
	// <0>
	// ******
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::process_low_level_message( packet_reader& reader, const u32 time_in_ms )
{
	// LOCALS
	// udp_match_connection::low_level_message_type_enum message_type<1>
	// ******

	// FUNCTION BODY[0x556ab0]: 43
	// <0x556ac1>|0x011|+0x02a|[1]:'575'
	// <0>
	// <1>
	// <0x556aeb>|0x03b|+0x00c:'578'
	// <0>
	// <0x556af7>|0x047|+0x005:'580'
	// <0>
	// <1>
	// <2>
	// <0x556afc>|0x04c|+0x00d:'584'
	// <0x556b09>|0x059|+0x00c:'585'
	// <0x556b15>|0x065|+0x005:'586'
	// <0>
	// <1>
	// <2>
	// <0x556b1a>|0x06a|+0x00c:'590'
	// <0x556b26>|0x076|+0x07a:'591'
	// <0x556ba0>|0x0f0|+0x002:'592'
	// <0>
	// <1>
	// <0x556ba2>|0x0f2|+0x00c:'595'
	// <0x556bae>|0x0fe|+0x00c:'596'
	// <0>
	// <0x556bba>|0x10a|+0x00a:'598'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// ******
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::instant_disconnect( disconnect_event_types_enum type )
{
	// LOCALS
	// u32 								i<1>
	// u32 								n<1>
	// udp_match_connection::channel& 	channel<2>
	// udp_match_packet* 				packet<3>
	// ******

	// FUNCTION BODY[0x5565a0]: 31
	// <0x5565af>|0x00f|+0x010:'649'
	// <0>
	// <0x5565bf>|0x01f|+0x010:'651'
	// <0x5565cf>|0x02f|+0x010:'652'
	// <0x5565df>|0x03f|+0x010:'653'
	// <0x5565ef>|0x04f|+0x010:'654'
	// <0x5565ff>|0x05f|+0x00f:'655'
	// <0x55660e>|0x06e|+0x00f:'656'
	// <0x55661d>|0x07d|+0x010:'657'
	// <0x55662d>|0x08d|+0x022:'658'
	// <0x55664f>|0x0af|+0x021:'659'
	// <0x556670>|0x0d0|+0x022:'660'
	// <0x556692>|0x0f2|+0x022:'661'
	// <0>
	// <0x5566b4>|0x114|+0x058:'663'
	// <0x55670c>|0x16c|+0x058:'664'
	// <0>
	// <0x556764>|0x1c4|+0x025|[1]:'666'
	// <0x556789>|0x1e9|+0x016|[2]:'667'
	// <0x55679f>|0x1ff|+0x019:'668'
	// <0x5567b8>|0x218|+0x02d|[3]:'669'
	// <0x5567e5>|0x245|+0x021:'670'
	// <0x556806>|0x266|+0x019:'671'
	// <0x55681f>|0x27f|+0x005:'672'
	// <0>
	// <0x556824>|0x284|+0x008:'674'
	// <0x55682c>|0x28c|+0x005:'675'
	// <0>
	// <1>
	// <0x556831>|0x291|+0x022:'678'
	// <0x556853>|0x2b3|+0x015:'679'
	// ******
}

// STATE[INPROGRESS]: still blocked on remaining empty STUB leaves in udp_match_packet.h
// (header_size, buffer_to_send/_size) + udp_match_stats; and even once those are real the
// single-TU anchor inlines the now-real inline helpers where the target keeps them
// out-of-line (the documented inline-vs-call wall). Next: implement those leaves, then diff.
void udp_match_connection::disconnect( )
{
	// LOCALS
	// sequence_number< u16 > 			test
	// u32 								i<1>
	// u32 								n<1>
	// udp_match_connection::channel& 	channel<2>
	// udp_match_packet* 				packet<3>
	// ******

	// FUNCTION BODY[0x556870]: 24
	// <0x55687f>|0x00f|+0x00c:'684'
	// <0>
	// <0x55688b>|0x01b|+0x010:'686'
	// <0>
	// <0x55689b>|0x02b|+0x011:'688'
	// <0x5568ac>|0x03c|+0x028:'689'
	// <0x5568d4>|0x064|+0x00d:'690'
	// <0x5568e1>|0x071|+0x005:'691'
	// <0>
	// <1>
	// <0x5568e6>|0x076|+0x058:'694'
	// <0x55693e>|0x0ce|+0x058:'695'
	// <0>
	// <0x556996>|0x126|+0x025|[1]:'697'
	// <0x5569bb>|0x14b|+0x016|[2]:'698'
	// <0x5569d1>|0x161|+0x019:'699'
	// <0x5569ea>|0x17a|+0x027|[3]:'700'
	// <0x556a11>|0x1a1|+0x021:'701'
	// <0x556a32>|0x1c2|+0x019:'702'
	// <0x556a4b>|0x1db|+0x002:'703'
	// <0x556a4d>|0x1dd|+0x005:'704'
	// <0>
	// <0x556a52>|0x1e2|+0x028:'706'
	// <0x556a7a>|0x20a|+0x028:'707'
	// ******
}

// STATE[PARTIAL]: control structure + ops match; base frame is 0x18 larger (intrusive set size() temp) + ICF call-name noise
u32 udp_match_connection::packets_count( ) const
{
	u32 result	= m_packets_to_send.size( ) + m_outgoing_packets.size( ) + m_unacknowledged_packets.size( );

	for ( u32 i = 0, n = m_channels.size( ) ; i < n ; ++i )
		result	+= m_channels[ i ].packets.size( );

	return result;

	// STRUCTURE DIFF[target 0x545d70 | base 0x44e9c0]: target 4 / base 4 stmts
	// .. same ..
	// ; aligned 4, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - all 4 statements aligned; 99.79% residual is the intrusive set size() temp frame + ICF call-name noise.
}


} // namespace network_core
} // namespace vostok
