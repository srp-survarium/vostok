////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
#define RENDER_STAGE_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
class vostok::render::vector<vostok::render::lpv_render_surface>;
struct vostok::render::box_geometry;
struct vostok::render::radiance_volume;
class vostok::render::vector<vostok::math::float4x4>;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::render_target;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::geometry_batch;
class vostok::render::light;
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_light_propagation_volumes : public stage {
public:
					stage_light_propagation_volumes	( renderer* in_renderer, renderer_context* context );
	virtual			~stage_light_propagation_volumes( );

	virtual	void	execute							( ) override;

			void	execute_impl					( );
			void	execute_smoothed_impl			(
						const u32		current_cascade_index,
						const u32		stage_index,
						const u32		propagation_iteration_index,
						const u32		render_stage_index,
						const u32		num_render_stages
					);

			bool	is_effects_ready				( ) const;

			void	draw_debug						( );

private:
	virtual	void	execute_disabled				( ) override;

			void	pre_lpv_batch_render			(
						float3 const&				light_color,
						const float					light_intensity,
						geometry_batch const&		batch
					);
			void	post_lpv_batch_render			( geometry_batch const& batch );

			void	render_to_rms					(
						float3 const&			light_color,
						const float				light_intensity,
						float4x4 const&			view_matrix,
						float4x4 const&			projection_matrix,
						vector< float4x4 >		transforms,
						const u32				cascade_index
					);
	inline	void	render_to_rms_smoothed			(
						float3 const&			arg_0,
						const float				arg_1,
						float4x4 const&			arg_2,
						float4x4 const&			arg_3,
						vector< float4x4 >		arg_4,
						const u32				arg_5,
						const u32				arg_6,
						const u32				arg_7
					) { /* no source */ }
			void	render_to_rms_smoothed2			(
						float3 const&			light_color,
						const float				light_intensity,
						float4x4 const&			view_matrix,
						float4x4 const&			projection_matrix,
						vector< float4x4 >		transforms,
						const u32				cascade_index,
						const u32				render_stage_index,
						const u32				num_render_stages
					);
			void	render_to_point_rms				( light* l, const u32 face_index, vector< float4x4 > transforms );
			void	render_to_sky_rms				(
						light*					sun,
						const u32				face_index,
						const u32				cascade_index,
						vector< float4x4 >		transforms
					);
			void	render_to_spot_rms				( light* l, vector< float4x4 > transforms );
			void	render_to_sun_rms				( light* sun, const u32 cascade_index, vector< float4x4 > transforms );
			void	render_to_sun_rms_smoothed		(
						light*					sun,
						const u32				cascade_index,
						vector< float4x4 >		transforms,
						const u32				stage_render_index,
						const u32				num_render_stages
					);

			void	downsample_rsm					(
						float3 const&		light_direction,
						float3 const&		grid_origin,
						float				grid_scale,
						const u32			cascade_index
					);
	inline	void	downsample_gbuffer				( ) { /* no source */ }

			void	inject_lighting					(
						const u32			cascade_index,
						float3 const&		light_position,
						float3 const&		light_direction,
						float				light_fov
					);
			void	inject_occluders				(
						const u32				cascade_index,
						float3 const&			light_position,
						float3 const&			light_direction,
						vector< float4x4 >		transforms
					);

			void	propagate_lighting				( const u32 cascade_index );
			void	propagate_lighting_smoothed		( const u32 cascade_index, const u32 propagation_iteration_index );

			void	register_light_constans			( );
			void	register_rsm_constans			( );

			void	set_rsm_contants				( float3 const& light_direction, float3 const& grid_origin, float grid_scale );
	inline	void	set_light_constans				( light* arg_0 ) { /* no source */ }

			void	render_quad						( );

	/* 0x0000 */	/* stage */
	/* 0x0010 */	float3							start_render_eye_position;
	/* 0x001c */	box_geometry					m_box_geometry;
	/* 0x002c */	radiance_volume*				m_radiance_volume;
	/* 0x0030 */	u32								m_num_cascades;
	/* 0x0034 */	u32								m_rsm_source_size;
	/* 0x0038 */	u32								m_rsm_downsampled_size;
	/* 0x003c */	u32								m_grid_size;
	/* 0x0040 */	bool							m_has_indirect_lighting;
	/* 0x0044 */	render_target_ptr				m_rt_downsampled_scene;
	/* 0x0048 */	res_texture_ptr					m_t_downsampled_scene;
	/* 0x004c */	float4x4						m_previous_view_matrix[4];
	/* 0x014c */	float4x4						m_previous_proj_matrix[4];
	/* 0x024c */	vector< lpv_render_surface >	m_caster_models[4];
	/* 0x027c */	render_target_ptr				m_rms_depth_stencil_source[4];
	/* 0x028c */	res_effect_ptr					m_fill_rsm_effect[15];
	/* 0x02c8 */	res_effect_ptr					m_downsample_rsm_effect;
	/* 0x02cc */	res_effect_ptr					m_apply_indirect_lighting_effect;
	/* 0x02d0 */	res_effect_ptr					m_downsample_gbuffer_effect;
	/* 0x02d4 */	untyped_buffer_ptr				m_screen_vertex_ib;
	/* 0x02d8 */	res_geometry_ptr				m_screen_vertex_geometry;
	/* 0x02dc */	shader_constant_host*			m_c_radiance_blend_factor;
	/* 0x02e0 */	shader_constant_host*			m_c_light_type;
	/* 0x02e4 */	shader_constant_host*			m_c_light_color;
	/* 0x02e8 */	shader_constant_host*			m_c_light_intensity;
	/* 0x02ec */	shader_constant_host*			m_c_light_position;
	/* 0x02f0 */	shader_constant_host*			m_c_light_direction;
	/* 0x02f4 */	shader_constant_host*			m_c_light_attenuation_power;
	/* 0x02f8 */	shader_constant_host*			m_c_light_range;
	/* 0x02fc */	shader_constant_host*			m_c_view_to_light_matrix;
	/* 0x0300 */	shader_constant_host*			m_c_diffuse_influence_factor;
	/* 0x0304 */	shader_constant_host*			m_c_specular_influence_factor;
	/* 0x0308 */	shader_constant_host*			m_c_light_spot_penumbra_half_angle_cosine;
	/* 0x030c */	shader_constant_host*			m_c_light_spot_umbra_half_angle_cosine;
	/* 0x0310 */	shader_constant_host*			m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine;
	/* 0x0314 */	shader_constant_host*			m_c_light_spot_falloff;
	/* 0x0318 */	shader_constant_host*			m_c_light_capsule_half_width;
	/* 0x031c */	shader_constant_host*			m_c_light_capsule_radius;
	/* 0x0320 */	shader_constant_host*			m_c_light_sphere_radius;
	/* 0x0324 */	shader_constant_host*			m_c_lighting_model;
	/* 0x0328 */	shader_constant_host*			m_c_light_local_to_world;
	/* 0x032c */	shader_constant_host*			m_c_near_far;
	/* 0x0330 */	shader_constant_host*			m_c_grid_origin;
	/* 0x0334 */	shader_constant_host*			m_c_grid_cell_size;
	/* 0x0338 */	shader_constant_host*			m_c_invert_rsm_size;
	/* 0x033c */	shader_constant_host*			m_c_interreflection_contribution;
	/* 0x0340 */	shader_constant_host*			m_c_cascade_index;
	/* 0x0344 */	shader_constant_host*			m_c_num_cascades;
	/* 0x0348 */	shader_constant_host*			m_c_ambient_color;
	/* 0x034c */	shader_constant_host*			m_c_smaller_cascade_grid_cell_size;
	/* 0x0350 */	shader_constant_host*			m_c_smaller_cascade_grid_size;
	/* 0x0354 */	shader_constant_host*			m_c_smaller_cascade_grid_origin;
	/* 0x0358 */	shader_constant_host*			m_c_grid_size;
	/* 0x035c */	shader_constant_host*			m_c_eye_ray_corner;
}; // class stage_light_propagation_volumes

STATIC_SIZE_ASSERT(stage_light_propagation_volumes, 0x360);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_LIGHT_PROPAGATION_VOLUMES_H_INCLUDED
