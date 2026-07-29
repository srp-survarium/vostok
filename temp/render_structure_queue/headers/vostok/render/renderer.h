////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDERER_H_INCLUDED
#define RENDER_RENDERER_H_INCLUDED

/* INCLUDES */
class vostok::fixed_vector<vostok::render::stage *,29>;
class vostok::render::cloud_simulation;
class vostok::render::renderer_context;
class vostok::render::shader_constant_host;
class vostok::render::stage_debug;
class vostok::render::stage_screen_image;
class vostok::render::stage_view_mode;
class vostok::render::stage_visibility;
class vostok::timing::timer;
struct vostok::render::cloud_interp_textures;
struct vostok::render::event_query;
struct vostok::render::hw_hiz_occlusion_manager;
class vostok::render::base_output_window;
class vostok::render::base_scene;
class vostok::render::base_scene_view;
class vostok::render::enum_render_stage_type;
typedef vostok::intrusive_list<vostok::render::frame_histogram_info,vostok::render::frame_histogram_info *,12,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy>
	vostok::render::frame_histogram_info_list;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::frame_histogram_info;
class vostok::render::render_target;
class vostok::render::res_effect;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::math::rectangle<vostok::math::float2>;
class vostok::render::environment_probe_generate_parameters;
class vostok::render::renderer_context_targets;
class vostok::render::scene;
class vostok::render::scene_view;
class vostok::render::sky_ambient_occlusion_map_generate_parameters;
class vostok::render::vector<vostok::render::render_surface_instance *>;
class vostok::ui::font;
class vostok::ui::world;

namespace vostok {
namespace render {

class renderer : public boost::noncopyable {
public:
			explicit			renderer							( renderer_context* renderer_context );
								~renderer							( );

			void				render								(
									base_scene_ptr const&				in_scene,
									base_scene_view_ptr const&			in_view,
									base_output_window_ptr const&		output_window,
									math::rectangle< float2 > const&	viewport,
									boost::function< void( bool ) > const&	on_draw_scene,
									bool								draw_debug_terrain,
									ui::font const*						default_font
								);

	inline	void				generate_sky_ao_map					(
									base_scene_ptr const&				arg_0,
									base_scene_view_ptr const&			arg_1,
									base_output_window_ptr const&		arg_2,
									sky_ambient_occlusion_map_generate_parameters const&	arg_3
								) { /* no source */ }
	inline	void				generate_environment_probe			(
									base_scene_ptr const&				arg_0,
									base_scene_view_ptr const&			arg_1,
									base_output_window_ptr const&		arg_2,
									environment_probe_generate_parameters const&	arg_3
								) { /* no source */ }
	inline	void				generate_environment_probe_face		(
									base_scene_ptr const&				arg_0,
									base_scene_view_ptr const&			arg_1,
									base_output_window_ptr const&		arg_2,
									const u32							arg_3,
									float3 const&						arg_4,
									const u32							arg_5,
									float4&								arg_6,
									const float							arg_7
								) { /* no source */ }

			void				recreate_stage						( enum_render_stage_type arg_0 );

	inline	void				pick_lighting_luminance				( const u32 arg_0, const u32 arg_1 ) { /* no source */ }

	inline	void				set_picking_lighting_luminance_mode	( bool arg_0 ) { /* no source */ }

			void				present								(
									base_output_window_ptr				in_output_window,
									math::rectangle< float2 > const&	viewport
								);

	inline	u32					frame_id							( ) const { /* no source */ }

			void				clear_resources						( );

			void				fill_opaque_models					( );
			void				sort_models							(
									vector< render_surface_instance* >&		instances,
									enum_render_stage_type					stage_type,
									const u32								tech_index
								);
			void				sort_models_by_distance				( vector< render_surface_instance* >& instances, bool from_near_to_far );

			void				setup_render_output_window			(
									base_output_window_ptr				in_output_window,
									math::rectangle< float2 > const&	viewport
								);

			void				toggle_render_stage					( enum_render_stage_type stage_type, bool toggle );

	inline	void				set_view_to_rain_shadow				( float4x4 const& arg_0 ) { /* no source */ }
	inline	float4x4 const&		get_view_to_rain_shadow				( ) const { /* no source */ }

	inline	void				draw_top_dip_models_list			(
									ui::world&		arg_0,
									const u32		arg_1,
									const u32		arg_2,
									const u32		arg_3
								) const { /* no source */ }

	inline	void				gather_statistics					( ) { /* no source */ }

	inline	res_effect_ptr		get_grass_trample_effect			( ) { /* no source */ }

			void				set_target_context					( renderer_context_targets const* targets_context, bool force_set );

			void				draw_debug							(
									scene*				scene,
									scene_view*			view,
									const float			frame_time,
									ui::font const*		default_font
								);

private:
	inline	void				test_perlin							( ) { /* no source */ }

			void				draw_luminance_picker_info			( ui::font const* default_font );
			void				draw_frame_histogram				( ) const;
			void				draw_stages_stats					( ui::font const* default_font );

			void				execute_stages						( );

			bool				is_effects_ready					( ) const;

	/* 0x0000 */	/* boost::noncopyable */
public:
	/* 0x0000 */	bool							m_picking_lighting_luminance_mode;
private:
	/* 0x0004 */	float4x4						m_view_to_rain_shadow;
	/* 0x0048 */	timing::timer					m_timing_timer;
	/* 0x0060 */	event_query*					m_timing_event;
	/* 0x0064 */	event_query*					m_frame_sync_event;
	/* 0x0068 */	render_target_ptr				m_debug_1x1_gpu_data_rt[4];
	/* 0x0078 */	res_texture_ptr					m_debug_1x1_gpu_data[4];
	/* 0x0088 */	res_texture_ptr					m_debug_1x1_cpu_data[4];
	/* 0x0098 */	float4							m_debug_readed_data[4];
	/* 0x00d8 */	fixed_vector< stage*, 29 >		m_stages;
	/* 0x0154 */	cloud_interp_textures			m_cloud_interp_textures;
	/* 0x015c */	hw_hiz_occlusion_manager*		m_occlusion_manager;
	/* 0x0160 */	renderer_context*				m_renderer_context;
	/* 0x0164 */	stage_screen_image*				m_present_stage;
	/* 0x0168 */	stage_view_mode*				m_view_mode_stage;
	/* 0x016c */	stage_debug*					m_stage_debug;
	/* 0x0170 */	stage_visibility*				m_visibility_stage;
	/* 0x0178 */	timing::timer					m_timer;
	/* 0x0190 */	timing::timer					m_fps_timer;
	/* 0x01a8 */	shader_constant_host*			m_gbuffer_to_screen_type;
	/* 0x01ac */	shader_constant_host*			m_c_eye_ray_corner;
	/* 0x01b0 */	shader_constant_host*			m_c_probe_position;
	/* 0x01b4 */	shader_constant_host*			m_c_pick_lighting_luminance_position;
	/* 0x01b8 */	res_effect_ptr					m_gbuffer_to_screen_shader;
	/* 0x01bc */	res_effect_ptr					m_fill_environment_probe_face;
	/* 0x01c0 */	res_effect_ptr					m_editor_apply_wireframe_shader;
	/* 0x01c4 */	res_effect_ptr					m_fill_sky_ao_map_effect;
	/* 0x01c8 */	res_effect_ptr					m_pick_light_luminance_effect;
	/* 0x01cc */	res_effect_ptr					m_grass_trample_effect;
	/* 0x01d0 */	float							m_last_frame_time;
	/* 0x01d4 */	float							m_current_time;
	/* 0x01d8 */	u32								m_frame_id;
	/* 0x01dc */	cloud_simulation				m_simulation;
	/* 0x0240 */	frame_histogram_info_list		m_fps_history;
	/* 0x0270 */	float4							m_selected_lighting_luminanace_in_screen;
}; // class renderer

STATIC_SIZE_ASSERT(renderer, 0x280);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDERER_H_INCLUDED
