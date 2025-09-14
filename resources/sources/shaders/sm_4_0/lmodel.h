#ifndef	LMODEL_H
#define LMODEL_H

#error do not include this header!

#include "common.h"


//////////////////////////////////////////////////////////////////////////////////////////
// Lighting formulas			// 
//float4 plight_infinity( float m, float3 pnt, float3 normal, float3 light_direction )
//{
//  	float3 N			= normal;							// normal 
//  	float3 V 		= -normalize	(pnt);					// vector2eye
//  	float3 L 		= -light_direction;						// vector2light
//  	float3 H			= normalize	(L+V);						// float-angle-vector 
////	return tex3D 		(s_material,	float3( dot(L,N), dot(H,N), m ) );		// sample material
//	return t_material.Sample( s_material1, float3( dot(L,N), dot(H,N), m ) ).xxxy;		// sample material
//}
/*
float plight_infinity2( float m, float3 pnt, float3 normal, float3 light_direction )
{
  	float3 N		= normal;							// normal 
  	float3 V 	= -normalize		(pnt);		// vector2eye
  	float3 L 	= -light_direction;					// vector2light
 	float3 H		= normalize			(L+V);			// float-angle-vector 
	float3 R     = reflect         	(-V,N);
	float 	s	= saturate(dot(L,R));
			s	= saturate(dot(H,N));
	float 	f 	= saturate(dot(-V,R));
			s  *= f;
	float4	r	= tex3D 			(s_material,	float3( dot(L,N), s, m ) );	// sample material
			r.w	= pow(saturate(s),4);
  	return	r	;
}
*/

//float4 plight_local( float m, float3 pnt, float3 normal, float3 light_position, float light_range_rsq, out float rsqr )
//{
//  	float3 N		= normal;							// normal 
//  	float3 L2P 	= pnt-light_position;                         		// light2point 
//  	float3 V 	= -normalize	(pnt);					// vector2eye
//  	float3 L 	= -normalize	((float3)L2P);					// vector2light
//  	float3 H		= normalize	(L+V);						// float-angle-vector
//		rsqr	= dot		(L2P,L2P);					// distance 2 light (squared)
//  	float  att 	= saturate	(1 - rsqr*light_range_rsq);			// q-linear attenuate
////	float4 light	= tex3D		(s_material, float3( dot(L,N), dot(H,N), m ) ); 	// sample material
//	float4 light	= t_material.Sample( s_material, float3( dot(L,N), dot(H,N), m ) ).xxxy;		// sample material
//  	return att*light;
//}

float phong_infinity_diffuse( float3 pnt, float3 normal, float3 light_dir, float translucency, float diffuse_power)
{
	float3 viewer 		= normalize	(pnt);
	float NdotL 		= dot( normal, -light_dir );
	float pure_diffuse	= saturate( NdotL );
	
	diffuse_power /= 255.f;
	float diffuse_powered = (0.7 + diffuse_power*0.3) * pure_diffuse/( pure_diffuse + diffuse_power * ( 1 - pure_diffuse ));
	float diffuse      = diffuse_powered + translucency/255.f * saturate( 0.4 - NdotL );
	
	return diffuse;
}

float2 phong_infinity( float3 pnt, float3 normal, float3 light_dir, float specular_power, float translucency, float diffuse_power)
{
	//light_dir = -light_dir;
	float3 viewer 		= normalize	(pnt);
	
	// Compute the reflection vector
	float3 reflection   = reflect( light_dir, normal );
		
	// Compute the angle between the reflection and the viewer
	float  RdotV        = max( -dot( reflection, viewer ), 0.0f );
		
	// Compute the specular color
	//float specular		= RdotV/( RdotV + (20*m) * ( 1 - RdotV ));
	float specular		= pow( RdotV, specular_power );
	
	float NdotL 		= dot( normal, -light_dir );
	
	float pure_diffuse	= saturate( NdotL );
	
	diffuse_power /= 255.f;
	float diffuse_powered = (0.7 + diffuse_power*0.3) * pure_diffuse/( pure_diffuse + diffuse_power * ( 1 - pure_diffuse ));
	//float diffuse_powered = pure_diffuse;

	// Compute the diffuse term for the Phong equation
	float diffuse      = diffuse_powered + translucency/255.f * saturate( 0.4 - NdotL );
		
	// Determine the final color    
	return float2( diffuse, specular );
}

float2 phong_infinity( float specular_power, float3 pnt, float3 normal, float3 light_dir)
{
	return phong_infinity( pnt, normal, light_dir, specular_power, 0.f, 1.f);
}

//float2 phong_point( float3 pnt, float3 normal, float3 light_pos, float range_rsqr, float specular_power, float translucency, float diffuse_power)
//{
//	// Normalized vector pointing to the point being lit from viewpoint.
//	float3 viewer 		= normalize	(pnt);
//	
//	// Vector from point to light
//	float3 light_vector = pnt - light_pos;
//	
//	// Light normalized vector
//	float3 light_dir	= normalize( light_vector);
//	
//	// Compute the reflection vector
//	float3 reflection   = reflect( light_dir, normal);
//		
//	// Compute the angle between the reflection and the viewer
//	float  RdotV        = max( -dot( reflection, viewer ), 0.0f );
//		
//	// Compute the specular color
//	//float specular		= RdotV/( RdotV + (20*m) * ( 1 - RdotV ));
//	float specular		= pow( RdotV, specular_power );
//	
//	float NdotL 		= dot( normal, -light_dir );
//	
//	float pure_diffuse	= saturate( NdotL );
//	
//	float diffuse_powered = (0.7 + diffuse_power*0.3) * pure_diffuse/( pure_diffuse + diffuse_power * ( 1 - pure_diffuse ));
//	//float diffuse_powered = pure_diffuse;
//
//	// Compute the diffuse term for the Phong equation
//	float diffuse      = diffuse_powered + translucency/255.f * saturate( 0.4 - NdotL );
//	
//	// Compute attenuation depending on light distance from the point being lit
//	float  attenuation 	= saturate	(1 - dot(light_vector,light_vector)*range_rsqr);
//	
//	// Determine the final color    
//	return float2( diffuse, specular )*attenuation;
//}
//
//float2 phong_point( float specular_power, float3 pnt, float3 normal, float3 light_pos, float range_rsqr)
//{
//	return phong_point( pnt, normal, light_pos, range_rsqr, specular_power, 0.f, 1.f);
//}

//	TODO: DX10: Remove path without blending
float4 blendp( float4 value, float4 tcp)
{
//	#ifndef FP16_BLEND  
//		value 	+= (float4)tex2Dproj 	(s_accumulator, tcp); 	// emulate blend
//	#endif
	return 	value;
}

float4 blend( float4 value, float2 tc)
{
//	#ifndef FP16_BLEND  
//		value 	+= (float4)tex2D 	(s_accumulator, tc); 	// emulate blend
//	#endif
	return 	value;
}

#endif