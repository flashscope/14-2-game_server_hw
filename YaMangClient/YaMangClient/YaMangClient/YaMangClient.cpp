// YaMangClient.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "YaMangClient.h"

#ifdef DEBUG
#include <crtdbg.h>
#endif // DEBUG

#include "Logger.h"
#include "MainWindow.h"
#include "NetworkManager.h"
#include "Exception.h"

INT_PTR g_LogHandle = -1;

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hInstance );
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// 메모리 릭을 체크하려면 아래의 #### 칸에 릭 난 곳 { 숫자 } 표기 된 숫자를 넣어주면 됩니다.
	// _CrtSetBreakAlloc( #### );
	// _CrtSetBreakAlloc( 192 );

	// SetUnhandledExceptionFilter( ExceptionFilter );

	InstallCrashReporter();

	// set_terminate() must be called from every thread
	BT_SetTerminate();
	BT_InsLogEntry( g_LogHandle, BTLL_INFO, _T( "Entering main() function" ) );

#ifdef _PRINT_CONSOLE
	Logger::GetInstance()->CreateConsole();
#endif


	// test code
	std::wstring parameter = lpCmdLine;
	printf_s( "parameter:%S \n", parameter.c_str() );

	srand( static_cast<unsigned int>( time( NULL ) ) );
	unsigned int pos = parameter.find( ' ' );
	if ( -1 == pos )
	{
		NetworkManager::GetInstance()->SetRoomNumber( 1 );
		NetworkManager::GetInstance()->SetMyPlayerID( 1000 + rand() % 101 );
	}
	else
	{
		if ( -1 == parameter.find( ' ', pos + 1 ) )
		{
			std::wstring roomNumber = parameter.substr( 0, pos );
			std::wstring playerID = parameter.substr( pos+1 );
			printf_s( "ROOM:%S PLAYER:%S \n", roomNumber.c_str( ), playerID.c_str( ) );

			NetworkManager::GetInstance( )->SetRoomNumber( std::stoi( roomNumber ) );
			NetworkManager::GetInstance( )->SetMyPlayerID( std::stoi( playerID ) );
		}
		else
		{
			NetworkManager::GetInstance()->SetRoomNumber( 1 );
			NetworkManager::GetInstance()->SetMyPlayerID( 1000 + rand() % 101 );
		}
	}

	// WS_POPUPWINDOW
	// WS_OVERLAPPEDWINDOW
	if ( false == MainWindow::GetInstance()->Create( L"YaMang", WS_POPUPWINDOW ) )
	{
		return 0;
	}

	MainWindow::GetInstance()->Display( nCmdShow );

#ifndef DEBUG
	HANDLE mutex = CreateMutex( NULL, FALSE, L"YaMangClientMutex" );
	if ( mutex == NULL )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"Create Mutex Error", L"Mutex", MB_OK | MB_ICONERROR );
		return -1;
	}

	DWORD waitResult = WaitForSingleObject( mutex, 1 );
	switch ( waitResult )
	{
		case WAIT_TIMEOUT:
		{	
							 MessageBox( MainWindow::GetInstance()->Window(), L"Client Already Running", L"Mutex", MB_OK | MB_ICONERROR );
							 MainWindow::Release();
							 return -1;
		}
			break;
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
		default:
			break;
	}
#endif

	int result = MainWindow::GetInstance()->RunGame();
	MainWindow::Release();

#ifdef _PRINT_CONSOLE
	Logger::GetInstance()->DestroyConsole();
	Logger::Release();
#endif

	BT_InsLogEntry( g_LogHandle, BTLL_INFO, _T( "Leaving main() function" ) );
	BT_CloseLogFile( g_LogHandle );

	return result;
}