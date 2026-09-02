// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_CUSTOM_ALLOC_HANDLER_H_INCLUDED
#define NETWORK_CORE_CUSTOM_ALLOC_HANDLER_H_INCLUDED

#include <vostok/network_core/handler_allocator.h>

namespace vostok {
namespace network_core {

template < typename Handler >
class custom_alloc_handler {
public:
	inline			custom_alloc_handler	( handler_allocator& allocator, Handler handler );

	// RESOLVED (#269 investigate): the claimed "6 out-of-line operator() symbols" do NOT exist.
	// An exhaustive index scan (mangled ??$?R.../??R... whose leftmost class token is
	// ?$custom_alloc_handler@) finds ZERO standalone custom_alloc_handler<..>::operator()
	// symbols in EITHER side - the operator() always inlines into the asio read_op/write_op
	// that invokes the handler (those op::operator() symbols carry the body and ARE matched).
	// Keeping operator() `inline` is correct; an out-of-line form would emit symbols the target
	// lacks and HURT the score. The only emitted custom_alloc_handler member is the ctor (size 25,
	// identical both sides at target 0x231f0 / base 0x6c120). See review_todos.md (ticked done).
	template < typename Arg1 >
	inline	void	operator()				( Arg1 const& arg1 ) { handler_( arg1 ); }

	template < typename Arg1, typename Arg2 >
	inline	void	operator()				( Arg1 const& arg1, Arg2 const& arg2 ) { handler_( arg1, arg2 ); }

	friend	void*	asio_handler_allocate	( u32 size, custom_alloc_handler< Handler >* this_handler )
	{
		return this_handler->m_allocator->allocate( size );
	}

	friend	void	asio_handler_deallocate	( void* pointer, u32 /*size*/, custom_alloc_handler< Handler >* this_handler )
	{
		this_handler->m_allocator->deallocate( pointer );
	}

	template < typename Function >
	friend	void	asio_handler_invoke		( Function function, custom_alloc_handler< Handler >* /*this_handler*/ )
	{
		function( );
	}

private:
	/* 0x0000 */	handler_allocator*		m_allocator;
	/* 0x0004 */	Handler					handler_;
}; // class custom_alloc_handler

template < typename Handler >
inline custom_alloc_handler< Handler >::custom_alloc_handler( handler_allocator& allocator, Handler handler ) :
	m_allocator	( &allocator ),
	handler_	( handler )
{
	/* no source */
}

template < typename Handler >
inline custom_alloc_handler< Handler > make_custom_alloc_handler( handler_allocator& allocator, Handler handler )
{
	custom_alloc_handler< Handler > const result( allocator, handler );
	return result;
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_CUSTOM_ALLOC_HANDLER_H_INCLUDED
