////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UNION_BASE_SHADER_CONFIGURATION_H_INCLUDED
#define RENDER_UNION_BASE_SHADER_CONFIGURATION_H_INCLUDED

namespace vostok {
namespace render {

struct union_base::shader_configuration {

	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	use_diffuse_texture;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	use_alpha_test;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_parallax;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_normal_texture;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_detail_normal_texture;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	use_detail_texture;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	use_translucency_texture;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 7 } )*/	use_specular_intensity_texture;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	use_fresnel_texture;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	use_roughness_texture;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_diffuse_power_texture;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_subuv;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_transparency_texture;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	use_variation_mask;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	shadowed_light;
	/* 0x0001 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 7 } )*/	alphablended_diffuse;
	/* 0x0002 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	alphablended_normal;
	/* 0x0002 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	use_sequence;
	/* 0x0002 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_reflection_mask;
	/* 0x0002 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_grass_fresnel_effect;
	/* 0x0002 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 4, position: 4 } )*/	light_type;
	/* 0x0003 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	use_bokeh_dof;
	/* 0x0003 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	use_bokeh_image;
	/* 0x0003 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 6, position: 2 } )*/	vertex_input_type;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 2, position: 0 } )*/	use_reflection;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 2, position: 2 } )*/	use_emissive;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 3, position: 4 } )*/	wind_motion;
	/* 0x0004 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 7 } )*/	decal_material;
	/* 0x0005 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 2, position: 0 } )*/	decal_type;
	/* 0x0005 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 5, position: 2 } )*/	post_process_blur_kernel;
	/* 0x0005 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 7 } )*/	use_anisotropic_direction_texture;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	is_anisotropic_material;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	use_ao_texture;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_organic_scattering_amount_mask;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_organic_scattering_depth_texture;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_organic_back_illumination_texture;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	use_organic_subdermal_texture;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	use_bound_normals;
	/* 0x0006 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 7 } )*/	use_up_directional_normals;
	/* 0x0007 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	use_local_reflections;
	/* 0x0007 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	use_diffuse_as_specular;
	/* 0x0007 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_vertex_blended_diffuse;
	/* 0x0007 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_vertex_blended_mask;
	/* 0x0007 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_vertex_blended_normal;
	/* 0x0007 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	use_vertex_blended_specular;
	/* 0x0008 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 3, position: 0 } )*/	num_used_terrain_layers;
	/* 0x0008 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 3, position: 3 } )*/	used_terrain_height_mask;
	/* 0x0008 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	use_indirect_specular;
	/* 0x0009 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 3, position: 0 } )*/	lod_index;
	/* 0x0009 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_specular_lighting;
	/* 0x0009 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_image_grain;
	/* 0x0009 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	env_probe_clip_by_normal;
	/* 0x0009 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	env_probe_with_shadows;
	/* 0x000a */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 2, position: 0 } )*/	env_probe_geometry_type;
	/* 0x000a */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_diffuse_masked_color;
	/* 0x000a */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_diffuse_masked_color_by_hue;
	/* 0x000a */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_uv_scrolling;
	/* 0x000a */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	use_normal_waves;
	/* 0x000a */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	use_fuzziness;
	/* 0x000b */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x20 ), length: 2, position: 0 } )*/	terrain_blend_mode;
	/* 0x000b */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	use_soft_edges;
	/* 0x000b */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	use_thickness_map;
	/* 0x000b */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	use_subsurface_scattering_mask_map;
	/* 0x000b */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	use_olta;
	/* 0x0000 */	u64		configuration[2];
}; // struct union_base::shader_configuration

STATIC_SIZE_ASSERT(union_base::shader_configuration, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UNION_BASE_SHADER_CONFIGURATION_H_INCLUDED
