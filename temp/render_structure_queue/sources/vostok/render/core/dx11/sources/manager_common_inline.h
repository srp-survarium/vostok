////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\SOURCES\MANAGER_COMMON_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\SOURCES\MANAGER_COMMON_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline bool reclaim<vostok::render::res_state>( vector< res_state* >& vec, res_state const* ptr )
{
	return false;

	// FUNCTION BODY[0x1266f0]: 13
	// <0x1266f1>|0x001|+0x002:'16'
	// <0x1266f3>|0x003|+0x003:'17'
	// <0>
	// <0x1266f6>|0x006|+0x00a:'19'
	// <0>
	// <0x126700>|0x010|+0x00f:'21'
	// <0>
	// <0x12670f>|0x01f|+0x022:'23'
	// <0x126731>|0x041|-0x026:'24'
	// <0>
	// <1>
	// <2>
	// <0x12670b>|0x01b|+0x002:'28'
	// <0x12670d>|0x01d|+0x026:'29'
	// ******
}

// STATE[STUB]
inline bool reclaim<vostok::render::shader_constant_table,vostok::render::resource_manager::constant_table_predicate>(
	set< shader_constant_table*, resource_manager::constant_table_predicate >&	container,
	shader_constant_table const*	ptr
)
{
	return false;

	// FUNCTION BODY[0x1265e0]: 7
	// <0x1265e0>|0x000|+0x00a:'34'
	// <0x1265ea>|0x00a|+0x004:'35'
	// <0x1265ee>|0x00e|+0x030:'36'
	// <0x12661e>|0x03e|+0x003:'37'
	// <0>
	// <1>
	// <0x126621>|0x041|-0x001:'40'
	// <0x126620>|0x040|+0x003:'41'
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

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\SOURCES\MANAGER_COMMON_INLINE_H_INCLUDED
