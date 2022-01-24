//�⺻ ConstantBuffer
cbuffer myDataCB : register(b0)
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12);
	float  fTime  : packoffset(c13.x);
	float  fGameTime  : packoffset(c13.y);
};
//���� ������ ��� ConstantBuffer
cbuffer myMapCB : register(b1)
{
	float g_iNumTile : packoffset(c0.x);
	float g_iNumCell : packoffset(c0.y);
	float g_iCellSize : packoffset(c0.z);
	float g_iSpaceDivision : packoffset(c0.w);
};
//�׸����� ��Ʈ���� ������ ��� ConstantBuffer
cbuffer cbShadowMatrix : register (b2)
{
	matrix g_matShadow : packoffset(c0);
};

//�⺻ ������ ��ǲ,�ƿ�ǲ ���̾ƿ�
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
	float2 nt : TEXCOORD1;
	float4 s : TEXCOORD2;
};
//�׸��� ������ �ƿ�ǲ ���̾ƿ�
struct VS_SOUTPUT
{
	float4 p : SV_POSITION;
	float2 d : TEXCOORD0;
	float4 c : TEXCOORD1;
};
//���� Ÿ�� ������ VertexShader
VS_OUTPUT VS( VS_INPUT vIn)
{
	VS_OUTPUT vOut;
	float4 p = float4(vIn.p,1.0f);
	p = mul(p, g_matWorld);
	
	vOut.s = mul(p, g_matShadow);

	vOut.nt = vIn.t;
	//Ÿ�� ������
	vOut.t.x = ((p.x / g_iCellSize) + (g_iNumTile * g_iNumCell) / 2.0f) / g_iNumCell;
	vOut.t.y = 1.0f - ((p.z / g_iCellSize) + (g_iNumTile * g_iNumCell) / 2.0f) / g_iNumCell;

	p = mul(p, g_matView);
	vOut.p = mul(p, g_matProj);
	vOut.n = vIn.n;
	vOut.c = vIn.c;

	return vOut;
}
//���� �׸��� ������ VertexShader
VS_SOUTPUT VS_Shadow(VS_INPUT vIn)
{

	VS_SOUTPUT vOut = (VS_SOUTPUT)0;
	vOut.p = float4(vIn.p, 1.0f);
	
	vOut.p = mul(vOut.p, g_matWorld);
	vOut.p = mul(vOut.p, g_matView);
	vOut.p = mul(vOut.p, g_matProj);
	vOut.d = vOut.p.zw;
	//�������� ����
	float depth = vOut.p.z * 1.0f / (1000.0f - 1.0f) + -1.0f / (1000.0f - 1.0f);
	vOut.c = float4(depth, depth, depth, 1);
	return vOut;
}