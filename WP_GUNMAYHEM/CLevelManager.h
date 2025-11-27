#pragma once
#include "CLevel.h"
#include "CMainLevel.h"
#include "CPlayLevel.h"

enum LEVEL_ID
{
	LEVEL_MAIN, 
	LEVEL_PLAY,
	LEVEL_END
};

class CLevelManager
{
private:
	CLevelManager();
	~CLevelManager();

private:
	static CLevelManager* m_pInstance;

	CLevel* m_pLevel[LEVEL_END] = { nullptr, };			// 레벨 배열
	LEVEL_ID m_eCurrentLevel = LEVEL_END;				// 현재 레벨을 가리키는 변수 (인덱스 용)
	bool m_bChange = false;
	LEVEL_ID m_eNextLevel = LEVEL_END;

public:
	void Initialize()
	{
		m_pLevel[LEVEL_MAIN] = new CMainLevel();
		m_pLevel[LEVEL_PLAY] = new CPlayLevel();
		m_eCurrentLevel = LEVEL_MAIN;
	}
	void ChangeLevel(LEVEL_ID _eNewLevelID);			// 레벨 전환
	void UpdateLevel();									// CurrentLevel의 Update 호출
	void DrawLevel(HDC mDC);							// CurrentLevel의 Render 호출
	
	static CLevelManager* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CLevelManager();
		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};