#include "CBmpMgr.h"

CBmpMgr::CBmpMgr()
{

}

void CBmpMgr::Insert_Bmp(int BmpName, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), [&](auto& MyPair)->bool
		{
			if (!lstrcmp(MyPair.first, pImgKey))
				return true;

			return false;
		});

	if (iter == m_mapBit.end())
	{
		CBitmap* pBmp = new CBitmap;
		pBmp->Load_Bmp(BmpName);

		m_mapBit.emplace(pImgKey, pBmp);
	}
}

HDC CBmpMgr::Find_Bmp(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), [&](auto& MyPair)->bool
		{
			if (!lstrcmp(MyPair.first, pImgKey))
				return true;

			return false;
		});

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MDC();
}

void CBmpMgr::Free()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), [](auto& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	});

	m_mapBit.clear();
}