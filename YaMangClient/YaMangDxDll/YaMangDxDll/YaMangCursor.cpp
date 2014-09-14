#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// 커서 입력에 대한 처리
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API HRESULT InitCursor( int textureSize, int cursorPosX, int cursorPosY )
{
	if ( FAILED( D3DXCreateSprite( g_D3dDevice, &g_CursorSprite ) ) ) //first parameter is our device, second is a empty sprite variable
	{
		return E_FAIL;
	}
	if ( g_CursorTex )
	{
		for ( int i = 0; i < g_CursorMaxSize; ++i )
		{
			if ( g_CursorTex[i] )
			{
				g_CursorTex[i]->Release();
			}
		}
		delete[] g_CursorTex;
		g_CursorTex = nullptr;
	}

	g_CursorTex = new LPDIRECT3DTEXTURE9[textureSize];
	g_CursorMaxSize = textureSize;

	for ( int i = 0; i < g_CursorMaxSize; ++i )
	{
		g_CursorTex[i] = nullptr;
	}

	SetCursorPosition( cursorPosX, cursorPosY ); // 0,0,0으로 초기화

	return S_OK;
}


YAMANGDXDLL_API HRESULT CreateCursorImage( LPCWSTR cursorImagePath /*= L"cursor_default.png"*/, int cursorType /*= 0 */ )
{
	if ( g_D3dDevice )
	{
		if ( g_CursorTex[cursorType] )
		{
			g_CursorTex[cursorType]->Release();
			g_CursorTex[cursorType] = nullptr;
		}
		if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, cursorImagePath, &g_CursorTex[cursorType] ) ) )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	return E_FAIL;
}

YAMANGDXDLL_API HRESULT ChangeCursorImage( int cursorType )
{
	if ( g_CursorTex && ( cursorType < g_CursorMaxSize ) && g_CursorTex[cursorType] )
	{
		g_CursorType = cursorType;

		return S_OK;
	}

	return E_FAIL;
}


YAMANGDXDLL_API void SetCursorPosition( int PosX, int PosY )
{
	//Log("Render Cursor Pos %d %d \n",PosX,PosY);
	g_CursorPos.x = static_cast<float>( PosX );
	g_CursorPos.y = static_cast<float>( PosY );
	g_CursorPos.z = 0.0f;
}