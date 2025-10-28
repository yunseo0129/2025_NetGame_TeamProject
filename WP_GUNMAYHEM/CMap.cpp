#include "CMap.h"

CMap::CMap(int x, int y)
{
	m_x = x;
	m_y = y;
}

CMap::~CMap()
{
}

void CMap::Draw(HDC mDC)
{
	SelectObject(g_BMPmDC, BMP_ground1);
	BitBlt(mDC, m_x - cameraX, m_y - 5 - cameraY, mapWidth, mapHeight, g_BMPmDC, 0, 0, SRCCOPY);
}

bool CMap::Update()
{
	return false;
}

void CMap::Free()
{
	//m_vMaps.clear();
}
