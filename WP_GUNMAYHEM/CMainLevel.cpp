#include "CMainLevel.h"
#include "CLevelManager.h"

CMainLevel::CMainLevel() { 
	Initialize();
}

void CMainLevel::Initialize()
{
    m_startMenu = { 500, 350, 800, 400 };
    m_endMenu = { 500, 450, 800, 500 };
	m_bLButtonDown = false; // 클릭 상태 초기화
}


void CMainLevel::Update()   // 입력 처리 
{
    // 마우스 왼쪽 버튼이 눌렸는지 확인
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        // 이전에 눌리지 않은 상태였다면 (Click 이벤트)
        if (!m_bLButtonDown) {
            m_bLButtonDown = true; // 눌림 상태로 변경

            POINT pt = {};
            GetCursorPos(&pt);              // 화면 좌표
            ScreenToClient(g_hWnd, &pt);    // 클라이언트(윈도우) 좌표로 변환

            // 사운드 재생
            channel->stop();
            clickSound->setMode(FMOD_LOOP_OFF);
            ssystem->playSound(clickSound, 0, false, &channel);

            // 클릭 위치 검사
            if (PtInRect(&m_startMenu, pt)) {
				// 시작 버튼 --> 게임 레벨로 전환
                CLevelManager::GetInstance()->ChangeLevel(LEVEL_PLAY);
            } 
            else if (PtInRect(&m_endMenu, pt)) {
                // 종료 버튼
                PostQuitMessage(0);
            } 
        }
    } 
    else {
        m_bLButtonDown = false; // 마우스 버튼이 떼어지면 상태 초기화
    }
}

void CMainLevel::Draw(HDC mDC)
{
    SelectObject(g_BMPmDC, BMP_startMenu);
    BitBlt(mDC, 0, 0, rt.right, rt.bottom, g_BMPmDC, 0, 0, SRCCOPY);
}

void CMainLevel::Free()
{
}
