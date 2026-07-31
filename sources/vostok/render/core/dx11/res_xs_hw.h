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

	res_xs_hw( ) : m_is_registered( false )
	{
		// Observed: vs_data [0x12f980], gs_data/ps_data [0x12fd40].
	}

	~res_xs_hw( )
	{
		// Observed: vs_data [0x12f9e0], gs_data/ps_data [0x12fd90].
		safe_release( m_shader_data.hardware_shader );
	}

	void destroy_impl( ) const
	{
		resource_manager::ref().release( this );
	}

public:
	HRESULT create_hw_shader( ID3D10Blob* shader_code )
	{
		// Observed: vs_data [0x12fa30], gs_data [0x12fe00],
		// ps_data [0x1301f0].
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
		ID3D11ShaderReflection* shader_reflection,
		fixed_vector<sampler_slot, 16>& samplers,
		fixed_vector<texture_slot, 128>& textures
	)
	{
		// Observed for all shader-data instantiations [0x12ffb0].
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
				tex_slot.slot_id	= ResDesc.BindPoint; // ??? may be removed

				textures.resize( math::max( ResDesc.BindPoint+1, textures.size()));
				ASSERT( textures[ResDesc.BindPoint].name.length() == 0);

				textures[ResDesc.BindPoint] = tex_slot;
			}
			else if( ResDesc.Type == D3D10_SIT_SAMPLER)
			{
				sampler_slot smp;
				smp.name		= ResDesc.Name;
				smp.slot_id		= ResDesc.BindPoint; // ??? may be removed
				smp.state		= resource_manager::ref().find_registered_sampler( ResDesc.Name);

				//ASSERT( smp.state, "The sampler_slot hasn't been registared!");

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
