#ifndef VOSTOK_RENDER_ENGINE_STAGE_POSTPROCESS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_POSTPROCESS_H_INCLUDED

#include <functional>

#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/untyped_buffer.h>

#include "effect_gather_luminance_histogram.h"
#include "material.h"
#include "material_effects.h"
#include "render_target.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "res_geometry.h"
#include "res_texture.h"
#include "scene_view.h"
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

// claude@NOTE: legacy post_process_parameters.h luminance constants relocated here (the canonical
// header dropped them; legacy NUM_TONEMAP_TEXTURES is superseded by rt_num_frame_luminance_targets)
#define NUM_HISTOGRAM_VALUES			16
#define MAX_TONEMAP_TEXTURE_DIMENSION	256
#define NUM_TONEMAP_PIXELS				(MAX_TONEMAP_TEXTURE_DIMENSION * MAX_TONEMAP_TEXTURE_DIMENSION)
// TODO: 0.5f
#define MIN_FRAME_LUMINANCE				0.0f
#define MAX_FRAME_LUMINANCE				4.0f

template<class T> T linear_interpolation2(T a, T b, float alpha)
{
	return a * (1 - alpha) + b * alpha;
}

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
		res_texture*	scene_texture,
		float			min_luminanace,
		float			max_luminanace
	)
	{
	//	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();

		s32 const last_target_index = s32(rt_num_frame_luminance_targets) - 1;

		for (u32 lum_rt_index=0; lum_rt_index<rt_num_frame_luminance_targets; lum_rt_index++)
		{
			backend::ref().set_render_targets	(&*m_context->m_targets->m_family[rt_frame_luminance0 + lum_rt_index].target, 0, 0, 0);
			backend::ref().clear_render_targets	(0.0f, 0.0f, 0.0f, 0.0f);
		}

		m_sh_gather_luminance_histogram->apply(effect_gather_luminance_histogram::gather_luminance_in_range, 0);
		backend::ref().set_ps_constant(m_luminance_range_parameter_parameter, float4(min_luminanace, max_luminanace, 0.0f, 0.0f));
		backend::ref().set_ps_texture( "t_frame_color0", scene_texture);
		fill_surface(m_context->m_targets->m_family[rt_frame_luminance0 + last_target_index].target, render_target_ptr( ));

		for (s32 lum_rt_index=last_target_index-1; lum_rt_index>=0; lum_rt_index--)
		{
			m_sh_gather_luminance_histogram->apply(effect_gather_luminance_histogram::gather_luminance_count, 0);
			backend::ref().set_ps_texture( "t_frame_color1", &*m_context->m_targets->m_family[rt_frame_luminance0 + lum_rt_index+1].texture);
			fill_surface(m_context->m_targets->m_family[rt_frame_luminance0 + lum_rt_index].target, render_target_ptr( ));
		}

		resource_manager::ref().copy2D(
			&*m_context->m_targets->m_family[rt_frame_luminance_current].texture,
			0,
			0,
			&*m_context->m_targets->m_family[rt_frame_luminance0 + 0].texture,
			0,
			0,
			m_context->m_targets->m_family[rt_frame_luminance0 + 0].texture->width(),
			m_context->m_targets->m_family[rt_frame_luminance0 + 0].texture->height(),
			0,
			0
		);
	}

	float4 compute_luminance_parameters( u32 frame_delta );

	void buid_luminance_histogram(
		res_texture*	scene_texture,
		u32				num_values,
		float			min_value,
		float			max_value,
		float*			out_array
	)
	{
		D3D11_VIEWPORT orig_viewport;
		backend::ref().get_viewport			(orig_viewport);

		backend::ref().set_render_targets	(&*m_context->m_targets->m_family[rt_frame_luminance_histogram].target, 0, 0, 0);
		backend::ref().clear_render_targets	(0.0f, 0.0f, 0.0f, 0.0f);

		u32 const num_histogam_values		= math::min<u32>(num_values, NUM_HISTOGRAM_VALUES);
		float2* ranges						= (float2*)ALLOCA(sizeof(float2) * num_histogam_values);
		float const step					= (max_value - min_value) / num_histogam_values;
		float c								= min_value;

		for (u32 i=0; i<num_histogam_values; i++)
		{
			ranges[i] = float2(c, c + step);
			c += step;
			if (i==num_histogam_values-1)
				ranges[i].y = 100000.0f;
		}

		for (u32 i=0; i<num_histogam_values; i++)
		{
			measure_per_pixel_luminance_percentage(scene_texture, ranges[i].x, ranges[i].y);

			D3D11_VIEWPORT tmp_viewport;
			tmp_viewport.TopLeftX	= float(i);
			tmp_viewport.TopLeftY	= 0.0f;
			tmp_viewport.Width		= 1;
			tmp_viewport.Height		= 1;
			tmp_viewport.MinDepth	= 0;
			tmp_viewport.MaxDepth	= 1.f;
			backend::ref().set_viewport( tmp_viewport);

			m_sh_gather_luminance_histogram->apply(effect_gather_luminance_histogram::gather_luminance_histogram, 0);
			fill_surface2(m_context->m_targets->m_family[rt_frame_luminance_histogram].target);

			backend::ref().set_viewport( orig_viewport);
		}

		if (out_array)
		{
			resource_manager::ref().copy2D(
				&*m_context->m_targets->m_family[rt_result_frame_luminance_histogram].texture,
				0,
				0,
				&*m_context->m_targets->m_family[rt_frame_luminance_histogram].texture,
				0,
				0,
				m_context->m_targets->m_family[rt_frame_luminance_histogram].texture->width(),
				m_context->m_targets->m_family[rt_frame_luminance_histogram].texture->height(),
				0,
				0
			);

			u32 row_pitch = 0;

			float4* data = (float4*)m_context->m_targets->m_family[rt_result_frame_luminance_histogram].texture->map2D(D3D11_MAP_READ, 0, row_pitch);

			for (u32 i=0; i<num_histogam_values; i++)
				out_array[i] = (*data++).x;

			m_context->m_targets->m_family[rt_result_frame_luminance_histogram].texture->unmap2D(0);
		}
	}

	void buid_luminance_histogram_start(
		res_texture*,
		u32,
		float,
		float,
		float*
	)
	{
		// claude@NOTE: no legacy ancestor - legacy has only the monolithic buid_luminance_histogram, no start/step/end split; matcher-phase work.
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
		// claude@NOTE: no legacy ancestor - legacy has only the monolithic buid_luminance_histogram; matcher-phase work.
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
		// claude@NOTE: no legacy ancestor - legacy has only the monolithic buid_luminance_histogram; matcher-phase work.
		// STATE[STUB]
	}

	void accumulate_motion_vectors( );

	float4 get_frame_luminance_parameters( bool& valid )
	{
		// claude@NOTE: post-legacy bool& validity out-param - the legacy no-arg body always
		// computed; assumed always-valid
		valid = true;

		// Average, Min, Max, Middle Gray
		vostok::math::float4					parameters(0.5f, 0.0f, 1.0f, 0.18f);

		post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();

		u32 const num_values				= NUM_HISTOGRAM_VALUES;
		float const min_luminance			= MIN_FRAME_LUMINANCE;
		float const max_luminance			= MAX_FRAME_LUMINANCE;
		float const luminance_range			= MAX_FRAME_LUMINANCE - MIN_FRAME_LUMINANCE;
		u32 const num_total_pixels			= NUM_TONEMAP_PIXELS;

		u32 const num_pixels_inv_bright		= u32((1.0f - pp_parameters.tonemap_bright_threshold) * num_total_pixels);
		u32 const num_pixels_median			= u32(pp_parameters.tonemap_median * num_total_pixels);
		u32 const num_pixels_darkness		= u32(pp_parameters.tonemap_darkness_threshold * num_total_pixels);

		float values						[num_values];
		buid_luminance_histogram			(
			&*m_context->m_targets->m_family[rt_generic_0].texture,
			array_size(values),
			min_luminance,
			max_luminance,
			values
		);

		float current_luminance				= min_luminance;
		u32   num_pixels					= 0;

		bool found_tonemap_bright			= false;
		bool found_tonemap_median			= false;
		bool found_tonemap_darkness			= false;

		for (s32 i = 0; i < num_values; i++)
		{
			u32 const num_in_next			= math::max<u32>(1, u32(values[i]));

			if (num_pixels + num_in_next >= num_pixels_inv_bright && !found_tonemap_bright)
			{
				parameters.z				= linear_interpolation2(
					current_luminance,
					current_luminance + luminance_range / num_values,
					float(num_pixels_inv_bright - num_pixels) / float(num_in_next)
				);
				found_tonemap_bright		= true;
			}
			if (num_pixels + num_in_next >= num_pixels_median && !found_tonemap_median)
			{
				parameters.x				= linear_interpolation2(
					current_luminance,
					current_luminance + luminance_range / num_values,
					float(num_pixels_median - num_pixels) / float(num_in_next)
				);
				found_tonemap_median		= true;
			}
			if (num_pixels + num_in_next >= num_pixels_darkness && !found_tonemap_darkness)
			{
				parameters.y				= linear_interpolation2(
					current_luminance,
					current_luminance + luminance_range / num_values,
					float(num_pixels_darkness - num_pixels) / float(num_in_next)
				);
				found_tonemap_darkness		= true;
			}
			current_luminance				+= luminance_range / num_values;
			num_pixels						+= num_in_next;
		}
		parameters.w						= pp_parameters.tonemap_middle_gray;
		return								parameters;
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
