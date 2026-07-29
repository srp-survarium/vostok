////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\RESOURCE_MANAGER_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\RESOURCE_MANAGER_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline bool resource_manager::shader_name_config_pair::operator<( resource_manager::shader_name_config_pair const& other ) const
{
	return false;

	// FUNCTION BODY[0x124800]: 2
	// <0x124800>|0x000|+0x029:'156'
	// <0x124829>|0x029|+0x018:'157'
	// <0x124841>|0x041|-0x001:'157'
	// <0x124840>|0x040|+0x006:'158'
	// ******
}

// STATE[STUB]
inline bool resource_manager::compare_predicate<vostok::render::res_signature>::operator()( res_signature const* const left, res_signature const* const right ) const
{
	return false;

	// FUNCTION BODY[0x126aa0]: 1
	// <0x126aa0>|0x000|+0x017:'429'
	// <0x126ab7>|0x017|-0x001:'429'
	// <0x126ab6>|0x016|+0x00e:'430'
	// ******
}

// STATE[STUB]
inline bool resource_manager::compare_member_predicate<vostok::render::res_geometry>::operator()( res_geometry const* const left, res_geometry const* const right ) const
{
	return false;

	// FUNCTION BODY[0x126b70]: 1
	// <0x126b70>|0x000|+0x019:'437'
	// <0x126b89>|0x019|+0x012:'437'
	// <0x126b9b>|0x02b|-0x013:'437'
	// <0x126b88>|0x018|+0x012:'438'
	// <0x126b9a>|0x02a|+0x031:'438'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	std::pair< vostok::fixed_string< 64 >, ID3D11SamplerState* >*
	// 	iterator_type;

	// typedef
	// 	u32*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::fs_new::virtual_path_string*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::shader_cache_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11BlendState, D3D11_BLEND_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\RESOURCE_MANAGER_H_INCLUDED
