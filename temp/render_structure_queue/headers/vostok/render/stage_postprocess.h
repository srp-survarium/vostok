////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_POSTPROCESS_H_INCLUDED
#define RENDER_STAGE_POSTPROCESS_H_INCLUDED

/* INCLUDES */
class vostok::render::map<vostok::render::render_surface_instance *,vostok::math::float4x4,stlp_std::less<vostok::render::render_surface_instance *> >;
class vostok::render::res_texture_list;
class vostok::render::shader_constant_host;
class vostok::render::stage;
class vostok::render::vector<vostok::render::material_effects>;
struct vostok::render::bloom_shader_constants;
struct vostok::render::dof_shader_constants;
struct vostok::render::scene_shader_constants;
class vostok::render::render_target;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::material,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::material;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage_postprocess : public stage {
public:
						stage_postprocess				( renderer* in_renderer, renderer_context* context );

	virtual	void		execute							( ) override;

	virtual	void		execute_disabled				( ) override;

private:
			bool		is_effects_ready				( ) const;

			void		fill_surface					( render_target_ptr surf0, render_target_ptr surf1 );
			void		fill_surface2					( render_target_ptr surf );

			void		clear_surface					( render_target_ptr surf );

			void		advanced_bloom					( );

			void		process_blur					(
							render_target*		rt0,
							res_texture*		t0,
							render_target*		rt1,
							res_texture*		t1,
							u32					kernel_index
						);

			void		measure_per_pixel_luminance		( res_texture* scene_texture, float4& out_avrg_min_max );
			void		compute_per_pixel_eye_adaptated_luminance( );

	inline	void		measure_per_pixel_luminance_percentage( res_texture* arg_0, float arg_1, float arg_2 ) { /* no source */ }

			float4		compute_luminance_parameters	( u32 frame_delta );

	inline	void		buid_luminance_histogram		(
							res_texture*	arg_0,
							u32				arg_1,
							float			arg_2,
							float			arg_3,
							float*			arg_4
						) { /* no source */ }
	inline	void		buid_luminance_histogram_start	(
							res_texture*	arg_0,
							u32				arg_1,
							float			arg_2,
							float			arg_3,
							float*			arg_4
						) { /* no source */ }
	inline	void		buid_luminance_histogram_step	(
							res_texture*	arg_0,
							const u32		arg_1,
							float			arg_2,
							float			arg_3,
							const u32		arg_4
						) { /* no source */ }
	inline	void		buid_luminance_histogram_end	(
							res_texture*	arg_0,
							u32				arg_1,
							float			arg_2,
							float			arg_3,
							float*			arg_4
						) { /* no source */ }

			void		accumulate_motion_vectors		( );

	inline	float4		get_frame_luminance_parameters	( bool& arg_0 ) { /* no source */ }

public:
	virtual				~stage_postprocess				( ) { /* no source */ }

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	float4x4						m_prev_view_matrix;
	/* 0x0050 */	res_effect_ptr					m_sh_gather_bloom;
	/* 0x0054 */	res_effect_ptr					m_sh_gather_luminance;
	/* 0x0058 */	res_effect_ptr					m_sh_gather_luminance_histogram;
	/* 0x005c */	res_effect_ptr					m_sh_eye_adaptation;
	/* 0x0060 */	res_effect_ptr					m_sh_blur[8];
	/* 0x0080 */	res_effect_ptr					m_sh_complex_blend[2][2][2];
	/* 0x00a0 */	res_effect_ptr					m_sh_effect_copy_image;
	/* 0x00a4 */	res_effect_ptr					m_post_process_antialiasing_shader;
	/* 0x00a8 */	res_effect_ptr					m_post_process_antialiasing_shader_fxaa;
	/* 0x00ac */	res_effect_ptr					m_post_process_antialiasing_shader_sraa;
	/* 0x00b0 */	res_effect_ptr					m_post_process_shader_sharpen;
	/* 0x00b4 */	res_effect_ptr					m_god_rays_effect;
	/* 0x00b8 */	res_effect_ptr					m_post_process_downsample_frame_effect;
	/* 0x00bc */	res_effect_ptr					m_image_space_reflections_effect;
	/* 0x00c0 */	res_effect_ptr					m_lens_flares_effect;
	/* 0x00c4 */	res_effect_ptr					m_motion_blur_effect;
	/* 0x00c8 */	res_effect_ptr					m_olta_effect;
	/* 0x00cc */	res_effect_ptr					m_temporal_antialiasing_effect;
	/* 0x00d0 */	res_effect_ptr					m_aberration_effect;
	/* 0x00d4 */	res_effect_ptr					m_motion_vectors_accumulation_effect;
	/* 0x00d8 */	untyped_buffer_ptr				m_screen_vertex_ib;
	/* 0x00dc */	res_geometry_ptr				m_screen_vertex_geometry;
	/* 0x00e0 */	shader_constant_host*			m_kernel_offsets;
	/* 0x00e4 */	shader_constant_host*			m_blur_offsets_weights;
	/* 0x00e8 */	shader_constant_host*			m_luminance_range_parameter_parameter;
	/* 0x00ec */	shader_constant_host*			m_elapsed_time_parameter;
	/* 0x00f0 */	shader_constant_host*			m_adaptation_factor;
	/* 0x00f4 */	shader_constant_host*			m_gamma_correction_factor;
	/* 0x00f8 */	shader_constant_host*			m_prev_view_matrix_parameter;
	/* 0x00fc */	shader_constant_host*			m_prev_world_view_matrix_parameter;
	/* 0x0100 */	shader_constant_host*			m_inverse_world_matrix_parameter;
	/* 0x0104 */	shader_constant_host*			m_god_rays_parameters0;
	/* 0x0108 */	shader_constant_host*			m_god_rays_parameters1;
	/* 0x010c */	shader_constant_host*			m_god_rays_parameters2;
	/* 0x0110 */	shader_constant_host*			m_c_eye_ray_corner;
	/* 0x0114 */	shader_constant_host*			m_c_frame_index;
	/* 0x0118 */	shader_constant_host*			m_frame_delta_parameter;
	/* 0x011c */	shader_constant_host*			m_motion_blur_scale_parameter;
	/* 0x0120 */	shader_constant_host*			m_aberration_parameters;
	/* 0x0124 */	shader_constant_host*			m_blur_target_size_parameter;
	/* 0x0128 */	shader_constant_host*			m_lens_flares_parameters;
	/* 0x012c */	shader_constant_host*			m_sun_direction_parameter;
	/* 0x0130 */	shader_constant_host*			m_frame_luminance_parameter;
	/* 0x0134 */	shader_constant_host*			m_fxaa_parameters;
	/* 0x0138 */	float4							kernel_offsets[8];
	/* 0x01b8 */	res_texture_list				m_textures;
	/* 0x01cc */	res_texture_ptr					m_color_grading_base_lut;
	/* 0x01d0 */	bloom_shader_constants			m_bloom_shader_constants;
	/* 0x01d8 */	dof_shader_constants			m_dof_shader_constants;
	/* 0x01e8 */	scene_shader_constants			m_scene_shader_constants;
	/* 0x020c */	vector< material_effects >		m_material_post_effects;
	/* 0x0218 */	material_ptr					m_test_material;
	/* 0x021c */	float2							m_image_grain_random_offsets;
	/* 0x0224 */	map< render_surface_instance*, float4x4, std::less< render_surface_instance* > >	m_prev_matrix_map;
}; // class stage_postprocess

STATIC_SIZE_ASSERT(stage_postprocess, 0x23C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_POSTPROCESS_H_INCLUDED
