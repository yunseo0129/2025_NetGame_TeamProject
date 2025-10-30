﻿#include "CPlayer.h"
#include "Default.h"
#include "CLevelManager.h"
#include "CBmpMgr.h"

PAINTSTRUCT ps;
HDC hDC;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	srand(time(0));
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // 더블 클릭을 사용할 수 있게 스타일 설정
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	RegisterClassEx(&WndClass);
	g_hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 900, 650, NULL, (HMENU)NULL, hInstance, NULL);
	//hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED | WS_CAPTION , 0, 0, 900, 650, NULL, (HMENU)NULL, hInstance, NULL);

	CLevelManager::GetInstance()->Initialize();

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0)) {
		// 이 방식은 WM_TIMER를 사용해 주기적으로 게임 로직을 호출하도록 해야함
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// LevelManager 해제
	CLevelManager::GetInstance()->DestroyInstance();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// 더블버퍼링용 변수
	HDC mDC;
	HBITMAP hBitmap;
	HBITMAP oldBitmap;

	//--- 메시지 처리하기
	switch (iMsg) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rt);
		CBmpMgr::GetInstance()->Insert_Bmp(IDB_P1LEFT_STAND, L"1SL");
		BMP_player1_left_stand = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1LEFT_STAND));
		BMP_player1_left_walk[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1LEFT_WALK1));
		BMP_player1_left_walk[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1LEFT_STAND));
		BMP_player1_left_walk[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1LEFT_WALK3));
		BMP_player1_left_walk[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1LEFT_STAND));

		BMP_player1_right_stand = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1RIGHT_STAND));
		BMP_player1_right_walk[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1RIGHT_WALK1));
		BMP_player1_right_walk[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1RIGHT_STAND));
		BMP_player1_right_walk[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1RIGHT_WALK3));
		BMP_player1_right_walk[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1RIGHT_STAND));

		BMP_player2_left_stand = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2LEFT_STAND));
		BMP_player2_left_walk[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2LEFT_WALK1));
		BMP_player2_left_walk[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2LEFT_STAND));
		BMP_player2_left_walk[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2LEFT_WALK3));
		BMP_player2_left_walk[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2LEFT_STAND));

		BMP_player2_right_stand = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2RIGHT_STAND));
		BMP_player2_right_walk[0] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2RIGHT_WALK1));
		BMP_player2_right_walk[1] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2RIGHT_STAND));
		BMP_player2_right_walk[2] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2RIGHT_WALK3));
		BMP_player2_right_walk[3] = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2RIGHT_STAND));

		BMP_gun1_left = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GUN1LEFT));
		BMP_gun1_right = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GUN1RIGHT));

		BMP_gun2_left = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GUN2LEFT));
		BMP_gun2_right = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GUN2RIGHT));

		BMP_itemBox = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ITEMBOX));
		BMP_startMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SMENU));
		BMP_pauseMenu = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PMENU));

		BMP_map1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MAP1));
		BMP_map2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MAP2));

		BMP_ground1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GROUND1));
		BMP_inform = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_INFORM));

		BMP_player1_inform = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P1INFORM));
		BMP_player2_inform = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_P2INFORM));

		g_BMPmDC = CreateCompatibleDC(NULL);

		//사운드
		result = FMOD::System_Create(&ssystem); //--- 사운드 시스템 생성
		if (result != FMOD_OK)
			exit(0);
		ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata); //--- 사운드 시스템 초기화
		ssystem->createSound("744_pistol3.wav", FMOD_LOOP_NORMAL, 0, &sound1); //--- 1번 사운드 생성 및 설정
		ssystem->createSound("761_snipe1.wav", FMOD_LOOP_NORMAL, 0, &sound2); //--- 2번 사운드 생성 및 설정
		ssystem->createSound("1_menu.wav", FMOD_LOOP_NORMAL, 0, &clickSound);
		ssystem->createSound("737_item.wav", FMOD_LOOP_NORMAL, 0, &itemSound);
		PlaySound(TEXT("787_music555.wav"), NULL, SND_ASYNC | SND_LOOP);


		SetTimer(hWnd, 1, 15, NULL);	// 메인 게임 루프 타이머 (15ms 간격)
		break;
	}

	// 입력 처리는 CLevel::Update 에서 처리하도록 하기
	case WM_CHAR:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	{
		// 모든 입력 로직 제거
		// CLevelManager::GetInstance()->UpdateLevel() 에서 처리
		// -> CMainLevel::Update(), CPlayLevel::Update() 등
		break;
	}
	case WM_TIMER:
	{
		// === 게임 로직 업데이트 및 화면 갱신 요청 ===
		switch (wParam) {
		case 1: // 메인 게임 루프 타이머
			CLevelManager::GetInstance()->UpdateLevel();
			InvalidateRect(g_hWnd, NULL, FALSE);
			break;

			//case 2: // 애니메이션 타이머 (나중에 CPlayLevel로 이동)
			// 	break;
		}
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);

		// ---- 더블 버퍼링 설정 ----
		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
		oldBitmap = (HBITMAP)SelectObject(mDC, hBitmap);

		// ---- 그리기는 모두 mDC에 그린다 ----
		Rectangle(mDC, 0, 0, rt.right, rt.bottom);	//화면 색칠

		// ==== 레벨 매니저에게 그리기 작업 위임 ====
		CLevelManager::GetInstance()->DrawLevel(mDC);

		// ---- 버퍼 복사 및 정리 ----
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY); // hDC에 mDC내용 복사

		// ---- 메모리 DC와 비트맵 해제 ----
		SelectObject(mDC, oldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(mDC);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		// ==== 리소스 해제 작업 ====
		DeleteDC(g_BMPmDC);
		DeleteObject(BMP_player1_left_stand);
		DeleteObject(BMP_player1_right_stand);
		for (int i = 0; i < 4; i++) {
			DeleteObject(BMP_player1_left_walk[i]);
			DeleteObject(BMP_player1_right_walk[i]);
		}
		DeleteObject(BMP_player2_left_stand);
		DeleteObject(BMP_player2_right_stand);
		for (int i = 0; i < 4; i++) {
			DeleteObject(BMP_player2_left_walk[i]);
			DeleteObject(BMP_player2_right_walk[i]);
		}

		KillTimer(hWnd, 1);
		// KillTimer(hWnd, 2);

		PlaySound(NULL, NULL, NULL);
		channel->stop();
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}