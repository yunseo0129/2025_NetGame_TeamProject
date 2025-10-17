#include "CBitmap.h"

CBitmap::CBitmap()
{
}

CBitmap::~CBitmap()
{
	Release();
}

void CBitmap::Load_Bmp(const TCHAR* pFilePath)
{
}

void CBitmap::Free()
{
	SelectObject(m_hMDC, m_holdBitmap);
	DeleteObject(m_hBitmap);

	DeleteDC(m_hMDC);
}