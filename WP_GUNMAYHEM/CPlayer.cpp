#include "CPlayer.h"

CPlayer::CPlayer()
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
	bullet_count = 0;
	for (CBullet& b : bullet) {
		if (b.exist == FALSE)
			b.c = 0;
	}
}

void CPlayer::gunFire()
{
	//불렛카운트 -> 불렛 카운트가 맥스불렛을 넘으면 발사가 안됨 (총알 없음)
	//불렛 카운트가 탄약 갯수를 넘지 않음 -> 총을 쏨 (비활성화인 총알 하나를 활성화 시킴) -> 불렛 카운트 증가
	if (bullet_count < maxBullet) {
		for (int i = 0; i < MAX_BULLET; i++) {
			if (bullet[i].exist == FALSE && bullet[i].c == 0) {
				bullet[i].exist = TRUE;
				bullet[i].x = x + pWidth / 2;
				bullet[i].y = y + pHeight / 2;
				bullet[i].travelDistance = 0; // 초기화
				//총알 방향 계산
				switch (looking) {
				case 0:
					bullet[i].vx = -1;
					bullet[i].vy = 0;
					break;
				case 1:
					bullet[i].vx = 1;
					bullet[i].vy = 0;
					break;
				}
				bullet_count++;
				channel->stop();
				switch (gunType) {
				case GUN_TYPE_PISTOL:
					sound1->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound1, 0, false, &channel); //--- 1번 사운드 재생
					break;
				case GUN_TYPE_SNIPE:
					sound2->setMode(FMOD_LOOP_OFF);
					ssystem->playSound(sound2, 0, false, &channel); //--- 2번 사운드 재생
					break;
				}
				break; 
			}
		}
	}
}

void CPlayer::update_bullet(CPlayer* Enemy)
{
	//1. 총알1 충돌체크
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullet[i].exist == TRUE) {
			//사거리 도달시
			if (bullet[i].travelDistance > range) {
				bullet[i].exist = FALSE;
			} else {
				if (bullet[i].x + BULLET_SIZE >= Enemy->x && bullet[i].x - BULLET_SIZE <= Enemy->x + pWidth &&
					bullet[i].y + BULLET_SIZE >= Enemy->y && bullet[i].y - BULLET_SIZE <= Enemy->y + pHeight) {

					Enemy->combo++;
					Enemy->comboTime = 0;
					Enemy->speed = (7 * Enemy->combo) * bullet[i].vx;
					bullet[i].exist = FALSE;
				}
			}
		}
	}
	//2. 총알1 이동 -> 플레이어의 방향에 따라(looking)
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullet[i].exist == TRUE) {
			bullet[i].x += bullet[i].vx * 13; // 총알 속도 조절
			bullet[i].travelDistance += 13;    // 이동 거리 추가
			if (bullet[i].c < 30) {
				bullet[i].c++;
			}
		}
	}

	// 플레이어의 충돌체크 로직 따로 리팩토링 필요할듯
	//콤보초기화 -> 콤보가 1 이상이되면 시간을 검사한다
	if (Enemy->combo > 0) {
		Enemy->comboTime++;
		//player2.combo++;

		if (Enemy->comboTime > 200) {
			Enemy->combo = 0;
			Enemy->comboTime = 0;
		}
	}
}