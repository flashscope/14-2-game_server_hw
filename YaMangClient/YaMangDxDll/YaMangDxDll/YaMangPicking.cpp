#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// Picking Ray를 위한 부분
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API void CalcPickingRay( int mouseX, int mouseY )
{
	D3DVIEWPORT9 viewPort;
	D3DXMATRIXA16 projectionMatrix;

	float rayX = 0.f;
	float rayY = 0.f;

	g_D3dDevice->GetViewport( &viewPort );
	g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

	rayX = ( ( ( ( ( mouseX - viewPort.X ) * 2.0f / (float)viewPort.Width ) - 1.0f ) ) - projectionMatrix( 2, 0 ) ) / projectionMatrix( 0, 0 );
	rayY = ( ( -( ( ( mouseY - viewPort.Y ) * 2.0f / (float)viewPort.Height ) - 1.0f ) ) - projectionMatrix( 2, 1 ) ) / projectionMatrix( 1, 1 );
	// Log( "Ray x : %f, y : %f\n", rayX, rayY );

	//viewport트랜스, 프로젝션 트랜스 역행
	g_RayOrigin = { 0.f, 0.f, 0.f };
	g_RayDirection = { rayX, rayY, 1.f };
	// Log( "뷰포트, 프로젝션 역행\n" );

	//뷰잉 트랜스 역행
	D3DXMATRIXA16 viewingMatrix;
	g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
	D3DXMatrixInverse( &viewingMatrix, 0, &viewingMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &viewingMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &viewingMatrix );
	// Log( "뷰잉 좌표 역행\n" );

	//월드 좌표로 역행
	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
	D3DXMatrixInverse( &worldMatrix, 0, &worldMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &worldMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &worldMatrix );
	// Log( "월드 좌표 역행\n" );
	// Log( "origin: %f,%f,%f\n direction: %f, %f, %f\n", g_RayOrigin.x, g_RayOrigin.y, g_RayOrigin.z, g_RayDirection.x, g_RayDirection.y, g_RayDirection.z );

}

YAMANGDXDLL_API HRESULT TransPickedTriangle( float* pickedX, float* pickedZ, int* PickedTriPointA, int* PickedTriPointB, int* PickedTriPointC )
{
	if ( !( pickedX && pickedZ ) )
	{
		return S_FALSE;
	}

	LPDIRECT3DVERTEXBUFFER9 presentVertexBuffer;
	g_Mesh->GetVertexBuffer( &presentVertexBuffer );

	CUSTOMVERTEX* VerticesStartPoint;
	presentVertexBuffer->Lock( 0, 0, (void**)&VerticesStartPoint, 0 );

	BOOL hit1 = false;
	BOOL hit2 = false;
	float dist1 = 0;
	float dist2 = 0;

	int trianglePointA = NULL;
	int trianglePointB = NULL;
	int trianglePointC = NULL;
	int trianglePointD = NULL;

	
	//한번 루프에에서 4점씩 확인(ABCD)
	//4점으로 삼각형 2개를 확인할 수 있기 때문에 한 루프에서 hit가 두 종류 발생될 수 있음
	//hit가 발생하면 더 이상 탐색하지 않아도 되기 때문에 loop문 탈출
	for ( UINT z = 0; ( z < ( g_ZHeight - 1 ) ) && !( hit1 | hit2 ); ++z )
	{
		if ( z % 2 == 0 )
		{
			for ( UINT x = 0; ( x < ( g_XHeight - 1 ) ) && !( hit1 | hit2 ); ++x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x + 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].m_VertexPoint, &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x + 1 );
				hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &VerticesStartPoint[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
		else
		{
			for ( UINT x = g_ZHeight - 1; ( x > 0 ) && !( hit1 | hit2 ); --x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x - 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].m_VertexPoint, &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x - 1 );
				hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &VerticesStartPoint[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
	}

	HRESULT result = S_FALSE;
	CUSTOMVERTEX* PickedPointA = nullptr;
	CUSTOMVERTEX* PickedPointB = nullptr;
	CUSTOMVERTEX* PickedPointC = nullptr;

	if ( ( hit1 && ( dist1 > 0 ) ) || ( hit2 && ( dist2 > 0 ) ) )
	{
		//CUSTOMVERTEX* intersectedVertexBufferStart;
		//g_Mesh->LockVertexBuffer( 0, (void**)&intersectedVertexBufferStart );

		if ( hit1 )
		{
			PickedPointA = &VerticesStartPoint[trianglePointA];
			PickedPointB = &VerticesStartPoint[trianglePointB];
			PickedPointC = &VerticesStartPoint[trianglePointC];
			
			if (PickedTriPointA && PickedTriPointB && PickedTriPointC)
			{
				*PickedTriPointA = trianglePointA;
				*PickedTriPointB = trianglePointB;
				*PickedTriPointC = trianglePointC;
			}
			

		}
		else
		{
			PickedPointA = &VerticesStartPoint[trianglePointB];
			PickedPointB = &VerticesStartPoint[trianglePointC];
			PickedPointC = &VerticesStartPoint[trianglePointD];

			if (PickedTriPointA && PickedTriPointB && PickedTriPointC)
			{
				*PickedTriPointA = trianglePointB;
				*PickedTriPointB = trianglePointC;
				*PickedTriPointC = trianglePointD;
			}
			
		}

		*pickedX += PickedPointA->m_VertexPoint.x;
		*pickedZ += PickedPointA->m_VertexPoint.z;

		//g_Mesh->UnlockVertexBuffer();

		result = S_OK;
	}

	presentVertexBuffer->Unlock();
	presentVertexBuffer->Release();

	return result;
}


YAMANGDXDLL_API HRESULT TransPickedTriangleQuadTree( float* pickedX, float* pickedZ, int* PickedTriPointA, int* PickedTriPointB, int* PickedTriPointC )
{
	if ( !( pickedX && pickedZ ) || !g_HeightMap )
	{
		return S_FALSE;
	}

	BOOL hit1 = false;
	BOOL hit2 = false;
	float dist1 = 0;
	float dist2 = 0;

	int trianglePointA = NULL;
	int trianglePointB = NULL;
	int trianglePointC = NULL;
	int trianglePointD = NULL;
	
	for ( UINT z = 0; ( z < ( g_HeightMapHeight - 1 ) ) && !( hit1 | hit2 ); ++z )
	{
		for ( UINT x = 0; ( x < ( g_HeightMapWidth - 1 ) ) && !( hit1 | hit2 ); ++x )
		{
			trianglePointA = g_HeightMapHeight * z + x;
			trianglePointB = g_HeightMapHeight * z + ( x + 1 );
			trianglePointC = g_HeightMapHeight *( z + 1 ) + x;
			hit1 = D3DXIntersectTri( &g_HeightMap[trianglePointA].m_VertexPoint, &g_HeightMap[trianglePointB].m_VertexPoint, &g_HeightMap[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

			trianglePointD = g_ZHeight*( z + 1 ) + ( x + 1 );
			hit2 = D3DXIntersectTri( &g_HeightMap[trianglePointB].m_VertexPoint, &g_HeightMap[trianglePointC].m_VertexPoint, &g_HeightMap[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
		}
	}

	HRESULT result = S_FALSE;
	CUSTOMVERTEX* PickedPointA = nullptr;
	CUSTOMVERTEX* PickedPointB = nullptr;
	CUSTOMVERTEX* PickedPointC = nullptr;

	if ( ( hit1 && ( dist1 > 0 ) ) || ( hit2 && ( dist2 > 0 ) ) )
	{
		//CUSTOMVERTEX* intersectedVertexBufferStart;
		//g_Mesh->LockVertexBuffer( 0, (void**)&intersectedVertexBufferStart );

		if ( hit1 )
		{
			PickedPointA = &g_HeightMap[trianglePointA];
			PickedPointB = &g_HeightMap[trianglePointB];
			PickedPointC = &g_HeightMap[trianglePointC];

			if ( PickedTriPointA && PickedTriPointB && PickedTriPointC )
			{
				*PickedTriPointA = trianglePointA;
				*PickedTriPointB = trianglePointB;
				*PickedTriPointC = trianglePointC;
			}
		}
		else
		{
			PickedPointA = &g_HeightMap[trianglePointB];
			PickedPointB = &g_HeightMap[trianglePointC];
			PickedPointC = &g_HeightMap[trianglePointD];

			if ( PickedTriPointA && PickedTriPointB && PickedTriPointC )
			{
				*PickedTriPointA = trianglePointB;
				*PickedTriPointB = trianglePointC;
				*PickedTriPointC = trianglePointD;
			}
		}

		*pickedX += PickedPointA->m_VertexPoint.x;
		*pickedZ += PickedPointA->m_VertexPoint.z;

		result = S_OK;
	}
	
	return result;
}

enum AreaModeType
{
	AREA_MODE_NONE,

	AREA_MODE_HIGHER,
	AREA_MODE_LOWER,

	AREA_MODE_PAINT_TEXTURE,

	AREA_MODE_MAX
};

YAMANGDXDLL_API void MapToolPickingEvent( int modeSelector, int PickedTriPointA, int PickedTriPointB, int PickedTriPointC )
{
	modeSelector = static_cast<AreaModeType>( modeSelector );

	LPDIRECT3DVERTEXBUFFER9 presentVertexBuffer;
	g_Mesh->GetVertexBuffer( &presentVertexBuffer );

	CUSTOMVERTEX* VerticesStartPoint;
	presentVertexBuffer->Lock( 0, 0, (void**)&VerticesStartPoint, 0 );



	//해당 모드에 따라서 동작이 발생하도록 하면 됨
	switch ( modeSelector )
	{
		case AREA_MODE_NONE:
			break;

		case AREA_MODE_HIGHER:
			VerticesStartPoint[PickedTriPointA].m_VertexPoint.y += 3;
			VerticesStartPoint[PickedTriPointB].m_VertexPoint.y += 3;
			VerticesStartPoint[PickedTriPointC].m_VertexPoint.y += 3;
			break;

		case AREA_MODE_LOWER:
			VerticesStartPoint[PickedTriPointA].m_VertexPoint.y -= 3;
			VerticesStartPoint[PickedTriPointB].m_VertexPoint.y -= 3;
			VerticesStartPoint[PickedTriPointC].m_VertexPoint.y -= 3;
			break;

		case AREA_MODE_PAINT_TEXTURE:
			//텍스쳐 입히는 방법은 4가지가 존재함
			//삼각형 모양이 4가지이기 때문에
// 			g_PickedPointA->m_VertexTexturePoint1.x = 0.f;
// 			g_PickedPointA->m_VertexTexturePoint1.y = 0.f;
// 			g_PickedPointB->m_VertexTexturePoint1.x = 1.f;
// 			g_PickedPointB->m_VertexTexturePoint1.y = 0.f;
// 			g_PickedPointC->m_VertexTexturePoint1.x = 1.f;
// 			g_PickedPointC->m_VertexTexturePoint1.y = 1.f;
			break;

		default:
			break;
	}

	presentVertexBuffer->Unlock();
	presentVertexBuffer->Release();
}
