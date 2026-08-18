uniform float	grid_size;
uniform float3	light_position;

struct v2g
{
	float4	hpos		: POSITION;
	float3	normal		: NORMAL;
	float3	flux		: COLOR;
	float3	world		: TEXCOORD0;
};

struct g2p
{
	float4	hpos		: SV_Position;
	float3	normal		: NORMAL;
	float3	flux		: COLOR;
	float3	world		: TEXCOORD0;
	uint	slice		: SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]
void main( triangle v2g input[3], inout TriangleStream<g2p> stream )
{
	for (int i = 0; i < 3; i++)
	{
		g2p		output;

		float2	clamped		= saturate( input[i].hpos.xy);

		output.slice		= (uint)floor( grid_size*input[i].hpos.z);
		output.hpos			= float4( clamped.x*2.0f - 1.0f, ( 1.0f - clamped.y)*2.0f - 1.0f, 0.0f, 1.0f);
		output.normal		= input[i].normal;
		output.flux			= input[i].flux;
		output.world		= input[i].world;

		stream.Append( output);
	}
	stream.RestartStrip();
}
