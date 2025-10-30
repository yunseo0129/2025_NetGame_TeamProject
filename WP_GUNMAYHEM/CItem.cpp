#include "CItem.h"

CItem::CItem()
{
	// 생성 시 초기화 - 낙하 시작	
	m_exist = true;
	m_x = 100 + (rand() % 70 * 10);
	m_y = -200;
	m_fstartY = 0;
	m_downTime = 0.f;
	m_downHeight = 0;
	m_falling = true;			// 생성과 동시에 낙하 시작
	m_type = rand() % 2 + 1;	// 0 or 1
}

CItem::~CItem()
{
}

void CItem::Draw(HDC mDC)
{
	if (m_exist) {
		SelectObject(g_BMPmDC, BMP_itemBox);
		BitBlt(mDC, m_x - cameraX, m_y - cameraY, iWidth, iHeight, g_BMPmDC, 0, 0, SRCCOPY);
	}
}

bool CItem::Update()
{
	return false;
}

void CItem::Free()
{
}