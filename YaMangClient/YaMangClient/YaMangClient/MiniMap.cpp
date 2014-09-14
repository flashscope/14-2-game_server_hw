#include "stdafx.h"
#include "MiniMap.h"
#include "UIManager.h"


MiniMap::MiniMap(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible)
{
	SetSpriteType(key);
	SetUIPosX(PosX);
	SetUIPosY(PosY);
	SetVisible(visible);
	SetSceneType(scene);

	UIManager::GetInstance()->AddUIObject(this);
}

MiniMap::~MiniMap()
{
}

void MiniMap::Update()
{
	UIObject::Update();
}

void MiniMap::Render() const
{
	UIObject::Render();
}
