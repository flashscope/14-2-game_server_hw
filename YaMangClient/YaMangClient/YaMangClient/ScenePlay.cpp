#include "stdafx.h"
#include "SharedDefine.h"
#include "Corps.h"
#include "ScenePlay.h"
#include "MacroSet.h"

#include "EnumSet.h"
#include "Action.h"
#include "SoundManager.h"
#include "MouseRender.h"
#include "NetworkManager.h"
#include "CameraController.h"
#include "TextManager.h"
#include "Renderer.h"
#include "UIObject.h"
#include "MiniMap.h"
#include "CorpsMark.h"
#include "Timer.h"
#include "Frustum.h"
#include "QuadTree.h"
#include "ResourceManager.h"

ScenePlay::ScenePlay()
{
}


ScenePlay::~ScenePlay()
{
}

//렌더 매니저 초기화가 우선되어야 함
void ScenePlay::Create()
{
	// 하드코딩으로 임시 추가 원래는 게임 시작패킷으로 받아야함
	int guardNum = 3;
	m_BasePlayerIDList.clear();
	m_BasePlayerIDList.reserve( guardNum ); // 임의로 3개 지정

	for ( int i = 0; i < guardNum; ++i )
	{
		m_BasePlayerIDList.push_back( 0 ); // 0은 봇의 id
	}

	m_MouseCursor = new MouseRender();
	
	// 뷰 프러스텀을 만들어 봅시다
	m_Frustum = new Frustum();

	if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
	{
		// 쿼드 트리도 만들어 봅시다.
		m_HeightMap = Renderer::GetInstance()->GetHeightMap();

		if ( m_HeightMap )
		{
			DWORD width = 0;
			DWORD height = 0;

			Renderer::GetInstance()->GetHeightMapSize( &width, &height );
			m_QuadTree = new QuadTree( width, height );

			m_QuadTree->Build( m_HeightMap );
		}
	}

	//UIObjects Init
	InitUIObjects();
}

void ScenePlay::Destroy()
{
	for ( auto iter : m_CorpsList )
	{
		auto& toBeDelete = iter.second;
		SafeDelete( toBeDelete );
	}
	m_CorpsList.clear();

	SafeDelete( m_Frustum );
	SafeDelete( m_MouseCursor );
}

void ScenePlay::Update()
{
	D3DXMATRIXA16	viewProjMatrix;
	viewProjMatrix = CameraController::GetInstance()->GetViewMatrix() * CameraController::GetInstance()->GetProjMatrix();
	m_Frustum->Make( &viewProjMatrix );
	
	for ( auto& iter : m_CorpsList )
	{
		auto& corps = iter.second;
		corps->Update();
	}

	m_MouseCursor->Update();

	//Map 보일지 말지 업데이트. 키보드 누를 때마다 인풋 디스패쳐에서 SetVisible 설정해줌
	m_Minimap->SetVisible(m_IsMapVisible);
	CheckRegenFlag();
}

void ScenePlay::Render() const
{
	Renderer::GetInstance()->RenderMap( m_QuadTree, m_Frustum, m_HeightMap, m_LODRatio );
	TextManager::GetInstance()->DrawTexts();

	for ( auto& iter : m_CorpsList )
	{
		auto& corps = iter.second;
		corps->Render();
	}

	m_MouseCursor->Render();
}

void ScenePlay::AddCorps( int corpsID, Corps* corps )
{
	if ( m_CorpsList.find( corpsID ) == m_CorpsList.end() )
	{
		m_CorpsList[corpsID] = corps;

		//부대 마크 추가
		//int PosX = static_cast<int>((corps->GetEyePoint().x / 512 * 400) + 570); // 맵 상대위치 하드코딩
		//int PosY = static_cast<int>((-corps->GetEyePoint().z / 512 * 400) + 320);
		SpriteKeyType spriteType = SPRITE_NONE;
		if (corps->GetOwnPlayerID() == NetworkManager::GetInstance()->GetMyPlayerID())
		{
			//RegenTime 초기화
			m_StackedTime = 0;

			if (corps->GetUnitType() == UnitType::UNIT_GUARD)
			{
				spriteType = SPRITE_UI_CORPSFLAG_BLUE;
			}
			else
			{
				spriteType = SPRITE_UI_CORPSMARK_BLUE;
			}
		}
		else if (corps->GetOwnPlayerID() == 0)
		{
			spriteType = SPRITE_UI_CORPSFLAG_GRAY;
		}
		else
		{
			if (corps->GetUnitType() == UnitType::UNIT_GUARD)
			{
				spriteType = SPRITE_UI_CORPSFLAG_RED;
			}
			else
			{
				spriteType = SPRITE_UI_CORPSMARK_RED;
			}
		}
		CorpsMark* mark = new CorpsMark(spriteType, SCENE_PLAY, 350, 100, false, corps, true);
		m_CorpsMarkList.push_back(mark);

	}
	else
	{
		// CheckCorps로 이미 체크하므로 필요 없을 듯... 지울까요? 혹시 모르니...
		SafeDelete( corps );

		// 중복 ID로 다시 보냈음
		assert( false );
		return;
	}
}

bool ScenePlay::CheckCorps( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) == m_CorpsList.end() )
	{
		return true;
	}
	return false;
}

void ScenePlay::ChangeCorpsFormation( int corpsID, FormationType formationType )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return;
		}
		m_CorpsList[corpsID]->SetFormation( formationType );
		ActionTransFormation action;
		m_CorpsList[corpsID]->ChangeAction( action );
	}
}


void ScenePlay::StartAttackCorps( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return;
		}
		m_CorpsList[corpsID]->StartFight();
	}
}


void ScenePlay::MoveCorpsStart( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector, float speed )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return;
		}
		m_CorpsList[corpsID]->SetTargetPosition( targetPosition );
		m_CorpsList[corpsID]->SetLookAtVector( lookAtVector );
		m_CorpsList[corpsID]->SetSpeed( speed );
		m_CorpsList[corpsID]->SetMoved( true );

		ActionMovePosition action;
		m_CorpsList[corpsID]->ChangeAction( action );
	}
}

void ScenePlay::MoveCorpsStop( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtPoint )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return;
		}
		// D3DXVECTOR3 view = lookAtPoint - targetPosition;
		m_CorpsList[corpsID]->SetTargetPosition( targetPosition );
		m_CorpsList[corpsID]->SetLookAtPoint( lookAtPoint );
		m_CorpsList[corpsID]->SetMoved( false );
		
		ActionStoppingPosition action;
		m_CorpsList[corpsID]->ChangeAction( action );
	}
}

void ScenePlay::SetCorpsHP( int corpsID, int unitNum )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return;
		}
		m_CorpsList[corpsID]->SetCorpsHP( unitNum );
	}
}

void ScenePlay::SyncOneCorp( int corpsID, D3DXVECTOR3 corpsNow, D3DXVECTOR3 corpsLook, int unitNum, FormationType formationType )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return;
		}
		m_CorpsList[corpsID]->SetTargetPosition( corpsNow );
		m_CorpsList[corpsID]->SetLookAtPoint( corpsLook );
		m_CorpsList[corpsID]->SetCorpsHP( unitNum );
		m_CorpsList[corpsID]->SetFormation( formationType );
	}
}

const Corps* ScenePlay::GetCorpsByID( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		return m_CorpsList[corpsID];
	}
	return nullptr;
}

UnitType ScenePlay::GetUnitTypeByID( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
			return UnitType::UNIT_NONE;
		}
		return m_CorpsList[corpsID]->GetUnitType();
	}
	return UnitType::UNIT_NONE;
}

Corps* ScenePlay::SearchCorpsByPosition( float x, float z, bool selectOption )
{
	Corps* resultCorps = nullptr;
	for ( auto& iter : m_CorpsList )
	{
		auto& thisCorps = iter.second;

		// 하나만 피킹 하기 위해서
		if ( resultCorps )
		{
			if ( selectOption )
			{
				thisCorps->SetSelected( false );
			}
			continue;
		}

		if ( thisCorps->GetOwnPlayerID() != NetworkManager::GetInstance()->GetMyPlayerID() )
		{
			// continue;
		}

		D3DXVECTOR3& thisPosition = thisCorps->GetEyePoint();
		
		double	margin = CameraController::GetInstance()->GetHeightGrade() + 5;
		// AABB
		if ( ( thisPosition.x - x < margin && thisPosition.x - x > -margin )
			 && ( thisPosition.x - x < margin && thisPosition.x - x > -margin ) )
		{
			// Log( "%d번 부대 1차 피킹!!! \n", thisCorps->GetCorpsID() );
		
			// BoundingCircle
			if ( thisCorps->IsContain( x, z ) )
			{
				// Log( "%d번 부대 2차 피킹!!! 성공! \n", thisCorps->GetCorpsID() );

				if ( selectOption )
				{
					thisCorps->SetSelected( true );
				}
				resultCorps = thisCorps;
			}
			else if ( selectOption )
			{
				thisCorps->SetSelected( false );
			}
		}
		else if ( selectOption )
		{
			thisCorps->SetSelected( false );
		}
	}

	return resultCorps;
}

void ScenePlay::UpdateBase( int BaseIndex, int playerID )
{
	// 안전장치를 여기에 달자...
	m_BasePlayerIDList.at( BaseIndex ) = playerID;

	// 테스트용 프린트 실제로는 사용 안함
	std::wstring test = L"";
	for ( UINT i = 0; i < m_BasePlayerIDList.size(); ++i )
	{
		test = test + L"[";
		test = test + std::to_wstring( i );
		test = test + L"][";
		test = test + std::to_wstring( m_BasePlayerIDList.at( i ) );
		test = test + L"] ";
	}
	//TextManager::GetInstance( )->RegistText( TEXT_TEST_BASE, test.c_str( ), 250, 300, SCENE_PLAY );
}

void ScenePlay::InitUIObjects()
{
	m_RegenBar = new UIObject(SPRITE_UI_REGEN_BAR, SCENE_PLAY, 250, 50, true);
	m_RegenFlag = new UIObject(SPRITE_UI_REGEN_FLAG, SCENE_PLAY, 250, 31, true);
	m_Minimap = new MiniMap(SPRITE_UI_MAP, SCENE_PLAY, 350, 100, false);
}

void ScenePlay::SetMapVisible(bool visible)
{
	m_IsMapVisible = visible;
	for (auto& iter : m_CorpsMarkList) 
	{ 
		iter->SetVisible(visible); 
	}
}

void ScenePlay::CheckRegenFlag()
{
	m_StackedTime += Timer::GetInstance()->GetElapsedTime();
	
	if (m_RegenTime > 0)
	{
		float flagPos = static_cast<float>(((m_StackedTime / m_RegenTime) * 700) + 250);

		if (flagPos >= 180)
		{
			m_RegenFlag->SetUIPosX(flagPos);
		}
	}
}
