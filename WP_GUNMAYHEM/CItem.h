#pragma once
#include "CObject.h"

const int iWidth = 40;
const int iHeight = 40;

class CItem final : public CObject{
public:
	CItem();
	~CItem();

public:
	virtual void Draw(HDC mDC) override;
	virtual bool Update() override;

private:
	virtual void Free() override;

private:
	bool m_exist;
	int m_x;
	int m_y;

public:
	ItemBoxInfo iInfo;
};

