#include "CPlayer.h"
#include "Default.h"

CPlayer player1;
CPlayer player2;

void reload1() {
	bullet1_count = 0;
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet1[i].exist == FALSE)
			bullet1[i].c = 0;
	}
}
void gunFire1(int way, BULLET bullet[]) {
	//불렛카운트 -> 불렛 카운트가 맥스불렛을 넘으면 발사가 안됨 (총알 없음)
	//불렛 카운트가 탄약 갯수를 넘지 않음 -> 총을 쏨 (비활성화인 총알 하나를 활성화 시킴) -> 불렛 카운트 증가
	if (bullet1_count < maxBullet1) {
		for (int i = 0; i < MAX_BULLET1; i++) {
			if (bullet[i].exist == FALSE && bullet[i].c == 0)
			{
				bullet[i].exist = TRUE;
				bullet[i].x = player1.x + pWidth / 2;
				bullet[i].y = player1.y + pHeight / 2;
				bullet[i].travelDistance = 0; // 초기화

				//총알 방향 계산
				switch (way) {
				case 0:
					bullet[i].vx = -1;
					bullet[i].vy = 0;
					break;
				case 1:
					bullet[i].vx = 1;
					bullet[i].vy = 0;
					break;
				}
				bullet1_count++;

				channel->stop();
				switch (gunType1) {
				case 1:
					sound1->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound1, 0, false, &channel); //--- 1번 사운드 재생
					break;
				case 2:
					sound2->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound2, 0, false, &channel); //--- 1번 사운드 재생
					break;
				}
				break;	//한번 쏘면 반복문 정지
			}
		}
	}
}
void update_bullet1() {
	//1. 총알1 충돌체크
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet1[i].exist == TRUE) {
			//사거리 도달시
			if (bullet1[i].travelDistance > range1) {
				bullet1[i].exist = FALSE;
			}
			else {
				if (bullet1[i].x + BULLET_SIZE >= player2.x && bullet1[i].x - BULLET_SIZE <= player2.x + pWidth &&
					bullet1[i].y + BULLET_SIZE >= player2.y && bullet1[i].y - BULLET_SIZE <= player2.y + pHeight) {

					player2.combo++;
					player2.comboTime = 0;
					player2.speed = (7 * player2.combo) * bullet1[i].vx;
					bullet1[i].exist = FALSE;
				}
			}
		}
	}
	//2. 총알1 이동 -> 플레이어의 방향에 따라(looking)
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet1[i].exist == TRUE) {
			bullet1[i].x += bullet1[i].vx * 13; // 총알 속도 조절
			bullet1[i].travelDistance += 13;    // 이동 거리 추가
			if (bullet1[i].c < 30) {
				bullet1[i].c++;
			}
		}
	}
	//콤보초기화 -> 콤보가 1 이상이되면 시간을 검사한다
	if (player2.combo > 0) {
		player2. comboTime++;
		//player2.combo++;

		if (player2.comboTime > 200) {
			player2.combo = 0;
			player2.comboTime = 0;
		}
	}
}

void reload2() {
	bullet2_count = 0;
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet2[i].exist == FALSE)
			bullet2[i].c = 0;
	}
}
void gunFire2(int way, BULLET bullet[]) {
	if (bullet2_count < maxBullet2) {
		for (int i = 0; i < MAX_BULLET1; i++) {
			if (bullet[i].exist == FALSE && bullet[i].c == 0)
			{
				bullet[i].exist = TRUE;
				bullet[i].x = player2.x + pWidth / 2;
				bullet[i].y = player2.y + pHeight / 2;
				bullet[i].travelDistance = 0; // 초기화

				//총알 방향 계산
				switch (way) {
				case 0:
					bullet[i].vx = -1;
					bullet[i].vy = 0;
					break;
				case 1:
					bullet[i].vx = 1;
					bullet[i].vy = 0;
					break;
				}
				bullet2_count++;

				channel->stop();
				switch (gunType2) {
				case 1:
					sound1->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound1, 0, false, &channel); //--- 1번 사운드 재생
					break;
				case 2:
					sound2->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound2, 0, false, &channel); //--- 1번 사운드 재생
					break;
				}
				break;	//한번 쏘면 반복문 정지
			}
		}
	}
}
void update_bullet2() {
	//1. 총알2 충돌체크
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet2[i].exist == TRUE) {
			//사거리 도달 시
			if (bullet2[i].travelDistance > range2) {
				bullet2[i].exist = FALSE;
			}
			else {
				if (bullet2[i].x + BULLET_SIZE >= player1.x && bullet2[i].x - BULLET_SIZE <= player1.x + pWidth &&
					bullet2[i].y + BULLET_SIZE >= player1.y && bullet2[i].y - BULLET_SIZE <= player1.y + pHeight) {

					player1.combo++;
					player1.comboTime = 0;
					player1.speed = (7 * player1.combo) * bullet2[i].vx;
					bullet2[i].exist = FALSE;
				}
			}
		}
	}
	//2. 총알 이동 -> 플레이어의 방향에 따라(looking)
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet2[i].exist == TRUE) {
			bullet2[i].x += bullet2[i].vx * 13; //총알 속도 조절
			bullet2[i].travelDistance += 13;    // 이동 거리 추가
			if (bullet2[i].c < 30) {
				bullet2[i].c++;
			}
		}
	}
	//콤보초기화 -> 콤보가 1 이상이되면 시간을 검사한다
	if (player1.combo > 0) {
		player1.comboTime++;
		if (player1.comboTime > 200) {
			player1.combo = 0;
			player1.comboTime = 0;
		}
	}
}

void update_player1() {
	// update speed :  player 1
	if (player1.isMoving) {
		// 가속도를 속도에 적용
		player1.speed += player1.acceleration;
		// 속도가 최대 속도를 초과하지 않도록 조정
		player1.speed = max(-MAX_SPEED, min(player1.speed, MAX_SPEED));
	}
	else {
		// 마찰력을 적용하여 점차 멈춤
		if (player1.speed > 0) {
			player1.speed = max(0.0f, player1.speed - FRICTION);
		}
		else if (player1.speed < 0) {
			player1.speed = min(0.0f, player1.speed + FRICTION);
		}
	}
	// 속도에 따라 플레이어 위치 업데이트
	player1.x += player1.speed;

	// jumping or falling
	if (player1.jumping == TRUE) {
		player1.jumpHeight = (player1.jumpTime * player1.jumpTime - player1.jumpPower * player1.jumpTime) * 4.f;
		player1.jumpTime += 0.2f;
		player1.y = (player1.jstartY + (int)player1.jumpHeight);

		//점프타임이 점프파워보다 커졌을 때, 떨어지는 중일 때를 충돌체크한다.
		if (player1.jumpHeight <= (player1.jumpTime * player1.jumpTime - player1.jumpPower * player1.jumpTime) * 4.f) {
			for (int i = 0; i < mapCount; i++) {
				if (player1.x + pWidth > maps[i].x && player1.x < maps[i].x + mapWidth &&
					player1.y + pHeight > maps[i].y && player1.y + pHeight < maps[i].y + 15)
				{
					player1.falling = FALSE;
					player1.jumping = FALSE;
					player1.jumpCount = 0;
					player1.y = maps[i].y - pHeight;
					break;
				}
			}
		}

	}
	if (player1.jumping == FALSE) {
		// 1. 플레이어의 위치 검사 -> 떨어지기 시작할지, 안떨어질지 판단 -> 다운카운트 중일때는 판단 x
		if (player1.downCount == 0) {
			for (int i = 0; i < mapCount; i++) {
				// 플레이어가 맵 위에 있을 때
				if (player1.x + pWidth > maps[i].x && player1.x < maps[i].x + mapWidth &&
					player1.y + pHeight > maps[i].y && player1.y + pHeight < maps[i].y + 15)
				{
					player1.jumping = FALSE;
					player1.falling = FALSE;
					player1.jumpCount = 0;
					if (player1.y + pHeight > maps[i].y) {
						player1.y = maps[i].y - pHeight;
					}
					break; // 맵 위에 있으면 더 이상 검사할 필요가 없으므로 반복문을 빠져나온다.
				}
				else {
					// 플레이어가 맵 위에 없을 때
					if (player1.falling == FALSE)	// 떨어지고 있지 않은 상태였다면
					{
						player1.downTime = 0;
						player1.downHeight = 0;
						player1.fstartY = player1.y; // 현재 좌표를 저장하고
						player1.falling = TRUE;		// 떨어지는 상태로 만든다.
					}
				}
			}
		}
		// 2. 떨어지는 중이면 -> 위치변동, 충돌체크 -> 다운카운트 중일때는 충돌체크 x
		if (player1.falling == TRUE) {
			player1.downHeight = (player1.downTime * (player1.downTime / 2)) * 4.f;
			player1.downTime += 0.2f;
			player1.y = (player1.fstartY + (int)player1.downHeight);

			if (player1.downCount == 0) {
				for (int i = 0; i < mapCount; i++) {
					if (player1.x + pWidth > maps[i].x && player1.x < maps[i].x + mapWidth &&
						player1.y + pHeight > maps[i].y && player1.y + pHeight < maps[i].y + 15)
					{
						player1.falling = FALSE;
						player1.jumping = FALSE;
						player1.jumpCount = 0;
						player1.y = maps[i].y - pHeight;
						break;
					}
				}
			}
		}
	}
	// 다운카운트 -> 0이면 증가 x / 0이 아닐때 증가 시작 / N도달시 0으로 초기화
	if (player1.downCount > 0) {
		player1.downCount++;
		if (player1.downCount > 30) {
			player1.downCount = 0;
		}
	}

	//아이템 충돌검사
	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i].exist == TRUE) {
			if (player1.x < item[i].x + iWidth / 2 && item[i].x + iWidth / 2 < player1.x + pWidth &&
				player1.y < item[i].y + iHeight / 2 && item[i].y + iHeight / 2 < player1.y + pHeight) {

				channel->stop();
				itemSound->setMode(FMOD_LOOP_OFF);
				ssystem->playSound(itemSound, 0, false, &channel); //--- 1번 사운드 재생
				item[i].exist = FALSE;
				//아이템에 따른 총 변화
				switch (item[i].type) {
				case 0:
					gunType1 = 2;
					maxBullet1 = 10;
					range1 = 600;
					break;
				case 1:
					gunType1 = 1;
					maxBullet1 = 20;
					range1 = 300;
					break;
				}
				reload1();
			}
		}
	}
}
void update_player2() {
	// update speed :  player 2
	if (player2.isMoving) {
		// 가속도를 속도에 적용
		player2.speed += player2.acceleration;
		// 속도가 최대 속도를 초과하지 않도록 조정
		player2.speed = max(-MAX_SPEED, min(player2.speed, MAX_SPEED));
	}
	else {
		// 마찰력을 적용하여 점차 멈춤
		if (player2.speed > 0) {
			player2.speed = max(0.0f, player2.speed - FRICTION);
		}
		else if (player2.speed < 0) {
			player2.speed = min(0.0f, player2.speed + FRICTION);
		}
	}
	// 속도에 따라 플레이어 위치 업데이트
	player2.x += player2.speed;

	// jumping or falling
	if (player2.jumping == TRUE) {
		player2.jumpHeight = (player2.jumpTime * player2.jumpTime - player2.jumpPower * player2.jumpTime) * 4.f;
		player2.jumpTime += 0.2f;
		player2.y = (player2.jstartY + (int)player2.jumpHeight);

		//점프타임이 점프파워보다 커졌을 때, 떨어지는 중일 때를 충돌체크한다.
		if (player2.jumpHeight <= (player2.jumpTime * player2.jumpTime - player2.jumpPower * player2.jumpTime) * 4.f) {
			for (int i = 0; i < mapCount; i++) {
				if (player2.x + pWidth > maps[i].x && player2.x < maps[i].x + mapWidth &&
					player2.y + pHeight > maps[i].y && player2.y + pHeight < maps[i].y + 15)
				{
					player2.falling = FALSE;
					player2.jumping = FALSE;
					player2.jumpCount = 0;
					player2.y = maps[i].y - pHeight;
					break;
				}
			}
		}

	}
	if (player2.jumping == FALSE) {
		// 1. 플레이어의 위치 검사 -> 떨어지기 시작할지, 안떨어질지 판단 -> 다운카운트 중일때는 판단 x
		if (player2.downCount == 0) {
			for (int i = 0; i < mapCount; i++) {
				// 플레이어가 맵 위에 있을 때
				if (player2.x + pWidth > maps[i].x && player2.x < maps[i].x + mapWidth &&
					player2.y + pHeight > maps[i].y && player2.y + pHeight < maps[i].y + 15)
				{
					player2.jumping = FALSE;
					player2.falling = FALSE;
					player2.jumpCount = 0;
					if (player2.y + pHeight > maps[i].y) {
						player2.y = maps[i].y - pHeight;
					}
					break; // 맵 위에 있으면 더 이상 검사할 필요가 없으므로 반복문을 빠져나온다.
				}
				else {
					// 플레이어가 맵 위에 없을 때
					if (player2.falling == FALSE)	// 떨어지고 있지 않은 상태였다면
					{
						player2.downTime = 0;
						player2.downHeight = 0;
						player2.fstartY = player2.y; // 현재 좌표를 저장하고
						player2.falling = TRUE;		// 떨어지는 상태로 만든다.
					}
				}
			}
		}
		// 2. 떨어지는 중이면 -> 위치변동, 충돌체크 -> 다운카운트 중일때는 충돌체크 x
		if (player2.falling == TRUE) {
			player2.downHeight = (player2.downTime * (player2.downTime / 2)) * 4.f;
			player2.downTime += 0.2f;
			player2.y = (player2.fstartY + (int)player2.downHeight);

			if (player2.downCount == 0) {
				for (int i = 0; i < mapCount; i++) {
					if (player2.x + pWidth > maps[i].x && player2.x < maps[i].x + mapWidth &&
						player2.y + pHeight > maps[i].y && player2.y + pHeight < maps[i].y + 15)
					{
						player2.falling = FALSE;
						player2.jumping = FALSE;
						player2.jumpCount = 0;
						player2.y = maps[i].y - pHeight;
						break;
					}
				}
			}
		}
	}
	// 다운카운트 -> 0이면 증가 x / 0이 아닐때 증가 시작 / N도달시 0으로 초기화
	if (player2.downCount > 0) {
		player2.downCount++;
		if (player2.downCount > 30) {
			player2.downCount = 0;
		}
	}

	//아이템 충돌검사
	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i].exist == TRUE) {
			if (player2.x < item[i].x + iWidth / 2 && item[i].x + iWidth / 2 < player2.x + pWidth &&
				player2.y < item[i].y + iHeight / 2 && item[i].y + iHeight / 2 < player2.y + pHeight) {

				channel->stop();
				itemSound->setMode(FMOD_LOOP_OFF);
				ssystem->playSound(itemSound, 0, false, &channel); //--- 1번 사운드 재생
				item[i].exist = FALSE;
				switch (item[i].type) {
				case 0:
					gunType2 = 2;
					maxBullet2 = 10;
					range2 = 600;
					break;
				case 1:
					gunType2 = 1;
					maxBullet2 = 20;
					range2 = 300;
					break;
				}
				reload2();
			}
		}
	}
}
void update_camera() {
	int playerCenterX = (player1.x + player2.x) / 2;
	int playerCenterY = (player1.y + player2.y) / 2;

	cameraX = playerCenterX - cameraWidth / 2;
	cameraY = playerCenterY - cameraHeight / 2 + 100;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	srand(time(0));
	HWND hWnd;
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 900, 650, NULL, (HMENU)NULL, hInstance, NULL);
	//hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED | WS_CAPTION , 0, 0, 900, 650, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//else { //메시지가 없는 경우에 호출 }
	}
	return msg.wParam;
}

//winmain 생략
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//WndProc 기본 활용 변수
	PAINTSTRUCT ps;
	HDC hDC, mDC;
	static HDC BMPmDC;
	HBITMAP hBitmap;			// 더블버퍼링을 위한 hBitmap
	//static RECT rt;			// 전체 화면의 사각영역 받아오기

	HPEN mPen, oldPen;
	HBRUSH mBrush, oldBrush;	// 더블버퍼링을 위한 mBrush

	static TCHAR lpOut1[100];	// wsprintf : 텍스트 저장용 변수
	static TCHAR lpOut2[100];	// wsprintf : 텍스트 저장용 변수
	static TCHAR Combo1[100];	// wsprintf : 텍스트 저장용 변수
	static TCHAR Combo2[100];	// wsprintf : 텍스트 저장용 변수

	static int start = FALSE;
	static int pause = FALSE;
	RECT startMenu = { 500, 350, 800, 400 };
	RECT endMenu = { 500, 450, 800, 500 };
	RECT pauseStartMenu = { 200, 350, 700, 400 };
	RECT pauseEndMenu = { 200, 450, 700, 500 };
	static int mx, my;

	static int mapType = 0;
	RECT changeMapMenu = { 600, 250, 800, 300 };
	static int end = FALSE;

	//--- 메시지 처리하기
	switch (iMsg) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rt);
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

		BMPmDC = CreateCompatibleDC(NULL);

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

		SetTimer(hWnd, 3, 100, NULL); // 애니메이션 타이머 (200ms 간격)
		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
			//player2 gunFire
		case '[':
		{
			gunFire2(player2.looking, bullet2);
			break;
		}
		// player1 jump
		case 'w':
		{
			if (player1.jumpCount < 2)
			{
				player1.jumpCount++;
				player1.jumpTime = 0.f;
				player1.jumpHeight = 0;
				player1.jstartY = player1.y;
				player1.jumping = TRUE;
				player1.falling = FALSE;
			}
			break;
		}
		case 's':
		{
			//아래점프 구현 -> 땅에 있을 때만 아래점프 가능
			if (player1.downCount == 0) {
				player1.downCount = 1;

				player1.downTime = 0;
				player1.downHeight = 0;
				player1.fstartY = player1.y; // 현재 좌표를 저장하고

				player1.jumping = FALSE;
				player1.falling = TRUE;
			}
			break;
		}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
	{	//다른 메세지가 발생할때, keyup메시지가 제때 작동하지 않는 버그가 있음 -> 멈추지 않음
		switch (wParam) {
		case VK_ESCAPE:
		{
			if (start == TRUE) {
				if (pause == FALSE) {
					start = FALSE;
					pause = TRUE;
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
				}
				else {
					start = TRUE;
					pause = FALSE;
					SetTimer(hWnd, 1, 15, NULL);
					SetTimer(hWnd, 2, 7000, NULL);
				}
			}
			break;
		}
		//player1 gunFire
		case VK_SPACE:
		{
			gunFire1(player1.looking, bullet1);
			break;
		}
		//player2 이동
		case VK_UP:
		{
			if (player2.jumpCount < 2)
			{
				player2.jumpCount++;
				player2.jumpTime = 0.f;
				player2.jumpHeight = 0;
				player2.jstartY = player2.y;
				player2.jumping = TRUE;
				player2.falling = FALSE;
			}
			break;
		}	
		case VK_DOWN:
		{
			//아래점프 구현 -> 땅에 있을 때만 아래점프 가능
			if (player2.downCount == 0) {
				player2.downCount = 1;

				player2.downTime = 0;
				player2.downHeight = 0;
				player2.fstartY = player2.y; // 현재 좌표를 저장하고

				player2.jumping = FALSE;
				player2.falling = TRUE;
			}
			break;
		}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_KEYUP:
	{
		switch (wParam) {
		case 'A':
			player1.isMoving = FALSE; // 키를 뗐을 때 이동 중지
			break;
		case 'D':
			player1.isMoving = FALSE; // 키를 뗐을 때 이동 중지
			break;
		case VK_LEFT:
			player2.isMoving = FALSE; // 키를 뗐을 때 이동 중지
			break;
		case VK_RIGHT:
			player2.isMoving = FALSE; // 키를 뗐을 때 이동 중지
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		channel->stop();
		clickSound->setMode(FMOD_LOOP_OFF);
		ssystem->playSound(clickSound, 0, false, &channel); //--- 1번 사운드 재생
			
		//시작 상태일때 누르는 좌표가 사각영역에 해당하면 시작 or 종료
		//정지 상태일때 누르는 좌표에 따라 다시시작 or 종료
		if (end == TRUE) {
			end = FALSE;
			start = FALSE;
			pause = FALSE;
		}
		else if (start == FALSE && pause == FALSE) {
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			POINT pt = { mx, my };
			if (PtInRect(&startMenu, pt)) {	//시작버튼 클릭 -> 초기화 진행, 타이머 시작
				start = TRUE;
				//총알1 초기화
				for (int i = 0; i < MAX_BULLET1; i++) {
					bullet1[i].exist = FALSE;
					bullet1[i].c = 0;
				}
				bullet1_count = 0;
				gunType1 = 1;
				range1 = 300;
				maxBullet1 = 20;

				//총알2 초기화
				for (int i = 0; i < MAX_BULLET1; i++) {
					bullet2[i].exist = FALSE;
					bullet2[i].c = 0;
				}
				bullet2_count = 0;
				gunType2 = 1;				
				range2 = 300;
				maxBullet2 = 20;

				//아이템 초기화
				for (int i = 0; i < MAX_ITEM; i++) {
					item[i].exist = FALSE;
				}

				//플레이어1 초기화
				if (TRUE) {
					//플레이어 초기화
					player1.exist = TRUE;
					player1.x = 150;
					player1.y = -200;
					//player1.ax = 0;
					player1.looking = 0;
					player1.life = MAXLIFE;
					player1.combo = 0;
					player1.downCount = 0;

					player1.fstartY = 0;
					player1.downTime = 0.f;
					player1.downHeight = 0;
					player1.falling = FALSE;

					player1.jstartY = 0;
					player1.jumpTime = 0.f;
					player1.jumpHeight = 0;
					player1.jumpPower = 12.f;
					player1.jumping = FALSE;

					//속도, 가속도 변수
					player1.acceleration = 0.0f;	// 현재 가속도
					player1.speed = 0.0f;			// 현재 속도
					player1.isMoving = FALSE;		// 이동 중인지 여부
				}
				//플레이어2 초기화
				if (TRUE) {
					//플레이어 초기화
					player2.exist = TRUE;
					player2.x = 650;
					player2.y = -200;
					//player2.ax = 0;
					player2.looking = 0;
					player2.life = MAXLIFE;
					player2.combo = 0;
					player2.downCount = 0;

					player2.fstartY = 0;
					player2.downTime = 0.f;
					player2.downHeight = 0;
					player2.falling = FALSE;

					player2.jstartY = 0;
					player2.jumpTime = 0.f;
					player2.jumpHeight = 0;
					player2.jumpPower = 12.f;
					player2.jumping = FALSE;

					//속도, 가속도 변수
					player2.acceleration = 0.0f;	// 현재 가속도
					player2.speed = 0.0f;			// 현재 속도
					player2.isMoving = FALSE;		// 이동 중인지 여부
				}
				//맵 초기화
				if (TRUE) {
					if (mapType == 0) {
						maps[0] = { 330, 170 };	//1단

						maps[1] = { 160, 300 };	//2단
						maps[2] = { 510, 300 };

						maps[3] = { 70, 420 };	//3단
						maps[4] = { 600, 420 };
					}
					else { //if (mapType == 1)
						maps[0] = { 160, 170 };	//1단
						maps[1] = { 510, 170 };

						maps[2] = { 330, 300 }; //2단

						maps[3] = { 70, 420 };	//3단
						maps[4] = { 600, 420 };
					}
				}

				//업데이트 타이머
				SetTimer(hWnd, 1, 15, NULL);
				//아이템 생성 타이머
				SetTimer(hWnd, 2, 7000, NULL);
			}
			else if (PtInRect(&endMenu, pt)) {
				DeleteDC(BMPmDC);
				DeleteObject(BMP_player1_left_stand);	//비트맵 삭제
				DeleteObject(BMP_player1_right_stand);	//비트맵 삭제
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player1_left_walk[i]);	//비트맵 삭제
					DeleteObject(BMP_player1_right_walk[i]);	//비트맵 삭제
				}
				DeleteObject(BMP_player2_left_stand);	//비트맵 삭제
				DeleteObject(BMP_player2_right_stand);	//비트맵 삭제
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player2_left_walk[i]);	//비트맵 삭제
					DeleteObject(BMP_player2_right_walk[i]);	//비트맵 삭제
				}
				PlaySound(NULL, NULL, NULL);
				channel->stop();
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				PostQuitMessage(0);
			}
			else if (PtInRect(&changeMapMenu, pt)) {
				mapType++;
				mapType = mapType % 2;
			}
		}
		else if (pause == TRUE) {
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			POINT pt = { mx, my };
			if (PtInRect(&pauseStartMenu, pt)) {
				start = TRUE;
				pause = FALSE;
				SetTimer(hWnd, 1, 15, NULL);
				SetTimer(hWnd, 2, 7000, NULL);
			}
			else if (PtInRect(&pauseEndMenu, pt)) {
				DeleteDC(BMPmDC);
				DeleteObject(BMP_player1_left_stand);	//비트맵 삭제
				DeleteObject(BMP_player1_right_stand);	//비트맵 삭제
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player1_left_walk[i]);	//비트맵 삭제
					DeleteObject(BMP_player1_right_walk[i]);	//비트맵 삭제
				}
				DeleteObject(BMP_player2_left_stand);	//비트맵 삭제
				DeleteObject(BMP_player2_right_stand);	//비트맵 삭제
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player2_left_walk[i]);	//비트맵 삭제
					DeleteObject(BMP_player2_right_walk[i]);	//비트맵 삭제
				}
				PlaySound(NULL, NULL, NULL);
				channel->stop();
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				PostQuitMessage(0);
			}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_TIMER:
	{
		//A - player1 left
		if (GetAsyncKeyState(0x41) & 0x8000) {
			player1.looking = 0;
			if (player1.isMoving && player1.speed > 0) { // 반대 방향으로 빠르게 감속
				player1.speed -= FRICTION;
			}
			else {
				player1.acceleration = -ACCELERATION;
				player1.isMoving = TRUE;
			}
		}
		//D - player1 right
		if (GetAsyncKeyState(0x44) & 0x8000) {
			player1.looking = 1;
			if (player1.isMoving && player1.speed < 0) { // 반대 방향으로 빠르게 감속
				player1.speed += FRICTION;
			}
			else {
				player1.acceleration = ACCELERATION;
				player1.isMoving = TRUE;
			}
		}
		//player2 left
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			player2.looking = 0;
			if (player2.isMoving && player2.speed > 0) { // 반대 방향으로 빠르게 감속
				player2.speed -= FRICTION;
			}
			else {
				player2.acceleration = -ACCELERATION;
				player2.isMoving = TRUE;
			}
		}
		//player2 right
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			player2.looking = 1;
			if (player2.isMoving && player2.speed < 0) { // 반대 방향으로 빠르게 감속
				player2.speed += FRICTION;
			}
			else {
				player2.acceleration = ACCELERATION;
				player2.isMoving = TRUE;
			}
		}
	
		switch (wParam) {
		case 1:
		{
			// check player1 out
			if (player1.y > rt.bottom) {
				player1.regen();
				reload1();
			}
			// check player2 out
			if (player2.y > rt.bottom) {
				player2.regen();
				reload2();
			}
			if (player1.life == 0 || player2.life == 0) {
				end = TRUE;
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
			}

			//update Players move
			update_player1();
			update_player2();

			//update bullets crush & move
			update_bullet1();
			update_bullet2();

			//update item create & move -> 하늘에서 떨어짐	
			for (int i = 0; i < MAX_ITEM; i++) {
				if (item[i].exist == TRUE) {
					for (int j = 0; j < mapCount; j++) {
						if (item[i].x + iWidth > maps[j].x && item[i].x < maps[j].x + mapWidth &&
							item[i].y + iHeight > maps[j].y && item[i].y + iHeight < maps[j].y + 15)
						{
							item[i].falling = FALSE;
							if (item[i].y + iHeight > maps[j].y) {
								item[i].y = maps[j].y - iHeight;
							}
							break; // 맵 위에 있으면 더 이상 검사할 필요가 없으므로 반복문을 빠져나온다.
						}
						else {
							if (item[i].falling == FALSE)	// 떨어지고 있지 않은 상태였다면
							{
								item[i].downTime = 0;
								item[i].downHeight = 0;
								item[i].fstartY = item[i].y; // 현재 좌표를 저장하고
								item[i].falling = TRUE;		// 떨어지는 상태로 만든다.
							}
						}
					}
					if (item[i].falling == TRUE) {
						item[i].downHeight = (item[i].downTime * (item[i].downTime / 2)) * 4.f;
						item[i].downTime += 0.2f;
						item[i].y = (item[i].fstartY + (int)item[i].downHeight);

						for (int j = 0; j < mapCount; j++) {
							if (item[i].x + iWidth > maps[j].x && item[i].x < maps[j].x + mapWidth &&
								item[i].y + iHeight > maps[j].y && item[i].y + iHeight < maps[j].y + 15)
							{
								item[i].falling = FALSE;
								item[i].y = maps[j].y - iHeight;
								break;
							}
						}
					}
				}
			}

			update_camera();
			break;
		}
		case 2:
		{	//아이템 생성 타이머
			for (int i = 0; i < MAX_ITEM; i++) {
				if (item[i].exist == FALSE) {
					item[i].exist = TRUE;
					item[i].x = 100 + (rand() % 70 * 10);
					item[i].y = -200;
					item[i].fstartY = 0;
					item[i].downTime = 0.f;
					item[i].downHeight = 0;
					item[i].falling = FALSE;
					item[i].type = rand() % 2;	// 0 or 1
					break;
				}
			}
			break;
		}
		case 3:
		{
			// 애니메이션 타이머
			if (player1.isMoving) {
				player1.anim_frame = (player1.anim_frame + 1) % 4; // 0, 1, 2, 3 반복
			}
			if (player2.isMoving) {
				player2.anim_frame = (player2.anim_frame + 1) % 4; // 0, 1, 2, 3 반복
			}
			break;
		}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);

		mDC = CreateCompatibleDC(hDC);									//메모리DC 생성
		HBITMAP memBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mDC, memBitmap);

		//	----그리기는 모두 mDC에 그린다----
		Rectangle(mDC, 0, 0, rt.right, rt.bottom);	//화면 색칠

		//시작메뉴 -> 시작, 종료 시작 -> 타이머시작 및 맵 출력 등 활성화
		if (start == FALSE) {
			SelectObject(BMPmDC, BMP_startMenu);
			BitBlt(mDC, 0, 0, rt.right, rt.bottom, BMPmDC, 0, 0, SRCCOPY);
			wsprintf(lpOut1, L"Type : %d", mapType);
			TextOut(mDC, changeMapMenu.left + 140, changeMapMenu.top + 50, lpOut1, lstrlen(lpOut1));
		}
		//그리기
		if (start == TRUE) {
			//배경화면(맵 타입에 따라 다르게)
			switch (mapType) {
			case 0:
				SelectObject(BMPmDC, BMP_map1);
				BitBlt(mDC, 0, 0, 900, 650, BMPmDC, 900 - cameraX/5, 650 - cameraY / 5, SRCCOPY);
				break;
			case 1:
				SelectObject(BMPmDC, BMP_map2);
				BitBlt(mDC, 0, 0, 900, 650, BMPmDC, 900 - cameraX / 5, 650 - cameraY / 5, SRCCOPY);
				break;
			}
			//맵(5개의 땅)
			SelectObject(BMPmDC, BMP_ground1);
			for (int i = 0; i < mapCount; i++) {
				BitBlt(mDC, maps[i].x - cameraX, maps[i].y - 5 - cameraY, mapWidth, mapHeight, BMPmDC, 0, 0, SRCCOPY);
				//Rectangle(mDC, maps[i].x - cameraX, maps[i].y - 5 - cameraY, maps[i].x + mapWidth - cameraX, maps[i].y + mapHeight - cameraY);
			}

			// 플레이어 1
			switch (player1.looking) {
			case 0:
				if (player1.isMoving) {
					SelectObject(BMPmDC, BMP_player1_left_walk[player1.anim_frame]);
				}
				else {
					SelectObject(BMPmDC, BMP_player1_left_stand);
				}
				break;
			case 1:
				if (player1.isMoving) {
					SelectObject(BMPmDC, BMP_player1_right_walk[player1.anim_frame]);
				}
				else {
					SelectObject(BMPmDC, BMP_player1_right_stand);
				}
				break;
			}
			TransparentBlt(mDC, player1.x - cameraX, player1.y - cameraY, pWidth, pHeight, BMPmDC, 0, 0, pWidth, pHeight, RGB(127, 127, 127));
			if (player1.combo > 0) {
				wsprintf(Combo1, L"Hit %d", player1.combo);
				TextOut(mDC, player1.x + pWidth - cameraX, player1.y - 10 - cameraY, Combo1, lstrlen(Combo1));
			}

			// 플레이어 2
			switch (player2.looking) {
			case 0:
				if (player2.isMoving) {
					SelectObject(BMPmDC, BMP_player2_left_walk[player2.anim_frame]);
				}
				else {
					SelectObject(BMPmDC, BMP_player2_left_stand);
				}
				break;
			case 1:
				if (player2.isMoving) {
					SelectObject(BMPmDC, BMP_player2_right_walk[player2.anim_frame]);
				}
				else {
					SelectObject(BMPmDC, BMP_player2_right_stand);
				}
				break;
			}
			TransparentBlt(mDC, player2.x - cameraX, player2.y - cameraY, pWidth, pHeight, BMPmDC, 0, 0, pWidth, pHeight, RGB(127, 127, 127));
			//Rectangle(mDC, player2.x, player2.y, player2.x + pWidth, player2.y + pHeight);
			if (player2.combo > 0) {
				wsprintf(Combo2, L"Hit %d", player2.combo);
				TextOut(mDC, player2.x + pWidth - cameraX, player2.y - 10 - cameraY, Combo2, lstrlen(Combo2));
			}

			//플레이어 1 거리표시(맵 밖으로 나갔을 때)
			if (player1.x > rt.right) {
				wsprintf(lpOut1, L"%d > ", player1.x - rt.right);
				TextOut(mDC, player1.x - 55 - cameraX, player1.y + pHeight / 2 - cameraY, lpOut1, lstrlen(lpOut1));
			}
			else if (player1.x + pWidth < rt.left) {
				wsprintf(lpOut1, L" < %d", rt.left - player1.x);
				TextOut(mDC, player1.x + pWidth + 25 - cameraX, player1.y + pHeight / 2 - cameraY, lpOut1, lstrlen(lpOut1));
			}
			else if (player1.y < rt.top) {
				wsprintf(lpOut1, L"%d", rt.top - player1.y);
				TextOut(mDC, player1.x + 10 - cameraX, player1.y - 25 - cameraY, lpOut1, lstrlen(lpOut1));
			}

			//플레이어 2 거리표시(맵 밖으로 나갔을 때)
			if (player2.x > rt.right) {
				wsprintf(lpOut1, L"%d > ", player2.x - rt.right);
				TextOut(mDC, player2.x - 55 - cameraX, player2.y + pHeight / 2 - cameraY, lpOut1, lstrlen(lpOut1));
			}
			else if (player2.x + pWidth < rt.left) {
				wsprintf(lpOut1, L" < %d", rt.left - player2.x);
				TextOut(mDC, player2.x + pWidth + 25 - cameraX, player2.y + pHeight / 2 - cameraY, lpOut1, lstrlen(lpOut1));
			}
			else if (player2.y < rt.top) {
				wsprintf(lpOut1, L"%d", rt.top - player2.y);
				TextOut(mDC, player2.x + 10 - cameraX, player2.y - 25  - cameraY, lpOut1, lstrlen(lpOut1));
			}
			// 총알1 ( 플레이어1 )
			for (int i = 0; i < MAX_BULLET1; i++) {
				//존재하는 총알만 출력
				if (bullet1[i].exist == TRUE) {
					mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0)); // GDI: 펜 만들기
					oldPen = (HPEN)SelectObject(mDC, mPen); // 새로운 펜 선택하기
					mBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

					Rectangle(mDC, 
						bullet1[i].x - BULLET_SIZE - cameraX, 
						bullet1[i].y - BULLET_SIZE - cameraY, 
						bullet1[i].x + BULLET_SIZE - cameraX, 
						bullet1[i].y + BULLET_SIZE - cameraY);

					SelectObject(mDC, oldBrush);
					DeleteObject(mBrush);
					SelectObject(mDC, oldPen);
					DeleteObject(mPen); // 만든 펜 객체 삭제하기

					for (int j = 0; j < bullet1[i].c; j++) {
						mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j)); // GDI: 펜 만들기
						oldPen = (HPEN)SelectObject(mDC, mPen); // 새로운 펜 선택하기
						mBrush = CreateSolidBrush(RGB(7 * j, 7 * j, 7 * j));
						oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

						Rectangle(mDC, 
							bullet1[i].x - bullet1[i].vx * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraX, 
							bullet1[i].y - bullet1[i].vy * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraY, 
							bullet1[i].x - bullet1[i].vx * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraX, 
							bullet1[i].y - bullet1[i].vy * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraY);

						SelectObject(mDC, oldBrush);
						DeleteObject(mBrush);
						SelectObject(mDC, oldPen);
						DeleteObject(mPen); // 만든 펜 객체 삭제하기
					}
				}
			}
			// 총알2 ( 플레이어2 )
			for (int i = 0; i < MAX_BULLET1; i++) {
				//존재하는 총알만 출력
				if (bullet2[i].exist == TRUE) {
					mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0)); // GDI: 펜 만들기
					oldPen = (HPEN)SelectObject(mDC, mPen); // 새로운 펜 선택하기
					mBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

					Rectangle(mDC, 
						bullet2[i].x - BULLET_SIZE - cameraX, 
						bullet2[i].y - BULLET_SIZE - cameraY,
						bullet2[i].x + BULLET_SIZE - cameraX, 
						bullet2[i].y + BULLET_SIZE - cameraY);

					SelectObject(mDC, oldBrush);
					DeleteObject(mBrush);
					SelectObject(mDC, oldPen);
					DeleteObject(mPen); // 만든 펜 객체 삭제하기

					for (int j = 0; j < bullet2[i].c; j++) {
						mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j)); // GDI: 펜 만들기
						oldPen = (HPEN)SelectObject(mDC, mPen); // 새로운 펜 선택하기
						mBrush = CreateSolidBrush(RGB(7 * j, 7 * j, 7 * j));
						oldBrush = (HBRUSH)SelectObject(mDC, mBrush);


						Rectangle(mDC, 
							bullet2[i].x - bullet2[i].vx * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraX,
							bullet2[i].y - bullet2[i].vy * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraY,
							bullet2[i].x - bullet2[i].vx * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraX,
							bullet2[i].y - bullet2[i].vy * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraY);

						SelectObject(mDC, oldBrush);
						DeleteObject(mBrush);
						SelectObject(mDC, oldPen);
						DeleteObject(mPen); // 만든 펜 객체 삭제하기
					}
				}
			}
			// 총1 ( 플레이어1 )
			switch (player1.looking) {
			case 0://왼쪽
				switch (gunType1) {
				case 1:
					SelectObject(BMPmDC, BMP_gun1_left);
					break;
				case 2:
					SelectObject(BMPmDC, BMP_gun2_left);
					break;
				}
				TransparentBlt(mDC,
					player1.x + pWidth / 2 - cameraX - gunWidth,
					player1.y + pHeight / 2 - 6 - cameraY,
					gunWidth, gunHeight,
					BMPmDC, 0, 0, gunWidth, gunHeight, RGB(127, 127, 127));
				break;
			case 1://오른쪽
				switch (gunType1) {
				case 1:
					SelectObject(BMPmDC, BMP_gun1_right);
					break;
				case 2:
					SelectObject(BMPmDC, BMP_gun2_right);
					break;
				}
				TransparentBlt(mDC,
					player1.x + pWidth / 2 - cameraX ,
					player1.y + pHeight / 2 - 6 - cameraY,
					gunWidth, gunHeight,
					BMPmDC, 0, 0, gunWidth, gunHeight, RGB(127, 127, 127));
				break;
			}
			// 총2 ( 플레이어2 )
			switch (player2.looking) {
			case 0://왼쪽
				switch (gunType2) {
				case 1:
					SelectObject(BMPmDC, BMP_gun1_left);
					break;
				case 2:
					SelectObject(BMPmDC, BMP_gun2_left);
					break;
				}
				TransparentBlt(mDC,
					player2.x + pWidth / 2 - cameraX - gunWidth,
					player2.y + pHeight / 2 - 6 - cameraY,
					gunWidth, gunHeight,
					BMPmDC, 0, 0, gunWidth, gunHeight, RGB(127, 127, 127));
				break;
			case 1://오른쪽
				switch (gunType2) {
				case 1:
					SelectObject(BMPmDC, BMP_gun1_right);
					break;
				case 2:
					SelectObject(BMPmDC, BMP_gun2_right);
					break;
				}
				TransparentBlt(mDC,
					player2.x + pWidth / 2 - cameraX,
					player2.y + pHeight / 2 - 6 - cameraY,
					gunWidth, gunHeight,
					BMPmDC, 0, 0, gunWidth, gunHeight, RGB(127, 127, 127));
				break;
			}
			// 아이템
			for (int i = 0; i < MAX_ITEM; i++) {
				if (item[i].exist) {
					SelectObject(BMPmDC, BMP_itemBox);
					BitBlt(mDC, item[i].x - cameraX, item[i].y - cameraY, iWidth, iHeight, BMPmDC, 0, 0, SRCCOPY);
				}
			}

			// 플레이어1 정보창 100, 500, 200, 150 
			SelectObject(BMPmDC, BMP_inform);
			BitBlt(mDC, 100, 500, 200, 150, BMPmDC, 0, 0, SRCCOPY);
			//Rectangle(mDC, 100 , 500 , 300 , 650);
			wsprintf(lpOut1, L"player1");
			TextOut(mDC, 112 , 512 , lpOut1, lstrlen(lpOut1));
			wsprintf(lpOut1, L"LIFE : %d", player1.life);
			TextOut(mDC, 112 , 532 , lpOut1, lstrlen(lpOut1));
			wsprintf(lpOut1, L"bullet : %d", maxBullet1 - bullet1_count);
			TextOut(mDC, 112 , 552 , lpOut1, lstrlen(lpOut1));
			SelectObject(BMPmDC, BMP_player1_inform);
			BitBlt(mDC, 240, 515, 45, 45, BMPmDC, 0, 0, SRCCOPY);

			// 플레이어2 정보창 570 , 500, 200, 150 
			SelectObject(BMPmDC, BMP_inform);
			BitBlt(mDC, 570, 500, 200, 150, BMPmDC, 0, 0, SRCCOPY);
			wsprintf(lpOut2, L"player2");
			TextOut(mDC, 582 , 512 , lpOut2, lstrlen(lpOut2));
			wsprintf(lpOut2, L"LIFE : %d", player2.life);
			TextOut(mDC, 582 , 532 , lpOut2, lstrlen(lpOut2));
			wsprintf(lpOut2, L"bullet : %d", maxBullet2 - bullet2_count);
			TextOut(mDC, 582 , 552 , lpOut2, lstrlen(lpOut2));
			SelectObject(BMPmDC, BMP_player2_inform);
			BitBlt(mDC, 710, 515, 45, 45, BMPmDC, 0, 0, SRCCOPY);
		}
		//정지메뉴
		if (pause == TRUE) {
			SelectObject(BMPmDC, BMP_pauseMenu);
			BitBlt(mDC, 0, 0, rt.right, rt.bottom, BMPmDC, 0, 0, SRCCOPY);	
		}
		//게임 종료
		if (end == TRUE) {
			// 종료 배경
			Rectangle(mDC, 0, 0, rt.right, rt.bottom);
			if (player1.life == 0)
			{
				wsprintf(lpOut1, L"Player 2 Win!");
			}
			else if (player2.life == 0)
			{
				wsprintf(lpOut1, L"Player 1 Win!");
			}
			TextOut(mDC, 400, 280, lpOut1, lstrlen(lpOut1));
			wsprintf(lpOut1, L"Click to Back to Menu");
			TextOut(mDC, 400, 300, lpOut1, lstrlen(lpOut1));
		}
		//	----mDC 그리기 종료----
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);		//hDC에 mDC내용 복사
		// 메모리 DC와 비트맵 해제
		SelectObject(mDC, oldBitmap);
		DeleteObject(memBitmap);
		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		DeleteDC(BMPmDC);
		DeleteObject(BMP_player1_left_stand);	//비트맵 삭제
		DeleteObject(BMP_player1_right_stand);	//비트맵 삭제
		for (int i = 0; i < 4; i++) {
			DeleteObject(BMP_player1_left_walk[i]);	//비트맵 삭제
			DeleteObject(BMP_player1_right_walk[i]);	//비트맵 삭제
		}
		DeleteObject(BMP_player2_left_stand);	//비트맵 삭제
		DeleteObject(BMP_player2_right_stand);	//비트맵 삭제
		for (int i = 0; i < 4; i++) {
			DeleteObject(BMP_player2_left_walk[i]);	//비트맵 삭제
			DeleteObject(BMP_player2_right_walk[i]);	//비트맵 삭제
		}

		PlaySound(NULL, NULL, NULL);
		channel->stop();
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}