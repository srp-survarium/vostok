#ifndef	FB_DETAIL_TEST_H
#define FB_DETAIL_TEST_H

float3	psf_tdetail( Texture2D _t_detail, float3 color, float2 tc)
{
	return 2 * color.rgb * _t_detail.Sample( s_detail, tc).rgb;
}


#endif // FB_DETAIL_TEST_H