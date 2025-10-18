#pragma once
#include "CObject.h"

const int mapWidth = 200;
const int mapHeight = 30;
const int mapCount = 5;

class CMap final : CObject
{
public:
	CMap();
	~CMap();

public:
	virtual void Draw(HDC mDC);
	virtual bool Update() { return false; };

private:
	virtual void Free();

private:
	int m_x;
	int m_y;
};