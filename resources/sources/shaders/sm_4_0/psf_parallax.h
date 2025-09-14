////////////////////////////////////////////////////////////////////////////
//	Created		: 13.10.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef	FB_BUMP_H
#define FB_BUMP_H

float constant_parallax_scale;

#if CONFIG_PARALLAX
float4 min_t_height_map;
float4 max_t_height_map;
#endif // #if CONFIG_PARALLAX

float get_parallax_height(Texture2D in_t_bump, inout float2 tc)
{
#if CONFIG_PARALLAX
	return in_t_bump.Sample(s_base, tc).x * constant_parallax_scale;
#else
	return 0;
#endif // #if CONFIG_PARALLAX	
}

void calculate_steepparalax(Texture2D in_t_bump, float3x3 tbn, float3 position, inout float2 out_tc, inout float out_height)
{
	static const float fParallaxStartFade = 8.0f;
	static const float fParallaxStopFade = 12.0f;
	
	out_height = get_parallax_height(in_t_bump, out_tc);
	
	if (position.z < fParallaxStopFade)
	{
		const float maxSamples = 25;
		const float minSamples = 5;
		const float fParallaxOffset = -0.013;

		float3	 eye = mul ( transpose(tbn), -position.xyz);
		
		eye = normalize(eye);
		
		//	Calculate number of steps
		float nNumSteps = lerp( maxSamples, minSamples, eye.z );
		
		float	fStepSize			= 1.0 / nNumSteps;
		float2	vDelta				= eye.xy * fParallaxOffset*1.2;
		float2	vTexOffsetPerStep	= fStepSize * vDelta;
		
		//	Prepare start data for cycle
		float2	vTexCurrentOffset	= out_tc;
		float	fCurrHeight			= 0.0;
		float	fCurrentBound		= 1.0;
		
		[unroll(25)]	//	Doesn't work with [loop]
		for( ;fCurrHeight < fCurrentBound; fCurrentBound -= fStepSize )
		{
			vTexCurrentOffset += vTexOffsetPerStep;		
			fCurrHeight = get_parallax_height(in_t_bump, vTexCurrentOffset.xy);
		}
		
		//	Reconstruct previouse step's data
		vTexCurrentOffset -= vTexOffsetPerStep;
		float fPrevHeight = get_parallax_height(in_t_bump, vTexCurrentOffset.xy);
		
		//	Smooth tc position between current and previouse step
		float	fDelta2 = ((fCurrentBound + fStepSize) - fPrevHeight);
		float	fDelta1 = (fCurrentBound - fCurrHeight);
		float	fParallaxAmount = (fCurrentBound * fDelta2 - (fCurrentBound + fStepSize) * fDelta1 ) / ( fDelta2 - fDelta1 );
		float	fParallaxFade 	= smoothstep(fParallaxStopFade, fParallaxStartFade, position.z);
		float2	vParallaxOffset = vDelta * ((1- fParallaxAmount )*fParallaxFade);
		
		out_tc  += vParallaxOffset;
	}
}

void calculate_paralax(Texture2D in_t_bump, float3x3 tbn, float3 position, inout float2 out_tc, inout float out_height)
{
//	why not a simpler one?
//	float3	 eye = mul( -position.xyz, tbn );
	float3	 eye = mul( transpose(tbn), -position.xyz);
	
	out_height = get_parallax_height(in_t_bump, out_tc);
	
	float	height	= get_parallax_height(in_t_bump, out_tc);
			height	= height*(parallax.x) + (parallax.y);
	
	out_tc  += height * normalize(eye).xy;
}

float2 psf_parallax( Texture2D _t_height_map, in float3 position, in float2 tc, in float3x3 tangent_trans, out float out_height)
{
	float2 out_tc = tc;
	
	if ( GLOBAL_ALLOW_STEEPPARALLAX)
	{
		calculate_steepparalax(_t_height_map, tangent_trans, position.xyz, out_tc, out_height);
	}
	else
	{
		calculate_paralax	  (_t_height_map, tangent_trans, position.xyz, out_tc, out_height);
	}
	return 	out_tc;
}

float2 psf_parallax( Texture2D _t_height_map, in float3 position, in float2 tc, in float3x3 tangent_trans)
{
	float dummy_height;
	return psf_parallax( _t_height_map, position, tc, tangent_trans, dummy_height);
}


#endif // FB_BUMP_H