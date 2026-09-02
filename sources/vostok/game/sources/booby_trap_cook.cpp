// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "booby_trap_cook.h"
#include "booby_trap.h"
#include <vostok/particle/world.h>
#include <vostok/sound/sound_emitter.h>

namespace survarium {

booby_trap_cook::booby_trap_cook( game_world& gw ) :
	m_game_world( gw )
{
}

void booby_trap_cook::query_for_derived_resources(
	resources::query_result_for_cook*		parent,
	booby_trap_core*						resource,
	configs::binary_config_ptr				config
)
{
	configs::binary_config_value const& data = config->get_root( )["data"];

	resources::request requests[5] = {
		{ (pcstr)data["model_armed"],		resources::static_model_instance_class },
		{ (pcstr)data["model_fired"],		resources::static_model_instance_class },
		{ (pcstr)data["model_disarmed"],	resources::static_model_instance_class },

		{ (pcstr)data["particle_fired"],	resources::particle_system_instance_class },

		{ (pcstr)data["sound_fired"],		resources::single_sound_class },
	};

	resources::query_resources(
		requests,
		boost::bind( &booby_trap_cook::on_models_ready, this, _1, static_cast< booby_trap* >( resource ) ),
		g_allocator,
		NULL,
		parent,
		assert_on_fail_true
	);
}

void booby_trap_cook::on_models_ready( resources::queries_result& data, booby_trap* game_resource )
{
	game_resource->m_models[0]		=
		NULL;
	game_resource->m_models[1]		= static_cast_resource_ptr< render::static_model_ptr >( data[0].get_unmanaged_resource( ) );
	game_resource->m_models[2]		= static_cast_resource_ptr< render::static_model_ptr >( data[1].get_unmanaged_resource( ) );
	game_resource->m_models[3]		= static_cast_resource_ptr< render::static_model_ptr >( data[2].get_unmanaged_resource( ) );

	game_resource->m_particle_fired	= static_cast_resource_ptr< particle::particle_system_instance_ptr >( data[3].get_unmanaged_resource( ) );

	game_resource->m_sound_fired	= static_cast_resource_ptr< sound::sound_emitter_ptr >( data[4].get_unmanaged_resource( ) ).c_ptr( );

	finish_query( data.get_parent_query( ), game_resource );
}


} // namespace survarium
