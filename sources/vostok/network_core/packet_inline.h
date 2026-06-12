////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

template < typename T >
inline packet< T >::packet( )
{
}

template < typename T >
inline void packet< T >::clear( ) { /* no source */ }

template < typename T >
inline void packet< T >::reserve( u32 size )
{
	if ( allocated_size( ) >= size )
		return;

	reallocate			( size );
}

template < typename T >
inline void packet< T >::resize( u32 size )
{
	ASSERT				( !buffer_size( ) );
	reserve				( size );
	m_buffer_size		= size;
}

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

template < typename T >
inline void packet< T >::append( bool value )
{
	append				( &value, sizeof( value ) );
}

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

template < typename T >
inline void packet< T >::append( float value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( float2 const& value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( float3 const& value )
{
	append				( &value, sizeof( value ) );
}

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
