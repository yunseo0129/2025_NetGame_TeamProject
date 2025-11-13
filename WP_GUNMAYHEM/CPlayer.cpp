#include "CPlayer.h"

CPlayer::CPlayer()
{
	exist = TRUE;
	x = 420;
	y = -200;
	looking = 0;
	life = MAXLIFE;
	
	downCount = 0;
	falling = TRUE; // 처음엔 낙하 상태
	jumping = FALSE;
	jumpCount = 0;
	jumpPower = 12.f;
	acceleration = 0.0f;
	speed = 0.0f;
	isMoving = FALSE;
	
	combo = 0;
	comboTime = 0;
	anim_frame = 0;
	anim_timer = 0;
	gunType = GUN_TYPE_PISTOL;
	range = 300;
	maxBullet = 20;
	reload();
}

void CPlayer::Draw(HDC mDC)
{
	// 1. 플레이어 그리기 
	HBITMAP hBitmapToDraw = BMP_player_right_stand[playerType];
	switch (looking) {
	case 0: // Left
		if (isMoving) hBitmapToDraw = BMP_player_left_walk[playerType][anim_frame];
		else hBitmapToDraw = BMP_player_left_stand[playerType];
		break;
	case 1: // Right
		if (isMoving) hBitmapToDraw = BMP_player_right_walk[playerType][anim_frame];
		else hBitmapToDraw = BMP_player_right_stand[playerType];
		break;
	}

	SelectObject(g_BMPmDC, hBitmapToDraw);
	TransparentBlt(mDC, x - cameraX, y - cameraY, pWidth, pHeight, g_BMPmDC, 0, 0, pWidth, pHeight, RGB(127, 127, 127));

	if (combo > 0) {
		TCHAR ComboText[20];
		wsprintf(ComboText, L"Hit %d", combo);
		TextOut(mDC, x + pWidth - cameraX, y - 10 - cameraY, ComboText, lstrlen(ComboText));
	}

	// 2. 총 그리기 (기존 WM_PAINT 로직)
	HBITMAP hGunBitmap = BMP_gun1_right; // 기본값
	int gunPosX = x + pWidth / 2 - cameraX;
	int gunPosY = y + pHeight / 2 - 6 - cameraY;

	switch (looking) {
	case 0: // Left
		hGunBitmap = (gunType == GUN_TYPE_PISTOL) ? BMP_gun1_left : BMP_gun2_left;
		gunPosX = x + pWidth / 2 - cameraX - gunWidth;
		break;
	case 1: // Right
		hGunBitmap = (gunType == GUN_TYPE_PISTOL) ? BMP_gun1_right : BMP_gun2_right;
		break;
	}
	SelectObject(g_BMPmDC, hGunBitmap);
	TransparentBlt(mDC, gunPosX, gunPosY, gunWidth, gunHeight, g_BMPmDC, 0, 0, gunWidth, gunHeight, RGB(127, 127, 127));


	//// 3. 총알 그리기 (기존 WM_PAINT 로직)
	//for (int i = 0; i < MAX_BULLET; i++) {
	//	if (bullet[i].exist == TRUE) {
	//		HPEN mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	//		HPEN oldPen = (HPEN)SelectObject(mDC, mPen);
	//		HBRUSH mBrush = CreateSolidBrush(RGB(0, 0, 0));
	//		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

	//		Rectangle(mDC,
	//				  bullet[i].x - BULLET_SIZE - cameraX,
	//				  bullet[i].y - BULLET_SIZE - cameraY,
	//				  bullet[i].x + BULLET_SIZE - cameraX,
	//				  bullet[i].y + BULLET_SIZE - cameraY);

	//		SelectObject(mDC, oldBrush);
	//		DeleteObject(mBrush);
	//		SelectObject(mDC, oldPen);
	//		DeleteObject(mPen);

	//		// 그라데이션 (꼬리)
	//		for (int j = 0; j < bullet[i].c; j++) {
	//			mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j));
	//			oldPen = (HPEN)SelectObject(mDC, mPen);
	//			mBrush = CreateSolidBrush(RGB(7 * j, 7 * j, 7 * j));
	//			oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

	//			Rectangle(mDC,
	//					  bullet[i].x - bullet[i].vx * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraX,
	//					  bullet[i].y - bullet[i].vy * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraY,
	//					  bullet[i].x - bullet[i].vx * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraX,
	//					  bullet[i].y - bullet[i].vy * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraY);

	//			SelectObject(mDC, oldBrush);
	//			DeleteObject(mBrush);
	//			SelectObject(mDC, oldPen);
	//			DeleteObject(mPen);
	//		}
	//	}
	//}

	//// 4. 정보창 그리기
	//switch (playerType) { 
	//case 1: { 
	//	// 플레이어1 정보창 100, 500, 200, 150 
	//	SelectObject(g_BMPmDC, BMP_inform);
	//	BitBlt(mDC, 100, 500, 200, 150, g_BMPmDC, 0, 0, SRCCOPY);
	//	//Rectangle(mDC, 100 , 500 , 300 , 650);
	//	wsprintf(lpOut, L"player1");
	//	TextOut(mDC, 112, 512, lpOut, lstrlen(lpOut));
	//	wsprintf(lpOut, L"LIFE : %d", life);
	//	TextOut(mDC, 112, 532, lpOut, lstrlen(lpOut));
	//	wsprintf(lpOut, L"bullet : %d", maxBullet - bullet_count);
	//	TextOut(mDC, 112, 552, lpOut, lstrlen(lpOut));
	//	SelectObject(g_BMPmDC, BMP_player1_inform);
	//	BitBlt(mDC, 240, 515, 45, 45, g_BMPmDC, 0, 0, SRCCOPY);
	//	break; 
	//}
	//case 2: { 
	//	// 플레이어2 정보창 570 , 500, 200, 150 
	//	SelectObject(g_BMPmDC, BMP_inform);
	//	BitBlt(mDC, 570, 500, 200, 150, g_BMPmDC, 0, 0, SRCCOPY);
	//	wsprintf(lpOut, L"player2");
	//	TextOut(mDC, 582, 512, lpOut, lstrlen(lpOut));
	//	wsprintf(lpOut, L"LIFE : %d", life);
	//	TextOut(mDC, 582, 532, lpOut, lstrlen(lpOut));
	//	wsprintf(lpOut, L"bullet : %d", maxBullet - bullet_count);
	//	TextOut(mDC, 582, 552, lpOut, lstrlen(lpOut));
	//	SelectObject(g_BMPmDC, BMP_player2_inform);
	//	BitBlt(mDC, 710, 515, 45, 45, g_BMPmDC, 0, 0, SRCCOPY);
	//	break; 
	//}
	//}

}

void CPlayer::Free()
{
	// Player가 동적 할당한거 X
}

void CPlayer::regen()
{
	acceleration = 0.0f;	// 현재 가속도
	speed = 0.0f;			// 현재 속도
	isMoving = FALSE;		// 이동 중인지 여부
	falling = FALSE;
	jumping = FALSE;
	x = 420;
	y = -200;
	life--;
	combo = 0;

	gunType = GUN_TYPE_PISTOL;
	range = 300;
	maxBullet = 20;
	reload();
}

bool CPlayer::Update()
{
	// 서버로 부터 받은 정보로 플레이어 상태 업데이트

	if (true)
	{
		x = pInfo.vPosition.x;
		y = pInfo.vPosition.y;
		gunType = pInfo.eItemType;
		// looking = pInfo.looking;
	}
	//else
	//{
	//	 // 1. 마찰력 적용 (항상)		// (isMoving과 관계없이 현재 속도에 대해 마찰을 먼저 계산)
	//	 if (!jumping && !falling)	// 지상에 있을 때만 마찰 적용 (옵션)
	//	 {
	//		 if (speed > 0.0f) {
	//			 speed = max(0.0f, speed - FRICTION);
	//		 } else if (speed < 0.0f) {
	//			 speed = min(0.0f, speed + FRICTION);
	//		 }
	//	 }

	//	 // 2. 가속도 적용 (isMoving == true 일 때, 즉 키가 눌렸을 때)
	//	 if (isMoving) {
	//		 speed += acceleration;
	//	 }

	//	 // 3. 속도 제한 (마찰과 가속이 모두 적용된 최종 속도를 제한)
	//	 speed = max(-MAX_SPEED, min(speed, MAX_SPEED));

	//	 // 4. 최종 속도를 위치에 적용
	//	 x += (int)speed;
	//}

	//// jumping or falling
	//// 땅 충돌 검사는 CPlayLevel에서 수행
	//if (jumping == TRUE) {
	//	jumpHeight = (jumpTime * jumpTime - jumpPower * jumpTime) * 4.f;
	//	jumpTime += 0.2f;
	//	y = (jstartY + (int)jumpHeight);
	//}
	//else if (falling == TRUE) { // jumping이 FALSE일 때만 낙하
	//	downHeight = (downTime * (downTime / 2)) * 4.f;
	//	downTime += 0.2f;
	//	y = (fstartY + (int)downHeight);
	//}

	//// 다운카운트 -> 0이면 증가 x / 0이 아닐때 증가 시작 / N도달시 0으로 초기화
	//if (downCount > 0) {
	//	downCount++;
	//	if (downCount > 30) {
	//		downCount = 0;
	//	}
	//}

	//// 애니메이션 프레임
	//anim_timer++;
	//if (anim_timer > 5) { // 5프레임마다 
	//	anim_timer = 0;
	//	if (isMoving) {
	//		anim_frame = (anim_frame + 1) % 4;
	//	} else {
	//		anim_frame = 0; // 멈추면 0번 프레임
	//	}
	//}

	//// 콤보 타이머
	//if (combo > 0) {
	//	comboTime++;
	//	if (comboTime > 200) { // 200 프레임 (약 3.3초)
	//		combo = 0;
	//		comboTime = 0;
	//	}
	//}
	//return false;


	// 서버로부터 받은 정보(x, y)는 CPlayLevel::Update()에서 이미 적용됨,
	// 여기서는 클라이언트 측 물리 로직을 모두 제거

	// === 1. 상태 추론 (애니메이션용) ===
	// x좌표가 변했는지 감지하여 isMoving 상태 결정
	if (x != prevX) {
		isMoving = TRUE;
	}
	else {
		isMoving = FALSE;
	}

	// x좌표 변화 방향으로 바라보는 방향(looking) 결정
	if (x < prevX) {
		looking = 0; // Left
	}
	else if (x > prevX) {
		looking = 1; // Right
	}
	// (좌표가 같으면 이전 방향 유지)

	// 현재 x좌표를 다음 프레임을 위해 저장
	prevX = x;


	// === 2. 기존 로직 유지 (물리 외) ===

	// (기존 점프/낙하 로직은 모두 삭제)

	// 다운카운트 (이 로직은 서버에서 처리해야 하지만, 일단 유지)
	if (downCount > 0) {
		downCount++;
		if (downCount > 30) {
			downCount = 0;
		}
	}

	// 애니메이션 프레임 (위에서 추론한 isMoving 사용)
	anim_timer++;
	if (anim_timer > 5) { // 5프레임마다 
		anim_timer = 0;
		if (isMoving) {
			anim_frame = (anim_frame + 1) % 4;
		}
		else {
			anim_frame = 0; // 멈추면 0번 프레임
		}
	}

	// 콤보 타이머
	if (combo > 0) {
		comboTime++;
		if (comboTime > 200) { // 200 프레임 (약 3.3초)
			combo = 0;
			comboTime = 0;
		}
	}
	return false;
}

void CPlayer::reload()
{

}

void CPlayer::gunFire()
{

}

void CPlayer::update_bullet()
{
	
}


void CPlayer::SetOnGround(int groundY)
{
	falling = FALSE;
	jumping = FALSE;
	jumpCount = 0;
	y = groundY - pHeight;
	downCount = 0; // 땅에 닿았으므로 downCount 리셋
}

void CPlayer::SetFalling()
{
	if (falling == FALSE && jumping == FALSE) // 점프 중이 아닐 때만
	{
		downTime = 0;
		downHeight = 0;
		fstartY = y;
		falling = TRUE;
	}
}

void CPlayer::ApplyItem(int gunType)
{
	switch (gunType) {
	case GUN_TYPE_SNIPE:
		this->gunType = GUN_TYPE_SNIPE;
		maxBullet = 10;
		range = 600;
		OutputDebugString(L"Apply SNIPE Item\n");
		break;
	case GUN_TYPE_PISTOL:
		this->gunType = GUN_TYPE_PISTOL;
		maxBullet = 20;
		range = 300;
		OutputDebugString(L"Apply PISTOL Item\n");
		break;
	}
	reload();
}