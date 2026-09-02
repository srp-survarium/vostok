// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/dx11/xs_descriptor.h>
namespace vostok {
namespace render {

template < typename shader_data >
void xs_descriptor<shader_data>::reset( res_xs_hw<shader_data>* hardware_shader )
{
	m_hardware_shader = hardware_shader;
	if ( m_hardware_shader )
		m_shader_data = hardware_shader->data( );
}

template < typename shader_data >
bool xs_descriptor<shader_data>::set_sampler( pcstr name, ID3D11SamplerState* state )
{
	u32 const size = m_shader_data.samplers.size( );
	for ( u32 i = 0; i < size; ++i ) {
		if ( m_shader_data.samplers[i].name != name )
			continue;
		m_shader_data.samplers[i].state = state;
		return true;
	}
	return false;
}

template < typename shader_data >
bool xs_descriptor<shader_data>::set_texture( pcstr name, res_texture* texture )
{
	u32 const size = m_shader_data.textures.size( );
	for ( u32 i = 0; i < size; ++i ) {
		if ( m_shader_data.textures[i].name != name )
			continue;
		m_shader_data.textures[i].texture = texture;
		return true;
	}
	return false;
}

template < typename shader_data >
bool xs_descriptor<shader_data>::use_texture( pcstr name )
{
	u32 const size = m_shader_data.textures.size( );
	for ( u32 i = 0; i < size; ++i )
		if ( m_shader_data.textures[i].name == name )
			return true;
	return false;
}

template < typename shader_data >
bool xs_descriptor<shader_data>::use_sampler( pcstr name )
{
	u32 const size = m_shader_data.samplers.size( );
	for ( u32 i = 0; i < size; ++i )
		if ( m_shader_data.samplers[i].name == name )
			return true;
	return false;
}

// Specialization definitions
template class xs_descriptor<vs_data>;
template class xs_descriptor<gs_data>;
template class xs_descriptor<ps_data>;

} // namespace render
} // namespace vostok
