// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_STATE_UTILS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_STATE_UTILS_H_INCLUDED
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {
namespace state_utils {

void reset( D3D11_RASTERIZER_DESC& desc );
void reset( D3D11_DEPTH_STENCIL_DESC& desc );
void reset( D3D11_BLEND_DESC& desc );

bool operator==(
	D3D11_RASTERIZER_DESC const& left,
	D3D11_RASTERIZER_DESC const& right
);
bool operator==(
	D3D11_DEPTH_STENCIL_DESC const& left,
	D3D11_DEPTH_STENCIL_DESC const& right
);
bool operator==(
	D3D11_BLEND_DESC const& left,
	D3D11_BLEND_DESC const& right
);
bool operator==(
	D3D11_SAMPLER_DESC const& left,
	D3D11_SAMPLER_DESC const& right
);

u32 get_hash( D3D11_RASTERIZER_DESC const& desc );
u32 get_hash( D3D11_DEPTH_STENCIL_DESC const& desc );
u32 get_hash( D3D11_BLEND_DESC const& desc );
u32 get_hash( D3D11_SAMPLER_DESC const& desc );

} // namespace state_utils
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_STATE_UTILS_H_INCLUDED
