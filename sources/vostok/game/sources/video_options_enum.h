// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIDEO_OPTIONS_ENUM_H_INCLUDED
#define VIDEO_OPTIONS_ENUM_H_INCLUDED

namespace survarium {

enum video_options_enum
{
	monitor_count_option			= 0x00,
	resolution_option				= 0x01,
	fullscreen_option				= 0x02,
	vsync_option					= 0x03,
	antialiasing_option				= 0x04,
	aniso_filtering_option			= 0x05,
	gamma_option					= 0x06,
	fov_option						= 0x07,
	graphics_quality_option			= 0x08,
	texture_quality_option			= 0x09,
	geometry_quality_option			= 0x0a,
	shadow_quality_option			= 0x0b,
	lightning_quality_option		= 0x0c,
	shading_quality_option			= 0x0d,
	decorations_option				= 0x0e,
	post_process_option				= 0x0f,
	ambient_occlusion_option		= 0x10,
	particles_quality_option		= 0x11,
	motion_blur_amount_option		= 0x12,
	video_options_count				= 0x13,
};

} // namespace survarium

#endif // #ifndef VIDEO_OPTIONS_ENUM_H_INCLUDED
