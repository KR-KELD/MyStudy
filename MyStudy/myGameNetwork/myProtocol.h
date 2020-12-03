#pragma once
#include <windows.h>
#include <iostream>

#pragma pack(push,1)
//��Ŷ ���
typedef struct
{
	uint16_t iotype; // ��Ŷ ����� ����
	uint16_t len;	// MSG ����Ʈ ũ��
	uint16_t type;	// � ��Ŷ�̾�
	uint32_t time;	// ��Ŷ���� �ð�
}PACKET_HEADER;
#define PACKET_HEADER_SIZE 10
//��Ŷ
typedef struct
{
	PACKET_HEADER	ph;			//���
	int8_t			msg[1024];	//�޽���
}UPACKET, *P_UPACKET;

//ä�� �޽��� ����ü
struct TChatMsg
{
	int  iCnt;			//ä�� ī��Ʈ
	char szName[32];	//���� �̸�
	char buffer[128];	//ä�� �޽��� ����
};
//�α��� ����ü
struct TLogin
{
	char szID[16];	//���̵�
	char szPS[16];	//��й�ȣ
};
#define TLoginSize sizeof(TLogin)
//�α��� ��� ����ü
struct TLoginResult
{
	int   iRet; //0:falid, 1:succeed, 2
};
typedef struct
{
	uint16_t	Point;				// �̺й� ����Ʈ.
	uint16_t	STR;				// ��.
	uint16_t	INT;				// ����.
	uint16_t	DEX;				// ��ø.
	uint16_t	CHA;				// ī������.
	uint16_t	Life;				// ���� �����. (�� ����)
	uint16_t	Stamina;			// ���� ������. (�� ����)
	int8_t		Status;				// ���� ���� ��.
	uint16_t	Skins[6];			// ���� ��� ����.(0:�Ӹ�, 1:��)
	int8_t		Parts[4];			// ������ ���� ��Ȳ. 
	int8_t		Level;				// ����.
	uint32_t	Exp;				// ����ġ.
	uint32_t	Skills[16];			// ��ų ���õ�. (0->10,000)

}TSTATUS_VARS;

//------------------------------------------------------------------------------
typedef struct
{
	int8_t		Rank;				// ���.
	int8_t		Medals;				// ȹ���� ���� ��.
	uint16_t	Fame;				// �� ��ġ.
	uint32_t	Win;				// ��.
	uint32_t	Lose;				// ��.

}TSCORE_VARS;
#pragma pack(pop)
//��Ŷ Ÿ��
enum TPACKET_TYPE
{
	PACKET_LOGIN_REQ = 10,
	PACKET_LOGIN_ACK,
	PACKET_CHAT_MSG = 100,
	PACKET_TSTATUS_VARS,
};