////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COMPILER_H_INCLUDED
#define RENDER_EFFECT_COMPILER_H_INCLUDED

/* INCLUDES */
class const vostok::vectora<vostok::render::effect_compiler::shader_cache_info>;
typedef associative_vector<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base>,vostok::render::vector,stlp_std::less<vostok::render::binary_shader_key_type> >
	associative_vector<vostok::render::binary_shader_key_type,vostok::render::binary_shader_source_ptr,vostok::render::vector,stlp_std::less<vostok::render::binary_shader_key_type> >;
class vostok::render::res_effect;
class vostok::render::res_shader_technique;
class vostok::render::sampler_state_descriptor;
class vostok::render::shader_constant_bindings;
class vostok::render::state_descriptor;
class vostok::render::vector<vostok::render::effect_compiler::texture_query_desc>;
class vostok::render::vector<vostok::render::texture_named_instance>;
class vostok::render::xs_descriptor<vostok::render::gs_data>;
class vostok::render::xs_descriptor<vostok::render::ps_data>;
class vostok::render::xs_descriptor<vostok::render::vs_data>;
class vostok::resources::query_result_for_cook;
class vostok::vectora<vostok::render::effect_compiler::shader_cache_info>;
class vostok::render::res_texture;
class vostok::shared_string;
typedef vostok::intrusive_ptr<vostok::render::res_xs_hw<vostok::render::gs_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs_hw<vostok::render::gs_data>_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_xs_hw<vostok::render::ps_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs_hw<vostok::render::ps_data>_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_xs_hw<vostok::render::vs_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs_hw<vostok::render::vs_data>_ptr;
class vostok::render::binary_shader_source;
class vostok::render::res_xs_hw<vostok::render::gs_data>;
class vostok::render::res_xs_hw<vostok::render::ps_data>;
class vostok::render::res_xs_hw<vostok::render::vs_data>;

/* FORWARD REFS */
class vostok::render::shader_configuration;
class vostok::render::shader_constant_binding;
class vostok::render::shader_include_getter;

namespace vostok {
namespace render {

class effect_compiler : public boost::noncopyable {
public:
	inline	void							set_mapping				( ) { /* no source */ }

			effect_compiler&				begin_pass				(
												pcstr							vs_name,
												pcstr							gs_name,
												pcstr							ps_name,
												shader_configuration const&		shader_config,
												shader_include_getter*			include_getter
											);
			effect_compiler&				end_pass				( );

			effect_compiler&				begin_technique			( );
			void							end_technique			( );

			effect_compiler&				set_depth				( bool enable, bool write_enable, D3D11_COMPARISON_FUNC cmp_func );
			effect_compiler&				set_stencil				(
												s32							enable,
												u32							ref,
												u8							read_mask,
												u8							write_mask,
												D3D11_COMPARISON_FUNC		func,
												D3D11_STENCIL_OP			fail,
												D3D11_STENCIL_OP			pass,
												D3D11_STENCIL_OP			zfail
											);
	inline	effect_compiler&				set_stencil_ref			( u32 arg_0 ) { /* no source */ }
			effect_compiler&				set_alpha_blend			(
												s32					blend_enable,
												D3D11_BLEND			src_blend,
												D3D11_BLEND			dest_blend,
												D3D11_BLEND_OP		blend_op,
												D3D11_BLEND			src_alpha_blend,
												D3D11_BLEND			dest_alpha_blend,
												D3D11_BLEND_OP		blend_alpha_op
											);
			effect_compiler&				set_cull_mode			( D3D11_CULL_MODE mode );
	inline	effect_compiler&				set_alpha_to_coverage	( s32 arg_0 ) { /* no source */ }

			effect_compiler&				color_write_enable		( D3D11_COLOR_WRITE_ENABLE mode );

			effect_compiler&				set_fill_mode			( D3D11_FILL_MODE fill_mode );

	inline	effect_compiler&				def_sampler				( pcstr arg_0, sampler_state_descriptor& arg_1 ) { /* no source */ }
	inline	effect_compiler&				def_sampler				( pcstr arg_0, D3D11_TEXTURE_ADDRESS_MODE arg_1, D3D11_FILTER arg_2 ) { /* no source */ }
	inline	sampler_state_descriptor&		begin_sampler			( pcstr arg_0 ) { /* no source */ }
	inline	effect_compiler&				end_sampler				( ) { /* no source */ }

			effect_compiler&				set_texture				(
												pcstr				name,
												pcstr				physical_name,
												res_texture_ptr*	out_texture,
												bool				streamed,
												u32					num_last_mips_used
											);
	inline	effect_compiler&				set_texture				(
												pcstr				hlsl_name,
												shared_string		phisical_name,
												bool				streamed,
												u32					num_last_mips_used
											) { /* no source */ }
			effect_compiler&				set_texture				(
												pcstr			name,
												res_texture*	texture,
												bool			streamed,
												u32				arg_3
											);

			effect_compiler&				bind_constant			( shader_constant_binding const& binding );

											effect_compiler			(
												res_effect&								effect,
												resources::query_result_for_cook*		parent,
												bool									shaders_cache_mode,
												associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > >*	in_shader_sources
											);
											~effect_compiler		( );

	inline	const vectora< effect_compiler::shader_cache_info >	get_cached_shaders_info	( ) const { /* no source */ }
private:
	inline	u32								get_sampler				( pcstr arg_0 ) { /* no source */ }

	inline	void							set_samp_texture		( u32 arg_0, pcstr arg_1 ) { /* no source */ }
	inline	void							set_samp_address		( u32 arg_0, u32 arg_1 ) { /* no source */ }
	inline	void							set_samp_filter_min		( u32 arg_0, u32 arg_1 ) { /* no source */ }
	inline	void							set_samp_filter_mip		( u32 arg_0, u32 arg_1 ) { /* no source */ }
	inline	void							set_samp_filter_mag		( u32 arg_0, u32 arg_1 ) { /* no source */ }
	inline	void							set_samp_filter			( u32 arg_0, D3D11_FILTER arg_1 ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
public:
	/* 0x0000 */	associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > >*	m_shader_sources;
	/* 0x0004 */	resources::query_result_for_cook*		m_parent_query;
private:
	/* 0x0008 */	vectora< effect_compiler::shader_cache_info >	m_shader_cache_info;
public:
	/* 0x0018 */	vector< texture_named_instance >		m_ps_used_textures;
	/* 0x0024 */	vector< effect_compiler::texture_query_desc >	m_textures_for_query;
private:
	/* 0x0030 */	res_xs_hw< ps_data >_ptr				m_ps_hw;
	/* 0x0034 */	res_xs_hw< gs_data >_ptr				m_gs_hw;
	/* 0x0038 */	res_xs_hw< vs_data >_ptr				m_vs_hw;
	/* 0x003c */	state_descriptor						m_state_descriptor;
	/* 0x01a8 */	sampler_state_descriptor				m_sampler_state_descriptor;
	/* 0x01e4 */	xs_descriptor< vs_data >				m_vs_descriptor;
	/* 0x3164 */	xs_descriptor< gs_data >				m_gs_descriptor;
	/* 0x60e0 */	xs_descriptor< ps_data >				m_ps_descriptor;
	/* 0x905c */	pcstr									m_curr_sampler_name;
	/* 0x9060 */	shader_constant_bindings				m_bindings;
	/* 0x906c */	res_shader_technique					m_sh_technique;
	/* 0x9084 */	res_effect&								m_compilation_target;
	/* 0x9088 */	u32										m_technique_idx;
	/* 0x908c */	u32										m_pass_idx;
	/* 0x9090 */	bool									m_shaders_cache_mode;
}; // class effect_compiler

STATIC_SIZE_ASSERT(effect_compiler, 0x9094);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COMPILER_H_INCLUDED
