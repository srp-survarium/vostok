#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_COMPILER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_COMPILER_H_INCLUDED

#include <boost/noncopyable.hpp>

#include <vostok/fixed_string.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/shared_string.h>
#include <vostok/vectora.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/shader_constant_binding.h>
#include <vostok/render/core/shader_constant_bindings.h>
#include <vostok/render/core/shader_include_getter.h>
#include <vostok/render/core/texture_named_instance.h>
#include <vostok/render/core/dx11/effect_constant_storage.h>
#include <vostok/render/core/dx11/res_texture.h>
#include <vostok/render/core/dx11/sampler_state_descriptor.h>
#include <vostok/render/core/dx11/state_descriptor.h>
#include <vostok/render/core/dx11/xs_descriptor.h>

namespace vostok {

namespace resources {
class query_result_for_cook;
} // namespace resources

namespace render {

class effect_compiler : public boost::noncopyable {
public:
	struct shader_cache_info {
		fs_new::virtual_path_string vertex_shader_name;
		fs_new::virtual_path_string pixel_shader_name;
		fs_new::virtual_path_string geometry_shader_name;
		shader_configuration configuration;
	};
	typedef vectora< shader_cache_info > shader_cache_info_vector;

	struct texture_query_desc {
		texture_query_desc( ) : m_mip_level_cut( 0 ), m_num_last_mips_used( 0 ) { }

		fixed_string<260> m_query_physicaly_path;
		u32 m_mip_level_cut;
		u32 m_num_last_mips_used;
	};
	typedef vector< texture_query_desc > textures_for_query_type;

public:
	// claude@NOTE: no legacy ancestor - set_mapping was declaration-only in the legacy header (no definition anywhere in the corpus); matcher-phase work.
	void set_mapping( ) { /* STATE[STUB] */ }

	effect_compiler& begin_pass(
		pcstr vs_name,
		pcstr gs_name,
		pcstr ps_name,
		shader_configuration const& shader_config,
		shader_include_getter* include_getter
	);
	effect_compiler& end_pass( );
	effect_compiler& begin_technique( );
	void end_technique( );

	effect_compiler& set_depth(
		bool enable,
		bool write_enable,
		D3D11_COMPARISON_FUNC cmp_func = D3D11_COMPARISON_LESS_EQUAL
	);
	effect_compiler& set_stencil(
		s32 enable,
		u32 ref = 0x00,
		u8 read_mask = 0x00,
		u8 write_mask = 0x00,
		D3D11_COMPARISON_FUNC func = D3D11_COMPARISON_ALWAYS,
		D3D11_STENCIL_OP fail = D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP pass = D3D11_STENCIL_OP_KEEP,
		D3D11_STENCIL_OP zfail = D3D11_STENCIL_OP_KEEP
	);
	effect_compiler& set_stencil_ref( u32 ref )
	{
		m_state_descriptor.set_stencil_ref( ref );
		return *this;
	}
	effect_compiler& set_alpha_blend(
		s32 blend_enable,
		D3D11_BLEND src_blend = D3D11_BLEND_ONE,
		D3D11_BLEND dest_blend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blend_op = D3D11_BLEND_OP_ADD,
		D3D11_BLEND src_alpha_blend = D3D11_BLEND_ONE,
		D3D11_BLEND dest_alpha_blend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blend_alpha_op = D3D11_BLEND_OP_ADD
	);
	effect_compiler& set_cull_mode( D3D11_CULL_MODE mode );
	effect_compiler& set_alpha_to_coverage( s32 enabled )
	{
		m_state_descriptor.set_alpha_to_coverage( enabled );
		return *this;
	}
	effect_compiler& color_write_enable( D3D11_COLOR_WRITE_ENABLE mode = D3D11_COLOR_WRITE_ENABLE_ALL );
	effect_compiler& set_fill_mode( D3D11_FILL_MODE fill_mode );

	effect_compiler& def_sampler( pcstr name, sampler_state_descriptor& sampler_desc )
	{
		if (m_shaders_cache_mode)
			return *this;

		if( resource_manager::ref().find_registered_sampler( name) )
		{
			ASSERT( 0, "A sampler with name \"%s\" was already registered globally.", name);
			return *this;
		}

		ID3D11SamplerState* sampler  =  resource_manager::ref().create_sampler_state( sampler_desc);

		bool res = false;
		if( m_vs_hw)
			res |= m_vs_descriptor.set_sampler( name, sampler);

		if( m_gs_hw)
			res |= m_gs_descriptor.set_sampler( name, sampler);

		if( m_ps_hw)
			res |= m_ps_descriptor.set_sampler( name, sampler);

		//if( !res)
		//	LOG_WARNING( "The sampler \"%s\" was specified in effect descriptor, but not used by any of effect shaders.", name);

		return *this;
	}
	effect_compiler& def_sampler(
		pcstr name,
		D3D11_TEXTURE_ADDRESS_MODE address,
		D3D11_FILTER filter
	)
	{
		if (m_shaders_cache_mode)
			return *this;

		m_sampler_state_descriptor.reset()
									.set_address_mode	( address, address, address)
									.set_filter		( filter);

		def_sampler( name, m_sampler_state_descriptor);

		return *this;
	}
	sampler_state_descriptor& begin_sampler( pcstr )
	{
		m_sampler_state_descriptor.m_effect_compiler = this;
		return m_sampler_state_descriptor;
	}
	effect_compiler& end_sampler( ) { return *this; }

	effect_compiler& set_texture(
		pcstr name,
		pcstr physical_name,
		res_texture_ptr* out_texture,
		bool streamed,
		u32 num_last_mips_used
	);
	effect_compiler& set_texture(
		pcstr hlsl_name,
		shared_string physical_name,
		bool streamed,
		u32 num_last_mips_used
	)
	{
		// FUNCTION BODY[0x89160]
		return set_texture(
			hlsl_name,
			physical_name.c_str( ),
			0,
			streamed,
			num_last_mips_used
		);
	}
	effect_compiler& set_texture(
		pcstr name,
		res_texture* texture,
		bool streamed,
		u32 num_last_mips_used
	);

	effect_compiler& bind_constant( shader_constant_binding const& binding );

	template < typename T >
	effect_compiler& bind_constant( shared_string hlsl_name, T const* source )
	{
		return bind_constant( shader_constant_binding( hlsl_name, source ) );
	}

	template < typename T >
	effect_compiler& set_constant( shared_string hlsl_name, T const& source )
	{
		// Observed float3 instantiation [0x887f0] and float2 [0x8cc90].
		return bind_constant(
			shader_constant_binding(
				hlsl_name,
				effect_constant_storage::ref( ).store_constant( source )
			)
		);
	}

	effect_compiler(
		res_effect& effect,
		resources::query_result_for_cook* parent,
		bool shaders_cache_mode,
		binary_shader_sources_type* in_shader_sources
	);
	~effect_compiler( );

	shader_cache_info_vector const get_cached_shaders_info( ) const
	{
		// FUNCTION BODY[0x130450]
		return m_shader_cache_info;
	}

private:
	u32 get_sampler( pcstr ) { return 0; }
	// claude@NOTE: no legacy ancestor - the six set_samp_* below were declaration-only in the legacy header (bodies existed only in the dead D3D9 blender_compiler generation); matcher-phase work.
	void set_samp_texture( u32, pcstr ) { /* STATE[STUB] */ }
	void set_samp_address( u32, u32 ) { /* STATE[STUB] */ }
	void set_samp_filter_min( u32, u32 ) { /* STATE[STUB] */ }
	void set_samp_filter_mip( u32, u32 ) { /* STATE[STUB] */ }
	void set_samp_filter_mag( u32, u32 ) { /* STATE[STUB] */ }
	void set_samp_filter( u32, D3D11_FILTER ) { /* STATE[STUB] */ }

public:
	binary_shader_sources_type* m_shader_sources;
	resources::query_result_for_cook* m_parent_query;

private:
	shader_cache_info_vector m_shader_cache_info;

public:
	vector<texture_named_instance> m_ps_used_textures;
	textures_for_query_type m_textures_for_query;

private:
	res_ps_hw_ptr m_ps_hw;
	res_gs_hw_ptr m_gs_hw;
	res_vs_hw_ptr m_vs_hw;
	state_descriptor m_state_descriptor;
	sampler_state_descriptor m_sampler_state_descriptor;
	vs_descriptor m_vs_descriptor;
	gs_descriptor m_gs_descriptor;
	ps_descriptor m_ps_descriptor;
	pcstr m_curr_sampler_name;
	shader_constant_bindings m_bindings;
	res_shader_technique m_sh_technique;
	res_effect& m_compilation_target;
	u32 m_technique_idx;
	u32 m_pass_idx;
	bool m_shaders_cache_mode;
};

STATIC_SIZE_ASSERT( effect_compiler::shader_cache_info, 0x350 );
STATIC_SIZE_ASSERT( effect_compiler::texture_query_desc, 0x118 );
STATIC_SIZE_ASSERT( effect_compiler, 0x9094 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_COMPILER_H_INCLUDED
