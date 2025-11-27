#pragma once
#include "CObject.h"

const int mapWidth = 200;
const int mapHeight = 30;

class CMap final : public CObject
{
public:
	CMap(int x, int y);
	~CMap();

public:
	virtual void Draw(HDC mDC) override;
	virtual bool Update() override;

private:
	virtual void Free() override;

private:
	int m_x;
	int m_y;
};