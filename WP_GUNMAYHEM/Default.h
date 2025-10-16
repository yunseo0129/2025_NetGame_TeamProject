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
int cameraX = 0;   // ī�޶��� �»�� X ��ǥ
int cameraY = 0;   // ī�޶��� �»�� Y ��ǥ

// �ִϸ��̼� ���� ����
int player1_anim_frame = 0;
int player1_anim_timer = 0;
int player1_anim_max_frame = 2; // 0, 1, 2 �� 3������

int player2_anim_frame = 0;
int player2_anim_timer = 0;
int player2_anim_max_frame = 2; // 0, 1, 2 �� 3������

// �ִϸ��̼� ��Ʈ�� �ڵ�
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

RECT rt;	//Ŭ�󿵿�

//����
FMOD::System* ssystem;
FMOD::Sound* sound1, * sound2, * clickSound, * itemSound;
FMOD::Channel* channel = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

//�Ѿ�
#define BULLET_SIZE 2
#define BULLET_SIZE2 4
#define MAX_BULLET1 20

typedef struct {
	int exist;	// ���翩�� (TRUE/FALSE)
	int x;		// ���� x��ǥ (�ȼ� ��)
	int y;		// ���� y��ǥ (�ȼ� ��)

	float vx;	// x��ǥ �̵�����
	float vy;	// y��ǥ �̵�����
	int c;		// �׶��̼��� ���� ī��Ʈ
	float travelDistance;   // �̵��� �Ÿ�
}BULLET;
BULLET bullet1[MAX_BULLET1 + 10];
BULLET bullet2[MAX_BULLET1 + 10];
int bullet1_count = 0;
int bullet2_count = 0;

int gunType1 = 1;		// 1:pistol		2:snipe
int gunType2 = 1;		// 1:pistol		2:snipe
int range1 = 300;		//�÷��̾�1�� ��Ÿ� (�������� ������ ��ȭ)
int range2 = 300;		//�÷��̾�2�� ��Ÿ� (�������� ������ ��ȭ)
int maxBullet1 = 20;
int maxBullet2 = 20;


//�� ����(�̹���)
#define gunWidth 80
#define gunHeight 30

//�÷��̾� (1, 2)
#define pWidth 45
#define pHeight 67
#define MAXLIFE 3
typedef struct {
	int exist;		//dead or alive
	int x;
	int y;
	float ax;		//���ӵ� : 
	int looking;	//left right -> paint & gunfire
	int life;
	int combo;
	int downCount;	//10�� Ÿ�̸� ���� �� �ε��� �˻� x

	//���� �� ���� ����
	int fstartY;
	float downTime;
	float downHeight;
	BOOL falling;

	int jstartY;
	float jumpTime;
	float jumpHeight;
	float jumpPower;
	BOOL jumping;

	//�ӵ�, ���ӵ� ����
	float acceleration;	// ���� ���ӵ�
	float speed;			// ���� �ӵ�
	bool isMoving;		// �̵� ������ ����
}PLAYER;
PLAYER player1;
PLAYER player2;
#define MAX_SPEED 5.0f      // �ִ� �ӵ�
#define ACCELERATION 0.1f   // ���ӵ�
#define FRICTION 0.05f      // ������(����)
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

	//���Ϻ���
	int fstartY;
	float downTime;
	float downHeight;
	BOOL falling;

	int type; //0: �Ѿ� 10��, ��Ÿ� 600,		1: �Ѿ� 20��, ��Ÿ� 300
}ITEMBOX;
ITEMBOX item[MAX_ITEM];
int item_count = 0;

//��(�簢��)
#define mapWidth 200
#define mapHeight 30
#define mapCount 5
typedef struct {
	int x;
	int y;
}MAPS;
MAPS maps[mapCount];
