#include "stdafx.h"
#include "CorpsMark.h"
#include "UIObject.h"
#include "UIManager.h"
#include "Corps.h"


CorpsMark::CorpsMark(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible, Corps* corps, bool ourCorps)
{
	SetSpriteType(key);
	SetUIPosX(PosX);
	SetUIPosY(PosY);
	SetVisible(visible);
	SetSceneType(scene);
	SetCorps(corps);
	SetIsOurCorps(ourCorps);

	UIManager::GetInstance()->AddUIObject(this);
}


CorpsMark::~CorpsMark()
{
}

void CorpsMark::Update()
{
	UIObject::Update();
	// 부대위치 실시간 확인 변경
	SetMarkPos();
	// 부대 죽었는지 실시간으로 확인해서 플래그 변경
	CheckSuicide();
	
}

void CorpsMark::Render() const
{
	UIObject::Render();
}

void CorpsMark::SetMarkPos()
{
	if (m_Corps)
	{
		SetUIPosX(static_cast<int>((m_Corps->GetEyePoint().x / 512 * 390) + 590)); // 맵 상대위치 하드코딩
		SetUIPosY(static_cast<int>((-m_Corps->GetEyePoint().z / 512 * 390) + 340));
	}
}

void CorpsMark::CheckSuicide()
{
	if (m_Corps->GetCorpsHP() == 0)
	{
		SetSuicide(true);
	}
}
