// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
#define NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <boost/aligned_storage.hpp>

namespace vostok {
namespace network_core {

class handler_allocator : private core::noncopyable {
public:
	// claude@MATCH: no ASSERT here - both target inline sites (udp_match_connection ctor
	// 0x205-0x219, udp_match_client ctor 0x191-0x1ad) show sub-ctor call + in_use_ store only,
	// no assert eater; the previously-added ASSERT( UNKNOWN_EXPRESSION ) emitted extra bytes.
	inline				handler_allocator	( )
	{
		in_use_			= false;
	}

	inline	void*		allocate			( u32 size )
	{
		if ( !in_use_ && size < storage_.size )
		{
			in_use_			= true;
			return storage_.address( );
		}
		else
		{
			return ::operator new( size );
		}
	}

	inline	void		deallocate			( void* pointer )
	{
		if ( pointer == storage_.address( ) )
		{
			in_use_			= false;
		}
		else
		{
			::operator delete( pointer );
		}
	}

private:
	/* 0x0000 */	boost::aligned_storage< 1024, 4294967295 >	storage_;
	/* 0x0400 */	bool	in_use_;
}; // class handler_allocator

STATIC_SIZE_ASSERT(handler_allocator, 0x408);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
