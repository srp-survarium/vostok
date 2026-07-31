////////////////////////////////////////////////////////////////////////////
//	Created		: 13.02.2009
//	Author		: Mykhailo Parfeniuk
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/effect_compiler.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/effect_constant_storage.h>

namespace vostok {
namespace render {

// REMAINDER: set_stencil_ref/set_alpha_to_coverage (canonical header inlines,
// already faithful), def_sampler/begin_sampler/end_sampler chain + set_texture
// x2 + begin_pass (need res_sampler/xs_descriptor shader-source wiring -
// port with the shader_binary_source_cook pass).




// 

effect_compiler& effect_compiler::set_stencil_ref	( u32 ref)
{
	if (m_shaders_cache_mode)
		return *this;
	
	m_state_descriptor.set_stencil_ref	( ref);

	return *this;
}


effect_compiler& effect_compiler::set_alpha_to_coverage ( BOOL is_enabled )
{
	if (m_shaders_cache_mode)
		return *this;

	m_state_descriptor.set_alpha_to_coverage	( is_enabled );
	
	return *this;	
}

effect_compiler& effect_compiler::def_sampler		( char const * name, sampler_state_descriptor & sampler_desc)
{
	if (m_shaders_cache_mode)
		return *this;

	if( resource_manager::ref().find_registered_sampler( name) )
	{
		ASSERT( 0, "A sampler with name \"%s\" was already registered globally.", name);
		return *this;
	}

	res_sampler_state* sampler  =  resource_manager::ref().create_sampler_state( sampler_desc);

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

effect_compiler& effect_compiler::def_sampler		( char const * name, D3D_TEXTURE_ADDRESS_MODE address , D3D_FILTER filter)
{
	if (m_shaders_cache_mode)
		return *this;

	m_sampler_state_descriptor.reset()
								.set_address_mode	( address, address, address)
								.set_filter		( filter);

	def_sampler( name, m_sampler_state_descriptor);

	return *this;
}

sampler_state_descriptor & effect_compiler::begin_sampler	( char const * name)
{
	if (m_shaders_cache_mode)
		return m_sampler_state_descriptor;

	m_sampler_state_descriptor.reset();
	m_curr_sampler_name = name;
	return m_sampler_state_descriptor;
}

effect_compiler			& effect_compiler::end_sampler		()
{
	if (m_shaders_cache_mode)
		return *this;

	ASSERT( m_curr_sampler_name, "There was no call of \"begin_sampler\" function!");

	if( !m_curr_sampler_name)
		return *this;

	def_sampler( m_curr_sampler_name, m_sampler_state_descriptor);
	m_curr_sampler_name = NULL;
	return *this;
}

effect_compiler& effect_compiler::set_texture		( char const * name, char const * physical_name, ref_texture* out_texture)
{
	if (m_shaders_cache_mode)
		return *this;
	
	if (physical_name == 0 || strings::equal(physical_name, ""))
		physical_name = "editor/default";
	
	bool res = false;
	
	if( m_vs_hw)
		res |= m_vs_descriptor.use_texture( name);
	
	if( m_gs_hw)
		res |= m_gs_descriptor.use_texture( name);
	
	if( m_ps_hw)
		res |= m_ps_descriptor.use_texture( name);
	
	if( !res)
	{
		//LOG_INFO( "The texture \"%s\" was specified in effect descriptor, but not used by any of effect shaders.", name);
		return *this;
	}
	
	fs_new::virtual_path_string	physical_name_lower_case	=	physical_name;
	physical_name_lower_case.make_lowercase			();
	res_texture* ref = ( resource_manager::ref().create_texture( physical_name_lower_case.c_str()/*, true*/));
// 	
// 	vostok::fixed_string<64> constant_name_min, constant_name_max;
// 	
// 	constant_name_min.assignf("min_%s", name);
// 	constant_name_max.assignf("max_%s", name);
// 	
// 	bind_constant( shader_constant_binding(constant_name_min.c_str(), &ref->m_rescale_min));
// 	bind_constant( shader_constant_binding(constant_name_max.c_str(), &ref->m_rescale_max));
// 	
	if (out_texture)
		*out_texture = 0;//ref;
	
	return set_texture( name, &*ref);
}

effect_compiler& effect_compiler::set_texture		( char const * name, res_texture * texture)
{
	if (m_shaders_cache_mode)
		return *this;
	
	bool res = false;
	
	if( m_vs_hw)
		res |= m_vs_descriptor.set_texture( name, texture);
	
	if( m_gs_hw)
		res |= m_gs_descriptor.set_texture( name, texture);
	
	if( m_ps_hw)
		res |= m_ps_descriptor.set_texture( name, texture);
	
	//if( !res)
	//	LOG_INFO( "The texture \"%s\" was specified in effect descriptor, but not used by any of effect shaders.", name);

	return *this;
}

// effect_compiler& effect_compiler::def_sampler_clf( char const * name, shared_string texture)
// {
// 	return def_sampler( name, texture, D3D_TEXTURE_ADDRESS_CLAMP, D3D_FILTER_MIN_MAG_LINEAR_MIP_POINT);
// }
// 
// effect_compiler& effect_compiler::def_sampler_rtf( char const * name, shared_string texture)
// {
// 	return def_sampler( name,texture, D3D_TEXTURE_ADDRESS_CLAMP, D3D_FILTER_MIN_MAG_MIP_POINT);
// }
// 
// effect_compiler& effect_compiler::def_sampler_clw( char const * name, shared_string texture)
// {
// 	u32 s = get_sampler( name);
// 	
// 	if ( u32( -1) != s)
// 	{
// 		def_sampler( name, texture, D3D_TEXTURE_ADDRESS_CLAMP, D3D_FILTER_MIN_MAG_LINEAR_MIP_POINT);
// 	}
// 
// 	return *this;
// }





effect_compiler& effect_compiler::begin_pass(pcstr vs_name, 
											 pcstr gs_name, 
											 pcstr ps_name, 
											 shader_configuration const& shader_config, 
											 shader_include_getter* include_getter)
{
	if (m_shaders_cache_mode)
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
	set_alpha_blend	( false, D3D_BLEND_ONE, D3D_BLEND_ZERO);
	
	return *this;
}






} // namespace render 
} // namespace vostok 

