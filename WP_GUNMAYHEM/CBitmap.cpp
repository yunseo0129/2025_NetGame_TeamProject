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

	// SelectObject : �غ��� dc�� gdi ������Ʈ�� �ҷ��� ��Ʈ���� �����ϱ� ���� ���
	// gdi ������Ʈ�� �����ϱ� ���� ������ ������ �ִ� ������Ʈ�� ��ȯ
	m_holdBitmap = (HBITMAP)SelectObject(m_hMDC, m_hBitmap);
}

void CBitmap::Free()
{
	SelectObject(m_hMDC, m_holdBitmap);
	DeleteObject(m_hBitmap);

	DeleteDC(m_hMDC);
}