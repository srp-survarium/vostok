////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_RENDERER_H_INCLUDED
#define RENDER_SCENE_RENDERER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::math::frustum;
class vostok::memory::base_allocator;
class vostok::render::engine::world;
class vostok::render::one_way_render_channel;
class vostok::math::uint2;
class vostok::particle::enum_particle_render_mode;
class vostok::render::base_output_window;
class vostok::render::base_scene;
class vostok::render::base_scene_view;
class vostok::render::enum_render_stage_type;
class vostok::render::render_model_instance;
class vostok::render::scene_view_mode;
class vostok::render::speedtree_instance;
class vostok::render::tracer_model_instance;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::particle::world;
class vostok::render::ambient_volume_properties;
class vostok::render::cloud_key_parameters;
class vostok::render::cloud_parameters;
class vostok::render::decal_properties;
class vostok::render::environment_probe_properties;
class vostok::render::grass_layer_data;
class vostok::render::grass_layer_desc;
class vostok::render::light_props;
class vostok::render::sky_ambient_occlusion_properties;
class vostok::render::trample_desc;
class vostok::render::volume_fog_parameters;
class vostok::vectora<vostok::render::buffer_fragment>;

namespace vostok {
namespace render {

class scene_renderer : public core::noncopyable {
								scene_renderer						(
									one_way_render_channel&		channel,
									memory::base_allocator&		allocator,
									engine::world&				render_engine_world,
									math::frustum*				frustum_listener
								);

public:
			void				reload_shaders						( );
			void				reload_modified_textures			( );

	inline	math::uint2			window_client_size					( base_output_window_ptr const& arg_0 ) { /* no source */ }

			void				set_view_matrix						( base_scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix );
			void				set_projection_matrix				( base_scene_view_ptr const& scene_view, float4x4 const& projection );

			void				begin_render_options_changing		( long volatile* waiting_for );
			void				end_render_options_changing			(
									base_scene_ptr const&		scene,
									base_output_window_ptr		output_window,
									bool						reload_all_materials,
									bool						shaders_recompile,
									long volatile*				waiting_for
								);

			void				reset_renderer						( );

			void				add_model							(
									base_scene_ptr const&				scene,
									render_model_instance_ptr const&	v,
									float4x4 const&						transform
								);
			void				update_model						(
									base_scene_ptr const&				scene,
									render_model_instance_ptr const&	render_model,
									float4x4 const&						model_to_world
								);
			void				remove_model						( base_scene_ptr const& scene, render_model_instance_ptr const& model );

	inline	void				update_model_vertex_buffer			( render_model_instance_ptr const& arg_0, vectora< buffer_fragment > const& arg_1 ) { /* no source */ }
			void				update_skeleton						( render_model_instance_ptr const& v, float4x4 const* matrices, u32 count );

	inline	void				set_model_ghost_mode				( render_model_instance_ptr const& arg_0, bool arg_1 ) { /* no source */ }
	inline	void				set_speedtree_instance_material		(
									speedtree_instance_ptr const&			arg_0,
									fs_new::virtual_path_string const&		arg_1,
									resources::unmanaged_resource_ptr const&	arg_2
								) { /* no source */ }
	inline	void				set_model_material					(
									render_model_instance_ptr const&		arg_0,
									fs_new::virtual_path_string const&		arg_1,
									resources::unmanaged_resource_ptr const&	arg_2
								) { /* no source */ }
	inline	void				set_model_visible					(
									render_model_instance_ptr const&		arg_0,
									fs_new::virtual_path_string const&		arg_1,
									u32										arg_2
								) { /* no source */ }
			void				set_model_visible					( render_model_instance_ptr const& v, u32 surface_id, u32 flags );
	inline	void				set_model_lod_params				(
									render_model_instance_ptr const&	arg_0,
									u8									arg_1,
									bool								arg_2,
									float								arg_3,
									float								arg_4,
									float								arg_5
								) { /* no source */ }

			void				build_lpv_geometry					( base_scene_ptr const& scene );

	inline	void				remove_unused_environment_cubemaps	( base_scene_ptr const& arg_0 ) { /* no source */ }

			void				add_vegetation_trample				( base_scene_ptr const& scene, trample_desc const& desc );
	inline	void				add_speedtree_instance				(
									base_scene_ptr const&				arg_0,
									speedtree_instance_ptr const&		arg_1,
									float4x4 const&						arg_2,
									bool								arg_3
								) { /* no source */ }
	inline	void				remove_speedtree_instance			( base_scene_ptr const& arg_0, speedtree_instance_ptr const& arg_1, bool arg_2 ) { /* no source */ }
	inline	void				update_speedtree_instance			(
									base_scene_ptr const&				arg_0,
									speedtree_instance_ptr const&		arg_1,
									float4x4 const&						arg_2,
									bool								arg_3
								) { /* no source */ }

	inline	void				populate_speedtree_forest			( base_scene_ptr const& arg_0 ) { /* no source */ }

			void				play_particle_system				(
									base_scene_ptr const&		scene,
									const resources::unmanaged_resource_ptr	arg_1 /* resources::unmanaged_resource_ptr in_instance */,
									float4x4 const&				transform
								);
	inline	void				stop_particle_system				( base_scene_ptr const& arg_0, resources::unmanaged_resource_ptr const& arg_1 ) { /* no source */ }

			void				remove_particle_system_instance		(
									base_scene_ptr const&		scene,
									resources::unmanaged_resource_ptr const&	in_instance
								);
			void				update_particle_system_instance		(
									base_scene_ptr const&		scene,
									resources::unmanaged_resource_ptr const&	instance,
									float4x4 const&				transform,
									bool						visible,
									bool						paused
								);

			bool				is_playing							( resources::unmanaged_resource_ptr const& instance );

	inline	void				set_particles_render_mode			( base_scene_view_ptr const& arg_0, particle::enum_particle_render_mode arg_1 ) { /* no source */ }
	inline	void				set_slomo							( base_scene_ptr const& arg_0, float arg_1 ) { /* no source */ }
			void				set_gamma_correction_factor			( const float value );

			void				add_light							( base_scene_ptr const& scene, u32 id, light_props* props );
			void				update_light						( base_scene_ptr const& scene, u32 id, light_props* props );
			void				remove_light						( base_scene_ptr const& scene, u32 id );

	inline	void				add_decal							( base_scene_ptr const& arg_0, u32 arg_1, decal_properties const& arg_2 ) { /* no source */ }
			void				update_decal						( base_scene_ptr const& scene, u32 id, decal_properties const& properties );
			void				remove_decal						( base_scene_ptr const& scene, u32 id );

			void				update_environment_probe			(
									base_scene_ptr const&					scene,
									u32										id,
									environment_probe_properties const&		properties
								);
			void				remove_environment_probe			( base_scene_ptr const& scene, u32 id );

			void				update_sky_ambient_occlusion		(
									base_scene_ptr const&		scene,
									u32							id,
									sky_ambient_occlusion_properties const&	properties
								);
			void				remove_sky_ambient_occlusion		( base_scene_ptr const& scene, u32 id );

			void				update_ambient_volume				( base_scene_ptr const& scene, u32 id, ambient_volume_properties const& properties );
			void				remove_ambient_volume				( base_scene_ptr const& scene, u32 id );

			void				add_tracer							(
									base_scene_ptr const&				scene,
									tracer_model_instance_ptr const&	instance,
									float4x4 const&						initialize_transform
								);
			void				update_tracer						(
									base_scene_ptr const&				scene,
									tracer_model_instance_ptr const&	instance,
									float4x4 const&						new_transform
								);
			void				remove_tracer						( base_scene_ptr const& scene, tracer_model_instance_ptr const& instance );

			void				update_lpv_occluder					( base_scene_ptr const& scene, u32 id, float4x4 const& transform );
			void				remove_lpv_occluder					( base_scene_ptr const& scene, u32 id );

	inline	void				add_volume_fog						( base_scene_ptr const& arg_0, u32 arg_1, volume_fog_parameters const& arg_2 ) { /* no source */ }
			void				update_volume_fog					( base_scene_ptr const& scene, u32 id, volume_fog_parameters const& in_parameters );
			void				remove_volume_fog					( base_scene_ptr const& scene, u32 id );

	inline	void				add_clouds							( base_scene_ptr const& arg_0, cloud_parameters const& arg_1 ) { /* no source */ }
	inline	void				update_clouds						( base_scene_ptr const& arg_0, cloud_parameters const& arg_1 ) { /* no source */ }
	inline	void				remove_clouds						( base_scene_ptr const& arg_0 ) { /* no source */ }

	inline	void				set_num_clouds_keys					( base_scene_ptr const& arg_0, const u32 arg_1 ) { /* no source */ }

	inline	void				apply_clouds_changes				( base_scene_ptr const& arg_0 ) { /* no source */ }

	inline	void				set_clouds_key						( base_scene_ptr const& arg_0, const u32 arg_1, cloud_key_parameters const& arg_2 ) { /* no source */ }
	inline	void				set_clouds_time						( base_scene_ptr const& arg_0, const float arg_1 ) { /* no source */ }
	inline	void				set_editor_mode						( base_scene_ptr const& arg_0, bool arg_1 ) { /* no source */ }
			void				set_post_process					(
									base_scene_view_ptr const&		scene_view,
									resources::unmanaged_resource_ptr const&	post_process_resource
								);
	inline	void				enable_post_process					( base_scene_view_ptr const& arg_0, bool arg_1 ) { /* no source */ }

	inline	void				set_view_mode						( base_scene_view_ptr const& arg_0, scene_view_mode arg_1 ) { /* no source */ }

	inline	void				toggle_render_stage					( enum_render_stage_type arg_0, bool arg_1 ) { /* no source */ }

			particle::world&	particle_world						( base_scene_ptr const& scene );

			void				set_sky_material					( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& mtl_ptr );
			void				set_portal_system					( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& pss_ptr );
	inline	void				test_action_portal_system			( base_scene_ptr const& arg_0 ) { /* no source */ }

	inline	void				clear_grass							( base_scene_ptr const& arg_0 ) { /* no source */ }
	inline	void				populate_grass						( base_scene_ptr const& arg_0 ) { /* no source */ }

	inline	void				add_grass_layer						( grass_layer_desc* arg_0, grass_layer_data* arg_1, base_scene_ptr const& arg_2 ) { /* no source */ }
	inline	void				update_grass_layer					(
									grass_layer_desc*			arg_0,
									grass_layer_data*			arg_1,
									base_scene_ptr const&		arg_2,
									bool						arg_3
								) { /* no source */ }
	inline	void				remove_grass_layer					( u8 arg_0, base_scene_ptr const& arg_1 ) { /* no source */ }

			void				set_grass							( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene );
			void				reset_grass							( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene );

	inline	void				set_fixed_lod						( u32 arg_0, base_scene& arg_1 ) { /* no source */ }
	inline	u32					get_fixed_lod						( base_scene const& arg_0 ) { /* no source */ }

private:
	inline	void				draw_present_impl					( ) { /* no source */ }

public:
	inline						~scene_renderer						( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	engine::world&				m_render_engine_world;
	/* 0x0004 */	one_way_render_channel&		m_channel;
	/* 0x0008 */	memory::base_allocator&		m_allocator;
	/* 0x000c */	math::frustum* const		m_frustum_listener;
	/* 0x0010 */	float4x4					m_view;
	/* 0x0050 */	float4x4					m_projection;
}; // class scene_renderer

STATIC_SIZE_ASSERT(scene_renderer, 0x90);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_RENDERER_H_INCLUDED
