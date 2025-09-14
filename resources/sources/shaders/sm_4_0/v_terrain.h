struct v_terrain
{
	float2 position 		: POSITION;
};

void fill_v_common( v_terrain input, out v_common output)
{
	output.position	= float3( input.position, 0.f);

	output.normal		= float3( 0.f, 0.f, 0.f );
	output.tc			= float2( 0.f, 0.f );
	output.tc1			= float2( 0.f, 0.f );
	output.color		= float4( 0.f, 0.f, 0.f, 0.f );
	output.color1		= float4( 0.f, 0.f, 0.f, 0.f );
	output.tc_detail	= float2( 0.f, 0.f );
	output.tangent		= float3( 0.f, 0.f, 0.f );
	output.binormal		= float3( 0.f, 0.f, 0.f );
}

