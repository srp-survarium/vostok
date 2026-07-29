////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\state_descriptor.h"

namespace vostok {
namespace render {

// STATE[STUB]
 state_descriptor::state_descriptor( )
{
	// FUNCTION BODY[0x557fc0]: 1
	// <0x557fc3>|0x003|+0x005:'16'
	// ******
}

// STATE[STUB]
void state_descriptor::reset( )
{
	// FUNCTION BODY[0x557f00]: 9
	// <0x557f01>|0x001|+0x003:'21'
	// <0>
	// <1>
	// <2>
	// <0x557f04>|0x004|+0x032:'25'
	// <0x557f36>|0x036|+0x050:'26'
	// <0x557f86>|0x086|+0x02b:'27'
	// <0>
	// <0x557fb1>|0x0b1|+0x007:'29'
	// ******
}

// STATE[STUB]
void state_descriptor::set_depth( bool enable, bool write_enable, D3D11_COMPARISON_FUNC cmp_func )
{
	// FUNCTION BODY[0x557ed0]: 5
	// <0>
	// <0x557ed0>|0x000|+0x00f:'35'
	// <0x557edf>|0x00f|+0x007:'36'
	// <0>
	// <0x557ee6>|0x016|+0x00a:'38'
	// ******
}

// STATE[STUB]
void state_descriptor::set_stencil(
	s32		enable,
	u32		ref,
	u8		read_mask,
	u8		write_mask
)
{
	// FUNCTION BODY[0x557ea0]: 6
	// <0>
	// <0x557ea0>|0x000|+0x007:'44'
	// <0x557ea7>|0x007|+0x007:'45'
	// <0x557eae>|0x00e|+0x00d:'46'
	// <0>
	// <0x557ebb>|0x01b|+0x007:'48'
	// ******
}

// STATE[STUB]
void state_descriptor::set_stencil_frontface(
	D3D11_COMPARISON_FUNC		func,
	D3D11_STENCIL_OP			fail,
	D3D11_STENCIL_OP			pass,
	D3D11_STENCIL_OP			zfail
)
{
	// FUNCTION BODY[0x557e70]: 6
	// <0>
	// <0x557e70>|0x000|+0x003:'54'
	// <0x557e73>|0x003|+0x00c:'55'
	// <0x557e7f>|0x00f|+0x00a:'56'
	// <0>
	// <0x557e89>|0x019|+0x006:'58'
	// ******
}

// STATE[STUB]
void state_descriptor::set_stencil_backface(
	D3D11_COMPARISON_FUNC		func,
	D3D11_STENCIL_OP			fail,
	D3D11_STENCIL_OP			pass,
	D3D11_STENCIL_OP			zfail
)
{
	// FUNCTION BODY[0x557e40]: 6
	// <0>
	// <0x557e40>|0x000|+0x003:'64'
	// <0x557e43>|0x003|+0x00c:'65'
	// <0x557e4f>|0x00f|+0x00a:'66'
	// <0>
	// <0x557e59>|0x019|+0x006:'68'
	// ******
}

// STATE[STUB]
void state_descriptor::set_alpha_blend(
	s32					blend_enable,
	D3D11_BLEND			src_blend,
	D3D11_BLEND			dest_blend,
	D3D11_BLEND_OP		blend_op,
	D3D11_BLEND			src_alpha_blend,
	D3D11_BLEND			dest_alpha_blend,
	D3D11_BLEND_OP		blend_alpha_op
)
{
	// FUNCTION BODY[0x557cd0]: 24
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
	// <0x557cde>|0x00e|+0x02e:'107'
	// <0x557d0c>|0x03c|+0x02d:'108'
	// <0x557d39>|0x069|+0x003:'109'
	// <0x557d3c>|0x06c|+0x003:'110'
	// <0x557d3f>|0x06f|+0x007:'111'
	// <0x557d46>|0x076|+0x003:'112'
	// <0x557d49>|0x079|+0x0f2:'113'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void state_descriptor::set_cull_mode( D3D11_CULL_MODE mode )
{
	// FUNCTION BODY[0x557cc0]: 2
	// <0x557cc0>|0x000|+0x003:'126'
	// <0x557cc3>|0x003|+0x00c:'127'
	// ******
}

// STATE[STUB]
void state_descriptor::color_write_enable( D3D11_COLOR_WRITE_ENABLE mode )
{
	// FUNCTION BODY[0x557c90]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x557c92>|0x002|+0x01f:'138'
	// <0>
	// <1>
	// <0x557cb1>|0x021|+0x00a:'141'
	// <0>
	// ******
}

// STATE[STUB]
void state_descriptor::set_fill_mode( D3D11_FILL_MODE fill_mode )
{
	// FUNCTION BODY[0x557c80]: 2
	// <0x557c80>|0x000|+0x002:'146'
	// <0x557c82>|0x002|+0x00b:'147'
	// ******
}

} // namespace render
} // namespace vostok
