#include "pch.h"
#include <vostok/render/core/vertex_buffer.h>

namespace vostok {
namespace render {

vertex_buffer::vertex_buffer( u32 size )
	: m_buffer( ),
	  m_size( size ),
	  m_position( 0 ),
	  m_discard_id( 0 ),
	  m_lock_count( 0 ),
	  m_lock_stride( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739e00]
}

void* vertex_buffer::lock( u32, u32, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739d70]
	return 0;
}

void vertex_buffer::unlock( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739d40]
}

} // namespace render
} // namespace vostok
