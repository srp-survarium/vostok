////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_sky.h"
#include "game_world.h"
#include "game.h"

#include <vostok/render/world.h>

#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/facade/vertex_input_type.h>

namespace survarium{

object_sky::object_sky( game_world& w ):
	super		( w )
{
}

object_sky::~object_sky( )
{
	
}

void object_sky::load( configs::binary_config_value const& t )
{
	super::load					(t);
	pcstr sky_material_name		= pcstr(t["material_name"]);
	
	vostok::render::material_effects_instance_cook_data* cook_data = 
		NEW(vostok::render::material_effects_instance_cook_data)(vostok::render::post_process_vertex_input_type, NULL, false);
	
	resources::user_data_variant		user_data;
	user_data.set						(cook_data);
	
	resources::query_resource			(
		sky_material_name, 
		resources::material_effects_instance_class, 
		boost::bind(
			&object_sky::material_ready, 
			this, 
			_1,
			cook_data
		),
		g_allocator,
		&user_data
	);
}

void object_sky::material_ready( resources::queries_result& data, vostok::render::material_effects_instance_cook_data* cook_data )
{
	DELETE (cook_data);
	
	if(data.is_successful())
	{
		m_game_world.get_game().renderer().scene().set_sky_material(m_game_world.get_game().get_render_scene(), 
																	data[0].get_unmanaged_resource());
	}
}

void object_sky::unload_contents( )
{
	
}

void object_sky::load_contents( )
{
	
}


} // namespace survarium