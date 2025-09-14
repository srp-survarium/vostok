#ifndef        HMODEL_H
#define HMODEL_H

#include "common.h"

cbuffer combine_globals
{
	uniform float4		L_ambient;		// L_ambient.w = skynbox-lerp-factor
	uniform float4		fog_color;
} // cbuffer combine_globals

sampler         s_env_s0;
sampler         s_env_s1;
//uniform samplerCUBE         sky_s0                ;
//uniform samplerCUBE         sky_s1                ;

TextureCube		t_env_s0;
TextureCube		t_env_s1;
TextureCube		t_sky_s0;
TextureCube		t_sky_s1;

uniform float4	env_color;        // color.w  = lerp factor
//uniform float3x4	m_V2W;

void hmodel ( out float3 hdiffuse, out float3 hspecular, 
			 float m, float h, float s, float3 Pnt, float3 normal)
{
        // hscale - something like diffuse reflection
	float3	nw		= mul( m_V2W, float4(normal.xyz, 0.f) ).xyz;
	float	hscale	= h;	//. *        (.5h + .5h*nw.y);

#ifdef         USE_GAMMA_22
			hscale	= (hscale*hscale);        // make it more linear
#endif

	// reflection vector
	float3	v2PntL	= normalize( Pnt );
	float3	v2Pnt	= mul( m_V2W, float4(v2PntL, 0.f) ).xyz;
	float3	vreflect= reflect( v2Pnt, nw );
	float	hspec	= .5h + .5h * dot( vreflect, v2Pnt );

	// material	// sample material
	//float4	light	= tex3D( s_material, float3(hscale, hspec, m) );
//	float4	light	= s_material.Sample( smp_material, float3( hscale, hspec, m ) ).xxxy;
//	float4	light	= t_material.SampleLevel( s_material1, float3( hscale, hspec, m ), 0 ).xxxy;
	float4	light	= float4(1,1,1,1);

	// diffuse color
//	float3	e0d		= texCUBE( env_s0, nw );
//	float3	e1d		= texCUBE( env_s1, nw );
	float3	e0d		= t_env_s0.SampleLevel( s_env_s0, nw, 0 ).xyz;
	float3	e1d		= t_env_s1.SampleLevel( s_env_s1, nw, 0 ).xyz;
	float3	env_d	= env_color.xyz * lerp( e0d, e1d, env_color.w );
			env_d	*=env_d;	// contrast
			hdiffuse= env_d * light.xyz + L_ambient.rgb;

	// specular color
	vreflect.y      = vreflect.y*2-1;	// fake remapping
//	float3	e0s		= texCUBE( env_s0, vreflect );
//	float3	e1s		= texCUBE( env_s1, vreflect );
//	float3	e0s		= t_env_s0.Sample( s_env_s0, vreflect );
//	float3	e1s		= t_env_s0.Sample( s_env_s1, vreflect );
	float3	e0s		= t_env_s0.SampleLevel( s_env_s0, vreflect, 0 ).xyz;
	float3	e1s		= t_env_s1.SampleLevel( s_env_s1, vreflect, 0 ).xyz;
	float3	env_s	= env_color.xyz * lerp( e0s, e1s, env_color.w);
			env_s	*=env_s;	// contrast
		hspecular	= env_s*light.w*s;                //*h*m*s        ;        //env_s        *light.w         * s;
}

/*
void         hmodel_table        (out float3 hdiffuse, out float3 hspecular, float m, float h, float s, float3 point, float3 normal)
{
        // hscale - something like diffuse reflection
        float         hscale         = h;

        // reflection vector
        float3         v2point        = normalize        (Pnt);
        float3        vreflect= reflect         (v2point,normal);
        float         hspec         = .5h+.5h*dot        (vreflect,v2point);

        // material
          float4         light        = tex3D                (s_material, float3(hscale, hspec, m) );                // sample material

        // diffuse color
        float3         env_d         = texCUBE         (env_s0,normal);

        // specular color
        float3         env_s          = texCUBE         (env_s0,vreflect);

        //
        hdiffuse        = env_d        *light.xyz         + L_ambient.rgb        ;
        hspecular        = env_s        *light.w         * s                ;
}
*/
#endif