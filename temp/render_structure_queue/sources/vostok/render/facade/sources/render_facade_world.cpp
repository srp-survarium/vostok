////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\facade\sources\render_facade_world.h"

namespace vostok {
namespace render {

// STATE[STUB]
 world::world(
	memory::base_allocator&					logic_allocator,
	memory::base_allocator*					editor_allocator,
	configs::binary_config_ptr const&		in_config,
	bool									is_editor
)
{
	// FUNCTION BODY[0x7380d0]: 12
	// <0>
	// <1>
	// <0x7380ee>|0x01e|+0x056:'37'
	// <0>
	// <1>
	// <0x738144>|0x074|+0x023:'40'
	// <0x738167>|0x097|+0x023:'41'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
 world::~world( )
{
	// FUNCTION BODY[0x7381b0]: 7
	// <0>
	// <1>
	// <2>
	// <0x7381b3>|0x003|+0x046:'54'
	// <0x7381f9>|0x049|+0x023:'55'
	// <0>
	// <0x73821c>|0x06c|+0x011:'57'
	// ******
}

// STATE[STUB]
engine::renderer& world::engine_renderer( )
{
	// FUNCTION BODY[0x738020]: 2
	// <0>
	// <0x738020>|0x000|+0x006:'63'
	// ******
}

// STATE[STUB]
game::renderer& world::game_renderer( )
{
	// FUNCTION BODY[0x738010]: 2
	// <0>
	// <0x738010>|0x000|+0x006:'69'
	// ******
}

// STATE[STUB]
void world::end_frame( )
{
	// FUNCTION BODY[0x737fe0]: 10
	// <0>
	// <1>
	// <2>
	// <0x737fe0>|0x000|+0x009:'87'
	// <0>
	// <0x737fe9>|0x009|+0x00c:'89'
	// <0x737ff5>|0x015|+0x006:'90'
	// <0>
	// <0x737ffb>|0x01b|+0x008:'92'
	// <0x738003>|0x023|+0x006:'93'
	// ******
}

// STATE[STUB]
void world::end_frame_logic( )
{
	// FUNCTION BODY[0x738080]: 11
	// <0x738083>|0x003|+0x005:'98'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x738088>|0x008|+0x015:'103'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x73809d>|0x01d|+0x027:'108'
	// ******
}

// STATE[STUB]
void world::tick( )
{
	// FUNCTION BODY[0x738030]: 5
	// <0x738030>|0x000|+0x00a:'130'
	// <0x73803a>|0x00a|+0x009:'131'
	// <0>
	// <0x738043>|0x013|+0x009:'133'
	// <0x73804c>|0x01c|+0x00d:'134'
	// ******
}

// STATE[STUB]
void world::clear_resources( )
{
	// FUNCTION BODY[0x738060]: 1
	// <0x738061>|0x001|+0x01c:'139'
	// ******
}

// STATE[STUB]
void world::enable_logic_thread_safety( const bool value )
{
	// FUNCTION BODY[0x737fb0]: 4
	// <0>
	// <0x737fb0>|0x000|+0x011:'145'
	// <0x737fc1>|0x011|+0x004:'146'
	// <0x737fc5>|0x015|+0x00d:'147'
	// ******
}

} // namespace render
} // namespace vostok
