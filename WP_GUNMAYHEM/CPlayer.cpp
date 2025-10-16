#include "CPlayer.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Draw(HDC BMPmDC, HDC mDC)
{
}

void CPlayer::Free()
{
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

void CPlayer::update()
{
	// update speed :  player 1
	if (isMoving) {
		// 가속도를 속도에 적용
		speed += acceleration;
		// 속도가 최대 속도를 초과하지 않도록 조정
		speed = max(-MAX_SPEED, min(speed, MAX_SPEED));
	} else {
		// 마찰력을 적용하여 점차 멈춤
		if (speed > 0) {
			speed = max(0.0f, speed - FRICTION);
		} else if (speed < 0) {
			speed = min(0.0f, speed + FRICTION);
		}
	}
	// 속도에 따라 플레이어 위치 업데이트
	x += speed;

	// jumping or falling
	if (jumping == TRUE) {
		jumpHeight = (jumpTime * jumpTime - jumpPower * jumpTime) * 4.f;
		jumpTime += 0.2f;
		y = (jstartY + (int)jumpHeight);

		//점프타임이 점프파워보다 커졌을 때, 떨어지는 중일 때를 충돌체크한다.
		if (jumpHeight <= (jumpTime * jumpTime - jumpPower * jumpTime) * 4.f) {
			for (int i = 0; i < mapCount; i++) {
				if (x + pWidth > maps[i].x && x < maps[i].x + mapWidth &&
					y + pHeight > maps[i].y && y + pHeight < maps[i].y + 15) {
					falling = FALSE;
					jumping = FALSE;
					jumpCount = 0;
					y = maps[i].y - pHeight;
					break;
				}
			}
		}

	}
	if (jumping == FALSE) {
		// 1. 플레이어의 위치 검사 -> 떨어지기 시작할지, 안떨어질지 판단 -> 다운카운트 중일때는 판단 x
		if (downCount == 0) {
			for (int i = 0; i < mapCount; i++) {
				// 플레이어가 맵 위에 있을 때
				if (x + pWidth > maps[i].x && x < maps[i].x + mapWidth &&
					y + pHeight > maps[i].y && y + pHeight < maps[i].y + 15) {
					jumping = FALSE;
					falling = FALSE;
					jumpCount = 0;
					if (y + pHeight > maps[i].y) {
						y = maps[i].y - pHeight;
					}
					break; // 맵 위에 있으면 더 이상 검사할 필요가 없으므로 반복문을 빠져나온다.
				} else {
					// 플레이어가 맵 위에 없을 때
					if (falling == FALSE)	// 떨어지고 있지 않은 상태였다면
					{
						downTime = 0;
						downHeight = 0;
						fstartY = y;		// 현재 좌표를 저장하고
						falling = TRUE;		// 떨어지는 상태로 만든다.
					}
				}
			}
		}
		// 2. 떨어지는 중이면 -> 위치변동, 충돌체크 -> 다운카운트 중일때는 충돌체크 x
		if (falling == TRUE) {
			downHeight = (downTime * (downTime / 2)) * 4.f;
			downTime += 0.2f;
			y = (fstartY + (int)downHeight);

			if (downCount == 0) {
				for (int i = 0; i < mapCount; i++) {
					if (x + pWidth > maps[i].x && x < maps[i].x + mapWidth &&
						y + pHeight > maps[i].y && y + pHeight < maps[i].y + 15) {
						falling = FALSE;
						jumping = FALSE;
						jumpCount = 0;
						y = maps[i].y - pHeight;
						break;
					}
				}
			}
		}
	}
	// 다운카운트 -> 0이면 증가 x / 0이 아닐때 증가 시작 / N도달시 0으로 초기화
	if (downCount > 0) {
		downCount++;
		if (downCount > 30) {
			downCount = 0;
		}
	}

	//아이템 충돌검사
	for (int i = 0; i < MAX_ITEM; i++) {
		if (item[i].exist == TRUE) {
			if (x < item[i].x + iWidth / 2 && item[i].x + iWidth / 2 < x + pWidth &&
				y < item[i].y + iHeight / 2 && item[i].y + iHeight / 2 < y + pHeight) {

				channel->stop();
				itemSound->setMode(FMOD_LOOP_OFF);
				ssystem->playSound(itemSound, 0, false, &channel); //--- 1번 사운드 재생
				item[i].exist = FALSE;
				//아이템에 따른 총 변화
				switch (item[i].type) {
				case 0:
					gunType = 2;
					maxBullet = 10;
					range = 600;
					break;
				case 1:
					gunType = 1;
					maxBullet = 20;
					range = 300;
					break;
				}
				reload();
			}
		}
	}
}

void CPlayer::reload()
{
}

void CPlayer::gunFire()
{
}

