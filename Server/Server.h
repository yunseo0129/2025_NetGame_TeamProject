#pragma once
// <TCP/IP>=====================================================================
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <queue>

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

#define SERVERPORT 9000
#define BUFSIZE 4096
#define MAX_PLAYERS 3

enum ITEMTYPE { ITEM_NONE, ITEM_PISTOL, ITEM_SNIPER };
enum PLAYER_STATE { STATE_NONE, STATE_IDLE, STATE_WALK, STATE_JUMP };
enum PLAYER_ACTION { ACTION_NONE, ACTION_MOVE_L, ACTION_MOVE_R, ACTION_JUMP_UP, ACTION_JUMP_DOWN, ACTION_SHOOT };
struct vec2 { float x = 0.f; float y = 0.f; };

// 통신용----------------------------------------------------------------------------
struct PlayerInfo {
	vec2			vPosition;					// 위치
	PLAYER_STATE	eState = STATE_NONE;		// 현재 상태
	ITEMTYPE		eItemType = ITEM_NONE;		// 현재 무기
	int				iLife = 0;					// 체력
	bool			isConnected = false;		// 연결 상태
};

struct BulletInfo {
	vec2			vStarting;					// 시작 위치
	vec2			vPosition;					// 현재 위치
	ITEMTYPE		eType;						// 총알 타입 (사거리 계산용)
};

struct ItemBoxInfo {
	vec2			vPosition;					// 위치
};

// 관리용----------------------------------------------------------------------------
struct Player {
	SOCKET			socket;						// 소켓
	PlayerInfo		info;						// 플레이어 정보
	vec2			vDirPow;					// 현재 받고있는 가속도 벡터
	CollisionBox	rtBox;						// 충돌 박스
};

struct Bullet {
	BulletInfo      vInfo;						// 총알 정보
	bool			isDead = false;				// 삭제 플래그
};

struct ItemBox {
	vec2			vPosition;					// 위치
	ITEMTYPE		eItemType;					// 아이템 타입
	bool			isDead = false;				// 삭제 플래그
};

struct CollisionBox {
	RECT			rtBox;						// 충돌 박스
};

struct Action {
	PLAYER_ACTION	eAct;
	int				iPlayerNum;
};

// SERVER -> CLIENT ------------------------------------------------------------------
struct SendData {
	PlayerInfo				playerInfo[3];		// 플레이어 정보
	int						iBulletCount;		// 총알 개수
	int						iItemBoxCount;		// 아이템 박스 개수
	std::vector<BulletInfo>	vecBullets;			// 총알 정보
	std::vector<ItemBoxInfo> vecItemBoxs;		// 아이템 박스 정보
};
// SERVER -> CLIENT ------------------------------------------------------------------

// CLIENT -> SERVER ------------------------------------------------------------------
struct PlayerInput {
	PLAYER_ACTION eAction = ACTION_NONE; // 플레이어 액션
};
// CLIENT -> SERVER ------------------------------------------------------------------


// 동기화를 위한 전역 크리티컬 섹션
CRITICAL_SECTION g_cs_Inputs;
CRITICAL_SECTION g_World_CS;

// 클라이언트 스레드에 소켓, ID 전달을 위한 구조체
struct ThreadParam {
	SOCKET		hClientSock;
	int			iPlayerID;
};
