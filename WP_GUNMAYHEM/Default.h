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

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WP_GUNMAYHEM";

int cameraWidth = 900;
int cameraHeight = 650;
int cameraX = 0;   // 카메라의 좌상단 X 좌표
int cameraY = 0;   // 카메라의 좌상단 Y 좌표

// 애니메이션 관련 변수
int player1_anim_frame = 0;
int player1_anim_timer = 0;
int player1_anim_max_frame = 2; // 0, 1, 2 총 3프레임

int player2_anim_frame = 0;
int player2_anim_timer = 0;
int player2_anim_max_frame = 2; // 0, 1, 2 총 3프레임

// 애니메이션 비트맵 핸들
HBITMAP BMP_player1_inform;
HBITMAP BMP_player2_inform;
HBITMAP BMP_player1_left_stand;
HBITMAP BMP_player1_left_walk[4];
HBITMAP BMP_player1_right_stand;
HBITMAP BMP_player1_right_walk[4];

HBITMAP BMP_player2_left_stand;
HBITMAP BMP_player2_left_walk[4];
HBITMAP BMP_player2_right_stand;
HBITMAP BMP_player2_right_walk[4];

HBITMAP BMP_gun1_left;
HBITMAP BMP_gun1_right;
HBITMAP BMP_gun2_left;
HBITMAP BMP_gun2_right;

HBITMAP BMP_itemBox;
HBITMAP BMP_startMenu;
HBITMAP BMP_pauseMenu;

HBITMAP BMP_map1;
HBITMAP BMP_map2;
HBITMAP BMP_ground1;
HBITMAP BMP_inform;

RECT rt;	//클라영역

//사운드
FMOD::System* ssystem;
FMOD::Sound* sound1, * sound2, * clickSound, * itemSound;
FMOD::Channel* channel = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

//총알
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
BULLET bullet1[MAX_BULLET1 + 10];
BULLET bullet2[MAX_BULLET1 + 10];
int bullet1_count = 0;
int bullet2_count = 0;

int gunType1 = 1;		// 1:pistol		2:snipe
int gunType2 = 1;		// 1:pistol		2:snipe
int range1 = 300;		//플레이어1의 사거리 (아이템을 먹으면 변화)
int range2 = 300;		//플레이어2의 사거리 (아이템을 먹으면 변화)
int maxBullet1 = 20;
int maxBullet2 = 20;


//총 길이(이미지)
#define gunWidth 80
#define gunHeight 30

//플레이어 (1, 2)
#define pWidth 45
#define pHeight 67
#define MAXLIFE 3
typedef struct {
	int exist;		//dead or alive
	int x;
	int y;
	float ax;		//가속도 : 
	int looking;	//left right -> paint & gunfire
	int life;
	int combo;
	int downCount;	//10의 타이머 동안 땅 부딪힘 검사 x

	//낙하 및 점프 변수
	int fstartY;
	float downTime;
	float downHeight;
	BOOL falling;

	int jstartY;
	float jumpTime;
	float jumpHeight;
	float jumpPower;
	BOOL jumping;

	//속도, 가속도 변수
	float acceleration;	// 현재 가속도
	float speed;			// 현재 속도
	bool isMoving;		// 이동 중인지 여부
}PLAYER;
PLAYER player1;
PLAYER player2;
#define MAX_SPEED 5.0f      // 최대 속도
#define ACCELERATION 0.1f   // 가속도
#define FRICTION 0.05f      // 마찰력(감속)
int jumpCount1 = 0;
int jumpCount2 = 0;
int comboTime1 = 0;
int comboTime2 = 0;

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
ITEMBOX item[MAX_ITEM];
int item_count = 0;

//맵(사각형)
#define mapWidth 200
#define mapHeight 30
#define mapCount 5
typedef struct {
	int x;
	int y;
}MAPS;
MAPS maps[mapCount];
