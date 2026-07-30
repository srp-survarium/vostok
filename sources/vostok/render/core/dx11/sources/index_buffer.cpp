#include "pch.h"
#include <vostok/render/core/index_buffer.h>

namespace vostok {
namespace render {

index_buffer::index_buffer( u32 size )
	: m_buffer( ),
	  m_size( size ),
	  m_position( 0 ),
	  m_discard_id( 0 ),
	  m_lock_size( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739c20]
}

u16* index_buffer::lock( u32, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739bb0]
	return 0;
}

void index_buffer::unlock( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739b80]
}

} // namespace render
} // namespace vostok
