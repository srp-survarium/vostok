#ifndef	FUN_SPECULAR_INTENSITY_H
#define FUN_SPECULAR_INTENSITY_H

float3 psf_tspecular_intensity( Texture2D t_spec_intensity, float2 tc)
{
	return t_spec_intensity.Sample(s_base, tc).rgb;
}


#endif // FUN_SPECULAR_INTENSITY_H
