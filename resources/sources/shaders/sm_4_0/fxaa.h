////////////////////////////////////////////////////////////////////////////
//	Reconstructed 2026-08-17 from the shipped v0.100b blob
//	(shaders/sm_4_0/post_process_fxaa.ps, plain permutation). The blob's
//	dependency list names resources.sources/shaders/sm_4_0/fxaa.h
//	(mtime 2013-05-03), absent from the recovered source drop - the drop
//	carries only the older fxaa_common.h generation. Byte-identity of the
//	one shader that includes this file is the acceptance test:
//	python3 -m vostok.shaders roundtrip post_process_fxaa.ps
//
//	This is NVIDIA FXAA 3.11 (FXAA_PC / FXAA_HLSL_4 quality path), trimmed
//	to what the blob proves was compiled: the console/360/PS3 paths and the
//	unused preset tables are not reconstructable from the blob and are left
//	out. Two provable local edits against stock 3.11:
//	* FxaaLuma() computes a luminance dot product
//	  dot(rgb, (0.299, 0.587, 0.114)) - the blob dp3's every neighbour
//	  sample - instead of stock's "luma in alpha" / "green as luma" channel
//	  read;
//	* lumaM stays the stock GREEN_AS_LUMA define (rgbyM.y), so the centre
//	  pixel's luma is its raw green channel while every neighbour gets the
//	  dot product - the blob really does mix the two.
//	Only FXAA_QUALITY__PRESET 39 is blob-proven (search steps
//	1,1,1,1,1,1.5,2,2,2,2,4,8), so only its table is carried.
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_FXAA_H_INCLUDED
#define SHADERS_SM_4_0_FXAA_H_INCLUDED

/*============================================================================
                             INTEGRATION KNOBS
============================================================================*/
#ifndef FXAA_PC
	#define FXAA_PC 0
#endif
#ifndef FXAA_HLSL_4
	#define FXAA_HLSL_4 0
#endif
#ifndef FXAA_GREEN_AS_LUMA
	#define FXAA_GREEN_AS_LUMA 0
#endif
#ifndef FXAA_DISCARD
	#define FXAA_DISCARD 0
#endif
#ifndef FXAA_GATHER4_ALPHA
	#define FXAA_GATHER4_ALPHA 0
#endif

/*============================================================================
                        FXAA QUALITY - TUNING KNOBS
============================================================================*/
#ifndef FXAA_QUALITY__PRESET
	#define FXAA_QUALITY__PRESET 39
#endif

/*============================================================================
                     FXAA QUALITY - EXTREME QUALITY
============================================================================*/
#if (FXAA_QUALITY__PRESET == 39)
	#define FXAA_QUALITY__PS 12
	#define FXAA_QUALITY__P0 1.0
	#define FXAA_QUALITY__P1 1.0
	#define FXAA_QUALITY__P2 1.0
	#define FXAA_QUALITY__P3 1.0
	#define FXAA_QUALITY__P4 1.0
	#define FXAA_QUALITY__P5 1.5
	#define FXAA_QUALITY__P6 2.0
	#define FXAA_QUALITY__P7 2.0
	#define FXAA_QUALITY__P8 2.0
	#define FXAA_QUALITY__P9 2.0
	#define FXAA_QUALITY__P10 4.0
	#define FXAA_QUALITY__P11 8.0
#endif

/*============================================================================
                                API PORTING
============================================================================*/
#define FxaaBool bool
#define FxaaDiscard clip(-1)
#define FxaaFloat float
#define FxaaFloat2 float2
#define FxaaFloat3 float3
#define FxaaFloat4 float4
#define FxaaHalf half
#define FxaaHalf2 half2
#define FxaaHalf3 half3
#define FxaaHalf4 half4
#define FxaaSat(x) saturate(x)

#if (FXAA_HLSL_4 == 1)
	#define FxaaInt2 int2
	struct FxaaTex { SamplerState smpl; Texture2D tex; };
	#define FxaaTexTop(t, p) t.tex.SampleLevel(t.smpl, p, 0.0)
	#define FxaaTexOff(t, p, o, r) t.tex.SampleLevel(t.smpl, p, 0.0, o)
#endif

/*============================================================================
                   GREEN AS LUMA OPTION SUPPORT FUNCTION
============================================================================*/
FxaaFloat FxaaLuma(FxaaFloat4 rgba)
{
	return dot(rgba.xyz, FxaaFloat3(0.299, 0.587, 0.114));
}

/*============================================================================
                             FXAA3 QUALITY - PC
============================================================================*/
#if (FXAA_PC == 1)
/*--------------------------------------------------------------------------*/
FxaaFloat4 FxaaPixelShader(
	//
	// Use noperspective interpolation here (turn off perspective interpolation).
	// {xy} = center of pixel
	FxaaFloat2 pos,
	//
	// Input texture.
	FxaaTex tex,
	//
	// Only used on FXAA Quality.
	// This must be from a constant/uniform.
	// {x_} = 1.0/screenWidthInPixels
	// {_y} = 1.0/screenHeightInPixels
	FxaaFloat2 fxaaQualityRcpFrame,
	//
	// Only used on FXAA Quality.
	// This used to be the FXAA_QUALITY__SUBPIX define.
	// It is here now to allow easier tuning.
	// Choose the amount of sub-pixel aliasing removal.
	FxaaFloat fxaaQualitySubpix,
	//
	// Only used on FXAA Quality.
	// This used to be the FXAA_QUALITY__EDGE_THRESHOLD define.
	// The minimum amount of local contrast required to apply algorithm.
	FxaaFloat fxaaQualityEdgeThreshold,
	//
	// Only used on FXAA Quality.
	// This used to be the FXAA_QUALITY__EDGE_THRESHOLD_MIN define.
	// Trims the algorithm from processing darks.
	FxaaFloat fxaaQualityEdgeThresholdMin
) {
/*--------------------------------------------------------------------------*/
	FxaaFloat2 posM;
	posM.x = pos.x;
	posM.y = pos.y;
	FxaaFloat4 rgbyM = FxaaTexTop(tex, posM);
	#if (FXAA_GREEN_AS_LUMA == 0)
		#define lumaM rgbyM.w
	#else
		#define lumaM rgbyM.y
	#endif
	FxaaFloat lumaS = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2( 0, 1), fxaaQualityRcpFrame.xy));
	FxaaFloat lumaE = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2( 1, 0), fxaaQualityRcpFrame.xy));
	FxaaFloat lumaN = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2( 0,-1), fxaaQualityRcpFrame.xy));
	FxaaFloat lumaW = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2(-1, 0), fxaaQualityRcpFrame.xy));
/*--------------------------------------------------------------------------*/
	FxaaFloat maxSM = max(lumaS, lumaM);
	FxaaFloat minSM = min(lumaS, lumaM);
	FxaaFloat maxESM = max(lumaE, maxSM);
	FxaaFloat minESM = min(lumaE, minSM);
	FxaaFloat maxWN = max(lumaN, lumaW);
	FxaaFloat minWN = min(lumaN, lumaW);
	FxaaFloat rangeMax = max(maxWN, maxESM);
	FxaaFloat rangeMin = min(minWN, minESM);
	FxaaFloat rangeMaxScaled = rangeMax * fxaaQualityEdgeThreshold;
	FxaaFloat range = rangeMax - rangeMin;
	FxaaFloat rangeMaxClamped = max(fxaaQualityEdgeThresholdMin, rangeMaxScaled);
	FxaaBool earlyExit = range < rangeMaxClamped;
/*--------------------------------------------------------------------------*/
	if(earlyExit)
		#if (FXAA_DISCARD == 1)
			FxaaDiscard;
		#else
			return rgbyM;
		#endif
/*--------------------------------------------------------------------------*/
	FxaaFloat lumaNW = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2(-1,-1), fxaaQualityRcpFrame.xy));
	FxaaFloat lumaSE = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2( 1, 1), fxaaQualityRcpFrame.xy));
	FxaaFloat lumaNE = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2( 1,-1), fxaaQualityRcpFrame.xy));
	FxaaFloat lumaSW = FxaaLuma(FxaaTexOff(tex, posM, FxaaInt2(-1, 1), fxaaQualityRcpFrame.xy));
/*--------------------------------------------------------------------------*/
	FxaaFloat lumaNS = lumaN + lumaS;
	FxaaFloat lumaWE = lumaW + lumaE;
	FxaaFloat subpixRcpRange = 1.0/range;
	FxaaFloat subpixNSWE = lumaNS + lumaWE;
	FxaaFloat edgeHorz1 = (-2.0 * lumaM) + lumaNS;
	FxaaFloat edgeVert1 = (-2.0 * lumaM) + lumaWE;
/*--------------------------------------------------------------------------*/
	FxaaFloat lumaNESE = lumaNE + lumaSE;
	FxaaFloat lumaNWNE = lumaNW + lumaNE;
	FxaaFloat edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
	FxaaFloat edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
/*--------------------------------------------------------------------------*/
	FxaaFloat lumaNWSW = lumaNW + lumaSW;
	FxaaFloat lumaSWSE = lumaSW + lumaSE;
	FxaaFloat edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
	FxaaFloat edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
	FxaaFloat edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
	FxaaFloat edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
	FxaaFloat edgeHorz = abs(edgeHorz3) + edgeHorz4;
	FxaaFloat edgeVert = abs(edgeVert3) + edgeVert4;
/*--------------------------------------------------------------------------*/
	FxaaFloat subpixNWSWNESE = lumaNWSW + lumaNESE;
	FxaaFloat lengthSign = fxaaQualityRcpFrame.x;
	FxaaBool horzSpan = edgeHorz >= edgeVert;
	FxaaFloat subpixA = subpixNSWE * 2.0 + subpixNWSWNESE;
/*--------------------------------------------------------------------------*/
	if(!horzSpan) lumaN = lumaW;
	if(!horzSpan) lumaS = lumaE;
	if(horzSpan) lengthSign = fxaaQualityRcpFrame.y;
	FxaaFloat subpixB = (subpixA * (1.0/12.0)) - lumaM;
/*--------------------------------------------------------------------------*/
	FxaaFloat gradientN = lumaN - lumaM;
	FxaaFloat gradientS = lumaS - lumaM;
	FxaaFloat lumaNN = lumaN + lumaM;
	FxaaFloat lumaSS = lumaS + lumaM;
	FxaaBool pairN = abs(gradientN) >= abs(gradientS);
	FxaaFloat gradient = max(abs(gradientN), abs(gradientS));
	if(pairN) lengthSign = -lengthSign;
	FxaaFloat subpixC = FxaaSat(abs(subpixB) * subpixRcpRange);
/*--------------------------------------------------------------------------*/
	FxaaFloat2 posB;
	posB.x = posM.x;
	posB.y = posM.y;
	FxaaFloat2 offNP;
	offNP.x = (!horzSpan) ? 0.0 : fxaaQualityRcpFrame.x;
	offNP.y = ( horzSpan) ? 0.0 : fxaaQualityRcpFrame.y;
	if(!horzSpan) posB.x += lengthSign * 0.5;
	if( horzSpan) posB.y += lengthSign * 0.5;
/*--------------------------------------------------------------------------*/
	FxaaFloat2 posN;
	posN.x = posB.x - offNP.x * FXAA_QUALITY__P0;
	posN.y = posB.y - offNP.y * FXAA_QUALITY__P0;
	FxaaFloat2 posP;
	posP.x = posB.x + offNP.x * FXAA_QUALITY__P0;
	posP.y = posB.y + offNP.y * FXAA_QUALITY__P0;
	FxaaFloat subpixD = ((-2.0)*subpixC) + 3.0;
	FxaaFloat lumaEndN = FxaaLuma(FxaaTexTop(tex, posN));
	FxaaFloat subpixE = subpixC * subpixC;
	FxaaFloat lumaEndP = FxaaLuma(FxaaTexTop(tex, posP));
/*--------------------------------------------------------------------------*/
	if(!pairN) lumaNN = lumaSS;
	FxaaFloat gradientScaled = gradient * 1.0/4.0;
	FxaaFloat lumaMM = lumaM - lumaNN * 0.5;
	FxaaFloat subpixF = subpixD * subpixE;
	FxaaBool lumaMLTZero = lumaMM < 0.0;
/*--------------------------------------------------------------------------*/
	lumaEndN -= lumaNN * 0.5;
	lumaEndP -= lumaNN * 0.5;
	FxaaBool doneN = abs(lumaEndN) >= gradientScaled;
	FxaaBool doneP = abs(lumaEndP) >= gradientScaled;
	if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P1;
	if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P1;
	FxaaBool doneNP = (!doneN) || (!doneP);
	if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P1;
	if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P1;
/*--------------------------------------------------------------------------*/
	if(doneNP) {
/*--------------------------------------------------------------------------*/
		if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
		if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
		if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
		if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
		doneN = abs(lumaEndN) >= gradientScaled;
		doneP = abs(lumaEndP) >= gradientScaled;
		if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P2;
		if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P2;
		doneNP = (!doneN) || (!doneP);
		if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P2;
		if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P2;
/*--------------------------------------------------------------------------*/
		#if (FXAA_QUALITY__PS > 3)
		if(doneNP) {
/*--------------------------------------------------------------------------*/
			if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
			if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
			if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
			if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
			doneN = abs(lumaEndN) >= gradientScaled;
			doneP = abs(lumaEndP) >= gradientScaled;
			if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P3;
			if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P3;
			doneNP = (!doneN) || (!doneP);
			if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P3;
			if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P3;
/*--------------------------------------------------------------------------*/
			#if (FXAA_QUALITY__PS > 4)
			if(doneNP) {
/*--------------------------------------------------------------------------*/
				if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
				if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
				if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
				if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
				doneN = abs(lumaEndN) >= gradientScaled;
				doneP = abs(lumaEndP) >= gradientScaled;
				if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P4;
				if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P4;
				doneNP = (!doneN) || (!doneP);
				if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P4;
				if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P4;
/*--------------------------------------------------------------------------*/
				#if (FXAA_QUALITY__PS > 5)
				if(doneNP) {
/*--------------------------------------------------------------------------*/
					if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
					if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
					if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
					if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
					doneN = abs(lumaEndN) >= gradientScaled;
					doneP = abs(lumaEndP) >= gradientScaled;
					if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P5;
					if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P5;
					doneNP = (!doneN) || (!doneP);
					if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P5;
					if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P5;
/*--------------------------------------------------------------------------*/
					#if (FXAA_QUALITY__PS > 6)
					if(doneNP) {
/*--------------------------------------------------------------------------*/
						if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
						if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
						if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
						if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
						doneN = abs(lumaEndN) >= gradientScaled;
						doneP = abs(lumaEndP) >= gradientScaled;
						if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P6;
						if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P6;
						doneNP = (!doneN) || (!doneP);
						if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P6;
						if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P6;
/*--------------------------------------------------------------------------*/
						#if (FXAA_QUALITY__PS > 7)
						if(doneNP) {
/*--------------------------------------------------------------------------*/
							if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
							if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
							if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
							if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
							doneN = abs(lumaEndN) >= gradientScaled;
							doneP = abs(lumaEndP) >= gradientScaled;
							if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P7;
							if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P7;
							doneNP = (!doneN) || (!doneP);
							if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P7;
							if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P7;
/*--------------------------------------------------------------------------*/
							#if (FXAA_QUALITY__PS > 8)
							if(doneNP) {
/*--------------------------------------------------------------------------*/
								if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
								if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
								if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
								if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
								doneN = abs(lumaEndN) >= gradientScaled;
								doneP = abs(lumaEndP) >= gradientScaled;
								if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P8;
								if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P8;
								doneNP = (!doneN) || (!doneP);
								if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P8;
								if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P8;
/*--------------------------------------------------------------------------*/
								#if (FXAA_QUALITY__PS > 9)
								if(doneNP) {
/*--------------------------------------------------------------------------*/
									if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
									if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
									if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
									if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
									doneN = abs(lumaEndN) >= gradientScaled;
									doneP = abs(lumaEndP) >= gradientScaled;
									if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P9;
									if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P9;
									doneNP = (!doneN) || (!doneP);
									if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P9;
									if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P9;
/*--------------------------------------------------------------------------*/
									#if (FXAA_QUALITY__PS > 10)
									if(doneNP) {
/*--------------------------------------------------------------------------*/
										if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
										if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
										if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
										if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
										doneN = abs(lumaEndN) >= gradientScaled;
										doneP = abs(lumaEndP) >= gradientScaled;
										if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P10;
										if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P10;
										doneNP = (!doneN) || (!doneP);
										if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P10;
										if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P10;
/*--------------------------------------------------------------------------*/
										#if (FXAA_QUALITY__PS > 11)
										if(doneNP) {
/*--------------------------------------------------------------------------*/
											if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
											if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
											if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
											if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
											doneN = abs(lumaEndN) >= gradientScaled;
											doneP = abs(lumaEndP) >= gradientScaled;
											if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P11;
											if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P11;
											doneNP = (!doneN) || (!doneP);
											if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P11;
											if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P11;
/*--------------------------------------------------------------------------*/
											#if (FXAA_QUALITY__PS > 12)
											if(doneNP) {
/*--------------------------------------------------------------------------*/
												if(!doneN) lumaEndN = FxaaLuma(FxaaTexTop(tex, posN.xy));
												if(!doneP) lumaEndP = FxaaLuma(FxaaTexTop(tex, posP.xy));
												if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
												if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
												doneN = abs(lumaEndN) >= gradientScaled;
												doneP = abs(lumaEndP) >= gradientScaled;
												if(!doneN) posN.x -= offNP.x * FXAA_QUALITY__P12;
												if(!doneN) posN.y -= offNP.y * FXAA_QUALITY__P12;
												doneNP = (!doneN) || (!doneP);
												if(!doneP) posP.x += offNP.x * FXAA_QUALITY__P12;
												if(!doneP) posP.y += offNP.y * FXAA_QUALITY__P12;
/*--------------------------------------------------------------------------*/
											}
											#endif
/*--------------------------------------------------------------------------*/
										}
										#endif
/*--------------------------------------------------------------------------*/
									}
									#endif
/*--------------------------------------------------------------------------*/
								}
								#endif
/*--------------------------------------------------------------------------*/
							}
							#endif
/*--------------------------------------------------------------------------*/
						}
						#endif
/*--------------------------------------------------------------------------*/
					}
					#endif
/*--------------------------------------------------------------------------*/
				}
				#endif
/*--------------------------------------------------------------------------*/
			}
			#endif
/*--------------------------------------------------------------------------*/
		}
		#endif
/*--------------------------------------------------------------------------*/
	}
/*--------------------------------------------------------------------------*/
	FxaaFloat dstN = posM.x - posN.x;
	FxaaFloat dstP = posP.x - posM.x;
	if(!horzSpan) dstN = posM.y - posN.y;
	if(!horzSpan) dstP = posP.y - posM.y;
/*--------------------------------------------------------------------------*/
	FxaaBool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
	FxaaFloat spanLength = (dstP + dstN);
	FxaaBool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
	FxaaFloat spanLengthRcp = 1.0/spanLength;
/*--------------------------------------------------------------------------*/
	FxaaBool directionN = dstN < dstP;
	FxaaFloat dst = min(dstN, dstP);
	FxaaBool goodSpan = directionN ? goodSpanN : goodSpanP;
	FxaaFloat subpixG = subpixF * subpixF;
	FxaaFloat pixelOffset = (dst * (-spanLengthRcp)) + 0.5;
	FxaaFloat subpixH = subpixG * fxaaQualitySubpix;
/*--------------------------------------------------------------------------*/
	FxaaFloat pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
	FxaaFloat pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
	if(!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;
	if( horzSpan) posM.y += pixelOffsetSubpix * lengthSign;
	#if (FXAA_DISCARD == 1)
		return FxaaTexTop(tex, posM);
	#else
		return FxaaFloat4(FxaaTexTop(tex, posM).xyz, lumaM);
	#endif
}
/*==========================================================================*/
#endif // #if (FXAA_PC == 1)

#endif // #ifndef SHADERS_SM_4_0_FXAA_H_INCLUDED
