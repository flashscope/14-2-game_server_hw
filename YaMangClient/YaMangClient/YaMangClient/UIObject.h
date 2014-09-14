#pragma once

#include "EnumSet.h"

class UIObject //: public Interface3D
{
public:
	UIObject();
	UIObject(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible);
	virtual ~UIObject();

	virtual void		Update();
	virtual void		Render() const;

	D3DXVECTOR3			GetUIPos() { return m_UIPosition; }
	float				GetUIPosX() { return m_UIPosition.x; }
	float				GetUIPosY() { return m_UIPosition.y; }
	bool				GetVisible() { return m_Visible; }
	bool				GetSuicide() { return m_Suicide; }

	SceneType			GetSceneType() { return m_SceneType; }
	SpriteKeyType		GetSpriteType() { return m_SpriteType; }

	void				SetUIPosition(int x, int y);
	void				SetUIPosX(int x) { m_UIPosition.x = static_cast<float>(x); }
	void				SetUIPosX(float x) { m_UIPosition.x = x; }
	void				SetUIPosY(int y) { m_UIPosition.y = static_cast<float>(y); }
	void				SetUIPosY(float y) { m_UIPosition.y = y; }
	void				SetVisible(bool visible) { m_Visible = visible; }
	void				SetSuicide(bool suicide) { m_Suicide = suicide; }

	void				SetSceneType(SceneType type) { m_SceneType = type; }
	void				SetSpriteType(SpriteKeyType type) { m_SpriteType = type; }

private:
	D3DXVECTOR3			m_UIPosition = {0, 0, 0};
	bool				m_Visible = true;
	bool				m_Suicide = false;

	SpriteKeyType		m_SpriteType = SPRITE_NONE;
	SceneType			m_SceneType = SCENE_NONE;
};

