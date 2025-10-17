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

	m_hBitmap = (HBITMAP)LoadImage(NULL,	// 프로그램 인스턴스 핸들(파일로부터 읽어오기 NULL값)
		pFilePath,  // 파일의 경로를 전달								
		IMAGE_BITMAP,// 어떤 타입을 읽어올지 전달(비트맵 설정)
		0, 0,			// 이미지 가로, 세로 사이즈(파일에서 읽기 때문에 별도로 설정하지 않음)
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : 파일로 이미지 읽어들임 LR_CREATEDIBSECTION : 읽어온 파일을 DIB형태로 변환

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