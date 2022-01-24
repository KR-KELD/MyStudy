// HLSL
#define MAX_BONE_MATRICES 255

cbuffer myDataCB : register(b0)
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12);
	float  fTime  : packoffset(c13.x);
	float  fGameTime  : packoffset(c13.y);
};

cbuffer cbAnimMatrices : register (b1)
{
	matrix m_matConstBoneWorld[MAX_BONE_MATRICES];
};
matrix FetchBoneTransform(uint iBone)
{
	matrix mret;
	mret = m_matConstBoneWorld[iBone];
	return mret;
}

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
};
VS_OUTPUT VS( VS_INPUT vIn)
{
	VS_OUTPUT vOut;
	float4 p = float4(vIn.p,1.0f);
	p = mul(p, g_matWorld);
	vOut.wp = p;
	p = mul(p, g_matView);
	vOut.p = mul(p, g_matProj);
	// dot
	vOut.n = vIn.n;
	//�����彦�̵�
	float fdot = dot(-vColor.xyz, vIn.n);
	vOut.c = float4(fdot, fdot, fdot, 1.0f);
	vOut.t = vIn.t;	
	return vOut;
}