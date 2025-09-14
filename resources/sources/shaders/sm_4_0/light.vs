////////////////////////////////////////////////////////////////////////////
//	Created		: 17.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "common_cbuffers.h"
#include "common_functions.h"
#include "light.h"

uniform float4					s_near_far;  // near, far inv_near, inv_far

pixel_shader_input main			( float4 position : POSITION )
{
	pixel_shader_input			result;
	result.position				= mul( m_WVP, position);
	result.texture_coordinates	= get_texcoord( result.position );
	return						result;
}