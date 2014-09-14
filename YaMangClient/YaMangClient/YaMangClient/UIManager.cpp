#include "stdafx.h"
#include "UIManager.h"
#include "UIObject.h"
#include "YaMangDxDll.h"
#include "ResourceDef.h"
#include "SceneManager.h"


UIManager::UIManager()
{
}


UIManager::~UIManager()
{
}

void UIManager::Init()
{

}

void UIManager::Update()
{
	m_NowSceneType = SceneManager::GetInstance()->GetNowSceneType();
	if (m_UIList.empty() == false)
	{
		for (auto& iter = m_UIList.begin(); iter != m_UIList.end();)
		{
			UIObject* thisObject = (*iter);

			if ( !thisObject)
			{
				iter = m_UIList.erase(iter);
				continue;
			}

			(*iter)->Update();

			if ((*iter)->GetSuicide() == true)
			{
				UIObject* tobeRemoved = *iter;
				SafeDelete(tobeRemoved);
				iter = m_UIList.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void UIManager::Render() const
{
	for (auto &iter : m_UIList)
	{
		if (iter->GetVisible() == true && (iter->GetSceneType() == m_NowSceneType))
		{
			iter->Render();
		}
	}
}

void UIManager::AddUIObject(UIObject* Object)
{
	m_UIList.push_back(Object);
}

void UIManager::Destroy()
{

}

