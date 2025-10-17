#include "CMainLevel.h"

CMainLevel::CMainLevel() { 
	 m_startMenu = { 500, 350, 800, 400 };
	 m_endMenu = { 500, 450, 800, 500 };
	 m_changeMapMenu = { 600, 250, 800, 300 };
	 m_ptClick = { 0, 0 };
}

void CMainLevel::Initialize()
{
	m_bClicked = false;
}

void CMainLevel::SetClickPoint(int mx, int my)
{
	m_ptClick = { mx, my };
	m_bClicked = true;
}

void CMainLevel::Update() // 입력 처리 포함 (WM_LBUTTONDOWN 메시지를 받은 것 처럼)
{
	if (m_bClicked) 
	{
		POINT pt = m_ptClick;

		channel->stop();
		clickSound->setMode(FMOD_LOOP_OFF);
		ssystem->playSound(clickSound, 0, false, &channel);

		if (PtInRect(&m_startMenu, pt)) {
			// 시작 버튼 클릭 시 처리
			//CLevelManager::GetInstance()->ChangeLevel(LEVEL_PLAY);
		}
		else if (PtInRect(&m_endMenu, pt)) {
			PostQuitMessage(0);
		}
		else if (PtInRect(&m_changeMapMenu, pt)) {
			// 맵 변경 버튼 클릭 시 처리
			mapType++;
			mapType = mapType % 2;
		}
		m_bClicked = false; // 클릭 처리 후 초기화
	}
}

void CMainLevel::Draw(HDC mDC)
{
	SelectObject(BMPmDC, BMP_startMenu);
	BitBlt(mDC, 0, 0, rt.right, rt.bottom, BMPmDC, 0, 0, SRCCOPY);
	
	wsprintf(lpOut, L"Type : %d", mapType);
	TextOut(mDC, m_changeMapMenu.left + 140, m_changeMapMenu.top + 50, lpOut, lstrlen(lpOut));
}

void CMainLevel::Free()
{
}
