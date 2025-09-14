////////////////////////////////////////////////////////////////////////////
//	Created		: 07.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPEEDTREE_COMMON_H_INCLUDED
#define SHADERS_SM_4_0_SPEEDTREE_COMMON_H_INCLUDED

#define speedtree_pi 3.141592653f
#define speedtree_pi_x2 (speedtree_pi * 2.0f)

cbuffer	speedtree_common_parameters
{
	float4 lod_profile_parameter;
	float4 tree_position_and_scale_parameter;	// xyz = position, w = scale
	float4 tree_rotation_parameter;			 	// xy = rotation values, w = lod lerp value (1 = highest, 0 = lowest), z = current lod value 
	float4 lod_reference_position_parameter;
}; // cbuffer	speedtree_common_parameters


float3 adjust_for_tree_rotation(float3 component, float2 rotation)
{
	float4 rotation_values = float4(rotation.xy, -rotation.x, 0.0f);
	
	return float3(dot(rotation_values.ywz, component.xyz), component.y, dot(rotation_values.xwy, component.xyz));
}

float4 uncompress_vector4(float4 in_vector)
{
	return in_vector / float4(127.5f, 127.5f, 127.5f, 255.0f) - float4(1.0f, 1.0f, 1.0f, 0.0);
}

float4 uncompress_wind_vector(float4 in_raw_value, float in_scalar)
{
	float4 result 	= in_raw_value / 127.5f - 1.0f;
		   result 	*= float4((in_scalar).xxx, speedtree_pi);
	
	return result;
}

float speedtree_fade(float value, float start, float end)
{
	return saturate((value - start) / (end - start));
}

float compute_billboard_fade(float in_distance)
{
	 // shouldn't be higher than 0.5
	const float offset_length 	= 0.5f;
	float offset 				= offset_length * (lod_profile_parameter.w - lod_profile_parameter.z);
	float start 				= lod_profile_parameter.z;
	float end 					= lod_profile_parameter.w - offset;
	
	return speedtree_fade		(in_distance, start, end);
}

float compute_3d_fade()
{
	float dist 		= distance(lod_reference_position_parameter.xyz, tree_position_and_scale_parameter.xyz);
	float offset 	= 0.7f * (lod_profile_parameter.w - lod_profile_parameter.z);
	float start 	= lod_profile_parameter.z + offset;
	float end 		= lod_profile_parameter.w;
	
	return 			1.0f - speedtree_fade(dist, start, end);
}

float3x3 rotation_matrix_z_axis(float angle)
{
    float2 sin_cos;
	
#ifdef SPEEDTREE_OPENGL
 	sin_cos.x = sin(angle);
	sin_cos.y = cos(angle);
#else
    sincos(angle, sin_cos.x, sin_cos.y);
#endif // #ifdef SPEEDTREE_OPENGL

    return float3x3(sin_cos.y, 	-sin_cos.x,  0.0f, 
                    sin_cos.x, 	sin_cos.y, 	 0.0f, 
                    0.0f, 		0.0f, 		 1.0f);
}

float2 rotate_z_axis(float angle, float3 in_point)
{
    float2 sin_cos;
	
#ifdef SPEEDTREE_OPENGL
 	sin_cos.x = sin(angle);
	sin_cos.y = cos(angle);
#else
    sincos(angle, sin_cos.x, sin_cos.y);
#endif // #ifdef SPEEDTREE_OPENGL
    
    return float2(dot(sin_cos.yx, in_point.xy), dot(float2(-sin_cos.x, sin_cos.y), in_point.xy));
}

float3x3 rotation_matrix_y_axis(float angle)
{
    float2 sin_cos;
	
#ifdef SPEEDTREE_OPENGL
 	sin_cos.x = sin(angle);
	sin_cos.y = cos(angle);
#else
    sincos(angle, sin_cos.x, sin_cos.y);
#endif // #ifdef SPEEDTREE_OPENGL
    
    return float3x3(sin_cos.y, 	0.0f,  sin_cos.x,
                    0.0f, 		1.0f,  0.0f,
                    -sin_cos.x, 0.0f,  sin_cos.y);
}

float2 rotate_y_axis(float angle, float3 in_point)
{
    float2 sin_cos;
	
#ifdef SPEEDTREE_OPENGL
 	sin_cos.x = sin(angle);
	sin_cos.y = cos(angle);
#else
    sincos(angle, sin_cos.x, sin_cos.y);
#endif // #ifdef SPEEDTREE_OPENGL
	
    return float2(dot(float2(sin_cos.y, -sin_cos.x), in_point.xz), dot(sin_cos.xy, in_point.xz));
}

float3x3 rotation_matrix_x_axis(float angle)
{
    float2 sin_cos;
	
#ifdef SPEEDTREE_OPENGL
 	sin_cos.x = sin(angle);
	sin_cos.y = cos(angle);
#else
    sincos(angle, sin_cos.x, sin_cos.y);
#endif // #ifdef SPEEDTREE_OPENGL
	
    return float3x3(1.0f, 	0.0f, 		0.0f,
                    0.0f, 	sin_cos.y, 	-sin_cos.x,
                    0.0f, 	sin_cos.x, 	sin_cos.y);
}

float2 rotate_x_axis(float angle, float3 in_point)
{
    float2 sin_cos;
	
#ifdef SPEEDTREE_OPENGL
 	sin_cos.x = sin(angle);
	sin_cos.y = cos(angle);
#else
    sincos(angle, sin_cos.x, sin_cos.y);
#endif // #ifdef SPEEDTREE_OPENGL
    
    return float2(dot(sin_cos.yx, in_point.yz), dot(float2(-sin_cos.x, sin_cos.y), in_point.yz));
}






// SpeedTree wind.

#define SPEEDTREE_FROND_RIPPLING
#define SPEEDTREE_BASIC_WIND
#define SPEEDTREE_DIRECTIONAL_WIND
//#define SPEEDTREE_ADVANCED_WIND

///////////////////////////////////////////////////////////////////////  
//  Configurations

#if defined(SPEEDTREE_DIRECTIONAL_WIND) && !defined(SPEEDTREE_BASIC_WIND)
	#define SPEEDTREE_BASIC_WIND
#endif

#if defined(SPEEDTREE_BASIC_WIND) || defined(SPEEDTREE_DIRECTIONAL_WIND)


cbuffer	speedtree_wind_parameters
{
	// wind
	float3		wind_direction_parameter;	// xyz = dir
	float4		wind_times_parameter;		// x = primary, y = secondary, z = frond, w = leaves
	float4		wind_distances_parameter;	// x = primary osc, y = secondary osc, z = wind height, w = height exponent
	float3		wind_leaves_parameter;		// x = distance, y = leaves lighting change, z = leaves windward scalar
	float3		wind_frond_ripple_parameter;// x = amount, y = u tile, z = v tile
	float3		wind_gust_parameter;		// x = combined wind strength, y = primary distance, z = gust scale
	float3		wind_gust_hints;			// x = vertical offset %, y = wind dir adjustment, z = unison
	float4		wind_rolling_branches; 		// x = max scale, y = min scale, z = speed, w = wavelength
	float4		wind_rolling_leaves; 		// x = max scale, y = min scale, z = speed, w = wavelength
	float4		wind_twitching_leaves; 		// x = amount, y = sharpness, z = frequency 1, w = frequency 2
	float4		wind_tumbling_leaves; 		// x = x tumble (radians), y = y tumble (radians), z = z tumble (radians), w = separation
}; // cbuffer speedtree_wind_parameters

#define SPEEDTREE_Y_UP 1

#define ROLLING
#define TWITCHING
#define TWO_AXIS_WIND

float speedtree_roll(float current, float max_scale, float min_scale, float speed, float wave_length, float3 pos, float time)
{
	float wind_angle 			= dot(pos, -wind_direction_parameter) / wave_length;
	float adjust 				= sin(wind_angle + time * speed);
	adjust						= (adjust + 1.0f) * 0.5f;
	
	return 						lerp(current * min_scale, current * max_scale, adjust);
}

float speedtree_twitch(float3 pos, float amount, float sharpness, float freq1, float freq2, float time)
{
	float twitch 				= sin(freq1 * time + (pos.x + pos.z)) * cos(freq2 * time + pos.y);
	twitch 						= (twitch + 1.0f) * 0.5f;
	
	return 						amount * pow(twitch, sharpness);
}

float3 common_wind_motion(float3 pos, float4 wind_data, out float3 out_offset)
{
	static const float frequency_scale = 100.0f;
	
	// get the oscillation times (they changed independently to allow smooth frequency changes in multiple components)
	float primary_time 			= wind_times_parameter.x + frequency_scale * tree_position_and_scale_parameter.x;
	float secondary_time 		= wind_times_parameter.y + frequency_scale * tree_position_and_scale_parameter.z;
	
	float3 orig_pos 			= pos;
	
	// compute how much the height contributes
	float adjust 				= max(pos.y * wind_distances_parameter.z, 0.0f);
	adjust 						= pow(adjust, wind_distances_parameter.w);
	
	// move a bare minimum due to gusting/strength
	float move_amount 			= wind_gust_parameter.y;
	
	// primary oscillation
	move_amount 				+= wind_distances_parameter.x * sin(primary_time * 0.3f) * cos(primary_time * 0.95f);
	move_amount 				*= adjust;
	
	// xz component
	pos.xz 						+= wind_direction_parameter.xz * move_amount;
	// move down a little to hide the sliding effect
	pos.y 						-= move_amount * wind_gust_hints.x;
	
	// secondary oscillation
	float oscillation 			= sin((secondary_time + wind_data.w) * 0.3f) * cos((secondary_time + wind_data.w) * 0.95f);
	
	// reported wind direction (this vector is not normalized and shouldn't be!)
	float3 dir 					= wind_data.xyz;
	
	#ifdef SPEEDTREE_ADVANCED_WIND
		oscillation				= speedtree_roll(
			oscillation, 
			wind_rolling_branches.x, 
			wind_rolling_branches.y, 
			wind_rolling_branches.z, 
			wind_rolling_branches.w, 
			pos.xyz, secondary_time
			);
	#endif
	
	#ifdef SPEEDTREE_DIRECTIONAL_WIND
		float3 new_wind_dir 	= wind_direction_parameter;
		
		// adjust based on artist's tuning
		new_wind_dir.z 			+= wind_gust_hints.y;
		new_wind_dir 			= normalize(new_wind_dir);
		
		// how long is it?  this length controls how much it should oscillate
		float len 				= length(wind_data.xyz);
		
		// make it oscillate as much as it would have if it wasn't going with the wind
		new_wind_dir 			*= len;
		
		// add the normal term and the 'with the wind term'
		float directionality 	= wind_gust_parameter.x * wind_gust_hints.z;
		pos.xyz 				+= (1.0f - directionality) * dir.xyz * oscillation * wind_distances_parameter.y;

	#ifdef SPEEDTREE_ADVANCED_WIND
		float3 vec_ortho 		= cross(dir.xyz, float3(0.0f, 0.0f, 1.0f));
		float oscillation2 		= sin((secondary_time + wind_data.w) * 0.5f) * cos((secondary_time + wind_data.w) * 0.6f);
		pos.xyz 				+= vec_ortho.xyz * oscillation2 * wind_distances_parameter.y;
	#endif
		
		pos.xyz 				+= directionality * new_wind_dir.xyz * lerp(oscillation, 1.0f, wind_gust_hints.z) * wind_distances_parameter.y * wind_gust_parameter.z;
	#else
		pos.xyz 				+= dir.xyz * oscillation * wind_distances_parameter.y;
	#endif
	
	out_offset 					= 0.6f * (pos - orig_pos);
	
	return pos;
}

float3 leaf_wind_motion(float3 pos, inout float3 direction, float scale)
{
	float2 dir 					= -normalize(pos.xz);
	float view_dot_wind 		= saturate(dot(dir.xy, wind_direction_parameter.xz));
	
	float dir_contribution 		= 0.5f + view_dot_wind * 2.0f;
	dir_contribution 			*= wind_leaves_parameter.z;
	
	float leaves_time 			= wind_times_parameter.w;
	
	float move_amount 			= (wind_leaves_parameter.x + dir_contribution * wind_leaves_parameter.x) * sin(leaves_time + direction.y * 1000.0f);
	pos.xyz 					+= direction.xyz * move_amount * scale;
	
	direction 					+= scale * float3(0.0f, saturate(scale * move_amount * wind_leaves_parameter.y), 0.0f);
	direction 					= normalize(direction);
	
	return pos;
}


#ifdef SPEEDTREE_ADVANCED_WIND

float3 leaf_wind_tumble(float3 pos, inout float3 direction, inout float3 tangent, inout float3 binormal, float scale, float3 anchor)
{
	float leaves_time 			= wind_times_parameter.w;

	float3 origin_pos 			= pos.xyz - anchor;

	float2 dir 					= -normalize(float3(anchor.xz, 0.0001f)).xy;
	float anchor_dot_wind 		= saturate(dot(dir.xy, wind_direction_parameter.xz));

	float dir_contribution 		= 0.5f + anchor_dot_wind * 2.0f;
	dir_contribution 			*= wind_leaves_parameter.z;

	// scale the effect based on rolling waves of wind and individual leaf twitches
	float effect_scale 			= speedtree_roll(
		1.0f, 
		wind_rolling_leaves.x, 
		wind_rolling_leaves.y, 
		wind_rolling_leaves.z, 
		wind_rolling_leaves.w, 
		anchor.xyz, 
		leaves_time
		);
	
	effect_scale 				+= speedtree_twitch(
		anchor.xyz, 
		wind_twitching_leaves.x, 
		wind_twitching_leaves.y, 
		wind_twitching_leaves.z, 
		wind_twitching_leaves.w, 
		leaves_time
		);
	
	
	// build an orientation that tumbles the leaves
	float z_amount 				= sin(leaves_time + wind_tumbling_leaves.w * (anchor.x + anchor.y + anchor.z));
	float3x3 mat_tumble			= rotation_matrix_y_axis(
		z_amount * effect_scale * scale * (wind_tumbling_leaves.z + dir_contribution * wind_tumbling_leaves.z)
		);
	
	float x_amount 				= cos(leaves_time + wind_tumbling_leaves.w * (-anchor.x - anchor.y - anchor.z));
	mat_tumble = mul(
		mat_tumble, 
		rotation_matrix_x_axis(x_amount * effect_scale * scale * (wind_tumbling_leaves.x + dir_contribution * wind_tumbling_leaves.x))
		);
	
	float y_amount 				= cos(leaves_time + wind_tumbling_leaves.w * (-anchor.x + anchor.y - anchor.z));
	mat_tumble = mul(
		mat_tumble, 
		rotation_matrix_z_axis(y_amount * effect_scale * scale * (wind_tumbling_leaves.y + dir_contribution * wind_tumbling_leaves.y))
		);
	
	// use the orientation matrix to tumble the vertex position and lighting data
	direction 					= mul(direction, mat_tumble);
	tangent 					= mul(tangent, mat_tumble);
	binormal 					= mul(binormal, mat_tumble);
	origin_pos 					= mul(origin_pos, mat_tumble);

	// add the anchor point back in and return the new position
	return 						origin_pos + anchor;
}
#endif

float3 billboard_motion(in float3 pos, float2 offset)
{
	// get the oscillation times (they changed independently to allow smooth frequency changes in multiple components)
	float primary_time 			= wind_times_parameter.x + offset.x;
	float secondary_time 		= wind_times_parameter.y + offset.y;
	
	// compute how much the height contributes
	float adjust 				= max(pos.y * wind_distances_parameter.z, 0.0f);
	adjust 						= pow(adjust, wind_distances_parameter.w);
	
	// move a bare minimum due to gusting/strength
	float move_amount 			= wind_gust_parameter.y;
	
	// primary oscillation
	move_amount 				+= wind_distances_parameter.x * sin(primary_time * 0.3f) * cos(primary_time * 0.95f);
	move_amount 				*= adjust;
	
	// xz component
	pos.xz 						+= wind_direction_parameter.xz * move_amount;
	// move down a little to hide the sliding effect
	pos.y 						-= move_amount * wind_gust_hints.x;
	
	return 						pos;
}

float3 compute_frond_ripple_offset(float3 lod_data, float3 position, float2 ripple, float3 normal)
{
	const float time 			= wind_times_parameter.z;
	float3 offset 				= abs(normal.xyz) * (sin(ripple[0] * time) * cos(ripple[0] * time + lod_data.z));
	offset 						*= 0.5f * ripple[1] * wind_frond_ripple_parameter.x;
	
	return 						offset;
}

float3 ripple_frond(float3 lod_data, float3 position, float2 ripple, float3 normal)
{
	float3 offset 				= compute_frond_ripple_offset(lod_data, position, ripple, normal);
	const float lod_lerp 		= tree_rotation_parameter.z;
	
	return 						lerp(lod_data.xyz, position.xyz, lod_lerp) + offset;
}

#endif // defined(SPEEDTREE_BASIC_WIND) || defined(SPEEDTREE_DIRECTIONAL_WIND)

#endif // #ifndef SHADERS_SM_4_0_SPEEDTREE_COMMON_H_INCLUDED