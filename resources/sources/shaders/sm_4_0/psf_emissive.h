#ifndef	PSF_EMISSIVE_POWER_H
#define PSF_EMISSIVE_POWER_H

float3	psf_emissive( Texture2D t_emiss, float2 tc, float3 multiplier)
{
 	return	multiplier*t_emiss.Sample( s_base, tc).rgb;
}

#endif // PSF_EMISSIVE_POWER_H