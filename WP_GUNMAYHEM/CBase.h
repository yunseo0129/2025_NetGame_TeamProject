#pragma once
#include "Default.h"

class CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	unsigned int AddRef();
	unsigned int Release();

private:
	unsigned int m_iRefCnt = { 0 };

public:
	virtual void Free() = 0;
};