////////////////////////////////////////////////////////////////////////////
//	Created		: 26.05.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/light_props.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/render/facade/one_way_render_channel.h>
#include "functor_command.h"
#include "functor_with_big_buffer_to_copy_command.h"
#include "update_skeleton_command.h"
#include "update_model_vertex_buffer_command.h"
#include <vostok/ui/world.h>

using vostok::render::scene_renderer;
using vostok::render::scene_ptr;
using vostok::render::scene_view_ptr;
using vostok::render::render_output_window_ptr;
using vostok::render::render_model_instance_ptr;
using vostok::math::float4x4;
using vostok::particle::particle_system_instance_ptr;

namespace vostok {
namespace render {

void defer_execution	( vostok::render::base_command& command, vostok::render::scene_view_ptr const& scene_view );

} // namespace render
} // namespace vostok

scene_renderer::scene_renderer	(
		vostok::render::one_way_render_channel&	channel,
		vostok::memory::base_allocator&			allocator,
		vostok::render::engine::world&			render_engine_world,
		vostok::math::frustum*					frustum_listener
	) :
	m_render_engine_world	( render_engine_world ),
	m_channel				( channel ),
	m_allocator				( allocator ),
	m_frustum_listener		( frustum_listener ),
	m_view					( math::float4x4().identity() ),
	m_projection			( math::create_perspective_projection( math::pi_d2, 4.f/3.f, .1f, 1000.f ) )
{
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4(m_view, m_projection) );
}

void scene_renderer::set_view_matrix			( scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix )
{
	R_ASSERT				( scene_view );
	m_view					= view_and_culling_matrix;
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4( m_view, m_projection ) );

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::set_view_matrix,
				&m_render_engine_world,
				scene_view,
				_1
			),
			view_and_culling_matrix
		)
	);
}

#ifndef MASTER_GOLD
void scene_renderer::set_view_matrix_only	( scene_view_ptr const& scene_view, float4x4 const& view )
{
	R_ASSERT		( scene_view );
	m_view			= view;
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4( m_view, m_projection ) );

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::set_view_matrix_only,
				&m_render_engine_world,
				scene_view,
				_1
			),
			view
		)
	);
}
#endif // #ifndef MASTER_GOLD

void scene_renderer::set_projection_matrix	( scene_view_ptr const& scene_view, float4x4 const& projection )
{
	R_ASSERT	( scene_view );
	m_projection = projection;
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4( m_view, m_projection ) );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::set_projection_matrix,
				&m_render_engine_world,
				scene_view,
				_1
			),
			projection
		)
	);
}

void scene_renderer::add_model	( scene_ptr const& scene, render_model_instance_ptr const& v, float4x4 const& transform )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) ( boost::bind( &vostok::render::engine::world::add_model, &m_render_engine_world, scene, v, _1, true), transform ) );
}

void scene_renderer::remove_model( scene_ptr const& scene, render_model_instance_ptr const& model ) 
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::remove_model, &m_render_engine_world, scene, model)));
}

void scene_renderer::update_model( scene_ptr const& scene, render_model_instance_ptr const& render_model, float4x4 const& model_to_world ) 
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) ( boost::bind( &vostok::render::engine::world::update_model, &m_render_engine_world, scene, render_model, _1), model_to_world));
}

void scene_renderer::update_skeleton( render_model_instance_ptr const& v, float4x4 const* matrices, u32 count )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, update_skeleton_command ) ( m_render_engine_world, v, matrices, count ) );
}

void scene_renderer::play_particle_system			( scene_ptr const& scene, particle_system_instance_ptr const& in_instance, vostok::float4x4 const& transform )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::play_particle_system,
				&m_render_engine_world,
				scene,
				in_instance,
				true,
				false,
				_1
			),
			transform
		)
	);
}

void scene_renderer::stop_particle_system( scene_ptr const& scene, particle_system_instance_ptr const& in_instance )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::stop_particle_system, &m_render_engine_world, scene, in_instance) ));
}

void scene_renderer::remove_particle_system_instance( scene_ptr const& scene, particle_system_instance_ptr const& in_instance )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::remove_particle_system_instance, &m_render_engine_world, in_instance, scene) ));
}

void scene_renderer::update_particle_system_instance( scene_ptr const& scene, particle_system_instance_ptr const& instance, vostok::float4x4 const& transform, bool visible, bool paused )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< math::float4x4 > ) ( boost::bind( &engine::world::update_particle_system_instance, &m_render_engine_world, instance, scene, _1, visible, paused), transform ) );
}

void scene_renderer::update_model_vertex_buffer( render_model_instance_ptr const& object, vostok::vectora<vostok::render::buffer_fragment> const& fragments )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, update_model_vertex_buffer_command ) ( m_render_engine_world, object, fragments, *vostok::render::editor::g_allocator ) );
}

void scene_renderer::add_decal( scene_ptr const& scene, u32 id, vostok::render::decal_properties const& properties)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< decal_properties > ) (
			boost::bind(
				&vostok::render::engine::world::add_decal,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			properties
		)
	);
}

void scene_renderer::update_decal( scene_ptr const& scene, u32 id, vostok::render::decal_properties const& properties)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< decal_properties > ) (
			boost::bind(
				&vostok::render::engine::world::update_decal,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			properties
		)
	);
}

void scene_renderer::remove_decal( scene_ptr const& scene, u32 id)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_decal,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

void scene_renderer::add_light( scene_ptr const& scene, u32 id, vostok::render::light_props const& props)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< light_props > ) (
			boost::bind(
				&vostok::render::engine::world::add_light,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			props
		)
	);
}

void scene_renderer::update_light( scene_ptr const& scene, u32 id, vostok::render::light_props const& props)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< light_props > ) (
			boost::bind(
				&vostok::render::engine::world::update_light,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			props
		)
	);
}

void scene_renderer::remove_light( scene_ptr const& scene, u32 id)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_light,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

void scene_renderer::terrain_add_cell( scene_ptr const& scene, render_model_instance_ptr const& v )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command) ( boost::bind(&render::engine::world::terrain_add_cell, &m_render_engine_world, scene, v)) );
}

void scene_renderer::terrain_remove_cell( scene_ptr const& scene, render_model_instance_ptr const& v )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command) ( boost::bind(&render::engine::world::terrain_remove_cell, &m_render_engine_world, scene, v)) );
}

void scene_renderer::set_slomo					( scene_ptr const& scene, float time_multiplier )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_slomo, &m_render_engine_world, scene, time_multiplier) ));
}

void scene_renderer::set_post_process			( scene_view_ptr const& scene_view, vostok::resources::unmanaged_resource_ptr const& post_process_resource)
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_post_process, &m_render_engine_world, scene_view, post_process_resource) ));
}

void scene_renderer::reload_shaders			( )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::reload_shaders, &m_render_engine_world) ) );
}

void scene_renderer::reload_modified_textures	( )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::reload_modified_textures, &m_render_engine_world) ) );
}

void scene_renderer::set_speedtree_instance_material ( 
	vostok::render::speedtree_instance_ptr const& v, 
	vostok::fs_new::virtual_path_string const& subsurface_name, 
	vostok::resources::unmanaged_resource_ptr const& mtl_ptr)
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command<fs_new::virtual_path_string>) ( boost::bind( &render::engine::world::set_speedtree_instance_material, &m_render_engine_world, v, _1, mtl_ptr), subsurface_name ) );
}

void scene_renderer::set_sky_material ( scene_ptr const& scene, vostok::resources::unmanaged_resource_ptr const& mtl_ptr)
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_sky_material, &m_render_engine_world, scene, mtl_ptr) ));
}

void scene_renderer::set_model_material(
		render_model_instance_ptr const& v,
		vostok::fs_new::virtual_path_string const& subsurface_name,
		vostok::resources::unmanaged_resource_ptr const& mtl_ptr
	)
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command<fs_new::virtual_path_string>) ( boost::bind( &render::engine::world::set_model_material, &m_render_engine_world, v, _1, mtl_ptr), subsurface_name ) );
}

void scene_renderer::set_model_visible( render_model_instance_ptr const& v, vostok::fs_new::virtual_path_string const& subsurface_name, bool value)
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command<fs_new::virtual_path_string>) ( boost::bind( &render::engine::world::set_model_visible, &m_render_engine_world, v, _1, value), subsurface_name ) );
}

void scene_renderer::set_view_mode( scene_view_ptr const& scene_view, vostok::render::scene_view_mode view_mode )
{
	R_ASSERT					( scene_view );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_view_mode, &m_render_engine_world, scene_view, view_mode) ));
}

void scene_renderer::toggle_render_stage( vostok::render::enum_render_stage_type stage_type, bool toggle )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::toggle_render_stage, &m_render_engine_world, stage_type, toggle) ));
}

void scene_renderer::set_particles_render_mode( scene_view_ptr const& scene_view, vostok::particle::enum_particle_render_mode render_mode )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_particles_render_mode, &m_render_engine_world, scene_view, render_mode) ));
}

void scene_renderer::enable_post_process( scene_view_ptr const& scene_view, bool enable )
{
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::enable_post_process, &m_render_engine_world, scene_view, enable) ));
}

void scene_renderer::set_model_ghost_mode( render_model_instance_ptr const& v, bool b )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_model_ghost_mode, &m_render_engine_world, v, b) ));
}

vostok::particle::world& scene_renderer::particle_world	( scene_ptr const& scene )
{
	R_ASSERT	( scene );
	return 		m_render_engine_world.particle_world( scene );
}

void scene_renderer::add_speedtree_instance( scene_ptr const& scene, vostok::render::speedtree_instance_ptr const& v, vostok::math::float4x4 const& transform, bool populate_forest )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< math::float4x4 > ) ( boost::bind( &engine::world::add_speedtree_instance, &m_render_engine_world, scene, v, _1, populate_forest), transform ) );	
}

void scene_renderer::populate_speedtree_forest( scene_ptr const& scene )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::populate_speedtree_forest, &m_render_engine_world, scene) ));
}

void scene_renderer::remove_speedtree_instance( scene_ptr const& scene, vostok::render::speedtree_instance_ptr const& v, bool populate_forest )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::remove_speedtree_instance, &m_render_engine_world, scene, v, populate_forest) ));
}

vostok::math::uint2 scene_renderer::window_client_size	( vostok::render::render_output_window_ptr const& render_output_window )
{
	return		m_render_engine_world.window_client_size( render_output_window );
}

void scene_renderer::update_speedtree_instance( scene_ptr const& scene, vostok::render::speedtree_instance_ptr const& v, vostok::math::float4x4 const& transform, bool populate_forest )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< math::float4x4 > ) ( boost::bind( &engine::world::update_speedtree_instance, &m_render_engine_world, scene, v, _1, populate_forest), transform ) );
}

#ifndef MASTER_GOLD
void scene_renderer::draw_render_statistics( vostok::ui::world& ui_world )
{
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind( &vostok::render::engine::world::draw_render_statistics, &m_render_engine_world, &ui_world),
			boost::bind( &render::defer_execution, _1, ui_world.get_scene_view() )
		)
	);
}
#endif // #ifndef MASTER_GOLD