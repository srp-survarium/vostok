#ifndef VOSTOK_RENDER_CORE_DX11_XS_DESCRIPTOR_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_XS_DESCRIPTOR_H_INCLUDED

#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/dx11/res_xs_hw.h>

namespace vostok {
namespace render {

template < typename shader_data >
class xs_descriptor {
	typedef intrusive_ptr<
		res_xs_hw<shader_data>,
		resource_intrusive_base,
		threading::single_threading_policy
	> ref_xs_hw;

public:
	xs_descriptor( ) { }

	void reset( res_xs_hw<shader_data>* hardware_shader );

	bool set_sampler( pcstr name, ID3D11SamplerState* state );
	bool set_texture( pcstr name, res_texture* texture );
	bool use_texture( pcstr name );
	bool use_sampler( pcstr name );

	shader_data& data( ) { return m_shader_data; }
	shader_data const& data( ) const { return m_shader_data; }
	ref_xs_hw const& hardware_shader( ) const { return m_hardware_shader; }

private:
	ref_xs_hw	m_hardware_shader;
	shader_data	m_shader_data;
};

typedef xs_descriptor<vs_data> vs_descriptor;
typedef xs_descriptor<gs_data> gs_descriptor;
typedef xs_descriptor<ps_data> ps_descriptor;

STATIC_SIZE_ASSERT( xs_descriptor<vs_data>, 0x2F80 );
STATIC_SIZE_ASSERT( xs_descriptor<gs_data>, 0x2F7C );
STATIC_SIZE_ASSERT( xs_descriptor<ps_data>, 0x2F7C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_XS_DESCRIPTOR_H_INCLUDED
