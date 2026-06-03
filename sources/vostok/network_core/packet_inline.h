////////////////////////////////////////////////////////////////////////////
//	Created		: 21.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_INLINE_H_INCLUDED
#define PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

template <typename T> // STATE[STUB]
inline packet<T>::packet		( )
{
}





template <typename T>
inline void packet<T>::reallocate	( u32 const new_size )
{
	m_allocated_size	= new_size;
	m_buffer_size		= std::min( m_buffer_size, m_allocated_size );
	m_buffer			= static_cast<pbyte>( VOSTOK_REALLOC_IMPL( m_allocator, m_buffer ? m_buffer - 3 : 0, new_size + 3, "packet" ) ) + 3;
}

template <typename T>
inline void packet<T>::reserve		( u32 const size )
{
	if ( m_allocated_size >= size )
		return;

	reallocate			( size );
}

template <typename T>
inline void	packet<T>::resize		( u32 const size )
{
	ASSERT				( !m_buffer_size );
	reserve				( size );
	m_buffer_size		= m_allocated_size;
}

template <typename T>
inline void	packet<T>::clone		( base_packet const& other )
{
	m_buffer_size		= 0;
	append				( other.m_buffer, other.m_buffer_size );
}

template <typename T> // STATE[STUB]
inline void packet<T>::append( u8 value )
{
	append				( &value, sizeof(value) ); // <0x8d751>|0x000|0x000:'57'
}

template <typename T>
inline void packet<T>::append( s8 value )
{
	append				( &value, sizeof(value) );
}

template <typename T> // STATE[STUB]
inline void packet<T>::append( u16 value )
{
	append				( &value, sizeof(value) ); // <0x8d731>|0x000|0x000:'69'
}

template <typename T>
inline void packet<T>::append( s16 value )
{
	append				( &value, sizeof(value) );
}

template <typename T>
inline void packet<T>::append( u32 value )
{
	append				( &value, sizeof(value) );
}

template <typename T>
inline void packet<T>::append( s32 value )
{
	append				( &value, sizeof(value) );
}

template <typename T>
inline void packet<T>::append( u64 value )
{
	append				( &value, sizeof(value) );
}

template <typename T>
inline void packet<T>::append( s64 value )
{
	append				( &value, sizeof(value) );
}

template <typename T> // STATE[STUB]
inline void packet<T>::append( float value )
{
	append				( &value, sizeof(value) ); // <0x8d711>|0x000|0x000:'105'
}

template <typename T> // STATE[STUB]
inline void packet<T>::append( float2 const& value )
{
	append				( &value, sizeof(value) ); // <0x8d701>|0x000|0x000:'111'
}

template <typename T> // STATE[STUB]
inline void packet<T>::append( float3 const& value )
{
	append				( &value, sizeof(value) ); // <0x8d6f1>|0x000|0x000:'117'
}

template <typename T> // STATE[STUB]
inline void packet<T>::append( pcvoid const buffer, u32 const buffer_size )
{
	if ( m_buffer_size + buffer_size > m_allocated_size ) {
		u32 new_allocated_size	= m_allocated_size ? m_allocated_size : buffer_size;
		while ( new_allocated_size < m_buffer_size + buffer_size )
			new_allocated_size	*= 2;

		reallocate		( new_allocated_size );
	}

	ASSERT				( m_buffer_size + buffer_size <= m_allocated_size );
	memcpy				( m_buffer + m_buffer_size, buffer, buffer_size );
	m_buffer_size		+= buffer_size;


	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7c6e5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN44") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7c6cb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN45") })
	// ******

	// FUNCTION BODY[0x8d690]: 11
	// <0x8d695>|0x000|0x000:'137'
	// <0x8d6af>|0x01a|0x01a:'138'
	// <0x8d6b5>|0x020|0x006:'139'
	// <0x8d6b9>|0x024|0x004:'140'
	// 1
	// <0x8d6c6>|0x031|0x00d:'142'
	// 1
	// 2
	// 3
	// <0x8d6cb>|0x036|0x005:'146'
	// <0x8d6db>|0x046|0x010:'147'
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef PACKET_INLINE_H_INCLUDED
