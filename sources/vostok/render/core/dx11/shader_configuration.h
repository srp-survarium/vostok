#ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONFIGURATION_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SHADER_CONFIGURATION_H_INCLUDED

namespace vostok {
namespace render {
namespace union_base {

struct shader_configuration {
#pragma warning( push )
#pragma warning( disable : 4201 )
	union {
		struct {
			bool use_diffuse_texture : 1;
			bool use_alpha_test : 1;
			bool use_parallax : 1;
			bool use_normal_texture : 1;
			bool use_detail_normal_texture : 1;
			bool use_detail_texture : 1;
			bool use_translucency_texture : 1;
			bool use_specular_intensity_texture : 1;

			bool use_fresnel_texture : 1;
			bool use_roughness_texture : 1;
			bool use_diffuse_power_texture : 1;
			bool use_subuv : 1;
			bool use_transparency_texture : 1;
			bool use_variation_mask : 1;
			bool shadowed_light : 1;
			bool alphablended_diffuse : 1;

			bool alphablended_normal : 1;
			bool use_sequence : 1;
			bool use_reflection_mask : 1;
			bool use_grass_fresnel_effect : 1;
			u8 light_type : 4;

			bool use_bokeh_dof : 1;
			bool use_bokeh_image : 1;
			u8 vertex_input_type : 6;

			u8 use_reflection : 2;
			u8 use_emissive : 2;
			u8 wind_motion : 3;
			bool decal_material : 1;

			u8 decal_type : 2;
			u8 post_process_blur_kernel : 5;
			bool use_anisotropic_direction_texture : 1;

			bool is_anisotropic_material : 1;
			bool use_ao_texture : 1;
			bool use_organic_scattering_amount_mask : 1;
			bool use_organic_scattering_depth_texture : 1;
			bool use_organic_back_illumination_texture : 1;
			bool use_organic_subdermal_texture : 1;
			bool use_bound_normals : 1;
			bool use_up_directional_normals : 1;

			bool use_local_reflections : 1;
			bool use_diffuse_as_specular : 1;
			bool use_vertex_blended_diffuse : 1;
			bool use_vertex_blended_mask : 1;
			bool use_vertex_blended_normal : 1;
			bool use_vertex_blended_specular : 1;

			u8 num_used_terrain_layers : 3;
			u8 used_terrain_height_mask : 3;
			bool use_indirect_specular : 1;

			u8 lod_index : 3;
			bool use_specular_lighting : 1;
			bool use_image_grain : 1;
			bool env_probe_clip_by_normal : 1;
			bool env_probe_with_shadows : 1;

			bool env_probe_geometry_type : 2;
			bool use_diffuse_masked_color : 1;
			bool use_diffuse_masked_color_by_hue : 1;
			bool use_uv_scrolling : 1;
			bool use_normal_waves : 1;
			bool use_fuzziness : 1;

			u8 terrain_blend_mode : 2;
			bool use_soft_edges : 1;
			bool use_thickness_map : 1;
			bool use_subsurface_scattering_mask_map : 1;
			bool use_olta : 1;
		};
		u64 configuration[2];
	};
#pragma warning( pop )
};

inline bool operator<(
	shader_configuration const& left,
	shader_configuration const& right
)
{
	// FUNCTION BODY[0x124630]
	return left.configuration[0] < right.configuration[0]
		|| (left.configuration[0] == right.configuration[0]
			&& left.configuration[1] < right.configuration[1]);
}

} // namespace union_base

struct shader_configuration : public union_base::shader_configuration {
	shader_configuration( ) { reset( ); }

	void reset( )
	{
		configuration[0] = 0;
		configuration[1] = 0;
		num_used_terrain_layers = 4;
	}
};

STATIC_SIZE_ASSERT( union_base::shader_configuration, 0x10 );
STATIC_SIZE_ASSERT( shader_configuration, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONFIGURATION_H_INCLUDED
