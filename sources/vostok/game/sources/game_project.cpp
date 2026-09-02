// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_project.h"

#include "game_memory.h"				// g_allocator (m_render_visuals array delete)
#include <vostok/resources_query_result.h>
#include <vostok/resources_queries_result.h>

#include "render_visual.h"
#include "base_game_scene.h"			// scene_renderer() / render_scene() / get_physics_world()
#include <vostok/render/facade/scene_renderer.h>	// scene_renderer::add_model/remove_model
#include "game_object_.h"				// (*it)->insert()/remove() (m_objects loop)
#include "damage_zone.h"				// damage_zone_core activate/deactivate (m_damage_zones loop)
#include "victory_items_container.h"	// usable_object insert/remove + id() (m_victory_items_containers loop)
#include <vostok/game_core/static_collision.h>		// static_collision::insert/remove
#include <vostok/game_core/ladder.h>				// ladder activate/deactivate (m_ladders loop)
#include <vostok/game_core/generic_anomaly_core.h>	// anomaly activate/deactivate (m_anomalies loop)
#include <vostok/sound/sound_emitter.h>				// sound_emitter_ptr (on_npc_loaded)

namespace survarium {

simple_game_project::simple_game_project( base_game_scene& s ) :
	resources::unmanaged_resource( 1 ),
	m_game_scene( s ),
	m_config( NULL ),
	m_objects( ),
	m_respawn_points( ),
	m_collision_geometries( ),
	m_damage_zones( ),
	m_artefact_containers( ),
	m_ladders( ),
	m_anomalies( ),
	m_victory_items_containers( ),
	m_render_visuals( NULL ),
	m_is_inserted( false )
{
	// claude@NOTE: Target structure matches; residual bytes depend on the owning caller/compiler context.
}

simple_game_project::~simple_game_project( )
{
	// claude@NOTE: Remaining bytes are implicit array/member/base destruction codegen.
	if ( m_render_visuals )
		VOSTOK_DELETE_ARRAY_IMPL( g_allocator, m_render_visuals );
}

bool simple_game_project::all_loaded( )
{
	return	m_loaded.visuals_loaded			&&
			m_loaded.collision_loaded		&&
			m_loaded.loaded_count == m_objects.size( )	&&
			m_loaded.all_queried;
}

void simple_game_project::on_npc_loaded( resources::queries_result& data )
{
	// claude@NOTE: Target elides the getter's unmanaged pointer temporary; the isolated base build retains it.
	static_cast_resource_ptr< sound::sound_emitter_ptr >( data[0].get_unmanaged_resource( ) );
}

void simple_game_project::insert( scheduler& scheduler )
{
	// claude@NOTE: Residual bytes are the target's caller-threaded scheduler ABI, not loop source shape.
	for ( u32 i = 0; i < m_render_visuals_count; ++i )
		m_render_visuals[i].insert( m_game_scene );

	for ( u32 i = 0; i < m_static_collision_objects_count; ++i )
		m_static_collision_objects[i].insert( m_game_scene.get_physics_world( ) );

	for ( vector< game_object_* >::iterator i = m_objects.begin( ),
		e = m_objects.end( );
		i != e;
		++i )
		( *i )->
			insert( );

	for ( vector< damage_zone_ptr >::iterator i = m_damage_zones.begin( ),
		e = m_damage_zones.end( );
		i != e;
		++i )
		if ( ( *i )->is_standalone( ) )
			( *i )->activate( NULL, m_game_scene.get_physics_world( ), scheduler );

	for ( vector< generic_anomaly_core* >::iterator i = m_anomalies.begin( ),
		e = m_anomalies.end( );
		i != e;
		++i )
		( *i )->activate( m_game_scene.get_physics_world( ), scheduler );

	for ( vector< victory_items_container* >::iterator i = m_victory_items_containers.begin( ),
		e = m_victory_items_containers.end( );
		i != e;
		++i )
		( *i )->insert( m_game_scene.get_physics_world( ) );

	for ( vector< ladder_ptr >::iterator i = m_ladders.begin( ),
		e = m_ladders.end( );
		i != e;
		++i )
		( *i )->activate( m_game_scene.get_physics_world( ) );

	m_is_inserted = true;
}

void simple_game_project::remove( )
{
	for ( u32 i = 0; i < m_render_visuals_count; ++i )
		m_render_visuals[i].remove( m_game_scene );

	for ( u32 i = 0; i < m_static_collision_objects_count; ++i )
		m_static_collision_objects[i].remove( m_game_scene.get_physics_world( ) );

	for ( vector< ladder_ptr >::iterator i = m_ladders.begin( ),
		e = m_ladders.end( );
		i != e;
		++i )
		( *i )->deactivate( );

	for ( vector< generic_anomaly_core* >::iterator i = m_anomalies.begin( ),
		e = m_anomalies.end( );
		i != e;
		++i )
		( *i )->deactivate( );

	for ( vector< damage_zone_ptr >::iterator i = m_damage_zones.begin( ),
		e = m_damage_zones.end( );
		i != e;
		++i )
		if ( ( *i )->is_standalone( ) )
			( *i )->deactivate( );

	for ( vector< game_object_* >::iterator i = m_objects.begin( ),
		e = m_objects.end( );
		i != e;
		++i )
		( *i )->
			remove( );

	for ( vector< victory_items_container* >::iterator i = m_victory_items_containers.begin( ),
		e = m_victory_items_containers.end( );
		i != e;
		++i )
		( *i )->remove( );

	m_is_inserted = false;
}

void render_visual::insert( base_game_scene& w )
{
	if ( model.c_ptr( ) )
		w.scene_renderer( ).add_model( w.render_scene( ), model->m_render_model, matrix );
}

void render_visual::remove( base_game_scene& w )
{
	if ( model.c_ptr( ) )
		w.scene_renderer( ).remove_model( w.render_scene( ), model->m_render_model );
}

victory_items_container_core* simple_game_project::get_items_container( u8 id )
{
	// claude@NOTE: Source shape matches; residual is target caller-context register and return-epilogue allocation.
	for ( vector< victory_items_container* >::iterator i = m_victory_items_containers.begin( ),
		e = m_victory_items_containers.end( );
		i != e;
		++i )
		if ( ( *i )->id( ) == id )
			return *i;

	return NULL;
}

} // namespace survarium
