//�� ��Ʈ���� �ִ� �ε���
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
//�ִϸ��̼� ��Ʈ���� ConstantBuffer
cbuffer cbAnimMatrices : register (b1)
{
	matrix m_matConstBoneWorld[MAX_BONE_MATRICES];
};
//�׸����� ��Ʈ���� ������ ��� ConstantBuffer
cbuffer cbShadowMatrix : register (b2)
{
	matrix g_matShadow : packoffset(c0);
};
//�ε����� �ش� �ִϸ��̼� ��Ʈ������ �ҷ��´�
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
	float3 tangent : TANGENT;
	float4 i : INDEX;
	float4 w : WEIGHT;
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

VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	float4 p = float4(vIn.p, 1.0f);
	float3 vNormal = vIn.n;
	float4x4 matAnim;
	float4 vLocal;
	//���� �����е��� ����ġ�� ������ ������ ��ְ��� ����
	for (int iBiped = 0; iBiped < 4; iBiped++)
	{
		uint iBoneIndex = (uint)vIn.i[iBiped];
		float fWeight = vIn.w[iBiped];

		vLocal = p;
		if (iBiped < 4)
		{
			matAnim = FetchBoneTransform(iBoneIndex);
			vOut.p += fWeight * mul(vLocal, matAnim);
			vOut.n += fWeight * mul(vNormal, (float3x3)matAnim);
		}
		else
		{
			iBoneIndex = (uint)vIn.i[iBiped];
			fWeight = vIn.w[iBiped];
			matAnim = FetchBoneTransform(iBoneIndex);
			vOut.p += fWeight * mul(vLocal, matAnim);
			vOut.n += fWeight * mul(vNormal, (float3x3)matAnim);
		}
	}
	vOut.p = mul(vOut.p, g_matWorld);
	vOut.s = mul(vOut.p, g_matShadow);
	vOut.p = mul(vOut.p, g_matView);
	vOut.p = mul(vOut.p, g_matProj);
	//�������� ����
	float depth = vOut.p.z * 1.0f / (1000.0f - 1.0f) + -1.0f / (1000.0f - 1.0f);
	vOut.c = float4(depth, depth, depth, 1);
	//���� ��ǥ���� ��ְ��� �ؽ��� ��ǥ��� ��ȯ
	vOut.n = normalize(mul(vOut.n, (float3x3)g_matWorld)); // g_matWorldInvTrans));
	//������ ���
	float3 T = normalize( mul( vIn.tangent, (float3x3)g_matWorld ));
	//�������� ���
	float3 B = normalize( cross( vOut.n, T ));
	//�������� ��Ʈ���� ����
	float3x3 tanMatrix = {T.x, B.x, vOut.n.x,
						  T.y, B.y, vOut.n.y,
						  T.z, B.z, vOut.n.z};
	vOut.light = normalize(mul(vColor.xyz, tanMatrix));
	vOut.t = vIn.t;	
	return vOut;
}

VS_SOUTPUT VS_Shadow(VS_INPUT vIn)
{
	VS_SOUTPUT vOut = (VS_SOUTPUT)0;
	float4 p = float4(vIn.p, 1.0f);
	float4x4 matMatrix;
	float4 vLocal;

	for (int iBiped = 0; iBiped < 4; iBiped++)
	{
		uint iBoneIndex = (uint)vIn.i[iBiped];
		float fWeight = vIn.w[iBiped];

		vLocal = p;
		if (iBiped < 4)
		{
			matMatrix = FetchBoneTransform(iBoneIndex);
			vOut.p += fWeight * mul(vLocal, matMatrix);
		}
		else
		{
			iBoneIndex = (uint)vIn.i[iBiped];
			fWeight = vIn.w[iBiped];
			matMatrix = FetchBoneTransform(iBoneIndex);
			vOut.p += fWeight * mul(vLocal, matMatrix);
		}
	}
	
	vOut.p = mul(vOut.p, g_matWorld);
	vOut.p = mul(vOut.p, g_matView);
	vOut.p = mul(vOut.p, g_matProj);
	vOut.d = vOut.p.zw;
	float depth = vOut.p.z * 1.0f / (1000.0f - 1.0f) + -1.0f / (1000.0f - 1.0f);
	vOut.c = float4(depth, depth, depth, 1);
	return vOut;
}