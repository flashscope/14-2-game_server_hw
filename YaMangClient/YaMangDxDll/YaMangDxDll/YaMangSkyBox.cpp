#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"


//////////////////////////////////////////////////////////////////////////
// SKYBOX
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API bool SetSkyBoxTexture( LPCTSTR skyBoxTexture, int id )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, skyBoxTexture, &g_SkyBoxTextures[id] ) ) )
	{
		return false;
	}
	return true;
}

YAMANGDXDLL_API HRESULT InitSkyBoxMesh( int size )
{
	for ( int indexBuffer = 0; indexBuffer < 6; ++indexBuffer )
	{
		if ( g_SkyBoxTextures[indexBuffer] != NULL )
		{
			g_SkyBoxTextures[indexBuffer]->Release();
			g_SkyBoxTextures[indexBuffer] = NULL;
		}
	}

	HRESULT hr = S_OK;

	if ( FAILED( hr = D3DXCreateMeshFVF( 12, 24, D3DXMESH_MANAGED, D3DFVF_SKYBOXVERTEX, g_D3dDevice, &g_SkyBoxMesh ) ) )
	{
		MessageBox( NULL, L"Could not Create Sky Box Mesh", L"YaMang.DLL", MB_OK );
		return hr;
	}

	SKYBOXVERTEX* vertexBuffer = nullptr;
	if ( FAILED( hr = g_SkyBoxMesh->LockVertexBuffer( 0, (void**)&vertexBuffer ) ) )
	{
		MessageBox( NULL, L"Could not Lock Sky Box Mesh Vertex Buffer", L"YaMang.DLL", MB_OK );
		g_SkyBoxMesh->Release();
		return hr;
	}

	// 전면   
	vertexBuffer[0] = SKYBOXVERTEX( -1.0f, -1.0f, -1.0f, 0.0f, 1.0f );
	vertexBuffer[1] = SKYBOXVERTEX( -1.0f, 1.0f, -1.0f, 0.0f, 0.0f );
	vertexBuffer[2] = SKYBOXVERTEX( 1.0f, 1.0f, -1.0f, 1.0f, 0.0f );
	vertexBuffer[3] = SKYBOXVERTEX( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f );
	// 후면
	vertexBuffer[4] = SKYBOXVERTEX( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	vertexBuffer[5] = SKYBOXVERTEX( -1.0f, 1.0f, 1.0f, 1.0f, 0.0f );
	vertexBuffer[6] = SKYBOXVERTEX( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f );
	vertexBuffer[7] = SKYBOXVERTEX( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f );
	// 좌측
	vertexBuffer[8] = SKYBOXVERTEX( -1.0f, -1.0f, 1.0f, 0.0f, 1.0f );
	vertexBuffer[9] = SKYBOXVERTEX( -1.0f, 1.0f, 1.0f, 0.0f, 0.0f );
	vertexBuffer[10] = SKYBOXVERTEX( -1.0f, 1.0f, -1.0f, 1.0f, 0.0f );
	vertexBuffer[11] = SKYBOXVERTEX( -1.0f, -1.0f, -1.0f, 1.0f, 1.0f );
	// 우측
	vertexBuffer[12] = SKYBOXVERTEX( 1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	vertexBuffer[13] = SKYBOXVERTEX( 1.0f, 1.0f, 1.0f, 1.0f, 0.0f );
	vertexBuffer[14] = SKYBOXVERTEX( 1.0f, 1.0f, -1.0f, 0.0f, 0.0f );
	vertexBuffer[15] = SKYBOXVERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f );
	// 상단
	vertexBuffer[16] = SKYBOXVERTEX( -1.0f, 1.0f, -1.0f, 0.0f, 1.0f );
	vertexBuffer[17] = SKYBOXVERTEX( -1.0f, 1.0f, 1.0f, 0.0f, 0.0f );
	vertexBuffer[18] = SKYBOXVERTEX( 1.0f, 1.0f, 1.0f, 1.0f, 0.0f );
	vertexBuffer[19] = SKYBOXVERTEX( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f );
	// 하단
	vertexBuffer[20] = SKYBOXVERTEX( -1.0f, -1.0f, -1.0f, 1.0f, 0.0f );
	vertexBuffer[21] = SKYBOXVERTEX( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	vertexBuffer[22] = SKYBOXVERTEX( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f );
	vertexBuffer[23] = SKYBOXVERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f );

	for ( UINT i = 0; i < 24; ++i )
	{
		vertexBuffer[i].m_X *= size;
		vertexBuffer[i].m_Y *= size;
		vertexBuffer[i].m_Y += size / 10;
		vertexBuffer[i].m_Z *= size;
	}

	g_SkyBoxMesh->UnlockVertexBuffer();

	WORD* indexBuffer = 0;
	if ( FAILED( hr = g_SkyBoxMesh->LockIndexBuffer( 0, (void**)&indexBuffer ) ) )
	{
		MessageBox( NULL, L"Could not Lock Sky Box Mesh Index Buffer", L"YaMang.DLL", MB_OK );
		g_SkyBoxMesh->Release();
		return hr;
	}
	indexBuffer[0] = 0;		indexBuffer[1] = 1;		indexBuffer[2] = 2;
	indexBuffer[3] = 0;		indexBuffer[4] = 2;		indexBuffer[5] = 3;

	indexBuffer[6] = 4;		indexBuffer[7] = 5;		indexBuffer[8] = 6;
	indexBuffer[9] = 4;		indexBuffer[10] = 6;	indexBuffer[11] = 7;

	indexBuffer[12] = 8;	indexBuffer[13] = 9;	indexBuffer[14] = 10;
	indexBuffer[15] = 8;	indexBuffer[16] = 10;	indexBuffer[17] = 11;

	indexBuffer[18] = 12;	indexBuffer[19] = 13;	indexBuffer[20] = 14;
	indexBuffer[21] = 12;	indexBuffer[22] = 14;	indexBuffer[23] = 15;

	indexBuffer[24] = 16;	indexBuffer[25] = 17;	indexBuffer[26] = 18;
	indexBuffer[27] = 16;	indexBuffer[28] = 18;	indexBuffer[29] = 19;

	indexBuffer[30] = 20;	indexBuffer[31] = 21;	indexBuffer[32] = 22;
	indexBuffer[33] = 20;	indexBuffer[34] = 22;	indexBuffer[35] = 23;

	g_SkyBoxMesh->UnlockIndexBuffer();

	DWORD* attributeBuffer = 0;
	if ( FAILED( hr = g_SkyBoxMesh->LockAttributeBuffer( 0, &attributeBuffer ) ) )
	{
		MessageBox( NULL, L"Could not Lock Sky Box Mesh Attribute Buffer", L"YaMang.DLL", MB_OK );
		g_SkyBoxMesh->Release();
		return hr;
	}

	// triangles 1 - 4
	for ( int a = 0; a < 2; a++ )
	{
		// subset 0
		attributeBuffer[a] = 0;
	}
	// triangles 5-8
	for ( int b = 2; b < 4; b++ )
	{
		// subset 1   
		attributeBuffer[b] = 1;
	}
	// triangles 9-12   
	for ( int c = 4; c < 6; c++ )
	{
		// subset 2   
		attributeBuffer[c] = 2;
	}
	// triangles 1-4   
	for ( int d = 6; d < 8; d++ )
	{
		// subset 0  
		attributeBuffer[d] = 3;
	}
	// triangles 5-8   
	for ( int e = 8; e < 10; e++ )
	{
		// subset 1
		attributeBuffer[e] = 4;
	}
	// triangles 9-12   
	for ( int f = 10; f < 12; f++ )
	{
		// subset 2   
		attributeBuffer[f] = 5;
	}
	g_SkyBoxMesh->UnlockAttributeBuffer();

	return S_OK;
}