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

	void reset( res_xs_hw<shader_data>* hardware_shader )
	{
		// Observed vs_data instantiation [0x130ec0].
		m_hardware_shader = hardware_shader;
		if ( m_hardware_shader )
			m_shader_data = hardware_shader->data( );
	}

	bool set_sampler( pcstr name, ID3D11SamplerState* state )
	{
		for ( u32 i = 0; i < m_shader_data.samplers.size( ); ++i ) {
			if ( m_shader_data.samplers[i].name != name )
				continue;
			m_shader_data.samplers[i].state = state;
			return true;
		}
		return false;
	}

	bool set_texture( pcstr name, res_texture* texture )
	{
		// Observed vs_data instantiation [0x130f60].
		for ( u32 i = 0; i < m_shader_data.textures.size( ); ++i ) {
			if ( m_shader_data.textures[i].name != name )
				continue;
			m_shader_data.textures[i].texture = texture;
			return true;
		}
		return false;
	}

	bool use_texture( pcstr name ) const
	{
		// Observed vs_data instantiation [0x131030].
		for ( u32 i = 0; i < m_shader_data.textures.size( ); ++i )
			if ( m_shader_data.textures[i].name == name )
				return true;
		return false;
	}

	bool use_sampler( pcstr name ) const
	{
		for ( u32 i = 0; i < m_shader_data.samplers.size( ); ++i )
			if ( m_shader_data.samplers[i].name == name )
				return true;
		return false;
	}

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
