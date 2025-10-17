#include "CLevelManager.h"

void CLevelManager::ChangeLevel(LEVEL_ID _eNextLevelID)
{
	m_eNextLevel = _eNextLevelID;
	m_bChange = true;
}

void CLevelManager::UpdateLevel()
{
	// 프레임 시작 시점에 레벨 전환이 일어나도록 여기서 Change 처리
	if (m_bChange) {			
		m_bChange = false;
		if (m_eCurrentLevel != LEVEL_END) {
			m_pLevel[m_eCurrentLevel]->Free();	// 기존 레벨의 Free 호출 -> 레벨 내부 객체들의 Free 호출
		}
		m_eCurrentLevel = m_eNextLevel;
		m_eNextLevel = LEVEL_END;
		m_pLevel[m_eCurrentLevel]->Initialize();
	}
	m_pLevel[m_eCurrentLevel]->Update();
}

void CLevelManager::DrawLevel(HDC mDC)
{
	m_pLevel[m_eCurrentLevel]->Draw(mDC);
}