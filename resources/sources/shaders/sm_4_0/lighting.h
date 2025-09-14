#ifndef __LIGHTINGCOMMON_H_INCLUDED__
#	define __LIGHTINGCOMMON_H_INCLUDED__

uniform sampler1D s_mat_params;

// float4 GetMaterialParamsTex(float m)
// {
	// return tex1D(s_mat_params, m);
// }

static const float4 MatParams[5] =
{
	//{C, m, R, interpolation between white and material color}
	{0.05f, 100.0f, 0.3f, 0.0f},
	{0.05f, 10.0f, 0.8f, 0.2f},
	{0.6f, 0.07f, 1.0f, 1.0f},
	{0.25f, 0.001f, 0.9f, 0.9f},
	{0.05f, 100.0f, 0.3f, 0.0f}, // Repeat of the first line
};

float4 GetMaterialParams(float m) //GetMaterialParamsConst
{
#	ifdef USE_LIGHTING_PARAMS_FROM_TEXTURE
	return tex1D(s_mat_params, m);
#	else
	float mFixed = m*4 - 0.5;
	if (mFixed<0) mFixed += 4;
	
	int   i1 = floor(mFixed);
	int   i2 = floor(mFixed+1);
	float t  = frac(mFixed);
	
	return lerp(MatParams[i1], MatParams[i2], t);
#	endif
}
#endif
