#pragma once
#include "CBase.h"

class CBitmap final : public CBase
{
public:
	CBitmap();
	~CBitmap() = default;

public:
	HDC Get_MDC() { return m_hMDC; }
	void Load_Bmp(const TCHAR* pFilePath);
	void Free();

private:
	HDC m_hMDC;

	HBITMAP m_hBitmap;
	HBITMAP m_holdBitmap;
};