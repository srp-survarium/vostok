////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_LIGHTS_H_INCLUDED
#define RENDER_STAGE_LIGHTS_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<64>;
class vostok::render::shader_constant_host;
class vostok::render::stage;
struct vostok::render::stage_lights::instance_data;
struct vostok::render::stage_lights::light_geometry;
struct vostok::render::stage_lights::lights_instance;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::render_target;
class vostok::render::res_declaration;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::environment_probe;
class vostok::render::light;
class vostok::render::lod_entry;
class vostok::render::render_particle_emitter_instance;
class vostok::render::render_surface_instance;
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::speedtree_tree_component;

namespace vostok {
namespace render {

class stage_lights : public stage {
public:
					stage_lights					( renderer* in_renderer, renderer_context* context, bool is_forward_lighting_pass );
	virtual			~stage_lights					( );

	virtual	void	execute							( ) override;

	virtual	void	execute_disabled				( ) override;

	virtual	void	debug_render					( ) override;

private:
			bool	is_effects_ready				( ) const;

			void	render_model_lighting			( render_surface_instance* instance, light* l );
			void	render_model_probe_lighting		(
						render_surface_instance*	instance,
						environment_probe*			probe,
						float						min_probe_scale
					);
			void	render_speedtree_lighting		(
						lod_entry const*					lod,
						SpeedTree::CInstance const*			instance,
						SpeedTree::SInstanceLod const*		instance_lod,
						speedtree_tree_component*			tree_component,
						light*								l
					);
			void	render_particle_lighting		( render_particle_emitter_instance* instance, light* l, u32 num_particles );
			void	render_particle_probe_lighting	(
						render_particle_emitter_instance*		instance,
						environment_probe*						probe,
						u32										num_particles
					);
			void	render_light					( light* l, bool shadowers_pass );
			void	render_shadowed_light			( light* l );

			void	draw_geometry					( light* l );

			u32		index_to_shadow_size			( u32 size_index ) const;

			void	new_sphere_geometry				( );
			void	create_pyramid_geometry			( );
			void	create_obb_geometry				( );

			void	fill_surface					( render_target_ptr surf );

	inline	void	render_to_cubemap				( u32 arg_0, light* arg_1 ) { /* no source */ }
	inline	void	render_to_cubemap_face			( u32 arg_0, float3 const& arg_1, float arg_2 ) { /* no source */ }

			void	make_spot_light_shadowmap		( u32 shadow_quality, light* l );
			void	make_plane_spot_light_shadowmap	( u32 shadow_quality, light* l );
			void	render_to_hw_shadowmap			(
						light*				l,
						u32					shadow_quality,
						float				z_bias,
						u32					smap_size,
						u32					smap_size_index,
						float4x4 const&		view_matrix,
						float4x4 const&		projection_matrix,
						u32					marge
					);

			void	make_skin_scattering_texture	( render_surface_instance* instance, light* l );

	inline	void	render_instanced_lights			( ) { /* no source */ }
	inline	void	flush_instanced_lights			( const u32 arg_0 ) { /* no source */ }

	/* 0x0000 */	/* stage */
	/* 0x0010 */	bool								m_enable_env_probes;
	/* 0x0014 */	render_target_ptr					m_rt_skin_scattering_position;
	/* 0x0018 */	res_texture_ptr						m_t_skin_scattering_position;
	/* 0x001c */	render_target_ptr					m_rt_skin_scattering_temp;
	/* 0x0020 */	res_texture_ptr						m_t_skin_scattering_temp;
	/* 0x0024 */	render_target_ptr					m_rt_skin_scattering;
	/* 0x0028 */	res_texture_ptr						m_t_skin_scattering;
	/* 0x002c */	render_target_ptr					m_rt_skin_scattering_stretch;
	/* 0x0030 */	res_texture_ptr						m_t_skin_scattering_stretch;
	/* 0x0034 */	render_target_ptr					m_rt_skin_scattering_small;
	/* 0x0038 */	res_texture_ptr						m_t_skin_scattering_small;
	/* 0x003c */	render_target_ptr					m_rt_skin_scattering_blurred_0;
	/* 0x0040 */	res_texture_ptr						m_t_skin_scattering_blurred_0;
	/* 0x0044 */	render_target_ptr					m_rt_skin_scattering_blurred_1;
	/* 0x0048 */	res_texture_ptr						m_t_skin_scattering_blurred_1;
	/* 0x004c */	render_target_ptr					m_rt_skin_scattering_blurred_2;
	/* 0x0050 */	res_texture_ptr						m_t_skin_scattering_blurred_2;
	/* 0x0054 */	render_target_ptr					m_rt_skin_scattering_blurred_3;
	/* 0x0058 */	res_texture_ptr						m_t_skin_scattering_blurred_3;
	/* 0x005c */	render_target_ptr					m_rt_skin_scattering_blurred_4;
	/* 0x0060 */	res_texture_ptr						m_t_skin_scattering_blurred_4;
	/* 0x0064 */	render_target_ptr					m_skin_scattering_render_target;
	/* 0x0068 */	render_target_ptr					m_skin_scattering_depth_stencil;
	/* 0x006c */	res_texture_ptr						m_skin_scattering_texture;
	/* 0x0070 */	render_target_ptr					m_shadow_depth_stencil[3];
	/* 0x007c */	res_texture_ptr						m_shadow_depth_stencil_texture[3];
	/* 0x0088 */	render_target_ptr					m_vcm_render_target;
	/* 0x008c */	render_target_ptr					m_vcm_depth_stencil;
	/* 0x0090 */	render_target_ptr					m_lookup_vcm_render_target[6];
	/* 0x00a8 */	render_target_ptr					m_lookup_vcm_depth_stencil[6];
	/* 0x00c0 */	res_texture_ptr						m_lookup_vcm_texture;
	/* 0x00c4 */	res_texture_ptr						m_lookup_vcm_depth_stencil_texture;
	/* 0x00c8 */	fixed_string< 64 >					m_lookup_vcm_render_target_names[6];
	/* 0x0290 */	fixed_string< 64 >					m_lookup_vcm_depth_stencil_names[6];
	/* 0x0458 */	untyped_buffer_ptr					m_lookup_vcm_ib;
	/* 0x045c */	res_geometry_ptr					m_lookup_vcm_geometry;
	/* 0x0460 */	render_target_ptr					m_cubemap_face_render_target[6];
	/* 0x0478 */	render_target_ptr					m_cubemap_face_depth_stencil[6];
	/* 0x0490 */	res_texture_ptr						m_cubemap_texture;
	/* 0x0494 */	res_texture_ptr						m_depth_stencil_cubemap_texture;
	/* 0x0498 */	fixed_string< 64 >					m_cubemap_face_render_target_names[6];
	/* 0x0660 */	fixed_string< 64 >					m_cubemap_face_depth_stencil_names[6];
	/* 0x0828 */	res_effect_ptr						m_effect_accum_mask;
	/* 0x082c */	res_effect_ptr						m_point_light_shadower;
	/* 0x0830 */	res_effect_ptr						m_point_light_accumulator;
	/* 0x0834 */	res_effect_ptr						m_shadowed_point_light_accumulator;
	/* 0x0838 */	res_effect_ptr						m_spot_light_accumulator;
	/* 0x083c */	res_effect_ptr						m_shadowed_spot_light_accumulator;
	/* 0x0840 */	res_effect_ptr						m_capsule_light_accumulator;
	/* 0x0844 */	res_effect_ptr						m_obb_light_accumulator;
	/* 0x0848 */	res_effect_ptr						m_shadowed_obb_light_accumulator;
	/* 0x084c */	res_effect_ptr						m_sphere_light_accumulator;
	/* 0x0850 */	res_effect_ptr						m_shadowed_sphere_light_accumulator;
	/* 0x0854 */	res_effect_ptr						m_plane_spot_light_accumulator;
	/* 0x0858 */	res_effect_ptr						m_shadowed_plane_spot_light_accumulator;
	/* 0x085c */	res_effect_ptr						m_sh_downsample_skin_irradiance_texture;
	/* 0x0860 */	res_effect_ptr						m_sh_fix_irradiance_texture;
	/* 0x0864 */	res_effect_ptr						m_shadow_effect;
	/* 0x0868 */	stage_lights::light_geometry		m_sphere_geometry;
	/* 0x0874 */	stage_lights::light_geometry		m_pyramid_geometry;
	/* 0x0880 */	stage_lights::light_geometry		m_obb_geometry;
	/* 0x088c */	untyped_buffer_ptr					m_screen_vertex_ib;
	/* 0x0890 */	res_geometry_ptr					m_screen_vertex_geometry;
	/* 0x0894 */	untyped_buffer_ptr					m_instance_vb_small;
	/* 0x0898 */	res_declaration_ptr					m_instance_declaration;
	/* 0x089c */	res_declaration_ptr					m_instance_declaration_small;
	/* 0x08a0 */	u32									m_num_instanced_lights;
	/* 0x08a4 */	stage_lights::instance_data*		m_instance_data_array;
	/* 0x08a8 */	stage_lights::lights_instance		m_lights_instance[4];
	/* 0x08b8 */	shader_constant_host*				m_probe_parameters0;
	/* 0x08bc */	shader_constant_host*				m_probe_parameters1;
	/* 0x08c0 */	shader_constant_host*				m_gamma_correction_factor;
	/* 0x08c4 */	shader_constant_host*				m_shadow[4];
	/* 0x08d4 */	shader_constant_host*				m_kernel_offsets;
	/* 0x08d8 */	shader_constant_host*				m_blur_offsets_weights;
	/* 0x08dc */	shader_constant_host*				m_c_light_type;
	/* 0x08e0 */	shader_constant_host*				m_c_light_color;
	/* 0x08e4 */	shader_constant_host*				m_c_light_intensity;
	/* 0x08e8 */	shader_constant_host*				m_c_light_position;
	/* 0x08ec */	shader_constant_host*				m_c_light_direction;
	/* 0x08f0 */	shader_constant_host*				m_c_light_attenuation_power;
	/* 0x08f4 */	shader_constant_host*				m_c_light_range;
	/* 0x08f8 */	shader_constant_host*				m_far_fog_color_and_distance;
	/* 0x08fc */	shader_constant_host*				m_near_fog_distance;
	/* 0x0900 */	shader_constant_host*				m_c_diffuse_influence_factor;
	/* 0x0904 */	shader_constant_host*				m_c_specular_influence_factor;
	/* 0x0908 */	shader_constant_host*				m_c_is_shadower;
	/* 0x090c */	shader_constant_host*				m_c_light_instances;
	/* 0x0910 */	float4x4*							m_light_instances;
	/* 0x0914 */	shader_constant_host*				m_c_light_spot_penumbra_half_angle_cosine;
	/* 0x0918 */	shader_constant_host*				m_c_light_spot_umbra_half_angle_cosine;
	/* 0x091c */	shader_constant_host*				m_c_light_spot_inversed_umbra_half_angle_cosine_minus_penumbra_half_angle_cosine;
	/* 0x0920 */	shader_constant_host*				m_c_light_spot_falloff;
	/* 0x0924 */	shader_constant_host*				m_c_light_capsule_half_width;
	/* 0x0928 */	shader_constant_host*				m_c_light_capsule_radius;
	/* 0x092c */	shader_constant_host*				m_c_light_sphere_radius;
	/* 0x0930 */	shader_constant_host*				m_c_light_local_to_world;
	/* 0x0934 */	shader_constant_host*				m_c_eye_ray_corner;
	/* 0x0938 */	shader_constant_host*				m_c_near_far;
	/* 0x093c */	shader_constant_host*				m_c_view_to_light_matrix;
	/* 0x0940 */	shader_constant_host*				m_c_shadow_z_bias;
	/* 0x0944 */	shader_constant_host*				m_c_shadow_map_size;
	/* 0x0948 */	shader_constant_host*				m_c_use_shadows;
	/* 0x094c */	shader_constant_host*				m_c_shadow_transparency;
	/* 0x0950 */	shader_constant_host*				m_c_is_unwrap_pass;
	/* 0x0954 */	shader_constant_host*				m_c_lighting_model;
	/* 0x0958 */	shader_constant_host*				m_ambient_color;
	/* 0x095c */	float4x4							m_view_to_light_matrix;
	/* 0x099c */	float4x4							m_light_to_texture_matrix;
	/* 0x09dc */	float								m_shadow_z_bias;
	/* 0x09e0 */	float								m_shadow_map_size;
	/* 0x09e4 */	const bool							m_is_forward_lighting_pass;
}; // class stage_lights

STATIC_SIZE_ASSERT(stage_lights, 0x9E8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_LIGHTS_H_INCLUDED
