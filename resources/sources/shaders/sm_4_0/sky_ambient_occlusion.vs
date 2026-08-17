////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 from the shipped v0.100b blob
//	(shaders/sm_4_0/sky_ambient_occlusion.vs, plain permutation) - the file
//	was absent from the recovered source drop. The shipped DXBC is
//	byte-identical to the proven light.vs compile, and the blob's dependency
//	list names the same include closure (common_cbuffers.h,
//	common_functions.h, light.h), so the source is light.vs's volume
//	transform under this name.
////////////////////////////////////////////////////////////////////////////

#include "common_cbuffers.h"
#include "common_functions.h"
#include "light.h"

pixel_shader_input main			( float4 position : POSITION )
{
	pixel_shader_input			result;
	result.position				= mul( m_WVP, position);
	result.texture_coordinates	= get_texcoord( result.position );
	return						result;
}
