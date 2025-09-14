struct quad_vs_input
{
    float2  Pos : POSITIONT;
    float2	Tex	: TEXCOORD0;
};

struct quad_vs2ps
{
    float2	Tex	: TEXCOORD0;
	float4	Pos : SV_POSITION;	
};

quad_vs2ps main( quad_vs_input In)
{                                                                             
    quad_vs2ps Out;
    
	Out.Tex = In.Tex;
	Out.Pos = float4(In.Pos, 0, 1);
	    
    return Out;                                                               
}                                                                           