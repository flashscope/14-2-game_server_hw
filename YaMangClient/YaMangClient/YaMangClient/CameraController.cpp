#include "stdafx.h"
#include "CameraController.h"
#include "InputDispatcher.h"
#include "Renderer.h"
#include "Timer.h"
#include "MouseManager.h"
#include "ResourceManager.h"
#include "MapManager.h"

CameraController::CameraController()
{
}

CameraController::~CameraController()
{
}

void CameraController::Init( float x, float z )
{
	m_EyePoint = { x, 1.0f, z };
	m_LookAtPoint = { 0, 0.5f, 0 };

	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVec3Normalize( &view, &view );

	m_LookAtPoint = m_EyePoint - view * 100;
	m_EyePoint = m_LookAtPoint - view;

	m_ViewMatrix;
	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );

	//////////////////////////////////////////////////////////////////////////
	// 카메라는 view 행렬이므로, 월드 상의 모든 물체에 대해서 사실은 역변환하는 행렬임
	// 그러므로 카메라를 높이값 만큼 이동하는 것은, 다른 모든 물체에 대해서 -높이값 만큼 이동
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, -MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ) - 5.0f, 0 );
	m_ViewMatrix = m_ViewMatrix * heightMatrix;
	Renderer::GetInstance()->SetViewMatrix( m_ViewMatrix );
	
	m_ZoomDirection = ZOOM_DIRECTION_BACK;

	m_Radius = static_cast<float>( ResourceManager::GetInstance()->GetMapSize() ) / 1.2f;

	D3DXMatrixPerspectiveFovLH( &m_ProjMatrix, D3DX_PI / 2, 1.0f, 0.1f, 1000.0f );
}

void CameraController::MoveForward( float speed, bool zoom )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	
	if ( !zoom )
	{
		speed *= ( m_ZoomStatus + 5 );

		// 크레인에 매달린 카메라 같은 효과를 주기 위해 y값 0으로 고정
		view.y = 0;
	}
	D3DXVec3Normalize( &view, &view );
	m_EyePoint += view * speed;
	m_LookAtPoint += view * speed;

	InterpolateCameraRadius();

	m_ViewMatrix;
	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, -MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ) - 5.0f, 0 );
	m_ViewMatrix = m_ViewMatrix * heightMatrix;
	Renderer::GetInstance()->SetViewMatrix( m_ViewMatrix );
}

void CameraController::MoveSide( float speed )
{
	speed *= (m_ZoomStatus + 5);

	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross, &m_UpVector, &view );
	m_EyePoint += cross * speed;
	m_LookAtPoint += cross * speed;

	InterpolateCameraRadius();

	m_ViewMatrix;
	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, -MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ) - 5.0f, 0 );
	m_ViewMatrix = m_ViewMatrix * heightMatrix;
	Renderer::GetInstance()->SetViewMatrix( m_ViewMatrix );
}

void CameraController::MoveElevate( float speed )
{
	m_LookAtPoint.y += speed;
	m_EyePoint.y += speed;

	InterpolateCameraRadius();

	m_ViewMatrix;
	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, -MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ) - 5.0f, 0 );
	m_ViewMatrix = m_ViewMatrix * heightMatrix;
	Renderer::GetInstance()->SetViewMatrix( m_ViewMatrix );
}

//////////////////////////////////////////////////////////////////////////
// Rotate Up과 Rotate Side는 Picking Ray 쏴서
// 피킹 된 점을 기준으로 회전해야 함
//
// 수정해야 됨
//////////////////////////////////////////////////////////////////////////
void CameraController::RotateUp( float angle )
{
	// 회전각 제한 1
	if ( m_LookAtPoint.y >= m_EyePoint.y && angle < 0 )
	{
		return;
	}
	D3DXVECTOR3 preView = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 view = { 0, 0, 0 };
	D3DXVECTOR3 axis = { 0, 0, 0 };

	D3DXVec3Cross( &axis, &m_UpVector, &preView );
	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXVec3TransformCoord( &view, &preView, &rotateMatrix );

	// 회전각 제한 2
	if ( preView.x * view.x < 0 || preView.y * view.y < 0 || preView.z * view.z < 0 )
	{
		return;
	}
	m_LookAtPoint = m_EyePoint + view;

	m_ViewMatrix;
	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, -MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ) - 5.0f, 0 );
	m_ViewMatrix = m_ViewMatrix * heightMatrix;
	Renderer::GetInstance()->SetViewMatrix( m_ViewMatrix );
}

void CameraController::RotateSide( float angle )
{
	// 회전축
	D3DXVECTOR3 axis = { 0, 1, 0 };
	D3DXVECTOR3 pickedPointOfCenter = Renderer::GetInstance()->GetPickedPointOfCenter();
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;

	D3DXMATRIXA16 transMatrix;
	D3DXMATRIXA16 rotateMatrix;

	D3DXMatrixTranslation( &transMatrix, -pickedPointOfCenter.x, 0, -pickedPointOfCenter.z );
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXMATRIXA16 resultMatrix = transMatrix * rotateMatrix;

	D3DXMatrixTranslation( &transMatrix, pickedPointOfCenter.x, 0, pickedPointOfCenter.z );
	resultMatrix = resultMatrix * transMatrix;

	D3DXVec3TransformCoord( &m_LookAtPoint, &m_LookAtPoint, &resultMatrix );
	D3DXVec3TransformCoord( &m_EyePoint, &m_EyePoint, &resultMatrix );
	// m_LookAtPoint = m_EyePoint + view;

	m_ViewMatrix;
	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, -MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ) - 5.0f, 0 );
	m_ViewMatrix = m_ViewMatrix * heightMatrix;
	Renderer::GetInstance()->SetViewMatrix( m_ViewMatrix );
}

void CameraController::ChangeMouseZoomStatus( short zoom )
{
	if ( m_ZoomStatus == ZOOM_STATUS_NONE )
	{
		return;
	}

	if ( zoom < 0 )
	{
		if ( m_ZoomStatus == ZOOM_STATUS_FARTHEST )
		{
			m_ZoomDegree = 2;
			return;
		}
		else if ( m_ZoomDegree > 2 )
		{
			m_ZoomStatus = static_cast<ZoomStatusType>( static_cast<int>(m_ZoomStatus)+1 );
			m_ZoomDegree = 0;
			m_ZoomDirection = ZOOM_DIRECTION_BACK;

			m_ZoomPointX = static_cast<float>( MouseManager::GetInstance()->GetMousePositionX() );
			m_ZoomPointY = static_cast<float>( MouseManager::GetInstance()->GetMousePositionY() );
			COORD zoomPoint = MouseManager::GetInstance()->GetBoundary();

			m_ZoomPointX -= zoomPoint.X / 2;
			m_ZoomPointY -= zoomPoint.Y / 2;
		}

		++m_ZoomDegree;
	}

	if ( zoom > 0 )
	{
		if ( m_ZoomStatus == ZOOM_STATUS_NEAREST )
		{
			m_ZoomDegree = 2;
			return;
		}
		else if ( m_ZoomDegree > 2 )
		{
			m_ZoomStatus = static_cast<ZoomStatusType>( static_cast<int>(m_ZoomStatus)-1 );
			m_ZoomDegree = 0;
			m_ZoomDirection = ZOOM_DIRECTION_FOWARD;

			m_ZoomPointX = static_cast<float>( MouseManager::GetInstance()->GetMousePositionX() );
			m_ZoomPointY = static_cast<float>( MouseManager::GetInstance()->GetMousePositionY() );
			COORD zoomPoint = MouseManager::GetInstance()->GetBoundary();

			m_ZoomPointX -= zoomPoint.X / 2;
			m_ZoomPointY -= zoomPoint.Y / 2;
		}

		++m_ZoomDegree;
	}

	// Log( "zoom enum : %d\n", m_ZoomStatus );
}

void CameraController::Update()
{
	if ( m_ZoomDirection == ZOOM_DIRECTION_NONE )
	{
		return;
	}

	float targetY = 0;
	float targetViewY = 0;

	switch ( m_ZoomStatus )
	{
		case ZOOM_STATUS_NEAREST:
			targetY = NEAREST_Y;
			targetViewY = NEAREST_VIEW_Y;
			break;
		case ZOOM_STATUS_DEGREE_1:
			targetY = DEGREE_1_Y;
			targetViewY = DEGREE_1_VIEW_Y;
			break;
		case ZOOM_STATUS_DEGREE_2:
			targetY = DEGREE_2_Y;
			targetViewY = DEGREE_2_VIEW_Y;
			break;
		case ZOOM_STATUS_DEGREE_3:
			targetY = DEGREE_3_Y;
			targetViewY = DEGREE_3_VIEW_Y;
			break;
		case ZOOM_STATUS_FARTHEST:
		default:
			targetY = FARTHEST_Y;
			targetViewY = FARTHEST_VIEW_Y;
			break;
	}

	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 10.0f;

	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVec3Normalize( &view, &view );
	float	dotView = D3DXVec3Dot( &view, &( -m_UpVector ) );

	// Log( "%-8f도!!! %-8f \n", dotView, targetViewY );
	// Log( "목표 = %f, 현위치 = %f \n", targetY, m_EyePoint.y );

	// 카메라 업
	if ( m_ZoomDirection == ZOOM_DIRECTION_BACK )
	{
		if ( dotView < targetViewY )
		{
			RotateUp( delta * dotView / ( m_EyePoint.y / 10 ) );
		}

		MoveElevate( delta * 10 );
		MoveForward( -delta * 10, true );

		if ( targetY < m_EyePoint.y )
		{
			m_ZoomDirection = ZOOM_DIRECTION_NONE;
			return;
		}
	}
	// 카메라 다운
	else if ( m_ZoomDirection == ZOOM_DIRECTION_FOWARD )
	{
		if ( dotView > targetViewY )
		{
			RotateUp( -delta * dotView / ( m_EyePoint.y / 10 ) );
		}

		MoveElevate( -delta * 10 );
		MoveForward( delta * 10, true );

		if ( targetY > m_EyePoint.y )
		{
			m_ZoomDirection = ZOOM_DIRECTION_NONE;
			return;
		}
	}

	if ( m_ZoomPointX > 0 )
	{
		m_ZoomPointX = __min( m_ZoomPointX, 50 );
		m_ZoomPointX -= delta;
		MoveSide( delta );
	}
	else if ( m_ZoomPointX < 0 )
	{
		m_ZoomPointX = __max( m_ZoomPointX, -50 );
		m_ZoomPointX += delta;
		MoveSide( -delta );
	}

	if ( m_ZoomPointY > 0 )
	{
		m_ZoomPointY = __min( m_ZoomPointY, 50 );
		m_ZoomPointY -= delta;
		MoveForward( -delta );
	}
	else if ( m_ZoomPointY < 0 )
	{
		m_ZoomPointY = __max( m_ZoomPointY, -50 );
		m_ZoomPointY += delta;
		MoveForward( delta );
	}
}

void CameraController::InterpolateCameraRadius()
{
	if ( D3DXVec3Length( &m_EyePoint ) < m_Radius )
	{
		return;
	}

	D3DXVECTOR3 eyeVector;
	D3DXVec3Normalize( &eyeVector, &m_EyePoint );

	m_EyePoint -= eyeVector;
	m_LookAtPoint -= eyeVector;
}
