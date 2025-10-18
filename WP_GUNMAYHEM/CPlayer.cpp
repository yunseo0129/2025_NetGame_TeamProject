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
	HBITMAP hBitmapToDraw = BMP_player1_right_stand;
	switch (playerType) {
	case 1:
	{
		hBitmapToDraw = BMP_player1_right_stand;
		switch (looking) {
		case 0: // Left
			if (isMoving) hBitmapToDraw = BMP_player1_left_walk[anim_frame];
			else hBitmapToDraw = BMP_player1_left_stand;
			break;
		case 1: // Right
			if (isMoving) hBitmapToDraw = BMP_player1_right_walk[anim_frame];
			else hBitmapToDraw = BMP_player1_right_stand;
			break;
		}
		break;
	}
	case 2:
	{
		hBitmapToDraw = BMP_player2_right_stand;
		switch (looking) {
		case 0: // Left
			if (isMoving) hBitmapToDraw = BMP_player2_left_walk[anim_frame];
			else hBitmapToDraw = BMP_player2_left_stand;
			break;
		case 1: // Right
			if (isMoving) hBitmapToDraw = BMP_player2_right_walk[anim_frame];
			else hBitmapToDraw = BMP_player2_right_stand;
			break;
		}
		break;
	}
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


	// 3. 총알 그리기 (기존 WM_PAINT 로직)
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullet[i].exist == TRUE) {
			HPEN mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
			HPEN oldPen = (HPEN)SelectObject(mDC, mPen);
			HBRUSH mBrush = CreateSolidBrush(RGB(0, 0, 0));
			HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

			Rectangle(mDC,
					  bullet[i].x - BULLET_SIZE - cameraX,
					  bullet[i].y - BULLET_SIZE - cameraY,
					  bullet[i].x + BULLET_SIZE - cameraX,
					  bullet[i].y + BULLET_SIZE - cameraY);

			SelectObject(mDC, oldBrush);
			DeleteObject(mBrush);
			SelectObject(mDC, oldPen);
			DeleteObject(mPen);

			// 그라데이션 (꼬리)
			for (int j = 0; j < bullet[i].c; j++) {
				mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j));
				oldPen = (HPEN)SelectObject(mDC, mPen);
				mBrush = CreateSolidBrush(RGB(7 * j, 7 * j, 7 * j));
				oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

				Rectangle(mDC,
						  bullet[i].x - bullet[i].vx * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraX,
						  bullet[i].y - bullet[i].vy * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraY,
						  bullet[i].x - bullet[i].vx * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraX,
						  bullet[i].y - bullet[i].vy * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraY);

				SelectObject(mDC, oldBrush);
				DeleteObject(mBrush);
				SelectObject(mDC, oldPen);
				DeleteObject(mPen);
			}
		}
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
	// update speed :  player
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
	// 땅 충돌 검사는 CPlayLevel에서 수행
	if (jumping == TRUE) {
		jumpHeight = (jumpTime * jumpTime - jumpPower * jumpTime) * 4.f;
		jumpTime += 0.2f;
		y = (jstartY + (int)jumpHeight);
	}
	else if (falling == TRUE) { // jumping이 FALSE일 때만 낙하
		downHeight = (downTime * (downTime / 2)) * 4.f;
		downTime += 0.2f;
		y = (fstartY + (int)downHeight);
	}

	// 다운카운트 -> 0이면 증가 x / 0이 아닐때 증가 시작 / N도달시 0으로 초기화
	if (downCount > 0) {
		downCount++;
		if (downCount > 30) {
			downCount = 0;
		}
	}

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

	// 콤보 타이머
	if (combo > 0) {
		comboTime++;
		if (comboTime > 200) { // 200 프레임 (약 3.3초)
			combo = 0;
			comboTime = 0;
		}
	}

	// 총알 이동 및 사거리 체크
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullet[i].exist == TRUE) {
			bullet[i].x += bullet[i].vx * 13; // 총알 속도
			bullet[i].travelDistance += 13;    // 이동 거리
			if (bullet[i].c < 30) {
				bullet[i].c++;
			}

			// 사거리 도달 시
			if (bullet[i].travelDistance > range) {
				bullet[i].exist = FALSE;
				bullet[i].c = 0; // (재사용을 위해 c도 0으로)
			}
		}
	}

	// 아이템/맵 충돌 검사는 CPlayLevel::Update()에서 수행
	return false;
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
	// 총알 vs 적 충돌 체크
	for (int i = 0; i < MAX_BULLET; i++) {
		if (bullet[i].exist == TRUE) {
			// CPlayer::GetRect()를 사용하도록 수정
			RECT bulletRect = { bullet[i].x - BULLET_SIZE, bullet[i].y - BULLET_SIZE,
								bullet[i].x + BULLET_SIZE, bullet[i].y + BULLET_SIZE };
			RECT enemyRect = Enemy->GetRect();

			if (CheckRectCollision(bulletRect, enemyRect)) {
				Enemy->combo++;
				Enemy->comboTime = 0; // 콤보 타이머 리셋
				Enemy->speed = (7 * Enemy->combo) * bullet[i].vx; // 넉백
				bullet[i].exist = FALSE;
				bullet[i].c = 0;
			}
		}
	}
	// 콤보 초기화 로직은 CPlayer::Update()로 이동
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
