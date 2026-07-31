#include "pch.h"
#include "stage_postprocess.h"

#include <vostok/render/core/backend.h>
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
#include "renderer_context_targets.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include "statistics.h"
#include <vostok/render/core/options.h>
#include "effect_post_process_mlaa.h"
#include "effect_post_process_fxaa.h"

namespace vostok {
namespace render {

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

bloom_shader_constants::bloom_shader_constants( ) :
	m_bloom_parameters	( 0 ),
	m_bloom_parameters1	( 0 )
{
	// FUNCTION BODY[0x6073a0]
	// claude@NOTE: m_bloom_parameters1 registration has no legacy ancestor - matcher-phase
	m_bloom_parameters = backend::ref().register_constant_host( "bloom_parameters", rc_float );
}

void bloom_shader_constants::set( float bloom_scale, float bloom_max_color, float3 const& )
{
	// FUNCTION BODY[0x606ab0]
	// claude@NOTE: halo_color parameter is post-legacy (feeds m_bloom_parameters1) - matcher-phase
	float4 bloom_parameters(bloom_scale, bloom_max_color, 0.0f, 0.0f);

	backend::ref().set_ps_constant(m_bloom_parameters, bloom_parameters);
}

dof_shader_constants::dof_shader_constants( ) :
	m_dof_parameters			( 0 ),
	m_dof_height_lights		( 0 ),
	m_blurriness_amount		( 0 ),
	m_bokeh_dof_parameters	( 0 )
{
	// FUNCTION BODY[0x607230]
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
	// FUNCTION BODY[0x6068d0]
	backend::ref().set_ps_constant(m_dof_height_lights, float4(blurriness_height_lights, 0));
	backend::ref().set_ps_constant(m_dof_parameters, float4(distance, region, power, 0));
	backend::ref().set_ps_constant(m_blurriness_amount, float4(near_blur_amout, far_blur_amout, 0, 0));
	backend::ref().set_ps_constant(m_bokeh_dof_parameters, float4(bokeh_dof_radius, bokeh_dof_density, 0, 0));
}

scene_shader_constants::scene_shader_constants( ) :
	m_frame_height_lights_and_desaturation_parameters	( 0 ),
	m_scene_mid_tones_parameters						( 0 ),
	m_scene_shadows_parameters						( 0 ),
	m_gamma_correction_factor							( 0 ),
	m_scene_fade_parameters							( 0 ),
	m_filmic_tonemap_packed_parameters_0				( 0 ),
	m_filmic_tonemap_packed_parameters_1				( 0 ),
	m_c_eye_ray_corner								( 0 ),
	m_image_grain_parameters							( 0 )
{
	// FUNCTION BODY[0x606f00]
	// claude@NOTE: filmic-tonemap/eye-ray/image-grain registrations have no legacy ancestor
	m_frame_height_lights_and_desaturation_parameters	= backend::ref().register_constant_host( "frame_height_lights_and_desaturation", rc_float );
	m_scene_mid_tones_parameters						= backend::ref().register_constant_host( "frame_mid_tones", rc_float );
	m_scene_shadows_parameters							= backend::ref().register_constant_host( "frame_shadows", rc_float );
	m_gamma_correction_factor							= backend::ref().register_constant_host( "gamma_correction_factor", rc_float );
	m_scene_fade_parameters								= backend::ref().register_constant_host( "frame_fade", rc_float );
}

void scene_shader_constants::set(
	renderer_context*,
	float3 const&	height_lights,
	float3 const&	mid_tones,
	float3 const&	shadows,
	float3 const&	fade_color,
	float			fade_amount,
	float			gamma_correction_factor,
	float			desaturation,
	float4 const&,
	post_process_parameters const&
)
{
	// FUNCTION BODY[0x606bc0]
	// claude@NOTE: context/image-grain/parameters arguments are post-legacy (filmic tonemap,
	// eye-ray, grain constants) - legacy slice ported, remainder matcher-phase
	float4 frame_height_lights_and_desaturation(height_lights, desaturation);
	float4 scene_fade(fade_color, fade_amount);

	backend::ref().set_ps_constant(m_frame_height_lights_and_desaturation_parameters, frame_height_lights_and_desaturation);
	backend::ref().set_ps_constant(m_scene_mid_tones_parameters, mid_tones);
	backend::ref().set_ps_constant(m_scene_shadows_parameters, shadows);
	backend::ref().set_ps_constant(m_gamma_correction_factor, gamma_correction_factor);
	backend::ref().set_ps_constant(m_scene_fade_parameters, scene_fade);
}

float gaussian( float x, float /*mean*/, float std_deviation )
{
	// FUNCTION BODY[0x606730]
	return ( 1.0f / vostok::math::sqrt( 2.0f * vostok::math::pi * std_deviation * std_deviation ) )
		* vostok::math::exp( (-x*x)/(2.0f * std_deviation * std_deviation) );
}

void get_gaussain_weights_offsets(
	float*	out_weights,
	float*	out_offsets,
	u32		buffer_size,
	float	blur_scale,
	float	blur_intencity,
	u32		num_samples,
	float
)
{
	// FUNCTION BODY[0x606760]
	// claude@NOTE: trailing float parameter is post-legacy - unused in the ported body
	for (u32 i=0; i<num_samples; i++)
	{
		out_offsets[i] = (static_cast<float>(i)-4.0f)*(1.0f/static_cast<float>(buffer_size));
		float x = (static_cast<float>(i) - 4.0f) / 4.0f;
		out_weights[i] = blur_intencity * gaussian( x, 0.0f, math::max(blur_scale,0.25f));
	}
}

bool stage_postprocess::is_effects_ready( ) const
{
	// FUNCTION BODY[0x6065e0]
	// claude@NOTE: legacy singular m_sh_blur / m_sh_complex_blend[3] checks remapped onto the
	// canonical [8] / [2][2][2] array shapes; new sraa/sharpen/motion effects unchecked yet
	return
		   m_sh_gather_bloom.c_ptr() != NULL
		&& m_sh_gather_luminance.c_ptr() != NULL
		&& m_sh_gather_luminance_histogram.c_ptr() != NULL
		&& m_sh_eye_adaptation.c_ptr() != NULL
		&& m_sh_blur[0].c_ptr() != NULL
		&& m_sh_complex_blend[0][0][0].c_ptr() != NULL
		&& m_sh_complex_blend[1][0][0].c_ptr() != NULL
		&& m_sh_complex_blend[1][1][0].c_ptr() != NULL
		&& m_sh_effect_copy_image.c_ptr() != NULL
		&& m_post_process_antialiasing_shader.c_ptr() != NULL
		&& m_post_process_antialiasing_shader_fxaa.c_ptr() != NULL
		&& m_god_rays_effect.c_ptr() != NULL;
}

stage_postprocess::stage_postprocess(
	renderer* in_renderer,
	renderer_context* context
) :
	stage										( in_renderer, context ),
	m_kernel_offsets							( 0 ),
	m_blur_offsets_weights						( 0 ),
	m_luminance_range_parameter_parameter			( 0 ),
	m_elapsed_time_parameter						( 0 ),
	m_adaptation_factor							( 0 ),
	m_gamma_correction_factor						( 0 ),
	m_prev_view_matrix_parameter					( 0 ),
	m_prev_world_view_matrix_parameter				( 0 ),
	m_inverse_world_matrix_parameter				( 0 ),
	m_god_rays_parameters0						( 0 ),
	m_god_rays_parameters1						( 0 ),
	m_god_rays_parameters2						( 0 ),
	m_c_eye_ray_corner							( 0 ),
	m_c_frame_index								( 0 ),
	m_frame_delta_parameter						( 0 ),
	m_motion_blur_scale_parameter					( 0 ),
	m_aberration_parameters						( 0 ),
	m_blur_target_size_parameter					( 0 ),
	m_lens_flares_parameters						( 0 ),
	m_sun_direction_parameter						( 0 ),
	m_frame_luminance_parameter					( 0 ),
	m_fxaa_parameters							( 0 ),
	m_image_grain_random_offsets					( 0.0f, 0.0f )
{
	// FUNCTION BODY[0x60b940]
	// claude@NOTE: legacy slice - effect_blur seeded only into m_sh_blur[0] (legacy had ONE blur
	// effect; the shipped 8-kernel family and the third complex-blend axis, sraa/sharpen/god-rays
	// parameters, motion/olta/temporal/aberration effects have no legacy ancestor - matcher-phase);
	// legacy effect_gather_sun_light_scattering_zone maps to m_god_rays_effect (rename)
	effect_manager::ref().create_effect<effect_gather_bloom>(&m_sh_gather_bloom);
	effect_manager::ref().create_effect<effect_gather_luminance>(&m_sh_gather_luminance);
	effect_manager::ref().create_effect<effect_gather_luminance_histogram>(&m_sh_gather_luminance_histogram);
	effect_manager::ref().create_effect<effect_eye_adaptation>(&m_sh_eye_adaptation);
	effect_manager::ref().create_effect<effect_blur_3>(&m_sh_blur[0]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<false, false, false> >(&m_sh_complex_blend[0][0][0]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<true, false, false> >(&m_sh_complex_blend[1][0][0]);
	effect_manager::ref().create_effect< effect_complex_post_process_blend<true, true, false> >(&m_sh_complex_blend[1][1][0]);
	effect_manager::ref().create_effect<effect_copy_image>(&m_sh_effect_copy_image);
	effect_manager::ref().create_effect<effect_post_process_mlaa>(&m_post_process_antialiasing_shader);
	effect_manager::ref().create_effect<effect_post_process_fxaa>(&m_post_process_antialiasing_shader_fxaa);

	effect_manager::ref().create_effect<effect_god_rays>(&m_god_rays_effect);


	m_blur_offsets_weights	= backend::ref().register_constant_host("offsets_weights", rc_float);
	m_kernel_offsets		= backend::ref().register_constant_host("kernel_offsets", rc_float);

	m_elapsed_time_parameter= backend::ref().register_constant_host("elapsed_time", rc_float);
	m_adaptation_factor		= backend::ref().register_constant_host("adaptation_factor", rc_float);

	m_sun_direction_parameter = backend::ref().register_constant_host("sun_direction_parameter", rc_float);

	m_frame_luminance_parameter = backend::ref().register_constant_host("frame_luminance_parameter", rc_float);

	m_luminance_range_parameter_parameter			 = backend::ref().register_constant_host( "luminance_range_parameter", rc_float );

	m_gamma_correction_factor							= backend::ref().register_constant_host( "gamma_correction_factor", rc_float );

	m_color_grading_base_lut = create_color_grading_base_lut(16);
	//resource_manager::ref().create_texture("resources/textures/color_grading/color_grading_base_lut");

	//m_material_post_effects.push_back(material_effects());

	//vostok::resources::query_resource(
 	//	"default_post_process_and_environment",
 	//	resources::material_class,
 	//	boost::bind(&stage_postprocess::on_material_loaded, this, _1),
 	//	render::g_allocator
 	//);
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


}

void stage_postprocess::fill_surface( render_target_ptr surf, render_target_ptr surf1 )
{
	// FUNCTION BODY[0x607880]
	// claude@NOTE: second target parameter is post-legacy; bound into RT slot 1 when present
	float t_w = float(surf->width());
	float t_h = float(surf->height());

	backend::ref().set_render_targets( &*surf, surf1.c_ptr() ? &*surf1 : 0, 0, 0);
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
	// FUNCTION BODY[0x607510]
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

	backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 2*3, 0, offset);
}

void stage_postprocess::clear_surface( render_target_ptr surf )
{
	// FUNCTION BODY[0x607460]
	backend::ref().set_render_targets	( &*surf, 0, 0, 0);
	backend::ref().clear_render_targets	(0.0f, 0.0f, 0.0f, 0.f);
}

void stage_postprocess::measure_per_pixel_luminance( res_texture* scene_texture, float4& out_avrg_min_max )
{
	// FUNCTION BODY[0x608b90]
	VOSTOK_UNREFERENCED_PARAMETER(out_avrg_min_max);

	s32 const last_target_index = s32(rt_num_frame_luminance_targets) - 1;

	m_sh_gather_luminance->apply( effect_gather_luminance::gather_log_luminance, 0 );
	backend::ref().set_ps_texture( "t_frame_color0", scene_texture);
	fill_surface(m_context->m_targets->m_family[rt_frame_luminance0 + last_target_index].target, render_target_ptr( ));

	for (s32 lum_rt_index=last_target_index-1; lum_rt_index>=0; lum_rt_index--)
	{
		if (lum_rt_index==0)
			m_sh_gather_luminance->apply( effect_gather_luminance::gather_exp_luminance, 0 );
		else
			m_sh_gather_luminance->apply( effect_gather_luminance::gather_luminance, 0 );

		backend::ref().set_ps_texture( "t_frame_color1", &*m_context->m_targets->m_family[rt_frame_luminance0 + lum_rt_index+1].texture);
		fill_surface(m_context->m_targets->m_family[rt_frame_luminance0 + lum_rt_index].target, render_target_ptr( ));
	}

	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();
//	if not using eye adaptation
	if (math::is_similar(pp_parameters.adaptation_speed, 0.0f, 0.05f))
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

void stage_postprocess::compute_per_pixel_eye_adaptated_luminance( )
{
	// FUNCTION BODY[0x6088c0]
	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();//m_material_post_effects[0].m_post_process_stage_parameters;

	m_sh_eye_adaptation->apply( 0, 0 );

	float time_delta					= m_context->get_time_delta();
	math::clamp							(time_delta, 0.0f, 1.0f / 30.0f);

	backend::ref().set_ps_constant( m_elapsed_time_parameter, time_delta );
	//backend::ref().set_ps_constant( m_adaptation_factor, (1.0f - math::pow(0.98f, pp_parameters.adaptation_rate * m_context->get_time_delta())) );
	backend::ref().set_ps_constant( m_adaptation_factor, time_delta * (1.0f / pp_parameters.adaptation_speed));

	static bool fist_pass = true;
	if (fist_pass)
	{
		backend::ref().set_ps_texture( "t_previous_luminance", &*m_context->m_targets->m_family[rt_frame_luminance0 + 0].texture );
		fist_pass = false;
	}
	else
	{
		backend::ref().set_ps_texture( "t_previous_luminance", &*m_context->m_targets->m_family[rt_frame_luminance_previous].texture );
	}

	backend::ref().set_ps_texture( "t_current_luminanace", &*m_context->m_targets->m_family[rt_frame_luminance0 + 0].texture );
	fill_surface2(m_context->m_targets->m_family[rt_frame_luminance_current].target);

	resource_manager::ref().copy2D(
		&*m_context->m_targets->m_family[rt_frame_luminance_previous].texture,
		0,
		0,
		&*m_context->m_targets->m_family[rt_frame_luminance_current].texture,
		0,
		0,
		m_context->m_targets->m_family[rt_frame_luminance_current].texture->width(),
		m_context->m_targets->m_family[rt_frame_luminance_current].texture->height(),
		0,
		0
	);
}

float4 stage_postprocess::compute_luminance_parameters( u32 frame_delta )
{
	// FUNCTION BODY[0x609bd0]
	// claude@NOTE: get_frame_luminance_parameters grew a bool& validity out-param (still a header
	// stub) - legacy no-arg calls adapted
	post_process_parameters const& pp_parameters = m_context->scene_view()->post_process_parameters();

	float4								frame_luminance_parameter;
	bool								frame_luminance_valid;

	if (m_context->scene_view()->get_render_frame_index() % frame_delta == 0)
	{
		frame_luminance_parameter		= get_frame_luminance_parameters(frame_luminance_valid);
		m_context->get_scene_view()->set_luminance_parameters(frame_luminance_parameter);

		if (m_context->scene_view()->get_render_frame_index() == frame_delta)
			m_context->get_scene_view()->set_prev_luminance_parameters(frame_luminance_parameter);
	}

	float time_delta					= m_context->get_time_delta();
	math::clamp							(time_delta, 1.0f / 300.0f, 1.0f / 30.0f);

	m_context->get_scene_view()->set_prev_luminance_parameters(
			m_context->scene_view()->get_prev_luminance_parameters()
		+
			(
				m_context->get_scene_view()->get_luminance_parameters() - m_context->scene_view()->get_prev_luminance_parameters()
			) * time_delta / pp_parameters.adaptation_speed
		);

	frame_luminance_parameter			= m_context->scene_view()->get_prev_luminance_parameters();

	for (u32 lum_rt_index=0; lum_rt_index<rt_num_frame_luminance_targets; lum_rt_index++)
	{
		backend::ref().set_render_targets	(&*m_context->m_targets->m_family[rt_frame_luminance0 + lum_rt_index].target, 0, 0, 0);
		backend::ref().clear_render_targets	(0.0f, 0.0f, 0.0f, 1.0f);
	}

	float4 frame_luminance_parameter0;
	measure_per_pixel_luminance			(&*m_context->m_targets->m_family[rt_generic_0].texture, frame_luminance_parameter0);

	if (!math::is_similar(pp_parameters.adaptation_speed, 0.0f, 0.05f))
		compute_per_pixel_eye_adaptated_luminance();

	return frame_luminance_parameter;
}

void stage_postprocess::execute_disabled( )
{
	// FUNCTION BODY[0x608770]
	// claude@NOTE: legacy PIX_EVENT( stage_postprocess) dropped - zero PIX symbols in target

	if (!is_effects_ready())
		return;

	m_sh_effect_copy_image->apply	(effect_copy_image::copy_rewrite, 0);
	backend::ref().set_ps_texture	( "t_base", &*m_context->m_targets->m_family[rt_generic_0].texture);
	backend::ref().set_ps_constant	(m_gamma_correction_factor, 1.0f);
	fill_surface					(m_context->m_targets->m_family[rt_present].target, render_target_ptr( ));
	m_context->set_w				(float4x4().identity());
}

void stage_postprocess::process_blur(
	render_target*,
	res_texture*,
	render_target*,
	res_texture*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6082c0]
	// claude@NOTE: no legacy ancestor - the blur ping-pong was inlined inside the legacy execute;
	// the shipped kernel-indexed split is matcher-phase
}

void stage_postprocess::advanced_bloom( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x608e00]
	// claude@NOTE: no legacy ancestor - advanced bloom postdates the legacy corpus
}

bool remove_model_skeletal_filter_predicate::operator()( render_surface_instance* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7fbf0]
	return false;
}

void stage_postprocess::accumulate_motion_vectors( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x607b00]
	// claude@NOTE: no legacy ancestor - motion-vector accumulation postdates the legacy corpus
}

void stage_postprocess::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x609c50]
	// claude@NOTE: legacy execute blocked - shipped pipeline adds motion blur/sraa/olta/temporal
	// AA/lens flares/aberration around the reshaped m_sh_blur[8] ping-pong; legacy ~250-line body
	// kept in temp/render_legacy/engine/sources/stage_postprocess.cpp as the remainder
}

} // namespace render
} // namespace vostok
