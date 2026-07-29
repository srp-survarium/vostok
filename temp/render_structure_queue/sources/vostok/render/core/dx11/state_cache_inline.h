////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\STATE_CACHE_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\STATE_CACHE_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline ID3D11RasterizerState* state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>::get_state( D3D11_RASTERIZER_DESC const& desc )
{
	// LOCALS
	// state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >::state_record rec
	// ******

	return NULL;

	// FUNCTION BODY[0x1250c0]: 18
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x1250cc>|0x00c|+0x007:'34'
	// <0>
	// <0x1250d3>|0x013|+0x007:'36'
	// <0>
	// <0x1250da>|0x01a|+0x004:'38'
	// <0>
	// <1>
	// <2>
	// <0x1250de>|0x01e|+0x042:'42'
	// <0>
	// <0x125120>|0x060|+0x028:'44'
	// <0x125148>|0x088|-0x00b:'44'
	// <0>
	// <1>
	// <0x12513d>|0x07d|+0x017:'47'
	// <0x125154>|0x094|-0x015:'47'
	// <0x12513f>|0x07f|+0x017:'48'
	// ******
}

// STATE[STUB]
inline ID3D11RasterizerState* state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>::find( D3D11_RASTERIZER_DESC const& desc, u32 CRC )
{
	// LOCALS
	// D3D11_RASTERIZER_DESC 			desc_candidate
	// ******

	// CALL SITE INFO
	// <0x127186> -> void < unknown >( D3D11_RASTERIZER_DESC* )
	// ******

	return NULL;

	// FUNCTION BODY[0x127150]: 32
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x127158>|0x008|+0x097:'57'
	// <0x1271ef>|0x09f|-0x07d:'57'
	// <0>
	// <0x127172>|0x022|+0x005:'59'
	// <0>
	// <1>
	// <0x127177>|0x027|+0x011:'62'
	// <0x127188>|0x038|+0x085:'63'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x12720d>|0x0bd|+0x005:'75'
	// <0x127212>|0x0c2|-0x010:'76'
	// <0>
	// <0x127202>|0x0b2|+0x002:'78'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x127204>|0x0b4|+0x014:'85'
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

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\STATE_CACHE_INLINE_H_INCLUDED
