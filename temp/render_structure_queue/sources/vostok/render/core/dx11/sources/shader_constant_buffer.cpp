////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\shader_constant_buffer.h"

namespace vostok {
namespace render {

// STATE[STUB]
 shader_constant_buffer::shader_constant_buffer(
	fixed_string< 64 > const&		name,
	enum_shader_type				dest,
	_D3D_CBUFFER_TYPE				type,
	u32								size
)
{
	// LOCALS
	// D3D11_BUFFER_DESC 				desc
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266ec>;
	// ******

	// CALL SITE INFO
	// <0x739055> -> HRESULT < unknown >( D3D11_BUFFER_DESC const*, D3D11_SUBRESOURCE_DATA const*, ID3D11Buffer** )
	// ******

	// FUNCTION BODY[0x738fd0]: 22
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
	// <11>
	// <12>
	// <13>
	// <0x739008>|0x038|+0x04f:'40'
	// <0x739057>|0x087|+0x04d:'41'
	// <0>
	// <1>
	// <0x7390a4>|0x0d4|+0x00f:'44'
	// <0>
	// <1>
	// <0x7390b3>|0x0e3|+0x011:'47'
	// ******
}

// STATE[STUB]
void shader_constant_buffer::destroy_impl( ) const
{
	// FUNCTION BODY[0x738fc0]: 1
	// <0x738fc0>|0x000|+0x00c:'53'
	// ******
}

// STATE[STUB]
 shader_constant_buffer::~shader_constant_buffer( )
{
	// FUNCTION BODY[0x738f80]: 3
	// <0>
	// <0x738f80>|0x000|+0x016:'59'
	// <0x738f96>|0x016|+0x022:'60'
	// ******
}

// STATE[STUB]
void shader_constant_buffer::update( )
{
	// CALL SITE INFO
	// <0x738f76> -> void < unknown >( ID3D11Resource*, u32, D3D11_BOX const*, pcvoid, u32, u32 )
	// ******

	// FUNCTION BODY[0x738f40]: 36
	// <0x738f40>|0x000|+0x006:'70'
	// <0>
	// <0x738f46>|0x006|+0x00b:'72'
	// <0x738f51>|0x011|+0x027:'73'
	// <0>
	// <0x738f78>|0x038|+0x004:'75'
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
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
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

	// ******

} // namespace render
} // namespace vostok
