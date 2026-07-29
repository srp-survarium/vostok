////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\render_target.h"

namespace vostok {
namespace render {

// STATE[STUB]
 render_target::render_target( )
{
	// FUNCTION BODY[0x55b970]: 7
	// <0x55b97d>|0x00d|+0x003:'27'
	// <0x55b980>|0x010|+0x003:'28'
	// <0x55b983>|0x013|+0x003:'29'
	// <0x55b986>|0x016|+0x003:'30'
	// <0x55b989>|0x019|+0x003:'31'
	// <0x55b98c>|0x01c|+0x003:'32'
	// <0x55b98f>|0x01f|+0x003:'33'
	// ******
}

// STATE[STUB]
 render_target::~render_target( )
{
	// FUNCTION BODY[0x55baf0]: 2
	// <0x55baf5>|0x005|+0x005:'38'
	// <0x55bafa>|0x00a|+0x00b:'39'
	// ******
}

// STATE[STUB]
void render_target::destroy_impl( ) const
{
	// FUNCTION BODY[0x55bb40]: 1
	// <0x55bb41>|0x001|+0x00e:'44'
	// ******
}

// STATE[STUB]
void render_target::save_as( pcstr file_name )
{
	// FUNCTION BODY[0x55b700]: 13
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
	// ******
}

// STATE[STUB]
void render_target::create_3d(
	pcstr				name,
	u32					width,
	u32					height,
	u32					depth,
	DXGI_FORMAT			format,
	enum_rt_usage		usage,
	D3D11_USAGE			memory_usage
)
{
	// LOCALS
	// D3D11_RENDER_TARGET_VIEW_DESC 	desc_rt
	// D3D11_TEXTURE3D_DESC 			desc
	// bool 							do_debug_break
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266d1>;
	// static bool 						ignore_always = <0x4c266d2>;
	// ******

	// CALL SITE INFO
	// <0x55bf77> -> HRESULT < unknown >( D3D11_TEXTURE3D_DESC const*, D3D11_SUBRESOURCE_DATA const*, ID3D11Texture3D** )
	// <0x55bfa7> -> HRESULT < unknown >( D3D11_TEXTURE3D_DESC const*, D3D11_SUBRESOURCE_DATA const*, ID3D11Texture3D** )
	// <0x55c033> -> HRESULT < unknown >( ID3D11Resource*, D3D11_RENDER_TARGET_VIEW_DESC const*, ID3D11RenderTargetView** )
	// <0x55c066> -> HRESULT < unknown >( ID3D11Resource*, D3D11_RENDER_TARGET_VIEW_DESC const*, ID3D11RenderTargetView** )
	// ******

	// FUNCTION BODY[0x55bed0]: 47
	// <0x55bedd>|0x00d|+0x010:'66'
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
	// <0x55beed>|0x01d|+0x00d:'78'
	// <0x55befa>|0x02a|+0x006:'79'
	// <0>
	// <1>
	// <2>
	// <0x55bf00>|0x030|+0x012:'83'
	// <0x55bf12>|0x042|+0x004:'84'
	// <0>
	// <0x55bf16>|0x046|+0x025:'86'
	// <0x55bf3b>|0x06b|+0x008:'87'
	// <0x55bf43>|0x073|+0x004:'88'
	// <0x55bf47>|0x077|+0x004:'89'
	// <0x55bf4b>|0x07b|+0x008:'90'
	// <0>
	// <0x55bf53>|0x083|+0x087:'92'
	// <0>
	// <1>
	// <0x55bfda>|0x10a|+0x016:'95'
	// <0x55bff0>|0x120|+0x004:'96'
	// <0x55bff4>|0x124|+0x008:'97'
	// <0x55bffc>|0x12c|+0x004:'98'
	// <0x55c000>|0x130|+0x004:'99'
	// <0x55c004>|0x134|+0x004:'100'
	// <0>
	// <0x55c008>|0x138|+0x091:'102'
	// <0>
	// <0x55c099>|0x1c9|+0x008:'104'
	// <0x55c0a1>|0x1d1|+0x038:'105'
	// <0x55c0d9>|0x209|+0x002:'106'
	// <0>
	// <0x55c0db>|0x20b|+0x041:'108'
	// <0x55c11c>|0x24c|+0x022:'109'
	// <0x55c13e>|0x26e|+0x00a:'110'
	// <0>
	// <0x55c148>|0x278|+0x010:'112'
	// ******
}

// STATE[STUB]
void render_target::create(
	pcstr				name,
	u32					width,
	u32					height,
	DXGI_FORMAT			format,
	enum_rt_usage		usage,
	res_texture_ptr		in_texture,
	u32					first_array_slice_index,
	u32					mip_slice
)
{
	// LOCALS
	// D3D11_DEPTH_STENCIL_VIEW_DESC 	desc_rt
	// D3D11_RENDER_TARGET_VIEW_DESC 	desc_rt
	// bool 							do_debug_break
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266d3>;
	// static bool 						ignore_always = <0x4c266d4>;
	// ******

	// CALL SITE INFO
	// <0x55b80e> -> HRESULT < unknown >( ID3D11Resource*, D3D11_DEPTH_STENCIL_VIEW_DESC const*, ID3D11DepthStencilView** )
	// <0x55b84c> -> HRESULT < unknown >( ID3D11Resource*, D3D11_DEPTH_STENCIL_VIEW_DESC const*, ID3D11DepthStencilView** )
	// <0x55b8d6> -> HRESULT < unknown >( ID3D11Resource*, D3D11_RENDER_TARGET_VIEW_DESC const*, ID3D11RenderTargetView** )
	// <0x55b910> -> HRESULT < unknown >( ID3D11Resource*, D3D11_RENDER_TARGET_VIEW_DESC const*, ID3D11RenderTargetView** )
	// ******

	// FUNCTION BODY[0x55b710]: 72
	// <0x55b71e>|0x00e|+0x009:'117'
	// <0x55b727>|0x017|+0x006:'118'
	// <0>
	// <0x55b72d>|0x01d|+0x003:'120'
	// <0x55b730>|0x020|+0x003:'121'
	// <0x55b733>|0x023|+0x003:'122'
	// <0x55b736>|0x026|+0x003:'123'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55b739>|0x029|+0x004:'128'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55b73d>|0x02d|+0x00f:'133'
	// <0>
	// <1>
	// <0x55b74c>|0x03c|+0x013:'136'
	// <0>
	// <1>
	// <2>
	// <0x55b75f>|0x04f|+0x00b:'140'
	// <0x55b76a>|0x05a|+0x00c:'141'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55b776>|0x066|+0x024:'149'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55b79a>|0x08a|+0x029:'154'
	// <0>
	// <0x55b7c3>|0x0b3|+0x00a:'156'
	// <0>
	// <1>
	// <0x55b7cd>|0x0bd|+0x008:'159'
	// <0x55b7d5>|0x0c5|+0x002:'160'
	// <0>
	// <0x55b7d7>|0x0c7|+0x008:'162'
	// <0>
	// <1>
	// <2>
	// <0x55b7df>|0x0cf|+0x081:'166'
	// <0>
	// <0x55b860>|0x150|+0x005:'168'
	// <0>
	// <1>
	// <0x55b865>|0x155|+0x008:'171'
	// <0x55b86d>|0x15d|+0x007:'172'
	// <0>
	// <0x55b874>|0x164|+0x005:'174'
	// <0>
	// <1>
	// <0x55b879>|0x169|+0x010:'177'
	// <0x55b889>|0x179|+0x004:'178'
	// <0x55b88d>|0x17d|+0x008:'179'
	// <0>
	// <0x55b895>|0x185|+0x002:'181'
	// <0>
	// <1>
	// <0x55b897>|0x187|+0x010:'184'
	// <0>
	// <1>
	// <0x55b8a7>|0x197|-0x152:'187'
	// <0>
	// <0x55b755>|0x045|+0x1f1:'189'
	// ******
}

// STATE[STUB]
void render_target::create(
	pcstr				name,
	u32					width,
	u32					height,
	DXGI_FORMAT			format,
	enum_rt_usage		usage,
	D3D11_USAGE			memory_usage,
	u32					sample_count
)
{
	// LOCALS
	// D3D11_TEXTURE2D_DESC 			desc
	// D3D11_DEPTH_STENCIL_VIEW_DESC 	ViewDesc
	// bool 							do_debug_break
	// bool 							do_debug_break
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266d5>;
	// static bool 						ignore_always = <0x4c266d6>;
	// static bool 						ignore_always = <0x10000>;
	// static bool 						ignore_always = <0x4c266d7>;
	// ******

	// CALL SITE INFO
	// <0x55bc57> -> HRESULT < unknown >( D3D11_TEXTURE2D_DESC const*, D3D11_SUBRESOURCE_DATA const*, ID3D11Texture2D** )
	// <0x55bc89> -> HRESULT < unknown >( D3D11_TEXTURE2D_DESC const*, D3D11_SUBRESOURCE_DATA const*, ID3D11Texture2D** )
	// <0x55bd4b> -> HRESULT < unknown >( ID3D11Resource*, D3D11_DEPTH_STENCIL_VIEW_DESC const*, ID3D11DepthStencilView** )
	// <0x55bd87> -> HRESULT < unknown >( ID3D11Resource*, D3D11_DEPTH_STENCIL_VIEW_DESC const*, ID3D11DepthStencilView** )
	// <0x55bdea> -> HRESULT < unknown >( ID3D11Resource*, D3D11_RENDER_TARGET_VIEW_DESC const*, ID3D11RenderTargetView** )
	// <0x55be1b> -> HRESULT < unknown >( ID3D11Resource*, D3D11_RENDER_TARGET_VIEW_DESC const*, ID3D11RenderTargetView** )
	// ******

	// FUNCTION BODY[0x55bb50]: 142
	// <0>
	// <1>
	// <0x55bb61>|0x011|+0x013:'195'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55bb74>|0x024|+0x008:'200'
	// <0>
	// <1>
	// <0x55bb7c>|0x02c|+0x00c:'203'
	// <0x55bb88>|0x038|+0x00c:'204'
	// <0>
	// <1>
	// <0x55bb94>|0x044|+0x007:'207'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55bb9b>|0x04b|+0x018:'215'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55bbb3>|0x063|+0x00f:'221'
	// <0x55bbc2>|0x072|+0x006:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55bbc8>|0x078|+0x024:'230'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x55bbec>|0x09c|+0x020:'240'
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
	// <0x55bc0c>|0x0bc|+0x0b6:'259'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55bcc2>|0x172|+0x00c:'265'
	// <0>
	// <1>
	// <0x55bcce>|0x17e|+0x016:'268'
	// <0>
	// <0x55bce4>|0x194|+0x004:'270'
	// <0>
	// <1>
	// <0x55bce8>|0x198|+0x008:'273'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55bcf0>|0x1a0|+0x004:'281'
	// <0x55bcf4>|0x1a4|+0x019:'282'
	// <0>
	// <1>
	// <0x55bd0d>|0x1bd|+0x008:'285'
	// <0x55bd15>|0x1c5|+0x002:'286'
	// <0>
	// <0x55bd17>|0x1c7|-0x014:'288'
	// <0>
	// <1>
	// <0x55bd03>|0x1b3|+0x008:'291'
	// <0x55bd0b>|0x1bb|+0x014:'292'
	// <0>
	// <1>
	// <0x55bd1f>|0x1cf|+0x09e:'295'
	// <0>
	// <0x55bdbd>|0x26d|+0x005:'297'
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
	// <0x55bdc2>|0x272|+0x08e:'308'
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
	// <0x55be50>|0x300|+0x008:'326'
	// <0x55be58>|0x308|+0x020:'327'
	// <0x55be78>|0x328|+0x002:'328'
	// <0>
	// <0x55be7a>|0x32a|+0x02b:'330'
	// <0x55bea5>|0x355|+0x009:'331'
	// <0x55beae>|0x35e|+0x008:'332'
	// <0>
	// <0x55beb6>|0x366|+0x00f:'334'
	// ******
}

// STATE[STUB]
void render_target::destroy( )
{
	// CALL SITE INFO
	// <0x55ba1b> -> unsigned long < unknown >()
	// <0x55bacf> -> unsigned long < unknown >()
	// <0x55bae1> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x55b9a0]: 20
	// <0x55b9a8>|0x008|+0x009:'339'
	// <0>
	// <0x55b9b1>|0x011|+0x048:'341'
	// <0x55b9f9>|0x059|+0x015:'342'
	// <0>
	// <1>
	// <0x55ba0e>|0x06e|+0x012:'345'
	// <0>
	// <0x55ba20>|0x080|+0x005:'347'
	// <0>
	// <0x55ba25>|0x085|+0x04c:'349'
	// <0>
	// <1>
	// <0x55ba71>|0x0d1|+0x005:'352'
	// <0>
	// <0x55ba76>|0x0d6|+0x04c:'354'
	// <0>
	// <1>
	// <0x55bac2>|0x122|+0x012:'357'
	// <0x55bad4>|0x134|+0x012:'358'
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
