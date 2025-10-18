#pragma once
#include "CObject.h"

const int mapWidth = 200;
const int mapHeight = 30;
const int mapCount = 5;

class CMap final : public CObject
{
public:
	CMap(int x, int y);
	~CMap();

public:
	virtual void Draw(HDC mDC) override;
	virtual bool Update() override { return false; };

	// 충돌 처리를 위한 함수 추가
	int GetTopY() const { return m_y; }
	RECT GetRect() const
	{
		return { m_x, m_y, m_x + mapWidth, m_y + mapHeight };
	}

private:
	virtual void Free() override;

private:
	int m_x;
	int m_y;
};