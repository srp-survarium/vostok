#include "pch.h"
#include "stage_postprocess.h"

#include <vostok/console_command.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include "effect_gather_bloom.h"
#include "effect_gather_luminance.h"
#include "effect_gather_luminance_histogram.h"
#include "effect_blur.h"
#include "effect_complex_post_process_blend.h"
#include "effect_copy_image.h"
#include "effect_eye_adaptation.h"
#include "effect_god_rays.h"
#include "vertex_formats.h"
#include "renderer_context.h"
#include "scene_view.h"
#include "scene.h"
#include "lights_db.h"
#include "light.h"
#include "material_effects.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context_targets.h"
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include "statistics.h"
#include <vostok/render/core/options.h>
#include "effect_post_process_mlaa.h"
#include "effect_post_process_fxaa.h"
#include "effect_post_process_sraa.h"
#include "effect_post_process_sharpen.h"
#include "effect_post_process_downsample_frame.h"
#include "effect_image_space_reflections.h"
#include "effect_lens_flares.h"
#include "effect_motion_blur.h"
#include "effect_olta.h"
#include "effect_temporal_antialiasing.h"
#include "effect_aberration.h"
#include "effect_motion_vectors_accumulation.h"

namespace vostok {
namespace render {

static u32 const supported_kernels[] = { 3, 5, 7, 9, 13, 17, 21, 25 };

static bool s_debug_pp_0 = true;
static console_commands::cc_bool s_debug_pp_0_cc(
	"r_debug_pp_0", s_debug_pp_0, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_1 = true;
static console_commands::cc_bool s_debug_pp_1_cc(
	"r_debug_pp_1", s_debug_pp_1, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_2 = true;
static console_commands::cc_bool s_debug_pp_2_cc(
	"r_debug_pp_2", s_debug_pp_2, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_3 = true;
static console_commands::cc_bool s_debug_pp_3_cc(
	"r_debug_pp_3", s_debug_pp_3, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_4 = true;
static console_commands::cc_bool s_debug_pp_4_cc(
	"r_debug_pp_4", s_debug_pp_4, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_5 = true;
static console_commands::cc_bool s_debug_pp_5_cc(
	"r_debug_pp_5", s_debug_pp_5, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_6 = true;
static console_commands::cc_bool s_debug_pp_6_cc(
	"r_debug_pp_6", s_debug_pp_6, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_7 = true;
static console_commands::cc_bool s_debug_pp_7_cc(
	"r_debug_pp_7", s_debug_pp_7, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_8 = true;
static console_commands::cc_bool s_debug_pp_8_cc(
	"r_debug_pp_8", s_debug_pp_8, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_9 = true;
static console_commands::cc_bool s_debug_pp_9_cc(
	"r_debug_pp_9", s_debug_pp_9, false, console_commands::command_type_user_specific
);
static bool s_debug_pp_10 = true;
static console_commands::cc_bool s_debug_pp_10_cc(
	"r_debug_pp_10", s_debug_pp_10, false, console_commands::command_type_user_specific
);

static bool s_pp_map2d_value = false;
static console_commands::cc_bool s_pp_map2d(
	"pp_map2d", s_pp_map2d_value, false, console_commands::command_type_user_specific
);

struct screen_vertex {
	float4	position;
	float2	tc;

	void set( float4 const& in_position, float2 const& in_tc )
	{
		position	= in_position;
		tc			= in_tc;
	}
};

STATIC_SIZE_ASSERT( screen_vertex, 0x18 );

namespace {

struct color_grading_pixel {
	byte red;
	byte green;
	byte blue;
	byte alpha;
};

STATIC_SIZE_ASSERT( color_grading_pixel, 0x4 );

} // namespace

// claude@NOTE: second COMDAT instance of this function lives here in the
// target (FUNCTION BODY[0x606e50]); the defining copy is core's
// resource_manager.cpp (0x551b20). Likely a header-inline in the original
// source - reconcile at matcher phase; declaration only to keep one
// definition per exe.
res_texture_ptr create_color_grading_base_lut( u32 );

bloom_shader_constants::bloom_shader_constants( )
{
	m_bloom_parameters = backend::ref().register_constant_host( "bloom_parameters", rc_float );
	m_bloom_parameters1 = backend::ref().register_constant_host( "bloom_parameters1", rc_float );
}

void bloom_shader_constants::set( float bloom_scale, float bloom_max_color, float3 const& halo_color )
{
	float4 bloom_parameters(bloom_scale, bloom_max_color, 0.0f, 0.0f);

	backend::ref().set_ps_constant(m_bloom_parameters, bloom_parameters);
	backend::ref().set_ps_constant(m_bloom_parameters1, float4(halo_color, 0.0f));
}

dof_shader_constants::dof_shader_constants( )
{
	m_dof_height_lights		= backend::ref().register_constant_host( "dof_height_lights", rc_float );
	m_dof_parameters		= backend::ref().register_constant_host( "dof_parameters", rc_float );
	m_blurriness_amount		= backend::ref().register_constant_host( "blurriness_amount", rc_float );
	m_bokeh_dof_parameters	= backend::ref().register_constant_host( "bokeh_dof_parameters", rc_float );
}

void dof_shader_constants::set(
	float3 const&	blurriness_height_lights,
	float			distance,
	float			region,
	float			power,
	float			near_blur_amout,
	float			far_blur_amout,
	float			bokeh_dof_radius,
	float			bokeh_dof_density
)
{
	backend::ref().set_ps_constant(m_dof_height_lights, float4(blurriness_height_lights, 0));
	backend::ref().set_ps_constant(m_dof_parameters, float4(distance, region, power, 0));
	backend::ref().set_ps_constant(m_blurriness_amount, float4(near_blur_amout, far_blur_amout, 0, 0));
	backend::ref().set_ps_constant(m_bokeh_dof_parameters, float4(bokeh_dof_radius, bokeh_dof_density, 0, 0));
}

scene_shader_constants::scene_shader_constants( )
{
	m_frame_height_lights_and_desaturation_parameters	= backend::ref().register_constant_host( "frame_height_lights_and_desaturation", rc_float );
	m_scene_mid_tones_parameters						= backend::ref().register_constant_host( "frame_mid_tones", rc_float );
	m_scene_shadows_parameters							= backend::ref().register_constant_host( "frame_shadows", rc_float );
	m_gamma_correction_factor							= backend::ref().register_constant_host( "gamma_correction_factor", rc_float );
	m_scene_fade_parameters								= backend::ref().register_constant_host( "frame_fade", rc_float );
	m_filmic_tonemap_packed_parameters_0				= backend::ref().register_constant_host( "filmic_tonemap_packed_parameters_0", rc_float );
	m_filmic_tonemap_packed_parameters_1				= backend::ref().register_constant_host( "filmic_tonemap_packed_parameters_1", rc_float );
	m_c_eye_ray_corner									= backend::ref().register_constant_host( "s_eye_ray_corner", rc_float );
	m_image_grain_parameters								= backend::ref().register_constant_host( "image_grain_parameters", rc_float );
}

void scene_shader_constants::set(
	renderer_context* context,
	float3 const&	height_lights,
	float3 const&	mid_tones,
	float3 const&	shadows,
	float3 const&	fade_color,
	float			fade_amount,
	float			gamma_correction_factor,
	float			desaturation,
	float4 const&	image_grain_parameters,
	post_process_parameters const& parameters
)
{
	float4 frame_height_lights_and_desaturation(height_lights, desaturation);
	float4 scene_fade(fade_color, fade_amount);

	backend::ref().set_ps_constant(m_frame_height_lights_and_desaturation_parameters, frame_height_lights_and_desaturation);
	backend::ref().set_ps_constant(m_scene_mid_tones_parameters, mid_tones);
	backend::ref().set_ps_constant(m_scene_shadows_parameters, shadows);
	backend::ref().set_ps_constant(m_gamma_correction_factor, gamma_correction_factor);
	backend::ref().set_ps_constant(m_scene_fade_parameters, scene_fade);

	backend::ref().set_ps_constant(
		m_filmic_tonemap_packed_parameters_0,
		float4(
			parameters.filmic_tonemap_shoulder_strength,
			parameters.filmic_tonemap_linear_strength,
			parameters.filmic_tonemap_linear_angle,
			parameters.filmic_tonemap_toe_strength
		)
	);

	backend::ref().set_ps_constant(
		m_filmic_tonemap_packed_parameters_1,
		float4(
			parameters.filmic_tonemap_toe_numerator,
			parameters.filmic_tonemap_toe_denominator,
			parameters.filmic_tonemap_liner_white,
			parameters.frame_dark_desaturation_limit
		)
	);

	backend::ref().set_ps_constant(m_image_grain_parameters, image_grain_parameters);
}

static float gaussian( float x, float mu, float sigma )
{
	float const g = ( x - mu ) * ( 1.0f / sigma );
	return expf( -0.5f * g * g );
}

static void get_gaussain_weights_offsets(
	float*	out_weights,
	float*	out_offsets,
	u32		buffer_size,
	float,
	float,
	u32		num_samples,
	float	bloom_kernel
)
{
	float const sigma = bloom_kernel * 0.5f;
	float sum = 0.0f;

	for ( u32 i = 0; i < num_samples; ++i )
	{
		out_offsets[i] = ( float( i ) - bloom_kernel ) * ( 1.0f / float( buffer_size ) );

		out_weights[i] = gaussian( float( i ), bloom_kernel, sigma );
		sum += out_weights[i];
	}

	for ( u32 i = 0; i < num_samples; ++i )
		out_weights[i] /= sum;
}

bool stage_postprocess::is_effects_ready( ) const
{
	return
		   m_sh_gather_bloom.c_ptr() != NULL
		&& m_sh_gather_luminance.c_ptr() != NULL
		&& m_sh_gather_luminance_histogram.c_ptr() != NULL
		&& m_sh_eye_adaptation.c_ptr() != NULL
		&& m_sh_blur[0].c_ptr() != NULL
		&& m_sh_blur[1].c_ptr() != NULL
		&& m_sh_blur[2].c_ptr() != NULL
		&& m_sh_blur[3].c_ptr() != NULL
		&& m_sh_blur[4].c_ptr() != NULL
		&& m_sh_blur[5].c_ptr() != NULL
		&& m_sh_blur[6].c_ptr() != NULL
		&& m_sh_blur[7].c_ptr() != NULL
		&& m_image_space_reflections_effect.c_ptr() != NULL
		&& m_sh_complex_blend[0][0][0].c_ptr() != NULL
		&& m_sh_complex_blend[1][0][0].c_ptr() != NULL
		&& m_sh_complex_blend[1][1][0].c_ptr() != NULL
		&& m_sh_complex_blend[0][0][1].c_ptr() != NULL
		&& m_sh_complex_blend[1][0][1].c_ptr() != NULL
		&& m_sh_complex_blend[1][1][1].c_ptr() != NULL
		&& m_sh_effect_copy_image.c_ptr() != NULL
		&& m_lens_flares_effect.c_ptr() != NULL
		&& m_post_process_antialiasing_shader
		&& m_post_process_antialiasing_shader_fxaa.c_ptr() != NULL
		&& m_post_process_antialiasing_shader_sraa.c_ptr() != NULL
		&& m_post_process_shader_sharpen.c_ptr() != NULL
		&& m_god_rays_effect.c_ptr() != NULL
		&& m_post_process_downsample_frame_effect.c_ptr() != NULL
		&& m_motion_blur_effect.c_ptr() != NULL
		&& m_olta_effect.c_ptr() != NULL
		&& m_aberration_effect.c_ptr() != NULL
		&& m_temporal_antialiasing_effect.c_ptr() != NULL
		&& m_motion_vectors_accumulation_effect.c_ptr() != NULL;
}

stage_postprocess::stage_postprocess(
	renderer* in_renderer,
	renderer_context* context
) :
	stage										( in_renderer, context ),
	m_prev_view_matrix						( float4x4().identity() )
{
	effect_manager::ref().create_effect<effect_gather_bloom>(&m_sh_gather_bloom);
	effect_manager::ref().create_effect<effect_gather_luminance>(&m_sh_gather_luminance);
	effect_manager::ref().create_effect<effect_gather_luminance_histogram>(&m_sh_gather_luminance_histogram);
	effect_manager::ref().create_effect<effect_eye_adaptation>(&m_sh_eye_adaptation);
	// 2 target lines are likely retail-compiled-out source.
	effect_manager::ref().create_effect< effect_blur<3> >(&m_sh_blur[0]);
	effect_manager::ref().create_effect< effect_blur<5> >(&m_sh_blur[1]);
	effect_manager::ref().create_effect< effect_blur<7> >(&m_sh_blur[2]);
	effect_manager::ref().create_effect< effect_blur<9> >(&m_sh_blur[3]);
	effect_manager::ref().create_effect< effect_blur<13> >(&m_sh_blur[4]);
	effect_manager::ref().create_effect< effect_blur<17> >(&m_sh_blur[5]);
	effect_manager::ref().create_effect< effect_blur<21> >(&m_sh_blur[6]);
	effect_manager::ref().create_effect< effect_blur<25> >(&m_sh_blur[7]);

	effect_manager::ref().create_effect< effect_complex_post_process_blend<false, false, false> >(&m_sh_complex_blend[0][0][0]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<true, false, false> >(&m_sh_complex_blend[1][0][0]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<true, true, false> >(&m_sh_complex_blend[1][1][0]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<false, false, true> >(&m_sh_complex_blend[0][0][1]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<true, false, true> >(&m_sh_complex_blend[1][0][1]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<true, true, true> >(&m_sh_complex_blend[1][1][1]);

	effect_manager::ref().create_effect<effect_copy_image>(&m_sh_effect_copy_image);
	effect_manager::ref().create_effect<effect_post_process_mlaa>(&m_post_process_antialiasing_shader);
	effect_manager::ref().create_effect<effect_post_process_fxaa>(&m_post_process_antialiasing_shader_fxaa);
	effect_manager::ref().create_effect<effect_post_process_sraa>(&m_post_process_antialiasing_shader_sraa);
	effect_manager::ref().create_effect<effect_post_process_sharpen>(&m_post_process_shader_sharpen);
	effect_manager::ref().create_effect<effect_god_rays>(&m_god_rays_effect);
	effect_manager::ref().create_effect<effect_post_process_downsample_frame>(&m_post_process_downsample_frame_effect);
	effect_manager::ref().create_effect<effect_image_space_reflections>(&m_image_space_reflections_effect);
	effect_manager::ref().create_effect<effect_lens_flares>(&m_lens_flares_effect);

	effect_manager::ref().create_effect<effect_motion_blur>(&m_motion_blur_effect);
	effect_manager::ref().create_effect<effect_olta>(&m_olta_effect);

	effect_manager::ref().create_effect<effect_temporal_antialiasing>(&m_temporal_antialiasing_effect);

	effect_manager::ref().create_effect<effect_aberration>(&m_aberration_effect);

	u8 data[Kb];
	effect_options_descriptor desc(data, sizeof(data));
	desc["vertex_input_type"] = skeletal_4_bones_mesh_vertex_input_type;
	desc["cull_mode"] = D3D11_CULL_NONE;

	effect_manager::ref().create_effect<effect_motion_vectors_accumulation>(&m_motion_vectors_accumulation_effect, desc);
	// 5 target lines are likely retail-compiled-out source.
	m_blur_offsets_weights	= backend::ref().register_constant_host("offsets_weights", rc_float);
	m_kernel_offsets		= backend::ref().register_constant_host("kernel_offsets", rc_float);
	m_elapsed_time_parameter= backend::ref().register_constant_host("elapsed_time", rc_float);
	m_adaptation_factor		= backend::ref().register_constant_host("adaptation_factor", rc_float);
	m_sun_direction_parameter = backend::ref().register_constant_host("sun_direction_parameter", rc_float);
	m_frame_luminance_parameter = backend::ref().register_constant_host("frame_luminance_parameter", rc_float);
	m_luminance_range_parameter_parameter			 = backend::ref().register_constant_host( "luminance_range_parameter", rc_float );
	m_gamma_correction_factor							= backend::ref().register_constant_host( "gamma_correction_factor", rc_float );
	m_fxaa_parameters								= backend::ref().register_constant_host( "fxaa_parameters", rc_float );
	m_god_rays_parameters0							= backend::ref().register_constant_host( "god_rays_parameters0", rc_float );
	m_god_rays_parameters1							= backend::ref().register_constant_host( "god_rays_parameters1", rc_float );
	m_god_rays_parameters2							= backend::ref().register_constant_host( "god_rays_parameters2", rc_float );
	m_c_eye_ray_corner								= backend::ref().register_constant_host( "s_eye_ray_corner", rc_float );
	m_c_frame_index									= backend::ref().register_constant_host( "frame_index", rc_int );

	m_blur_target_size_parameter						= backend::ref().register_constant_host( "blur_target_size", rc_float );

	m_lens_flares_parameters							= backend::ref().register_constant_host( "lens_flares_parameters", rc_float );
	m_prev_view_matrix_parameter						= backend::ref().register_constant_host( "prev_view_matrix_parameter", rc_float );
	m_prev_world_view_matrix_parameter				= backend::ref().register_constant_host( "prev_world_view_matrix_parameter", rc_float );
	m_inverse_world_matrix_parameter					= backend::ref().register_constant_host( "inverse_world_matrix_parameter", rc_float );
	m_frame_delta_parameter							= backend::ref().register_constant_host( "frame_delta_parameter", rc_float );
	m_motion_blur_scale_parameter						= backend::ref().register_constant_host( "motion_blur_scale_parameter", rc_float );
	// 2 target lines are likely retail-compiled-out source.
	m_aberration_parameters							= backend::ref().register_constant_host( "aberration_parameters", rc_float );
	// 2 target lines are likely retail-compiled-out source.
	m_color_grading_base_lut = create_color_grading_base_lut(16);

	m_textures.resize(10, NULL);

	const D3D_INPUT_ELEMENT_DESC screen_vertex_layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	    0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	};

	u16 indices[6]			= { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib		= resource_manager::ref().create_buffer( 6*sizeof(u16), indices, enum_buffer_type_index, false, false);
	m_screen_vertex_geometry	= resource_manager::ref().create_geometry(
		screen_vertex_layout,
		sizeof(screen_vertex),
		backend::ref().vertex.buffer(),
		*m_screen_vertex_ib
	);

	m_enabled						= options::ref().current.m_enabled_post_process_stage;

	m_image_grain_random_offsets	= float2(0.0f, 0.0f);
}

void stage_postprocess::fill_surface( render_target_ptr surf0, render_target_ptr surf1 )
{
	float t_w = float(surf0->width());
	float t_h = float(surf0->height());

	backend::ref().set_render_targets( &*surf0, surf1.c_ptr() ? &*surf1 : 0, 0, 0);
	backend::ref().set_depth_stencil_target(0);//reset_depth_stencil_target();

	u32	    color = math::color_rgba( 255u, 255u, 255u, 255u);

	float2	p0( 0,0);
	float2	p1( 1,1);

	u32		offset;

	vertex_formats::TL* pv = ( vertex_formats::TL*)backend::ref().vertex.lock( 4, sizeof(vertex_formats::TL), offset);
	pv->set( 0,	  t_h, 0, 1.0, color, p0.x, p1.y); pv++;
	pv->set( 0,	  0,   0, 1.0, color, p0.x, p0.y); pv++;
	pv->set( t_w, t_h, 0, 1.0, color, p1.x, p1.y); pv++;
	pv->set( t_w, 0,   0, 1.0, color, p1.x, p0.y); pv++;
	backend::ref().vertex.unlock();

	m_context->m_g_quad_uv->apply( );

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

// TODO: rename
void stage_postprocess::fill_surface2( render_target_ptr surf )
{
	backend::ref().set_render_targets( &*surf, 0, 0, 0);
	backend::ref().set_depth_stencil_target(0);

	u32		offset;

	screen_vertex* pv = (screen_vertex*)backend::ref().vertex.lock(4, sizeof(screen_vertex), offset);
	pv->set( float4(-1.0f, -1.0f, 0.0f, 1.0f), float2(0.0f, 1.0f)); pv++;
	pv->set( float4(-1.0f,  1.0f, 0.0f, 1.0f), float2(0.0f, 0.0f)); pv++;
	pv->set( float4( 1.0f, -1.0f, 0.0f, 1.0f), float2(1.0f, 1.0f)); pv++;
	pv->set( float4( 1.0f,  1.0f, 0.0f, 1.0f), float2(1.0f, 0.0f)); pv++;
	backend::ref().vertex.unlock();

	m_screen_vertex_geometry->apply( );

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( surf->width( ) );
	tmp_viewport.Height = float( surf->height( ) );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;
	backend::ref( ).set_viewport( tmp_viewport );

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);

	backend::ref( ).set_viewport( orig_viewport );
}

void stage_postprocess::clear_surface( render_target_ptr surf )
{
	backend::ref().set_render_targets	( &*surf, 0, 0, 0);
	backend::ref().clear_render_targets	(0.0f, 0.0f, 0.0f, 0.f);
}

void stage_postprocess::measure_per_pixel_luminance( res_texture* scene_texture, float4& out_avrg_min_max )
{
	VOSTOK_UNREFERENCED_PARAMETER(out_avrg_min_max);

	s32 const last_target_index = s32(rt_frame_luminance0 + rt_num_frame_luminance_targets) - 1;

	m_sh_gather_luminance->apply( effect_gather_luminance::gather_log_luminance, 0 );
	backend::ref().set_ps_texture( "t_frame_color0", scene_texture);
	fill_surface2(m_context->m_targets->m_family[last_target_index].target);

	for (s32 lum_rt_index=last_target_index-1; lum_rt_index>=rt_frame_luminance0; lum_rt_index--)
	{
		if (lum_rt_index==rt_frame_luminance0)
			m_sh_gather_luminance->apply( effect_gather_luminance::gather_exp_luminance, 0 );
		else
			m_sh_gather_luminance->apply( effect_gather_luminance::gather_luminance, 0 );

		backend::ref().set_ps_texture( "t_frame_color1", &*m_context->get_t(enum_render_target_index(lum_rt_index+1)) );
		fill_surface2(m_context->m_targets->m_family[lum_rt_index].target);
	}
	// 2 target lines are likely retail-compiled-out source.
	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();
	if (math::is_similar(pp_parameters.adaptation_speed, 0.0f, 0.05f))
	{
		// 11 target lines are likely retail-compiled-out source.
		m_sh_eye_adaptation->apply( 1, 0 );
		backend::ref().set_ps_texture( "t_luminance", &*m_context->get_t(rt_frame_luminance0) );
		fill_surface2(m_context->m_targets->m_family[rt_frame_luminance_current].target);
	}
}

void stage_postprocess::compute_per_pixel_eye_adaptated_luminance( )
{
	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();

	m_sh_eye_adaptation->apply( 0, 0 );

	float time_delta					= m_context->get_time_delta();
	math::clamp							(time_delta, 0.0f, 1.0f / 30.0f);

	backend::ref().set_ps_constant( m_elapsed_time_parameter, time_delta );

	backend::ref().set_ps_constant( m_adaptation_factor, time_delta * (1.0f / pp_parameters.adaptation_speed));

	static bool fist_pass = true;
	if (fist_pass)
	{
		backend::ref().set_ps_texture( "t_previous_luminance", &*m_context->get_t(rt_frame_luminance0) );
		fist_pass = false;
	}
	else
	{
		backend::ref().set_ps_texture( "t_previous_luminance", &*m_context->get_t(rt_frame_luminance_previous) );
	}

	backend::ref().set_ps_texture( "t_current_luminanace", &*m_context->get_t(rt_frame_luminance0) );
	fill_surface2(m_context->m_targets->m_family[rt_frame_luminance_current].target);

	backend::ref().flush_rt_shader_resources();

	m_sh_eye_adaptation->apply( 1, 0 );
	backend::ref().set_ps_texture( "t_luminance", &*m_context->get_t(rt_frame_luminance_current) );
	fill_surface2(m_context->m_targets->m_family[rt_frame_luminance_previous].target);
}

float4 stage_postprocess::compute_luminance_parameters( u32 frame_delta )
{
	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();

	float4								frame_luminance_parameter;
	measure_per_pixel_luminance			(&*m_context->get_t(rt_generic_0), frame_luminance_parameter);

	if (!math::is_similar(pp_parameters.adaptation_speed, 0.0f, 0.05f))
		compute_per_pixel_eye_adaptated_luminance();

	return frame_luminance_parameter;
}

void stage_postprocess::execute_disabled( )
{
	if (!is_effects_ready())
		return;

	m_sh_effect_copy_image->apply	(effect_copy_image::copy_rewrite, 0);
	backend::ref().set_ps_texture	( "t_base", &*m_context->get_t(rt_generic_0));
	backend::ref().set_ps_constant	(m_gamma_correction_factor, 1.0f);
	fill_surface					(m_context->get_rt(rt_present), render_target_ptr( ));
	m_context->set_w				(float4x4().identity());

	m_prev_view_matrix = m_context->get_v();
}

void stage_postprocess::process_blur(
	render_target*	rt0,
	res_texture*	t0,
	render_target*	rt1,
	res_texture*	t1,
	u32			kernel_index
)
{
	backend::ref( ).flush_rt_shader_resources( );

	float t_w = float( rt0->width( ) );
	float t_h = float( rt0->height( ) );

	u32 const bloom_kernal = supported_kernels[kernel_index];

	float* weights_h = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
	float* offsets_h = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
	float* weights_v = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
	float* offsets_v = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );

	float const bloom_radius = float( bloom_kernal - 1 ) * 0.5f;

	get_gaussain_weights_offsets( weights_h, offsets_h, u32( t_w ), bloom_radius, bloom_radius, bloom_kernal, bloom_radius );
	get_gaussain_weights_offsets( weights_v, offsets_v, u32( t_h ), bloom_radius, bloom_radius, bloom_kernal, bloom_radius );

	float4* offsets_weights = static_cast<float4*>( ALLOCA( sizeof( float4 ) * bloom_kernal ) );

	for ( u32 i = 0; i < bloom_kernal; ++i ) {
		offsets_weights[i] = float4( offsets_h[i], weights_h[i], offsets_v[i], weights_v[i] );
	}
	m_sh_blur[kernel_index]->apply( 0, 0 );
	backend::ref( ).set_ps_texture( "t_base", t0 );
	backend::ref( ).set_ps_constant( m_blur_offsets_weights, offsets_weights, bloom_kernal );
	fill_surface( rt1, render_target_ptr( ) );

	backend::ref( ).flush_rt_shader_resources( );

	m_sh_blur[kernel_index]->apply( 1, 0 );
	backend::ref( ).set_ps_texture( "t_base", t1 );
	backend::ref( ).set_ps_constant( m_blur_offsets_weights, offsets_weights, bloom_kernal );
	fill_surface( rt0, render_target_ptr( ) );
}

void stage_postprocess::advanced_bloom( )
{
	post_process_parameters const& pp_parameters = m_context->scene_view( )->post_process_parameters( );

	u32 const kernel_index = math::clamp_r( pp_parameters.blur_kernel, 0u, 7u );

	float t_w = float( m_context->get_t( rt_blur_0 )->width( ) );
	float t_h = float( m_context->get_t( rt_blur_0 )->height( ) );
	float t_w_inv = 1.0f / float( m_context->get_t( rt_blur_0 )->width( ) );
	float t_h_inv = 1.0f / float( m_context->get_t( rt_blur_0 )->height( ) );
	// 2 target lines are likely retail-compiled-out source.
	m_sh_blur[0]->apply( 3, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_2 ) );
	backend::ref( ).set_ps_constant( m_blur_target_size_parameter, float4( t_w, t_h, t_w_inv, t_h_inv ) );
	fill_surface( m_context->get_rt( rt_blur_4 ), render_target_ptr( ) );
	backend::ref( ).flush_rt_views( );
	// 2 target lines are likely retail-compiled-out source.
	m_sh_blur[0]->apply( 3, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_4 ) );
	backend::ref( ).set_ps_constant( m_blur_target_size_parameter, float4( t_w * 0.5f, t_h * 0.5f, t_w_inv * 2.0f, t_h_inv * 2.0f ) );
	fill_surface( m_context->get_rt( rt_blur_5 ), render_target_ptr( ) );
	backend::ref( ).flush_rt_views( );
	// 2 target lines are likely retail-compiled-out source.
	m_sh_blur[0]->apply( 3, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_5 ) );
	backend::ref( ).set_ps_constant( m_blur_target_size_parameter, float4( t_w * 0.25f, t_h * 0.25f, t_w_inv * 4.0f, t_h_inv * 4.0f ) );
	fill_surface( m_context->get_rt( rt_blur_6 ), render_target_ptr( ) );
	backend::ref( ).flush_rt_views( );
	// 2 target lines are likely retail-compiled-out source.
	m_sh_blur[0]->apply( 3, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_6 ) );
	backend::ref( ).set_ps_constant( m_blur_target_size_parameter, float4( t_w * 0.125f, t_h * 0.125f, t_w_inv * 8.0f, t_h_inv * 8.0f ) );
	fill_surface( m_context->get_rt( rt_blur_7 ), render_target_ptr( ) );
	backend::ref( ).flush_rt_views( );
	// 2 target lines are likely retail-compiled-out source.
	process_blur( m_context->get_rt( rt_blur_2 ).c_ptr( ), m_context->get_t( rt_blur_2 ).c_ptr( ), m_context->get_rt( rt_blur_3 ).c_ptr( ), m_context->get_t( rt_blur_3 ).c_ptr( ), kernel_index );
	// 2 target lines are likely retail-compiled-out source.
	process_blur( m_context->get_rt( rt_blur_4 ).c_ptr( ), m_context->get_t( rt_blur_4 ).c_ptr( ), m_context->get_rt( rt_blur_4_0 ).c_ptr( ), m_context->get_t( rt_blur_4_0 ).c_ptr( ), kernel_index );
	// 2 target lines are likely retail-compiled-out source.
	process_blur( m_context->get_rt( rt_blur_5 ).c_ptr( ), m_context->get_t( rt_blur_5 ).c_ptr( ), m_context->get_rt( rt_blur_5_0 ).c_ptr( ), m_context->get_t( rt_blur_5_0 ).c_ptr( ), kernel_index );
	// 2 target lines are likely retail-compiled-out source.
	process_blur( m_context->get_rt( rt_blur_6 ).c_ptr( ), m_context->get_t( rt_blur_6 ).c_ptr( ), m_context->get_rt( rt_blur_6_0 ).c_ptr( ), m_context->get_t( rt_blur_6_0 ).c_ptr( ), kernel_index );
	// 2 target lines are likely retail-compiled-out source.
	process_blur( m_context->get_rt( rt_blur_7 ).c_ptr( ), m_context->get_t( rt_blur_7 ).c_ptr( ), m_context->get_rt( rt_blur_7_0 ).c_ptr( ), m_context->get_t( rt_blur_7_0 ).c_ptr( ), kernel_index );

	clear_surface( m_context->get_rt( rt_blur_3 ) );

	m_sh_blur[0]->apply( 4, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_2 ) );
	fill_surface( m_context->get_rt( rt_blur_3 ), render_target_ptr( ) );

	m_sh_blur[0]->apply( 5, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_4 ) );
	fill_surface( m_context->get_rt( rt_blur_3 ), render_target_ptr( ) );

	m_sh_blur[0]->apply( 5, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_5 ) );
	fill_surface( m_context->get_rt( rt_blur_3 ), render_target_ptr( ) );

	m_sh_blur[0]->apply( 5, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_6 ) );
	fill_surface( m_context->get_rt( rt_blur_3 ), render_target_ptr( ) );

	m_sh_blur[0]->apply( 5, 0 );
	backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_7 ) );
	fill_surface( m_context->get_rt( rt_blur_3 ), render_target_ptr( ) );
}

inline bool remove_model_skeletal_filter_predicate::operator()( render_surface_instance* in_model )
{
	material_effects& effects = in_model->m_render_surface->get_material_effects( );

	if ( !effects.m_effects[gbuffer_render_stage] )
		return true;

	bool result =
		in_model->m_render_surface->get_vertex_input_type( ) == skeletal_4_bones_mesh_vertex_input_type ||
		in_model->m_render_surface->get_vertex_input_type( ) == skeletal_3_bones_mesh_vertex_input_type ||
		in_model->m_render_surface->get_vertex_input_type( ) == skeletal_2_bones_mesh_vertex_input_type ||
		in_model->m_render_surface->get_vertex_input_type( ) == skeletal_1_bones_mesh_vertex_input_type;

	if ( m_inverse_flag )
		result = !result;

	return result;
}

void stage_postprocess::accumulate_motion_vectors( )
{
	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_object_motion_vectors ),
		0,
		0,
		0
	);
	backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_context->get_rt( rt_object_motion_vectors )->width( ) );
	tmp_viewport.Height = float( m_context->get_rt( rt_object_motion_vectors )->height( ) );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;
	backend::ref( ).set_viewport( tmp_viewport );

	vector< render_surface_instance* > visible_models;
	vector< render_surface_instance* > visible_moved_models;
	visible_models.reserve( 1024 );
	visible_moved_models.reserve( 256 );
	visible_models = m_context->get_scene_view( )->get_visible_opaque_models( );
	visible_moved_models = m_context->get_scene_view( )->get_visible_moved_opaque_models( );

	visible_models.erase(
		std::remove_if(
			visible_models.begin( ),
			visible_models.end( ),
			remove_model_skeletal_filter_predicate( true )
		),
		visible_models.end( )
	);
	visible_moved_models.erase(
		std::remove_if(
			visible_moved_models.begin( ),
			visible_moved_models.end( ),
			remove_model_skeletal_filter_predicate( false )
		),
		visible_moved_models.end( )
	);

	render_surface_instance** it = visible_moved_models.begin( );
	for ( ; it != visible_moved_models.end( ); ++it )
		visible_models.push_back( *it );

	it = visible_models.begin( );
	for ( ; it != visible_models.end( ); ++it )
	{
		render_surface_instance& instance = **it;
		material_effects& effects = instance.m_render_surface->get_material_effects( );

		m_context->set_w( *instance.m_transform );
		effects.m_effects[gbuffer_render_stage]->apply( 6, 0 );

		float4x4 local_to_world = *instance.m_transform;
		float4x4 prev_world_matrix = local_to_world;

		prev_matrix_map_type::iterator prev_matrix_it = m_prev_matrix_map.find( &instance );
		if ( prev_matrix_it != m_prev_matrix_map.end( ) )
		{
			prev_world_matrix = prev_matrix_it->second;
			m_prev_matrix_map[&instance] = local_to_world;
		}
		else
		{
			m_prev_matrix_map.insert( std::make_pair( &instance, *instance.m_transform ) );
		}

		local_to_world.try_invert( local_to_world );
		backend::ref( ).set_ps_constant(
			m_prev_world_view_matrix_parameter,
			transpose( mul4x3( prev_world_matrix, m_prev_view_matrix ) )
		);
		backend::ref( ).set_ps_constant(
			m_inverse_world_matrix_parameter,
			transpose( local_to_world )
		);

		instance.set_constants( );
		instance.m_render_surface->m_render_geometry.geom->apply( );
		backend::ref( ).render_indexed(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			instance.m_render_surface->m_render_geometry.primitive_count * 3,
			0,
			0
		);
	}

	backend::ref( ).set_viewport( orig_viewport );
}

void stage_postprocess::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !is_enabled( ) )
	{
		execute_disabled( );
		return;
	}

	static bool prev_view_initialized = false;
	if ( !prev_view_initialized )
	{
		m_prev_view_matrix = m_context->get_v( );
		prev_view_initialized = true;
	}

	if ( !m_context->scene_view( )->is_use_post_process( ) )
	{
		m_sh_effect_copy_image->apply( effect_copy_image::copy_rewrite, 0 );
		backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_generic_0 ) );
		backend::ref( ).set_ps_constant( m_gamma_correction_factor, 1.0f );
		fill_surface( m_context->get_rt( rt_present ), render_target_ptr( ) );
		m_context->set_w( float4x4( ).identity( ) );
		return;
	}

	post_process_parameters const& pp_parameters =
		m_context->scene_view( )->post_process_parameters( );

	if ( options::ref( ).current.m_post_process_quality &&
		 pp_parameters.use_image_space_reflections )
	{
		m_post_process_downsample_frame_effect->apply( 0, 0 );
		fill_surface2( m_context->get_rt( rt_final_frame_downsampled ) );
		m_post_process_downsample_frame_effect->apply( 1, 0 );
		fill_surface2( m_context->get_rt( rt_final_frame_downsampled_temp ) );
		backend::ref( ).flush_rt_shader_resources( );
		m_post_process_downsample_frame_effect->apply( 2, 0 );
		fill_surface2( m_context->get_rt( rt_final_frame_downsampled ) );

		m_image_space_reflections_effect->apply( 0, 0 );
		backend::ref( ).set_ps_constant(
			m_blur_target_size_parameter,
			m_context->get_screen_resolution( )
		);
		fill_surface2( m_context->get_rt( rt_generic_0 ) );
	}

	float3 sun_direction( 0.0f, 0.0f, 0.0f );
	light* the_sun = m_context->scene( )->lights( ).get_sun( ).c_ptr( );
	if ( options::ref( ).current.m_post_process_quality &&
		 options::ref( ).current.m_use_god_rays &&
		 the_sun && the_sun->m_enabled )
	{
		m_god_rays_effect->apply( 0, 0 );
		sun_direction = the_sun->direction;
		backend::ref( ).set_ps_constant( m_sun_direction_parameter, sun_direction );
		fill_surface2( m_context->get_rt( rt_light_scattering_mask ) );

		m_god_rays_effect->apply( 1, 0 );
		backend::ref( ).set_ps_constant( m_sun_direction_parameter, sun_direction );
		backend::ref( ).set_ps_constant(
			m_god_rays_parameters0,
			float4(
				pp_parameters.god_rays_color_0.xyz( ),
				pp_parameters.god_rays_color_blend_power
			)
		);
		backend::ref( ).set_ps_constant(
			m_god_rays_parameters1,
			float4(
				pp_parameters.god_rays_color_1.xyz( ),
				pp_parameters.god_rays_intensity
			)
		);
		backend::ref( ).set_ps_constant(
			m_god_rays_parameters2,
			float4( pp_parameters.god_rays_attenuation_power, 0.0f, 0.0f, 0.0f )
		);
		fill_surface2( m_context->get_rt( rt_light_scattering_result ) );
	}

	float t_w = float( m_context->get_t( rt_blur_0 )->width( ) );
	float t_h = float( m_context->get_t( rt_blur_0 )->height( ) );
	float s_u = 1.0f / backend::ref( ).target_width( );
	float s_v = 1.0f / backend::ref( ).target_height( );

	kernel_offsets[0] = float4( -s_u,  s_v, 0.0f, 0.0f );
	kernel_offsets[1] = float4(  s_u,  s_v, 0.0f, 0.0f );
	kernel_offsets[2] = float4( -s_u, -s_v, 0.0f, 0.0f );
	kernel_offsets[3] = float4(  s_u, -s_v, 0.0f, 0.0f );
	kernel_offsets[4] = float4( -s_u, 0.0f, 0.0f, 0.0f );
	kernel_offsets[5] = float4(  s_u, 0.0f, 0.0f, 0.0f );
	kernel_offsets[6] = float4( 0.0f, -s_v, 0.0f, 0.0f );
	kernel_offsets[7] = float4( 0.0f,  s_v, 0.0f, 0.0f );

	backend::ref( ).flush_rt_views( );
	clear_surface( m_context->get_rt( rt_blur_0 ) );
	clear_surface( m_context->get_rt( rt_blur_1 ) );
	clear_surface( m_context->get_rt( rt_blur_2 ) );
	clear_surface( m_context->get_rt( rt_blur_3 ) );

	float4 frame_luminance_parameter = compute_luminance_parameters( 16 );

	if ( s_debug_pp_2 && pp_parameters.enable_bloom )
	{
		m_sh_gather_bloom->apply( 0, 0 );
		m_dof_shader_constants.set(
			pp_parameters.dof_height_lights,
			pp_parameters.dof_focus_distance,
			pp_parameters.dof_focus_region,
			pp_parameters.dof_focus_power,
			pp_parameters.dof_near_blur_amount,
			pp_parameters.dof_far_blur_amount,
			pp_parameters.bokeh_dof_radius,
			pp_parameters.bokeh_dof_density
		);
		backend::ref( ).set_ps_constant( m_kernel_offsets, kernel_offsets );
		m_bloom_shader_constants.set(
			pp_parameters.bloom_scale,
			pp_parameters.bloom_max_color,
			pp_parameters.bloom_halo_color
		);
		backend::ref( ).set_ps_constant(
			m_frame_luminance_parameter,
			frame_luminance_parameter
		);
		fill_surface(
			m_context->get_rt( rt_blur_0 ),
			m_context->get_rt( rt_blur_2 )
		);
	}

	if ( s_debug_pp_3 )
	{
		u32 const dof_kernel_index = math::clamp_r(
			pp_parameters.dof_blur_kernel,
			0u,
			7u
		);
		u32 const dof_kernal = supported_kernels[dof_kernel_index];
		float* weights_h = static_cast<float*>( ALLOCA( sizeof( float ) * dof_kernal ) );
		float* offsets_h = static_cast<float*>( ALLOCA( sizeof( float ) * dof_kernal ) );
		float* weights_v = static_cast<float*>( ALLOCA( sizeof( float ) * dof_kernal ) );
		float* offsets_v = static_cast<float*>( ALLOCA( sizeof( float ) * dof_kernal ) );
		float const dof_radius = float( dof_kernal - 1 ) * 0.5f;

		get_gaussain_weights_offsets(
			weights_h, offsets_h, u32( t_w ),
			dof_radius, dof_radius, dof_kernal, dof_radius
		);
		get_gaussain_weights_offsets(
			weights_v, offsets_v, u32( t_h ),
			dof_radius, dof_radius, dof_kernal, dof_radius
		);

		float4* offsets_weights = static_cast<float4*>(
			ALLOCA( sizeof( float4 ) * dof_kernal )
		);
		for ( u32 i = 0; i < dof_kernal; ++i )
			offsets_weights[i] = float4(
				offsets_h[i], weights_h[i], offsets_v[i], weights_v[i]
			);

		m_sh_blur[dof_kernel_index]->apply( 0, 0 );
		m_textures[0] = m_context->get_t( rt_blur_0 );
		backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_0 ) );
		backend::ref( ).set_ps_constant(
			m_blur_offsets_weights, offsets_weights, dof_kernal
		);
		fill_surface( m_context->get_rt( rt_blur_1 ), render_target_ptr( ) );

		backend::ref( ).flush_rt_shader_resources( );

		m_sh_blur[dof_kernel_index]->apply( 1, 0 );
		m_textures[1] = m_context->get_t( rt_blur_1 );
		backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_1 ) );
		backend::ref( ).set_ps_constant(
			m_blur_offsets_weights, offsets_weights, dof_kernal
		);
		fill_surface( m_context->get_rt( rt_blur_0 ), render_target_ptr( ) );
	}

	static math::random32 randomizer( 1000 );
	if ( m_context->scene_view( )->get_render_frame_index( ) %
		 pp_parameters.image_grain_update_frequency == 0 )
	{
		m_image_grain_random_offsets = float2(
			randomizer.random_f( 1.0f ),
			randomizer.random_f( 1.0f )
		);
	}

	float4 image_grain_parameters(
		m_image_grain_random_offsets.x,
		m_image_grain_random_offsets.y,
		pp_parameters.image_grain_scale,
		pp_parameters.vignette_power
	);

	if ( s_debug_pp_4 )
	{
		if ( options::ref( ).current.m_post_process_quality &&
			 pp_parameters.enable_advanced_bloom )
		{
			advanced_bloom( );
		}
		else
		{
			u32 const kernel_index = math::clamp_r(
				pp_parameters.blur_kernel,
				0u,
				7u
			);
			u32 const bloom_kernal = supported_kernels[kernel_index];
			float* weights_h = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
			float* offsets_h = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
			float* weights_v = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
			float* offsets_v = static_cast<float*>( ALLOCA( sizeof( float ) * bloom_kernal ) );
			float const bloom_radius = float( bloom_kernal - 1 ) * 0.5f;

			get_gaussain_weights_offsets(
				weights_h, offsets_h, u32( t_w ),
				bloom_radius, bloom_radius, bloom_kernal, bloom_radius
			);
			get_gaussain_weights_offsets(
				weights_v, offsets_v, u32( t_h ),
				bloom_radius, bloom_radius, bloom_kernal, bloom_radius
			);

			float4* offsets_weights = static_cast<float4*>(
				ALLOCA( sizeof( float4 ) * bloom_kernal )
			);
			for ( u32 i = 0; i < bloom_kernal; ++i )
				offsets_weights[i] = float4(
					offsets_h[i], weights_h[i], offsets_v[i], weights_v[i]
				);

			backend::ref( ).flush_rt_shader_resources( );
			m_sh_blur[kernel_index]->apply( 0, 0 );
			m_textures[0] = m_context->get_t( rt_blur_2 );
			backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_2 ) );
			backend::ref( ).set_ps_constant(
				m_blur_offsets_weights, offsets_weights, bloom_kernal
			);
			fill_surface( m_context->get_rt( rt_blur_1 ), render_target_ptr( ) );

			backend::ref( ).flush_rt_shader_resources( );
			m_sh_blur[kernel_index]->apply( 2, 0 );
			m_textures[1] = m_context->get_t( rt_blur_1 );
			backend::ref( ).set_ps_texture( "t_base", &*m_context->get_t( rt_blur_1 ) );
			backend::ref( ).set_ps_constant(
				m_blur_offsets_weights, offsets_weights, bloom_kernal
			);
			fill_surface( m_context->get_rt( rt_blur_3 ), render_target_ptr( ) );
		}
	}

	if ( s_debug_pp_5 && pp_parameters.use_dynamic_lens_flares )
	{
		backend::ref( ).flush_rt_shader_resources( );
		m_lens_flares_effect->apply( 0, 0 );
		if ( pp_parameters.lens_flares_mask_texture )
			backend::ref( ).set_ps_texture(
				"t_lensdirt",
				&*pp_parameters.lens_flares_mask_texture
			);
		float4 lens_flares_parameters(
			pp_parameters.lens_flares_multiplier,
			0.0f,
			0.0f,
			0.0f
		);
		backend::ref( ).set_ps_constant(
			m_lens_flares_parameters,
			lens_flares_parameters
		);
		fill_surface2( m_context->get_rt( rt_lens_flares ) );
	}
	else
	{
		clear_surface( m_context->get_rt( rt_lens_flares ) );
	}

	if ( s_debug_pp_6 )
	{
		bool use_bokeh_dof = false;
		bool use_bokeh_image = false;
		bool use_image_grain = false;
		if ( options::ref( ).current.m_post_process_quality )
		{
			use_bokeh_dof = pp_parameters.use_bokeh_dof;
			if ( use_bokeh_dof )
				use_bokeh_image = pp_parameters.use_bokeh_image;
			use_image_grain = pp_parameters.use_image_grain;
		}

		m_sh_complex_blend
			[use_bokeh_dof]
			[use_bokeh_image]
			[use_image_grain]->apply( 0, 0 );
		m_dof_shader_constants.set(
			pp_parameters.dof_height_lights,
			pp_parameters.dof_focus_distance,
			pp_parameters.dof_focus_region,
			pp_parameters.dof_focus_power,
			pp_parameters.dof_near_blur_amount,
			pp_parameters.dof_far_blur_amount,
			pp_parameters.bokeh_dof_radius,
			pp_parameters.bokeh_dof_density
		);
		m_scene_shader_constants.set(
			m_context,
			pp_parameters.frame_height_lights,
			pp_parameters.frame_mid_tones,
			pp_parameters.frame_shadows,
			pp_parameters.frame_fade_color,
			pp_parameters.frame_fade_amount,
			options::ref( ).current.m_gamma_correction_factor,
			pp_parameters.frame_desaturation,
			image_grain_parameters,
			pp_parameters
		);

		if ( options::ref( ).current.m_post_process_quality &&
			 pp_parameters.use_color_grading_lut &&
			 pp_parameters.color_grading_texture.c_ptr( ) )
		{
			backend::ref( ).set_ps_texture(
				"t_color_grading_lut",
				&*pp_parameters.color_grading_texture
			);
		}
		else
		{
			backend::ref( ).set_ps_texture(
				"t_color_grading_lut",
				&*m_color_grading_base_lut
			);
		}

		backend::ref( ).set_ps_constant( m_sun_direction_parameter, sun_direction );
		backend::ref( ).set_ps_constant(
			m_frame_luminance_parameter,
			frame_luminance_parameter
		);
		fill_surface( m_context->get_rt( rt_generic_1 ), render_target_ptr( ) );
	}

	if ( s_debug_pp_7 )
	{
		if ( options::ref( ).current.m_enabled_fxaa &&
			 !options::ref( ).current.m_use_temporal_antialiasing )
		{
			backend::ref( ).flush_rt_shader_resources( );
			m_post_process_antialiasing_shader_fxaa->apply( 0, 0 );
			backend::ref( ).set_ps_constant(
				m_fxaa_parameters,
				float4(
					options::ref( ).current.m_fxaa_quality_subpix,
					options::ref( ).current.m_fxaa_quality_edge_threshold,
					options::ref( ).current.m_fxaa_quality_edge_threshold_min,
					0.0f
				)
			);
			fill_surface2( m_context->get_rt( rt_generic_0 ) );
		}

		if ( options::ref( ).current.m_enabled_fxaa &&
			 options::ref( ).current.m_enabled_sharpen &&
			 !options::ref( ).current.m_use_temporal_antialiasing )
		{
			backend::ref( ).flush_rt_shader_resources( );
			m_post_process_shader_sharpen->apply( 0, 0 );
			fill_surface2( m_context->get_rt( rt_generic_1 ) );
			m_post_process_shader_sharpen->apply( 1, 0 );
			fill_surface2( m_context->get_rt( rt_present ) );
		}
		else
		{
			m_sh_effect_copy_image->apply( effect_copy_image::copy_rewrite, 0 );
			if ( options::ref( ).current.m_enabled_fxaa &&
				 !options::ref( ).current.m_use_temporal_antialiasing )
			{
				backend::ref( ).set_ps_texture(
					"t_base",
					&*m_context->get_t( rt_generic_0 )
				);
			}
			else
			{
				backend::ref( ).set_ps_texture(
					"t_base",
					&*m_context->get_t( rt_generic_1 )
				);
			}
			backend::ref( ).set_ps_constant(
				m_gamma_correction_factor,
				options::ref( ).current.m_gamma_correction_factor
			);
			fill_surface( m_context->get_rt( rt_present ), render_target_ptr( ) );
		}
	}

	float3 const* const eye_rays = m_context->get_eye_rays( );

	if ( s_debug_pp_8 )
	{
		m_olta_effect->apply( 0, 0 );
		fill_surface2( m_context->get_rt( rt_albedo ) );

		backend::ref( ).flush_rt_shader_resources( );

		m_olta_effect->apply( 1, 0 );
		fill_surface2( m_context->get_rt( rt_present ) );
	}
	// 2 target lines are likely retail-compiled-out source.
	if ( options::ref( ).current.m_use_temporal_antialiasing )
	{

		device::ref( ).d3d_context( )->CopyResource(
			m_context->get_t( rt_albedo )->hw_texture( ),
			m_context->get_t( rt_present )->hw_texture( )
		);

		m_temporal_antialiasing_effect->apply( 0, 0 );
		backend::ref( ).set_ps_constant( m_prev_view_matrix_parameter, transpose( m_prev_view_matrix ) );
		backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ((float4*)eye_rays)[0] );
		backend::ref( ).set_ps_constant( m_c_frame_index, m_context->scene_view( )->get_render_frame_index( ) & 1 );
		fill_surface2( m_context->get_rt( rt_present ) );
		// 2 target lines are likely retail-compiled-out source.
		device::ref( ).d3d_context( )->CopyResource(
			m_context->get_t( rt_previous_present )->hw_texture( ),
			m_context->get_t( rt_albedo )->hw_texture( )
		);
	}

	if ( options::ref( ).current.m_use_motion_blur ||
		 ( options::ref( ).current.m_use_motion_vectors_in_taa && options::ref( ).current.m_use_temporal_antialiasing ) )
	{
		if ( options::ref( ).current.m_use_temporal_antialiasing ||
			 options::ref( ).current.m_post_process_quality )
		{
			backend::ref( ).flush_rt_shader_resources( );
			accumulate_motion_vectors( );
		}
	}

	if ( ( options::ref( ).current.m_use_temporal_antialiasing ||
		   options::ref( ).current.m_post_process_quality ) &&
		 options::ref( ).current.m_use_motion_blur )
	{
		backend::ref( ).flush_rt_shader_resources( );
		// 2 target lines are likely retail-compiled-out source.
		m_motion_blur_effect->apply( 0, 0 );
		backend::ref( ).set_ps_constant( m_prev_view_matrix_parameter, transpose( m_prev_view_matrix ) );
		backend::ref( ).set_ps_constant( m_c_eye_ray_corner, ((float4*)eye_rays)[0] );
		backend::ref( ).set_ps_constant( m_frame_delta_parameter, m_context->get_time_delta( ) );
		backend::ref( ).set_ps_constant( m_motion_blur_scale_parameter, options::ref( ).current.m_motion_blur_scale );
		fill_surface2( m_context->get_rt( rt_albedo ) );

		device::ref( ).d3d_context( )->CopyResource(
			m_context->get_t( rt_present )->hw_texture( ),
			m_context->get_t( rt_albedo )->hw_texture( )
		);
	}
	if ( options::ref( ).current.m_post_process_quality &&
		 pp_parameters.use_aberration )
	{
		m_aberration_effect->apply( 0, 0 );
		backend::ref( ).set_ps_constant(
			m_aberration_parameters,
			float4(
				pp_parameters.aberration_max_variance,
				pp_parameters.aberration_min_variance,
				pp_parameters.aberration_power,
				0.0f
			)
		);
		fill_surface2( m_context->get_rt( rt_albedo ) );

		device::ref( ).d3d_context( )->CopyResource(
			m_context->get_t( rt_present )->hw_texture( ),
			m_context->get_t( rt_albedo )->hw_texture( )
		);
	}

	m_prev_view_matrix = m_context->get_v( );
	// 2 target lines are likely retail-compiled-out source.
	m_context->set_w( float4x4( ).identity( ) );
}

} // namespace render
} // namespace vostok
