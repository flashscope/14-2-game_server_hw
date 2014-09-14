#include "stdafx.h"
#include "SceneResult.h"
#include "yaMangDxDll.h"
#include "EnumSet.h"
#include "PlayerManager.h"
#include "TextManager.h"
#include "SoundManager.h"


SceneResult::SceneResult()
{
}


SceneResult::~SceneResult()
{
}

void SceneResult::Create()
{

}

void SceneResult::Destroy()
{

}

void SceneResult::Update()
{
	if ( !m_SceneIsStart )
	{
		if ( PlayerManager::GetInstance()->GetWin() )
		{
			SoundManager::GetInstance()->StopAllSound();
			SoundManager::GetInstance()->PlaySound( SOUND_SCENE_RESULT_BGM_WIN );
			SoundManager::GetInstance()->SetVolume( SOUND_SCENE_RESULT_BGM_WIN, 0.3f );
		}
		else
		{
			SoundManager::GetInstance()->StopAllSound();
			SoundManager::GetInstance()->PlaySound( SOUND_SCENE_RESULT_BGM_LOSE );
			SoundManager::GetInstance()->SetVolume( SOUND_SCENE_RESULT_BGM_LOSE, 0.3f );
		}
		m_SceneIsStart = true;
	}
}

void SceneResult::Render() const
{
	if ( PlayerManager::GetInstance()->GetWin() )
	{
		RenderSprite( SPRITE_RESULT_WIN, m_Position );
	}
	else
	{
		RenderSprite( SPRITE_RESULT_LOSE, m_Position );
	}

	TextManager::GetInstance()->DrawTexts();
}
