// yaMangDxDll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
//input args: 윈도우 핸들
//향후 공용으로 사용할 D3D, D3DDevice에 대해 초기화 진행
//프로그램에서 1회만 실행 후 Process 종료까지 사용
//참고 URL : http://www.delmadang.com/community/bbs_view.asp?bbsNo=17&indx=426040 
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd, long width, long height )
{
	if ( nullptr == ( g_D3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		// 오류
		MessageBox( NULL, L"Could not Create D3D", L"YaMang.DLL", MB_OK );
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;
	DWORD dwVSProcess;
	g_D3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

	// 지원하는 정점쉐이더 버전이 1.0이하라면 SW쉐이더를, 1.0이상이면 HW쉐이더를 생성한다.
	dwVSProcess = 
		( caps.VertexShaderVersion < D3DVS_VERSION( 1, 0 ) ) ? D3DCREATE_SOFTWARE_VERTEXPROCESSING : D3DCREATE_HARDWARE_VERTEXPROCESSING;

	if ( FAILED( g_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hWnd, dwVSProcess, &d3dpp, &g_D3dDevice ) ) )
	{
		MessageBox( NULL, L"Could not CreateDevice", L"YaMang.DLL", MB_OK );
		return E_FAIL;
	}

	g_D3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_D3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	
	g_StartWidth = static_cast<float>( width );
	g_StartHeight = static_cast<float>( height );

	SetAspectRatio( width, height );

	//텍스트 출력 자원 초기화
	D3DXCreateFont(
		g_D3dDevice						// Device 객체 
		, 30					// 폰트 높이 
		, 0					// 폰트 길이 
		, FW_NORMAL						// 폰트 굵기 (기본 FW_BOLD) 
		, 1								// 밉레벨 
		, false							// 폰트 기울기 켜기/끄기 설정 
		, DEFAULT_CHARSET				// 문자셋 
		, OUT_DEFAULT_PRECIS			// 출력 정확도 설정 
		, DEFAULT_QUALITY				// 퀄리티 설정 
		, DEFAULT_PITCH | FF_DONTCARE	// 피치 설정 
		, L"맑은 고딕"						// 글꼴 설정 
		, &g_Font						// 초기화할 Font 
		);								// 폰트 초기화

	D3DXCreateSprite( g_D3dDevice, &g_Sprite ); // 스프라이트 초기화 
#ifdef _PRINT_CONSOLE
	Logger::GetInstance();
#endif

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//Text Render
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderText( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom )
{
	g_Sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	// 스프라이트 그리기 시작
	// 2D일 경우 D3DXSPRITE_SORT_TEXTURE, 3D일 경우 D3DXSPRITE_OBJECTSPACE
	RECT rt = {
		static_cast<LONG>( left ),
		static_cast<LONG>( top ),
		static_cast<LONG>( right ),
		static_cast<LONG>( bottom )
	}; // 그릴 위치
	
	// 비율 고정
	D3DXMATRIXA16 ratioMat;
	D3DXMatrixIdentity( &ratioMat );
	D3DXMatrixScaling( &ratioMat, g_StartWidth / g_NowWidth, g_StartHeight / g_NowHeight, 1 );
	g_Sprite->SetTransform( &ratioMat );

	g_Font->DrawText( g_Sprite, text
					  , -1										// 총 문자열 수(그냥 -1 해도 됨.) 
					  , &rt									// 그릴 위치 
					  , DT_NOCLIP								// 옵션 플래그
					  , D3DCOLOR_XRGB( RGB_R, RGB_G, RGB_B )	// 그릴 색 , default로 흰색
					  );
	g_Sprite->End();											// 스프라이트 그리기 끝 
}


//매트릭스 출력용
//Log("%f, %f, %f, %f \n", matrix->_11, matrix->_12, matrix->_13, matrix->_14);
//Log("%f, %f, %f, %f \n", matrix->_21, matrix->_22, matrix->_23, matrix->_24);
//Log("%f, %f, %f, %f \n", matrix->_31, matrix->_32, matrix->_33, matrix->_34);
//Log("%f, %f, %f, %f \n", matrix->_41, matrix->_42, matrix->_43, matrix->_44);
//Log("==============================");

YAMANGDXDLL_API void SetCameraMatrix( D3DXMATRIXA16* matrix )
{
	g_D3dDevice->SetTransform( D3DTS_VIEW, matrix );
}

YAMANGDXDLL_API void SetWorldAreaMatrix( D3DXMATRIXA16* matrix )
{
	g_D3dDevice->SetTransform( D3DTS_WORLD, matrix );
}


YAMANGDXDLL_API void SetAspectRatio( long width, long height )
{
	D3DXMATRIXA16 matProj;
	float aspectRatio = static_cast<float>( width ) / static_cast<float>( height );
	
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 5, aspectRatio, 1.0f, 2000.0f );
	g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_NowWidth = static_cast<float>( width );
	g_NowHeight = static_cast<float>( height );
}



//////////////////////////////////////////////////////////////////////////
///Camera 함수
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void SetCameraView(float x /* = 0 */, float y /* = 0 */, float z/* = 0 */)
{
	g_EyePoint = { x, y, z };
	g_LookAtPoint.x = x;
	g_LookAtPoint.y = y;
	g_LookAtPoint.z = z + 1;
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixLookAtLH(&mat, &g_EyePoint, &g_LookAtPoint, &g_UpVector);
	SetCameraMatrix(&mat);
}

YAMANGDXDLL_API void MoveCamera( float x, float y, float z )
{
	D3DXVECTOR3 view = g_LookAtPoint - g_EyePoint;
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross, &g_UpVector, &view );
	D3DXVec3Normalize( &view, &view );
	view.y = 0;
	g_EyePoint = g_EyePoint + cross * x + view * z;
	g_LookAtPoint = g_LookAtPoint + cross * x + view * z;

	g_LookAtPoint.y += y;
	g_EyePoint.y += y;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector );
	SetCameraMatrix( &viewMatrix );
}

void Lighting( int lightNum )
{
	//재질 속성 부여
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_D3dDevice->SetMaterial( &mtrl );

	D3DXVECTOR3 vecDir0;
	D3DLIGHT9 light0;
	ZeroMemory( &light0, sizeof( D3DLIGHT9 ) );
	light0.Type = D3DLIGHT_DIRECTIONAL;
	light0.Diffuse.r = 1.f;
	light0.Diffuse.g = 1.f;
	light0.Diffuse.b = 1.f;
	vecDir0 = D3DXVECTOR3( -1.f, 1.f, 0.f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&light0.Direction, &vecDir0 );
	light0.Range = 1000.f;


	D3DXVECTOR3 vecDir1;
	D3DLIGHT9 light1;
	ZeroMemory( &light1, sizeof( D3DLIGHT9 ) );
	light1.Type = D3DLIGHT_DIRECTIONAL;
	light1.Diffuse.r = 1.f;
	light1.Diffuse.g = 1.f;
	light1.Diffuse.b = 1.f;
	vecDir1 = D3DXVECTOR3( 1.f, 1.f, 0.f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&light1.Direction, &vecDir1 );
	light1.Range = 1000.f;

	g_D3dDevice->SetLight( 0, &light0 );
	g_D3dDevice->SetLight( 1, &light1 );

	switch ( lightNum )
	{
		case 1:
			g_D3dDevice->LightEnable( 0, TRUE );
			g_D3dDevice->LightEnable( 1, FALSE );
			break;
		case 2:
			g_D3dDevice->LightEnable( 0, FALSE );
			g_D3dDevice->LightEnable( 1, TRUE );
			break;
		default:
			break;
	}

	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_D3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080 );
}

// 내보낸 변수의 예제입니다.
// YAMANGDXDLL_API int nyaMangDxDll=0;
