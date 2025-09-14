////////////////////////////////////////////////////////////////////////////
//	Created		: 23.11.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef	FB_TC_MODIFIERS_H
#define FB_TC_MODIFIERS_H

float2 psf_rotate_tc(float2 tc, float2 center, float angle)
{
	float cosine, sine;
	sincos(angle, sine, cosine);
	
	return center + float2( 
						dot( float2(cosine,-sine), tc-center ), 
						dot( float2(sine,cosine),  tc-center ) 
					);
}

float2 psf_move_tc(float2 in_tc, float2 offset)
{
	return in_tc + offset;
}

float2 psf_calculate_sequence_tc(float2 tc, float size_u, float size_v, float start_frame, float time)
{
	const float num_images 		= 	size_u * size_v;
	
	const float index 			= 	num_images * frac(time + start_frame / num_images);
	
	const float index_v			=	floor(floor(index) / floor(size_u));
	const float index_u			=	floor(index - size_u * index_v); // %
	
	const float2 uv_pos			=	float2(index_u, index_v) / float2(size_u, size_v);
	const float2 uv_size		=	(1.0f).xx / float2(size_u, size_v);
	
	return uv_pos + tc * uv_size;
}

#endif // FB_TC_MODIFIERS_H

/*
	xy -= c;
	
	x' = [cos, -sin] [x]
	y' = [sin,  cos] [y]
	
	x' = cos * x + -sin * y    dot(c-s,xy)
	y' = sin * x +  cos * y    dot(sc,xy)
	
	f = c + x'y';
*/
