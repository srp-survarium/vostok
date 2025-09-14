#ifndef	FUN_SPECULAR_POWER_H
#define FUN_SPECULAR_POWER_H

uniform float min_roughness;
uniform float max_roughness;

float2 psf_tspecular_power( Texture2D t_spec_power, float2 tc)
{
	return min_roughness + (max_roughness - min_roughness) * max( t_spec_power.Sample( s_base, tc).yy, 0.01f );
}

#endif // FUN_SPECULAR_POWER_H