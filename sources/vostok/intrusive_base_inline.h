////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_INTRUSIVE_BASE_INLINE_H_INCLUDED
#define VOSTOK_INTRUSIVE_BASE_INLINE_H_INCLUDED

inline vostok::intrusive_base::intrusive_base							( ) :
	m_reference_count	( 0 )
{
}

template <typename T>
inline void vostok::intrusive_base::destroy							( T* object ) const
{
	MT_DELETE			( object );
}

inline vostok::intrusive_interlocked_base::intrusive_interlocked_base	( ) :
	m_reference_count	( 0 )
{
}

template <typename T>
inline void vostok::intrusive_interlocked_base::destroy				( T* object ) const
{
	MT_DELETE			( object );
}

#endif // #ifndef VOSTOK_INTRUSIVE_BASE_INLINE_H_INCLUDED