////////////////////////////////////////////////////////////////////////////
//	Created		: 05.07.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED

namespace vostok {
namespace render {

struct volume_fog_parameters
{
	inline	volume_fog_parameters	( );

	math::float4x4	transform;
	math::float3	fog_color;
	math::float2	direction;
	float			height_falloff_offset;
	float			density;
	float			speed;
	float			noise_scale;
	float			wave_length;
	float			near_density;
	float			transparency_multiplier;
	float			density_offset;
}; // struct volume_fog_parameters

namespace
{
	static float clear_value	= -1.0f;
} // namespace

inline volume_fog_parameters::volume_fog_parameters( )
{
	transform					= vostok::math::float4x4().identity();
	fog_color					= float3( clear_value, clear_value, clear_value );
	direction					= float2( clear_value, 0.0f );
	density						= clear_value;
	speed						= clear_value;
	noise_scale					= clear_value;
	wave_length					= clear_value;
	transparency_multiplier		= clear_value;
	height_falloff_offset		= 0.0f;
	near_density				= 0.0f;
	density_offset				= 0.0f;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_VOLUME_FOG_PARAMETERS_H_INCLUDED
