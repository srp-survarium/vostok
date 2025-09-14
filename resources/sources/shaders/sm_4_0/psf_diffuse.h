#ifndef	FB_DIFFUSE_TEST_H
#define FB_DIFFUSE_TEST_H

float4	psf_tdiffuse_impl( float4 default_color, Texture2D _t_base, float2 tc)
{
	if ( CONFIG_TDIFFUSE)
		return _t_base.Sample( s_base, tc);
	else
		return default_color;
}


float4	psf_tdiffuse( float4 default_color, v2p_common I)
{
	if ( CONFIG_TDIFFUSE)
		return psf_tdiffuse_impl( default_color, t_base, I.tc);
	else
		return default_color;
}


#endif // FB_DIFFUSE_TEST_H