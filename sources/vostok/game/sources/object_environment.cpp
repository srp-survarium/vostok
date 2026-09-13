// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "object_environment.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

object_environment::object_environment( base_game_scene& w ) :
	game_object_( w )
{
}

object_environment::~object_environment( )
{
}

// claude@NOTE: structure faithful (same query_resources idiom as STRUCTURE-MATCH sibling
// object_particle_visual, ~80% ceiling). QUANTITY is the target optimizer folding the
// 1-element request r[] init into the query_resources call (drops the r named local); our
// Master Gold base keeps it as its own statement. Not a source shape diff.
void object_environment::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	pcstr post_effect_name = pcstr( t["post_effect"] );

	resources::request r[] =
	{
		{ post_effect_name, resources::material_class },
	};

	resources::query_resources(
		r,
		1,
		boost::bind( &object_environment::material_ready, this, _1, cb ),
		g_allocator
	);
}

void object_environment::material_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	m_postprocess = data[0].get_unmanaged_resource();

	cb( *this );
}

void object_environment::remove( )
{
}

void object_environment::insert( )
{
	get_game_scene().renderer().scene().set_post_process( get_game_scene().render_scene_view(), m_postprocess );
}

} // namespace survarium
