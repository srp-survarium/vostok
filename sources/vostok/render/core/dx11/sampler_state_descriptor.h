// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SAMPLER_STATE_DESCRIPTOR_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SAMPLER_STATE_DESCRIPTOR_H_INCLUDED
#include <cfloat>

#include <vostok/math_float4.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class effect_compiler;
class resource_manager;

class sampler_state_descriptor {
	friend class effect_compiler;
	friend class resource_manager;

private:
	explicit sampler_state_descriptor( effect_compiler& compiler ) :
		m_effect_compiler( &compiler )
	{
	}

public:
	sampler_state_descriptor( ) : m_effect_compiler( 0 ), m_updated( false )
	{
		reset( );
	}

	sampler_state_descriptor& reset( )
	{
		m_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_desc.MipLODBias = 0.f;
		m_desc.MaxAnisotropy = 1;
		m_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_desc.BorderColor[0] = 1.f;
		m_desc.BorderColor[1] = 1.f;
		m_desc.BorderColor[2] = 1.f;
		m_desc.BorderColor[3] = 1.f;
		m_desc.MinLOD = -FLT_MAX;
		m_desc.MaxLOD = FLT_MAX;
		return *this;
	}

	sampler_state_descriptor& set(
		D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE address_mode
	)
	{
		m_desc.Filter = filter;
		m_desc.AddressU = address_mode;
		m_desc.AddressV = address_mode;
		m_desc.AddressW = address_mode;
		return *this;
	}

	sampler_state_descriptor& set_filter( D3D11_FILTER filter )
	{
		m_desc.Filter = filter;
		return *this;
	}

	sampler_state_descriptor& set_address_mode(
		D3D11_TEXTURE_ADDRESS_MODE address_u,
		D3D11_TEXTURE_ADDRESS_MODE address_v,
		D3D11_TEXTURE_ADDRESS_MODE address_w
	)
	{
		m_desc.AddressU = address_u;
		m_desc.AddressV = address_v;
		m_desc.AddressW = address_w;
		return *this;
	}

	sampler_state_descriptor& set_mip( float lod_bias, float min_lod, float max_lod )
	{
		m_desc.MipLODBias = lod_bias;
		m_desc.MinLOD = min_lod;
		m_desc.MaxLOD = max_lod;
		return *this;
	}

	sampler_state_descriptor& set_max_anisotropy( u32 max_anisotropy )
	{
		m_desc.MaxAnisotropy = max_anisotropy;
		return *this;
	}

	sampler_state_descriptor& set_comparison_function( D3D11_COMPARISON_FUNC function )
	{
		m_desc.ComparisonFunc = function;
		return *this;
	}

	sampler_state_descriptor& set_border_color( float4 color )
	{
		m_desc.BorderColor[0] = color.x;
		m_desc.BorderColor[1] = color.y;
		m_desc.BorderColor[2] = color.z;
		m_desc.BorderColor[3] = color.w;
		return *this;
	}

	effect_compiler& end_sampler( ) { return *m_effect_compiler; }

private:
	effect_compiler*	m_effect_compiler;
	D3D11_SAMPLER_DESC	m_desc;
	bool				m_updated;
};

STATIC_SIZE_ASSERT( sampler_state_descriptor, 0x3C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SAMPLER_STATE_DESCRIPTOR_H_INCLUDED
