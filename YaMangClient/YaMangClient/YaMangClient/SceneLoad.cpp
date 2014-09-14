#include "stdafx.h"
#include "SceneLoad.h"
#include "yaMangDxDll.h"
#include "EnumSet.h"
#include "SoundManager.h"


SceneLoad::SceneLoad()
{
}


SceneLoad::~SceneLoad()
{
}

void SceneLoad::Create()
{

}

void SceneLoad::Destroy()
{

}

void SceneLoad::Update()
{
	if ( !m_SceneIsStart )
	{
		SoundManager::GetInstance()->StopAllSound();
		SoundManager::GetInstance()->PlaySound( SOUND_SCENE_PLAY_BGM3 );
		SoundManager::GetInstance()->SetVolume( SOUND_SCENE_PLAY_BGM3, 0.3f );
		m_SceneIsStart = true;
	}
}

void SceneLoad::Render() const
{
	RenderSprite( SPRITE_LOADING, m_Position );
}
