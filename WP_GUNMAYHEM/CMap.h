#pragma once
#include "CObject.h"

class CMap final : public CObject
{
public:
	CMap();
	~CMap();

public:
	virtual void Draw(HDC mDC);
	virtual bool Update();

	void MapSelect(bool _isMap1);
	std::vector<MAPS> GetMaps() const { return m_vMaps; }

private:
	virtual void Free();

private:
	std::vector<MAPS> m_vMaps;
};