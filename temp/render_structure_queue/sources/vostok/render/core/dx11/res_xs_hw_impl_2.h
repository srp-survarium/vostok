////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\RES_XS_HW_IMPL_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\RES_XS_HW_IMPL_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline  res_xs_hw<vostok::render::ps_data>::res_xs_hw<vostok::render::ps_data>( )
{
	// FUNCTION BODY[0x12fd40]: 1
	// <0>
	// ******
}

// STATE[STUB]
inline  res_xs_hw<vostok::render::ps_data>::~res_xs_hw<vostok::render::ps_data>( )
{
	// CALL SITE INFO
	// <0x12fd9e> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x12fd90]: 1
	// <0x12fd90>|0x000|+0x017:'27'
	// ******
}

// STATE[STUB]
inline HRESULT res_xs_hw<vostok::render::ps_data>::create_hw_shader( ID3D10Blob* shader_code )
{
	// LOCALS
	// u32 								buffer_size
	// ID3D11ShaderReflection* 			shader_reflection
	// _D3D11_SHADER_DESC 				shader_desc
	// ******

	// CALL SITE INFO
	// <0x13020c> -> void* < unknown >()
	// <0x130216> -> unsigned long < unknown >()
	// <0x130222> -> void* < unknown >()
	// <0x13022c> -> unsigned long < unknown >()
	// <0x130248> -> HRESULT < unknown >( pcvoid, unsigned long, ID3D11ClassLinkage*, ID3D11PixelShader** )
	// <0x130278> -> HRESULT < unknown >( _D3D11_SHADER_DESC* )
	// ******

	// FUNCTION BODY[0x1301f0]: 47
	// <0>
	// <1>
	// <0x1301fb>|0x00b|+0x013:'55'
	// <0x13020e>|0x01e|+0x00a:'56'
	// <0>
	// <0x130218>|0x028|+0x034:'58'
	// <0>
	// <0x13024c>|0x05c|+0x006:'60'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x130252>|0x062|+0x017:'66'
	// <0>
	// <1>
	// <2>
	// <0x130269>|0x079|+0x011:'70'
	// <0>
	// <1>
	// <2>
	// <0x13027a>|0x08a|+0x019:'74'
	// <0x130293>|0x0a3|+0x015:'75'
	// <0>
	// <0x1302a8>|0x0b8|+0x021:'77'
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
	// <0x1302c9>|0x0d9|-0x00d:'96'
	// <0>
	// <1>
	// <0x1302bc>|0x0cc|+0x002:'99'
	// <0x1302be>|0x0ce|+0x0ca:'100'
	// ******
}

// STATE[STUB]
inline void res_xs_hw<vostok::render::ps_data>::parse_resources(
	ID3D11ShaderReflection*					shader_reflection,
	fixed_vector< sampler_slot, 16 >&		samplers,
	fixed_vector< texture_slot, 128 >&		textures
)
{
	// LOCALS
	// _D3D11_SHADER_DESC 				shader_desc
	// u32 								i
	// _D3D11_SHADER_INPUT_BIND_DESC 	ResDesc
	// texture_slot 					tex_slot
	// sampler_slot 					smp
	// ******

	// CALL SITE INFO
	// <0x12ffd6> -> HRESULT < unknown >( _D3D11_SHADER_DESC* )
	// <0x130003> -> HRESULT < unknown >( u32, _D3D11_SHADER_INPUT_BIND_DESC* )
	// ******

	// FUNCTION BODY[0x12ffb0]: 48
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x12ffc0>|0x010|+0x018:'111'
	// <0>
	// <0x12ffd8>|0x028|+0x01f:'113'
	// <0>
	// <1>
	// <0x12fff7>|0x047|+0x00e:'116'
	// <0>
	// <1>
	// <2>
	// <0x130005>|0x055|+0x00d:'120'
	// <0>
	// <0x130012>|0x062|+0x00a:'122'
	// <0x13001c>|0x06c|+0x050:'123'
	// <0>
	// <1>
	// <0x13006c>|0x0bc|+0x031:'126'
	// <0>
	// <1>
	// <0x13009d>|0x0ed|+0x014:'129'
	// <0x1300b1>|0x101|+0x024:'130'
	// <0x1300d5>|0x125|+0x009:'131'
	// <0>
	// <0x1300de>|0x12e|+0x00a:'133'
	// <0x1300e8>|0x138|+0x054:'134'
	// <0x13013c>|0x18c|+0x004:'135'
	// <0x130140>|0x190|+0x010:'136'
	// <0>
	// <1>
	// <2>
	// <0x130150>|0x1a0|+0x034:'140'
	// <0>
	// <1>
	// <0x130184>|0x1d4|+0x05c:'143'
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

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\RES_XS_HW_IMPL_H_INCLUDED
