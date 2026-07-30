#include "pch.h"
#include <vostok/render/core/res_render_output.h>
#include <vostok/render/core/res_texture.h>

namespace vostok {
namespace render {

res_render_output::res_render_output( HWND, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b5f0]
}

res_render_output::~res_render_output( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ad40]
}

void res_render_output::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ae60]
}

void res_render_output::initialize_swap_chain( IDXGISwapChain* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b570]
}

void res_render_output::present( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55adc0]
}

DXGI_RATIONAL res_render_output::select_refresh(
	u32,
	u32,
	DXGI_FORMAT
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ac40]
	DXGI_RATIONAL result = { 0, 0 };
	return result;
}

void res_render_output::select_resolution(
	u32&,
	u32&,
	bool,
	HWND
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x55abe0]
}

bool set_client_rect( HWND, s32, s32, s32, s32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ab60]
	return false;
}

void res_render_output::set_size( u32, u32, bool, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b4a0]
}

void res_render_output::resize( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b480]
}

void res_render_output::resize( bool, u32, u32, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55b160]
}

} // namespace render
} // namespace vostok
