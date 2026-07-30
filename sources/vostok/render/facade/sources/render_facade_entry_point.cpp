#include "pch.h"
#include <vostok/render/facade/sources/render_facade_entry_point.h>

namespace vostok {
namespace render {

logic_allocator_type* logic::g_allocator = 0;
editor_allocator_type* editor::g_allocator = 0;

void set_memory_allocator(
	memory::doug_lea_allocator& render_allocator,
	memory::base_allocator& logic_allocator,
	memory::base_allocator& editor_allocator
)
{
	g_allocator = &render_allocator;
	logic::g_allocator = &logic_allocator;
	editor::g_allocator = &editor_allocator;

	// FUNCTION BODY[0x738250]
}

world* create_world(
	memory::base_allocator&,
	memory::base_allocator*,
	configs::binary_config_ptr const&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x738270]
	return 0;
}

void destroy_world( world*& instance )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7382a0]
	instance = 0;
}

} // namespace render
} // namespace vostok
