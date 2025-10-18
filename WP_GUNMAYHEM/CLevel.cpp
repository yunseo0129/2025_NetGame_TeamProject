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
			if (obj != nullptr) {
				delete obj;			// 포인터가 가리키는 객체를 삭제
				obj = nullptr;		// 리스트에 저장된 포인터를 nullptr로 변경
			}
		}
		m_ObjList[i].clear();
	}
}