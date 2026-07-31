////////////////////////////////////////////////////////////////////////////
//	Created		: 31.08.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/backend.h>
#include "scene.h"
//#include "model_manager.h"
#include "lights_db.h"
#include "terrain.h"
#include "speedtree.h"
#include "speedtree_forest.h"
#include "speedtree_tree.h"
#include "speedtree_instance_impl.h"
#include "grass_world.h"
#include "render_particle_emitter_instance.h"
#include <vostok/particle/world.h>
#include "light.h"
#include "statistics.h"
#include "system_renderer.h"
#include <vostok/render/engine/world.h>
#include "decal_instance.h"
#include "material.h"
#include "material_manager.h"
#include <vostok/collision/space_partitioning_tree.h>

#include <vostok/render/core/options.h>

#include <vostok/render/core/res_geometry.h>

#include "render_model_grass.h"

static vostok::command_line::key s_grass_key( "grass", "", "", "grass test" );
static bool is_grass_loaded	= false;


namespace vostok {
namespace render {

// void scene::on_particle_world_created(resources::queries_result& result)
// {
// 	m_particle_world	= static_cast_checked<particle::world*>(result[0].get_unmanaged_resource().c_ptr());
// 	m_particle_engine.set_particle_world( *m_particle_world );
// }

static int test_grass = 0;

void scene::select_terrain_cells	( float4x4 const & mat_vp, render::vector<terrain_render_model_instance_ptr> & cells)
{
	ASSERT( m_terrain);
	m_terrain->select_cells( mat_vp, cells);
}



void scene::select_particle_system_instance	( particle::particle_system_instance_ptr const& instance, bool const is_selected )
{
	particle_system_instances_type::iterator const found	= std::find( m_particle_system_instances.begin(), m_particle_system_instances.end(), instance );
	if ( found == m_particle_system_instances.end() ) {
		if ( is_selected )
			m_particle_system_instances.push_back ( instance );

		return;
	}

	if ( !is_selected )
		m_particle_system_instances.erase	( found );
}

void scene::select_speedtree_instance		( speedtree_instance_ptr const& instance, bool const is_selected )
{
	speedtree_instances_type::iterator const found	= std::find( m_speedtree_instances.begin(), m_speedtree_instances.end(), instance );
	if ( found == m_speedtree_instances.end() ) {
		if ( is_selected )
			m_speedtree_instances.push_back	( instance );

		return;
	}

	if ( !is_selected )
		m_speedtree_instances.erase			( found );
}

} // namespace render
} // namespace vostok
