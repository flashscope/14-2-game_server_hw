#include "stdafx.h"
#include "PlayerManager.h"
#include "NetworkManager.h"
#include "PacketType.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "Corps.h"
#include "SoundManager.h"

PlayerManager::PlayerManager()
{
}


PlayerManager::~PlayerManager()
{
	ClearSelectedCorps();
}

bool PlayerManager::IsCorpsInIdList( int corpsId ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		for ( auto& iter : m_SelectedCorpsList )
		{
			if ( iter == corpsId )
			{
				return true;
			}
		}
	}

	return false;
}

void PlayerManager::AttackCorpsById( int corpsID ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) != typeid( *scene ) )
		{
			return;
		}

		ScenePlay* scenePlay = static_cast<ScenePlay*>( scene );
		for ( auto& iter : m_SelectedCorpsList )
		{
			
			const Corps* myCorps = scenePlay->GetCorpsByID( iter );
			const Corps* targetCorps = scenePlay->GetCorpsByID( corpsID );

			if ( nullptr == myCorps || nullptr == targetCorps )
			{
				return;
			}

			if ( myCorps->GetOwnPlayerID( ) == targetCorps->GetOwnPlayerID( ) )
			{
				// 타겟이 내꺼면 공격 중단
				break;
			}
			AttackCorpsRequest attackCorpsData;
			attackCorpsData.m_MyCorpsID = iter;
			attackCorpsData.m_TargetCorpsID = corpsID;

			
			SoundManager::GetInstance()->PlaySound( SOUND_CORPS_MOVE );
			NetworkManager::GetInstance( )->SendPlayPacket( &attackCorpsData );
		}
	}
}

void PlayerManager::MoveCorpsToPosition( float x, float z ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) != typeid( *scene ) )
		{
			return;
		}

		SoundManager::GetInstance()->PlaySound( SOUND_CORPS_MOVE );

		for ( auto& iter : m_SelectedCorpsList )
		{
			MoveCorpsRequest moveCorpsData;

			moveCorpsData.m_CorpsID = iter;
			moveCorpsData.m_NowX = x;
			moveCorpsData.m_NowZ = z;
			moveCorpsData.m_TargetX = x;
			moveCorpsData.m_TargetZ = z;

			NetworkManager::GetInstance( )->SendPlayPacket( &moveCorpsData );
		}
	}
}

void PlayerManager::ChangeCorpsFormation( FormationType formation ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		for ( auto& iter : m_SelectedCorpsList )
		{
			ChangeCorpsFormationRequest changeCorpsFormationData;
			changeCorpsFormationData.m_CorpsID = iter;
			changeCorpsFormationData.m_FormationType = formation;

			NetworkManager::GetInstance( )->SendPlayPacket( &changeCorpsFormationData );
		}
	}
}

