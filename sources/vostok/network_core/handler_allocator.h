////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
#define NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED

#include <boost/aligned_storage.hpp>

namespace vostok {
namespace network_core {

class handler_allocator : public core::noncopyable {
public:
	inline				handler_allocator	( ) : in_use_( false ) { }

	inline	void*		allocate			( u32 size )
	{
		if ( !in_use_ && size < storage_.size )
		{
			in_use_	= true;
			return storage_.address( );
		}

		return ::operator new( size );
	}

	inline	void		deallocate			( void* pointer )
	{
		if ( pointer == storage_.address( ) )
			in_use_	= false;
		else
			::operator delete( pointer );
	}

	inline				~handler_allocator	( ) { }

private:
	boost::aligned_storage<1024>	storage_;
	bool							in_use_;
}; // class handler_allocator

STATIC_SIZE_ASSERT(handler_allocator, 0x408);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HANDLER_ALLOCATOR_H_INCLUDED
