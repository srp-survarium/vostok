#include "pch.h"
#include <vostok/render/facade/sources/render_facade_entry_point.h>
#include <vostok/render/world.h>

namespace vostok {
namespace render {

static uninitialized_reference< world >	s_world;

logic_allocator_type* logic::g_allocator = 0;
editor_allocator_type* editor::g_allocator = 0;

void set_memory_allocator(
	memory::doug_lea_allocator& render_allocator,
	memory::base_allocator& logic_allocator,
	memory::base_allocator& editor_allocator
)
{
	R_ASSERT			( !g_allocator );
	g_allocator			= &render_allocator;

	R_ASSERT			( !logic::g_allocator );
	logic::g_allocator	= &logic_allocator;

	R_ASSERT			( !editor::g_allocator );
	editor::g_allocator	= &editor_allocator;

}

world* create_world(
	memory::base_allocator& logic_allocator,
	memory::base_allocator* editor_allocator,
	configs::binary_config_ptr const& in_config,
	bool is_editor
)
{
	VOSTOK_CONSTRUCT_REFERENCE( s_world, world )(
		logic_allocator,
		editor_allocator,
		in_config,
		is_editor
	);
	return &*s_world;
}

void destroy_world( world*& instance )
{
	R_ASSERT					( &*s_world == instance );
	VOSTOK_DESTROY_REFERENCE	( s_world );
	instance					= 0;
}

} // namespace render
} // namespace vostok
