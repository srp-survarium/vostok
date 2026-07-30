#include "pch.h"
#include <vostok/render/core/state_utils.h>

namespace vostok {
namespace render {
namespace state_utils {

void crc::process( crc::processor&, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12f920]
}

void reset( D3D11_RASTERIZER_DESC& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x558290]
}

void reset( D3D11_DEPTH_STENCIL_DESC& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x558240]
}

void reset( D3D11_BLEND_DESC& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5581e0]
}

bool operator==(
	D3D11_RASTERIZER_DESC const&,
	D3D11_RASTERIZER_DESC const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x558170]
	return false;
}

bool operator==(
	D3D11_DEPTH_STENCIL_DESC const&,
	D3D11_DEPTH_STENCIL_DESC const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5580f0]
	return false;
}

bool operator==(
	D3D11_BLEND_DESC const&,
	D3D11_BLEND_DESC const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x558070]
	return false;
}

bool operator==(
	D3D11_SAMPLER_DESC const&,
	D3D11_SAMPLER_DESC const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x557fd0]
	return false;
}

u32 get_hash( D3D11_RASTERIZER_DESC const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5587a0]
	return 0;
}

u32 get_hash( D3D11_DEPTH_STENCIL_DESC const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5586e0]
	return 0;
}

u32 get_hash( D3D11_BLEND_DESC const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5583d0]
	return 0;
}

u32 get_hash( D3D11_SAMPLER_DESC const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5582e0]
	return 0;
}

} // namespace state_utils
} // namespace render
} // namespace vostok
