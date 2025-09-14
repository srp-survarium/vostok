struct	GBufferData
{
	float3  Position;
	float   Mtl;
	float3  Normal;
	float   Hemi; // AO
	float3  Color;
	float   Gloss;
};

#ifndef GBUFFER_OPTIMIZATION

struct	f_deffer
{
	half4	PositionMtl	: COLOR0;        // Px, Py, Pz, Mtl-ID
	half4	NormalHemi	: COLOR1;        // Nx, Ny, Nz, Hemi
	half4	ColorGloss	: COLOR2;        // R, G, B,  Gloss
};

f_deffer GBufferPack (half4 Position, half4 Normal, half4 Color)
{
	f_deffer Result;

	Result.PositionMtl	= Position;
	Result.NormalHemi	= Normal;
	Result.ColorGloss	= Color;

	return Result;
}

GBufferData GBufferUnpack(float2 TC : TEXCOORD, float2 Pos2D)
{
	GBufferData GData;

	float4 PositionMtl = tex2D(s_position, TC);

	GData.Position	= PositionMtl.xyz;
	GData.Mtl		= PositionMtl.w;

	float4 NormalHemi	= tex2D(s_normal, TC);

	GData.Normal	= NormalHemi.xyz;
	GData.Hemi		= NormalHemi.w;

	float4	ColorGloss	= tex2D(s_diffuse, TC);

	GData.Color		= ColorGloss.xyz;
	GData.Gloss		= ColorGloss.w;

	return GData;
}

GBufferData GBufferUnpackWithOffset(float2 tc : TEXCOORD, float2 OffsetTC : TEXCOORD, float2 pos2d)
{
	return GBufferUnpack(OffsetTC, 0);
}

#else

struct	f_deffer
{
	half4	PackedPosLProps	: COLOR0;        // Pz, Mtl-ID, Hemi
	half4	PackedNormal   	: COLOR1;        // Packed Nx, Packed Ny
	half4	Color          	: COLOR2;        // R, G, B,  Gloss
};

half2 GBufferPackPosition (float Depth)
{
	half2 Result;
	
	float DepthRange = 1.0; //FIX ME!!! Maybe z_near - z_far???? or its postprojection space?
	float DepthValue = Depth * (256.0 / DepthRange);
	Result = float2(floor(DepthValue) / 256.0, frac(DepthValue));
	return Result;
}

half4 GBufferPackNormal (half4 Normal)
{
	half4 Result;

	Result.rg = 0.5 * (Normal.xy + float2(1, 1));
	Result.b = Normal.w;
	Result.a = (Normal.z > 0 ? 1.0 : 0.0);

	return Result;
}

f_deffer GBufferPack (half4 Position, half4 Normal, half4 Color)
{
	f_deffer Result;

	Result.PackedPosLProps	= half4(Position.zw, 0.0, 0.0)/*GBufferPackLProps(Position.w, Normal.w)*/;
	Result.PackedNormal		= GBufferPackNormal(Normal);
	Result.Color			= Color;

	return Result;
}

float3 GBufferUnpackNormal (float4 Normal)
{
	float3 Result;

	Result.xy = (2.0 * abs(Normal.rg)) - float2(1, 1);
	Result.z = (Normal.a > 0.0 ? 1.0 : -1.0) * sqrt(abs(1 - Result.x * Result.x - Result.y * Result.y));

	return Result;
}

float3 GBufferUnpackPosition (float Depth, float2 Pos2D)
{
	float3 Result;
	
	// 3d view space pos reconstruction math
	// center of the plane (0,0) or (0.5,0.5) at distance 1 is eyepoint(0,0,0) + lookat (assuming |lookat| ==1
	// left/right = (0,0,1) -/+ tan(fHorzFOV/2) * (1,0,0 ) 
	// top/bottom = (0,0,1) +/- tan(fVertFOV/2) * (0,1,0 )
	// lefttop		= ( -tan(fHorzFOV/2),  tan(fVertFOV/2), 1 )
	// righttop		= (  tan(fHorzFOV/2),  tan(fVertFOV/2), 1 )
	// leftbottom   = ( -tan(fHorzFOV/2), -tan(fVertFOV/2), 1 )
	// rightbottom	= (  tan(fHorzFOV/2), -tan(fVertFOV/2), 1 )
	Result = float3(Depth * (Pos2D * pos_decompression_params.zw - pos_decompression_params.xy), Depth);

	return Result;
}

GBufferData GBufferUnpack(float2 TC : TEXCOORD, float2 Pos2D)
{
	GBufferData GData;

	float2	PositionLProps	= tex2D(s_position, TC);

	GData.Position  = GBufferUnpackPosition(PositionLProps.r, Pos2D);
	GData.Mtl		= PositionLProps.g;//GBufferUnpackMtl(PositionLProps.y);

	float4	PackedNormal	= tex2D(s_normal, TC);

	GData.Normal	= GBufferUnpackNormal(PackedNormal);
	GData.Hemi		= PackedNormal.b;//GBufferUnpackHemi(PositionLProps.y);

	float4	ColorData		= tex2D(s_diffuse, TC);

	GData.Color		= ColorData.xyz;
	GData.Gloss		= ColorData.w;

	return GData;
}

GBufferData GBufferUnpackWithOffset(float2 tc : TEXCOORD, float2 OffsetTC : TEXCOORD, float2 pos2d)
{
	float2  delta	  = ( ( OffsetTC - tc ) * pos_decompression_params2.xy );

	return GBufferUnpack(OffsetTC, pos2d + delta);
}

#endif