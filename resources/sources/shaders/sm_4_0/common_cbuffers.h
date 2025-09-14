#ifndef	common_cbuffers_h_included
#define	common_cbuffers_h_included

cbuffer	dynamic_transforms
{
	uniform float4x4		m_WVP;		// World View Projection composition
	uniform float3x4		m_WV;		
	uniform float3x4	    m_W;		
}

cbuffer	material_parameters
{
	uniform float4	    	solid_color_specular;		// color.xyz, specular_intensity
	uniform float4	    	solid_material_params;		// specular_power, diffuse_power, material_param1(translucesy), material_id
	uniform float4	    	solid_emission_color;		// emission color xyz
	//uniform float4	    	solid_diffuse_attenuation_color;		// diffuse attenuation color
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

cbuffer	static_globals
{
	uniform float4x4 	m_P;
	uniform float4x4 	m_VP;
	uniform float3x4	m_V;			
	
	uniform float4		fog_params;		// x=near*(1/(far-near)), ?,?, w = -1/(far-near)
	
//	uniform float3		L_sun_color;
	uniform float3 		eye_position;	// in world space
	uniform float3 		eye_direction;
	
	uniform float		scene_time;
	
	//	uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)
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

uniform float			fresnel_at_0_degree;
uniform float			alpha_model_parameter;
uniform float3x4 		m_V2W;

#endif	//	common_cbuffers_h_included