#pragma once
#include <windows.h>
#include <iostream>

#pragma pack(push,1)
//패킷 헤더
typedef struct
{
	uint16_t iotype; // 패킷 입출력 종류
	uint16_t len;	// MSG 바이트 크기
	uint16_t type;	// 어떤 태킷이야
	uint32_t time;	// 패킷전송 시간
}PACKET_HEADER;
#define PACKET_HEADER_SIZE 10
//패킷
typedef struct
{
	PACKET_HEADER	ph;			//헤더
	int8_t			msg[1024];	//메시지
}UPACKET, *P_UPACKET;

//채팅 메시지 구조체
struct TChatMsg
{
	int  iCnt;			//채팅 카운트
	char szName[32];	//유저 이름
	char buffer[128];	//채팅 메시지 버퍼
};
//로그인 구조체
struct TLogin
{
	char szID[16];	//아이디
	char szPS[16];	//비밀번호
};
#define TLoginSize sizeof(TLogin)
//로그인 결과 구조체
struct TLoginResult
{
	int   iRet; //0:falid, 1:succeed, 2
};
typedef struct
{
	uint16_t	Point;				// 미분배 포인트.
	uint16_t	STR;				// 힘.
	uint16_t	INT;				// 지능.
	uint16_t	DEX;				// 민첩.
	uint16_t	CHA;				// 카리스마.
	uint16_t	Life;				// 현재 생명력. (힘 종속)
	uint16_t	Stamina;			// 현재 지구력. (힘 종속)
	int8_t		Status;				// 현재 상태 값.
	uint16_t	Skins[6];			// 현재 모습 상태.(0:머리, 1:얼굴)
	int8_t		Parts[4];			// 부위별 피해 상황. 
	int8_t		Level;				// 레벨.
	uint32_t	Exp;				// 경험치.
	uint32_t	Skills[16];			// 스킬 숙련도. (0->10,000)

}TSTATUS_VARS;

//------------------------------------------------------------------------------
typedef struct
{
	int8_t		Rank;				// 계급.
	int8_t		Medals;				// 획득한 훈장 수.
	uint16_t	Fame;				// 명성 수치.
	uint32_t	Win;				// 승.
	uint32_t	Lose;				// 패.

}TSCORE_VARS;
#pragma pack(pop)
//패킷 타입
enum TPACKET_TYPE
{
	PACKET_LOGIN_REQ = 10,
	PACKET_LOGIN_ACK,
	PACKET_CHAT_MSG = 100,
	PACKET_TSTATUS_VARS,
};