// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "object_sky.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

object_sky::object_sky( base_game_scene& w ) :
	game_object_( w )
{
}

object_sky::~object_sky( )
{
}

// claude@NOTE: structure faithful (same idiom as STRUCTURE-MATCH sibling
// object_particle_visual, ~80% ceiling). QUANTITY is the target optimizer folding the NEW
// cook_data + the inlined variant set() into the user_data decl line and dropping the
// r[]/ud[] named locals; our Master Gold base keeps them split. Not a source shape diff.
void object_sky::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	pcstr sky_material_name = pcstr( t["material_name"] );

	render::material_effects_instance_cook_data* cook_data =
		VOSTOK_NEW_IMPL( ::survarium::g_allocator, render::material_effects_instance_cook_data )( render::post_process_vertex_input_type, NULL, false );

	resources::user_data_variant user_data;
	user_data.set( cook_data );

	resources::request r[] =
	{
		{ sky_material_name, resources::material_effects_instance_class },
	};

	resources::user_data_variant const* ud[] = { &user_data };

	resources::query_resources(
		r,
		1,
		boost::bind( &object_sky::material_ready, this, _1, cook_data, cb ),
		g_allocator,
		ud
	);
}

void object_sky::material_ready(
	resources::queries_result&		data,
	render::material_effects_instance_cook_data*	cook_data,
	boost::function< void( game_object_& ) >&	cb
)
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, cook_data );

	m_sky_material = data[0].get_unmanaged_resource();

	cb( *this );
}

void object_sky::insert( )
{
	get_game_scene().renderer().scene().set_sky_material( get_game_scene().render_scene(), m_sky_material );
}

void object_sky::remove( )
{
	get_game_scene().renderer().scene().set_sky_material( get_game_scene().render_scene(), resources::unmanaged_resource_ptr() );
}

} // namespace survarium
