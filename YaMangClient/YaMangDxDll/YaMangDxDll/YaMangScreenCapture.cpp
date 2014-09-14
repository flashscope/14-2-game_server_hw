#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// screenShot
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void TakeScreenShot()
{

	IDirect3DSurface9 *surface;

	// GetBackBuffer
	g_D3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &surface );

	time_t timer;
	struct tm t;

	timer = time( NULL ); // 현재 시각을 초 단위로 얻기
	localtime_s( &t, &timer ); // 초 단위의 시간을 분리하여 구조체에 넣기

	std::string directoryName = "ScreenShot";

	struct stat st;
	if ( stat( directoryName.c_str(), &st ) != 0 )
	{
		if ( _mkdir( directoryName.c_str() ) != 0 )
		{
			Log( "MKDIR FAILED! \n" );
			return;
		}
	}
	std::string fileName = "ScreenShot/YamangScreen-" + std::to_string( t.tm_year + 1900 ) + "-" + std::to_string( t.tm_mon + 1 ) + "-" + std::to_string( t.tm_mday ) + "_" + std::to_string( t.tm_hour ) + "-" + std::to_string( t.tm_min ) + "-" + std::to_string( t.tm_sec ) + ".png";

	// save the surface
	D3DXSaveSurfaceToFileA( fileName.c_str(), D3DXIFF_PNG, surface, NULL, NULL );

	if ( surface != nullptr )
	{
		surface->Release();
		surface = nullptr;
	}

}