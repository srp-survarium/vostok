struct VS_INPUT
{
    float2  Pos : POSITION0;
    float2	Tex	: TEXCOORD0;
};

struct VS_OUTPUT
{
    half2	Tex	: TEXCOORD0;
	half4	Pos : POSITION;	
};

VS_OUTPUT main( VS_INPUT In)
{                                                                             
    VS_OUTPUT Out;
    
	Out.Tex = In.Tex;
	Out.Pos = half4(In.Pos, 0, 1);
	    
    return Out;                                                               
}                                                                           