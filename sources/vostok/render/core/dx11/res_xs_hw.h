// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_HW_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_XS_HW_H_INCLUDED
#include <d3dcompiler.h>
#include <vostok/shared_string.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/dx11/xs_data.h>
#include <vostok/render/core/dx11/sources/com_utils.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

template < typename shader_data >
class res_xs_hw : public resource_intrusive_base {
	friend class resource_intrusive_base;
	friend class resource_manager;

	res_xs_hw( );
	~res_xs_hw( );

	void destroy_impl( ) const
	{
		resource_manager::ref().release( this );
	}

public:
	HRESULT create_hw_shader( ID3D10Blob* shader_code );

	typename shader_data::hw_interface* hardware_shader( ) const
	{
		return m_shader_data.hardware_shader;
	}

	shader_data const& data( ) const { return m_shader_data; }
	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }
	shared_string const& name( ) const { return m_name; }

	void set_name( shared_string const& name )
	{
		m_name = name;
	}

private:
	HRESULT create_hw_shader(
		ID3D10Blob* shader_code,
		typename shader_data::hw_interface** hardware_shader
	);

	void parse_resources(
		ID3D11ShaderReflection* shader_reflection,
		fixed_vector<sampler_slot, 16>& samplers,
		fixed_vector<texture_slot, 128>& textures
	);

private:
	shader_data	m_shader_data;
	shared_string	m_name;
	bool			m_is_registered;
};

typedef res_xs_hw<vs_data> res_vs_hw;
typedef res_xs_hw<gs_data> res_gs_hw;
typedef res_xs_hw<ps_data> res_ps_hw;

typedef intrusive_ptr<
	res_vs_hw,
	resource_intrusive_base,
	threading::single_threading_policy
> res_vs_hw_ptr;
typedef intrusive_ptr<
	res_gs_hw,
	resource_intrusive_base,
	threading::single_threading_policy
> res_gs_hw_ptr;
typedef intrusive_ptr<
	res_ps_hw,
	resource_intrusive_base,
	threading::single_threading_policy
> res_ps_hw_ptr;

STATIC_SIZE_ASSERT( res_xs_hw<vs_data>, 0x2F88 );
STATIC_SIZE_ASSERT( res_xs_hw<gs_data>, 0x2F84 );
STATIC_SIZE_ASSERT( res_xs_hw<ps_data>, 0x2F84 );

} // namespace render
} // namespace vostok

#include <vostok/render/core/dx11/res_xs_hw_impl.h>

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_HW_H_INCLUDED
