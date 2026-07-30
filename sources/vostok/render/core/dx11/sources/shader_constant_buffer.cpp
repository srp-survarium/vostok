#include "pch.h"
#include <vostok/render/core/shader_constant_buffer.h>

namespace vostok {
namespace render {

shader_constant_buffer::shader_constant_buffer(
	fixed_string<64> const&,
	enum_shader_type,
	D3D_CBUFFER_TYPE,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x738fd0]
}

void shader_constant_buffer::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x738fc0]
}

shader_constant_buffer::~shader_constant_buffer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x738f80]
}

void shader_constant_buffer::update( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x738f40]
}

} // namespace render
} // namespace vostok
