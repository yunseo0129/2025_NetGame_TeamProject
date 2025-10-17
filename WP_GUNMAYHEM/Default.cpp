#include "Default.h"

// Default.h에 extern으로 선언된 모든 전역 변수들의 정의(Definition) 및 초기화
int mapType;
HDC BMPmDC; // 더블버퍼링을 위한 mDC

HWND g_hWnd;

HINSTANCE g_hInst = NULL;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WP_GUNMAYHEM";

int cameraWidth = 900;
int cameraHeight = 650;
int cameraX = 0;   // 카메라의 좌상단 X 좌표
int cameraY = 0;   // 카메라의 좌상단 Y 좌표

// 애니메이션 비트맵 핸들
HBITMAP BMP_player1_inform = NULL;
HBITMAP BMP_player2_inform = NULL;
HBITMAP BMP_player1_left_stand = NULL;
HBITMAP BMP_player1_left_walk[4] = { NULL, NULL, NULL, NULL };
HBITMAP BMP_player1_right_stand = NULL;
HBITMAP BMP_player1_right_walk[4] = { NULL, NULL, NULL, NULL };

HBITMAP BMP_player2_left_stand = NULL;
HBITMAP BMP_player2_left_walk[4] = { NULL, NULL, NULL, NULL };
HBITMAP BMP_player2_right_stand = NULL;
HBITMAP BMP_player2_right_walk[4] = { NULL, NULL, NULL, NULL };

HBITMAP BMP_gun1_left = NULL;
HBITMAP BMP_gun1_right = NULL;
HBITMAP BMP_gun2_left = NULL;
HBITMAP BMP_gun2_right = NULL;

HBITMAP BMP_itemBox = NULL;
HBITMAP BMP_startMenu = NULL;
HBITMAP BMP_pauseMenu = NULL;

HBITMAP BMP_map1 = NULL;
HBITMAP BMP_map2 = NULL;
HBITMAP BMP_ground1 = NULL;
HBITMAP BMP_inform = NULL;

RECT rt = {};	//클라영역 ({}로 RECT 구조체를 0으로 초기화)

//사운드 =====================================================================
FMOD::System* ssystem = nullptr;
FMOD::Sound* sound1 = nullptr, * sound2 = nullptr, * clickSound = nullptr, * itemSound = nullptr;
FMOD::Channel* channel = 0;
FMOD_RESULT result = FMOD_OK;
void* extradriverdata = 0;

// 아이템 =====================================================================
// ITEMBOX 구조체 정의는 Default.h에 있음
ITEMBOX item[MAX_ITEM] = {};
int item_count = 0;

// 맵(사각형) =====================================================================
// MAPS 구조체 정의는 Default.h에 있음
MAPS maps[mapCount] = {};