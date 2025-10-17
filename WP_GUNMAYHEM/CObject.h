#pragma once
#include "CBase.h"

class CObject abstract : public CBase
{
public:
	virtual ~CObject() = 0;

public:
	virtual void Draw(HDC BMPmDC, HDC mDC) = 0;
	virtual bool Update() = 0;

private:
	virtual void Free() = 0;
};