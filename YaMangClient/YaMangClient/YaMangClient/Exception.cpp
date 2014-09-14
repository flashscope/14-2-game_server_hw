#include "stdafx.h"
#include <DbgHelp.h>
#include "Exception.h"
#include "MacroSet.h"
#include "Logger.h"

LONG WINAPI ExceptionFilter( EXCEPTION_POINTERS* exceptionInfo )
{
	if ( IsDebuggerPresent() )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	char fileName[] = "YaMangClient_minidump-" __DATE__ "-" __TIME__".dmp";
	int offSet = 0;
	while ( fileName[offSet] != NULL )
	{
		if ( fileName[offSet] == ' ' || fileName[offSet] == ':' )
		{
			fileName[offSet] = '_';
		}
		++offSet;
	}
	
	USES_CONVERSION;
	std::wstring outputFileName;
	outputFileName += A2W( fileName );
	
	HANDLE hFile = CreateFile( outputFileName.c_str(), GENERIC_READ | GENERIC_WRITE,
								0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) )
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = exceptionInfo;
		mdei.ClientPointers = FALSE;

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)( MiniDumpWithPrivateReadWriteMemory |
											 MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo |
											 MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules );

		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
						   hFile, mdt, ( exceptionInfo != 0 ) ? &mdei : 0, 0, NULL );

		CloseHandle( hFile );
	}
	else
	{
		Log( "CreateFile failed. Error: %u \n", GetLastError() );
	}
	// 예외처리 종료 신호 반환
	return EXCEPTION_EXECUTE_HANDLER;
}
