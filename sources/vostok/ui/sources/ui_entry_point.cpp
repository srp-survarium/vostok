#include "pch.h"
#include "ui_world.h"
#include <vostok/ui/api.h>
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(ui_entry_point)

namespace vostok {
namespace ui {

world* vostok::ui::create_world(engine& engine, render::ui::renderer& renderer, memory::base_allocator& allocator, render::scene_view_ptr const& scene_view )
{
	return						VOSTOK_NEW_IMPL( allocator, ui_world )( engine, renderer, allocator, scene_view );
}

void destroy_world( world*& world )
{
	ui_world* world_inherited	= static_cast_checked<ui_world*>(world);
	VOSTOK_DELETE_IMPL			( world_inherited->allocator(), world_inherited );
	world						= 0;
}

} // namespace vostok
} // namespace ui