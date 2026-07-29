////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\res_texture.h"

namespace vostok {
namespace render {

// STATE[STUB]
explicit res_texture::res_texture( bool pool_texture )
{
	// FUNCTION BODY[0x55c5b0]: 12
	// <0x55c628>|0x078|+0x03a:'47'
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

// STATE[STUB]
 res_texture::~res_texture( )
{
	// CALL SITE INFO
	// <0x55c449> -> unsigned long < unknown >()
	// <0x55c465> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x55c430]: 5
	// <0x55c439>|0x009|+0x01c:'63'
	// <0x55c455>|0x025|+0x01d:'64'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void res_texture::destroy_impl( ) const
{
	// FUNCTION BODY[0x55c670]: 1
	// <0x55c671>|0x001|+0x040:'72'
	// ******
}

// STATE[STUB]
void res_texture::save_as( pcstr file_name )
{
	// FUNCTION BODY[0x55c300]: 16
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
	// ******
}

// STATE[STUB]
void res_texture::desc_update( )
{
	// LOCALS
	// D3D11_RESOURCE_DIMENSION 		type
	// ******

	// CALL SITE INFO
	// <0x55c2bb> -> void < unknown >( D3D11_RESOURCE_DIMENSION* )
	// <0x55c2d3> -> void < unknown >( D3D11_TEXTURE2D_DESC* )
	// <0x55c2f2> -> void < unknown >( D3D11_TEXTURE3D_DESC* )
	// ******

	// FUNCTION BODY[0x55c2a0]: 18
	// <0x55c2a1>|0x001|+0x00c:'97'
	// <0x55c2ad>|0x00d|+0x004:'98'
	// <0>
	// <1>
	// <0x55c2b1>|0x011|+0x00c:'101'
	// <0x55c2bd>|0x01d|+0x006:'102'
	// <0>
	// <0x55c2c3>|0x023|+0x006:'104'
	// <0x55c2c9>|0x029|+0x00c:'105'
	// <0x55c2d5>|0x035|+0x007:'106'
	// <0>
	// <0x55c2dc>|0x03c|+0x006:'108'
	// <0>
	// <0x55c2e2>|0x042|+0x006:'110'
	// <0x55c2e8>|0x048|+0x00c:'111'
	// <0x55c2f4>|0x054|+0x007:'112'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void res_texture::set_hw_texture(
	ID3D11Resource*		surface,
	u32					mip_level_cut,
	bool				staging,
	bool				srgb,
	bool				depth_stencil
)
{
	// LOCALS
	// D3D11_SHADER_RESOURCE_VIEW_DESC 	view_desc
	// D3D11_RESOURCE_DIMENSION 		type
	// bool 							do_debug_break
	// bool 							do_debug_break
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266cd>;
	// static bool 						ignore_always = <0x4c266ce>;
	// static bool 						ignore_always = <0x4c266cf>;
	// ******

	// CALL SITE INFO
	// <0x55c6da> -> unsigned long < unknown >()
	// <0x55c6f6> -> unsigned long < unknown >()
	// <0x55c714> -> unsigned long < unknown >()
	// <0x55c748> -> void < unknown >( D3D11_RESOURCE_DIMENSION* )
	// <0x55c827> -> HRESULT < unknown >( ID3D11Resource*, D3D11_SHADER_RESOURCE_VIEW_DESC const*, ID3D11ShaderResourceView** )
	// <0x55c864> -> HRESULT < unknown >( ID3D11Resource*, D3D11_SHADER_RESOURCE_VIEW_DESC const*, ID3D11ShaderResourceView** )
	// <0x55c8b2> -> HRESULT < unknown >( ID3D11Resource*, D3D11_SHADER_RESOURCE_VIEW_DESC const*, ID3D11ShaderResourceView** )
	// <0x55c8ea> -> HRESULT < unknown >( ID3D11Resource*, D3D11_SHADER_RESOURCE_VIEW_DESC const*, ID3D11ShaderResourceView** )
	// <0x55c93c> -> HRESULT < unknown >( ID3D11Resource*, D3D11_SHADER_RESOURCE_VIEW_DESC const*, ID3D11ShaderResourceView** )
	// <0x55c971> -> HRESULT < unknown >( ID3D11Resource*, D3D11_SHADER_RESOURCE_VIEW_DESC const*, ID3D11ShaderResourceView** )
	// ******

	// FUNCTION BODY[0x55c6c0]: 104
	// <0x55c6ca>|0x00a|+0x009:'144'
	// <0x55c6d3>|0x013|+0x009:'145'
	// <0>
	// <0x55c6dc>|0x01c|+0x00a:'147'
	// <0>
	// <0x55c6e6>|0x026|+0x018:'149'
	// <0>
	// <0x55c6fe>|0x03e|+0x01a:'151'
	// <0>
	// <0x55c718>|0x058|+0x006:'153'
	// <0>
	// <0x55c71e>|0x05e|+0x006:'155'
	// <0x55c724>|0x064|+0x006:'156'
	// <0>
	// <0x55c72a>|0x06a|+0x008:'158'
	// <0>
	// <0x55c732>|0x072|+0x005:'160'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55c737>|0x077|+0x013:'165'
	// <0>
	// <0x55c74a>|0x08a|+0x00d:'167'
	// <0>
	// <1>
	// <2>
	// <0x55c757>|0x097|+0x00a:'171'
	// <0>
	// <0x55c761>|0x0a1|+0x008:'173'
	// <0>
	// <1>
	// <2>
	// <0x55c769>|0x0a9|+0x002:'177'
	// <0>
	// <0x55c76b>|0x0ab|+0x006:'179'
	// <0>
	// <0x55c771>|0x0b1|+0x003:'181'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55c774>|0x0b4|+0x009:'186'
	// <0x55c77d>|0x0bd|+0x017:'187'
	// <0>
	// <0x55c794>|0x0d4|+0x002:'189'
	// <0>
	// <1>
	// <2>
	// <0x55c796>|0x0d6|+0x00b:'193'
	// <0>
	// <1>
	// <0x55c7a1>|0x0e1|+0x002:'196'
	// <0>
	// <1>
	// <0x55c7a3>|0x0e3|+0x008:'199'
	// <0>
	// <0x55c7ab>|0x0eb|+0x00b:'201'
	// <0>
	// <1>
	// <2>
	// <0x55c7b6>|0x0f6|+0x007:'205'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x55c7bd>|0x0fd|+0x019:'212'
	// <0>
	// <1>
	// <0x55c7d6>|0x116|+0x008:'215'
	// <0x55c7de>|0x11e|+0x002:'216'
	// <0>
	// <0x55c7e0>|0x120|-0x014:'218'
	// <0>
	// <1>
	// <0x55c7cc>|0x10c|+0x008:'221'
	// <0x55c7d4>|0x114|+0x014:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55c7e8>|0x128|+0x014:'227'
	// <0x55c7fc>|0x13c|+0x07b:'228'
	// <0x55c877>|0x1b7|+0x005:'229'
	// <0>
	// <1>
	// <0x55c87c>|0x1bc|+0x009:'232'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55c885>|0x1c5|+0x006:'240'
	// <0x55c88b>|0x1cb|+0x079:'241'
	// <0x55c904>|0x244|+0x005:'242'
	// <0x55c909>|0x249|+0x00c:'243'
	// <0>
	// <1>
	// <0x55c915>|0x255|-0x00a:'246'
	// <0>
	// <0x55c90b>|0x24b|+0x098:'248'
	// ******
}

// STATE[STUB]
void* res_texture::map2D(
	D3D11_MAP		mode,
	u32				mip_level,
	u32&			row_pitch,
	bool			dot_not_wait
)
{
	// LOCALS
	// D3D11_RESOURCE_DIMENSION 		type
	// D3D11_MAPPED_SUBRESOURCE 		mapped_res
	// ******

	// CALL SITE INFO
	// <0x55c555> -> void < unknown >( D3D11_RESOURCE_DIMENSION* )
	// <0x55c58f> -> HRESULT < unknown >( ID3D11Resource*, u32, D3D11_MAP, u32, D3D11_MAPPED_SUBRESOURCE* )
	// ******

	return NULL;

	// FUNCTION BODY[0x55c530]: 29
	// <0x55c533>|0x003|+0x012:'252'
	// <0>
	// <1>
	// <0x55c545>|0x015|+0x012:'255'
	// <0x55c557>|0x027|+0x006:'256'
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
	// <0x55c55d>|0x02d|+0x034:'268'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55c591>|0x061|+0x00a:'273'
	// <0x55c59b>|0x06b|+0x00a:'274'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55c5a5>|0x075|-0x006:'280'
	// <0x55c59f>|0x06f|+0x008:'281'
	// ******
}

// STATE[STUB]
void res_texture::unmap2D( u32 mip_level )
{
	// CALL SITE INFO
	// <0x55c52a> -> void < unknown >( ID3D11Resource*, u32 )
	// ******

	// FUNCTION BODY[0x55c510]: 5
	// <0>
	// <1>
	// <2>
	// <0x55c510>|0x000|+0x01c:'287'
	// <0>
	// ******
}

// STATE[STUB]
void* res_texture::map3D( D3D11_MAP mode, u32 mip_level, u32& row_pitch )
{
	// LOCALS
	// D3D11_RESOURCE_DIMENSION 		type
	// D3D11_MAPPED_SUBRESOURCE 		mapped_res
	// ******

	// CALL SITE INFO
	// <0x55c4c5> -> void < unknown >( D3D11_RESOURCE_DIMENSION* )
	// <0x55c4f1> -> HRESULT < unknown >( ID3D11Resource*, u32, D3D11_MAP, u32, D3D11_MAPPED_SUBRESOURCE* )
	// ******

	return NULL;

	// FUNCTION BODY[0x55c4a0]: 24
	// <0x55c4a3>|0x003|+0x012:'294'
	// <0>
	// <1>
	// <0x55c4b5>|0x015|+0x012:'297'
	// <0x55c4c7>|0x027|+0x006:'298'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x55c4cd>|0x02d|+0x026:'308'
	// <0>
	// <0x55c4f3>|0x053|+0x00a:'310'
	// <0x55c4fd>|0x05d|+0x00a:'311'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55c507>|0x067|-0x006:'317'
	// <0x55c501>|0x061|+0x008:'318'
	// ******
}

// STATE[STUB]
void res_texture::unmap3D( u32 mip_level )
{
	// CALL SITE INFO
	// <0x55c49a> -> void < unknown >( ID3D11Resource*, u32 )
	// ******

	// FUNCTION BODY[0x55c480]: 5
	// <0>
	// <1>
	// <2>
	// <0x55c480>|0x000|+0x01c:'324'
	// <0>
	// ******
}

// STATE[STUB]
ID3D11Resource* res_texture::hw_texture( )
{
	return NULL;

	// FUNCTION BODY[0x55c290]: 4
	// <0>
	// <1>
	// <2>
	// <0x55c290>|0x000|+0x006:'333'
	// ******
}

// STATE[STUB]
void res_texture::clone( res_texture* other )
{
	// CALL SITE INFO
	// <0x55c323> -> unsigned long < unknown >()
	// <0x55c33f> -> unsigned long < unknown >()
	// <0x55c411> -> unsigned long < unknown >()
	// <0x55c423> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x55c310]: 32
	// <0x55c313>|0x003|+0x01c:'338'
	// <0x55c32f>|0x01f|+0x01c:'339'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55c34b>|0x03b|+0x004:'345'
	// <0x55c34f>|0x03f|+0x00f:'346'
	// <0x55c35e>|0x04e|+0x010:'347'
	// <0x55c36e>|0x05e|+0x006:'348'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55c374>|0x064|+0x00c:'354'
	// <0>
	// <0x55c380>|0x070|+0x00c:'356'
	// <0x55c38c>|0x07c|+0x00c:'357'
	// <0x55c398>|0x088|+0x00c:'358'
	// <0x55c3a4>|0x094|+0x038:'359'
	// <0x55c3dc>|0x0cc|+0x00c:'360'
	// <0x55c3e8>|0x0d8|+0x00c:'361'
	// <0x55c3f4>|0x0e4|+0x00d:'362'
	// <0>
	// <1>
	// <0x55c401>|0x0f1|+0x00a:'365'
	// <0x55c40b>|0x0fb|+0x008:'366'
	// <0>
	// <0x55c413>|0x103|+0x00a:'368'
	// <0x55c41d>|0x10d|+0x009:'369'
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
