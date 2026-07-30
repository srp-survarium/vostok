#include "pch.h"
#include <vostok/render/core/untyped_buffer.h>

namespace vostok {
namespace render {

untyped_buffer::untyped_buffer(
	u32 size,
	pcvoid,
	enum_buffer_type type,
	bool,
	bool
)
	: m_hardware_buffer( 0 ), m_size( size ), m_type( type )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55c1a0]
}

untyped_buffer::~untyped_buffer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55c170]
}

void untyped_buffer::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x55c190]
}

} // namespace render
} // namespace vostok
