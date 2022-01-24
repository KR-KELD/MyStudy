// HLSL
cbuffer myDataCB : register(b0)
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12);
	float  fTime  : packoffset(c13.x);
	float  fGameTime  : packoffset(c13.y);
};

struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float4 wp : TEXCOORD1;
	float3 tex : TEXCOORD2;
};
VS_OUTPUT VS( VS_INPUT vIn)
{
	VS_OUTPUT vOut;
	float4 p = float4(vIn.p,1.0f);
	p = mul(p, g_matWorld);
	vOut.wp = p;
	p = mul(p, g_matView);
	float4 fProj = mul(p, g_matProj);
	vOut.p = fProj.xyww;
	// dot
	float fDot = dot(-vColor.xyz,vIn.n);
	vOut.c = float4(fDot,fDot,fDot,1.0f);
	vOut.t = vIn.t;	
	vOut.tex = normalize(vIn.p);
	return vOut;
}