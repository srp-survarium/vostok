#ifndef	common_cbuffers_h_included
#define	common_cbuffers_h_included

cbuffer	dynamic_transforms
{
	uniform float4x4		m_WVP;		// World View Projection composition
	uniform float3x4		m_WV;		
	uniform float3x4	    m_W;		
}

// Ship layout, byte-proven: the alpha-test permutations of depth_accumulate
// (and eleven other shipped names) record exactly these 38 members at these
// offsets - alpha_ref_parameter at 476, 504 bytes in all. The recovered
// 3-member version was this cbuffer's head.
cbuffer	material_parameters
{
	uniform float4	solid_color_specular;		// color.xyz, specular_intensity
	uniform float4	solid_material_params;		// specular_power, diffuse_power, material_param1(translucesy), material_id
	uniform float4	solid_emission_color;		// emission color xyz
	uniform float4	specular_fresnel_roughness_parameters;
	uniform float4	packed_variation_mask_parameters;
	uniform float4	variation_color;
	uniform float4	far_fog_color_and_distance;
	uniform float4	environment_skylight_upper_color;
	uniform float4	environment_skylight_lower_color;
	uniform float4	environment_skylight_parameters;
	uniform float4	detail_normal_parameters;
	uniform float4	specular_intensity_ranges;
	uniform float4	specular_color_multiplier;
	uniform float4	vegetation_fresnel_parameters;
	uniform float4	sequence_parameters;
	uniform float4	uv_scrolling_parameters;
	uniform float3	detailed_bending_parameters;
	uniform float3	wind_info_parameters;
	uniform float3	ambient_color;
	uniform float3	hue_matrix_component_x;
	uniform float3	hue_matrix_component_y;
	uniform float3	hue_matrix_component_z;
	uniform float3	fuzziness_parameters;
	uniform float3	constant_diffuse_mask_color;
	uniform float3	sun_direction;
	uniform float3	sun_color;
	uniform float3	specular_color_parameter;
	uniform float3	vertex_blend_parameters;
	uniform float3	normal_waves_parameters;
	uniform float2	constant_tile_uv;
	uniform float	ditail_texture_tile;
	uniform float	alpha_ref_parameter;
	uniform float	reflection_power;
	uniform float	min_reflection_angle;
	uniform float	near_fog_distance;
	uniform float	fog_alpha;
	uniform float	smoothness_multiplier;
	uniform float	wind_scale;
}

cbuffer	MOVED_FROM_dynamic_transforms
{
	//	Used by VS only
	uniform float4			L_material;	// 0,0,0,mid
	//uniform float4          hemi_cube_pos_faces;
	//uniform float4          hemi_cube_neg_faces;
	//uniform	float4 			dt_params;	//	Detail params
}

cbuffer	shader_params
{
	float	alpha_ref;
}

// Ship layout, byte-proven: every static_globals RDEF among the shipped blobs
// (73 shaders) records exactly these members at these offsets - m_V2W inside
// the cbuffer, eye_position_view_space present, screen_res live.
cbuffer	static_globals
{
	uniform float4x4 	m_P;
	uniform float4x4 	m_VP;
	uniform float3x4	m_V;
	uniform float3x4 	m_V2W;

	uniform float4		fog_params;		// x=near*(1/(far-near)), ?,?, w = -1/(far-near)

//	uniform float3		L_sun_color;
	uniform float3 		eye_position;	// in world space
	uniform float3 		eye_position_view_space;
	uniform float3 		eye_direction;

	uniform float		scene_time;

	uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)
}

cbuffer	MOVED_FROM_static_globals
{
	uniform float4x4 	m_texgen;
	uniform float4x4 	mVPTexgen;
	uniform float4		timers;
	uniform float4		fog_plane;
	uniform float4 		pos_decompression_params;
	uniform float4 		pos_decompression_params2;
	uniform float4		L_hemi_color;
	uniform float3		L_sun_dir_w;
}

// The loose fresnel_at_0_degree / alpha_model_parameter / m_V2W uniforms this
// header used to declare are gone at ship: no shipped $Globals lists them
// (an unused member of a used $Globals still appears in RDEF, so declaring
// them anywhere common would show), and m_V2W lives in static_globals above.

#endif	//	common_cbuffers_h_included