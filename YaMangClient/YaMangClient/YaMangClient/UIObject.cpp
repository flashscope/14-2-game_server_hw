#include "stdafx.h"
#include "UIObject.h"
#include "YaMangDxDll.h"
#include "UIManager.h"



UIObject::UIObject()
{
}

UIObject::UIObject(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible)
{
	SetSpriteType(key);
	SetUIPosX(PosX);
	SetUIPosY(PosY);
	SetVisible(visible);
	SetSceneType(scene);

	UIManager::GetInstance()->AddUIObject(this);
}

UIObject::~UIObject()
{
}

void UIObject::Update()
{
}

void UIObject::Render() const
{
	RenderSprite( m_SpriteType, m_UIPosition );
}

void UIObject::SetUIPosition( int x, int y )
{
	SetUIPosX( x );
	SetUIPosY( y );
}
