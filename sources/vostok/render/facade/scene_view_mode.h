// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_SCENE_VIEW_MODE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SCENE_VIEW_MODE_H_INCLUDED
namespace vostok {
namespace render {

enum scene_view_mode
{
	wireframe_view_mode = 0x00,
	wireframe_two_sided_view_mode = 0x01,
	unlit_view_mode = 0x02,
	lit_view_mode = 0x03,
	normals_view_mode = 0x04,
	specular_color_view_mode = 0x05,
	translucency_view_mode = 0x06,
	fresnel_view_mode = 0x07,
	roughness_view_mode = 0x08,
	miplevel_view_mode = 0x09,
	lighting_view_mode = 0x0a,
	lighting_diffuse_view_mode = 0x0b,
	lighting_specular_view_mode = 0x0c,
	lighting_specular_mul_intensity_view_mode = 0x0d,
	shader_complexity_view_mode = 0x0e,
	texture_density_view_mode = 0x0f,
	geometry_complexity_view_mode = 0x10,
	emissive_only_view_mode = 0x11,
	distortion_only_view_mode = 0x12,
	ambient_occlusion_only_view_mode = 0x13,
	unlit_with_ao_view_mode = 0x14,
	overdraw_view_mode = 0x15,
	indirect_lighting_view_mode = 0x16,
	vertex_alpha_view_mode = 0x17,
	lpv_geometry_view_mode = 0x18,
	lighting_luminance_view_mode = 0x19,
	num_view_modes = 0x1a,
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SCENE_VIEW_MODE_H_INCLUDED
