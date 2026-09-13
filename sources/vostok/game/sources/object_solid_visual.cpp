// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "object_particle_visual.h"
#include "base_game_scene.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/particle/world.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

object_particle_visual::object_particle_visual( base_game_scene& w ) :
	game_object_static( w )
{
}

#line 453
void object_particle_visual::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	resources::user_data_variant ud;
	particle::world* p = &get_game_scene().renderer().scene().particle_world( get_game_scene().render_scene() );
	ud.set( p );

	pcstr lib_name = pcstr( t["lib_name"] );

	resources::query_resource(
		lib_name,
		resources::particle_system_instance_class,
		boost::bind( &object_particle_visual::on_visual_ready, this, _1, cb ),
		g_allocator,
		&ud
	);
}

void object_particle_visual::on_visual_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{


	m_particle_system_instance_ptr = static_cast_resource_ptr< particle::particle_system_instance_ptr >( data[0].get_unmanaged_resource() );
	cb( *this );
}

void object_particle_visual::insert( )
{
	get_game_scene().renderer().scene().play_particle_system( get_game_scene().render_scene(), m_particle_system_instance_ptr, m_transform );
}

void object_particle_visual::remove( )
{
	get_game_scene().renderer().scene().remove_particle_system_instance( get_game_scene().render_scene(), m_particle_system_instance_ptr );
}

} // namespace survarium
