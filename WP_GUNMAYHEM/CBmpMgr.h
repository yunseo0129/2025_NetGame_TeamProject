#pragma once
#include "CBase.h"
#include "CBitmap.h"

// 싱글톤임 GetInstance 불러서 사용할 것
// 비트맵들 싹다 들고있게 짤 예정

class CBmpMgr final : public CBase
{
private:
	CBmpMgr();
	~CBmpMgr() = default;

public:
	void		Insert_Bmp(int BmpName, const TCHAR* pImgKey);
	HDC			Find_Bmp(const TCHAR* pImgKey);
	void		Free();

public:
	static CBmpMgr* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new CBmpMgr;

		return m_pInstance;
	}
	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CBmpMgr* m_pInstance;
	std::map<const TCHAR*, CBitmap*> m_mapBit;
};