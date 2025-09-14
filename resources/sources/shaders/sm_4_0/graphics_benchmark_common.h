////////////////////////////////////////////////////////////////////////////
//	Created		: 05.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "common.h"


struct	v_GB_Test
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
	float4	Color	: COLOR; 
	float4 	Color0	: TEXCOORD1;
	float4 	Color1	: TEXCOORD2;
	float4 	Color2	: TANGENT;
	float4 	Color3	: BINORMAL0;
	float4 	Color4	: BINORMAL1;
	float4 	Color5	: BLENDINDICES;
	float4 	Color6	: BLENDWEIGHT;

	float3 	Normal	: NORMAL;
};

struct	v2p_GB_Test
{
	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)

	float4	Color	: COLOR;
	
	float2 	Tex0	: TEXCOORD0;
	
	float4 	Color0	: TEXCOORD1;
	float4 	Color1	: TEXCOORD2;
	float4 	Color2	: TANGENT;
	float4 	Color3	: BINORMAL0;
	float4 	Color4	: BINORMAL1;
	float4 	Color5	: BLENDINDICES;
	float4 	Color6	: BLENDWEIGHT;
	

	float3 	Normal	: NORMAL;
};

uniform float4 gvX, gvY, gvZ, gvW;
uniform float  gsX, gsY, gsZ, gsW;
float4 m_view_dir;


sampler s_first_mip;
static const float2 tex_xy = float2(1024.0,1024.0);

float2 get_coord_offset( in Texture2D tex, in float2 start_uv, in float jump_radius) 
{
	return start_uv + jump_radius * ( normalize(tex.Sample(s_first_mip, start_uv).xy) ) / tex_xy;
}



#define max3(x,y,z)   max(x,max(y,z))
#define max4(x,y,z,w) max(x,max(y,max(z,w)))
#define max5(x0,y0,z0,w0,x1) max(max4(x0,y0,z0,w0),x1)
#define max6(x0,y0,z0,w0,x1,y1) max3(max4(x0,y0,z0,w0),x1,y1)
#define max7(x0,y0,z0,w0,x1,y1,z1) max(max4(x0,y0,z0,w0),max3(x1,y1,y1))
#define max8(x0,y0,z0,w0,x1,y1,z1,w1) max(max4(x0,y0,z0,w0),max4(x1,y1,y1,w1))
#define max9(x0,y0,z0,w0,x1,y1,z1,w1,x2) max(max8(x0,y0,z0,w0,x1,y1,y1,w1),x2)

float4 fsqrt4(float4 X)
{
	return X / ( X + (0.18 + 0.6*X) * ( 1 - X ) );
}

float fsqrt(float X)
{
	return X / ( X + (0.18 + 0.6*X) * ( 1 - X ) );
}

float fsqrt2(float X)
{
	return X / (1.42 * X - 0.6 * X * X + 0.18);
}

float4 fpow4(float4 X, float K)
{
	return X / ( X + K * ( 1 - X ) );
}

float fpow(float X, float K)
{
	return X / ( X + K * ( 1 - X ) );
}

	
	//(400/(400-0.2) - 400*0.2/((200-0.2) *x))

//65536

float NormalizeZ(float z)
{
	return (z / 800.0);
}

float3 f24_to_rgb8(float f)
{
	return float3( f, frac(f * 256.0), frac(f * 65536.0));
}

float rgb8_to_f24(float3 V)
{
	return V.x + V.y * 1.0 / 256.0 + V.z * 1.0 / 65536.0;
}

float2 f16_to_rg8(float f)
{
	return float2( f, frac(f * 256.0) );
}

float rg8_to_f16(float2 V)
{
	return V.x + V.y * 1.0 / 256.0;
}





float3 
Strauss(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float fSmoothness, float fMetalness, float fTransparency)
{
    // Make sure the interpolated inputs and
    // constant parameters are normalized
    float3 n = _normal;
    float3 l = _light;
    float3 v = _view;
    float3 h = reflect( l, n );
	
    // Declare any aliases:
    float NdotL   = dot( n, l );
    float NdotV   = dot( n, v );
    float HdotV   = dot( h, v );
    float fNdotL  = 1 - NdotL;//fresnel( NdotL );
    float s_cubed = fSmoothness * fSmoothness * fSmoothness;
	
    // Evaluate the diffuse term
    float d  = ( 1.0f - fMetalness * fSmoothness );
    float Rd = ( 1.0f - s_cubed ) * ( 1.0f - fTransparency );
    float3 diffuse = NdotL * d * Rd * vDiffuse;
 
    // Compute the inputs into the specular term
    float r = ( 1.0f - fTransparency ) - Rd;
 
    float j = fNdotL * (1 - NdotL) * (1 - NdotL);//shadow( NdotL ) * shadow( NdotV );
 
    // 'k' is used to provide small off-specular
    // peak for very rough surfaces. Can be changed
    // to suit desired results...
    const float k = 0.1f;
    float reflect = min( 1.0f, r + j * ( r + k ) );
 
    float3 C1 = float3( 1.0f, 1.0f, 1.0f );
    float3 Cs = C1 + fMetalness * (1.0f - fNdotL) * (vDiffuse - C1);
 
    // Evaluate the specular term
    float3 specular = Cs * reflect * vSpecular;
    specular *= pow( -HdotV, 3.0f / (1.0f - fSmoothness) );
	
    // Composite the final result, ensuring
    // the values are >= 0.0f yields better results. Some
    // combinations of inputs generate negative values which
    // looks wrong when rendered...
    diffuse  = max( 0.0f, diffuse );
    specular = max( 0.0f, specular );
    return diffuse + specular;
}

float3 
OrenNayar(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float fRoughness)
{
    const float PI = 3.14159f;
	
    // Make sure the interpolated inputs and
    // constant parameters are normalized
    float3 n = _normal;
    float3 l = _light;
    float3 v = _view;
 
    // Compute the other aliases
    float alpha    = max( acos( dot( v, n ) ), acos( dot( l, n ) ) );
    float beta     = min( acos( dot( v, n ) ), acos( dot( l, n ) ) );
    float gamma    = dot( v - n * dot( v, n ), l - n * dot( l, n ) );
    float rough_sq = fRoughness * fRoughness;
 
    float C1       = 1.0f - 0.5f * ( rough_sq / ( rough_sq + 0.33f ) );
 
    float C2       = 0.45f * ( rough_sq / ( rough_sq + 0.09 ) );
    if( gamma >= 0 )
    {
        C2 *= sin( alpha );
    }
    else
    {
        C2 *= ( sin( alpha ) - pow( (2 * beta) / PI, 3 ) );
    }
 
    float C3  = (1.0f / 8.0f) ;
    C3       *= ( rough_sq / ( rough_sq + 0.09f ) );
    C3       *= pow( ( 4.0f * alpha * beta ) / (PI * PI), 2 );
 
    float A = gamma * C2 * tan( beta );
    float B = (1 - abs( gamma )) * C3 * tan( (alpha + beta) / 2.0f );
 
    return vDiffuse * max( 0.0f, dot( n, l ) ) * ( C1 + A + B );
}


float3 
Phong_OtherReflect(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float specPower)
{
	float3 refl = -_view + _normal * dot(_normal,_view) * 2.0;
	
	float3	DiffuseLighting  = saturate(dot(_normal,_light));
	float3 SpecularLighting = pow(saturate(dot(   refl,   _light)), specPower);
	
	return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

float fpow8(float X)
{
	return X*X*X*X*X*X*X*X;
}

float3 
Phong_OtherPower(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float specPower)
{
	float3	DiffuseLighting  = saturate(dot(_normal,_light));
	float3 SpecularLighting = fpow(saturate(dot(   reflect(_view,_normal),   _light)), specPower);
	
	return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

float3 
Phong_FPower8(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float specPower)
{
	float3	DiffuseLighting  = saturate(dot(_normal,_light));
	float3 SpecularLighting = fpow8(saturate(dot(   reflect(_view,_normal),   _light)));
	
	return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

float3 
Phong(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float specPower)
{
	float3	DiffuseLighting  = saturate(dot(_normal,_light));
	float3 SpecularLighting = pow(saturate(dot(   reflect(_view,_normal),   _light)), specPower);
	
	return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

half3
Phong_halfs(half3 vDiffuse, half3 vSpecular, half3 _normal, half3 _light, half3 _view, half specPower)
{
	half3	DiffuseLighting  = saturate(dot(_normal,_light));
	half3 SpecularLighting = pow(saturate(dot(   reflect(_view,_normal),   _light)), specPower);
	
	return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

float3 
Phong_Mul(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float specPower)
{
	float3	DiffuseLighting  = saturate(dot(_normal,_light));
	float3 SpecularLighting = (saturate(dot(   reflect(_view,_normal),   _light)) );
	
	return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

float3 
Blinn(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float specPower)
{
	float3	DiffuseLighting  = saturate(dot(_normal,_light));
	float3 SpecularLighting = pow(saturate(dot(   dot( normalize(_light + _view), _normal),    _light)), specPower);
	
    return vDiffuse * DiffuseLighting + SpecularLighting * vSpecular;
}

float3 
CookTorrance(float3 vDiffuse, float3 vSpecular, float3 _normal, float3 _light, float3 _view, float roughness_val)
{
	if (roughness_val <= 0.0) return 0.0;

	float3  half_vec = normalize( _view + _light );

	float NdotL    = max( dot( _normal, _light ), 0.0 );
	float NdotV    = max( dot( _normal, _view ), 0.0 );
	float NdotH    = max( dot( _normal, half_vec ), 1.0e-7 );
	float VdotH    = max( dot( _view,   half_vec ), 0.0 );

	float geometric = 2.0 * NdotH / VdotH;
		 geometric = min( 1.0, geometric * min(NdotV, NdotL) );

	float r_sq          = roughness_val * roughness_val;
	float NdotH_sq      = NdotH * NdotH;
	float NdotH_sq_r    = 1.0 / (NdotH_sq * r_sq);
	float roughness_exp = (NdotH_sq - 1.0) * ( NdotH_sq_r );
	float roughness     = exp(roughness_exp) * NdotH_sq_r / (4.0 * NdotH_sq );

	float fresnel       = 1.0 / (1.0 + NdotV);

	float Rs = min(1.0, (fresnel * geometric * roughness) / (NdotV * NdotL + 1.0));
	
	return NdotL  * (vDiffuse + vSpecular * Rs);
}

half3
CookTorrance_halfs(half3 vDiffuse, half3 vSpecular, half3 _normal, half3 _light, half3 _view, half roughness_val)
{
	if (roughness_val <= 0.0) return 0.0;

	half3  half_vec = normalize( _view + _light );

	half NdotL    = max( dot( _normal, _light ), 0.0 );
	half NdotV    = max( dot( _normal, _view ), 0.0 );
	half NdotH    = max( dot( _normal, half_vec ), 1.0e-7 );
	half VdotH    = max( dot( _view,   half_vec ), 0.0 );

	half geometric = 2.0 * NdotH / VdotH;
		 geometric = min( 1.0, geometric * min(NdotV, NdotL) );

	half r_sq          = roughness_val * roughness_val;
	half NdotH_sq      = NdotH * NdotH;
	half NdotH_sq_r    = 1.0 / (NdotH_sq * r_sq);
	half roughness_exp = (NdotH_sq - 1.0) * ( NdotH_sq_r );
	half roughness     = exp(roughness_exp) * NdotH_sq_r / (4.0 * NdotH_sq );

	half fresnel       = 1.0 / (1.0 + NdotV);

	half Rs = min(1.0, (fresnel * geometric * roughness) / (NdotV * NdotL + 1.0));
	
	return NdotL  * (vDiffuse + vSpecular * Rs);
}