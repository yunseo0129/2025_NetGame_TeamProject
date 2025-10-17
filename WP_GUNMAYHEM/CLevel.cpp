#include "CLevel.h"

void CLevel::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();
			if (true == iResult)
			{
				// Safe_Delete<CObject*>(*iter);
				// iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CLevel::Draw(HDC mDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_ObjList[i])
		{
			obj->Draw(mDC);
		}
	}
}

void CLevel::Free()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_ObjList[i])
		{
			// Safe_Delete<CObject*>(obj);
		}
		m_ObjList[i].clear();
	}
}