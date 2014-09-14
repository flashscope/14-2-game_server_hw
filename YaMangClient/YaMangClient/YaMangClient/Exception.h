#pragma once
#include "MacroSet.h"
#include "LibraryBugTrap/BugTrap.h"

LONG WINAPI ExceptionFilter( EXCEPTION_POINTERS* exceptionInfo );

extern INT_PTR g_LogHandle;

static void InstallCrashReporter()
{
	BT_SetAppName( _T( "YaMangClient Crash Report" ) );
	BT_SetFlags( BTF_DETAILEDMODE | BTF_SCREENCAPTURE );

	BT_SetSupportServer( _T( "10.73.45.142" ), 9999 );
	
	BT_InstallSehFilter( );

	g_LogHandle = BT_OpenLogFile( NULL, BTLF_TEXT );
	BT_SetLogSizeInEntries( g_LogHandle, 100 );
	BT_SetLogFlags( g_LogHandle, BTLF_SHOWTIMESTAMP );
	BT_SetLogEchoMode( g_LogHandle, BTLE_STDERR | BTLE_DBGOUT );

	WCHAR buildTime[] = L"Build Time : " _T( __DATE__ ) L"-" _T( __TIME__ );
	BT_InsLogEntry( g_LogHandle, BTLL_INFO, buildTime );

	PCTSTR logFileName = BT_GetLogFileName( g_LogHandle );
	BT_AddLogFile( logFileName );
}

inline void CRASH_ASSERT( bool isOk )
{
	if ( isOk )
	{
		return;
	}

	int* crashVal = 0;
	*crashVal = 0xDEADBEEF;
}