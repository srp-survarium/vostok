#include "pch.h"
#include "renderer_context_targets.h"

#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

u32 renderer_context_targets::s_new_id = 0;

pcstr rt_index_to_name( enum_render_target_index index )
{
	switch ( index )
	{
	case rt_gbuffer_position_downsampled:		return "$user$gbuffer_position_downsampled";
	case rt_final_frame_downsampled_temp:		return "$user$final_frame_downsampledtemp";
	case rt_final_frame_downsampled:				return "$user$final_frame_downsampled";
	case rt_indirect_lighting_specular:			return "$user$indirect_lighting_specular";
	case rt_light_scattering_mask:				return "$user$light_scattering_mask";
	case rt_light_scattering_result:				return "$user$light_scattering_result";
	case rt_local_reflection_result:				return "$user$local_reflection_result";
	case rt_local_reflection_result_params:		return "$user$local_reflection_result_params";
	case rt_sun_translucensy_help_data:			return "$user$sun_translucensy_help_data";
	case rt_position:							return "$user$position";
	case rt_normal:								return "$user$normal";
	case rt_normal_copy:							return "$user$normal_copy";
	case rt_albedo:								return "$user$albedo";
	case rt_one_layer_transparency_alpha:		return "$user$one_layer_transparency_alpha";
	case rt_distortion:							return "$user$distortion";
	case rt_distortion_mask:						return "$user$distortion_mask";
	case rt_object_motion_vectors:				return "$user$object_motion_vectors";
	case rt_ssao_accumulator:					return "$user$ssao_accumulator";
	case rt_ssao_accumulator_full_x:				return "$user$ssao_accumulator_full_x";
	case rt_ssao_temporal_mask:					return "$user$ssao_temporal_mask";
	case rt_ssao_prev_accumulator_full_x:		return "$user$ssao_prev_accumulator_full_x";
	case rt_ssao_accumulator_z:					return "$user$ssao_accumulator_z";
	case rt_ssao_prev_accumulator_z:				return "$user$ssao_prev_accumulator_z";
	case rt_decals_diffuse:						return "$user$decals_diffuse";
	case rt_decals_normal:						return "$user$decals_normal";
	case rt_decals_smoothness:					return "$user$decals_smoothness";
	case rt_accumulator_diffuse:					return "$user$accum_diffuse";
	case rt_decals_blend_result:					return "$user$decals_blend_result";
	case rt_accumulator_specular:				return "$user$accum_specular";
	case rt_lpv_accumulation:					return "$user$lpv_accumulation";
	case rt_blur_0:								return "$user$blur0";
	case rt_blur_1:								return "$user$blur1";
	case rt_blur_2:								return "$user$blur2";
	case rt_blur_3:								return "$user$blur3";
	case rt_blur_4:								return "$user$blur4";
	case rt_blur_4_0:							return "$user$blur40";
	case rt_blur_5:								return "$user$blur5";
	case rt_blur_5_0:							return "$user$blur50";
	case rt_blur_6:								return "$user$blur6";
	case rt_blur_6_0:							return "$user$blur60";
	case rt_blur_7:								return "$user$blur7";
	case rt_blur_7_0:							return "$user$blur70";
	case rt_blur_8:								return "$user$blur8";
	case rt_blur_8_0:							return "$user$blur80";
	case rt_lens_flares:						return "$user$lens_flares";
	case rt_present:							return "$user$present";
	case rt_previous_present:					return "$user$previous_present";
	case rt_generic_0:							return "$user$generic0";
	case rt_generic_1:							return "$user$generic1";
	case rt_particle_result:					return "$user$particle_result";
	case rt_particle_lighting:					return "$user$particle_lighting";
	case rt_rain_result:							return "$user$rain_result";
	case rt_frame_luminance_previous:			return "$user$frame_luminance_previous";
	case rt_frame_luminance_current:				return "$user$frame_luminance";
	case rt_frame_luminance_histogram:			return "$user$frame_luminance_histogram";
	case rt_apply_indirect_lighting_ds:			return "$user$apply_indirect_lighting_ds";
	case rt_frame_luminance0:					return "$user$frame_luminance0";
	case rt_frame_luminance1:					return "$user$frame_luminance1";
	case rt_frame_luminance2:					return "$user$frame_luminance2";
	case rt_frame_luminance3:					return "$user$frame_luminance3";
	case rt_frame_luminance4:					return "$user$frame_luminance4";
	case rt_frame_luminance5:					return "$user$frame_luminance5";
	case rt_frame_luminance6:					return "$user$frame_luminance6";
	case rt_frame_luminance7:					return "$user$frame_luminance7";
	case rt_frame_luminance8:					return "$user$frame_luminance8";
	case rt_mie_scattering:						return "$user$mie_scattering";
	case rt_rayleigh_scattering:				return "$user$rayleigh_scattering";
	case rt_frame_lum_scene_downsampled:		return "$user$frame_luminance_scene_color_downsampled";
	case rt_result_frame_luminance_histogram:	return "$user$result_frame_luminance_histogram";
	case rt_frame_luminance_lockable:			return "$user$frame_luminance_lockable";
	default:									return 0;
	}
}

renderer_context_targets::renderer_context_targets( math::uint2 size ) :
	m_size			( 0, 0 ),
	m_memory_usage	( 0 )
{
	create_targets( size, true );
}

renderer_context_targets::~renderer_context_targets( )
{
}

static u32 get_format_block_size( DXGI_FORMAT format )
{
	switch ( format )
	{
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R16_FLOAT:				return 2;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R24G8_TYPELESS:		return 4;
	case DXGI_FORMAT_R8_UNORM:				return 1;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:	return 8;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:	return 16;
	default:								NODEFAULT( return 4 );
	}
}

void renderer_context_targets::new_rt(
	enum_render_target_index	index,
	DXGI_FORMAT					in_format,
	math::uint2 const			in_size,
	enum_rt_usage				usage,
	bool						enabled
)
{
	if ( !enabled )
		return;

	render_target_instance& instance = m_family[index];
	pcstr const original_name = rt_index_to_name( index );
	if ( instance.orig_name.get_buffer( ) != original_name )
		instance.orig_name = original_name;

	instance.name.assignf( "%s_%d", instance.orig_name.get_buffer(), m_id );
	instance.target = resource_manager::ref().create_render_target(
		instance.name.get_buffer(),
		in_size.x,
		in_size.y,
		in_format,
		usage,
		res_texture_ptr( ),
		0,
		D3D11_USAGE_DEFAULT,
		1,
		0
	);
	instance.texture = instance.target->get_texture( );
	m_memory_usage += get_format_block_size( in_format ) * in_size.x * in_size.y;
}

void renderer_context_targets::new_lt(
	enum_render_target_index	index,
	DXGI_FORMAT					in_format,
	math::uint2 const			in_size
)
{
	render_target_instance& instance = m_family[index];
	pcstr const original_name = rt_index_to_name( index );
	if ( instance.orig_name.get_buffer( ) != original_name )
		instance.orig_name = original_name;

	instance.name.assignf( "%s_%d", instance.orig_name.get_buffer(), m_id );
	instance.target = 0;
	instance.texture = resource_manager::ref().create_texture2d(
		instance.name.get_buffer(),
		in_size.x,
		in_size.y,
		0,
		in_format,
		D3D_USAGE_STAGING,
		0,
		1,
		false
	);
	m_memory_usage += get_format_block_size( in_format ) * in_size.x * in_size.y;
}

void renderer_context_targets::create_targets( math::uint2 size, bool force_resize )
{
	if ( !force_resize && m_size == size )
		return;

	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		m_family[i].target = 0;
		m_family[i].texture = 0;
	}

	m_size = size;
	m_id = s_new_id++;
	m_memory_usage = 0;

	math::uint2 const size_blur_1x( math::max( size.x >> 2, 1u ), math::max( size.y >> 2, 1u ) );
	math::uint2 const size_blur_2x( math::max( size.x >> 3, 1u ), math::max( size.y >> 3, 1u ) );
	math::uint2 const size_blur_4x( math::max( size.x >> 4, 1u ), math::max( size.y >> 4, 1u ) );
	math::uint2 const size_blur_8x( math::max( size.x >> 5, 1u ), math::max( size.y >> 5, 1u ) );
	math::uint2 const size_blur_16x( math::max( size.x >> 6, 1u ), math::max( size.y >> 6, 1u ) );
	math::uint2 const size_blur_32x( math::max( size.x >> 7, 1u ), math::max( size.y >> 7, 1u ) );
	math::uint2 const size_d2( math::max( size.x >> 1, 1u ), math::max( size.y >> 1, 1u ) );
	math::uint2 const size_d4( math::max( size.x >> 2, 1u ), math::max( size.y >> 2, 1u ) );
	math::uint2 const size_d8( math::max( size.x >> 3, 1u ), math::max( size.y >> 3, 1u ) );
	math::uint2 const size_d16( math::max( size.x >> 4, 1u ), math::max( size.y >> 4, 1u ) );

	new_rt( rt_generic_0, DXGI_FORMAT_R11G11B10_FLOAT, size, enum_rt_usage_render_target, true );
	new_rt( rt_generic_1, DXGI_FORMAT_R11G11B10_FLOAT, size, enum_rt_usage_render_target, true );
	// 7 target lines are likely retail-compiled-out source.
	new_rt( rt_present, DXGI_FORMAT_R8G8B8A8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_previous_present, DXGI_FORMAT_R8G8B8A8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_accumulator_diffuse, DXGI_FORMAT_R10G10B10A2_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_accumulator_specular, DXGI_FORMAT_R10G10B10A2_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_albedo, DXGI_FORMAT_R8G8B8A8_UNORM, size, enum_rt_usage_render_target, true );
	new_ds( rt_apply_indirect_lighting_ds, DXGI_FORMAT_R24G8_TYPELESS, size );
	new_rt( rt_sun_translucensy_help_data, DXGI_FORMAT_R16G16_FLOAT, size, enum_rt_usage_render_target, true );
	new_rt( rt_normal, DXGI_FORMAT_R10G10B10A2_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_decals_blend_result, DXGI_FORMAT_R10G10B10A2_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_normal_copy, DXGI_FORMAT_R10G10B10A2_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_distortion, DXGI_FORMAT_R16G16_FLOAT, size, enum_rt_usage_render_target, true );
	new_rt( rt_distortion_mask, DXGI_FORMAT_R8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_object_motion_vectors, DXGI_FORMAT_R16G16_FLOAT, size, enum_rt_usage_render_target, true );
	new_rt( rt_position, DXGI_FORMAT_R16_FLOAT, size, enum_rt_usage_render_target, true );
	new_rt( rt_ssao_accumulator_full_x, DXGI_FORMAT_R8G8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_one_layer_transparency_alpha, DXGI_FORMAT_R8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_decals_diffuse, DXGI_FORMAT_R8G8B8A8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_decals_normal, DXGI_FORMAT_R8G8B8A8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_decals_smoothness, DXGI_FORMAT_R8G8_UNORM, size, enum_rt_usage_render_target, true );
	new_rt( rt_ssao_prev_accumulator_full_x, DXGI_FORMAT_R8G8_UNORM, size, enum_rt_usage_render_target, options::ref( ).current.m_ssao_use_temporal_filtering );
	new_rt( rt_ssao_accumulator_z, DXGI_FORMAT_R16_FLOAT, size, enum_rt_usage_render_target, options::ref( ).current.m_ssao_use_temporal_filtering );
	new_rt( rt_ssao_prev_accumulator_z, DXGI_FORMAT_R16_FLOAT, size, enum_rt_usage_render_target, options::ref( ).current.m_ssao_use_temporal_filtering );

	new_rt( rt_ssao_temporal_mask, DXGI_FORMAT_R8G8_UNORM, size_d2, enum_rt_usage_render_target, options::ref( ).current.m_ssao_use_temporal_filtering );
	new_rt( rt_lpv_accumulation, DXGI_FORMAT_R11G11B10_FLOAT, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_indirect_lighting_specular, DXGI_FORMAT_R11G11B10_FLOAT, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_ssao_accumulator, DXGI_FORMAT_R16G16_FLOAT, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_gbuffer_position_downsampled, DXGI_FORMAT_R16_FLOAT, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_light_scattering_mask, DXGI_FORMAT_R8_UNORM, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_light_scattering_result, DXGI_FORMAT_R10G10B10A2_UNORM, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_rain_result, DXGI_FORMAT_R8G8B8A8_UNORM, size_d2, enum_rt_usage_render_target, true );
	new_rt( rt_particle_result, DXGI_FORMAT_R16G16B16A16_FLOAT, size_d4, enum_rt_usage_render_target, true );
	new_rt( rt_particle_lighting, DXGI_FORMAT_R16G16B16A16_FLOAT, size_d4, enum_rt_usage_render_target, true );
	new_rt( rt_frame_lum_scene_downsampled, DXGI_FORMAT_R16G16B16A16_FLOAT, size_d4, enum_rt_usage_render_target, true );
	new_rt( rt_local_reflection_result, DXGI_FORMAT_R8G8B8A8_UNORM, size_d4, enum_rt_usage_render_target, true );
	new_rt( rt_local_reflection_result_params, DXGI_FORMAT_R16G16B16A16_FLOAT, size_d4, enum_rt_usage_render_target, true );

	new_rt( rt_final_frame_downsampled, DXGI_FORMAT_R16G16B16A16_FLOAT, size_d16, enum_rt_usage_render_target, true );
	new_rt( rt_final_frame_downsampled_temp, DXGI_FORMAT_R16G16B16A16_FLOAT, size_d16, enum_rt_usage_render_target, true );
	new_rt( rt_lens_flares, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_1x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_0, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_1x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_1, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_1x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_2, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_1x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_3, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_1x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_4, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_2x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_4_0, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_2x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_5, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_4x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_5_0, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_4x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_6, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_8x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_6_0, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_8x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_7, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_16x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_7_0, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_16x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_8, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_32x, enum_rt_usage_render_target, true );
	new_rt( rt_blur_8_0, DXGI_FORMAT_R16G16B16A16_FLOAT, size_blur_32x, enum_rt_usage_render_target, true );

	new_rt( rt_frame_luminance0, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 1, 1 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance1, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 2, 2 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance2, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 4, 4 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance3, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 8, 8 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance4, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 16, 16 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance5, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 32, 32 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance6, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 64, 64 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance7, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 128, 128 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance8, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 256, 256 ), enum_rt_usage_render_target, true );

	new_rt( rt_frame_luminance_current, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 1, 1 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance_previous, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 1, 1 ), enum_rt_usage_render_target, true );
	new_rt( rt_frame_luminance_histogram, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 16, 1 ), enum_rt_usage_render_target, true );
	new_lt( rt_result_frame_luminance_histogram, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 16, 1 ) );
	new_lt( rt_frame_luminance_lockable, DXGI_FORMAT_R32G32B32A32_FLOAT, math::uint2( 16, 1 ) );
	new_rt( rt_mie_scattering, DXGI_FORMAT_R16G16B16A16_FLOAT, math::uint2( 256, 128 ), enum_rt_usage_render_target, true );
	new_rt( rt_rayleigh_scattering, DXGI_FORMAT_R16G16B16A16_FLOAT, math::uint2( 256, 128 ), enum_rt_usage_render_target, true );

	LOG_INFO( "render targets memory usage: %d", m_memory_usage );

	backend::ref().set_render_targets( &*m_family[rt_frame_luminance_previous].target, 0, 0, 0 );
	backend::ref().clear_render_targets( .25f, .25f, .25f, .25f );
	backend::ref().set_render_targets( &*m_family[rt_frame_luminance_current].target, 0, 0, 0 );
	backend::ref().clear_render_targets( .25f, .25f, .25f, .25f );
	backend::ref().set_render_targets( &*m_family[rt_ssao_prev_accumulator_full_x].target, 0, 0, 0 );
	backend::ref().clear_render_targets( 0.f, 0.f, 0.f, 0.f );
	backend::ref().set_render_targets( &*m_family[rt_ssao_prev_accumulator_z].target, 0, 0, 0 );
	backend::ref().clear_render_targets( 0.f, 0.f, 0.f, 0.f );
}

void renderer_context_targets::resize( math::uint2 size, bool force_resize )
{
	create_targets( size, force_resize );
}

} // namespace render
} // namespace vostok
