////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENGINE_WORLD_H_INCLUDED
#define RENDER_ENGINE_WORLD_H_INCLUDED

/* INCLUDES */
class vostok::render::renderer;
class vostok::configs::binary_config;
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
class vostok::ui::font* const;
class survarium::flash_movie_resource;
class survarium::scaleform_render_command;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::math::color;
class vostok::math::rectangle<vostok::math::float2>;
class vostok::particle::world;
class vostok::render::ambient_volume_properties;
class vostok::render::cloud_key_parameters;
class vostok::render::cloud_parameters;
class vostok::render::decal_properties;
class vostok::render::environment_probe_generate_parameters;
class vostok::render::environment_probe_properties;
class vostok::render::grass_layer_data;
class vostok::render::grass_layer_desc;
class vostok::render::light_props;
class vostok::render::sky_ambient_occlusion_map_generate_parameters;
class vostok::render::sky_ambient_occlusion_properties;
class vostok::render::trample_desc;
class vostok::render::ui::vertex;
class vostok::render::vector<unsigned short>;
class vostok::render::vector<vostok::render::vertex_colored>;
class vostok::render::volume_fog_parameters;
class vostok::resources::queries_result;
class vostok::ui::font;
class vostok::vectora<unsigned short>;
class vostok::vectora<vostok::render::buffer_fragment>;
class vostok::vectora<vostok::render::ui::vertex>;
class vostok::vectora<vostok::render::vertex_colored>;
class survarium::flash_text_manager;

namespace vostok {
namespace render {

class engine::world : public boost::noncopyable {
public:
	inline	void				set_renderer_configuration			( fs_new::virtual_path_string const& arg_0, bool arg_1 ) { /* no source */ }

			void				apply_render_options_changes		( );

			void				reset_renderer						( bool async_effects );

private:
			void				on_renderer_configuration_config_loaded( bool async_effects, resources::queries_result& data );

public:
								world								( configs::binary_config_ptr const& in_config, bool is_editor );
								~world								( );

			void				initialize							( bool is_editor );

			void				show_movie							( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie );
			void				hide_movie							( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie );

			void				show_text_manager					( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm );
			void				hide_text_manager					( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm );

			void				execute_scaleform_command			( survarium::scaleform_render_command command );

			void				clear_resources						( );

			void				begin_render_options_changing		( long volatile* waiting_for );
			void				end_render_options_changing			(
									base_scene_ptr const&		scene,
									base_output_window_ptr		output_window,
									bool						reload_all_materials,
									bool						shaders_recompile,
									long volatile*				waiting_for
								);

			void				draw_lines							(
									base_scene_ptr const&				scene,
									vectora< vertex_colored > const&	vertices,
									vectora< u16 > const&				indices
								);
			void				draw_triangles						(
									base_scene_ptr const&				scene,
									vectora< vertex_colored > const&	vertices,
									vectora< u16 > const&				indices
								);
	inline	void				draw_text							(
									pcstr					arg_0,
									float2 const&			arg_1,
									ui::font* const			arg_2,
									math::color const&		arg_3
								) { /* no source */ }

			void				set_view_matrix						( base_scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix );
			void				set_projection_matrix				( base_scene_view_ptr const& scene_view, float4x4 const& projection_matrix );

	inline	math::uint2			window_client_size					( base_output_window_ptr const& arg_0 ) { /* no source */ }

	inline	void				draw_debug_lines					( vector< vertex_colored > const& arg_0, vector< u16 > const& arg_1 ) { /* no source */ }
	inline	void				draw_debug_triangles				( vector< vertex_colored > const& arg_0, vector< u16 > const& arg_1 ) { /* no source */ }
	inline	void				draw_editor_lines					( vector< vertex_colored > const& arg_0, vector< u16 > const& arg_1 ) { /* no source */ }
	inline	void				draw_editor_triangles				( vector< vertex_colored > const& arg_0, vector< u16 > const& arg_1 ) { /* no source */ }

	inline	void				setup_grid_render_mode				( u32 arg_0 ) { /* no source */ }
	inline	void				remove_grid_render_mode				( ) { /* no source */ }

	inline	void				setup_rotation_control_modes		( bool arg_0 ) { /* no source */ }

			void				draw_scene							(
									base_scene_ptr const&				scene,
									base_scene_view_ptr const&			view,
									base_output_window_ptr const&		output_window,
									math::rectangle< float2 > const&	viewport,
									boost::function< void( bool ) > const&	on_draw_scene,
									ui::font const*						default_font
								);

	inline	void				pick_lighting_luminance				( const u32 arg_0, const u32 arg_1 ) { /* no source */ }

	inline	void				set_picking_lighting_luminance_mode	( bool arg_0 ) { /* no source */ }

	inline	void				generate_environment_probe			(
									base_scene_ptr const&				arg_0,
									base_scene_view_ptr const&			arg_1,
									base_output_window_ptr const&		arg_2,
									environment_probe_generate_parameters const&	arg_3
								) { /* no source */ }
	inline	void				generate_sky_ao_map					(
									base_scene_ptr const&				arg_0,
									base_scene_view_ptr const&			arg_1,
									base_output_window_ptr const&		arg_2,
									sky_ambient_occlusion_map_generate_parameters const&	arg_3
								) { /* no source */ }

			void				end_frame							( );

			u32					frame_id							( );

			void				reload_shaders						( );
			void				reload_modified_textures			( );

			void				build_lpv_geometry					( base_scene_ptr const& scene );

			void				remove_unused_environment_cubemaps	( base_scene_ptr const& scene );

			void				add_vegetation_trample				( base_scene_ptr const& in_scene, trample_desc const& desc );
			void				add_model							(
									base_scene_ptr const&				in_scene,
									render_model_instance_ptr const&	v,
									float4x4 const&						transform,
									bool								apply_transform
								);
			void				update_model						(
									base_scene_ptr const&				in_scene,
									render_model_instance_ptr const&	v,
									float4x4 const&						transform
								);
			void				remove_model						( base_scene_ptr const& in_scene, render_model_instance_ptr const& v );

			void				add_speedtree_instance				(
									base_scene_ptr const&				in_scene,
									speedtree_instance_ptr const&		v,
									float4x4 const&						transform,
									bool								populate_forest
								);
			void				remove_speedtree_instance			(
									base_scene_ptr const&				in_scene,
									speedtree_instance_ptr const&		v,
									bool								populate_forest
								);
			void				update_speedtree_instance			(
									base_scene_ptr const&				in_scene,
									speedtree_instance_ptr const&		v,
									float4x4 const&						transform,
									bool								populate_forest
								);

			void				populate_speedtree_forest			( base_scene_ptr const& in_scene );

			void				set_speedtree_instance_material		(
									speedtree_instance_ptr const&			v,
									fs_new::virtual_path_string const&		subsurface_name,
									resources::unmanaged_resource_ptr		in_mtl_ptr
								);
			void				set_model_material					(
									render_model_instance_ptr const&		v,
									fs_new::virtual_path_string const&		subsurface_name,
									resources::unmanaged_resource_ptr		m
								);
			void				set_model_visible					(
									render_model_instance_ptr const&		v,
									fs_new::virtual_path_string const&		subsurface_name,
									u32										flags
								);
			void				set_model_visible_by_id				( render_model_instance_ptr const& v, u32 subsurface_id, u32 flags );
			void				set_model_lod_params				(
									render_model_instance_ptr const&	v,
									u8									type,
									bool								use_default,
									float								p0,
									float								p1,
									float								p2
								);

	inline	void				update_system_model					( render_model_instance_ptr const& arg_0, float4x4 const& arg_1 ) { /* no source */ }

			void				set_model_ghost_mode				( render_model_instance_ptr const& v, bool value );

	inline	void				draw_terrain_debug					( ) { /* no source */ }

	inline	void				set_selection_parameters			( float4 const& arg_0, float arg_1 ) { /* no source */ }

			void				update_model_vertex_buffer			( render_model_instance_ptr const& v, vectora< buffer_fragment > const& fragments );
	inline	void				update_model_index_buffer			( render_model_instance_ptr const& arg_0, vectora< buffer_fragment > const& arg_1 ) { /* no source */ }

			void				add_light							( base_scene_ptr const& in_scene, u32 id, light_props* props );
			void				update_light						( base_scene_ptr const& in_scene, u32 id, light_props* props );
			void				remove_light						( base_scene_ptr const& in_scene, u32 id );

			void				add_tracer							(
									base_scene_ptr const&				in_scene,
									tracer_model_instance_ptr const&	instance,
									float4x4 const&						initialize_transform
								);
			void				update_tracer						(
									base_scene_ptr const&				in_scene,
									tracer_model_instance_ptr const&	instance,
									float4x4 const&						new_transform
								);
			void				remove_tracer						( base_scene_ptr const& in_scene, tracer_model_instance_ptr const& instance );

			void				add_decal							( base_scene_ptr const& in_scene, u32 id, decal_properties const& properties );
			void				update_decal						( base_scene_ptr const& in_scene, u32 id, decal_properties const& properties );
			void				remove_decal						( base_scene_ptr const& in_scene, u32 id );

			void				update_environment_probe			(
									base_scene_ptr const&					in_scene,
									u32										id,
									environment_probe_properties const&		properties
								);
			void				remove_environment_probe			( base_scene_ptr const& in_scene, u32 id );

			void				update_sky_ambient_occlusion		(
									base_scene_ptr const&		in_scene,
									u32							id,
									sky_ambient_occlusion_properties const&	properties
								);
			void				remove_sky_ambient_occlusion		( base_scene_ptr const& in_scene, u32 id );

			void				update_ambient_volume				(
									base_scene_ptr const&				in_scene,
									u32									id,
									ambient_volume_properties const&	properties
								);
			void				remove_ambient_volume				( base_scene_ptr const& in_scene, u32 id );

			void				update_lpv_occluder					( base_scene_ptr const& in_scene, u32 id, float4x4 const& transform );
			void				remove_lpv_occluder					( base_scene_ptr const& in_scene, u32 id );

			void				add_volume_fog						(
									base_scene_ptr const&			in_scene,
									u32								id,
									volume_fog_parameters const&	in_parameters
								);
			void				update_volume_fog					(
									base_scene_ptr const&			in_scene,
									u32								id,
									volume_fog_parameters const&	in_parameters
								);
			void				remove_volume_fog					( base_scene_ptr const& in_scene, u32 id );

			void				add_clouds							( base_scene_ptr const& in_scene, cloud_parameters const& parameters );
			void				update_clouds						( base_scene_ptr const& in_scene, cloud_parameters const& parameters );
			void				remove_clouds						( base_scene_ptr const& in_scene );

			void				apply_clouds_changes				( base_scene_ptr const& in_scene );

			void				set_num_clouds_keys					( base_scene_ptr const& in_scene, const u32 num_keys );
			void				set_clouds_key						(
									base_scene_ptr const&			in_scene,
									const u32						index,
									cloud_key_parameters const&		parameters
								);
			void				set_clouds_time						( base_scene_ptr const& in_scene, const float time );
			void				set_editor_mode						( base_scene_ptr const& in_scene, bool is_editor_mode );

			void				update_skeleton						( render_model_instance_ptr const& v, float4x4* matrices, u32 count );

	inline	void				setup_view_and_output				(
									base_scene_view_ptr const&			arg_0,
									base_output_window_ptr const&		arg_1,
									math::rectangle< float2 > const&	arg_2
								) { /* no source */ }

			void				draw_ui_vertices					(
									ui::vertex const*		vertices,
									u32 const&				count,
									s32						prim_type,
									s32						point_type
								);

	inline	void				clear_zbuffer						( float arg_0 ) { /* no source */ }

	inline	void				draw_screen_lines					(
									base_scene_ptr const&		arg_0,
									float2 const*				arg_1,
									u32							arg_2,
									math::color const&			arg_3,
									float						arg_4,
									u32							arg_5
								) { /* no source */ }
	inline	void				draw_3D_screen_lines				(
									base_scene_ptr const&		arg_0,
									float3 const*				arg_1,
									u32							arg_2,
									math::color const&			arg_3,
									float						arg_4,
									u32							arg_5,
									bool						arg_6
								) { /* no source */ }
	inline	void				draw_3D_screen_point				(
									base_scene_ptr const&		arg_0,
									float3 const&				arg_1,
									math::color					arg_2,
									float						arg_3,
									bool						arg_4
								) { /* no source */ }

			void				play_particle_system				(
									base_scene_ptr const&					in_scene,
									resources::unmanaged_resource_ptr		in_instance,
									bool									use_transform,
									bool									always_looping,
									float4x4 const&							transform
								);
			void				stop_particle_system				( base_scene_ptr const& in_scene, resources::unmanaged_resource_ptr in_instance );

			void				remove_particle_system_instance		(
									resources::unmanaged_resource_ptr		particle_system_instance,
									base_scene_ptr const&					in_scene
								);
			void				update_particle_system_instance		(
									resources::unmanaged_resource_ptr		particle_system_instance,
									base_scene_ptr const&					in_scene,
									float4x4 const&							transform,
									bool									visible,
									bool									paused
								);

			bool				is_playing							( resources::unmanaged_resource_ptr const& instance );

			void				set_gamma_correction_factor			( const float value );

			void				resize_render_output_window			(
									base_output_window_ptr const&		output_window,
									const u32							width,
									const u32							height,
									const bool							fullscreen
								);

			void				goto_fullscreen						( base_output_window_ptr const& output_window );

			void				set_slomo							( base_scene_ptr const& scene, float time_multiplier );
			void				set_view_mode						( base_scene_view_ptr view_ptr, scene_view_mode view_mode );
			void				set_particles_render_mode			( base_scene_view_ptr view_ptr, particle::enum_particle_render_mode render_mode );

			void				enable_post_process					( base_scene_view_ptr view_ptr, bool enable );
			void				set_post_process					(
									base_scene_view_ptr						view_ptr,
									resources::unmanaged_resource_ptr		post_process_resource
								);

			void				toggle_render_stage					( enum_render_stage_type stage_type, bool toggle );

			particle::world&	particle_world						( base_scene_ptr const& scene );

			void				set_sky_material					( base_scene_ptr const& in_scene, resources::unmanaged_resource_ptr mtl_ptr );
			void				set_portal_system					( base_scene_ptr const& scene, resources::unmanaged_resource_ptr pss_ptr );
			void				test_action_portal_system			( base_scene_ptr const& scene );

			void				clear_grass							( base_scene_ptr const& s );
			void				populate_grass						( base_scene_ptr const& s );

			void				add_grass_layer						( grass_layer_desc* desc, grass_layer_data* data, base_scene_ptr const& s );
			void				update_grass_layer					(
									grass_layer_desc*			desc,
									grass_layer_data*			data,
									base_scene_ptr const&		s,
									bool						is_set
								);
			void				remove_grass_layer					( u8 id, base_scene_ptr const& s );

			void				set_grass							( resources::unmanaged_resource_ptr grass, base_scene_ptr const& s );
			void				reset_grass							( resources::unmanaged_resource_ptr grass, base_scene_ptr const& s );

	inline	pcstr				type								( ) { /* no source */ }

	static	void				draw_text							(
									vectora< ui::vertex >&		output,
									pcstr const&				text,
									ui::font const&				font,
									float2 const&				position,
									math::color const&			text_color,
									math::color const&			selection_color,
									u32							max_line_width,
									bool						is_multiline,
									u32							start_selection,
									u32							end_selection
								);

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	renderer*		m_renderer;
private:
	/* 0x0004 */	u32				m_frame_id;
	/* 0x0008 */	bool			m_initialized;
	/* 0x0009 */	bool			m_enable_terrain_debug_mode;
}; // class engine::world

STATIC_SIZE_ASSERT(engine::world, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENGINE_WORLD_H_INCLUDED
