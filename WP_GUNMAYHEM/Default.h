#pragma once
// <TCP/IP>=====================================================================
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000

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
#include <array>

#define         VK_MAX            0xff
#include <map>
#include <algorithm>

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

struct vec2 { float x = 0.f; float y = 0.f; };

struct PlayerInfo {
	vec2			vPosition;					// 위치
	PLAYER_STATE	eState = STATE_NONE;		// 현재 상태
	ITEMTYPE		eItemType = ITEM_NONE;		// 현재 무기
	int				iBullet = 0;				// 현재 총알 개수
	int				iLife = 0;					// 체력
	bool			isConnected = false;		// 연결 상태

	int looking = 1; // 0: Left, 1: Right
};

struct BulletInfo {
	vec2			vStarting;					// 시작 위치
	vec2			vPosition;					// 현재 위치
	ITEMTYPE		eType;						// 총알 타입 (사거리 계산용)
	RECT			colBox;						// 충돌 박스
	BOOL			exist = FALSE;				// 삭제 플래그
	int				id = -1;                    // 소유자 ID
};

struct ItemBoxInfo {
	vec2			vPosition;					// 위치
	RECT			colBox;						// 충돌 박스
	BOOL			exist = FALSE;				// 삭제 플래그
};

// CLIENT -> SERVER ------------------------------------------------------------
enum PLAYER_INPUT_KEY { KEY_NONE, KEY_LEFT, KEY_RIGHT, KEY_JUMP, KEY_DOWNJUMP, KEY_SHOOT };
struct Player_input {
	bool isDown;
	PLAYER_INPUT_KEY key = KEY_NONE;
	int id;
};

// SERVER -> CLIENT ------------------------------------------------------------
struct SendData {
	PlayerInfo				playerInfo[3];		// 플레이어 정보
	std::array<BulletInfo, 100>	arrBullets;			// 총알 정보
	std::array<ItemBoxInfo, 10> arrItemBoxs;		// 아이템 박스 정보
	bool isChanged = false;
};