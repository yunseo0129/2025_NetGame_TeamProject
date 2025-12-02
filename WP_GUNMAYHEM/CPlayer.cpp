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
		// 내 플레이어 표시
		if (isMyPlayer) {
			// 텍스트 배경을 투명하게 설정 (기존 설정 저장)
			int nOldBkMode = SetBkMode(mDC, TRANSPARENT);

			// 출력할 문자열 (역삼각형)
			const WCHAR* marker = L"▼";

			// 위치 계산 (플레이어 머리 위 중앙)
			int markX = (x - cameraX) + (pWidth / 2) - 8;
			int markY = (y - cameraY) - 20; // 머리 위로 20픽셀 띄움

			// 출력
			TextOut(mDC, markX, markY, marker, lstrlen(marker));

			// DC 설정 복구 (다른 그리기에 영향 주지 않도록)
			SetBkMode(mDC, nOldBkMode);
		}

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