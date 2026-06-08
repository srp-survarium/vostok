////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_CUSTOM_ALLOC_HANDLER_H_INCLUDED
#define NETWORK_CORE_CUSTOM_ALLOC_HANDLER_H_INCLUDED

#include <vostok/network_core/handler_allocator.h>

namespace vostok {
namespace network_core {

template < typename Handler >
class custom_alloc_handler {
public:
	inline			custom_alloc_handler	( handler_allocator& allocator, Handler handler );

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

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_CUSTOM_ALLOC_HANDLER_H_INCLUDED
