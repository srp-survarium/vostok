////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\utils.h"

namespace vostok {
namespace render {

// STATE[STUB]
u32 utils::get_format_4x4_pixel_size( DXGI_FORMAT format )
{
	return 0;

	// FUNCTION BODY[0x559120]: 1
	// <0x559120>|0x000|+0x007:'223'
	// ******
}

// STATE[STUB]
u32 utils::calc_surface_size(
	u32				width,
	u32				height,
	DXGI_FORMAT		format,
	u32				row_min_pitch,
	u32&			row_pitch
)
{
	return 0;

	// FUNCTION BODY[0x559130]: 12
	// <0x55913d>|0x00d|+0x009:'265'
	// <0>
	// <0x559146>|0x016|+0x00b:'267'
	// <0x559151>|0x021|+0x00f:'268'
	// <0>
	// <0x559160>|0x030|+0x00a:'270'
	// <0x55916a>|0x03a|+0x010:'271'
	// <0>
	// <0x55917a>|0x04a|+0x007:'273'
	// <0x559181>|0x051|+0x00a:'273'
	// <0>
	// <1>
	// <0x55918b>|0x05b|-0x00b:'276'
	// <0x559180>|0x050|+0x014:'277'
	// ******
}

} // namespace render
} // namespace vostok
