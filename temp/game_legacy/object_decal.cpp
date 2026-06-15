////////////////////////////////////////////////////////////////////////////
//	Created		: 07.07.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_decal.h"
#include "game_world.h"
#include <vostok/render/world.h>

#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/collision/collision_object.h>

namespace survarium{

object_decal::~object_decal()
{
	m_game_scene.renderer().scene().remove_decal(m_game_scene.get_render_scene(), m_decal_id);
}

void object_decal::load( configs::binary_config_value const& t )
{
	super::load							(t);
	
	m_projection_on_static_geometry		= t.value_exists("projection_on_static_geometry") ? 
										  bool(t["projection_on_static_geometry"]) : true;
	
	m_projection_on_skeleton_geometry	= t.value_exists("projection_on_skeleton_geometry") ? 
										  bool(t["projection_on_skeleton_geometry"]) : true;
	
	m_projection_on_terrain_geometry	= t.value_exists("projection_on_terrain_geometry") ? 
										  bool(t["projection_on_terrain_geometry"]) : true;

	m_projection_on_speedtree_geometry	= t.value_exists("projection_on_speedtree_geometry") ? 
										  bool(t["projection_on_speedtree_geometry"]) : true;

	m_projection_on_particle_geometry	= t.value_exists("projection_on_particle_geometry") ? 
										  bool(t["projection_on_particle_geometry"]) : true;
	
	m_alpha_angle						= t.value_exists("alpha_angle") ? 
										  float(t["alpha_angle"]) : -90.0f;
	
	m_clip_angle						= t.value_exists("clip_angle") ? 
										  float(t["clip_angle"]) : -90.0f;
	
	m_decal_far_distance				= t.value_exists("decal_far_distance") ? 
										  float(t["decal_far_distance"]) : 1.0f;
	
	m_decal_width						= t.value_exists("decal_width") ? 
										  float(t["decal_width"]) : 1.0f;
	
	m_decal_height						= t.value_exists("decal_height") ? 
										  float(t["decal_height"]) : 1.0f;
	
	pcstr decal_material_name			= pcstr(t["decal_material"]);
	
	vostok::render::material_effects_instance_cook_data* cook_data = 
		NEW(vostok::render::material_effects_instance_cook_data)(vostok::render::decal_vertex_input_type, NULL, false);
	
	resources::user_data_variant		user_data;
	user_data.set						(cook_data);
	
	resources::query_resource			(
		decal_material_name, 
		resources::material_effects_instance_class, 
		boost::bind(
			&object_decal::material_ready, 
			this, 
			_1,
			cook_data
		),
		g_allocator,
		&user_data
	);
}

void object_decal::material_ready( resources::queries_result& data, vostok::render::material_effects_instance_cook_data* cook_data )
{
	DELETE											(cook_data);
	
	if(data.is_successful())
	{
		m_game_scene.renderer().scene().update_decal(
			m_game_scene.get_render_scene(), 
			m_decal_id, 
			vostok::render::decal_properties(
				m_transform,
				data[0].get_unmanaged_resource(),
				float3(m_decal_width, m_decal_height, m_decal_far_distance),
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
}

} // namespace survarium