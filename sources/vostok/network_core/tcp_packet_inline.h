////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

inline tcp_packet::tcp_packet( memory::base_allocator& allocator ) :
	m_allocator			( allocator ),
	m_allocated_size	( 0 )
{
	/* no source */
}

// STATE[INPROGRESS]: target dtor (0x972b0) is fully optimized/inlined (1/45 instr
// equal, 2.2%); our /Od out-of-line body diverges in shape - target lacks the
// separate VOSTOK_FREE_IMPL call statement (inlined). LTCG/opt mismatch, not a
// source-shape fix from our side.
inline tcp_packet::~tcp_packet( )
{
	if ( buffer( ) )
	{
		pbyte real_buffer	= buffer( ) - 3;
		VOSTOK_FREE_IMPL	( m_allocator, real_buffer );
	}

	// STRUCTURE DIFF[target 0x972b0 | base 0x7fd80]: target 2 / base 3 stmts
	//   1: 0x006 <0x3> | 0x009 <0xe> | if ( buffer( ) )   SIZE
	//   2: 0x009 <0xd> | 0x017 <0x11> | pbyte real_buffer	= buffer( ) - 3;   SIZE
	//   3: --          | 0x028 <0x2f> | VOSTOK_FREE_IMPL	( m_allocator, real_buffer );   ONLY base
	// ; aligned 0, size-diffs 2, quantity-diffs 1, blank-gaps 1
	// VERDICT: STRUCTURE MISMATCH (both) - target dtor is optimized/inlined (free folded in, no standalone call stmt); base is /Od out-of-line. Not source-fixable; LTCG/opt divergence.
}

inline u32 tcp_packet::allocated_size( ) const
{
	return m_allocated_size;
}

// STATE[0%|PARTIAL]: mirrors network::packet::reallocate (m_allocated_size store,
// std::min buffer_size, realloc(m_buffer?-3:0, new_size+3)+3). UNPAIRED: our base keeps
// this callee standalone (?reallocate@tcp_packet@..QAEXI@Z, rva 0x914F0, 125 bytes,
// this unit) while the target inlined it INTO packet< tcp_packet >::reallocate (the
// 56-byte AAE COMDAT at rva 0x97400, packet.h unit) and kept no tcp_packet-level symbol
// here - an LTCG inline-direction flip objdiff cannot pair, not steerable from source.
inline void tcp_packet::reallocate( u32 new_size )
{
	m_allocated_size	= new_size;
	m_buffer_size		= std::min( m_buffer_size, m_allocated_size );
	m_buffer			= static_cast< pbyte >( VOSTOK_REALLOC_IMPL( m_allocator, m_buffer ? m_buffer - 3 : 0, new_size + 3, "packet" ) ) + 3;
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_TCP_PACKET_INLINE_H_INCLUDED
