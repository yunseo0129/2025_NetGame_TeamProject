#include "CPlayer.h"

CPlayer::CPlayer()
{
	exist = true;
	x = 0;
	y = 0;
	looking = 0;
}

void CPlayer::Draw(HDC mDC)
{
	if (exist && pInfo.isConnected) {
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
		wsprintf(info_text, L"player %d", playerType + 1);
		TextOut(mDC, 112 + plusX * playerType, 512, info_text, lstrlen(info_text));
		wsprintf(info_text, L"LIFE : %d", pInfo.iLife);
		TextOut(mDC, 112 + plusX * playerType, 532, info_text, lstrlen(info_text));
		wsprintf(info_text, L"bullet : %d", pInfo.iBullet);
		TextOut(mDC, 112 + plusX * playerType, 552, info_text, lstrlen(info_text));
		// 초상화
		SelectObject(g_BMPmDC, BMP_player_inform[playerType]);
		BitBlt(mDC, 240 + plusX * playerType, 515, 45, 45, g_BMPmDC, 0, 0, SRCCOPY);
	}
}

bool CPlayer::Update()
{
	// 서버로 부터 받은 정보로 플레이어 상태 업데이트
	if (pInfo.isConnected)
	{
		x = pInfo.vPosition.x;
		y = pInfo.vPosition.y;
		gunType = pInfo.eItemType;
		looking = pInfo.looking;

		if (pInfo.iLife <= 0) {
			exist = false;
		}

		// 상태 추론 (애니메이션용)
		// x좌표가 변했는지 감지하여 isMoving 상태 결정
		isMoving = false;
		if (x != prevX) {
			isMoving = true;
		} 
		prevX = x;

		// 애니메이션 프레임 
		anim_timer++;
		if (anim_timer > 5) { // 5프레임마다 
			anim_timer = 0;
			if (isMoving) {
				anim_frame = (anim_frame + 1) % 4;
			} else {
				anim_frame = 0; // 멈추면 0번 프레임
			}
		}
	}
	return false;
}


void CPlayer::Free()
{
}