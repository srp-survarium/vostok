#include "pch.h"
#include "stage_postprocess.h"

namespace vostok {
namespace render {

namespace {

struct color_grading_pixel {
	byte red;
	byte green;
	byte blue;
	byte alpha;
};

STATIC_SIZE_ASSERT( color_grading_pixel, 0x4 );

} // namespace

res_texture_ptr create_color_grading_base_lut( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x606e50]
	return res_texture_ptr( );
}

bloom_shader_constants::bloom_shader_constants( ) :
	m_bloom_parameters	( 0 ),
	m_bloom_parameters1	( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6073a0]
}

void bloom_shader_constants::set( float, float, float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x606ab0]
}

dof_shader_constants::dof_shader_constants( ) :
	m_dof_parameters			( 0 ),
	m_dof_height_lights		( 0 ),
	m_blurriness_amount		( 0 ),
	m_bokeh_dof_parameters	( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x607230]
}

void dof_shader_constants::set(
	float3 const&,
	float,
	float,
	float,
	float,
	float,
	float,
	float
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6068d0]
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
	// STATE[STUB]
	// FUNCTION BODY[0x606f00]
}

void scene_shader_constants::set(
	renderer_context*,
	float3 const&,
	float3 const&,
	float3 const&,
	float3 const&,
	float,
	float,
	float,
	float4 const&,
	post_process_parameters const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x606bc0]
}

float gaussian( float, float, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x606730]
	return 0.0f;
}

void get_gaussain_weights_offsets(
	float*,
	float*,
	u32,
	float,
	float,
	u32,
	float
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x606760]
}

bool stage_postprocess::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6065e0]
	return false;
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
	// STATE[STUB]
	// FUNCTION BODY[0x60b940]
}

void stage_postprocess::fill_surface( render_target_ptr, render_target_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x607880]
}

void stage_postprocess::fill_surface2( render_target_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x607510]
}

void stage_postprocess::clear_surface( render_target_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x607460]
}

void stage_postprocess::measure_per_pixel_luminance( res_texture*, float4& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x608b90]
}

void stage_postprocess::compute_per_pixel_eye_adaptated_luminance( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6088c0]
}

float4 stage_postprocess::compute_luminance_parameters( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x609bd0]
	return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}

void stage_postprocess::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x608770]
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
}

void stage_postprocess::advanced_bloom( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x608e00]
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
}

void stage_postprocess::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x609c50]
}

} // namespace render
} // namespace vostok
