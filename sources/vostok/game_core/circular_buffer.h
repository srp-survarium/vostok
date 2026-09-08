// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED

namespace survarium {

template < typename T >
class circular_buffer : private boost::noncopyable {
public:
	inline				circular_buffer		( memory::base_allocator& allocator, const u32 max_count ) :
		m_history	( VOSTOK_ALLOC_IMPL( allocator, T, max_count ) ),
		m_allocator	( allocator ),
		m_max_count	( max_count ),
		m_head		( 0 ),
		m_tail		( 0 )
	{
	}
	inline				~circular_buffer	( );

public:
	// sushi@TODO: recover resize/insert preservation rules and whether is_valid means allocated or live storage.
	inline	void		resize				( const u32 arg_0 ) { /* no source */ }
	inline	u32			size				( ) const { return ( m_head + m_max_count - m_tail ) % m_max_count; }

	inline	T&			new_item			( );
	inline	T&			insert				( const u32 arg_0 ) { /* no source */ }
	inline	void		pop_tail			( ) { m_tail = next( m_tail ); }

	inline	T&			oldest				( ) { return m_history[ m_tail ]; }
	inline	T const&	oldest				( ) const { return m_history[ m_tail ]; }

	inline	T&			newest				( ) { return m_history[ previous( m_head ) ]; }
	inline	T const&	newest				( ) const { return m_history[ previous( m_head ) ]; }

	inline	bool		empty				( ) const { return m_head == m_tail; }
	inline	bool		full				( ) const { return next( m_head ) == m_tail; }

	inline	u32			next				( u32 arg_0 ) const { return ( arg_0 + 1 ) % m_max_count; }
	inline	u32			previous			( u32 arg_0 ) const { return ( arg_0 + m_max_count - 1 ) % m_max_count; }

	inline	T&			operator[]			( const u32 arg_0 ) { return m_history[ arg_0 ]; }
	inline	T const&	operator[]			( const u32 arg_0 ) const { return m_history[ arg_0 ]; }

	inline	u32			head				( ) const { return m_head; }
	inline	u32			tail				( ) const { return m_tail; }
	inline	u32			max_count			( ) const { return m_max_count; }
	inline	u32			index				( T const& item ) const { return u32( &item - m_history ); }
	inline	bool		is_valid			( T const& arg_0 ) const { /* no source */ }


private:
	/* 0x0000 */	T*							m_history;
	/* 0x0004 */	memory::base_allocator&		m_allocator;
	/* 0x0008 */	u32							m_max_count;
	/* 0x000c */	u32							m_head;
	/* 0x0010 */	u32							m_tail;
}; // class circular_buffer<T>

} // namespace survarium

#include <vostok/game_core/circular_buffer_inline.h>

#endif // #ifndef CIRCULAR_BUFFER_H_INCLUDED
