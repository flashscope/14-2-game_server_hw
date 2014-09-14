#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"
#include <timeapi.h>

YAMANGDXDLL_API HRESULT ShaderCreate( int size )
{
	if ( size <= 0 || g_EffectSize != 0 )
	{
		return E_FAIL;
	}

	g_Effects = new LPD3DXEFFECT[size];
	g_EffectSize = size;

	for ( int i = 0; i < g_EffectSize; ++i )
	{
		g_Effects[i] = nullptr;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT ShaderImport( LPCTSTR effectFile, int id )
{
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG | D3DXSHADER_NO_PRESHADER;

	HRESULT hr = S_OK;
	hr = D3DXCreateEffectFromFile( g_D3dDevice, effectFile, NULL, NULL, dwShaderFlags, NULL, &g_Effects[id], NULL );

	if ( FAILED( hr ) )
	{
		MessageBox( NULL, L"Effect Load Failed", effectFile, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

enum UnitStatusFlagType
{
	UNIT_STATUS_FLAG_ENEMY = 0x1,
	UNIT_STATUS_FLAG_ATTACK = 0x2,
	UNIT_STATUS_FLAG_SELECT = 0x4
};

YAMANGDXDLL_API void DrawBillboardByTexture( int id, char flag )
{
	if ( id <= 0 || id >= g_MeshTextureSize )
	{
		return;
	}

	// 빌보드 정점
	BILLBOARDVERTEX vtx[4] =
	{
		{ -1, 0, 0, 0, 1 },
		{ -1, 4, 0, 0, 0 },
		{ 1, 0, 0, 1, 1 },
		{ 1, 4, 0, 1, 0 }
	};

	if ( g_Effects[1] )
	{
		D3DVERTEXELEMENT9	ele[MAX_FVF_DECL_SIZE];

		// FVF를 사용해서 정점선언값을 자동으로 채워넣는다
		D3DXDeclaratorFromFVF( BILLBOARDVERTEX::FVF, ele );
		LPDIRECT3DVERTEXDECLARATION9	decl;

		// 정점선언값으로 decl을 생성한다.
		g_D3dDevice->CreateVertexDeclaration( ele, &decl );
		g_D3dDevice->SetVertexDeclaration( decl );

		UINT nPass;

		float thisTime = D3DX_PI * ( timeGetTime() % 600 ) / 300;
		g_Effects[1]->SetFloat( "time", thisTime );

		float	inputColor[3] = { 0.0f };

		if ( flag & UNIT_STATUS_FLAG_ATTACK )
		{
			g_Effects[1]->SetBool( "isAttack", true );
		}
		else
		{
			g_Effects[1]->SetBool( "isAttack", false );
		}

		if ( flag & UNIT_STATUS_FLAG_ENEMY )
		{
			inputColor[0] = 0.3f;
		}
		else
		{
			inputColor[2] = 0.3f;
		}
		
		if ( flag & UNIT_STATUS_FLAG_SELECT )
		{
			inputColor[0] += 0.3f;
			inputColor[1] += 0.3f;
			inputColor[2] += 0.3f;
		}
		
		g_Effects[1]->SetFloatArray( "inputColor", inputColor, 3 );

		// fx출력에 사용할 테크닉 선정
		g_Effects[1]->SetTechnique( "MyShader" );

		D3DXMATRIXA16 worldMatrix;
		g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
		D3DXMATRIXA16 viewingMatrix;
		g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
		D3DXMATRIXA16 projectionMatrix;
		g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

		D3DXMATRIXA16 thisMatrix = worldMatrix * viewingMatrix * projectionMatrix;

		g_Effects[1]->SetMatrix( "matWVP", &thisMatrix );

		g_Effects[1]->SetTexture( "tex0", g_MeshTextures[id] );
		
		// fx를 사용한 출력개시
		g_Effects[1]->Begin( &nPass, D3DXFX_DONOTSAVESHADERSTATE );

		// PASS 개수만큼 출력
		for ( UINT i = 0; i < nPass; ++i )
		{
			g_Effects[1]->BeginPass( i );
			
			g_D3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( BILLBOARDVERTEX ) );
			
			g_Effects[1]->EndPass();
		}

		g_Effects[1]->End();
	}
	else
	{
		g_D3dDevice->SetVertexShader( NULL );
		g_D3dDevice->SetPixelShader( NULL );
				
		// 0번 텍스처에 빌보드 텍스처를 올린다
		g_D3dDevice->SetTexture( 0, g_MeshTextures[id] );
		g_D3dDevice->SetFVF( BILLBOARDVERTEX::FVF );
		g_D3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( BILLBOARDVERTEX ) );
	}

	g_IsEffectReady = false;
}

YAMANGDXDLL_API HRESULT MeshTextureCreateBySize( INT size )
{
	if ( size <= 0 || g_MeshTextureSize != 0)
	{
		return E_FAIL;
	}

	g_MeshTextures = new LPDIRECT3DTEXTURE9[size];
	g_MeshTextureSize = size;

	for ( int i = 0; i < g_MeshTextureSize; ++i )
	{
		g_MeshTextures[i] = nullptr;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT MeshTextureImport( LPCTSTR meshTexture, int id )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, meshTexture, &g_MeshTextures[id] ) ) )
	{
		MessageBox( NULL, L"Mesh Texture Load Failed", meshTexture, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}