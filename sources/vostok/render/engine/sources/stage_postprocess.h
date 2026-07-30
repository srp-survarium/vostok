#ifndef VOSTOK_RENDER_ENGINE_STAGE_POSTPROCESS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_POSTPROCESS_H_INCLUDED

#include <functional>

#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/untyped_buffer.h>

#include "material.h"
#include "material_effects.h"
#include "render_target.h"
#include "res_geometry.h"
#include "res_texture.h"
#include "stage.h"

namespace vostok {
namespace render {

class post_process_parameters;
class renderer;
class renderer_context;
class render_surface_instance;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

struct bloom_shader_constants {
	bloom_shader_constants( );

	void set(
		float bloom_scale,
		float bloom_max_color,
		float3 const& halo_color
	);

private:
	shader_constant_host*	m_bloom_parameters;
	shader_constant_host*	m_bloom_parameters1;
};

STATIC_SIZE_ASSERT( bloom_shader_constants, 0x8 );

struct dof_shader_constants {
	dof_shader_constants( );

	void set(
		float3 const& blurriness_height_lights,
		float distance,
		float region,
		float power,
		float near_blur_amount,
		float far_blur_amount,
		float bokeh_dof_radius,
		float bokeh_dof_density
	);

private:
	shader_constant_host*	m_dof_parameters;
	shader_constant_host*	m_dof_height_lights;
	shader_constant_host*	m_blurriness_amount;
	shader_constant_host*	m_bokeh_dof_parameters;
};

STATIC_SIZE_ASSERT( dof_shader_constants, 0x10 );

struct scene_shader_constants {
	scene_shader_constants( );

	void set(
		renderer_context* context,
		float3 const& height_lights,
		float3 const& mid_tones,
		float3 const& shadows,
		float3 const& fade_color,
		float fade_amount,
		float gamma_correction_factor,
		float desaturation,
		float4 const& image_grain_parameters,
		post_process_parameters const& parameters
	);

private:
	shader_constant_host*	m_frame_height_lights_and_desaturation_parameters;
	shader_constant_host*	m_scene_mid_tones_parameters;
	shader_constant_host*	m_scene_shadows_parameters;
	shader_constant_host*	m_gamma_correction_factor;
	shader_constant_host*	m_scene_fade_parameters;
	shader_constant_host*	m_filmic_tonemap_packed_parameters_0;
	shader_constant_host*	m_filmic_tonemap_packed_parameters_1;
	shader_constant_host*	m_c_eye_ray_corner;
	shader_constant_host*	m_image_grain_parameters;
};

STATIC_SIZE_ASSERT( scene_shader_constants, 0x24 );

struct remove_model_skeletal_filter_predicate {
	explicit remove_model_skeletal_filter_predicate( bool inverse_flag ) :
		m_inverse_flag( inverse_flag )
	{
	}

	bool operator()( render_surface_instance* model );

	bool m_inverse_flag;
};

STATIC_SIZE_ASSERT( remove_model_skeletal_filter_predicate, 0x1 );

class stage_postprocess : public stage {
public:
	stage_postprocess( renderer* in_renderer, renderer_context* context );
	virtual ~stage_postprocess( ) { }

	virtual void execute( ) override;
	virtual void execute_disabled( ) override;

private:
	bool is_effects_ready( ) const;

	void fill_surface( render_target_ptr surf0, render_target_ptr surf1 );
	void fill_surface2( render_target_ptr surf );
	void clear_surface( render_target_ptr surf );
	void advanced_bloom( );
	void process_blur(
		render_target* rt0,
		res_texture* texture0,
		render_target* rt1,
		res_texture* texture1,
		u32 kernel_index
	);

	void measure_per_pixel_luminance(
		res_texture* scene_texture,
		float4& out_average_min_max
	);

	void compute_per_pixel_eye_adaptated_luminance( );

	void measure_per_pixel_luminance_percentage(
		res_texture*,
		float,
		float
	)
	{
		// STATE[STUB]
	}

	float4 compute_luminance_parameters( u32 frame_delta );

	void buid_luminance_histogram(
		res_texture*,
		u32,
		float,
		float,
		float*
	)
	{
		// STATE[STUB]
	}

	void buid_luminance_histogram_start(
		res_texture*,
		u32,
		float,
		float,
		float*
	)
	{
		// STATE[STUB]
	}

	void buid_luminance_histogram_step(
		res_texture*,
		u32,
		float,
		float,
		u32
	)
	{
		// STATE[STUB]
	}

	void buid_luminance_histogram_end(
		res_texture*,
		u32,
		float,
		float,
		float*
	)
	{
		// STATE[STUB]
	}

	void accumulate_motion_vectors( );

	float4 get_frame_luminance_parameters( bool& valid )
	{
		// STATE[STUB]
		valid = false;
		return float4( 0.0f, 0.0f, 0.0f, 0.0f );
	}

private:
	float4x4					m_prev_view_matrix;
	res_effect_ptr				m_sh_gather_bloom;
	res_effect_ptr				m_sh_gather_luminance;
	res_effect_ptr				m_sh_gather_luminance_histogram;
	res_effect_ptr				m_sh_eye_adaptation;
	res_effect_ptr				m_sh_blur[8];
	res_effect_ptr				m_sh_complex_blend[2][2][2];
	res_effect_ptr				m_sh_effect_copy_image;
	res_effect_ptr				m_post_process_antialiasing_shader;
	res_effect_ptr				m_post_process_antialiasing_shader_fxaa;
	res_effect_ptr				m_post_process_antialiasing_shader_sraa;
	res_effect_ptr				m_post_process_shader_sharpen;
	res_effect_ptr				m_god_rays_effect;
	res_effect_ptr				m_post_process_downsample_frame_effect;
	res_effect_ptr				m_image_space_reflections_effect;
	res_effect_ptr				m_lens_flares_effect;
	res_effect_ptr				m_motion_blur_effect;
	res_effect_ptr				m_olta_effect;
	res_effect_ptr				m_temporal_antialiasing_effect;
	res_effect_ptr				m_aberration_effect;
	res_effect_ptr				m_motion_vectors_accumulation_effect;
	untyped_buffer_ptr			m_screen_vertex_ib;
	res_geometry_ptr			m_screen_vertex_geometry;
	shader_constant_host*		m_kernel_offsets;
	shader_constant_host*		m_blur_offsets_weights;
	shader_constant_host*		m_luminance_range_parameter_parameter;
	shader_constant_host*		m_elapsed_time_parameter;
	shader_constant_host*		m_adaptation_factor;
	shader_constant_host*		m_gamma_correction_factor;
	shader_constant_host*		m_prev_view_matrix_parameter;
	shader_constant_host*		m_prev_world_view_matrix_parameter;
	shader_constant_host*		m_inverse_world_matrix_parameter;
	shader_constant_host*		m_god_rays_parameters0;
	shader_constant_host*		m_god_rays_parameters1;
	shader_constant_host*		m_god_rays_parameters2;
	shader_constant_host*		m_c_eye_ray_corner;
	shader_constant_host*		m_c_frame_index;
	shader_constant_host*		m_frame_delta_parameter;
	shader_constant_host*		m_motion_blur_scale_parameter;
	shader_constant_host*		m_aberration_parameters;
	shader_constant_host*		m_blur_target_size_parameter;
	shader_constant_host*		m_lens_flares_parameters;
	shader_constant_host*		m_sun_direction_parameter;
	shader_constant_host*		m_frame_luminance_parameter;
	shader_constant_host*		m_fxaa_parameters;
	float4						kernel_offsets[8];
	res_texture_list			m_textures;
	res_texture_ptr				m_color_grading_base_lut;
	bloom_shader_constants		m_bloom_shader_constants;
	dof_shader_constants			m_dof_shader_constants;
	scene_shader_constants		m_scene_shader_constants;
	vector<material_effects>		m_material_post_effects;
	material_ptr					m_test_material;
	float2						m_image_grain_random_offsets;
	map<
		render_surface_instance*,
		float4x4,
		std::less<render_surface_instance*>
	>							m_prev_matrix_map;
};

STATIC_SIZE_ASSERT( stage_postprocess, 0x23C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_POSTPROCESS_H_INCLUDED
