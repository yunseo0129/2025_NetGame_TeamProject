#include "CLevel.h"

void CLevel::Update(HWND hWnd)
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