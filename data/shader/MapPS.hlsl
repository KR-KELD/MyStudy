//�������� �ؽ��� ������
#define DEPTH_TEX_SIZE 1024
//PCF ���� �ܰ�
#define PCF_KERNEL 3
//��ǻ�� �ؽ���
Texture2D g_txDiffuse : register(t0);
//����ũ�� �븻 �ؽ���
Texture2D g_txNormal : register(t1);
//���÷��� �ؽ��� 4��
Texture2D g_txSplat[4] : register(t2);
//������ �ؽ���
Texture2D g_txShadowMap : register(t6);
//���ø�
SamplerState g_Sample : register(s0);
SamplerState g_SamplerClamp : register(s1);
SamplerComparisonState g_SamplerCom : register(s2);

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
	float2 nt : TEXCOORD1;
	float4 s : TEXCOORD2;
};
struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};

struct VS_SOUTPUT
{
	float4 p : SV_POSITION;
	float2 d : TEXCOORD0;
	float4 c : TEXCOORD1;
};

PS_OUTPUT PS( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	float4 vTexture = g_txDiffuse.Sample( g_Sample, vIn.t);
	float4 vNormalTex = g_txNormal.Sample(g_Sample, vIn.nt);

	float4 vTex[4];
	vTex[0] = g_txSplat[0].Sample(g_Sample, vIn.t);
	vTex[1] = g_txSplat[1].Sample(g_Sample, vIn.t);
	vTex[2] = g_txSplat[2].Sample(g_Sample, vIn.t);
	vTex[3] = g_txSplat[3].Sample(g_Sample, vIn.t);
	//���÷��� �ؽ��� ��Ƽ �ؽ��ĸ�
	float3 vTexColor = vTexture.xyz * (1.0f - vNormalTex.r) + vTex[0].xyz * vNormalTex.r;
	vTexColor = vTexColor * (1.0f - vNormalTex.g) + vTex[1].xyz * vNormalTex.g;
	vTexColor = vTexColor * (1.0f - vNormalTex.b) + vTex[2].xyz * vNormalTex.b;
	vTexColor = vTexColor * (1.0f - vNormalTex.a) + vTex[3].xyz * vNormalTex.a;

	float4 vFinalColor = float4(vTexColor, 1.0f);
	//������ ��ġ
	float3 Light = float3(vColor.x,vColor.y,vColor.z);
	Light = normalize(Light);
	//�׸����� ����(����) ���
	float ratio = saturate(dot(vIn.n,-Light));

	float fLightAmount = 0.0f;
	//�������� ������ ����ȭ
	float3 vShadowProj = vIn.s.xyz / vIn.s.w;
	
	const float fdelta = 1.0f / DEPTH_TEX_SIZE;
	//���������κ��� PCF�� ������ ����
	int iHalf = (PCF_KERNEL - 1) / 2;
	
	for (int v = -iHalf; v <= iHalf; v++)
	{
		for (int u = -iHalf; u <= iHalf; u++)
		{
			//����� ���� �ؽ��� ��ǥ
			float2 vOffset = float2(u*fdelta, v*fdelta);	
			//�ֺ� ���� �÷��� �����Ͽ� ������ ���� ���⸦ ����
			//������ �������� �������� ���̰�
			fLightAmount += g_txShadowMap.SampleCmpLevelZero(g_SamplerCom, 
									vShadowProj.xy+vOffset, vShadowProj.z - 0.0013f).r;
		}											
	}	
	//������ ���� ���� ������� Ŀ���� ������ �����༭ ���� ���� ���⸦ ����
	fLightAmount /= PCF_KERNEL * PCF_KERNEL;
	vFinalColor = vFinalColor * max(0.5f,fLightAmount) * ratio;
	vFinalColor.w = 1.0f;
	vOut.c = vFinalColor;
	return vOut;
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
	vOut.c = float4(vIn.d.x,vIn.d.x,vIn.d.x,1.0f);
	vOut.c = vIn.c;
	return vOut;
}