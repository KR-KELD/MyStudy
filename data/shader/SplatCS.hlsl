struct BufType
{
	float2 vPickPos;	
	float2 vRadius;
	int    iIndex;  //알파 마스크
	float  fTexWidth;
	float  fTexHeight;
};
//원본을 복사한 텍스쳐
Texture2D<float4>			CopyMap   : register(t0);
//구조화 버퍼(피킹 지점과 범위 등의 정보를 포함)
StructuredBuffer<BufType>   SelectMap : register(t1);
//변경된 텍스쳐
RWTexture2D<float4>			OutputMap  : register(u0);

#define size_x 32
#define size_y 32

[numthreads(size_x, size_y,1)]
void CS( uint3 GroupID : SV_GroupID,
         uint3 DispatchThreadID : SV_DispatchThreadID,
		 uint3 GroupThreadID : SV_GroupThreadID,
		 uint  GroupIndex : SV_GroupIndex)
{
	//텍스쳐 좌표를 디바이스 쓰레드 번호로 받는다
	int3 texturelocation = int3(0,0,0);
	texturelocation.x = DispatchThreadID.x;
	texturelocation.y = DispatchThreadID.y;	
	float2 txPos = texturelocation.xy;
	//현재 텍스처 좌표와 피킹지점 거리
	float1 fDistance = distance(txPos, SelectMap[0].vPickPos);
	//복사본의 같은 텍스처 위치의 텍스처 컬러 로드
	float4 vAlphaColor = CopyMap.Load(texturelocation);
	//외부원 반지름과 거리 비교
	if(fDistance < SelectMap[0].vRadius.x)
	{		
		float fDot = 1.0f-(fDistance/SelectMap[0].vRadius.x);
		//내부원 반지름과 거리 비교
		if (fDistance < SelectMap[0].vRadius.y)
		{
			fDot = 1.0f;
		}
		else
		{
			float1 fRatio = (fDistance - SelectMap[0].vRadius.y) / 
				(SelectMap[0].vRadius.x -  SelectMap[0].vRadius.y);
			fDot = lerp(1.0f,0.0f,fRatio);
		}
		//알파 마스크 맵에 선택한 스플래팅 텍스쳐값을 넣는다
		switch(SelectMap[0].iIndex)
		{
		case 0 : vAlphaColor.x = max(vAlphaColor.x, fDot);break;
		case 1 : vAlphaColor.y = max(vAlphaColor.y, fDot);break;
		case 2 : vAlphaColor.z = max(vAlphaColor.z, fDot);break;
		case 3 : vAlphaColor.w = max(vAlphaColor.w, fDot);break;
		case 99 :
		{
			vAlphaColor = float4(0.0f,0.0f,0.0f,0.0f);
		} break;
		}
		OutputMap[texturelocation.xy] = vAlphaColor;
	}		
}
