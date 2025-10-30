#include "CBitmap.h"

CBitmap::CBitmap()
{
}

void CBitmap::Load_Bmp(int i)
{
	HDC		hDC = GetDC(g_hWnd);
	m_hMDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(i));

	// SelectObject : 준비한 dc의 gdi 오브젝트에 불러온 비트맵을 선택하기 위해 사용
	// gdi 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트를 반환
	m_holdBitmap = (HBITMAP)SelectObject(m_hMDC, m_hBitmap);
}

void CBitmap::Free()
{
	SelectObject(m_hMDC, m_holdBitmap);
	DeleteObject(m_hBitmap);

	DeleteDC(m_hMDC);
}