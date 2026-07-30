#include "pch.h"
#include <vostok/render/core/render_target.h>

namespace vostok {
namespace render {

render_target::render_target( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b970]
}

render_target::~render_target( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55baf0]
}

void render_target::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x55bb40]
}

void render_target::save_as( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b700]
}

void render_target::create_3d(
	pcstr,
	u32,
	u32,
	u32,
	DXGI_FORMAT,
	enum_rt_usage,
	D3D11_USAGE
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55bed0]
}

void render_target::create(
	pcstr,
	u32,
	u32,
	DXGI_FORMAT,
	enum_rt_usage,
	D3D11_USAGE,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b710]
}

void render_target::create(
	pcstr,
	u32,
	u32,
	DXGI_FORMAT,
	enum_rt_usage,
	res_texture_ptr,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55bb50]
}

void render_target::destroy( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b9a0]
}

} // namespace render
} // namespace vostok
