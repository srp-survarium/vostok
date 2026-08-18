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
