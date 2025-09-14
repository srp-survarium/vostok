#ifndef	FB_REFLECTION_H
#define FB_REFLECTION_H

//uniform float3x4 m_V2W;

float3	psf_reflection(float3 default_color, TextureCube _t_cubemap, float3 normal, float3 pixel_view_position)
{
	float3 to_eye = normalize( pixel_view_position);
	
	float3 r_view_space  = reflect( to_eye, normal);
	
	float3 r_world_space = mul( (float3x3)m_V2W, r_view_space);
	
	if ( CONFIG_REFLECTION)
		return _t_cubemap.Sample( s_rtlinear, r_world_space).rgb;
	else
		return default_color;
}

#endif // FB_REFLECTION_H
/*
#if CONFIG_REFLECTION
#	define V2P_PARAM_TANGENTS		1
#endif // #if CONFIG_REFLECTION

#ifndef	FB_REFLECTION_H
#define FB_REFLECTION_H

uniform float3x4 m_V2W;

float3	psf_reflection(float3 default_color, float3 normal, float3 pixel_view_position, v2p_common	I)
{
	float3 t = mul(m_W,I.tbn_x);
	float3 b = mul(m_W,I.tbn_y);
	float3 n = mul(m_W,I.tbn_z);
	
	float3x3 tbn = float3x3(t,b,n);
	normal = mul(float3(0,0,1),tbn);
	
	float3 to_eye = eye_position - mul(m_V2W,float4(pixel_view_position,1));
	
	float3 r = reflect(to_eye, normalize(normal));
	
	if ( CONFIG_REFLECTION)
		return t_cubemap.Sample( s_base, -r).rgb;
	else
		return default_color;
}



#endif // FB_REFLECTION_H
*/