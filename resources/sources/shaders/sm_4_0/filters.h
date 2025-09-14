////////////////////////////////////////////////////////////////////////////
//	Created		: 12.04.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_FILTERS_H_INCLUDED
#define SHADERS_SM_4_0_FILTERS_H_INCLUDED


float depth_based_diagonal_filter(in float2 uv, in Texture2D color_texture, in sampler in_sampler, in float2 screen_size)
{
	float center_depth 		= gbuffer_read_frame_depth(uv);
	float ao_sum_xy 		= 0.0f;
	float ao_sum_yx 		= 0.0f;
	float total_weight_xy 	= 0.0f;
	float total_weight_yx 	= 0.0f;
	
	int x;
	UNROLL for (x=-2; x<=2; x++)
	{
		int y 				= x; 
		float2 sample_uv 	= uv + float2(x, y) / screen_size;
		float sample_ao 	= color_texture.Sample(in_sampler, sample_uv).x;
		float sample_depth 	= gbuffer_read_frame_depth(sample_uv);
		
		float depth_result  = 1.0f - saturate(pow((sample_depth - center_depth), 2.0f) / (0.001f * center_depth)) + 0.2f;
		
		total_weight_xy 	+= depth_result;
		
		ao_sum_xy 			+= sample_ao * depth_result;
	}
	
	UNROLL for (x=-2; x<=2; x++)
	{
		int y 				= -x; 
		float2 sample_uv 	= uv + float2(x, y) / screen_size;
		float sample_ao 	= color_texture.Sample(in_sampler, sample_uv).x;
		float sample_depth 	= gbuffer_read_frame_depth(sample_uv);
		
		float depth_result  = 1.0f - saturate(pow((sample_depth - center_depth), 2.0f) / (0.001f * center_depth)) + 0.2f;
		
		total_weight_yx     += depth_result;
		 
		ao_sum_yx 			+= sample_ao * depth_result;
	}
	return min(ao_sum_xy / total_weight_xy, ao_sum_yx / total_weight_yx);
}


float gaussian_coef(int x, int y)
{	
	float sigma = 1.0f;	
	return  1;//( 1 / ( sqrt(2.0f * 3.14f) * sigma ) ) * exp( -(x*x+y*y) / (2.0f * sigma * sigma) );
}

float photometric_weight(float4 intensity) 
{ 
	float sigma = 2.0f; 
	float weight = ((1.0f / sqrt(2.0f * 3.14f)) / sigma) * exp((intensity.x * intensity.x)/(2.0f * sigma * sigma));
	return weight; 
} 

float bilateral_filter(float2 uv, in Texture2D color_texture, in sampler in_sampler, in float2 screen_size)
{	
	float s    = color_texture.Sample(s_base, uv).x;
	float c	   = 1.0f;
	
	float4 k22 = color_texture.Sample(in_sampler, uv + c*float2( 0, 0) / screen_size);
	float  w22 = gaussian_coef(0,0);
	
	float4 k11 = color_texture.Sample(in_sampler, uv + c*float2(-1,-1) / screen_size);
	float  w11 = photometric_weight(abs(k22-k11)) * gaussian_coef(2,2);
	
	float4 k12 = color_texture.Sample(in_sampler, uv + c*float2(-1, 0) / screen_size);
	float  w12 = photometric_weight(abs(k22-k12)) * gaussian_coef(1,1);
	
	float4 k13 = color_texture.Sample(in_sampler, uv + c*float2(-1, 1) / screen_size);
	float  w13 = photometric_weight(abs(k22-k13)) * gaussian_coef(2,2);
	
	float4 k21 = color_texture.Sample(in_sampler, uv + c*float2( 0,-1) / screen_size);
	float  w21 = photometric_weight(abs(k22-k21)) * gaussian_coef(1,1);
	
	float4 k23 = color_texture.Sample(in_sampler, uv + c*float2( 0, 1) / screen_size);
	float  w23 = photometric_weight(abs(k22-k23)) * gaussian_coef(1,1);
	
	float4 k31 = color_texture.Sample(in_sampler, uv + c*float2( 1,-1) / screen_size);
	float  w31 = photometric_weight(abs(k22-k31)) * gaussian_coef(2,2);
	
	float4 k32 = color_texture.Sample(in_sampler, uv + c*float2( 1, 0) / screen_size);
	float  w32 = photometric_weight(abs(k22-k32)) * gaussian_coef(1,1);
	
	float4 k33 = color_texture.Sample(in_sampler, uv + c*float2( 1, 1) / screen_size);
	float  w33 = photometric_weight(abs(k22-k33)) * gaussian_coef(2,2);
	
	return (k11*w11 + k12*w12 + k13*w13 + k21*w21 + k22*w22 + k23*w23 + k31*w31 + k32*w32 + k33*w33).x/4.0f;
}


float4 simple_filter_2x2(float2 uv, in Texture2D color_texture, in sampler in_sampler, float2 texture_size)
{
	float2 texel_pos 	= texture_size * uv; 
	float2 lerps 		= frac(texel_pos); 
	float2 texel_size 	= 1.0f / texture_size;                 
	
	float4 sourcevals[4]; 
	sourcevals[0] = color_texture.Sample(in_sampler, uv); 
	sourcevals[1] = color_texture.Sample(in_sampler, uv + float2(texel_size.x, 0)); 
	sourcevals[2] = color_texture.Sample(in_sampler, uv + float2(0, texel_size.y)); 
	sourcevals[3] = color_texture.Sample(in_sampler, uv + float2(texel_size.x, texel_size.y));   
    /*
	float center_depth 		= gbuffer_read_frame_depth(uv);
	float ao_sum_xy 		= 0.0f;
	float ao_sum_yx 		= 0.0f;
	float total_weight_xy 	= 0.0f;
	float total_weight_yx 	= 0.0f;
	
	for (int i=0; i<4; i++)
	{
		float sample_depth 	= gbuffer_read_frame_depth(sample_uv);
		float depth_result  = 1.0f - saturate(pow((sample_depth - center_depth), 2.0f) / (0.001f * center_depth)) + 0.2f;
		
		total_weight_xy 	+= depth_result;
		
		ao_sum_xy 			+= sample_ao * depth_result;		
	}
	*/
		
	float4 interpolated = lerp(lerp(sourcevals[0], sourcevals[1], lerps.x), lerp(sourcevals[2], sourcevals[3], lerps.x ), lerps.y); 

	return interpolated;
}

// Change these 2 defines to change precision,
#define vec float3
#define toVec(x) x.rgb

#define s2(a, b)				temp = a; a = min(a, b); b = max(temp, b);
#define mn3(a, b, c)			s2(a, b); s2(a, c);
#define mx3(a, b, c)			s2(b, c); s2(a, c);

#define mnmx3(a, b, c)			mx3(a, b, c); s2(a, b);                                   // 3 exchanges
#define mnmx4(a, b, c, d)		s2(a, b); s2(c, d); s2(a, c); s2(b, d);                   // 4 exchanges
#define mnmx5(a, b, c, d, e)	s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);           // 6 exchanges
#define mnmx6(a, b, c, d, e, f) s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f); // 7 exchanges

float3 median_filter_3x3(in float2 uv, in Texture2D in_texture, in sampler in_sampler, in float2 screen_size)
{
	vec v[9];
	
	// Add the pixels which make up our window to the pixel array.
	UNROLL for(int dX = -1; dX <= 1; ++dX) 
	{
		UNROLL for(int dY = -1; dY <= 1; ++dY) 
		{
		  float2 offset = float2(float(dX), float(dY));
			
		  // If a pixel in the window is located at (x+dX, y+dY), put it at index (dX + R)(2R + 1) + (dY + R) of the
		  // pixel array. This will fill the pixel array, with the top left pixel of the window at pixel[0] and the
		  // bottom right pixel of the window at pixel[N-1].
		  v[(dX + 1) * 3 + (dY + 1)] = toVec(in_texture.Sample( in_sampler, uv + offset / screen_size));
		}
	}
	vec temp;
	
	// Starting with a subset of size 6, remove the min and max each time
	mnmx6(v[0], v[1], v[2], v[3], v[4], v[5]);
	mnmx5(v[1], v[2], v[3], v[4], v[6]);
	mnmx4(v[2], v[3], v[4], v[7]);
	mnmx3(v[3], v[4], v[8]);
	
	return v[4];
}


#endif	//	SHADERS_SM_4_0_FILTERS_H_INCLUDED