#ifndef VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_TARGETS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_TARGETS_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_uint2.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/res_texture_list.h>

#include "render_target.h"
#include "res_texture.h"

namespace vostok {
namespace render {

enum enum_render_target_index {
	rt_gbuffer_position_downsampled			= 0x00,
	rt_final_frame_downsampled_temp			= 0x01,
	rt_final_frame_downsampled				= 0x02,
	rt_indirect_lighting_specular			= 0x03,
	rt_light_scattering_mask				= 0x04,
	rt_light_scattering_result				= 0x05,
	rt_local_reflection_result				= 0x06,
	rt_local_reflection_result_params		= 0x07,
	rt_sun_translucensy_help_data			= 0x08,
	rt_position								= 0x09,
	rt_normal								= 0x0a,
	rt_normal_copy							= 0x0b,
	rt_albedo								= 0x0c,
	rt_one_layer_transparency_alpha			= 0x0d,
	rt_distortion							= 0x0e,
	rt_distortion_mask						= 0x0f,
	rt_object_motion_vectors				= 0x10,
	rt_ssao_accumulator						= 0x11,
	rt_ssao_accumulator_full_x				= 0x12,
	rt_ssao_temporal_mask					= 0x13,
	rt_ssao_prev_accumulator_full_x			= 0x14,
	rt_ssao_accumulator_z					= 0x15,
	rt_ssao_prev_accumulator_z				= 0x16,
	rt_decals_diffuse						= 0x17,
	rt_decals_normal							= 0x18,
	rt_decals_smoothness					= 0x19,
	rt_accumulator_diffuse					= 0x1a,
	rt_decals_blend_result					= 0x1b,
	rt_accumulator_specular					= 0x1c,
	rt_lpv_accumulation						= 0x1d,
	rt_blur_0								= 0x1e,
	rt_blur_1								= 0x1f,
	rt_blur_2								= 0x20,
	rt_blur_3								= 0x21,
	rt_blur_4								= 0x22,
	rt_blur_4_0								= 0x23,
	rt_blur_5								= 0x24,
	rt_blur_5_0								= 0x25,
	rt_blur_6								= 0x26,
	rt_blur_6_0								= 0x27,
	rt_blur_7								= 0x28,
	rt_blur_7_0								= 0x29,
	rt_blur_8								= 0x2a,
	rt_blur_8_0								= 0x2b,
	rt_lens_flares							= 0x2c,
	rt_present								= 0x2d,
	rt_previous_present						= 0x2e,
	rt_generic_0							= 0x2f,
	rt_generic_1							= 0x30,
	rt_particle_result						= 0x31,
	rt_particle_lighting					= 0x32,
	rt_rain_result							= 0x33,
	rt_frame_luminance_previous				= 0x34,
	rt_frame_luminance_current				= 0x35,
	rt_frame_luminance_histogram				= 0x36,
	rt_apply_indirect_lighting_ds			= 0x37,
	rt_frame_luminance0						= 0x38,
	rt_frame_luminance1						= 0x39,
	rt_frame_luminance2						= 0x3a,
	rt_frame_luminance3						= 0x3b,
	rt_frame_luminance4						= 0x3c,
	rt_frame_luminance5						= 0x3d,
	rt_frame_luminance6						= 0x3e,
	rt_frame_luminance7						= 0x3f,
	rt_frame_luminance8						= 0x40,
	rt_mie_scattering						= 0x41,
	rt_rayleigh_scattering					= 0x42,
	rt_frame_lum_scene_downsampled			= 0x43,
	rt_result_frame_luminance_histogram		= 0x44,
	rt_frame_luminance_lockable				= 0x45,
	rt_num_render_targets					= 0x46,
	rt_num_frame_luminance_targets			= 0x09,
};

pcstr rt_index_to_name( enum_render_target_index index );

struct render_target_instance {
	fixed_string< 64 >	orig_name;
	fixed_string< 64 >	name;
	render_target_ptr	target;
	res_texture_ptr		texture;
};

STATIC_SIZE_ASSERT( render_target_instance, 0xA0 );

class renderer_context_targets {
public:
	explicit renderer_context_targets( math::uint2 size );
	~renderer_context_targets( );

	void new_rt(
		enum_render_target_index	index,
		DXGI_FORMAT				in_format,
		math::uint2 const		in_size,
		enum_rt_usage			usage,
		bool					enabled
	);
	void new_ds(
		enum_render_target_index index,
		DXGI_FORMAT in_format,
		math::uint2 const in_size
	)
	{
		new_rt( index, in_format, in_size, enum_rt_usage_depth_stencil, true );
	}
	void new_lt(
		enum_render_target_index	index,
		DXGI_FORMAT				in_format,
		math::uint2 const		in_size
	);

	math::uint2 size( ) const { return m_size; }
	void resize( math::uint2 size, bool force_resize );
	u32 memory_usage( ) const { return m_memory_usage; }
	u32 get_id( ) const { return static_cast< u32 >( m_id ); }

public:
	render_target_instance	m_family[rt_num_render_targets];
	math::uint2				m_size;

private:
	void create_targets( math::uint2 size, bool force_resize );
	static u32				s_new_id;
	s32 m_id;
	u32 m_memory_usage;
};

STATIC_SIZE_ASSERT( renderer_context_targets, 0x2BD0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_TARGETS_H_INCLUDED
