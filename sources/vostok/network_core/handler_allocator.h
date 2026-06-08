////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
#define NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <boost/aligned_storage.hpp>

namespace vostok {
namespace network_core {

class handler_allocator : public core::noncopyable {
public:
	inline				handler_allocator	( )
	{
		// FUNCTION BODY[0x131730]: 0
		// <0x131730>|0x000|+0x019:'29'	{
		// <0x131749>|0x019|      :'30'	}
		// ******
	}

	inline	void*		allocate			( u32 size ) { return NULL; }

	inline	void		deallocate			( void* pointer ) { /* no source */ }

	inline				~handler_allocator	( ) { /* no source */ }

private:
	/* 0x0000 */	boost::aligned_storage< 1024, 4294967295 >	storage_;
	/* 0x0400 */	bool	in_use_;
}; // class handler_allocator

STATIC_SIZE_ASSERT(handler_allocator, 0x408);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
