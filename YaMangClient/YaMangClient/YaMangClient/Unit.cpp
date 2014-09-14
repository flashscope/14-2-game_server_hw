#include "stdafx.h"
#include "Corps.h"
#include "Unit.h"
#include "SharedDefine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "Act.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CameraController.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "Frustum.h"

Unit::Unit( Corps* owner, UINT unitId )
: m_Corps( owner ), m_UnitID( unitId )
{
	ClearAct();
	m_Collision = new Collision( this, 0.7f );
	CollisionManager::GetInstance()->AddCollision( m_Collision );
}

Unit::~Unit()
{
	CollisionManager::GetInstance()->RemoveColiision( m_Collision );
	SafeDelete( m_Collision );
}

void Unit::Update()
{
	DoAct();
}

void Unit::Render() const
{
	if ( !m_Visible || !m_MeshKey || CameraController::GetInstance()->GetHeightGrade() > 3 )
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

	D3DXMATRIXA16 thisMatrix = GetMatrix( false );

	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ), 0 );
	
	thisMatrix = thisMatrix * heightMatrix;
	Renderer::GetInstance()->SetWorldMatrix( thisMatrix );

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	//if ( mesh )
	//{
	//	Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	//}

	char flag = 0;

	if ( m_Corps->IsSelected() )
	{
		flag |= UNIT_STATUS_SELECT;

		D3DXMATRIXA16 viewMatrix = CameraController::GetInstance()->GetViewMatrix();
		D3DXMATRIXA16 billMatrix;
		D3DXMatrixIdentity( &billMatrix );

		billMatrix._11 = viewMatrix._11;
		billMatrix._13 = viewMatrix._13;
		billMatrix._31 = viewMatrix._31;
		billMatrix._33 = viewMatrix._33;

		D3DXMatrixInverse( &billMatrix, NULL, &billMatrix );

		D3DXMATRIXA16 scaleMatrix;
		D3DXMatrixScaling( &scaleMatrix, 1.3f, 1.3f, 1.3f );

		billMatrix = billMatrix * scaleMatrix;

		billMatrix._41 = thisMatrix._41;
		billMatrix._42 = thisMatrix._42;
		billMatrix._43 = thisMatrix._43;

		Renderer::GetInstance()->SetWorldMatrix( billMatrix );
	}
	if ( m_Corps->IsFight() )
	{
		flag |= UNIT_STATUS_ATTACK;
	}
	if ( m_Corps->IsEnemy() )
	{
		flag |= UNIT_STATUS_ENEMY;
	}

	Renderer::GetInstance()->RenderBillboard( m_Corps->GetCorpsTextureType(), flag );
	Renderer::GetInstance()->SetShader( false );
}

void Unit::SetStartPosition()
{
	D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();

	D3DXMATRIXA16 formMatrix;
	D3DXMatrixIdentity( &formMatrix );

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitID ) ) * 15;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

	formMatrix = formMatrix * parentMatrix;

	D3DXVec3TransformCoord( &m_EyePoint, &m_EyePoint, &formMatrix );
	D3DXVec3TransformCoord( &m_LookAtPoint, &m_LookAtPoint, &formMatrix );

	m_TargetPoint = m_LookAtPoint;
}

void Unit::FindDestination()
{
	D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();

	D3DXMATRIXA16 formMatrix;
	D3DXMatrixIdentity( &formMatrix );

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitID ) ) * 15;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

	formMatrix = formMatrix * parentMatrix;

	D3DXVECTOR3	targetPoint = { 0.0f, 0.0f, 0.1f };
	D3DXVec3TransformCoord( &m_TargetPoint, &targetPoint, &formMatrix );

	/*Log( "%f %f %f \n", m_TargetPoint.x, m_TargetPoint.y, m_TargetPoint.z );
	Log( "%f %f %f \n", m_EyePoint.x, m_EyePoint.y, m_EyePoint.z );
	Log( "%f %f %f \n\n", m_LookAtPoint.x, m_LookAtPoint.y, m_LookAtPoint.z );*/
}

void Unit::RotateToDestination()
{
	m_ActionStatus = ACTION_STATUS_NONE;

	if ( m_LookAtPoint.x != m_TargetPoint.x )
	{
		m_LookAtPoint.x = m_TargetPoint.x;
		m_ActionStatus = ACTION_STATUS_ROTATE;
	}
	if ( m_LookAtPoint.z != m_TargetPoint.z )
	{
		m_LookAtPoint.z = m_TargetPoint.z;
		m_ActionStatus = ACTION_STATUS_ROTATE;
	}
}

void Unit::MoveToDestination()
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() );
		
	CollisionManager::GetInstance()->CheckCollision( m_Collision );
	if ( m_Collision->IsCollide() )
	{
		D3DXVECTOR3 rev = m_Collision->GetReverseVector();
		D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
		rev.y = view.y = 0;
		D3DXVec3Normalize( &view, &view );

		// Log( "충돌 중!!!! %f %f %f \n", rev.x, rev.y, rev.z );

		if ( D3DXVec3Dot( &rev, &view ) > 0 )
		{
			D3DXVECTOR3 cross;
			D3DXVec3Cross( &cross, &view, &m_UpVector );

			m_EyePoint += cross * static_cast<float>( 10 - m_UnitID )* time / 1000;
			m_LookAtPoint += cross * static_cast<float>( 10 - m_UnitID )* time / 1000;

			// rev = (rev + view) / 100;
			// m_Collision->GetCompetitor()->SetEyePoint( m_Collision->GetCompetitor()->GetEyePoint() - static_cast<float>( 10 - m_UnitID )* rev * time / 1000 );
		}
		else
		{
			m_EyePoint += rev * static_cast<float>( 10 - m_UnitID )* time / 1000;
			m_LookAtPoint += rev * static_cast<float>( 10 - m_UnitID )* time / 1000;
		}
	}

	if ( m_TargetPoint.x - m_EyePoint.x > 0.5f || m_TargetPoint.x - m_EyePoint.x < -0.5f )
	{
		m_EyePoint.x += ( m_TargetPoint.x - m_EyePoint.x ) * time / 1000;
		m_ActionStatus = ACTION_STATUS_GOFOWARD;
	}
	if ( m_TargetPoint.z - m_EyePoint.z > 0.5f || m_TargetPoint.z - m_EyePoint.z < -0.5f )
	{
		m_EyePoint.z += ( m_TargetPoint.z - m_EyePoint.z ) * time / 1000;
		m_ActionStatus = ACTION_STATUS_GOFOWARD;
	}
}

void Unit::ClearAct()
{
	ActDefault act;
	m_Act = act;
}

void Unit::LookForward()
{
	// m_LookAtPoint = m_LookAtPoint * 0.9f + m_Corps->GetLookAtVector() * 0.1f;
}