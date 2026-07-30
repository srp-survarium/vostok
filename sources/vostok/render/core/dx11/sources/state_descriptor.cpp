#include "pch.h"

#include <vostok/render/core/dx11/state_descriptor.h>

namespace vostok {
namespace render {

state_descriptor::state_descriptor( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557fc0]
}

void state_descriptor::reset( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557f00]
}

void state_descriptor::set_depth( bool, bool, D3D11_COMPARISON_FUNC )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557ed0]
}

void state_descriptor::set_stencil( s32, u32, u8, u8 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557ea0]
}

void state_descriptor::set_stencil_frontface(
	D3D11_COMPARISON_FUNC,
	D3D11_STENCIL_OP,
	D3D11_STENCIL_OP,
	D3D11_STENCIL_OP
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x557e70]
}

void state_descriptor::set_stencil_backface(
	D3D11_COMPARISON_FUNC,
	D3D11_STENCIL_OP,
	D3D11_STENCIL_OP,
	D3D11_STENCIL_OP
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x557e40]
}

void state_descriptor::set_alpha_blend(
	s32,
	D3D11_BLEND,
	D3D11_BLEND,
	D3D11_BLEND_OP,
	D3D11_BLEND,
	D3D11_BLEND,
	D3D11_BLEND_OP
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x557cd0]
}

void state_descriptor::set_cull_mode( D3D11_CULL_MODE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557cc0]
}

void state_descriptor::color_write_enable( D3D11_COLOR_WRITE_ENABLE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557c90]
}

void state_descriptor::set_fill_mode( D3D11_FILL_MODE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x557c80]
}

} // namespace render
} // namespace vostok
