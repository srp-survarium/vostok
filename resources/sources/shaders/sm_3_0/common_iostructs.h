#ifndef	common_iostructs_h_included
#define	common_iostructs_h_included

////////////////////////////////////////////////////////////////
//	This file contains io structs:
//	v_name	:	input for vertex shader.
//	v2p_name:	output for vertex shader.
//	p_name	:	input for pixel shader.
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//	TL0uv
struct	v_TL0uv_positiont
{
	float4	P		: POSITION;
	float4	Color	: COLOR; 
};

struct	v_TL0uv
{
	float4	P		: POSITION;
	float4	Color	: COLOR; 
};

struct	v2p_TL0uv
{
	float4	Color	: COLOR;
	float4 	HPos	: POSITION;	// Clip-space position 	(for rasterization)
};

struct	p_TL0uv
{
	float4	Color	: COLOR;
//	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
};


struct 	v2p_volume
{
	float4 	tc		:TEXCOORD0;
#ifdef 	USE_SJITTER
	float4 	tcJ		:TEXCOORD1;
#endif
	float4 	hpos	:SV_Position;	// Clip-space position 	(for rasterization)
};
struct 	p_volume
{
	float4 	tc		:TEXCOORD0;
#ifdef 	USE_SJITTER
	float4 	tcJ		:TEXCOORD1;
#endif
//	float4 	hpos	:SV_Position;	// Clip-space position 	(for rasterization)
};

////////////////////////////////////////////////////////////////
//	TL
struct	v_TL_positiont
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
	float4	Color	: COLOR; 
};

struct	v_TL
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
	float4	Color	: COLOR; 
};

struct	v2p_TL
{
	float2 	Tex0	: TEXCOORD0;
	float4	Color	: COLOR;
	float4 	HPos	: POSITION;	// Clip-space position 	(for rasterization)
};

struct	p_TL
{
	float2 	Tex0	: TEXCOORD0;
	float4	Color	: COLOR;
//	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
};

////////////////////////////////////////////////////////////////
//	TL2uv
struct	v_TL2uv
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4	Color	: COLOR; 
};

struct	v2p_TL2uv
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4	Color	: COLOR;
	float4 	HPos	: POSITION;	// Clip-space position 	(for rasterization)
};

struct	p_TL2uv
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4	Color	: COLOR;
	float2 	Pos		: VPOS;	// Clip-space position 	(for rasterization)
};
////////////////////////////////////////////////////////////////
//	postpr
struct	v_postpr
{
	float4	P		: POSITION;
	float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
	float2	Tex1	: TEXCOORD1;	// base2 (duality)
	float2	Tex2	: TEXCOORD2;	// base  (noise)
	float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
	float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
};

struct	v2p_postpr
{
	float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
	float2	Tex1	: TEXCOORD1;	// base2 (duality)
	float2	Tex2	: TEXCOORD2;	// base  (noise)
	float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
	float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
	float4 	HPos	: POSITION;	// Clip-space position 	(for rasterization)
};

struct	p_postpr
{
	float2 	Tex0	: TEXCOORD0;	// base1 (duality)	
	float2	Tex1	: TEXCOORD1;	// base2 (duality)
	float2	Tex2	: TEXCOORD2;	// base  (noise)
	float4	Color	: COLOR0;		// multiplier, color.w = noise_amount
	float4	Gray	: COLOR1;		// (.3,.3,.3.,amount)
//	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
};
////////////////////////////////////////////////////////////////
//	build	(bloom_build)
struct	v_build
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float2 	Tex2	: TEXCOORD2;
	float2	Tex3	: TEXCOORD3;
};

struct	v2p_build
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float2 	Tex2	: TEXCOORD2;
	float2	Tex3	: TEXCOORD3;
	float4 	HPos	: POSITION;	// Clip-space position 	(for rasterization)
};

struct	p_build
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float2 	Tex2	: TEXCOORD2;
	float2	Tex3	: TEXCOORD3;
//	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
};
////////////////////////////////////////////////////////////////
//	filter	(bloom_filter)
struct	v_filter
{
	float4	P		: POSITION;
	float4 	Tex0	: TEXCOORD0;
	float4	Tex1	: TEXCOORD1;
	float4 	Tex2	: TEXCOORD2;
	float4	Tex3	: TEXCOORD3;
	float4 	Tex4	: TEXCOORD4;
	float4	Tex5	: TEXCOORD5;
	float4 	Tex6	: TEXCOORD6;
	float4	Tex7	: TEXCOORD7;
};

struct	v2p_filter
{
	float4 	Tex0	: TEXCOORD0;
	float4	Tex1	: TEXCOORD1;
	float4 	Tex2	: TEXCOORD2;
	float4	Tex3	: TEXCOORD3;
	float4 	Tex4	: TEXCOORD4;
	float4	Tex5	: TEXCOORD5;
	float4 	Tex6	: TEXCOORD6;
	float4	Tex7	: TEXCOORD7;
	float4 	HPos	: POSITION;	// Clip-space position 	(for rasterization)
};

struct	p_filter
{
	float4 	Tex0	: TEXCOORD0;
	float4	Tex1	: TEXCOORD1;
	float4 	Tex2	: TEXCOORD2;
	float4	Tex3	: TEXCOORD3;
	float4 	Tex4	: TEXCOORD4;
	float4	Tex5	: TEXCOORD5;
	float4 	Tex6	: TEXCOORD6;
	float4	Tex7	: TEXCOORD7;
//	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
};

////////////////////////////////////////////////////////////////
//	aa_AA
struct	v_aa_AA
{
	float4 P		:POSITION;
	float2 	Tex0	:TEXCOORD0;
	float2	Tex1	:TEXCOORD1;
	float2 	Tex2	:TEXCOORD2;
	float2	Tex3	:TEXCOORD3;
	float2	Tex4	:TEXCOORD4;
	float4	Tex5	:TEXCOORD5;
	float4	Tex6	:TEXCOORD6;
};

struct	v2p_aa_AA
{
	float2 	Tex0	:TEXCOORD0;
	float2	Tex1	:TEXCOORD1;
	float2 	Tex2	:TEXCOORD2;
	float2	Tex3	:TEXCOORD3;
	float2	Tex4	:TEXCOORD4;
	float4	Tex5	:TEXCOORD5;
	float4	Tex6	:TEXCOORD6;
	float4 	HPos	:POSITION;	// Clip-space position 	(for rasterization)
};

////////////////////////////////////////////////////////////////
//	dumb
struct 	v_dumb
{
	float4	P		:POSITION;	// Clip-space position 	(for rasterization)
};

struct 	v2p_dumb
{
	float4	HPos	:POSITION;	// Clip-space position 	(for rasterization)
};

#endif	//	common_iostructs_h_included