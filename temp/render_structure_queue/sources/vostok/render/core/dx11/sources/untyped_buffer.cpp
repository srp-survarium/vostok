////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\untyped_buffer.h"

namespace vostok {
namespace render {

// STATE[STUB]
 untyped_buffer::untyped_buffer(
	const u32			size,
	pcvoid const		data,
	enum_buffer_type	type,
	const bool			is_dynamic,
	bool				staging
)
{
	// LOCALS
	// D3D11_SUBRESOURCE_DATA 			init_data
	// D3D11_BUFFER_DESC 				desc
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266d0>;
	// ******

	// CALL SITE INFO
	// <0x55c235> -> HRESULT < unknown >( D3D11_BUFFER_DESC const*, D3D11_SUBRESOURCE_DATA const*, ID3D11Buffer** )
	// ******

	// FUNCTION BODY[0x55c1a0]: 25
	// <0>
	// <0x55c1b0>|0x010|+0x009:'20'
	// <0>
	// <0x55c1b9>|0x019|+0x007:'22'
	// <0x55c1c0>|0x020|+0x00f:'23'
	// <0x55c1cf>|0x02f|+0x016:'24'
	// <0x55c1e5>|0x045|+0x004:'25'
	// <0>
	// <0x55c1e9>|0x049|+0x006:'27'
	// <0>
	// <0x55c1ef>|0x04f|+0x008:'29'
	// <0x55c1f7>|0x057|+0x004:'30'
	// <0x55c1fb>|0x05b|+0x008:'31'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x55c203>|0x063|+0x034:'41'
	// <0x55c237>|0x097|+0x04d:'42'
	// <0>
	// ******
}

// STATE[STUB]
 untyped_buffer::~untyped_buffer( )
{
	// CALL SITE INFO
	// <0x55c17d> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x55c170]: 1
	// <0x55c170>|0x000|+0x016:'48'
	// ******
}

// STATE[STUB]
void untyped_buffer::destroy_impl( ) const
{
	// FUNCTION BODY[0x55c190]: 1
	// <0x55c190>|0x000|+0x00b:'53'
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
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
