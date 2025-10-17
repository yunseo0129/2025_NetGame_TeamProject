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
	HDC		hDC = GetDC(g_hWnd);
	m_hMDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,	// ���α׷� �ν��Ͻ� �ڵ�(���Ϸκ��� �о���� NULL��)
		pFilePath,  // ������ ��θ� ����								
		IMAGE_BITMAP,// � Ÿ���� �о���� ����(��Ʈ�� ����)
		0, 0,			// �̹��� ����, ���� ������(���Ͽ��� �б� ������ ������ �������� ����)
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : ���Ϸ� �̹��� �о���� LR_CREATEDIBSECTION : �о�� ������ DIB���·� ��ȯ

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