#include "CBullet.h"

CBullet::CBullet()
{
	exist = FALSE;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
	c = 0;
	travelDistance = 0;
}

CBullet::~CBullet()
{
}

void CBullet::Draw(HDC BMPmDC, HDC mDC)
{
}

void CBullet::Free()
{
}

bool CBullet::Update()
{
	return false;
}