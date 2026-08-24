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
	private:
		// STATE[REMOVED]: never referenced - the factory new_udp_match_packet uses placement
		// new directly, not call_constructor. Uninstantiated both sides.
		static	inline	void	call_constructor	( udp_match_packet& packet ) { /* no source */ } // STATE[REMOVED]
	private:
		friend	void	delete_udp_match_packet( memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&, udp_match_packet*& );

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
private:
	// trivial dtor (POD/aggregate members); the empty body is correct. Referenced via
	// helper::call_destructor in delete_udp_match_packet but never emitted standalone
	// (inlined into the destroying scope - no ??1 target symbol).
	inline				~udp_match_packet	( ) { /* no source */ }

public:
	inline	u32			allocated_size		( ) const { return sizeof( m_buffer ) - header_size( ); }

	inline	pcbyte		buffer_to_send		( ) const { return m_buffer.elems; }
	inline	pbyte		buffer_to_send		( ) { return m_buffer.elems; }

	inline	u8			header_size			( ) const
	{
		return				(u8)( base_packet::m_buffer - m_buffer.elems );
	}
	inline	u32			buffer_to_send_size	( ) const { return buffer_size( ) + header_size( ); }

	inline	void		reallocate			( u32 new_size ) { UNREACHABLE_CODE( new_size ); }

	// the placement-new factory constructs through the private default ctor.
	friend	class		udp_match_connection;
	friend	class		udp_network_flow_emulator;
	friend	class		vostok::network::match_client_impl;
	friend	udp_match_packet*	::vostok::network_core::new_udp_match_packet(
		memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
	);

public:
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

private:
	/* 0x002b */	boost::array< u8, 256 >			m_buffer;
}; // class udp_match_packet

STATIC_SIZE_ASSERT(udp_match_packet, 0x12C);
STATIC_SIZE_ASSERT(udp_match_packet::helper, 0x1);

inline udp_match_packet* new_udp_match_packet(
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >&	allocator
)
{
	udp_match_packet* const	result	= (udp_match_packet*)allocator.allocate( );
	new( result ) udp_match_packet( );
	return					result;
}

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
