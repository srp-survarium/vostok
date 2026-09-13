// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
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

// the compiland keeps its legacy name: the 2011/12 TU defined the whole
// object_*_visual family (git show 885e1d4a4:temp/game_legacy/object_solid_visual.h), but
// only object_particle_visual survived into the shipped PDB (source lines
// 455-491 of a much larger file)

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

object_particle_visual::object_particle_visual( base_game_scene& w ) :
	game_object_static( w )
{
}

void object_particle_visual::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	resources::user_data_variant ud;
	ud.set( &get_game_scene().renderer().scene().particle_world( get_game_scene().render_scene() ) );

	pcstr lib_name = pcstr( t["lib_name"] );

	resources::request r[] =
	{
		{ lib_name, resources::particle_system_instance_class },
	};

	resources::user_data_variant const* user_data[] = { &ud };

	resources::query_resources(
		r,
		1,
		boost::bind( &object_particle_visual::on_visual_ready, this, _1, cb ),
		g_allocator,
		user_data
	);
}

void object_particle_visual::on_visual_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	m_particle_system_instance_ptr = static_cast_resource_ptr< particle::particle_system_instance_ptr >( data[0].get_unmanaged_resource() );

	cb( *this );
}

// claude@NOTE: STRUCTURE MATCH (1 stmt). The facade by-value play_particle_system fix
// has landed, so the temp intrusive_ptr copy is no longer the wall. Residual is the
// call-boundary register cascade: target keeps get_game_scene() in esi and computes
// render_scene() (`lea ecx,[esi+4]`) last, using edx for the renderer().scene() chain;
// the base uses ecx for the chain and `add esi,4`. Non-steerable /Od arg-eval
// scheduling, same as weapon::play_weapon_*_pfx.
void object_particle_visual::insert( )
{
	get_game_scene().renderer().scene().play_particle_system( get_game_scene().render_scene(), m_particle_system_instance_ptr, m_transform );
}

void object_particle_visual::remove( )
{
	get_game_scene().renderer().scene().remove_particle_system_instance( get_game_scene().render_scene(), m_particle_system_instance_ptr );
}

} // namespace survarium
