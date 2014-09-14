#include "stdafx.h"
#include "BreadBoard.h"
#include "Arrow.h"
#include "Guard.h"
#include "Knight.h"
#include "Pike.h"
#include "Sword.h"
#include "MacroSet.h"
#include "Corps.h"
#include "MacroSet.h"
#include "Action.h"
#include "Timer.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CameraController.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "King.h"
#include "NetworkManager.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ScenePlay.h"
#include "Frustum.h"

Corps::Corps( int corpsId, int playerId, PositionInfo pos )
: m_CorpsID( corpsId ), m_OwnerPlayerID( playerId )
{
	m_FormationArray.fill( nullptr );
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_DEFENSE )] = new DefenseBread();
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_DESTROY )] = new DestroyBread();
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_RUSH )] = new RushBread();

	m_EyePoint = pos.m_EyePoint;
	m_TargetPoint = pos.m_EyePoint;
	m_LookAtPoint = pos.m_LookAtPoint;
	
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 0.1f, 0.1f, 0.1f };

	ActionDefault action;
	m_Action = action;

	if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
	{
		m_MeshKey = MESH_KEY_CORPS_RUSH_MINE;
		m_IsEnemy = false;
	}
	else
	{
		m_MeshKey = MESH_KEY_CORPS_RUSH_ENEMY;
		m_IsEnemy = true;
	}
}

Corps::~Corps()
{
	for ( auto& toBeDelete : m_UnitList )
	{
		SafeDelete( toBeDelete );
	}
	m_UnitList.clear();

	for ( auto& toBeDelete : m_FormationArray )
	{
		SafeDelete( toBeDelete );
	}
	m_FormationArray.fill( nullptr );
}

void Corps::Create( UINT num, UnitType unitType )
{
	m_UnitType = unitType;

	m_UnitList.reserve( num );
	Unit* unit = nullptr;

	for ( UINT i = 0; i < num; ++i )
	{
		switch ( unitType )
		{
			case  UnitType::UNIT_ARROW:
				unit = new Arrow( this, i );
				m_TextureType = CORPS_TEXTURE_ARROW;
				break;
			case  UnitType::UNIT_GUARD:
				unit = new Guard( this, i );
				m_TextureType = CORPS_TEXTURE_GUARD;
				break;
			case  UnitType::UNIT_KNIGHT:
				unit = new Knight( this, i );
				m_TextureType = CORPS_TEXTURE_KNIGHT;
				break;
			case  UnitType::UNIT_PIKE:
				unit = new Pike( this, i );
				m_TextureType = CORPS_TEXTURE_PIKE;
				break;
			case  UnitType::UNIT_SWORD:
				unit = new Sword( this, i );
				m_TextureType = CORPS_TEXTURE_SWORD;
				break;
			case  UnitType::UNIT_KING:
				unit = new King( this, i );
				m_TextureType = CORPS_TEXTURE_KING;
				break;
			default:
				return;
		}

		if ( unit )
		{
			m_UnitList.push_back( unit );
			unit = nullptr;
		}
	}
}

void Corps::Update()
{
	if ( m_UnitList.empty() )
	{
		SetVisible( false );
		return;
	}

	DoAction();
	for ( auto& iter : m_UnitList )
	{
		iter->Update();
	}

	// 고마해라. 많이 맞았다 아이가...
	if ( Timer::GetInstance()->GetNowTime() - m_AttacktedTime > 2000 )
	{
		m_IsFight = false;
	}
}

void Corps::Render() const
{
	for ( auto& iter : m_UnitList )
	{
		iter->Render();
	}

	if ( !m_Visible || !m_MeshKey || CameraController::GetInstance()->GetHeightGrade() <= 3 )
	{
		return;
	}

	if ( SCENE_PLAY == SceneManager::GetInstance()->GetNowSceneType() )
	{
		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		ScenePlay* scenePlay = static_cast<ScenePlay*>( scene );

		Frustum* frustum = scenePlay->GetFrustum();

		if ( frustum )
		{
			D3DXVECTOR3	center = m_EyePoint;
			if ( !frustum->IsIn( &center ) )
			{
				return;
			}
		}
	}

	D3DXMATRIXA16 thisMatrix = GetMatrix();

	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ), 0 );
	
	float scale = 1.0f + static_cast<float>( m_UnitList.size() * 0.2 );

	D3DXMATRIXA16 scaleMatrix;
	D3DXMatrixScaling( &scaleMatrix, scale, scale, scale );


	thisMatrix = scaleMatrix * thisMatrix * heightMatrix;

	Renderer::GetInstance()->SetWorldMatrix( thisMatrix );

	if ( IsSelected() )
	{
		Renderer::GetInstance()->SetShader( true );
	}
	else
	{
		Renderer::GetInstance()->SetShader( false );
	}

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	if ( mesh )
	{
		Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	}

	if ( m_TextureType != CORPS_TEXTURE_NONE && m_TextureType != CORPS_TEXTURE_MAX )
	{
		D3DXMATRIXA16 viewMatrix = CameraController::GetInstance()->GetViewMatrix();
		D3DXMATRIXA16 billMatrix;
		D3DXMatrixIdentity( &billMatrix );

		billMatrix._11 = viewMatrix._11;
		billMatrix._13 = viewMatrix._13;
		billMatrix._31 = viewMatrix._31;
		billMatrix._33 = viewMatrix._33;

		D3DXMatrixInverse( &billMatrix, NULL, &billMatrix );

		D3DXMATRIXA16 scaleMatrix;
		D3DXMatrixScaling( &scaleMatrix, 3.0f, 3.0f, 3.0f );

		billMatrix = billMatrix * scaleMatrix;

		billMatrix._41 = thisMatrix._41;
		billMatrix._42 = thisMatrix._42 + 5;
		billMatrix._43 = thisMatrix._43;

		Renderer::GetInstance()->SetWorldMatrix( billMatrix );

		char flag = 0;

		if ( IsSelected() )
		{
			flag |= UNIT_STATUS_SELECT;
		}
		if ( IsFight() )
		{
			flag |= UNIT_STATUS_ATTACK;
		}
		if ( IsEnemy() )
		{
			flag |= UNIT_STATUS_ENEMY;
		}

		Renderer::GetInstance()->RenderBillboard( m_TextureType, flag );
	}

	Renderer::GetInstance()->SetShader( false );
}

void Corps::SetVisible( bool visible )
{
	m_Visible = visible;

	for ( auto& iter : m_UnitList )
	{
		iter->SetVisible( visible );
	}
}

D3DXVECTOR3 Corps::GetFormation( int unitId ) const
{
	return m_FormationArray[static_cast<size_t>(m_TargetFormation)]->m_Position[unitId];
}

void Corps::GoFoward()
{
	InterpolatePosition();

	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;

	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * m_Speed / ( m_IsInterpolated ? 2 : 1 );

	m_EyePoint += view * delta;
	m_LookAtPoint += view * delta;
}

void Corps::ClearAction()
{
	ActionDefault act;
	m_Action = act;
}

void Corps::SetAct( ActFunc act )
{
	for ( auto& iter : m_UnitList )
	{
		iter->ChangeAct( act );
	}
}

void Corps::SetActStatus( ActionStatusType actionStatus )
{
	for ( auto& iter : m_UnitList )
	{
		iter->SetActionStatus( actionStatus );
	}
}

void Corps::LookForward()
{
	for ( auto& iter : m_UnitList )
	{
		iter->LookForward();
	}
}

void Corps::SetCorpsHP( int unitNum )
{
	int nowUnitNum = m_UnitList.size();

	while ( nowUnitNum > unitNum )
	{
		if ( nowUnitNum < 1)
		{
			return;
		}
		Unit* unit = m_UnitList.back( );

		// unit->죽는모션?
		// 좀비(?) 매니저 한테 넘겨야 됨
		// 임시로 제거
		SafeDelete( unit );
		
		m_UnitList.pop_back();
		nowUnitNum = m_UnitList.size( );

		// 자 이제부터 쳐 맞기를 시작하지.
		m_AttacktedTime = Timer::GetInstance()->GetNowTime();
		m_IsFight = true;
	}
}

bool Corps::IsContain( float x, float z )
{
	std::shared_ptr<Collision> clickCollision(new Collision( this, 5.0f + CameraController::GetInstance()->GetHeightGrade() ));
	
	D3DXVECTOR3 backupPosition = m_EyePoint;
	m_EyePoint.x = x;
	m_EyePoint.y = MapManager::GetInstance()->GetHeightByPosition( x, z );
	m_EyePoint.z = z;
	
	CollisionManager::GetInstance()->CheckCollision( &(*clickCollision) );
	m_EyePoint = backupPosition;

	if ( clickCollision->IsCollide() )
	{
		Unit* unit = dynamic_cast<Unit*>( clickCollision->GetCompetitor() );
		if ( unit && unit->GetOwner()->GetCorpsID() == m_CorpsID )
		{
			return true;
		}
	}

	return false;
}

void Corps::SetFormation( FormationType formation )
{
	m_TargetFormation = formation;

	switch ( formation )
	{
		case FormationType::FORMATION_DEFENSE:
		{
			if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
			{
				m_MeshKey = MESH_KEY_CORPS_DEFENSE_MINE;
			}
			else
			{
				m_MeshKey = MESH_KEY_CORPS_DEFENSE_ENEMY;
			}
		}
			break;
		case FormationType::FORMATION_DESTROY:
		{
			if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
			{
				m_MeshKey = MESH_KEY_CORPS_DESTROY_MINE;
			}
			else
			{
				m_MeshKey = MESH_KEY_CORPS_DESTROY_ENEMY;
			}
		}
			break;
		case FormationType::FORMATION_RUSH:
		case FormationType::FORMATION_NONE:
		case FormationType::FORMATION_MAX:
		default:
		{
			if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
			{
				m_MeshKey = MESH_KEY_CORPS_RUSH_MINE;
			}
			else
			{
				m_MeshKey = MESH_KEY_CORPS_RUSH_ENEMY;
			}
		}
			break;
	}
}

void Corps::InterpolatePosition()
{
	D3DXVECTOR3 distance = m_TargetPoint - m_EyePoint;
	if ( D3DXVec3Length( &distance ) > 3.0f )
	{
		float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
		float delta = time * m_Speed / ( m_IsMoved ? 2 : 1 );

		// Log( "목표치까지 거리 %4f ( %4f %4f ) -> (%4f %4f ) \n", D3DXVec3Length( &distance ),
		// 	 m_EyePoint.x, m_EyePoint.z, m_TargetPoint.x, m_TargetPoint.z);

		D3DXVec3Normalize( &distance, &distance );
		m_EyePoint += delta * distance;
		m_LookAtPoint += delta * distance;

		m_IsInterpolated = true;
	}
	else
	{
		m_IsInterpolated = false;
	}
}

void Corps::StartFight()
{
	m_IsFight = true;
	m_AttacktedTime = Timer::GetInstance()->GetNowTime();
}
