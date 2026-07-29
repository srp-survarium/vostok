////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\facade\sources\render_facade_entry_point.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_world''( )
{
	// FUNCTION BODY[0x7d4140]
	// ******
}

// STATE[STUB]
void set_memory_allocator(
	memory::doug_lea_allocator&		render_allocator,
	memory::base_allocator&			logic_allocator,
	memory::base_allocator&			editor_allocator
)
{
	// FUNCTION BODY[0x738250]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x738250>|0x000|+0x015:'36'
	// ******
}

// STATE[STUB]
world* create_world(
	memory::base_allocator&					logic_allocator,
	memory::base_allocator*					editor_allocator,
	configs::binary_config_ptr const&		in_config,
	bool									is_editor
)
{
	return NULL;

	// FUNCTION BODY[0x738270]: 2
	// <0x738271>|0x001|+0x020:'46'
	// <0x738291>|0x021|+0x006:'47'
	// ******
}

// STATE[STUB]
void destroy_world( world*& world )
{
	// FUNCTION BODY[0x7382a0]: 3
	// <0>
	// <0x7382a0>|0x000|+0x00b:'53'
	// <0x7382ab>|0x00b|+0x014:'54'
	// ******
}

} // namespace render
} // namespace vostok
