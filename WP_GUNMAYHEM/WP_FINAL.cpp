#include "Default.h"


void reload1() {
	bullet1_count = 0;
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet1[i].exist == FALSE)
			bullet1[i].c = 0;
	}
}
void gunFire1(int way, BULLET bullet[]) {
	//�ҷ�ī��Ʈ -> �ҷ� ī��Ʈ�� �ƽ��ҷ��� ������ �߻簡 �ȵ� (�Ѿ� ����)
	//�ҷ� ī��Ʈ�� ź�� ������ ���� ���� -> ���� �� (��Ȱ��ȭ�� �Ѿ� �ϳ��� Ȱ��ȭ ��Ŵ) -> �ҷ� ī��Ʈ ����
	if (bullet1_count < maxBullet1) {
		for (int i = 0; i < MAX_BULLET1; i++) {
			if (bullet[i].exist == FALSE && bullet[i].c == 0)
			{
				bullet[i].exist = TRUE;
				bullet[i].x = player1.x + pWidth / 2;
				bullet[i].y = player1.y + pHeight / 2;
				bullet[i].travelDistance = 0; // �ʱ�ȭ

				//�Ѿ� ���� ���
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
					ssystem->playSound(sound1, 0, false, &channel); //--- 1�� ���� ���
					break;
				case 2:
					sound2->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound2, 0, false, &channel); //--- 1�� ���� ���
					break;
				}
				break;	//�ѹ� ��� �ݺ��� ����
			}
		}
	}
}
void update_bullet1() {
	//1. �Ѿ�1 �浹üũ
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet1[i].exist == TRUE) {
			//��Ÿ� ���޽�
			if (bullet1[i].travelDistance > range1) {
				bullet1[i].exist = FALSE;
			}
			else {
				if (bullet1[i].x + BULLET_SIZE >= player2.x && bullet1[i].x - BULLET_SIZE <= player2.x + pWidth &&
					bullet1[i].y + BULLET_SIZE >= player2.y && bullet1[i].y - BULLET_SIZE <= player2.y + pHeight) {

					player2.combo++;
					comboTime2 = 0;
					player2.speed = (7 * player2.combo) * bullet1[i].vx;
					bullet1[i].exist = FALSE;
				}
			}
		}
	}
	//2. �Ѿ�1 �̵� -> �÷��̾��� ���⿡ ����(looking)
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet1[i].exist == TRUE) {
			bullet1[i].x += bullet1[i].vx * 13; // �Ѿ� �ӵ� ����
			bullet1[i].travelDistance += 13;    // �̵� �Ÿ� �߰�
			if (bullet1[i].c < 30) {
				bullet1[i].c++;
			}
		}
	}
	//�޺��ʱ�ȭ -> �޺��� 1 �̻��̵Ǹ� �ð��� �˻��Ѵ�
	if (player2.combo > 0) {
		comboTime2++;
		//player2.combo++;

		if (comboTime2 > 200) {
			player2.combo = 0;
			comboTime2 = 0;
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
				bullet[i].travelDistance = 0; // �ʱ�ȭ

				//�Ѿ� ���� ���
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
					ssystem->playSound(sound1, 0, false, &channel); //--- 1�� ���� ���
					break;
				case 2:
					sound2->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound2, 0, false, &channel); //--- 1�� ���� ���
					break;
				}
				break;	//�ѹ� ��� �ݺ��� ����
			}
		}
	}
}
void update_bullet2() {
	//1. �Ѿ�2 �浹üũ
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet2[i].exist == TRUE) {
			//��Ÿ� ���� ��
			if (bullet2[i].travelDistance > range2) {
				bullet2[i].exist = FALSE;
			}
			else {
				if (bullet2[i].x + BULLET_SIZE >= player1.x && bullet2[i].x - BULLET_SIZE <= player1.x + pWidth &&
					bullet2[i].y + BULLET_SIZE >= player1.y && bullet2[i].y - BULLET_SIZE <= player1.y + pHeight) {

					player1.combo++;
					comboTime1 = 0;
					player1.speed = (7 * player1.combo) * bullet2[i].vx;
					bullet2[i].exist = FALSE;
				}
			}
		}
	}
	//2. �Ѿ� �̵� -> �÷��̾��� ���⿡ ����(looking)
	for (int i = 0; i < MAX_BULLET1; i++) {
		if (bullet2[i].exist == TRUE) {
			bullet2[i].x += bullet2[i].vx * 13; //�Ѿ� �ӵ� ����
			bullet2[i].travelDistance += 13;    // �̵� �Ÿ� �߰�
			if (bullet2[i].c < 30) {
				bullet2[i].c++;
			}
		}
	}
	//�޺��ʱ�ȭ -> �޺��� 1 �̻��̵Ǹ� �ð��� �˻��Ѵ�
	if (player1.combo > 0) {
		comboTime1++;
		if (comboTime1 > 200) {
			player1.combo = 0;
			comboTime1 = 0;
		}
	}
}

void regenerate_1() {
	player1.acceleration = 0.0f;	// ���� ���ӵ�
	player1.speed = 0.0f;			// ���� �ӵ�
	player1.isMoving = FALSE;		// �̵� ������ ����
	player1.falling = FALSE;
	player1.jumping = FALSE;
	player1.x = 420;
	player1.y = -200;
	player1.life--;
	player1.combo = 0;

	gunType1 = 1;
	range1 = 300;
	maxBullet1 = 20;
	reload1();
}
void regenerate_2() {
	player2.acceleration = 0.0f;	// ���� ���ӵ�
	player2.speed = 0.0f;			// ���� �ӵ�
	player2.isMoving = FALSE;		// �̵� ������ ����
	player2.falling = FALSE;
	player2.jumping = FALSE;
	player2.x = 420;
	player2.y = -200;
	player2.life--;
	player2.combo = 0;

	gunType2 = 1;
	range2 = 300;
	maxBullet2 = 20;
	reload2();
}
void update_player1() {
	// update speed :  player 1
	if (player1.isMoving) {
		// ���ӵ��� �ӵ��� ����
		player1.speed += player1.acceleration;
		// �ӵ��� �ִ� �ӵ��� �ʰ����� �ʵ��� ����
		player1.speed = max(-MAX_SPEED, min(player1.speed, MAX_SPEED));
	}
	else {
		// �������� �����Ͽ� ���� ����
		if (player1.speed > 0) {
			player1.speed = max(0.0f, player1.speed - FRICTION);
		}
		else if (player1.speed < 0) {
			player1.speed = min(0.0f, player1.speed + FRICTION);
		}
	}
	// �ӵ��� ���� �÷��̾� ��ġ ������Ʈ
	player1.x += player1.speed;

	// jumping or falling
	if (player1.jumping == TRUE) {
		player1.jumpHeight = (player1.jumpTime * player1.jumpTime - player1.jumpPower * player1.jumpTime) * 4.f;
		player1.jumpTime += 0.2f;
		player1.y = (player1.jstartY + (int)player1.jumpHeight);

		//����Ÿ���� �����Ŀ����� Ŀ���� ��, �������� ���� ���� �浹üũ�Ѵ�.
		if (player1.jumpHeight <= (player1.jumpTime * player1.jumpTime - player1.jumpPower * player1.jumpTime) * 4.f) {
			for (int i = 0; i < mapCount; i++) {
				if (player1.x + pWidth > maps[i].x && player1.x < maps[i].x + mapWidth &&
					player1.y + pHeight > maps[i].y && player1.y + pHeight < maps[i].y + 15)
				{
					player1.falling = FALSE;
					player1.jumping = FALSE;
					jumpCount1 = 0;
					player1.y = maps[i].y - pHeight;
					break;
				}
			}
		}

	}
	if (player1.jumping == FALSE) {
		// 1. �÷��̾��� ��ġ �˻� -> �������� ��������, �ȶ������� �Ǵ� -> �ٿ�ī��Ʈ ���϶��� �Ǵ� x
		if (player1.downCount == 0) {
			for (int i = 0; i < mapCount; i++) {
				// �÷��̾ �� ���� ���� ��
				if (player1.x + pWidth > maps[i].x && player1.x < maps[i].x + mapWidth &&
					player1.y + pHeight > maps[i].y && player1.y + pHeight < maps[i].y + 15)
				{
					player1.jumping = FALSE;
					player1.falling = FALSE;
					jumpCount1 = 0;
					if (player1.y + pHeight > maps[i].y) {
						player1.y = maps[i].y - pHeight;
					}
					break; // �� ���� ������ �� �̻� �˻��� �ʿ䰡 �����Ƿ� �ݺ����� �������´�.
				}
				else {
					// �÷��̾ �� ���� ���� ��
					if (player1.falling == FALSE)	// �������� ���� ���� ���¿��ٸ�
					{
						player1.downTime = 0;
						player1.downHeight = 0;
						player1.fstartY = player1.y; // ���� ��ǥ�� �����ϰ�
						player1.falling = TRUE;		// �������� ���·� �����.
					}
				}
			}
		}
		// 2. �������� ���̸� -> ��ġ����, �浹üũ -> �ٿ�ī��Ʈ ���϶��� �浹üũ x
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
						jumpCount1 = 0;
						player1.y = maps[i].y - pHeight;
						break;
					}
				}
			}
		}
	}
	// �ٿ�ī��Ʈ -> 0�̸� ���� x / 0�� �ƴҶ� ���� ���� / N���޽� 0���� �ʱ�ȭ
	if (player1.downCount > 0) {
		player1.downCount++;
		if (player1.downCount > 30) {
			player1.downCount = 0;
		}
	}

	//������ �浹�˻�
	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i].exist == TRUE) {
			if (player1.x < item[i].x + iWidth / 2 && item[i].x + iWidth / 2 < player1.x + pWidth &&
				player1.y < item[i].y + iHeight / 2 && item[i].y + iHeight / 2 < player1.y + pHeight) {

				channel->stop();
				itemSound->setMode(FMOD_LOOP_OFF);
				ssystem->playSound(itemSound, 0, false, &channel); //--- 1�� ���� ���
				item[i].exist = FALSE;
				//�����ۿ� ���� �� ��ȭ
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
		// ���ӵ��� �ӵ��� ����
		player2.speed += player2.acceleration;
		// �ӵ��� �ִ� �ӵ��� �ʰ����� �ʵ��� ����
		player2.speed = max(-MAX_SPEED, min(player2.speed, MAX_SPEED));
	}
	else {
		// �������� �����Ͽ� ���� ����
		if (player2.speed > 0) {
			player2.speed = max(0.0f, player2.speed - FRICTION);
		}
		else if (player2.speed < 0) {
			player2.speed = min(0.0f, player2.speed + FRICTION);
		}
	}
	// �ӵ��� ���� �÷��̾� ��ġ ������Ʈ
	player2.x += player2.speed;

	// jumping or falling
	if (player2.jumping == TRUE) {
		player2.jumpHeight = (player2.jumpTime * player2.jumpTime - player2.jumpPower * player2.jumpTime) * 4.f;
		player2.jumpTime += 0.2f;
		player2.y = (player2.jstartY + (int)player2.jumpHeight);

		//����Ÿ���� �����Ŀ����� Ŀ���� ��, �������� ���� ���� �浹üũ�Ѵ�.
		if (player2.jumpHeight <= (player2.jumpTime * player2.jumpTime - player2.jumpPower * player2.jumpTime) * 4.f) {
			for (int i = 0; i < mapCount; i++) {
				if (player2.x + pWidth > maps[i].x && player2.x < maps[i].x + mapWidth &&
					player2.y + pHeight > maps[i].y && player2.y + pHeight < maps[i].y + 15)
				{
					player2.falling = FALSE;
					player2.jumping = FALSE;
					jumpCount2 = 0;
					player2.y = maps[i].y - pHeight;
					break;
				}
			}
		}

	}
	if (player2.jumping == FALSE) {
		// 1. �÷��̾��� ��ġ �˻� -> �������� ��������, �ȶ������� �Ǵ� -> �ٿ�ī��Ʈ ���϶��� �Ǵ� x
		if (player2.downCount == 0) {
			for (int i = 0; i < mapCount; i++) {
				// �÷��̾ �� ���� ���� ��
				if (player2.x + pWidth > maps[i].x && player2.x < maps[i].x + mapWidth &&
					player2.y + pHeight > maps[i].y && player2.y + pHeight < maps[i].y + 15)
				{
					player2.jumping = FALSE;
					player2.falling = FALSE;
					jumpCount2 = 0;
					if (player2.y + pHeight > maps[i].y) {
						player2.y = maps[i].y - pHeight;
					}
					break; // �� ���� ������ �� �̻� �˻��� �ʿ䰡 �����Ƿ� �ݺ����� �������´�.
				}
				else {
					// �÷��̾ �� ���� ���� ��
					if (player2.falling == FALSE)	// �������� ���� ���� ���¿��ٸ�
					{
						player2.downTime = 0;
						player2.downHeight = 0;
						player2.fstartY = player2.y; // ���� ��ǥ�� �����ϰ�
						player2.falling = TRUE;		// �������� ���·� �����.
					}
				}
			}
		}
		// 2. �������� ���̸� -> ��ġ����, �浹üũ -> �ٿ�ī��Ʈ ���϶��� �浹üũ x
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
						jumpCount2 = 0;
						player2.y = maps[i].y - pHeight;
						break;
					}
				}
			}
		}
	}
	// �ٿ�ī��Ʈ -> 0�̸� ���� x / 0�� �ƴҶ� ���� ���� / N���޽� 0���� �ʱ�ȭ
	if (player2.downCount > 0) {
		player2.downCount++;
		if (player2.downCount > 30) {
			player2.downCount = 0;
		}
	}

	//������ �浹�˻�
	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i].exist == TRUE) {
			if (player2.x < item[i].x + iWidth / 2 && item[i].x + iWidth / 2 < player2.x + pWidth &&
				player2.y < item[i].y + iHeight / 2 && item[i].y + iHeight / 2 < player2.y + pHeight) {

				channel->stop();
				itemSound->setMode(FMOD_LOOP_OFF);
				ssystem->playSound(itemSound, 0, false, &channel); //--- 1�� ���� ���
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
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // ���� Ŭ���� ����� �� �ְ� ��Ÿ�� ����
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
		//else { //�޽����� ���� ��쿡 ȣ�� }
	}
	return msg.wParam;
}

//winmain ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//WndProc �⺻ Ȱ�� ����
	PAINTSTRUCT ps;
	HDC hDC, mDC;
	static HDC BMPmDC;
	HBITMAP hBitmap;			// ������۸��� ���� hBitmap
	//static RECT rt;			// ��ü ȭ���� �簢���� �޾ƿ���

	HPEN mPen, oldPen;
	HBRUSH mBrush, oldBrush;	// ������۸��� ���� mBrush

	static TCHAR lpOut1[100];	// wsprintf : �ؽ�Ʈ ����� ����
	static TCHAR lpOut2[100];	// wsprintf : �ؽ�Ʈ ����� ����
	static TCHAR Combo1[100];	// wsprintf : �ؽ�Ʈ ����� ����
	static TCHAR Combo2[100];	// wsprintf : �ؽ�Ʈ ����� ����

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

	//--- �޽��� ó���ϱ�
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

		//����
		result = FMOD::System_Create(&ssystem); //--- ���� �ý��� ����
		if (result != FMOD_OK)
			exit(0);
		ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata); //--- ���� �ý��� �ʱ�ȭ
		ssystem->createSound("744_pistol3.wav", FMOD_LOOP_NORMAL, 0, &sound1); //--- 1�� ���� ���� �� ����
		ssystem->createSound("761_snipe1.wav", FMOD_LOOP_NORMAL, 0, &sound2); //--- 2�� ���� ���� �� ����
		ssystem->createSound("1_menu.wav", FMOD_LOOP_NORMAL, 0, &clickSound);
		ssystem->createSound("737_item.wav", FMOD_LOOP_NORMAL, 0, &itemSound);
		PlaySound(TEXT("787_music555.wav"), NULL, SND_ASYNC | SND_LOOP);

		SetTimer(hWnd, 3, 100, NULL); // �ִϸ��̼� Ÿ�̸� (200ms ����)
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
		//player2 �̵�
		case 'w':
		{
			if (jumpCount1 < 2)
			{
				jumpCount1++;
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
			//�Ʒ����� ���� -> ���� ���� ���� �Ʒ����� ����
			if (player1.downCount == 0) {
				player1.downCount = 1;

				player1.downTime = 0;
				player1.downHeight = 0;
				player1.fstartY = player1.y; // ���� ��ǥ�� �����ϰ�

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
	{	//�ٸ� �޼����� �߻��Ҷ�, keyup�޽����� ���� �۵����� �ʴ� ���װ� ���� -> ������ ����
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
		//player2 �̵�
		case VK_UP:
		{
			if (jumpCount2 < 2)
			{
				jumpCount2++;
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
			//�Ʒ����� ���� -> ���� ���� ���� �Ʒ����� ����
			if (player2.downCount == 0) {
				player2.downCount = 1;

				player2.downTime = 0;
				player2.downHeight = 0;
				player2.fstartY = player2.y; // ���� ��ǥ�� �����ϰ�

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
			player1.isMoving = FALSE; // Ű�� ���� �� �̵� ����
			break;
		case 'D':
			player1.isMoving = FALSE; // Ű�� ���� �� �̵� ����
			break;
		case VK_LEFT:
			player2.isMoving = FALSE; // Ű�� ���� �� �̵� ����
			break;
		case VK_RIGHT:
			player2.isMoving = FALSE; // Ű�� ���� �� �̵� ����
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		channel->stop();
		clickSound->setMode(FMOD_LOOP_OFF);
		ssystem->playSound(clickSound, 0, false, &channel); //--- 1�� ���� ���
			
		//���� �����϶� ������ ��ǥ�� �簢������ �ش��ϸ� ���� or ����
		//���� �����϶� ������ ��ǥ�� ���� �ٽý��� or ����
		if (end == TRUE) {
			end = FALSE;
			start = FALSE;
			pause = FALSE;
		}
		else if (start == FALSE && pause == FALSE) {
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			POINT pt = { mx, my };
			if (PtInRect(&startMenu, pt)) {	//���۹�ư Ŭ�� -> �ʱ�ȭ ����, Ÿ�̸� ����
				start = TRUE;
				//�Ѿ�1 �ʱ�ȭ
				for (int i = 0; i < MAX_BULLET1; i++) {
					bullet1[i].exist = FALSE;
					bullet1[i].c = 0;
				}
				bullet1_count = 0;
				gunType1 = 1;
				range1 = 300;
				maxBullet1 = 20;

				//�Ѿ�2 �ʱ�ȭ
				for (int i = 0; i < MAX_BULLET1; i++) {
					bullet2[i].exist = FALSE;
					bullet2[i].c = 0;
				}
				bullet2_count = 0;
				gunType2 = 1;				
				range2 = 300;
				maxBullet2 = 20;

				//������ �ʱ�ȭ
				for (int i = 0; i < MAX_ITEM; i++) {
					item[i].exist = FALSE;
				}

				//�÷��̾�1 �ʱ�ȭ
				if (TRUE) {
					//�÷��̾� �ʱ�ȭ
					player1.exist = TRUE;
					player1.x = 150;
					player1.y = -200;
					player1.ax = 0;
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

					//�ӵ�, ���ӵ� ����
					player1.acceleration = 0.0f;	// ���� ���ӵ�
					player1.speed = 0.0f;			// ���� �ӵ�
					player1.isMoving = FALSE;		// �̵� ������ ����
				}
				//�÷��̾�2 �ʱ�ȭ
				if (TRUE) {
					//�÷��̾� �ʱ�ȭ
					player2.exist = TRUE;
					player2.x = 650;
					player2.y = -200;
					player2.ax = 0;
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

					//�ӵ�, ���ӵ� ����
					player2.acceleration = 0.0f;	// ���� ���ӵ�
					player2.speed = 0.0f;			// ���� �ӵ�
					player2.isMoving = FALSE;		// �̵� ������ ����
				}
				//�� �ʱ�ȭ
				if (TRUE) {
					if (mapType == 0) {
						maps[0] = { 330, 170 };	//1��

						maps[1] = { 160, 300 };	//2��
						maps[2] = { 510, 300 };

						maps[3] = { 70, 420 };	//3��
						maps[4] = { 600, 420 };
					}
					else { //if (mapType == 1)
						maps[0] = { 160, 170 };	//1��
						maps[1] = { 510, 170 };

						maps[2] = { 330, 300 }; //2��

						maps[3] = { 70, 420 };	//3��
						maps[4] = { 600, 420 };
					}
				}

				//������Ʈ Ÿ�̸�
				SetTimer(hWnd, 1, 15, NULL);
				//������ ���� Ÿ�̸�
				SetTimer(hWnd, 2, 7000, NULL);
			}
			else if (PtInRect(&endMenu, pt)) {
				DeleteDC(BMPmDC);
				DeleteObject(BMP_player1_left_stand);	//��Ʈ�� ����
				DeleteObject(BMP_player1_right_stand);	//��Ʈ�� ����
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player1_left_walk[i]);	//��Ʈ�� ����
					DeleteObject(BMP_player1_right_walk[i]);	//��Ʈ�� ����
				}
				DeleteObject(BMP_player2_left_stand);	//��Ʈ�� ����
				DeleteObject(BMP_player2_right_stand);	//��Ʈ�� ����
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player2_left_walk[i]);	//��Ʈ�� ����
					DeleteObject(BMP_player2_right_walk[i]);	//��Ʈ�� ����
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
				DeleteObject(BMP_player1_left_stand);	//��Ʈ�� ����
				DeleteObject(BMP_player1_right_stand);	//��Ʈ�� ����
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player1_left_walk[i]);	//��Ʈ�� ����
					DeleteObject(BMP_player1_right_walk[i]);	//��Ʈ�� ����
				}
				DeleteObject(BMP_player2_left_stand);	//��Ʈ�� ����
				DeleteObject(BMP_player2_right_stand);	//��Ʈ�� ����
				for (int i = 0; i < 4; i++) {
					DeleteObject(BMP_player2_left_walk[i]);	//��Ʈ�� ����
					DeleteObject(BMP_player2_right_walk[i]);	//��Ʈ�� ����
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
			if (player1.isMoving && player1.speed > 0) { // �ݴ� �������� ������ ����
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
			if (player1.isMoving && player1.speed < 0) { // �ݴ� �������� ������ ����
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
			if (player2.isMoving && player2.speed > 0) { // �ݴ� �������� ������ ����
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
			if (player2.isMoving && player2.speed < 0) { // �ݴ� �������� ������ ����
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
				regenerate_1();
			}
			// check player2 out
			if (player2.y > rt.bottom) {
				regenerate_2();
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

			//update item create & move -> �ϴÿ��� ������	
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
							break; // �� ���� ������ �� �̻� �˻��� �ʿ䰡 �����Ƿ� �ݺ����� �������´�.
						}
						else {
							if (item[i].falling == FALSE)	// �������� ���� ���� ���¿��ٸ�
							{
								item[i].downTime = 0;
								item[i].downHeight = 0;
								item[i].fstartY = item[i].y; // ���� ��ǥ�� �����ϰ�
								item[i].falling = TRUE;		// �������� ���·� �����.
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
		{	//������ ���� Ÿ�̸�
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
			// �ִϸ��̼� Ÿ�̸�
			if (player1.isMoving) {
				player1_anim_frame = (player1_anim_frame + 1) % 4; // 0, 1, 2, 3 �ݺ�
			}
			if (player2.isMoving) {
				player2_anim_frame = (player2_anim_frame + 1) % 4; // 0, 1, 2, 3 �ݺ�
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

		mDC = CreateCompatibleDC(hDC);									//�޸�DC ����
		HBITMAP memBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mDC, memBitmap);

		//	----�׸���� ��� mDC�� �׸���----
		Rectangle(mDC, 0, 0, rt.right, rt.bottom);	//ȭ�� ��ĥ

		//���۸޴� -> ����, ���� ���� -> Ÿ�̸ӽ��� �� �� ��� �� Ȱ��ȭ
		if (start == FALSE) {
			SelectObject(BMPmDC, BMP_startMenu);
			BitBlt(mDC, 0, 0, rt.right, rt.bottom, BMPmDC, 0, 0, SRCCOPY);
			wsprintf(lpOut1, L"Type : %d", mapType);
			TextOut(mDC, changeMapMenu.left + 140, changeMapMenu.top + 50, lpOut1, lstrlen(lpOut1));
		}
		//�׸���
		if (start == TRUE) {
			//���ȭ��(�� Ÿ�Կ� ���� �ٸ���)
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
			//��(5���� ��)
			SelectObject(BMPmDC, BMP_ground1);
			for (int i = 0; i < mapCount; i++) {
				BitBlt(mDC, maps[i].x - cameraX, maps[i].y - 5 - cameraY, mapWidth, mapHeight, BMPmDC, 0, 0, SRCCOPY);
				//Rectangle(mDC, maps[i].x - cameraX, maps[i].y - 5 - cameraY, maps[i].x + mapWidth - cameraX, maps[i].y + mapHeight - cameraY);
			}

			// �÷��̾� 1
			switch (player1.looking) {
			case 0:
				if (player1.isMoving) {
					SelectObject(BMPmDC, BMP_player1_left_walk[player1_anim_frame]);
				}
				else {
					SelectObject(BMPmDC, BMP_player1_left_stand);
				}
				break;
			case 1:
				if (player1.isMoving) {
					SelectObject(BMPmDC, BMP_player1_right_walk[player1_anim_frame]);
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

			// �÷��̾� 2
			switch (player2.looking) {
			case 0:
				if (player2.isMoving) {
					SelectObject(BMPmDC, BMP_player2_left_walk[player2_anim_frame]);
				}
				else {
					SelectObject(BMPmDC, BMP_player2_left_stand);
				}
				break;
			case 1:
				if (player2.isMoving) {
					SelectObject(BMPmDC, BMP_player2_right_walk[player2_anim_frame]);
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

			//�÷��̾� 1 �Ÿ�ǥ��(�� ������ ������ ��)
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

			//�÷��̾� 2 �Ÿ�ǥ��(�� ������ ������ ��)
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
			// �Ѿ�1 ( �÷��̾�1 )
			for (int i = 0; i < MAX_BULLET1; i++) {
				//�����ϴ� �Ѿ˸� ���
				if (bullet1[i].exist == TRUE) {
					mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0)); // GDI: �� �����
					oldPen = (HPEN)SelectObject(mDC, mPen); // ���ο� �� �����ϱ�
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
					DeleteObject(mPen); // ���� �� ��ü �����ϱ�

					for (int j = 0; j < bullet1[i].c; j++) {
						mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j)); // GDI: �� �����
						oldPen = (HPEN)SelectObject(mDC, mPen); // ���ο� �� �����ϱ�
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
						DeleteObject(mPen); // ���� �� ��ü �����ϱ�
					}
				}
			}
			// �Ѿ�2 ( �÷��̾�2 )
			for (int i = 0; i < MAX_BULLET1; i++) {
				//�����ϴ� �Ѿ˸� ���
				if (bullet2[i].exist == TRUE) {
					mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0)); // GDI: �� �����
					oldPen = (HPEN)SelectObject(mDC, mPen); // ���ο� �� �����ϱ�
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
					DeleteObject(mPen); // ���� �� ��ü �����ϱ�

					for (int j = 0; j < bullet2[i].c; j++) {
						mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j)); // GDI: �� �����
						oldPen = (HPEN)SelectObject(mDC, mPen); // ���ο� �� �����ϱ�
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
						DeleteObject(mPen); // ���� �� ��ü �����ϱ�
					}
				}
			}
			// ��1 ( �÷��̾�1 )
			switch (player1.looking) {
			case 0://����
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
			case 1://������
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
			// ��2 ( �÷��̾�2 )
			switch (player2.looking) {
			case 0://����
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
			case 1://������
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
			// ������
			for (int i = 0; i < MAX_ITEM; i++) {
				if (item[i].exist) {
					SelectObject(BMPmDC, BMP_itemBox);
					BitBlt(mDC, item[i].x - cameraX, item[i].y - cameraY, iWidth, iHeight, BMPmDC, 0, 0, SRCCOPY);
				}
			}

			// �÷��̾�1 ����â 100, 500, 200, 150 
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

			// �÷��̾�2 ����â 570 , 500, 200, 150 
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
		//�����޴�
		if (pause == TRUE) {
			SelectObject(BMPmDC, BMP_pauseMenu);
			BitBlt(mDC, 0, 0, rt.right, rt.bottom, BMPmDC, 0, 0, SRCCOPY);	
		}
		//���� ����
		if (end == TRUE) {
			// ���� ���
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
		//	----mDC �׸��� ����----
		BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);		//hDC�� mDC���� ����
		// �޸� DC�� ��Ʈ�� ����
		SelectObject(mDC, oldBitmap);
		DeleteObject(memBitmap);
		DeleteDC(mDC);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		DeleteDC(BMPmDC);
		DeleteObject(BMP_player1_left_stand);	//��Ʈ�� ����
		DeleteObject(BMP_player1_right_stand);	//��Ʈ�� ����
		for (int i = 0; i < 4; i++) {
			DeleteObject(BMP_player1_left_walk[i]);	//��Ʈ�� ����
			DeleteObject(BMP_player1_right_walk[i]);	//��Ʈ�� ����
		}
		DeleteObject(BMP_player2_left_stand);	//��Ʈ�� ����
		DeleteObject(BMP_player2_right_stand);	//��Ʈ�� ����
		for (int i = 0; i < 4; i++) {
			DeleteObject(BMP_player2_left_walk[i]);	//��Ʈ�� ����
			DeleteObject(BMP_player2_right_walk[i]);	//��Ʈ�� ����
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
	return DefWindowProc(hWnd, iMsg, wParam, lParam); //--- ���� �� �޽��� ���� ������ �޽����� OS��
}