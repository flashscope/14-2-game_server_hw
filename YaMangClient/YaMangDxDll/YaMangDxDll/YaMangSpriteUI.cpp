#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// UI
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitUISprite( int textureSize )
{
	if ( FAILED( D3DXCreateSprite( g_D3dDevice, &g_UISprite ) ) ) //first parameter is our device, second is a empty sprite variable
	{
		return E_FAIL;
	}
	if ( g_UITexures )
	{
		for ( int i = 0; i < g_UIMaxSize; ++i )
		{
			if ( g_UITexures[i] )
			{
				g_UITexures[i]->Release();
			}
		}
		delete[] g_UITexures;
		g_UITexures = nullptr;
	}

	g_UITexures = new LPDIRECT3DTEXTURE9[textureSize];
	g_UIMaxSize = textureSize;

	for ( int i = 0; i < g_UIMaxSize; ++i )
	{
		g_UITexures[i] = nullptr;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT CreateSpriteImage( LPCWSTR ImagePath /*= L"UI_default.png"*/, INT typeNum /*= 0*/ )
{
	if ( g_D3dDevice )
	{
		if ( g_UITexures[typeNum] )
		{
			g_UITexures[typeNum]->Release();
			g_UITexures[typeNum] = nullptr;
		}
		if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, ImagePath, &g_UITexures[typeNum] ) ) )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	return E_FAIL;
}

YAMANGDXDLL_API HRESULT RenderSprite( INT typeNum, D3DXVECTOR3 pos )
{
	if ( g_UISprite )
	{
		// 비율 고정
		D3DXMATRIXA16 ratioMat;
		D3DXMatrixIdentity( &ratioMat );
		D3DXMatrixScaling( &ratioMat, g_StartWidth / g_NowWidth, g_StartHeight / g_NowHeight, 1 );
		g_UISprite->SetTransform( &ratioMat );

		g_UISprite->Begin( D3DXSPRITE_ALPHABLEND );
		g_UISprite->Draw( g_UITexures[typeNum], NULL, NULL, &pos, 0xFFFFFFFF );
		g_UISprite->End();

		g_D3dDevice->EndScene();
		return S_OK;
	}
	return E_FAIL;
}