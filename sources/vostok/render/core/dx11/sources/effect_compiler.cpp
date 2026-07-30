#include "pch.h"

#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

effect_compiler::effect_compiler(
	res_effect& effect,
	resources::query_result_for_cook* parent,
	bool shaders_cache_mode,
	binary_shader_sources_type* in_shader_sources
) :
	m_shader_sources( in_shader_sources ),
	m_parent_query( parent ),
	m_shader_cache_info( g_allocator ),
	m_curr_sampler_name( 0 ),
	m_compilation_target( effect ),
	m_technique_idx( 0 ),
	m_pass_idx( 0 ),
	m_shaders_cache_mode( shaders_cache_mode )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6710]
}

effect_compiler::~effect_compiler( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6570]
}

effect_compiler& effect_compiler::set_depth(
	bool,
	bool,
	D3D11_COMPARISON_FUNC
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6020]
	return *this;
}

effect_compiler& effect_compiler::set_stencil(
	s32,
	u32,
	u8,
	u8,
	D3D11_COMPARISON_FUNC,
	D3D11_STENCIL_OP,
	D3D11_STENCIL_OP,
	D3D11_STENCIL_OP
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5f80]
	return *this;
}

effect_compiler& effect_compiler::set_alpha_blend(
	s32,
	D3D11_BLEND,
	D3D11_BLEND,
	D3D11_BLEND_OP,
	D3D11_BLEND,
	D3D11_BLEND,
	D3D11_BLEND_OP
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5f10]
	return *this;
}

effect_compiler& effect_compiler::set_texture(
	pcstr,
	pcstr,
	res_texture_ptr* out_texture,
	bool,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6c70]
	if ( out_texture )
		*out_texture = res_texture_ptr( );
	return *this;
}

effect_compiler& effect_compiler::set_texture(
	pcstr,
	res_texture*,
	bool,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5e60]
	return *this;
}

effect_compiler& effect_compiler::set_cull_mode( D3D11_CULL_MODE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5e10]
	return *this;
}

effect_compiler& effect_compiler::color_write_enable( D3D11_COLOR_WRITE_ENABLE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5dc0]
	return *this;
}

effect_compiler& effect_compiler::set_fill_mode( D3D11_FILL_MODE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a5d70]
	return *this;
}

effect_compiler& effect_compiler::bind_constant( shader_constant_binding const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6090]
	return *this;
}

effect_compiler& effect_compiler::begin_pass(
	pcstr,
	pcstr,
	pcstr,
	shader_configuration const&,
	shader_include_getter*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6100]
	return *this;
}

effect_compiler& effect_compiler::end_pass( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a68f0]
	return *this;
}

effect_compiler& effect_compiler::begin_technique( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a66c0]
	return *this;
}

void effect_compiler::end_technique( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7a6b70]
}

} // namespace render
} // namespace vostok
