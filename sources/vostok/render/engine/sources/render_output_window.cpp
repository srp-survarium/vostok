#include "pch.h"
#include "render_output_window.h"

namespace vostok {
namespace render {

// STATE[STUB]
render_output_window::render_output_window(
	output_window_configuration const& window_configuration
) :
	m_targets(
		math::uint2( window_configuration.width, window_configuration.height )
	),
	m_window			( static_cast< HWND__* >( window_configuration.hwnd ) ),
	m_windowed			( window_configuration.windowed ),
	m_flash_renderer	( 0 )
{
	// FUNCTION BODY[0x653c70]: 22
	// <0x653ce6>|0x076|+0x024:'24'
	// <0>
	// <0x653d0a>|0x09a|+0x00a:'26'
	// <0x653d14>|0x0a4|+0x07b:'27'
	// <0x653d8f>|0x11f|+0x007:'28'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x653d96>|0x126|+0x034:'33'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x653dca>|0x15a|+0x02c:'39'
	// <0>
	// <0x653df6>|0x186|+0x004:'41'
	// <0x653dfa>|0x18a|+0x006:'42'
	// <0>
	// <0x653e00>|0x190|+0x007:'44'
	// <0x653e07>|0x197|+0x016:'45'
	// ******
}

// STATE[STUB]
void render_output_window::goto_fullscreen( )
{
	// FUNCTION BODY[0x653a60]: 1
	// <0x653a61>|0x001|+0x00c:'50'
	// ******
}

// STATE[STUB]
void render_output_window::set_size(
	const u32		width,
	const u32		height,
	const bool		fullscreen,
	bool			force_resize
)
{
	// FUNCTION BODY[0x653b70]: 20
	// <0>
	// <1>
	// <0x653b81>|0x011|+0x010:'57'
	// <0>
	// <1>
	// <0x653b91>|0x021|+0x02e:'60'
	// <0>
	// <1>
	// <2>
	// <0x653bbf>|0x04f|+0x013:'64'
	// <0>
	// <0x653bd2>|0x062|+0x01c:'66'
	// <0x653bee>|0x07e|+0x015:'67'
	// <0>
	// <0x653c03>|0x093|+0x009:'69'
	// <0>
	// <0x653c0c>|0x09c|+0x01b:'71'
	// <0x653c27>|0x0b7|+0x00b:'72'
	// <0x653c32>|0x0c2|+0x02b:'73'
	// <0>
	// ******
}

// STATE[STUB]
void render_output_window::resize( bool force_resize )
{
	// FUNCTION BODY[0x653a70]: 21
	// <0x653a7d>|0x00d|+0x011:'79'
	// <0>
	// <1>
	// <0x653a8e>|0x01e|+0x010:'82'
	// <0>
	// <0x653a9e>|0x02e|+0x01b:'84'
	// <0>
	// <1>
	// <0x653ab9>|0x049|+0x018:'87'
	// <0>
	// <1>
	// <2>
	// <0x653ad1>|0x061|+0x023:'91'
	// <0x653af4>|0x084|+0x018:'92'
	// <0>
	// <0x653b0c>|0x09c|+0x009:'94'
	// <0>
	// <0x653b15>|0x0a5|+0x01c:'96'
	// <0x653b31>|0x0c1|+0x007:'97'
	// <0x653b38>|0x0c8|+0x02b:'98'
	// <0>
	// ******
}

// STATE[STUB]
math::uint2 render_output_window::get_window_client_size( HWND__* const window, bool windowed )
{
	// LOCALS
	// tagRECT 							rect
	// ******

	// CALL SITE INFO
	// <0x653a0f> -> int < unknown >( HWND__*, tagRECT* )
	// <0x653a1c> -> int < unknown >( HWND__*, tagRECT* )
	// ******

	return vostok::math::uint2(1, 1);

	// FUNCTION BODY[0x653a00]: 6
	// <0>
	// <1>
	// <0x653a03>|0x003|+0x023:'106'
	// <0x653a26>|0x026|+0x014:'107'
	// <0>
	// <0x653a3a>|0x03a|+0x006:'109'
	// <0x653a40>|0x040|-0x004:'109'
	// <0x653a3c>|0x03c|+0x013:'110'
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
