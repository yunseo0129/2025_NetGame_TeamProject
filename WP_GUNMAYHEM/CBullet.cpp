#include "CBullet.h"

CBullet::CBullet()
{
	m_exist = FALSE;
	m_x = 0;
	m_y = 0;
}

CBullet::~CBullet()
{
}

void CBullet::Draw(HDC mDC)
{
	if (m_exist)
	{
		HPEN mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(mDC, mPen);
		HBRUSH mBrush = CreateSolidBrush(RGB(0, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

		Rectangle(mDC,
				  m_x - BULLET_SIZE - cameraX,
				  m_y - BULLET_SIZE - cameraY,
				  m_x + BULLET_SIZE - cameraX,
				  m_y + BULLET_SIZE - cameraY);

		SelectObject(mDC, oldBrush);
		DeleteObject(mBrush);
		SelectObject(mDC, oldPen);
		DeleteObject(mPen);
	}
}

void CBullet::Free()
{
}

bool CBullet::Update()
{
	if (true) {
		m_exist = bInfo.exist;
		m_x = bInfo.vPosition.x;
		m_y = bInfo.vPosition.y;
	}
	return false;
}