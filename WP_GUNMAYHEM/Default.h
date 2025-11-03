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

enum ACTION {
	ACTION_NONE,
	ACTION_MOVE_R,
	ACTION_MOVE_L,
	ACTION_JUMP_UP,
	ACTION_JUMP_DOWN,
	ACTION_FIRE
};
struct ClientPlay {
	ACTION eAction = ACTION_NONE;
};

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
extern HBITMAP BMP_player1_inform;
extern HBITMAP BMP_player2_inform;
extern HBITMAP BMP_player1_left_stand;
extern HBITMAP BMP_player1_left_walk[4];
extern HBITMAP BMP_player1_right_stand;
extern HBITMAP BMP_player1_right_walk[4];

extern HBITMAP BMP_player2_left_stand;
extern HBITMAP BMP_player2_left_walk[4];
extern HBITMAP BMP_player2_right_stand;
extern HBITMAP BMP_player2_right_walk[4];

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



//extern ITEMBOX item[MAX_ITEM];
//extern int item_count;
//extern MAPS maps[mapCount];