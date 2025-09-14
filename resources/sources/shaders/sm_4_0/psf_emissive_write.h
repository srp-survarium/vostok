#ifndef	PSF_EMISSIVE_WRITE_H
#define PSF_EMISSIVE_WRITE_H


void	psf_emissive_write( float3 emission_color, inout g_struct output)
{
#if RT_EXTENTION_TARGET
		output.emissive.xyz = emission_color * 0.25f * 0.5f;
		output.emissive.w   = 0.0f;
#endif
}



#endif // PSF_EMISSIVE_WRITE_H
