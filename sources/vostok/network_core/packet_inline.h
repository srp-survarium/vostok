////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[0%|PARTIAL]: empty base_packet ctor delegation; target carcass is the
// packet< udp_match_packet > instance (10B), UNPAIRED - not base-anchored here
// (udp_match_packet's private ctor lives in another matcher's file).
template < typename T >
inline packet< T >::packet( )
{
}

template < typename T >
inline void packet< T >::clear( ) { /* no source */ }

// STATE[INLINED]: no standalone symbol (inlined into resize); body needed so resize matches.
template < typename T >
inline void packet< T >::reserve( u32 size )
{
	if ( allocated_size( ) >= size )
		return;

	reallocate			( size );
}

// STATE[0%|PARTIAL]: ASSERT(!buffer_size()); reserve(size); m_buffer_size = size -
// matches the target's 5-stmt shape (ASSERT call, allocated_size() check via
// reserve, reallocate, m_buffer_size store). UNPAIRED (no base COMDAT emission of
// the tcp instance) + allocated_size()/reserve inline-vs-call on the base side.
template < typename T >
inline void packet< T >::resize( u32 size )
{
	ASSERT				( !buffer_size( ) );
	reserve				( size );
	m_buffer_size		= size;
}

// STATE[INLINED]: no standalone symbol (inlined into tcp_packet_socket::send); body
// needed so send's clone() step emits. claude@MATCH: legacy's direct friend reads
// (other.m_buffer/m_buffer_size) are DISPROVEN - the target clone row (0x123f08, 0x24B)
// lowers the two operands as out-of-line CALLS (push call-result x2 before append), which
// direct member reads can never produce; shipped clone() calls the accessors. The base
// residual (our LTCG inlines them to field reads) is the accessor inline-vs-call wall.
template < typename T >
inline void packet< T >::clone( base_packet const& other )
{
	m_buffer_size		= 0;
	append				( other.buffer( ), other.buffer_size( ) );
}

// STATE[INLINED]: append(&value,sizeof) - the scalar overloads all forward to the
// buffer/size primitive; bodies needed so call sites (serialize chains) emit.
template < typename T >
inline void packet< T >::append( bool value )
{
	append				( &value, sizeof( value ) );
}

// STATE[0%|PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), UNPAIRED - not base-anchored here.
template < typename T >
inline void packet< T >::append( u8 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( s8 value )
{
	append				( &value, sizeof( value ) );
}

// STATE[0%|PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), UNPAIRED - not base-anchored here.
template < typename T >
inline void packet< T >::append( u16 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( s16 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( u32 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( s32 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( u64 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( s64 value )
{
	append				( &value, sizeof( value ) );
}

// STATE[0%|PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), UNPAIRED - not base-anchored here.
template < typename T >
inline void packet< T >::append( float value )
{
	append				( &value, sizeof( value ) );
}

// STATE[0%|PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), UNPAIRED - not base-anchored here.
template < typename T >
inline void packet< T >::append( float2 const& value )
{
	append				( &value, sizeof( value ) );
}

// STATE[0%|PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), UNPAIRED - not base-anchored here.
template < typename T >
inline void packet< T >::append( float3 const& value )
{
	append				( &value, sizeof( value ) );
}

// STATE[UNVERIFIED]: NO source-byte evidence - no packet< T >::append(pcstr[,u8])
// instantiation exists anywhere in the target index (standalone or inline site), so
// these bodies are a legacy prior only (audit candidate 5b), byte-neutral (never
// emitted). Not INLINED (that needs consumer bytes); revisit if a consumer pairs.
template < typename T >
inline void packet< T >::append( pcstr string )
{
	append				( string, static_cast< u8 >( strlen( string ) ) );
}

template < typename T >
inline void packet< T >::append( pcstr string, u8 string_length )
{
	ASSERT				( string_length <= u8(-1) );
	append				( string_length );
	append				( static_cast< pcvoid >( string ), string_length );
}

// STATE[0%|PARTIAL]: CRTP form of network::packet::append (grow-by-doubling + memcpy);
// pairs at 0% for both instantiations (tcp 120B, udp 85B). The target instance inlines
// reallocate/allocated_size whole-program (LTCG) into one tight register-allocated body;
// the base instance keeps them as calls with spills - the divergence is LTCG cross-fn
// inlining, not the source. Serves both tcp + udp instantiations.
template < typename T >
inline void packet< T >::append( pcvoid buffer, u32 buffer_size )
{
	if ( m_buffer_size + buffer_size > allocated_size( ) ) {
		u32 new_allocated_size	= allocated_size( ) ? allocated_size( ) : buffer_size;
		while ( new_allocated_size < m_buffer_size + buffer_size )
			new_allocated_size	*= 2;

		reallocate		( new_allocated_size );
	}

	ASSERT				( m_buffer_size + buffer_size <= allocated_size( ) );
	memcpy				( this->buffer( ) + m_buffer_size, buffer, buffer_size );
	m_buffer_size		+= buffer_size;

	// STRUCTURE DIFF (tcp instance, target 0x972f0 / base 0xa2990): target 6 stmts / base 8 stmts
	// b.diff    |t.addr |b.addr |t.sz|b.sz|t.ln|b.ln|b.code
	// ----------+-------+-------+----+----+----+----+------
	// SIZE +0x11|0x97309|0xa2999|0x6 |0x17|0   |0   |if ( m_buffer_size + buffer_size > allocated_size( ) ) {
	// SIZE +0x28|0x9730f|0xa29b0|0x4 |0x2c|+1  |+1  |u32 new_allocated_size = allocated_size( ) ? allocated_size( ) : buffer_size;
	// BASE_ONLY |--     |0xa29dc|--  |0xe |--  |+2  |while ( new_allocated_size < m_buffer_size + buffer_size )
	// SIZE -0x25|0x9731d|0xa29f4|0x31|0xc |+5  |+5  |reallocate ( new_allocated_size );
	// SIZE -0x4 |0x9734e|0xa2a00|0x10|0xc |+8  |+8  |ASSERT ( m_buffer_size + buffer_size <= allocated_size( ) );
	// SIZE +0x1b|0x9735e|0xa2a0c|0x7 |0x22|+9  |+9  |memcpy ( this->buffer( ) + m_buffer_size, buffer, buffer_size );
	// BASE_ONLY |--     |0xa2a2e|--  |0xf |--  |+10 |m_buffer_size += buffer_size;
	// VERDICT: STRUCTURE MATCH (shape ok) - all 6 target line rows pair with the same source
	// lines; the 2 BASE_ONLY rows (while header, final +=) are rows the /Ox target merges into
	// neighbours (loop rotation + tail fold), not extra source statements. Non-steerable.
}

template < typename T >
inline T const& packet< T >::implementation( ) const { return *static_cast< T const* >( this ); }

template < typename T >
inline T& packet< T >::implementation( ) { return *static_cast< T* >( this ); }

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
