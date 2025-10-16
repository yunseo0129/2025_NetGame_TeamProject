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
}

void CPlayer::reload()
{
}

void CPlayer::gunFire()
{
}

