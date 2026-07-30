#ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_HW_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_XS_HW_H_INCLUDED

#include <vostok/shared_string.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/dx11/xs_data.h>

namespace vostok {
namespace render {

class resource_manager;

template < typename shader_data >
class res_xs_hw : public resource_intrusive_base {
	friend class resource_intrusive_base;
	friend class resource_manager;

	res_xs_hw( ) : m_is_registered( false )
	{
		// STATE[STUB]
		// Observed: vs_data [0x12f980], gs_data/ps_data [0x12fd40].
	}

	~res_xs_hw( )
	{
		// STATE[STUB]
		// Observed: vs_data [0x12f9e0], gs_data/ps_data [0x12fd90].
	}

	void destroy_impl( ) const { /* STATE[STUB] */ }

public:
	HRESULT create_hw_shader( ID3D10Blob* shader_code )
	{
		// STATE[STUB]
		// Observed: vs_data [0x12fa30], gs_data [0x12fe00],
		// ps_data [0x1301f0].
		return create_hw_shader( shader_code, &m_shader_data.hardware_shader );
	}

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
		// Observed vs_data instantiation [0x126bd0].
		m_name = name;
	}

private:
	HRESULT create_hw_shader(
		ID3D10Blob* shader_code,
		typename shader_data::hw_interface** hardware_shader
	);

	void parse_resources(
		ID3D11ShaderReflection*,
		fixed_vector<sampler_slot, 16>&,
		fixed_vector<texture_slot, 128>&
	)
	{
		// STATE[STUB]
		// Observed for all shader-data instantiations [0x12ffb0].
	}

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

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_HW_H_INCLUDED
