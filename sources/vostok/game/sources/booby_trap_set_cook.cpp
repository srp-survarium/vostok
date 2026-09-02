// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "booby_trap_set_cook.h"
#include "booby_trap_set.h"

namespace survarium {

void booby_trap_set_cook::query_for_derived_resources(
	resources::query_result_for_cook*		parent,
	booby_trap_set_core*					resource,
	booby_trap_set_cook_data const&			cook_data,
	configs::binary_config_ptr				config
)
{
	if ( !cook_data.is_local_player )
	{
		finish_query( parent, resource );
		return;
	}

	configs::binary_config_value const& data = config->get_root( )["data"];

	resources::request requests[2] = {
		{ (pcstr)data["model_ghost_allowed"],	resources::static_model_instance_class },
		{ (pcstr)data["model_ghost_denied"],	resources::static_model_instance_class },
	};

	resources::query_resources(
		requests,
		boost::bind( &booby_trap_set_cook::on_models_ready, this, _1, static_cast< booby_trap_set* >( resource ) ),
		g_allocator,
		NULL,
		parent,
		assert_on_fail_true
	);
}

void booby_trap_set_cook::on_models_ready( resources::queries_result& data, booby_trap_set* game_resource )
{
	game_resource->m_model_ghost_allowed	= static_cast_resource_ptr< render::static_model_ptr >( data[0].get_unmanaged_resource( ) );
	game_resource->m_model_ghost_denied		= static_cast_resource_ptr< render::static_model_ptr >( data[1].get_unmanaged_resource( ) );

	finish_query( data.get_parent_query( ), game_resource );
}


} // namespace survarium
