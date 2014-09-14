#pragma once
#include "EnumSet.h"
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void	Create() = 0;
	virtual void	Destroy() = 0;

	virtual void	Update() = 0;
	virtual void	Render() const = 0;

	SceneType		GetSceneType() { return m_SceneType; }

protected:
	bool			m_SceneIsStart = false;

private:
	SceneType		m_SceneType;

};

