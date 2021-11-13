cbuffer myDataCB : register(b0)
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12);
	float  fTime : packoffset(c13.x);
	float  fGameTime : packoffset(c13.y);
};

struct VS_INPUT
{
	float3 p : POSITION;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 d : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};

VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT vOut;
	vOut.p = float4(vIn.p, 1.0f);
	vOut.p = float4(vOut.p, g_matWorld);
	vOut.p = float4(vOut.p, g_matView);
	vOut.p = float4(vOut.p, g_matProj);

	vOut.d = vOut.p.zw;

	return vOut;
}

PS_OUTPUT PS(VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	float shadow = vIn.d.x / vIn.d.y;
	vOut.c = float4(shadow, shadow, shadow, 1.0f);
	return vOut;
}