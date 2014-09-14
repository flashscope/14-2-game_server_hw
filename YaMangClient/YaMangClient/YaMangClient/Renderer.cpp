#include "stdafx.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "CameraController.h"
#include "QuadTree.h"
#include "MapManager.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	LONG width = rect.right - rect.left;
	LONG height = rect.bottom - rect.top;

	if ( S_OK == InitD3D( MainWindow::GetInstance( )->Window( ), width, height ) )
	{
		m_Result = true;
		m_PrevTime = timeGetTime();
	}

	return m_Result;
}

void Renderer::Destroy()
{
	if ( m_Result )
	{
		D3DCleanUp( );
	}
}

void Renderer::RenderBegin()
{
	if ( m_Result )
	{
		m_DPCall = 0;
		m_IsReady = PreRendering( );
	}
}

void Renderer::RenderEnd()
{
	if ( m_Result && m_IsReady )
	{
		UINT nowTime = timeGetTime();
		if ( m_PrevTime + 1000 < nowTime )
		{
			m_PrevTime = nowTime;
			m_FPS = m_Frame;
			m_Frame = 0;
		}

		wchar_t ws[100] = { 0, };
		wsprintf( ws, L"FPS : %d,  DPCall : %d,  Map : %d ", m_FPS, m_DPCall, m_HeightMapVertis );
		//TextManager::GetInstance( )->RegistText( TEXT_FPS, ws, 20, 20, SCENE_PLAY );
		m_Frame++;

		PostRendering( );
		m_IsReady = false;
	}
}

void Renderer::RenderMesh( MESHOBJECT* mesh )
{
	if ( mesh && m_IsReady )
	{	
		++m_DPCall;
		Rendering( mesh );
	}
}

void Renderer::RenderMap( QuadTree* quadTree, Frustum* frustum, CUSTOMVERTEX* heightMap, float ratioOfLOD )
{
	if ( ResourceManager::GetInstance()->IsMapReady() )
	{
		LPDWORD index = nullptr;

		if ( !quadTree || !frustum || !heightMap || FAILED( PreRenderHeightWithMapQuadTree( &index ) ))
		{
			RenderHeightMap();
		}
		else
		{
			m_HeightMapVertis = quadTree->GenerateIndex( index, heightMap, frustum, ratioOfLOD );
			RenderHeightMapWithQuadTree( m_HeightMapVertis, m_IsHeightMapWire );
		}

		RenderSkyBox( CameraController::GetInstance()->GetEyePoint() );
	}
}

void Renderer::RenderBillboard( int id, char flag )
{
	DrawBillboardByTexture( id, flag );
}

void Renderer::GetHeightMapSize( DWORD* width, DWORD* height ) const
{
	GetHeightMapSizeForQuadTree( width, height );
	MapManager::GetInstance()->SetHeightMapSize( *width, *height );
}

CUSTOMVERTEX* Renderer::GetHeightMap() const
{
	CUSTOMVERTEX* heightMap;
	GetHeightMapForQuadTree( &heightMap );
	MapManager::GetInstance()->SetHeightMap( heightMap );

	return heightMap;
}

void Renderer::SetViewMatrix( D3DXMATRIXA16& matrix ) const
{
	SetCameraMatrix( &matrix );
}

void Renderer::SetWorldMatrix( D3DXMATRIXA16& matrix ) const
{
	SetWorldAreaMatrix( &matrix );
}

void Renderer::ResizeWindow( LONG width, LONG height )
{
	if ( m_Result )
	{
		SetAspectRatio( width, height );
	}
}

void Renderer::WriteText( LPCWSTR text, float left, float top, int RGB_R /*= 255*/, int RGB_G /*= 255*/, int RGB_B /*= 255*/, float right /*= 0*/, float bottom /*= 0 */ ) const
{
	if ( m_Result )
	{
		RenderText( text, left, top, RGB_R, RGB_G, RGB_B, right, bottom );
	}
}

D3DXVECTOR3 Renderer::GetPickedPointOfCenter() const
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	SHORT centerOfScreenX = static_cast<SHORT>( ( rect.right - rect.left ) / 2 );
	SHORT centerOfScreenY = static_cast<SHORT>( ( rect.bottom - rect.top ) / 2 );

	CalcPickingRay( centerOfScreenX, centerOfScreenY );

	float pickedX = 0;
	float pickedZ = 0;
	if ( S_FALSE != ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() && TransPickedTriangleQuadTree( &pickedX, &pickedZ ) ) || TransPickedTriangle( &pickedX, &pickedZ ) )
	{
		// Log( "피킹 실패하였으므로 다시 피킹합니다. %d \n", rect.bottom - rect.top );
		CalcPickingRay( centerOfScreenX, rect.bottom - rect.top );
		if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
		{
			TransPickedTriangleQuadTree( &pickedX, &pickedZ );
		}
		else
		{
			TransPickedTriangle( &pickedX, &pickedZ );
		}
	}
	// Log( "[%d %d] -> [%f, %f] 으로 피킹 중 \n", centerOfScreenX, centerOfScreenY, pickedX, pickedZ );

	D3DXVECTOR3 returnPoint = { pickedX, 0, pickedZ };
	return returnPoint;
}

void Renderer::SetShader( bool mode )
{
	SetEffect( mode );
}