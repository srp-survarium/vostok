////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\BACKEND_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\BACKEND_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void backend::set_ps_constant<float>( shader_constant_host const* c, float const& arg )
{
	// FUNCTION BODY[0x672c0]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x672c0>|0x000|+0x011:'391'
	// <0x672d1>|0x011|+0x003:'392'
	// <0>
	// ******
}

// STATE[STUB]
inline void backend::get_viewport( D3D11_VIEWPORT& viewport )
{
	// LOCALS
	// u32 								count
	// ******

	// CALL SITE INFO
	// <0x668d3> -> void < unknown >( u32*, D3D11_VIEWPORT* )
	// ******

	// FUNCTION BODY[0x668b0]: 2
	// <0>
	// <0x668b1>|0x001|+0x024:'489'
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
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\BACKEND_INLINE_H_INCLUDED
