////////////////////////////////////////////////////////////////////////////
//	Created 	: 18.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_ARRAY_CHUNK_READER_H_INCLUDED
#define VOSTOK_MEMORY_ARRAY_CHUNK_READER_H_INCLUDED

namespace vostok {
namespace memory {

template < typename implementation >
class array_chunk_reader {
public:
	inline	void	construct		( );
	inline	u32		chunk_position	( u32 chunk_id );

private:
	inline	implementation&	impl	( );
}; // class reader

} // namespace memory
} // namespace vostok

#include <vostok/memory_array_chunk_reader_inline.h>

#endif // #ifndef VOSTOK_MEMORY_ARRAY_CHUNK_READER_H_INCLUDED