// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "object_decal.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32 decal_ids = 0;

object_decal::object_decal( base_game_scene& w ) :
	game_object_static( w )
{
	m_decal_id = decal_ids++;
}

object_decal::~object_decal( )
{
}

// claude@NOTE: structure faithful (same idiom as STRUCTURE-MATCH sibling
// object_particle_visual, which itself caps ~80%). Residual is a build-config ceiling, not
// a source shape: target inlines configs::binary_config_value::operator float at every
// float() read (asserts off -> cmp [.+14h],2 / movss|cvtsi2ss), our Master Gold base never
// inlines it (asserts active -> call to the /Od out-of-line COMDAT, too big for /Ob2; same
// in object_wire). Target's optimizer also folds the request r[]/ud[] inits into the
// query_resources call (drops the r/ud named locals); ours keep them split.
void object_decal::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	m_projection_on_static_geometry		= t.value_exists( "projection_on_static_geometry" ) ?
										  bool( t["projection_on_static_geometry"] ) : true;

	m_projection_on_skeleton_geometry	= t.value_exists( "projection_on_skeleton_geometry" ) ?
										  bool( t["projection_on_skeleton_geometry"] ) : true;

	m_projection_on_terrain_geometry	= t.value_exists( "projection_on_terrain_geometry" ) ?
										  bool( t["projection_on_terrain_geometry"] ) : true;

	m_projection_on_speedtree_geometry	= t.value_exists( "projection_on_speedtree_geometry" ) ?
										  bool( t["projection_on_speedtree_geometry"] ) : true;

	m_projection_on_particle_geometry	= t.value_exists( "projection_on_particle_geometry" ) ?
										  bool( t["projection_on_particle_geometry"] ) : true;

	m_alpha_angle						= t.value_exists( "alpha_angle" ) ?
										  float( t["alpha_angle"] ) : -90.0f;

	m_clip_angle						= t.value_exists( "clip_angle" ) ?
										  float( t["clip_angle"] ) : -90.0f;

	m_decal_far_distance				= t.value_exists( "decal_far_distance" ) ?
										  float( t["decal_far_distance"] ) : 1.0f;

	m_decal_width						= t.value_exists( "decal_width" ) ?
										  float( t["decal_width"] ) : 1.0f;

	m_decal_height						= t.value_exists( "decal_height" ) ?
										  float( t["decal_height"] ) : 1.0f;

	m_draw_priority						= t.value_exists( "draw_priority" ) ?
										  float( t["draw_priority"] ) : 0.0f;

	pcstr decal_material_name = pcstr( t["decal_material"] );

	render::material_effects_instance_cook_data* cook_data =
		VOSTOK_NEW_IMPL( ::survarium::g_allocator, render::material_effects_instance_cook_data )( render::decal_vertex_input_type, NULL, false );

	resources::user_data_variant user_data;
	user_data.set( cook_data );

	resources::request r[] =
	{
		{ decal_material_name, resources::material_effects_instance_class },
	};

	resources::user_data_variant const* ud[] = { &user_data };

	resources::query_resources(
		r,
		boost::bind( &object_decal::material_ready, this, _1, cook_data, cb ),
		g_allocator,
		ud
	);
}

void object_decal::insert( )
{
	get_game_scene().renderer().scene().update_decal(
		get_game_scene().render_scene(),
		m_decal_id,
		render::decal_properties(
			m_transform,
			m_material,
			float3( m_decal_width, m_decal_height, m_decal_far_distance ),
			m_alpha_angle / 90.0f,
			m_clip_angle / 90.0f,
			m_projection_on_terrain_geometry,
			m_projection_on_static_geometry,
			m_projection_on_speedtree_geometry,
			m_projection_on_skeleton_geometry,
			m_projection_on_particle_geometry
		)
	);
}

void object_decal::remove( )
{
	get_game_scene().renderer().scene().remove_decal( get_game_scene().render_scene(), m_decal_id );
}

void object_decal::material_ready(
	resources::queries_result&		data,
	render::material_effects_instance_cook_data*	cook_data,
	boost::function< void( game_object_& ) >&	cb
)
{
	VOSTOK_DELETE_IMPL( ::survarium::g_allocator, cook_data );

	m_material = data[0].get_unmanaged_resource();

	cb( *this );
}

} // namespace survarium
