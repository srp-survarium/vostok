#ifndef VOSTOK_RENDER_FACADE_SCENE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SCENE_RENDERER_H_INCLUDED

#include <boost/bind.hpp>
#include <vostok/render/facade/model.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/engine/sources/cloud_parameters.h>
#include <vostok/render/facade/cloud_key.h>
#include <vostok/render/facade/one_way_render_channel.h>
#include <vostok/render/facade/volume_fog_parameters.h>
#include <vostok/render/facade/sources/editor_allocator.h>
#include <vostok/render/facade/sources/functor_command.h>
#include <vostok/render/facade/sources/functor_with_big_buffer_to_copy_command.h>
#include <vostok/render/facade/sources/scene_renderer.h>
#include <vostok/render/facade/sources/update_model_vertex_buffer_command.h>

namespace vostok {
namespace render {

inline void scene_renderer::update_model_vertex_buffer(
	render_model_instance_ptr const& object,
	vectora< buffer_fragment > const& fragments
)
{
	m_channel.owner_push_back(
		VOSTOK_NEW_IMPL( m_allocator, update_model_vertex_buffer_command )(
			m_render_engine_world,
			object,
			fragments,
			*vostok::render::editor::g_allocator
		)
	);
}

inline void scene_renderer::set_speedtree_instance_material(
	speedtree_instance_ptr const&			instance,
	fs_new::virtual_path_string const&	subsurface_name,
	resources::unmanaged_resource_ptr const&	material
)
{
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< fs_new::virtual_path_string > ) (
			boost::bind(
				&vostok::render::engine::world::set_speedtree_instance_material,
				&m_render_engine_world,
				instance,
				_1,
				material
			),
			subsurface_name
		)
	);
}

inline void scene_renderer::set_model_material(
	render_model_instance_ptr const&		model,
	fs_new::virtual_path_string const&	subsurface_name,
	resources::unmanaged_resource_ptr const&	material
)
{
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< fs_new::virtual_path_string > ) (
			boost::bind(
				&vostok::render::engine::world::set_model_material,
				&m_render_engine_world,
				model,
				_1,
				material
			),
			subsurface_name
		)
	);
}

inline void scene_renderer::set_model_visible(
	render_model_instance_ptr const&		model,
	fs_new::virtual_path_string const&	subsurface_name,
	u32								flags
)
{
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< fs_new::virtual_path_string > ) (
			boost::bind(
				&vostok::render::engine::world::set_model_visible,
				&m_render_engine_world,
				model,
				_1,
				flags
			),
			subsurface_name
		)
	);
}

inline void scene_renderer::set_model_lod_params(
	render_model_instance_ptr const&	model,
	u8								type,
	bool							use_default,
	float							p0,
	float							p1,
	float							p2
)
{
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::set_model_lod_params,
				&m_render_engine_world,
				model,
				type,
				use_default,
				p0,
				p1,
				p2
			)
		)
	);
}

inline void scene_renderer::remove_unused_environment_cubemaps( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_unused_environment_cubemaps,
				&m_render_engine_world,
				scene
			)
		)
	);
}

inline void scene_renderer::add_speedtree_instance(
	base_scene_ptr const&			scene,
	speedtree_instance_ptr const&	instance,
	float4x4 const&				transform,
	bool						populate_forest
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::add_speedtree_instance,
				&m_render_engine_world,
				scene,
				instance,
				_1,
				populate_forest
			),
			transform
		)
	);
}

inline void scene_renderer::remove_speedtree_instance(
	base_scene_ptr const&			scene,
	speedtree_instance_ptr const&	instance,
	bool						populate_forest
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_speedtree_instance,
				&m_render_engine_world,
				scene,
				instance,
				populate_forest
			)
		)
	);
}

inline void scene_renderer::update_speedtree_instance(
	base_scene_ptr const&			scene,
	speedtree_instance_ptr const&	instance,
	float4x4 const&				transform,
	bool						populate_forest
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::update_speedtree_instance,
				&m_render_engine_world,
				scene,
				instance,
				_1,
				populate_forest
			),
			transform
		)
	);
}

inline void scene_renderer::populate_speedtree_forest( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::populate_speedtree_forest,
				&m_render_engine_world,
				scene
			)
		)
	);
}

inline void scene_renderer::add_volume_fog(
	base_scene_ptr const&			scene,
	u32							id,
	volume_fog_parameters const&	parameters
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< volume_fog_parameters > ) (
			boost::bind(
				&vostok::render::engine::world::add_volume_fog,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			parameters
		)
	);
}

inline void scene_renderer::add_clouds( base_scene_ptr const& scene, cloud_parameters const& parameters )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< cloud_parameters > ) (
			boost::bind(
				&vostok::render::engine::world::add_clouds,
				&m_render_engine_world,
				scene,
				_1
			),
			parameters
		)
	);
}

inline void scene_renderer::update_clouds( base_scene_ptr const& scene, cloud_parameters const& parameters )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< cloud_parameters > ) (
			boost::bind(
				&vostok::render::engine::world::update_clouds,
				&m_render_engine_world,
				scene,
				_1
			),
			parameters
		)
	);
}

inline void scene_renderer::remove_clouds( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_clouds,
				&m_render_engine_world,
				scene
			)
		)
	);
}

inline void scene_renderer::set_num_clouds_keys( base_scene_ptr const& scene, const u32 num_keys )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::set_num_clouds_keys,
				&m_render_engine_world,
				scene,
				num_keys
			)
		)
	);
}

inline void scene_renderer::apply_clouds_changes( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::apply_clouds_changes,
				&m_render_engine_world,
				scene
			)
		)
	);
}

inline void scene_renderer::set_clouds_key(
	base_scene_ptr const&			scene,
	const u32					index,
	cloud_key_parameters const&	parameters
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< cloud_key_parameters > ) (
			boost::bind(
				&vostok::render::engine::world::set_clouds_key,
				&m_render_engine_world,
				scene,
				index,
				_1
			),
			parameters
		)
	);
}

inline void scene_renderer::set_clouds_time( base_scene_ptr const& scene, const float time )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::set_clouds_time,
				&m_render_engine_world,
				scene,
				time
			)
		)
	);
}

inline void scene_renderer::set_editor_mode( base_scene_ptr const& scene, bool is_editor_mode )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::set_editor_mode,
				&m_render_engine_world,
				scene,
				is_editor_mode
			)
		)
	);
}

inline void scene_renderer::clear_grass( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::clear_grass,
				&m_render_engine_world,
				scene
			)
		)
	);
}

inline void scene_renderer::populate_grass( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::populate_grass,
				&m_render_engine_world,
				scene
			)
		)
	);
}

inline void scene_renderer::add_grass_layer(
	grass_layer_desc*		desc,
	grass_layer_data*		data,
	base_scene_ptr const&	scene
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::add_grass_layer,
				&m_render_engine_world,
				desc,
				data,
				scene
			)
		)
	);
}

inline void scene_renderer::update_grass_layer(
	grass_layer_desc*		desc,
	grass_layer_data*		data,
	base_scene_ptr const&	scene,
	bool					is_set
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::update_grass_layer,
				&m_render_engine_world,
				desc,
				data,
				scene,
				is_set
			)
		)
	);
}

inline void scene_renderer::remove_grass_layer( u8 id, base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_grass_layer,
				&m_render_engine_world,
				id,
				scene
			)
		)
	);
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SCENE_RENDERER_H_INCLUDED
