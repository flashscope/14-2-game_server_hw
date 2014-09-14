#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"
#include <timeapi.h>

enum RenderingOption
{
	SETTING_NONE,

	LIGHT_SETTING_ON,
	LIGHT_SETTING_OFF,
};

//////////////////////////////////////////////////////////////////////////
//render를 pre - main - post renderring으로 변경
//pre에서는 matrix, view, light에 대한 setting 진행
//main에서는 mesh object에 대한 직접 rendering 진행
//post에서는 pre에서 설정한 setting 초기화
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API bool PreRendering()
{
	if ( NULL == g_D3dDevice )
	{
		return false;
	}

	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 10, 10, 10 ), 1.0f, 0 );

	bool flag = false;

	// 렌더 방어코드
	// pre rendering 단계에서 진행되지 않으면 향후 render 모두 실패
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		D3DXMATRIXA16 identityMatrix;
		D3DXMatrixIdentity( &identityMatrix );
		SetWorldAreaMatrix( &identityMatrix );

		// lightsetting
		int lightNum = 1;
		Lighting( lightNum );

		flag = true;

		g_D3dDevice->SetVertexShader( NULL );
		g_D3dDevice->SetPixelShader( NULL );
		g_IsEffectReady = false;
	}

	return flag;
}

YAMANGDXDLL_API void SetEffect( bool isEffect )
{
	g_IsEffectReady = isEffect;
}

YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal )
{
	if ( g_IsEffectReady && g_Effects[0] )
	{
		D3DVERTEXELEMENT9	ele[MAX_FVF_DECL_SIZE];

		// FVF를 사용해서 정점선언값을 자동으로 채워넣는다
		D3DXDeclaratorFromFVF( D3DFVF_CUSTOMVERTEX, ele );
		LPDIRECT3DVERTEXDECLARATION9	decl;

		// 정점선언값으로 decl을 생성한다.
		g_D3dDevice->CreateVertexDeclaration( ele, &decl );
		g_D3dDevice->SetVertexDeclaration( decl );

		UINT nPass;
		float thisTime = D3DX_PI * (timeGetTime() % 1400) / 700;

		g_Effects[0]->SetFloat( "time", thisTime );

		// fx출력에 사용할 테크닉 선정
		g_Effects[0]->SetTechnique( "MyShader" );
				
		D3DXMATRIXA16 worldMatrix;
		g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
		D3DXMATRIXA16 viewingMatrix;
		g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
		D3DXMATRIXA16 projectionMatrix;
		g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

		D3DXMATRIXA16 thisMatrix = worldMatrix * viewingMatrix * projectionMatrix;

		g_Effects[0]->SetMatrix( "matWVP", &thisMatrix );

		g_Effects[0]->SetTexture( "tex0", inputVal->MeshTexture[0] );

		// fx를 사용한 출력개시
		g_Effects[0]->Begin( &nPass, D3DXFX_DONOTSAVESHADERSTATE );
		
		// PASS 개수만큼 출력
		for ( UINT i = 0; i < nPass; ++i )
		{
			g_Effects[0]->BeginPass( i );
			
			for ( DWORD j = 0; j < inputVal->NumMaterials; ++j )
			{
				( inputVal->importedMesh )->DrawSubset( j );
			}

			g_Effects[0]->EndPass();
		}

		/// fx를 사용한 출력종료
		g_Effects[0]->End();
	}
	else
	{
		g_D3dDevice->SetVertexShader( NULL );
		g_D3dDevice->SetPixelShader( NULL );

		g_D3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
		{
			g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
			g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

			( inputVal->importedMesh )->DrawSubset( i );
		}
	}

	g_IsEffectReady = false;
}


YAMANGDXDLL_API void PostRendering()
{
	D3DXMATRIXA16 identityMatrix;
	D3DXMatrixIdentity( &identityMatrix );
	SetWorldAreaMatrix( &identityMatrix );

	g_D3dDevice->EndScene();

	//Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}

//////////////////////////////////////////////////////////////////////////
//heightMap을 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderHeightMap()
{
	//현재 조명이 적용되지 않았음
	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	IDirect3DVertexBuffer9* RenderVertexBuffer = nullptr;
	g_Mesh->GetVertexBuffer( &RenderVertexBuffer );

	IDirect3DIndexBuffer9* RenderIndexBuffer = nullptr;
	g_Mesh->GetIndexBuffer( &RenderIndexBuffer );

	g_D3dDevice->SetStreamSource( 0, RenderVertexBuffer, 0, sizeof( CUSTOMVERTEX ) );
	g_D3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_D3dDevice->SetTexture( 0, g_MapTexture );
	//g_D3dDevice->SetTexture( 1, g_MapTextureArray[1] );

	//설정된 uv 좌표값에 따라 연산
	g_D3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );

	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	
	g_D3dDevice->SetIndices( RenderIndexBuffer );
	g_D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, g_XHeight * g_ZHeight, 0, ( g_XHeight - 1 ) * ( g_ZHeight - 1 ) * 2 + ( g_ZHeight - 2 ) * 3 );
}


//////////////////////////////////////////////////////////////////////////
//Cursor를 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT RenderCursor()
{
	if ( g_CursorSprite && g_CursorTex && g_CursorTex[g_CursorType] )
	{
		// 비율 고정
		D3DXMATRIXA16 ratioMat;
		D3DXMatrixIdentity( &ratioMat );
		D3DXMatrixScaling( &ratioMat, g_StartWidth / g_NowWidth, g_StartHeight / g_NowHeight, 1 );
		g_CursorSprite->SetTransform( &ratioMat );

		g_CursorSprite->Begin( D3DXSPRITE_ALPHABLEND );
		g_CursorSprite->Draw( g_CursorTex[g_CursorType], NULL, NULL, &g_CursorPos, 0xFFFFFFFF );
		g_CursorSprite->End();
		g_D3dDevice->EndScene();
		return S_OK;
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
//SkyBox를 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderSkyBox( D3DXVECTOR3 eyePoint )
{
	if ( g_SkyBoxMesh )
	{
		D3DXMATRIXA16 moveMatrix;
		D3DXMatrixTranslation( &moveMatrix, eyePoint.x, eyePoint.y, eyePoint.z );
		g_D3dDevice->SetTransform( D3DTS_WORLD, &moveMatrix );

		g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		
		g_D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD );
		g_D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD );

		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

		for ( int i = 0; i < 6; i++ )
		{
			g_D3dDevice->SetTexture( 0, g_SkyBoxTextures[i] );
			g_SkyBoxMesh->DrawSubset( i );
		}

		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	}
}


//////////////////////////////////////////////////////////////////////////
//캐릭터 툴을 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderingTool( MESHOBJECT* inputVal )
{
	if ( NULL == g_D3dDevice )
	{
		return;
	}

	if ( g_Mesh == nullptr )
	{
		InitGroundMesh( 100, 100 );
	}
	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 30, 10, 10 ), 1.0f, 0 );

	// 렌더 방어코드
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		// SetupTranslateMatricesTool();
		// 일단 height map 등 다른 쪽을 생각할 것

		// ViewSetting();

		// lightsetting
		// 일단 1로 진행, 향후 라이트 개수 등 확정되면 인자 받아 설정
		int lightNum = 1;
		Lighting( lightNum );
		// Log( "라이팅 세팅!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Log( "Render Begin \n" );
		// Log( "pre render 완료!\n" );
	}

	// 카메라 셋팅
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector );
	SetCameraMatrix( &viewMatrix );

	// 보여주기 위한 땅을 만듬
	//InitGroundMesh(100, 100);
	CreateRawGround( 100, 100, 10 );
	RenderHeightMap();

	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	// Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
	g_D3dDevice->EndScene();

	// Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}