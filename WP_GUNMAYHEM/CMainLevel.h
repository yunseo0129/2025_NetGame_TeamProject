#pragma once
#include "CLevel.h"

class CMainLevel final : public CLevel{
public:
	CMainLevel();
	virtual ~CMainLevel() = default;

public:
	virtual void Initialize() override;
	virtual void Update() override;                     
	virtual void Draw(HDC mDC) override;
	virtual void Free() override;

	void SetClickPoint(int mx, int my);

private:
	RECT m_startMenu;       // 시작 버튼 영역
	RECT m_endMenu;         // 종료 버튼 영역
	RECT m_changeMapMenu;   // 맵 변경 버튼 영역
	TCHAR lpOut[100];		// wsprintf : 텍스트 저장용 변수

	POINT m_ptClick;			// 마우스 클릭 좌표 저장
	bool m_bClicked = false;	// 클릭 여부
};

