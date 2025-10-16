#pragma once
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

//총알 =====================================================================
#define BULLET_SIZE 2	// 권총
#define BULLET_SIZE2 4	// 저격총
#define MAX_BULLET1 20	// 총알 최대 개수

typedef struct {
	int exist;	// 존재여부 (TRUE/FALSE)
	int x;		// 현재 x좌표 (픽셀 상)
	int y;		// 현재 y좌표 (픽셀 상)

	float vx;	// x좌표 이동방향
	float vy;	// y좌표 이동방향 // 0으로 고정
	int c;		// 그라데이션을 위한 카운트
	float travelDistance;   // 이동한 거리
}BULLET;
extern BULLET bullet1[MAX_BULLET1 + 10];
extern BULLET bullet2[MAX_BULLET1 + 10];
extern int bullet1_count;
extern int bullet2_count;

//총 길이(이미지)
#define gunWidth 80
#define gunHeight 30


// 아이템 =====================================================================
#define iWidth 40
#define iHeight 40
#define MAX_ITEM 5
typedef struct {
	int exist;
	int x;
	int y;

	//낙하변수
	int fstartY;
	float downTime;
	float downHeight;
	BOOL falling;

	int type; //0: 총알 10개, 사거리 600,		1: 총알 20개, 사거리 300
}ITEMBOX;
extern ITEMBOX item[MAX_ITEM];
extern int item_count;

// 맵(사각형) =====================================================================
#define mapWidth 200
#define mapHeight 30
#define mapCount 5
typedef struct {
	int x;
	int y;
}MAPS;
extern MAPS maps[mapCount];