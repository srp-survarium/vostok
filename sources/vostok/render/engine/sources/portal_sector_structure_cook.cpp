// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "portal_sector_structure_cook.h"
#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace render {
namespace culling {

portal_sector_structure_cook::portal_sector_structure_cook( ) :
	resources::translate_query_cook(
		resources::portal_sector_structure_class,
		reuse_false,
		use_current_thread_id
)
{
}

void portal_sector_structure_cook::translate_query( resources::query_result_for_cook& parent )
{
	configs::binary_config_ptr game_progect_cfg_ptr;

	if (
		parent.user_data()->try_get( game_progect_cfg_ptr ) &&
		game_progect_cfg_ptr->get_root().value_exists( "portal_system" )
	)
	{
		configs::binary_config_value portal_system_cfg = game_progect_cfg_ptr->get_root()["portal_system"];
		u32 const sectors_count = portal_system_cfg["sectors"].size();
		u32 const portals_count = portal_system_cfg["portals"].size();
		portal_sector_structure* structure = NEW( portal_sector_structure )(
			g_allocator,
			sectors_count,
			portals_count
		);
		structure->load( &portal_system_cfg );
		parent.set_unmanaged_resource( structure, resources::nocache_memory, sizeof( portal_sector_structure ) );
		parent.finish_query( result_success );
		return;
	}

	parent.finish_query( result_error );
}

void portal_sector_structure_cook::delete_resource( resources::resource_base* resource )
{
	portal_sector_structure* structure = static_cast<portal_sector_structure*>( resource );
	DELETE( structure );
}

} // namespace culling
} // namespace render
} // namespace vostok
