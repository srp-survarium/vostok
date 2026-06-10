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
	// STATE[100%|DONE]: source matched; no base COMDAT (LTCG inlines into ctors)
	// claude@MATCH: no ASSERT here - both target inline sites (udp_match_connection ctor
	// 0x205-0x219, udp_match_client ctor 0x191-0x1ad) show sub-ctor call + in_use_ store only,
	// no assert eater; the previously-added ASSERT( UNKNOWN_EXPRESSION ) emitted extra bytes.
	inline				handler_allocator	( )
	{
		in_use_			= false;
	}

	// STATE[100%|DONE]: legacy boost-example body (temp/network_legacy/sources/
	// handler_allocator.h), byte-proven: the base boost_asio_handler_alloc_helpers::
	// allocate< custom_alloc_handler<..> > COMDAT (rva 0x899e0, 94 bytes) is instruction-
	// identical to the target's (rva 0x122ba0) - in_use_ test, size<storage_.size,
	// return storage address / else operator new, incl. the double jmp-short block exit.
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

	// STATE[DONE]: legacy body; no standalone deallocate-helper COMDAT exists on either
	// side (asio_handler_deallocate inlines into the op dtors) - adopted with allocate
	// as one proven pair; recv_op::do_complete 76->100, 2x async_receive 98.06->100.
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

	inline				~handler_allocator	( ) { /* no source */ }

private:
	/* 0x0000 */	boost::aligned_storage< 1024, 4294967295 >	storage_;
	/* 0x0400 */	bool	in_use_;
}; // class handler_allocator

STATIC_SIZE_ASSERT(handler_allocator, 0x408);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
