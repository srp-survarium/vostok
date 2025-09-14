////////////////////////////////////////////////////////////////////////////
//	Created		: 08.04.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_STRINGS_STREAM_INLINE_H_INCLUDED
#define VOSTOK_STRINGS_STREAM_INLINE_H_INCLUDED

inline vostok::strings::stream::stream				( memory::base_allocator* allocator ) :
	m_allocator		( allocator ),
	m_buffer		( 0 ),
	m_buffer_size	( 0 ),
	m_allocated_size( 0 )
{
}

inline vostok::strings::stream::~stream				( )
{
	VOSTOK_FREE_IMPL	( *m_allocator, m_buffer );
}

inline pcbyte vostok::strings::stream::get_buffer		( ) const
{
	return			m_buffer;
}

inline u32 vostok::strings::stream::get_buffer_size	( ) const
{
	return			m_buffer_size;
}

inline void vostok::strings::stream::clear			( )
{
	m_buffer_size	= 0;
}

#endif // #ifndef VOSTOK_STRINGS_STREAM_INLINE_H_INCLUDED