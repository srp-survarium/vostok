#ifndef	FUN_TRANSLUCENT_H
#define FUN_TRANSLUCENT_H

float3 psf_ttranslucency( Texture2D t_trans, float2 tc)
{
	return t_trans.Sample( s_base, tc).xyz;
}

#endif // FUN_TRANSLUCENT_H