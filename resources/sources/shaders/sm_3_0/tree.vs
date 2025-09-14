struct	v_vert
{
	float4 	P		: POSITION;
	float4	N		: NORMAL;
	float4 	T		: TANGENT;
	float4 	B		: BINORMAL;
};

struct vf
{
	float4 hpos	: POSITION;
	float4 c0	: COLOR0;
};

uniform float4x4 	m_WVP;

vf main(v_vert v)
{
	vf o;
	o.hpos 		= mul(m_WVP, v.P);

	half4 n = normalize(v.N);
	o.c0 	= float4(0.3, 0.5, 0.8, 1)*dot(n, normalize(float4(2, 1, 2, 0)));
	return o;
};