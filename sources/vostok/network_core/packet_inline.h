////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[PARTIAL]: empty base_packet ctor delegation; target carcass is the
// packet< udp_match_packet > instance, not base-anchored here (udp_match_packet's
// private ctor lives in another matcher's file).
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

// STATE[PARTIAL]: ASSERT(!buffer_size()); reserve(size); m_buffer_size = size -
// matches the target's 5-stmt shape (ASSERT call, allocated_size() check via
// reserve, reallocate, m_buffer_size store). Capped by the address-anchored
// debug COMDAT + allocated_size()/reserve inline-vs-call on the base side.
template < typename T >
inline void packet< T >::resize( u32 size )
{
	ASSERT				( !buffer_size( ) );
	reserve				( size );
	m_buffer_size		= size;
}

template < typename T >
inline void packet< T >::clone( base_packet const& other ) { /* no source */ }

template < typename T >
inline void packet< T >::append( bool value ) { /* no source */ }

// STATE[PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), not base-anchored here.
template < typename T >
inline void packet< T >::append( u8 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( s8 value ) { /* no source */ }

// STATE[PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), not base-anchored here.
template < typename T >
inline void packet< T >::append( u16 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( s16 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( u32 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( s32 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( u64 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( s64 value ) { /* no source */ }

// STATE[PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), not base-anchored here.
template < typename T >
inline void packet< T >::append( float value )
{
	append				( &value, sizeof( value ) );
}

// STATE[PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), not base-anchored here.
template < typename T >
inline void packet< T >::append( float2 const& value )
{
	append				( &value, sizeof( value ) );
}

// STATE[PARTIAL]: append(&value,sizeof) - body exact; target carcass is the
// packet< udp_match_packet > instance (0x7d6f0-0x7d75x), not base-anchored here.
template < typename T >
inline void packet< T >::append( float3 const& value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( pcstr string ) { /* no source */ }

template < typename T >
inline void packet< T >::append( pcstr string, u8 string_length ) { /* no source */ }

// STATE[PARTIAL]: CRTP form of network::packet::append (grow-by-doubling + memcpy);
// statement shape matches target. The target instance inlines reallocate/
// allocated_size whole-program (LTCG) into one tight register-allocated body; the
// base instance keeps them as calls with spills, so it scores low - the divergence
// is LTCG cross-fn inlining, not the source. Serves both tcp + udp instantiations.
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
}

template < typename T >
inline T const& packet< T >::implementation( ) const { return *static_cast< T const* >( this ); }

template < typename T >
inline T& packet< T >::implementation( ) { return *static_cast< T* >( this ); }

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
