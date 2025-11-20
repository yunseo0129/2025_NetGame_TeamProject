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
	if (pInfo.isConnected) {
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

		// 정보창 그리기
		// 플레이어1 정보창 위치 : 100, 500, 200, 150 
		int plusX = 250; // 0, 250, 500 ---> (plusX * playerType)으로 x값 조정
		SelectObject(g_BMPmDC, BMP_inform);
		BitBlt(mDC, 100 + plusX * playerType, 500, 200, 150, g_BMPmDC, 0, 0, SRCCOPY);
		wsprintf(lpOut, L"player %d", playerType + 1);
		TextOut(mDC, 112 + plusX * playerType, 512, lpOut, lstrlen(lpOut));
		wsprintf(lpOut, L"LIFE : %d", pInfo.iLife);
		TextOut(mDC, 112 + plusX * playerType, 532, lpOut, lstrlen(lpOut));
		wsprintf(lpOut, L"bullet : %d", 999);
		TextOut(mDC, 112 + plusX * playerType, 552, lpOut, lstrlen(lpOut));
		// 초상화
		SelectObject(g_BMPmDC, BMP_player_inform[playerType]);
		BitBlt(mDC, 240 + plusX * playerType, 515, 45, 45, g_BMPmDC, 0, 0, SRCCOPY);
	}
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
	if (pInfo.isConnected)
	{

		if (true) {
			x = pInfo.vPosition.x;
			y = pInfo.vPosition.y;
			gunType = pInfo.eItemType;
			looking = pInfo.looking;
		}

		// 상태 추론 (애니메이션용)
		// x좌표가 변했는지 감지하여 isMoving 상태 결정
		if (x != prevX) {
			isMoving = TRUE;
		} else {
			isMoving = FALSE;
		}
		prevX = x;


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
			} else {
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