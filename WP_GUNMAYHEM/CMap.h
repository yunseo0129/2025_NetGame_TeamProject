#pragma once
#include "CObject.h"

class CMap final : CObject
{
public:
	CMap();
	~CMap();

public:
	virtual void Draw(HDC BMPmDC, HDC mDC);

private:
	virtual void Free();

private:

};