#include "CMainLevel.h"

CMainLevel::CMainLevel() { 
	 m_startMenu = { 500, 350, 800, 400 };
	 m_endMenu = { 500, 450, 800, 500 };
	 m_changeMapMenu = { 600, 250, 800, 300 };
	 m_ptClick = { 0, 0 };
}

void CMainLevel::Initialize()
{
}

void CMainLevel::Update() // 입력 처리 포함 (WM_LBUTTONDOWN 메시지를 받은 것 처럼)
{
	
}

void CMainLevel::Draw(HDC mDC)
{
	//SelectObject(BMPmDC, BMP_startMenu);
	//BitBlt(mDC, 0, 0, rt.right, rt.bottom, BMPmDC, 0, 0, SRCCOPY);
	//
	//wsprintf(lpOut1, L"Type : %d", mapType);
	//TextOut(mDC, changeMapMenu.left + 140, changeMapMenu.top + 50, lpOut1, lstrlen(lpOut1));
}

void CMainLevel::Free()
{
}
