#pragma once
// <TCP/IP>=====================================================================
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000
#define BUFSIZE     4096
//#define FILE_NAME_MAX 256

//char* SERVERIP = (char*)"127.0.0.1";
//==============================================================================

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "fmod_vc.lib")

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>

#include "resource.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <list>
#include <vector>

#define         VK_MAX            0xff
#include <map>
#include <algorithm>

bool CheckRectCollision(const RECT& rect1, const RECT& rect2);

extern HWND g_hWnd;
extern int g_mapType;
extern HDC g_BMPmDC; // 더블버퍼링을 위한 mDC

// extern 키워드를 사용하여 전역 변수들을 선언(Declaration)으로 변경
constexpr int GUN_TYPE_PISTOL = 1;
constexpr int GUN_TYPE_SNIPE = 2;

extern HINSTANCE g_hInst;
extern LPCTSTR lpszClass;
extern LPCTSTR lpszWindowName;

extern int cameraWidth;
extern int cameraHeight;
extern int cameraX;   // 카메라의 좌상단 X 좌표
extern int cameraY;   // 카메라의 좌상단 Y 좌표

// 애니메이션 비트맵 핸들
extern HBITMAP BMP_player_inform[3];
extern HBITMAP BMP_player_left_stand[3];
extern HBITMAP BMP_player_left_walk[3][4];
extern HBITMAP BMP_player_right_stand[3];
extern HBITMAP BMP_player_right_walk[3][4];


extern HBITMAP BMP_gun1_left;
extern HBITMAP BMP_gun1_right;
extern HBITMAP BMP_gun2_left;
extern HBITMAP BMP_gun2_right;

extern HBITMAP BMP_itemBox;
extern HBITMAP BMP_startMenu;
extern HBITMAP BMP_pauseMenu;

extern HBITMAP BMP_map1;
extern HBITMAP BMP_map2;
extern HBITMAP BMP_ground1;
extern HBITMAP BMP_inform;

extern RECT rt;	//클라영역

//사운드 =====================================================================
extern FMOD::System* ssystem;
extern FMOD::Sound* sound1, * sound2, * clickSound, * itemSound;
extern FMOD::Channel* channel;
extern FMOD_RESULT result;
extern void* extradriverdata;

//==============================================================================
enum ITEMTYPE { ITEM_NONE, ITEM_PISTOL, ITEM_SNIPER };
enum PLAYER_STATE { STATE_NONE, STATE_IDLE, STATE_WALK, STATE_JUMP };

struct PLAYER_ACTION {
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool space = false;
};

struct vec2 { float x = 0.f; float y = 0.f; };

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
};

struct ItemBoxInfo {
	vec2			vPosition;					// 위치
};

// SERVER -> CLIENT ------------------------------------------------------------
struct SendData {
	PlayerInfo				playerInfo[3];		// 플레이어 정보
	int						iBulletCount;		// 총알 개수
	int						iItemBoxCount;		// 아이템 박스 개수
	std::vector<BulletInfo>	vecBullets;			// 총알 정보
	std::vector<ItemBoxInfo> vecItemBoxs;		// 아이템 박스 정보
};
// SERVER -> CLIENT ------------------------------------------------------------

// CLIENT -> SERVER ------------------------------------------------------------
//struct PlayerInput {
//	PLAYER_ACTION myAction; // 플레이어 액션
//};
// CLIENT -> SERVER ------------------------------------------------------------

constexpr int MAX_PLAYERS = 3;

// ==========================================================
// 1. (임시) PlayerState - 플레이어 한 명의 핵심 상태
// ==========================================================
struct PlayerState {
	bool  isConnected = false;
	float x = 0.f;
	float y = 0.f;
};

// ==========================================================
// 2. (임시) MovementData - 서버가 클라이언트에 보낼 이동 데이터 묶음
// ==========================================================
struct MovementData {
	PlayerState players[MAX_PLAYERS];
};