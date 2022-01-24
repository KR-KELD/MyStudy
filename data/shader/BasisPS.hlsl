// HLSL
Texture2D g_txDiffuse;// : register(t0);
SamplerState g_Sample;// : register(s0);

cbuffer myDataCB
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12.x);
	float  fTime  : packoffset(c13.x);
	float  fGameTime  : packoffset(c13.y);
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};

PS_OUTPUT PS( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	float4 vTexture = g_txDiffuse.Sample( g_Sample, vIn.t);
	float3 Light = float3(-1.0f,-1.0f,-1.0f);
	Light = normalize(Light);
	float ratio = saturate(dot(-Light,vIn.n));
	vOut.c = vTexture;// *vIn.c * ratio;
	return vOut;
}

PS_OUTPUT PSLine( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	vOut.c = vIn.c;
	return vOut;
}