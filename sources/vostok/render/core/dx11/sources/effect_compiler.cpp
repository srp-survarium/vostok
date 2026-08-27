#include "pch.h"

#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/dx11/res_state.h>
#include <vostok/render/core/res_xs.h>

namespace vostok {
namespace render {

static command_line::key s_no_effect_result( "no_effect_result", "", "", "" );
static command_line::key s_one_texture_result( "one_texture", "", "", "" );

effect_compiler::effect_compiler(
	res_effect& effect,
	resources::query_result_for_cook* parent,
	bool shaders_cache_mode,
	binary_shader_sources_type* in_shader_sources
) :
	m_shader_sources( in_shader_sources ),
	m_parent_query( parent ),
	m_shader_cache_info( memory::g_mt_allocator ),
	m_sampler_state_descriptor( *this ),
	m_curr_sampler_name( 0 ),
	m_compilation_target( effect ),
	m_technique_idx( 0 ),
	m_pass_idx( 0 ),
	m_shaders_cache_mode( shaders_cache_mode )
{
}

effect_compiler::~effect_compiler( )
{
}

effect_compiler& effect_compiler::set_depth(
	bool enable,
	bool write_enable,
	D3D11_COMPARISON_FUNC cmp_func
)
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_state_descriptor.set_depth	( enable, write_enable, cmp_func);

	return *this;
}

effect_compiler& effect_compiler::set_stencil(
	s32 enable,
	u32 ref,
	u8 read_mask,
	u8 write_mask,
	D3D11_COMPARISON_FUNC func,
	D3D11_STENCIL_OP fail,
	D3D11_STENCIL_OP pass,
	D3D11_STENCIL_OP zfail
)
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_state_descriptor.set_stencil	( enable, ref, read_mask, write_mask);
	m_state_descriptor.set_stencil_frontface	( func, fail, pass, zfail);
	m_state_descriptor.set_stencil_backface	( func , fail, pass, zfail);

	return *this;
}

effect_compiler& effect_compiler::set_alpha_blend(
	s32 blend_enable,
	D3D11_BLEND src_blend,
	D3D11_BLEND dest_blend,
	D3D11_BLEND_OP blend_op,
	D3D11_BLEND src_alpha_blend,
	D3D11_BLEND dest_alpha_blend,
	D3D11_BLEND_OP blend_alpha_op
)
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_state_descriptor.set_alpha_blend(
		blend_enable,
		src_blend,
		dest_blend,
		blend_op,
		src_alpha_blend,
		dest_alpha_blend,
		blend_alpha_op
	);

	return *this;
}

effect_compiler& effect_compiler::set_texture(
	pcstr				name,
	pcstr				physical_name,
	res_texture_ptr*	out_texture,
	bool				streamed,
	u32					num_last_mips_used
)
{
	if (
		!physical_name ||
		!strcmp( physical_name, "") ||
		(
			s_one_texture_result.is_set() &&
			!strstr( physical_name, "$user$") &&
			!strstr( physical_name, "ui/")
		)
	)
		physical_name	= "editor/default";

	fixed_string<512>	s	= physical_name;

	if ( !options::ref().current.m_use_texture_streaming)
		num_last_mips_used	= u32(-1);

	fs_new::virtual_path_string	physical_name_lower_case	= physical_name;
	physical_name_lower_case.make_lowercase();

	if (m_shaders_cache_mode || s_no_effect_result)
	{
		texture_query_desc	desc;
		desc.m_query_physicaly_path		= physical_name_lower_case.c_str();
		desc.m_num_last_mips_used		= num_last_mips_used;

		m_textures_for_query.push_back	( desc);

		return *this;
	}

	bool	res		= false;

	if ( m_vs_hw)
		res		= m_vs_descriptor.use_texture	( name);

	if ( m_gs_hw)
		res		|= m_gs_descriptor.use_texture	( name);

	bool	used_in_ps	= false;

	if ( m_ps_hw)
	{
		used_in_ps	= m_ps_descriptor.use_texture	( name);
		res			|= used_in_ps;
	}

	if ( !res)
		return *this;

	res_texture*	texture	= resource_manager::ref().create_texture(
		physical_name_lower_case.c_str(),
		0,
		0,
		false,
		true,
		true,
		num_last_mips_used
	);

	if ( used_in_ps && streamed && options::ref().current.m_use_texture_streaming)
	{
		texture_named_instance	instance;

		instance.texture	= texture;
		instance.path		= physical_name;
		m_ps_used_textures.push_back	( instance);
	}

	if ( out_texture)
		*out_texture	= texture;

	return set_texture( name, texture, streamed, num_last_mips_used);
}

effect_compiler& effect_compiler::set_texture(
	pcstr			name,
	res_texture*	texture,
	bool			streamed,
	u32
)
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	u32		num_last_mips_used	= u32(-1);

	if ( m_vs_hw)
		m_vs_descriptor.set_texture	( name, texture);

	if ( m_gs_hw)
		m_gs_descriptor.set_texture	( name, texture);

	if ( m_ps_hw)
		m_ps_descriptor.set_texture	( name, texture);

	return *this;
}

effect_compiler& effect_compiler::set_cull_mode( D3D11_CULL_MODE mode )
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_state_descriptor.set_cull_mode( mode);
	return  *this;
}

effect_compiler& effect_compiler::color_write_enable( D3D11_COLOR_WRITE_ENABLE mode )
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_state_descriptor.color_write_enable( mode);
	return *this;
}

effect_compiler& effect_compiler::set_fill_mode( D3D11_FILL_MODE fill_mode )
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_state_descriptor.set_fill_mode	( fill_mode);
	return *this;
}

effect_compiler& effect_compiler::bind_constant( shader_constant_binding const& binding )
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_bindings.add( binding);
	return *this;
}

effect_compiler& effect_compiler::begin_pass(
	pcstr vs_name,
	pcstr gs_name,
	pcstr ps_name,
	shader_configuration const& shader_config,
	shader_include_getter* include_getter
)
{
	if (m_shaders_cache_mode || s_no_effect_result)
	{
		shader_cache_info				info;
		info.vertex_shader_name			= vs_name;
		info.geometry_shader_name		= gs_name;
		info.pixel_shader_name			= ps_name;
		info.configuration				= shader_config;
		m_shader_cache_info.push_back	(info);

		return *this;
	}

	m_state_descriptor.reset();
	m_bindings.clear();

	m_vs_hw = resource_manager::ref().create_vs_hw( vs_name, shader_config, include_getter, m_shader_sources);
	m_gs_hw = resource_manager::ref().create_gs_hw( gs_name, shader_config, include_getter, m_shader_sources);
	m_ps_hw = resource_manager::ref().create_ps_hw( ps_name, shader_config, include_getter, m_shader_sources);

	if (m_vs_hw)
		m_vs_descriptor.reset( &*m_vs_hw);
	else
		m_vs_descriptor.reset( NULL );

	if (m_gs_hw)
		m_gs_descriptor.reset( &*m_gs_hw);
	else
		m_gs_descriptor.reset( NULL );

	if (m_ps_hw)
		m_ps_descriptor.reset( &*m_ps_hw);
	else
		m_ps_descriptor.reset( NULL );

	set_depth		( true, true);
	set_alpha_blend	( false, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);

	return *this;
}

effect_compiler& effect_compiler::end_pass( )
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return *this;

	m_vs_descriptor.data().constants.apply_bindings(m_bindings);
	m_gs_descriptor.data().constants.apply_bindings(m_bindings);
	m_ps_descriptor.data().constants.apply_bindings(m_bindings);

	res_state_ptr	state	= resource_manager::ref().create_state	( m_state_descriptor);

	res_vs_ptr		vs		= resource_manager::ref().create_vs (m_vs_descriptor);
	res_gs_ptr		gs		= resource_manager::ref().create_gs (m_gs_descriptor);
	res_ps_ptr		ps		= resource_manager::ref().create_ps (m_ps_descriptor);

//	shader_constant_table_ptr constants= resource_manager::ref().create_const_table( m_constants);
//	ref_texture_list tex_lst= effect_manager::ref().create_texture_list( m_tex_lst);

	res_pass_ptr pass = effect_manager::ref().create_pass(res_pass(vs, gs, ps, state));

	m_sh_technique.m_passes.push_back(pass);

	m_state_descriptor.reset();
//	m_constants.clear();
//	m_tex_lst.clear();
	m_bindings.clear();

	m_vs_hw = 0;
	m_gs_hw = 0;
	m_ps_hw = 0;

	++m_pass_idx;

	return *this;
}

effect_compiler& effect_compiler::begin_technique( )
{
	if (m_shaders_cache_mode)
		return *this;

	m_sh_technique.m_passes.clear();
	m_pass_idx = 0;

	return *this;
}

void effect_compiler::end_technique( )
{
	if (m_shaders_cache_mode || s_no_effect_result)
		return;

	res_shader_technique_ptr se = effect_manager::ref().create_effect_technique( m_sh_technique);

	m_compilation_target.m_techniques.push_back( se);

	m_sh_technique.m_passes.clear();

	++m_technique_idx; m_pass_idx = 0;
}

} // namespace render
} // namespace vostok
