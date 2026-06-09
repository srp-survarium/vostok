////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED

#include <vostok/network_core/packet.h>
#include <vostok/network_core/sequence_number.h>
#include <vostok/memory_single_size_buffer_allocator.h>
#include <boost/array.hpp>
#include <boost/intrusive/set_hook.hpp>

namespace vostok {
namespace network_core {

class udp_match_client_session;
class delayed_packets_predicate;
class udp_network_flow_emulator;

class udp_match_packet;

// allocator-backed factory / disposer; both reach udp_match_packet's private ctor and
// helper::call_destructor, so they are befriended below.
inline udp_match_packet* new_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
);
inline void delete_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator,
	udp_match_packet*&		packet
);

class udp_match_packet : public packet< udp_match_packet > {
public:
	class helper {
	public:
		static	inline	void	call_constructor	( udp_match_packet& packet ) { /* no source */ }
	private:
		// STATE[PARTIAL]: delete &packet - dtor inlined (hook destruct, ICF-folded name) then
		// operator delete; shape exact, residual is the single-TU anchor codegen.
		static	inline	void	call_destructor		( udp_match_packet& packet )
		{
			delete	&packet;

			// STRUCTURE DIFF[target 0xda9c0 | base 0x98240]: target 1 / base 1 stmts
			//   1: 0x004 <0x2c> | 0x006 <0x50> | delete	&packet;   SIZE
			// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
			// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the inlined udp_match_packet dtor (single-TU anchor inline-vs-call wall).
		}

		friend	void	::vostok::network_core::delete_udp_match_packet(
			memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator,
			udp_match_packet*&		packet
		);
	}; // class helper

private:
	// STATE[STUB]
	inline				udp_match_packet	( )
	{
		// FUNCTION BODY[0xea900]: 2
		// <0xea999>|0x099|+0x00b:'48'
		// <0xea9a4>|0x0a4|+0x007:'49'
		// ******
	}
public:
	inline				~udp_match_packet	( ) { /* no source */ }

	inline	u32			allocated_size		( ) const { return 0; }

	inline	pcbyte		buffer_to_send		( ) const { return NULL; }
	inline	pbyte		buffer_to_send		( ) { return NULL; }

	// STATE[STUB]
	inline	u8			header_size			( ) const
	{
		return 0;
		// FUNCTION BODY[0x8d670]
		// <0x8d670>|0x000|      :'62'	{
		// ******
	}
	inline	u32			buffer_to_send_size	( ) const { return 0; }

	inline	void		reallocate			( u32 new_size ) { /* no source */ }

	// the channel's boost::intrusive::set names &udp_match_packet::set_member_hook and
	// the connection's udp_match_packet_list names &udp_match_packet::next - both reach
	// these private members (a friend's nested types share its access).
	friend	class		udp_match_connection;
	friend	class		delayed_packets_predicate;
	friend	class		udp_network_flow_emulator;

	// the placement-new factory constructs through the private default ctor.
	friend	udp_match_packet*	::vostok::network_core::new_udp_match_packet(
		memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
	);

private:
	/* 0x0008 */	boost::intrusive::set_member_hook<>	set_member_hook;
	/* 0x0018 */	udp_match_client_session*		client_session;
	/* 0x001c */	udp_match_packet*				next;
	/* 0x0020 */	u32								last_send_time_in_ms;
	/* 0x0024 */	sequence_number< u8 >			sequence_id;
	/* 0x0026 */	sequence_number< u16 >			order_id;
	/* 0x0028 */	u8								message_type;
	/* 0x0029 */	u8								send_count;
	/* 0x002a */	u8								channel_id		: 6;
	/* 0x002a */	u8								is_reliable		: 1;
	/* 0x002a */	u8								is_ordered		: 1;
	/* 0x002b */	boost::array< u8, 256 >			m_buffer;
}; // class udp_match_packet

STATIC_SIZE_ASSERT(udp_match_packet, 0x12C);
STATIC_SIZE_ASSERT(udp_match_packet::helper, 0x1);

// STATE[PARTIAL]: allocate + placement-new into `result` local + return; shape exact.
// Residual is the single-TU anchor inlining the udp_match_packet ctor (target keeps it out-of-line).
inline udp_match_packet* new_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
)
{
	pvoid const	memory	= allocator.allocate( );
	udp_match_packet* const	result	= new ( memory ) udp_match_packet;
	return	result;

	// STRUCTURE DIFF[target 0xdaa00 | base 0x985c0]: target 3 / base 3 stmts
	//   2: 0x011 <0x2e> | 0x011 <0x71> | udp_match_packet* const	result	= new ( memory ) udp_match_packet;   SIZE
	// .. same ..
	// ; aligned 2, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - fixed quantity (split ctor/return into `result` local + return); sole SIZE is the inlined udp_match_packet ctor (single-TU anchor inline-vs-call wall).
}

// STATE[PARTIAL]: call_destructor then allocator.deallocate (nulls pointer); body exact.
// The single-TU anchor inlines this everywhere, so no standalone COMDAT emits (target
// keeps it out-of-line, called from send/enqueue/etc) - the documented inline-vs-call wall.
inline void delete_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator,
	udp_match_packet*&		packet
)
{
	udp_match_packet::helper::call_destructor	( *packet );
	allocator.deallocate	( reinterpret_cast< pvoid& >( packet ) );

	// VERDICT: STRUCTURE UNVERIFIED - no base symbol (single-TU anchor inlines this everywhere; target keeps it out-of-line, called from send/enqueue/etc - inline-vs-call wall).
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
