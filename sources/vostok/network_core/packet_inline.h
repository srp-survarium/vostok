// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

template < typename T >
inline packet< T >::packet( )
{
}

template < typename T >
inline void packet< T >::clear( )
{
	m_buffer_size	= 0;
}

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

template < typename T >
inline void packet< T >::clone( base_packet const& other )
{
	m_buffer_size		= 0;
	append				( other.buffer( ), other.buffer_size( ) );
}

template < typename T >
inline void packet< T >::append( const bool value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const u8 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const s8 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const u16 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const s16 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const u32 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const s32 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const u64 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const s64 value )
{
	append				( &value, sizeof( value ) );
}

template < typename T >
inline void packet< T >::append( const float value )
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
