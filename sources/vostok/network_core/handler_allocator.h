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
	// STATE[0%|DONE]: target keeps a standalone ctor COMDAT (0x121730, 32B) our base link
	// never emits (LTCG inlines it into both base ctor call sites) - unpairable, 0% banked.
	// claude@MATCH: no ASSERT here - both target inline sites (udp_match_connection ctor
	// 0x205-0x219, udp_match_client ctor 0x191-0x1ad) show sub-ctor call + in_use_ store only,
	// no assert eater; the previously-added ASSERT( UNKNOWN_EXPRESSION ) emitted extra bytes.
	inline				handler_allocator	( )
	{
		in_use_			= false;
	}

	// STATE[INLINED]: no standalone allocate symbol on the target side - body proven from
	// the boost_asio_handler_alloc_helpers::allocate< custom_alloc_handler<..> > consumer
	// COMDATs, which PAIR at 100.0 (base 0x899e0 vs target 0x122ba0, 94 bytes,
	// instruction-identical): in_use_ test, size<storage_.size, return storage address /
	// else operator new, incl. the double jmp-short block exit. Legacy boost-example body.
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

	// STATE[INLINED]: legacy body; no standalone deallocate symbol on either side
	// (asio_handler_deallocate inlines into the op dtors) - body proven from consumer
	// bytes: recv_op::do_complete 76->100, 2x async_receive 98.06->100 on adoption.
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
