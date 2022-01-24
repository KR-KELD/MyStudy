// HLSL
static const int g_iNumTexture = 6;
Texture2D g_txDiffuse;// : register(t0);
Texture2D g_txDiffuseArray[g_iNumTexture] : register(t1);
SamplerState		g_Sample : register(s0);
SamplerState        g_samPointClamp : register(s1);
SamplerState        g_samLinearWrap : register(s2);

#define g_iNumLight 3
#define HALF_VECTOR

cbuffer myDataCB : register(b0)
{
	float4x4  g_matWorld: packoffset(c0);
	float4x4  g_matView: packoffset(c4);
	float4x4  g_matProj: packoffset(c8);
	float4 vColor : packoffset(c12.x);
	float  fTime  : packoffset(c13.x);
	float  fGameTime  : packoffset(c13.y);
};
//--------------------------------------------------------------------------------------
//Lighting Variables
//--------------------------------------------------------------------------------------
cbuffer cb1: register(b1)
{
						//������Ʈ�� �����Ʈ����Ʈ(�ֺ���)������
	float4				g_cAmbientMaterial[g_iNumLight];
						//������Ʈ�� ��ǻ�����Ʈ(Ȯ�걤) ������
	float4				g_cDiffuseMaterial[g_iNumLight];
						//������Ʈ�� ����ŧ������Ʈ(�ݻ籤) ������
	float4				g_cSpecularMaterial[g_iNumLight];
						//������Ʈ�� ���̼Ƕ���Ʈ(���Ɽ-�޽ð� ������) ������
	float4				g_cEmissionMaterial[g_iNumLight];

	float4				g_cAmbientLightColor[g_iNumLight];
	float4				g_cDiffuseLightColor[g_iNumLight];
	float4				g_cSpecularLightColor[g_iNumLight];
};
//--------------------------------------------------------------------------------------
//Lighting Variables
//--------------------------------------------------------------------------------------
cbuffer cb2: register(b2)
{
	matrix				g_matWorldInverse[g_iNumLight];
	float4				g_vLightDir[g_iNumLight];	
	float4				g_vLightPos[g_iNumLight];	
	float4				g_vEyeDir[g_iNumLight];	
	float4				g_vEyePos[g_iNumLight];	
};
//--------------------------------------------------------------------------------------
//Lighting Variables
//--------------------------------------------------------------------------------------
cbuffer cb3: register(b3)
{	
	float4		g_vSpotInfo[g_iNumLight];
	//float		g_fTheta[g_iNumLight] : packoffset(c0.x);		// ���� ���� ���� ����	
	//float		g_fPhi[g_iNumLight] : packoffset(c0.y);			// �ܺ� ���� ���� ����
	//float		g_fFalloff[g_iNumLight] : packoffset(c0.z);		// ���� �ܰ� �ܺ� ���� �ֵ�( Luminance )	
	//float     g_nNumLights[g_iNumLight] : packoffset(c0.w);	// �Ÿ�
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float4 wp : TEXCOORD1;
	uint PrimitiveID : SV_PrimitiveID;
};
struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};
float4 Diffuse(float3 vNormal, float4 vLightDir)
{
	//���� ����� �ȼ��� �븻���� �������� ���� ���⸦ ���
	float fIntensity = max(0, dot(vNormal, normalize(-vLightDir.xyz)));
	float4 diffuse = float4(1,1,1,1)* fIntensity;
	diffuse.w = 1.0f;
	return diffuse;
}
float4 Specular(float3 vNormal, float4 vLightDir,
					float4 vEyeDir)
{
	float  fPower = 0.0f;
	
#ifndef HALF_VECTOR
	//�ݻ� ���͸� ��� 
	float3 R = reflect(vLightDir.xyz, vNormal);
	//���� ���⸦ ����ϰ� 0~1������ ������ش��� �ݻ����� vEyeDir.w ������ �ŵ����� ���ش�
	fPower = pow(saturate(dot(R, -vEyeDir)), vEyeDir.w);
#else
	//�������͸� ���
	float3 vHalf = normalize(-vLightDir.xyz + -vEyeDir.xyz);
	fPower = pow(saturate(dot(vNormal, vHalf)), vEyeDir.w);
#endif
	float4 specular = float4(1,1,1,1) * fPower;
	specular.w = 1.0f;
	return specular;
}
float4 ComputePointLight( float3 vVertexPos, float3 vVertexNormal, int nNumLights )
{
	float4 vPointLightColor= float4(0,0,0,1);
	for( int iLight = 0; iLight < nNumLights; iLight++ )
	{		
		float4 vLight;
		//�ȼ��� ����Ʈ ���������� ����Ʈ ����Ʈ�� ������ ���
		vLight.xyz	= normalize( vVertexPos - g_vLightPos[iLight].xyz );
		//�ȼ��� ����Ʈ�� �Ÿ��� ���
		vLight.w	= distance( vVertexPos, g_vLightPos[iLight].xyz );
		//���� �߽����κ��� �־������� ������ ���踦 �ش�
		float fLuminance = smoothstep( vLight.w-5,  vLight.w, g_vLightPos[iLight].w);
		//����Ʈ�� ������ŭ ���� �ջ�
		vPointLightColor += Diffuse(vVertexNormal, vLight) *  fLuminance;
	}			
	vPointLightColor.w =1.0f;
	return vPointLightColor;
}
float4 ComputeSpotLight(float3 vVertexPos,	float3 vVertexNormal,	int nNumLights)
{	
	float4 vSpotLightColor = float4(0, 0, 0, 1);
	for (int iLight = 0; iLight < nNumLights; iLight++)
	{		
		//�������� ����
		float fInner = g_vSpotInfo[iLight].x;
		//�ܺ����� ����
		float fOutner = g_vSpotInfo[iLight].y;
		//�ܺ��ܰ� �������� �ֵ�
		float fFalloff = g_vSpotInfo[iLight].z;
		//����
		float fRange = g_vSpotInfo[iLight].w;
		float4 vLight;
		//�ȼ��� ����Ʈ�� ������ ���
		vLight.xyz = normalize(vVertexPos - g_vLightPos[iLight].xyz);
		//�ȼ��� ����Ʈ�� �Ÿ��� ���
		vLight.w = distance(vVertexPos, g_vLightPos[iLight].xyz);
		//����Ʈ���� �ȼ��������� ����� ���� ������ ������ ���
		float fDot = max(0.0f, dot(vLight.xyz, g_vLightDir[iLight].xyz));
		fDot = acos(fDot) * (180.0f / 3.141592f);
		//�ܺ����� �������� ������
		if( fDot < fOutner) 
		{
			//���� ���⸦ ������ �������� �����Ų�� �ּҰ��� 0.3f��
			//������ �������� ������ �ִ�ġ
			float fIntensity = max(0.3f,1.0f-smoothstep(fInner, fOutner, fDot));
			//���� �ջ�
			vSpotLightColor += 
			Diffuse(vVertexNormal, vLight)+Specular(vVertexNormal, vLight,g_vEyeDir[iLight] )*fIntensity;
		}
		else
		{
			vSpotLightColor += g_cAmbientLightColor[iLight];
		}
	}
	vSpotLightColor.w = 1.0f;
	return vSpotLightColor;
}
float4 Rim( VS_OUTPUT vIn)
{
	float fRimLightcolor = smoothstep(1.0f - vIn.wp.w, 1.0f, 
							1.0f - max(0, dot(vIn.n, vIn.wp.xyz)));
	float4 vFinalColor = float4(fRimLightcolor, fRimLightcolor, fRimLightcolor, 1.0f);	
	return vFinalColor;
}
float Geom(VS_OUTPUT vIn)
{
	//�������͸� ���
	float3 vReflect = normalize(-g_vLightDir[0] + -g_vEyeDir[0]);
	//�Ϻ꺤�Ϳ� �ȼ��� �븻���� ������ ����?
	float fDot  = pow(saturate(dot(vIn.n, vReflect)), 1);

	float NdotL = saturate(dot(vIn.n, -g_vLightDir[0]));
	float NdotH = saturate(dot(vIn.n, vReflect));
	float NdotV = saturate(dot(vIn.n, -g_vEyeDir[0]));
	float VdotH = saturate(dot(-g_vEyeDir[0], vReflect));

	float Geom = min(min(2 * (NdotH*NdotV / VdotH), 2 * (NdotH*NdotL / VdotH)), 1.0f) / max(0.1f, NdotV);
	return Geom;
}

PS_OUTPUT PS( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	float4 vTexture = g_txDiffuse.Sample( g_Sample, vIn.t);
	float4 lightColor = ComputeSpotLight(vIn.wp, vIn.n, 3);	
	vOut.c = Diffuse(vIn.n, g_vLightDir[0])*Geom(vIn)*lightColor;
	return vOut;
}
PS_OUTPUT PS_Default( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	float4 vTexture = g_txDiffuse.Sample( g_Sample, vIn.t);
	vOut.c = vTexture;
	return vOut;
}
PS_OUTPUT PSLine( VS_OUTPUT vIn)
{
	PS_OUTPUT vOut;
	vOut.c = vIn.c;
	return vOut;
}

float4 GetTextureColor(uint index, float2 uv)
{
	float4 fColor = float4(1,1,1,1);
	switch (index)
	{
	case 0: fColor = g_txDiffuseArray[0].Sample(g_Sample, uv); break;
	case 1: fColor = g_txDiffuseArray[1].Sample(g_Sample, uv); break;
	case 2: fColor = g_txDiffuseArray[2].Sample(g_Sample, uv); break;
	case 3: fColor = g_txDiffuseArray[3].Sample(g_Sample, uv); break;
	case 4: fColor = g_txDiffuseArray[4].Sample(g_Sample, uv); break;
	case 5: fColor = g_txDiffuseArray[5].Sample(g_Sample, uv); break;
	}	
	return fColor;
}
float4 PS_TextureIndex(VS_OUTPUT vIn) : SV_Target
{
	uint index = vIn.PrimitiveID / 2;
	float4 fColor = float4(1, 1, 1, 1);
	fColor = GetTextureColor(index, vIn.t);
	return fColor;	
}