#pragma once
#include "Scene.h"
#include "EnumSet.h"
class SceneResult:
	public Scene
{
public:
	SceneResult();
	virtual ~SceneResult( );

	virtual void	Create();
	virtual void	Destroy();
	virtual void	Update();
	virtual void	Render() const;

private:
	SceneType		m_SceneType = SCENE_RESULT;

	D3DXVECTOR3 m_Position = { 0, 0, 0 };
};

