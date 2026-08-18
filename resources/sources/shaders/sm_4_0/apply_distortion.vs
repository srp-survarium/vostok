////////////////////////////////////////////////////////////////////////////
//	Created		: 05.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "common.h"

// The shipped blob is a pure pass-through: the quad already arrives in clip
// space, so there is no screen_res viewport transform (the $Globals/
// static_globals cbuffers are wholly unreferenced and fxc drops them), and
// the vertex carries no COLOR - the ISGN is exactly POSITION0 + TEXCOORD0.
// Note the SV_POSITION spelling: the signature chunk keeps it verbatim, and
// common_iostructs.h's v2p_TL spells it SV_Position.
struct	v_distortion
{
	float4	P		: POSITION;
	float2	Tex0	: TEXCOORD0;
};

struct	v2p_distortion
{
	float4 	HPos	: SV_POSITION;	// Clip-space position 	(for rasterization)
	float2 	Tex0	: TEXCOORD0;
};

v2p_distortion main ( v_distortion I )
{
	v2p_distortion O;

	O.HPos = I.P;
	O.Tex0 = I.Tex0;

	return O;
}
