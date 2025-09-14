#ifndef	PSF_MAKE_TANGENT_TRANSFORM
#define PSF_MAKE_TANGENT_TRANSFORM

float3x3 psf_make_tangent_transform( float3 tbn_x, float3 tbn_y, float3 tbn_z)
{
	return float3x3( tbn_x, tbn_y, tbn_z);
}

#endif // PSF_MAKE_TANGENT_TRANSFORM