// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED

namespace survarium {

// pdb-parser printed the monomorphised name `circular_buffer<T>`; invalid as the
// primary-template declaration (C2988). First real includer is
// game/sources/player.h (m_history member).
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
	inline	void		resize				( const u32 arg_0 ) { /* no source */ }
	inline	u32			size				( ) const { /* no source */ }

	inline	T&			new_item			( );
	inline	T&			insert				( const u32 arg_0 ) { /* no source */ }
	inline	void		pop_tail			( ) { /* no source */ m_tail = next( m_tail ); }

	inline	T&			oldest				( ) { /* no source */ return m_history[ m_tail ]; }
	inline	T const&	oldest				( ) const { /* no source */ return m_history[ m_tail ]; }

	inline	T&			newest				( ) { /* no source */ return m_history[ previous( m_head ) ]; }
	inline	T const&	newest				( ) const { /* no source */ return m_history[ previous( m_head ) ]; }

	inline	bool		empty				( ) const { /* no source */ return m_head == m_tail; }
	inline	bool		full				( ) const { /* no source */ }

	inline	u32			next				( u32 arg_0 ) const { /* no source */ return ( arg_0 + 1 ) % m_max_count; }
	inline	u32			previous			( u32 arg_0 ) const { /* no source */ return ( arg_0 + m_max_count - 1 ) % m_max_count; }

	inline	T&			operator[]			( const u32 arg_0 ) { /* no source */ return m_history[ arg_0 ]; }
	inline	T const&	operator[]			( const u32 arg_0 ) const { /* no source */ return m_history[ arg_0 ]; }

	inline	u32			head				( ) const { /* no source */ return m_head; }
	inline	u32			tail				( ) const { /* no source */ return m_tail; }
	inline	u32			max_count			( ) const { /* no source */ return m_max_count; }
	inline	u32			index				( T const& arg_0 ) const { /* no source */ }
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
