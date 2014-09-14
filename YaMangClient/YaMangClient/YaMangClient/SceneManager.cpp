#include "stdafx.h"
#include "SceneManager.h"
#include "SceneLoad.h"
#include "ScenePlay.h"
#include "SceneResult.h"

SceneManager::SceneManager()
{
	m_SceneList.fill( nullptr );
}


SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_SceneList[SCENE_LOAD] = new SceneLoad();
	m_SceneList[SCENE_LOAD]->Create( );

	m_SceneList[SCENE_PLAY] = new ScenePlay();
	m_SceneList[SCENE_PLAY]->Create();

	m_SceneList[SCENE_RESULT] = new SceneResult();
	m_SceneList[SCENE_RESULT]->Create();
}

void SceneManager::Destroy()
{
	for ( auto& toBeDelete : m_SceneList )
	{
		if ( toBeDelete != nullptr )
		{
			toBeDelete->Destroy();
			delete toBeDelete;
		}
	}
	m_SceneList.fill( nullptr );
}

void SceneManager::ChangeScene( SceneType scene )
{
	switch ( scene )
	{
		case SCENE_LOAD:
			if ( m_SceneList[SCENE_LOAD] )
			{
				m_NowScene = m_SceneList[SCENE_LOAD];
				m_NowSceneType = SCENE_LOAD;
			}
			break;
		case SCENE_PLAY:
			if ( m_SceneList[SCENE_PLAY] )
			{
				m_NowScene = m_SceneList[SCENE_PLAY];
				m_NowSceneType = SCENE_PLAY;
			}
			break;
		case SCENE_RESULT:
			if ( m_SceneList[SCENE_RESULT] )
			{
				m_NowScene = m_SceneList[SCENE_RESULT];
				m_NowSceneType = SCENE_RESULT;
			}
			break;
		default:
			break;
	}
}

void SceneManager::Update() const
{
	if ( m_NowScene )
	{
		m_NowScene->Update();
	}
}

void SceneManager::Render() const
{
	if ( m_NowScene )
	{
		m_NowScene->Render();
	}
}
