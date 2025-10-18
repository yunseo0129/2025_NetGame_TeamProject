#pragma once
#include "CLevel.h"
#include "CPlayer.h"
#include "CBullet.h"

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

private:

};
