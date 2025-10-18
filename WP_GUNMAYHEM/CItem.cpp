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
	m_falling = true;
	m_type = rand() % 2;	// 0 or 1
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
	// 충돌 처리는 플레이 레벨에서 수행

	// 낙하 처리만 수행
	if (m_exist == false) {
		if (m_falling == true) {
			m_downHeight = (m_downTime * (m_downTime / 2)) * 4.f;
			m_downTime += 0.2f;
			m_y = (m_fstartY + (int)m_downHeight);
		}
	}
	return false;
}

void CItem::Free()
{
}