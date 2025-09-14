#ifndef	FUN_DIFFUSE_POWER_H
#define FUN_DIFFUSE_POWER_H

float psf_tdiffuse_power( Texture2D t_diffuse_power, float2 tc)
{
	return 255*t_diffuse_power.Sample( s_base, tc).x;
}



#endif // FUN_DIFFUSE_POWER_H
