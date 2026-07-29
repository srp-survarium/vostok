////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\BACKEND_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\BACKEND_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void backend::set_vb( untyped_buffer* vb, u32 vb_stride, u32 offset )
{
	// FUNCTION BODY[0x645b0]: 5
	// <0x645b0>|0x000|+0x028:'33'
	// <0>
	// <0x645d8>|0x028|+0x006:'35'
	// <0x645de>|0x02e|+0x006:'36'
	// <0x645e4>|0x034|+0x00a:'37'
	// ******
}

// STATE[STUB]
inline void backend::reset_depth_stencil_target( )
{
	// FUNCTION BODY[0x645f0]: 2
	// <0x645f0>|0x000|+0x00c:'257'
	// <0x645fc>|0x00c|+0x00f:'258'
	// ******
}

// STATE[STUB]
inline void backend::flush_rt_shader_resources( )
{
	// LOCALS
	// ID3D11ShaderResourceView*[12] 	rv
	// ******

	// CALL SITE INFO
	// <0x6465f> -> void < unknown >( u32, u32, ID3D11ShaderResourceView* const* )
	// <0x6467b> -> void < unknown >( u32, u32, ID3D11ShaderResourceView* const* )
	// <0x64696> -> void < unknown >( u32, u32, ID3D11ShaderResourceView* const* )
	// ******

	// FUNCTION BODY[0x64610]: 9
	// <0>
	// <1>
	// <0x64613>|0x003|+0x04e:'265'
	// <0x64661>|0x051|+0x01c:'266'
	// <0x6467d>|0x06d|+0x01b:'267'
	// <0>
	// <0x64698>|0x088|+0x01a:'269'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
inline void backend::flush_rt_views( )
{
	// LOCALS
	// ID3D11RenderTargetView*[8] 		tv
	// ******

	// CALL SITE INFO
	// <0x646f6> -> void < unknown >( u32, ID3D11RenderTargetView* const*, ID3D11DepthStencilView* )
	// ******

	// FUNCTION BODY[0x646c0]: 4
	// <0>
	// <1>
	// <2>
	// <0x646c9>|0x009|+0x02f:'279'
	// ******
}

// STATE[STUB]
inline void backend::set_render_output( res_render_output const* render_output )
{
	// FUNCTION BODY[0x64700]: 4
	// <0x64700>|0x000|+0x02a:'307'
	// <0>
	// <0x6472a>|0x02a|+0x01a:'309'
	// <0x64744>|0x044|+0x012:'310'
	// <0x64756>|0x056|-0x002:'310'
	// <0x64754>|0x054|+0x00a:'311'
	// ******
}

// STATE[STUB]
inline void backend::reset( )
{
	// CALL SITE INFO
	// <0x64c0d> -> void < unknown >( u32, ID3D11RenderTargetView* const*, ID3D11DepthStencilView* )
	// <0x64c2d> -> void < unknown >( D3D_PRIMITIVE_TOPOLOGY )
	// ******

	// FUNCTION BODY[0x64760]: 38
	// <0x64764>|0x004|+0x012:'418'
	// <0x64776>|0x016|+0x020:'419'
	// <0>
	// <0x64796>|0x036|+0x02a:'421'
	// <0x647c0>|0x060|+0x00f:'422'
	// <0>
	// <0x647cf>|0x06f|+0x00f:'424'
	// <0x647de>|0x07e|+0x015:'425'
	// <0x647f3>|0x093|+0x03e:'426'
	// <0x64831>|0x0d1|+0x02b:'427'
	// <0x6485c>|0x0fc|+0x03f:'428'
	// <0x6489b>|0x13b|+0x06d:'429'
	// <0>
	// <0x64908>|0x1a8|+0x015:'431'
	// <0x6491d>|0x1bd|+0x028:'432'
	// <0x64945>|0x1e5|+0x03c:'433'
	// <0x64981>|0x221|+0x06a:'434'
	// <0>
	// <0x649eb>|0x28b|+0x01a:'436'
	// <0x64a05>|0x2a5|+0x02b:'437'
	// <0x64a30>|0x2d0|+0x03f:'438'
	// <0x64a6f>|0x30f|+0x06d:'439'
	// <0>
	// <0x64adc>|0x37c|+0x02b:'441'
	// <0x64b07>|0x3a7|+0x036:'442'
	// <0x64b3d>|0x3dd|+0x028:'443'
	// <0>
	// <1>
	// <0x64b65>|0x405|+0x054:'446'
	// <0x64bb9>|0x459|+0x006:'447'
	// <0>
	// <0x64bbf>|0x45f|+0x015:'449'
	// <0x64bd4>|0x474|+0x03b:'450'
	// <0>
	// <0x64c0f>|0x4af|+0x006:'452'
	// <0>
	// <1>
	// <0x64c15>|0x4b5|+0x01a:'455'
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
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::shader_cache_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
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
	// 	vostok::render::stage**
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// typedef
	// 	vostok::render::ui::vertex*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\BACKEND_INLINE_H_INCLUDED
