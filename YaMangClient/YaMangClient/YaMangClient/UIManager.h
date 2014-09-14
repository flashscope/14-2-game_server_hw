#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

class UIObject;

class UIManager :public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	void		Init();
	void		Destroy();

	void		Update();
	void		Render() const;

	void		AddUIObject( UIObject* Object );

private:
	std::list<UIObject*> m_UIList;

	SceneType	m_NowSceneType = SCENE_NONE;
};

