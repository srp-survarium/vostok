#ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_HW_IMPL_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_XS_HW_IMPL_H_INCLUDED

namespace vostok {
namespace render {

template < typename shader_data >
res_xs_hw<shader_data>::res_xs_hw( ) :
	m_is_registered( false )
{
}

template < typename shader_data >
res_xs_hw<shader_data>::~res_xs_hw( )
{
	safe_release( m_shader_data.hardware_shader );
}

template < typename shader_data >
HRESULT res_xs_hw<shader_data>::create_hw_shader( ID3D10Blob* shader_code )
{
	ASSERT( shader_code);

	void const* buffer	= shader_code->GetBufferPointer();
	u32 buffer_size		= (u32)shader_code->GetBufferSize();

	HRESULT hr = create_hw_shader( shader_code, &m_shader_data.hardware_shader);

	if ( SUCCEEDED( hr))
	{
		ID3D11ShaderReflection* shader_reflection;

		hr			= D3DReflect( buffer, buffer_size, IID_ID3D11ShaderReflection, (void**)&shader_reflection);

		D3D11_SHADER_DESC shader_desc;

		shader_reflection->GetDesc(&shader_desc);

		m_shader_data.instruction_count = static_cast<u16>( shader_desc.InstructionCount );

		m_shader_data.constants.parse	( shader_reflection, (enum_shader_type)shader_data::type);
		parse_resources		( shader_reflection, m_shader_data.samplers, m_shader_data.textures);

		safe_release( shader_reflection);
	}
	else
	{
		LOG_ERROR( "! CreateVertexShader hr == %08x", ( int)hr);
	}

	return hr;
}

template < typename shader_data >
void res_xs_hw<shader_data>::parse_resources( ID3D11ShaderReflection* shader_reflection,
	fixed_vector<sampler_slot, 16>& samplers,
	fixed_vector<texture_slot, 128>& textures )
{
	D3D11_SHADER_DESC	shader_desc;

	shader_reflection->GetDesc( &shader_desc);

	for( u32 i=0; i<shader_desc.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC	ResDesc;
		shader_reflection->GetResourceBindingDesc(i, &ResDesc);

		ASSERT(ResDesc.BindCount==1);

		if( ResDesc.Type == D3D10_SIT_TEXTURE)
		{
			texture_slot tex_slot;
			tex_slot.name		= ResDesc.Name;
			tex_slot.slot_id	= ResDesc.BindPoint;

			textures.resize( math::max( ResDesc.BindPoint+1, textures.size()));
			ASSERT( textures[ResDesc.BindPoint].name.length() == 0);

			textures[ResDesc.BindPoint] = tex_slot;
		}
		else if( ResDesc.Type == D3D10_SIT_SAMPLER)
		{
			sampler_slot smp;
			smp.name		= ResDesc.Name;
			smp.slot_id		= ResDesc.BindPoint;
			smp.state		= resource_manager::ref().find_registered_sampler( ResDesc.Name);

			samplers.resize( math::max( ResDesc.BindPoint+1, samplers.size()));
			ASSERT( samplers[ResDesc.BindPoint].name.length() == 0);

			samplers[ResDesc.BindPoint] = smp;
		}
		else if( ResDesc.Type == D3D11_SIT_UAV_RWTYPED)
		{
			NOT_IMPLEMENTED();
		}
		else
		{
			continue;
		}
	}
	return;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_HW_IMPL_H_INCLUDED
