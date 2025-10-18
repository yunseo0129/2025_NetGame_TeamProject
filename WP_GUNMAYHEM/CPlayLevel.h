#pragma once
#include "CLevel.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CItem.h"

// 플레이 레벨 ====
// 캐릭터, 아이템, 맵, 총알 오브젝트들
// Draw - 맵, 캐릭터, 캐릭터 정보, 총알은 그라데이션 신경쓰기
// Update - 입력처리, 충돌처리, 아이템 생성, 캐릭터 이동, 총알 이동

class CPlayLevel final : public CLevel {
public:
	CPlayLevel();
	virtual ~CPlayLevel() = default;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw(HDC mDC) override;
	virtual void Free() override;

	void update_camera();
	void ProcessPlayerPhysics(CPlayer* player);

private:
	// 아이템 생성 타이머
	float m_itemSpawnTimer = 0.f;
	float m_itemSpawnDelay = 1000.f; // 7초 (기존 WM_TIMER 2번)

	// 타이머 관리를 위한 시간 변수
	float m_deltaTime = 0.f; // 직전 프레임과의 시간 차
	ULONGLONG m_prevTime = 0;

	CPlayer* m_pPlayer1 = nullptr;
	CPlayer* m_pPlayer2 = nullptr;
};
