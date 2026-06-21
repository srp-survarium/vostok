////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CIRCULAR_BUFFER_INLINE_H_INCLUDED
#define CIRCULAR_BUFFER_INLINE_H_INCLUDED

namespace survarium {

template < typename T >
circular_buffer<T>::~circular_buffer( )
{
	while ( !empty( ) )
		pop_tail( );

	VOSTOK_FREE_IMPL( m_allocator, m_history );
}

// claude@NOTE: structure faithful to target 0x8f000 (4 stmts, 0 named locals -
// the `result` reference elides). The target keeps new_item out-of-line (a real
// call from serialize_current_state); our LTCG inlines it into its sole caller,
// so it has no standalone base COMDAT to pair. Inline-vs-call wall - not
// source-steerable from this unit.
template < typename T >
T& circular_buffer<T>::new_item( )
{
	new ( &m_history[ m_head ] ) T( );
	T& result	= m_history[ m_head ];
	m_head		= next( m_head );

	if ( m_head == m_tail )
		m_tail = next( m_tail );

	return result;
}

} // namespace survarium

#endif // #ifndef CIRCULAR_BUFFER_INLINE_H_INCLUDED
