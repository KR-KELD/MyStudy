#define DEPTH_TEX_SIZE 1024
#define PCF_KERNEL 3

//��ǻ�� �ؽ���
Texture2D g_txDiffuse : register(t0);
//��� �ؽ���
Texture2D g_txNormalMap : register(t1);
//����������� �ؽ���
Texture2D g_txShadowMap : register(t2);
//���ø�
SamplerState g_Sample : register(s0);
SamplerState g_SamplerClamp : register(s1);
SamplerComparisonState g_SamplerCom : register(s2);
cbuffer myDataCB
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12);
	float  fTime  : packoffset(c13.x);
	float  fGameTime  : packoffset(c13.y);
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 light : TEXCOORD1;
	float4 s : TEXCOORD2;
};

struct VS_SOUTPUT
{
	float4 p : SV_POSITION;
	float2 d : TEXCOORD0;
	float4 c : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};

PS_OUTPUT PS( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	//��ָ���
	float4 normal =	g_txNormalMap.Sample( g_Sample, vIn.t * 10);
	normal = normalize( (normal - 0.5f) * 2.0f );			

	float  fDot	= saturate( dot( normal.xyz, -vIn.light ));
	float fLightAmount = 0.0f;
	float3 vShadowProj = vIn.s.xyz / vIn.s.w;
	const float fdelta = 1.0f / DEPTH_TEX_SIZE;
	int iHalf = (PCF_KERNEL - 1) / 2;
	for (int v = -iHalf; v <= iHalf; v++)
	{
		for (int u = -iHalf; u <= iHalf; u++)
		{
			float2 vOffset = float2(u*fdelta, v*fdelta);	
			fLightAmount += g_txShadowMap.SampleCmpLevelZero(g_SamplerCom, 
									vShadowProj.xy+vOffset, vShadowProj.z - 0.0013f).r;
		}											
	}	
	fLightAmount /= PCF_KERNEL * PCF_KERNEL;

	float4 vFinalColor = g_txDiffuse.Sample(g_Sample, vIn.t);
	vFinalColor = vFinalColor * max(0.5f,fLightAmount) * fDot;
	vFinalColor.w = 1.0f;
	vOut.c = vFinalColor;
	return vOut;

	//float3 vShadowProj;
	//vShadowProj.xyz = vIn.s.xyz / vIn.s.w;
	////vShadowProj.z = vIn.s.z * 1.0f / (1000.0f - 1.0f) - 1.0f / (1000.0f - 1.0f);
	//float shadow = g_txShadowMap.Sample(g_SamplerClamp,vShadowProj.xy);
	//float4 vFinalColor = g_txDiffuse.Sample(g_Sample, vIn.t);
	//if (shadow <= vShadowProj.z)
	//{
	//	fDot = min(fDot, 0.5f);
	//}
	//vFinalColor *= fDot;
	//vFinalColor.a = 1.0f;







	//float3 Light = normalize(vColor.xyz);
	//float c = dot(-Light,vIn.n);
	//float4 vLightColor = float4(c,c,c,1);
	//float4 vAmbientLight = float4(0.3f, 0.3f, 0.3f,1.0f);
	//vTexture.a = 1.0f;
	//vOut.c = vFinalColor; // vFinalColor * (vAmbientLight + vLightColor);
	//return vOut;
}

PS_OUTPUT PSLine( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	vOut.c = vIn.c;
	return vOut;
}

PS_OUTPUT PS_Shadow(VS_SOUTPUT vIn)
{
	PS_OUTPUT vOut;
	//float shadow = vIn.d.x / vIn.d.y;
	//vOut.c = float4(shadow, shadow, shadow, 1.0f);
	vOut.c = float4(vIn.d.x, vIn.d.x, vIn.d.x, 1.0f);
	vOut.c = vIn.c;
	return vOut;
}