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

// udp_match_connection.cpp's purge/sort helpers and udp_network_flow_emulator.cpp's
// tick predicate live at GLOBAL scope (the target mangling carries no namespace) and
// reach udp_match_packet's private members.
class sequence_id_predicate;
class remove_all_predicate;
class packets_in_list_predicate;
class delayed_packets_predicate;

namespace vostok {

namespace network {
	class match_client_impl;
} // namespace network

namespace network_core {

class udp_match_client_session;
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
		friend	void	delete_udp_match_packet( memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&, udp_match_packet*& );

		// STATE[100%|DONE]
		static	inline	void	call_destructor		( udp_match_packet& packet )
		{
			packet.~udp_match_packet( );
		}

		friend	void	::vostok::network_core::delete_udp_match_packet(
			memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator,
			udp_match_packet*&		packet
		);
	}; // class helper

private:
	// STATE[65%|DONE]: structure 2/2 stmts; residual is LTCG inlining (packet/set_member_hook ctors inlined vs target's out-of-line calls)
	inline				udp_match_packet	( ) :
		next					( NULL ),
		last_send_time_in_ms	( 0xFFFFFFFF ),
		sequence_id				( 0xFFFF ),
		order_id				( 0xFFFF ),
		send_count				( 0 ),
		channel_id				( 0x3F ),
		is_reliable				( 0 ),
		is_ordered				( 0 )
	{
		base_packet::m_buffer	= m_buffer.elems + 6;
		m_buffer.elems[0]		= 0;
	}
public:
	inline				~udp_match_packet	( ) { /* no source */ }

	// STATE[INLINED]: no standalone target symbol; the LTCG append(pcvoid,u32) COMDAT
	// (0x7d690) folds this to `100h - movzx(header_size())` - payload capacity is the
	// raw array minus the wire header. (`sizeof` vs `m_buffer.size()` indistinguishable
	// in the folded emission.)
	inline	u32			allocated_size		( ) const { return sizeof( m_buffer ) - header_size( ); }

	// STATE[INLINED]: no standalone target symbol; send/handle_send inline this as
	// `packet + 0x2B` - the raw wire buffer starts at m_buffer.elems.
	inline	pcbyte		buffer_to_send		( ) const { return m_buffer.elems; }
	// STATE[INLINED]
	inline	pbyte		buffer_to_send		( ) { return m_buffer.elems; }

	// STATE[0%|PARTIAL]: source proven by the send/handle_send inline sites (and the
	// append COMDAT's `100h - movzx(header_size())` fold); base COMDAT anchored
	// (address-take), but the surviving target symbol is an optimized LTCG leaf
	// (mov eax,[ecx]; sub eax,ecx; sub eax,2Bh; ret - frameless) a /Od body cannot
	// pair against - the None score is that emission, not a source divergence.
	inline	u8			header_size			( ) const
	{
		return				(u8)( base_packet::m_buffer - m_buffer.elems );
	}
	// STATE[INLINED]: no standalone target symbol; send's inline expansion is
	// `buffer_size()-fold + header_size() call` in that operand order.
	inline	u32			buffer_to_send_size	( ) const { return buffer_size( ) + header_size( ); }

	// sushi@TODO: body unrecovered - the out-of-line packet<udp_match_packet>::reallocate
	// (0x112e20) is a never-called int3 stub, and append's grow path ends in an
	// ICF-folded no-arg call (an eater/abort macro?) the LTCG emission can't disambiguate.
	inline	void		reallocate			( u32 new_size ) { /* no source */ }

	// the channel's boost::intrusive::set names &udp_match_packet::set_member_hook and
	// the connection's udp_match_packet_list names &udp_match_packet::next - both reach
	// these private members (a friend's nested types share its access).
	friend	class		udp_match_connection;
	friend	class		::delayed_packets_predicate;
	friend	class		udp_network_flow_emulator;
	friend	class		move_to_list_predicate;
	friend	class		::sequence_id_predicate;
	friend	class		::remove_all_predicate;
	friend	class		::packets_in_list_predicate;
	// match_client_impl::clone_packet copies the wire-header fields directly
	friend	class		vostok::network::match_client_impl;
	friend	udp_match_packet*	new_udp_match_packet( memory::single_size_buffer_allocator< 300, threading::single_threading_policy >& );
	friend	void				delete_udp_match_packet( memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&, udp_match_packet*& );

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

// STATE[99%|DONE]: structure 3/3 stmts; residual is LTCG frame size (0x20 vs 0x34)
inline udp_match_packet* new_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
)
{
	udp_match_packet* const	result	= (udp_match_packet*)allocator.allocate( );
	new( result ) udp_match_packet( );
	return					result;
}

// STATE[61%|DONE]: structure 4/4 stmts; residual is LTCG inlining (deallocate inlined in target, out-of-line in base)
inline void delete_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator,
	udp_match_packet*&		packet
)
{
	udp_match_packet::helper::call_destructor( *packet );
	void*					buffer	= packet;
	allocator.deallocate	( buffer );
	packet					= NULL;
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKET_H_INCLUDED
