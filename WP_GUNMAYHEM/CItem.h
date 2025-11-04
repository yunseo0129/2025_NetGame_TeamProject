#pragma once
#include "CObject.h"

const int iWidth = 40;
const int iHeight = 40;
const int MAX_ITEM = 5;
// int item_count = 0;

class CItem final : public CObject{
public:
	CItem();
	~CItem();

public:
	virtual void Draw(HDC mDC) override;
	virtual bool Update() override;

	bool IsFalling() const { return m_falling; }
	RECT GetRect() const { return { m_x, m_y, m_x + iWidth, m_y + iHeight }; }

	void SetDead() { m_exist = false; }
	bool IsDead() const { return !m_exist; }
	int GetGunType() const { return m_type; }

private:
	virtual void Free() override;

private:
	bool m_exist;
	int m_x;
	int m_y;

	//낙하변수
	int m_fstartY;
	float m_downTime;
	float m_downHeight;
	bool m_falling;

	int m_type; //0: 총알 10개, 사거리 600,		1: 총알 20개, 사거리 300
};

