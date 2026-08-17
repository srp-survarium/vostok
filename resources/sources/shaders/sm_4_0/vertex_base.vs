////////////////////////////////////////////////////////////////////////////
//	Created		: 01.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 from the shipped v0.100b blobs
//	(shaders/sm_4_0/vertex_base.vs, all 90 permutations). Byte-identity
//	against every blob is the acceptance test:
//	python3 -m vostok.shaders roundtrip vertex_base.vs
//
//	The recovered file was a thin wrapper over vertex_input.h, but the
//	per-type headers behind it are a later drift: the shipped
//	CONFIG_VERTEX_INPUT_TYPE numbering is the engine's
//	enum_vertex_input_type (sources/vostok/render/facade/
//	vertex_input_type.h) - 0 null, 1 static, 2 static colored,
//	3..6 skeletal with 4..1 bones, 7 particle, 8 particle subuv,
//	9 beamtrail, 10 decal, 11 grassmesh, 12 post process, 13 wires,
//	14 user - not the mapping in common_defines.h. The shipped structs,
//	the LOD0 view-position path, the skeletal prev-frame bones, the
//	0.00007125 static wind and the grassmesh sway exist in no recovered
//	header, so this file is self-contained: it includes nothing and
//	declares the ship cbuffer layouts itself (material_parameters here is
//	the full 38-member ship layout, not the 3-member one in
//	common_cbuffers.h).
//
//	Four more shipped names are this file, or nearly:
//	forward_sky.vs is byte-identical in all 15 permutations it ships;
//	vertex_base_lpv.vs (15), vertex_base_shadow.vs (38) and
//	vertex_base_shadow_batched.vs (38) each replace only the static arm -
//	and lpv/shadow also the skeleton arm - and are byte-identical to this
//	file everywhere else. Each carries its own reconstruction header; read
//	them together, because they are what pins which parts of this file are
//	shared and which are the g-stage pass's own.
////////////////////////////////////////////////////////////////////////////

/*	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
		CONFIG_WIND_MOTION,
		CONFIG_LOD_INDEX,
		CONFIG_USE_UP_DIRECTIONAL_NORMALS,
		CONFIG_USE_BOUND_NORMALS,
		CONFIG_USE_SUBUV,
		GLOBAL_USE_VEGETATION_TRAMPLE,
*/

cbuffer	dynamic_transforms
{
	uniform float4x4		m_WVP;		// World View Projection composition
	uniform float3x4		m_WV;
	uniform float3x4	    m_W;
}

cbuffer	material_parameters
{
	uniform float4	    	solid_color_specular;
	uniform float4	    	solid_material_params;
	uniform float4	    	solid_emission_color;
	uniform float4	    	specular_fresnel_roughness_parameters;
	uniform float4	    	packed_variation_mask_parameters;
	uniform float4	    	variation_color;
	uniform float4	    	far_fog_color_and_distance;
	uniform float4	    	environment_skylight_upper_color;
	uniform float4	    	environment_skylight_lower_color;
	uniform float4	    	environment_skylight_parameters;
	uniform float4	    	detail_normal_parameters;
	uniform float4	    	specular_intensity_ranges;
	uniform float4	    	specular_color_multiplier;
	uniform float4	    	vegetation_fresnel_parameters;
	uniform float4	    	sequence_parameters;
	uniform float4	    	uv_scrolling_parameters;
	uniform float3	    	detailed_bending_parameters;
	uniform float3	    	wind_info_parameters;
	uniform float3	    	ambient_color;
	uniform float3	    	hue_matrix_component_x;
	uniform float3	    	hue_matrix_component_y;
	uniform float3	    	hue_matrix_component_z;
	uniform float3	    	fuzziness_parameters;
	uniform float3	    	constant_diffuse_mask_color;
	uniform float3	    	sun_direction;
	uniform float3	    	sun_color;
	uniform float3	    	specular_color_parameter;
	uniform float3	    	vertex_blend_parameters;
	uniform float3	    	normal_waves_parameters;
	uniform float2	    	constant_tile_uv;
	uniform float	    	ditail_texture_tile;
	uniform float	    	alpha_ref_parameter;
	uniform float	    	reflection_power;
	uniform float	    	min_reflection_angle;
	uniform float	    	near_fog_distance;
	uniform float	    	fog_alpha;
	uniform float	    	smoothness_multiplier;
	uniform float	    	wind_scale;
}

cbuffer	static_globals
{
	uniform float4x4 	m_P;
	uniform float4x4 	m_VP;
	uniform float3x4	m_V;
	uniform float3x4 	m_V2W;

	uniform float4		fog_params;

	uniform float3 		eye_position;
	uniform float3 		eye_position_view_space;
	uniform float3 		eye_direction;

	uniform float		scene_time;

	uniform float4		screen_res;
}

cbuffer	skeleton_bones
{
	uniform float4x4	bones_matrices[80];
}

cbuffer	skeleton_bones_prev
{
	uniform float4x4	prev_bones_matrices[80];
}

float3 	unpack_normal( float3 v )	{ return 2*v-1; }
float3	unpack_D3DCOLOR( float3 c ) { return c.bgr; }

void vsf_tangent_space_transform( float3 normal, float3 tangent, float3 bitangent, out float3 tbn_x, out float3 tbn_y, out float3 tbn_z)
{
	float3x3 TBN = mul( (float3x3)m_WV,
		transpose(	float3x3(
						tangent.xyz,
						bitangent,
						normal.xyz)) );

	tbn_x 	 = TBN[0];
	tbn_y 	 = TBN[1];
	tbn_z 	 = TBN[2];
}

#if CONFIG_VERTEX_INPUT_TYPE==1 || CONFIG_VERTEX_INPUT_TYPE==2
//////////////////////////////////////////////////////////////////////////
// static mesh (2 = vertex colored)
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
	float4	normal				: NORMAL;
	float4	tangent				: TANGENT;
	float4	binormal			: BINORMAL;
	float2	tc					: TEXCOORD0;
#if CONFIG_VERTEX_INPUT_TYPE==2
	float4	color				: COLOR;
#endif
};

#if CONFIG_LOD_INDEX==0
struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	tbn_x				: TEXCOORD0;
	float3	tbn_y				: TEXCOORD1;
	float3	tbn_z				: TEXCOORD2;
	float3	position			: TEXCOORD3;
	float3	view_position		: TEXCOORD4;
	float2	tc					: TEXCOORD5;
#if CONFIG_VERTEX_INPUT_TYPE==2
	float3	color				: TEXCOORD6;
	float	color_alpha			: TEXCOORD7;
#endif
};
#else // #if CONFIG_LOD_INDEX==0
struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	tbn_x				: TEXCOORD1;
	float3	tbn_y				: TEXCOORD2;
	float3	tbn_z				: TEXCOORD3;
	float2	tc					: TEXCOORD4;
#if CONFIG_VERTEX_INPUT_TYPE==2
	float	color_alpha			: TEXCOORD5;
#endif
};
#endif // #if CONFIG_LOD_INDEX==0

void add_wind_motion(in float3 local_position, inout float3 position)
{
#if (CONFIG_WIND_MOTION==1 || CONFIG_WIND_MOTION==2) && CONFIG_LOD_INDEX==0
	float dist  				= length(local_position) * local_position.y;
	float s, c;

	float3 object_pos			=	float3(m_W[0][3], m_W[1][3], m_W[2][3]);

	const float period			=	0.25f;
	const float movement_mult 	=	0.00007125f;

	float time					=	scene_time + scene_time;

	s							=	sin(time + object_pos.x * period);
	c							=	cos(time + object_pos.z * period);

	dist						*=	movement_mult;

	position.x 					+=  dist * s;
	position.z 					+=  dist * c;
#endif
}

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
#if CONFIG_USE_UP_DIRECTIONAL_NORMALS
	float3 normal				= float3(0.f, 1.f, 0.f);
	float3 tangent				= float3(1.f, 0.f, 0.f);
	float3 binormal				= float3(0.f, 0.f, 1.f);
#else
	float3 normal				= unpack_normal( unpack_D3DCOLOR( input.normal.xyz));
	float3 tangent				= unpack_normal( unpack_D3DCOLOR( input.tangent.xyz));
	float3 binormal				= unpack_normal( unpack_D3DCOLOR( input.binormal.xyz));
#endif

#if CONFIG_LOD_INDEX==0
	float3 position				= mul(m_W,  float4( input.position, 1.f));

	add_wind_motion				(input.position, position);

	float3 view_position		= mul(m_V,  float4( position, 1.f));

	output.position				= position;
	output.homogeneous_position	= mul( m_P,  float4( view_position, 1.f));
	output.view_position		= view_position;
#else // #if CONFIG_LOD_INDEX==0
	output.homogeneous_position	= mul( m_WVP,  float4( input.position, 1.f));
	output.position				= mul(m_W,  float4( input.position, 1.f));
#endif // #if CONFIG_LOD_INDEX==0

	vsf_tangent_space_transform	(normal, tangent, binormal, output.tbn_x, output.tbn_y, output.tbn_z);

	output.tc 					= input.tc;

#if CONFIG_VERTEX_INPUT_TYPE==2
#if CONFIG_LOD_INDEX==0
	output.color				= input.color.xyz;
#endif
	output.color_alpha			= input.color.w;
#endif
}

#elif CONFIG_VERTEX_INPUT_TYPE>=3 && CONFIG_VERTEX_INPUT_TYPE<=6
//////////////////////////////////////////////////////////////////////////
// skeleton mesh (3 = 4 bones, 4 = 3 bones, 5 = 2 bones, 6 = 1 bone)
//////////////////////////////////////////////////////////////////////////

#define BONES_COUNT		(7-CONFIG_VERTEX_INPUT_TYPE)

struct vertex_input_struct
{
	float3	position		: POSITION;
	uint4	bones_indices	: BLENDINDICES;
#if BONES_COUNT==4
	float3	bones_weights	: BLENDWEIGHT;
#elif BONES_COUNT==3
	float2	bones_weights	: BLENDWEIGHT;
#elif BONES_COUNT==2
	float	bones_weights	: BLENDWEIGHT;
#endif
	float4	normal			: NORMAL;
	float4	tangent			: TANGENT;
	float4	binormal		: BINORMAL;
	float2	tc				: TEXCOORD0;
};

struct vertex_output_struct
{
	float3	tbn_x				: TEXCOORD0;
	float3	tbn_y				: TEXCOORD1;
	float3	tbn_z				: TEXCOORD2;
	float4  homogeneous_position: SV_Position;
	float3	view_position		: TEXCOORD3;
	float3	prev_position		: TEXCOORD4;
	float3	position			: TEXCOORD5;
	float2	tc					: TEXCOORD6;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 normal		= unpack_normal( unpack_D3DCOLOR( input.normal.xyz));
	float3 tangent		= unpack_normal( unpack_D3DCOLOR( input.tangent.xyz));
	float3 binormal		= unpack_normal( unpack_D3DCOLOR( input.binormal.xyz));

	float4x4 m0			= bones_matrices[ input.bones_indices[0] ];
#if BONES_COUNT>=2
	float4x4 m1			= bones_matrices[ input.bones_indices[1] ];
#endif
#if BONES_COUNT>=3
	float4x4 m2			= bones_matrices[ input.bones_indices[2] ];
#endif
#if BONES_COUNT>=4
	float4x4 m3			= bones_matrices[ input.bones_indices[3] ];
#endif

#if BONES_COUNT==4
	float last_weight	= 1.f - input.bones_weights[0] - input.bones_weights[1] - input.bones_weights[2];

	float3 skinned_position	=
		mul( m0, float4(input.position, 1.f) ).xyz * input.bones_weights[0] +
		mul( m1, float4(input.position, 1.f) ).xyz * input.bones_weights[1] +
		mul( m2, float4(input.position, 1.f) ).xyz * input.bones_weights[2] +
		mul( m3, float4(input.position, 1.f) ).xyz * last_weight;

	float3 skinned_normal	=
		normalize(
			mul( (float3x3)m0, normal ) * input.bones_weights[0] +
			mul( (float3x3)m1, normal ) * input.bones_weights[1] +
			mul( (float3x3)m2, normal ) * input.bones_weights[2] +
			mul( (float3x3)m3, normal ) * last_weight
		);

#if CONFIG_LOD_INDEX==0
	float3 skinned_tangent	=
		normalize(
			mul( (float3x3)m0, tangent ) * input.bones_weights[0] +
			mul( (float3x3)m1, tangent ) * input.bones_weights[1] +
			mul( (float3x3)m2, tangent ) * input.bones_weights[2] +
			mul( (float3x3)m3, tangent ) * last_weight
		);

	float3 skinned_binormal	=
		normalize(
			mul( (float3x3)m0, binormal ) * input.bones_weights[0] +
			mul( (float3x3)m1, binormal ) * input.bones_weights[1] +
			mul( (float3x3)m2, binormal ) * input.bones_weights[2] +
			mul( (float3x3)m3, binormal ) * last_weight
		);

	float3 prev_position	=
		mul( prev_bones_matrices[ input.bones_indices[0] ], float4(input.position, 1.f) ).xyz * input.bones_weights[0] +
		mul( prev_bones_matrices[ input.bones_indices[1] ], float4(input.position, 1.f) ).xyz * input.bones_weights[1] +
		mul( prev_bones_matrices[ input.bones_indices[2] ], float4(input.position, 1.f) ).xyz * input.bones_weights[2] +
		mul( prev_bones_matrices[ input.bones_indices[3] ], float4(input.position, 1.f) ).xyz * last_weight;
#endif // #if CONFIG_LOD_INDEX==0

#elif BONES_COUNT==3
	float last_weight	= 1.f - input.bones_weights[0] - input.bones_weights[1];

	float3 skinned_position	=
		mul( m0, float4(input.position, 1.f) ).xyz * input.bones_weights[0] +
		mul( m1, float4(input.position, 1.f) ).xyz * input.bones_weights[1] +
		mul( m2, float4(input.position, 1.f) ).xyz * last_weight;

	float3 skinned_normal	=
		normalize(
			mul( (float3x3)m0, normal ) * input.bones_weights[0] +
			mul( (float3x3)m1, normal ) * input.bones_weights[1] +
			mul( (float3x3)m2, normal ) * last_weight
		);

#if CONFIG_LOD_INDEX==0
	float3 skinned_tangent	=
		normalize(
			mul( (float3x3)m0, tangent ) * input.bones_weights[0] +
			mul( (float3x3)m1, tangent ) * input.bones_weights[1] +
			mul( (float3x3)m2, tangent ) * last_weight
		);

	float3 skinned_binormal	=
		normalize(
			mul( (float3x3)m0, binormal ) * input.bones_weights[0] +
			mul( (float3x3)m1, binormal ) * input.bones_weights[1] +
			mul( (float3x3)m2, binormal ) * last_weight
		);

	float3 prev_position	=
		mul( prev_bones_matrices[ input.bones_indices[0] ], float4(input.position, 1.f) ).xyz * input.bones_weights[0] +
		mul( prev_bones_matrices[ input.bones_indices[1] ], float4(input.position, 1.f) ).xyz * input.bones_weights[1] +
		mul( prev_bones_matrices[ input.bones_indices[2] ], float4(input.position, 1.f) ).xyz * last_weight;
#endif // #if CONFIG_LOD_INDEX==0

#elif BONES_COUNT==2
	float last_weight	= 1.f - input.bones_weights;

	float3 skinned_position	=
		mul( m0, float4(input.position, 1.f) ).xyz * input.bones_weights +
		mul( m1, float4(input.position, 1.f) ).xyz * last_weight;

	float3 skinned_normal	=
		normalize(
			mul( (float3x3)m0, normal ) * input.bones_weights +
			mul( (float3x3)m1, normal ) * last_weight
		);

#if CONFIG_LOD_INDEX==0
	float3 skinned_tangent	=
		normalize(
			mul( (float3x3)m0, tangent ) * input.bones_weights +
			mul( (float3x3)m1, tangent ) * last_weight
		);

	float3 skinned_binormal	=
		normalize(
			mul( (float3x3)m0, binormal ) * input.bones_weights +
			mul( (float3x3)m1, binormal ) * last_weight
		);

	float3 prev_position	=
		mul( prev_bones_matrices[ input.bones_indices[0] ], float4(input.position, 1.f) ).xyz * input.bones_weights +
		mul( prev_bones_matrices[ input.bones_indices[1] ], float4(input.position, 1.f) ).xyz * last_weight;
#endif // #if CONFIG_LOD_INDEX==0

#else // BONES_COUNT==1
	float3 skinned_position	= mul( m0, float4(input.position, 1.f) ).xyz;
	float3 skinned_normal	= normalize( mul( (float3x3)m0, normal ) );

#if CONFIG_LOD_INDEX==0
	float3 skinned_tangent	= normalize( mul( (float3x3)m0, tangent ) );
	float3 skinned_binormal	= normalize( mul( (float3x3)m0, binormal ) );
	float3 prev_position	= mul( prev_bones_matrices[ input.bones_indices[0] ], float4(input.position, 1.f) ).xyz;
#endif // #if CONFIG_LOD_INDEX==0
#endif

#if CONFIG_LOD_INDEX!=0
	float3 skinned_tangent	= tangent;
	float3 skinned_binormal	= binormal;
	float3 prev_position	= skinned_position;
#endif // #if CONFIG_LOD_INDEX!=0

	vsf_tangent_space_transform	(skinned_normal, skinned_tangent, skinned_binormal, output.tbn_x, output.tbn_y, output.tbn_z);

	float3 position				= mul(m_W,  float4( skinned_position, 1.f));
	float3 view_position		= mul(m_V,  float4( position, 1.f));

	output.prev_position		= prev_position;
	output.position				= position;
	output.homogeneous_position	= mul( m_P,  float4( view_position, 1.f));
	output.view_position		= view_position;
	output.tc 					= input.tc;
}

#elif CONFIG_VERTEX_INPUT_TYPE==7 || CONFIG_VERTEX_INPUT_TYPE==8
//////////////////////////////////////////////////////////////////////////
// particle (8 = subuv)
//////////////////////////////////////////////////////////////////////////

float3   up_view_vector;
float3   right_view_vector;

float	 use_align_by_dir;
float3   view_location;

float	 use_fixed_axis;
float3   rotation_fixed_axis;

float locked_no_ratate_axis_index;
// up, right
static const float3 locked_axis[6][2] =
{
	{float3(0,1,0), float3(0,0,-1)},
	{float3(0,0,1), float3(1,0,0)},
	{float3(0,1,0), float3(-1,0,0)},

	{float3(0,1,0), float3(0,0,1)},
	{float3(0,0,-1), float3(1,0,0)},
	{float3(0,1,0), float3(1,0,0)},
};

struct vertex_input_struct
{
	float3	position			: POSITION;
	float4	color				: TEXCOORD0;
	float2	tc					: TEXCOORD1;
	float2	size				: TEXCOORD2;
	float	rotation			: TEXCOORD3;
	float	frame				: TEXCOORD4;
	float3	prev_position		: TEXCOORD5;
#if CONFIG_USE_SUBUV
	float2	size_tc				: TEXCOORD6;
	float4	blend_tc			: TEXCOORD7;
#endif // #if CONFIG_USE_SUBUV
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float3	normal				: NORMAL;
	float4	color				: COLOR;
	float2	tc					: TEXCOORD1;
	float3  tangent				: TEXCOORD2;
	float3  binormal			: TEXCOORD3;
#if CONFIG_USE_SUBUV
	float4	tc1					: TEXCOORD4;
#endif // #if CONFIG_USE_SUBUV
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 position 			= input.position;

	float3 up_result;
	float3 right_result;

	//BRANCH
	if (locked_no_ratate_axis_index>=0.0f)
	{
		int index				= floor(locked_no_ratate_axis_index);
		up_result				= locked_axis[index][0];
		right_result			= locked_axis[index][1];
	}
	else
	{
		float sine_rotation, cosine_rotation;
		sincos(input.rotation, sine_rotation, cosine_rotation);

		float3 right_rotated	= cosine_rotation * right_view_vector.xyz - sine_rotation * up_view_vector.xyz;
		float3 up_rotated		= sine_rotation * right_view_vector.xyz   + cosine_rotation * up_view_vector.xyz;

		float3 view_direction  	= normalize(view_location.xyz - position);

		float3 particle_direction 	= normalize(position - input.prev_position);

		up_result				= up_rotated;
		right_result 			= right_rotated;

		if (use_align_by_dir>0.0f)
		{
			up_result 			= particle_direction;
			right_result 		= normalize(cross(view_direction, particle_direction));
		}

		if (use_fixed_axis>0.0f)
		{
			up_result			= rotation_fixed_axis.xyz;
			right_result		= normalize(cross(view_direction, rotation_fixed_axis.xyz));
		}
	}

#if CONFIG_USE_SUBUV
	position    				+= (-input.size_tc.y + 0.5) * input.size.y * up_result     +
							    (input.size_tc.x - 0.5) * input.size.x * right_result;
	output.tc1		  			= input.blend_tc;
#else
	position    				+= (-input.tc.y + 0.5) * input.size.y * up_result     +
							    (input.tc.x - 0.5) * input.size.x * right_result;
#endif // #if CONFIG_USE_SUBUV

	output.tangent				= normalize(right_result);
	output.binormal				= normalize(up_result);
	output.normal				= cross(up_result, right_result);

	output.homogeneous_position = mul(m_VP, float4(position, 1.0f));

	output.color	  			= input.color;
	output.position				= position;
	output.tc		  			= input.tc;
}

#elif CONFIG_VERTEX_INPUT_TYPE==9
//////////////////////////////////////////////////////////////////////////
// particle beamtrail
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
	float4	color				: TEXCOORD0;
	float2	tc					: TEXCOORD1;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float4	color				: TEXCOORD1;
	float2	tc					: TEXCOORD2;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.position				= mul( m_W,  float4(input.position, 1.f));
	output.tc 					= input.tc;
	output.color				= input.color;
	output.homogeneous_position	= mul(m_WVP,  float4(input.position, 1.f));
}

#elif CONFIG_VERTEX_INPUT_TYPE==11
//////////////////////////////////////////////////////////////////////////
// grass mesh
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
	float4	normal				: NORMAL;
	float4	tangent				: TANGENT;
	float4	binormal			: BINORMAL;
	float2	tc					: TEXCOORD0;
	float4	object_position		: OBJECT_POSITON;
	float4	color_and_wind		: COLOR_AND_WIND;
};

struct vertex_output_struct
{
	float3	tbn_x				: TEXCOORD0;
	float3	tbn_y				: TEXCOORD1;
	float3	tbn_z				: TEXCOORD2;
	float4	color				: TEXCOORD3;
	float4  homogeneous_position: SV_Position;
	float4	homogeneous_position_copy	: TEXCOORD4;
	float3	view_position		: TEXCOORD5;
	float3	position			: TEXCOORD6;
	float2	tc					: TEXCOORD7;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	float3 normal				= unpack_normal( unpack_D3DCOLOR( input.normal.xyz));
	float3 tangent				= unpack_normal( unpack_D3DCOLOR( input.tangent.xyz));
	float3 binormal				= unpack_normal( unpack_D3DCOLOR( input.binormal.xyz));

	float3x3 TBN				= mul( (float3x3)m_V,
									transpose(	float3x3(
													tangent.xyz,
													binormal,
													normal.xyz)) );

	output.tbn_x				= TBN[0];
	output.tbn_y				= TBN[1];
	output.tbn_z				= TBN[2];

	output.color				= float4(1.0f, 1.0f, 1.0f, 1.0f);

	float2 f1					= fmod( input.position.xz * 400.0f, 1.0f);
	float2 f2					= fmod( input.position.xz * 0.4f, 1.0f);

	f1							= exp2( (f1 * 0.25f + 3.0f) * float2( 3.203671f, 2.832576f));
	f2							= exp2( (f2 * 0.25f + 3.0f) * float2( 3.203671f, 2.832576f));

	float2 phase				= frac( float2(f2.x, f1.x) * float2(f2.y, f1.y)) - float2( 0.5f, 0.25f);
	float phase1				= phase.x;
	float phase2				= phase.y;

	float vertical_scale		= (phase1 * 0.5f + 0.5f) * 2.0f;

	float wind_strength			= phase2 * wind_info_parameters.z;

	float height				= input.object_position.y * 0.25f;
	float dist					= length(input.object_position.xyz) * height;

	float term1					= wind_strength * dist;
	float term2					= dist * wind_info_parameters.z;

	float wind_power			= input.color_and_wind.w;

	float phase_time			= phase2 * wind_power * scene_time;
	float spatial				= phase2 * 500.0f;

	float3 world_object			= mul(m_W, float4(input.object_position.xyz, 1.f));

	float xarg					= spatial * world_object.x;
	float zarg					= spatial * world_object.z + phase_time;

	float c						= cos((world_object.z * 0.81f + scene_time) * wind_power);
	term2						= term2 * (c * c);

	term1						= term1 * (sin(phase_time * 16.0f + xarg) + 0.75f);
	term1						= term1 * abs(cos(zarg));

	term1						= term1 * 0.5f;
	term2						= term2 * 0.5f;

	float2 wind_direction		= normalize(wind_info_parameters.xy);

	float3 offset;
	offset.xz					= term1 * wind_direction + term2 * wind_direction;
	offset.y					= -(vertical_scale * length(offset.xz));

	float3 position				= offset + input.position;

	float3 view_position		= mul(m_V,  float4( position, 1.f));

	output.position				= position;

	float4 homogeneous_position	= mul( m_P,  float4( view_position, 1.f));

	output.view_position		= view_position;
	output.homogeneous_position	= homogeneous_position;
	output.homogeneous_position_copy	= homogeneous_position;
	output.tc					= input.tc;
}

#elif CONFIG_VERTEX_INPUT_TYPE==12
//////////////////////////////////////////////////////////////////////////
// post process
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.position				= mul(m_WV,  float4(input.position, 1.f));
	output.homogeneous_position	= mul(m_WVP,  float4(input.position, 1.f));
}

#elif CONFIG_VERTEX_INPUT_TYPE==13
//////////////////////////////////////////////////////////////////////////
// wires
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
	float3	normal				: NORMAL;
	float3	tangent				: TANGENT;
	float3	binormal			: BINORMAL;
	float2	tc					: TEXCOORD0;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float2	tc					: TEXCOORD1;
	float3	tbn_x				: TEXCOORD2;
	float3	tbn_y				: TEXCOORD3;
	float3	tbn_z				: TEXCOORD4;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.position				= mul(m_W,  float4(input.position, 1.f));
	output.tc 					= input.tc;
	output.homogeneous_position	= mul(m_WVP,  float4(input.position, 1.f));

	vsf_tangent_space_transform	(input.normal, input.tangent, input.binormal, output.tbn_x, output.tbn_y, output.tbn_z);
}

#elif CONFIG_VERTEX_INPUT_TYPE==14
//////////////////////////////////////////////////////////////////////////
// user
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
	float2	tc					: TEXCOORD0;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
	float2	tc					: TEXCOORD1;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.position				= mul(m_W,  float4(input.position, 1.f));
	output.tc					= input.tc;
	output.homogeneous_position	= mul(m_WVP,  float4(input.position, 1.f));
}

#else // NULL_VERTEX_INPUT_TYPE (0), decal (10)
//////////////////////////////////////////////////////////////////////////
// null mesh / decal
//////////////////////////////////////////////////////////////////////////

struct vertex_input_struct
{
	float3	position			: POSITION;
};

struct vertex_output_struct
{
	float4  homogeneous_position: SV_Position;
	float3	position			: TEXCOORD0;
};

void fill_output_vertex(in vertex_input_struct input, out vertex_output_struct output)
{
	output.position				= mul(m_W, float4(input.position, 1.f));
	output.homogeneous_position = mul(m_WVP, float4(input.position, 1.f));
}

#endif

void main (in vertex_input_struct input,
		   out vertex_output_struct output)
{
	fill_output_vertex(input, output);
}
