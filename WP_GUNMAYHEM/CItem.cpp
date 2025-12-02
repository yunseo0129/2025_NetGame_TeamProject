#include "CItem.h"

CItem::CItem()
{
	m_x = 0;
	m_y = 0;
	m_exist = false;
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
	// 아이템 사라질 때 사운드 재생
	if (iInfo.exist == false && m_exist == true) {
		channel->stop();
		itemSound->setMode(FMOD_LOOP_OFF);
		ssystem->playSound(itemSound, 0, false, &channel);
	}

	m_exist = iInfo.exist;
	m_x = iInfo.vPosition.x;
	m_y = iInfo.vPosition.y;
	
	return false;
}

void CItem::Free()
{
}