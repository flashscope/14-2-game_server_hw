#pragma once

#include "MacroSet.h"
#ifndef _PRINT_CONSOLE
#define _PRINT_CONSOLE
#endif

enum LogStatusType
{
	LOG_NONE,

	LOG_CONSOLE,
	LOG_FILE,

	LOG_MAX
};

template <typename ... Types >
inline void Log( const Types& ...args )
{
#ifdef _DEBUG
	if ( LOG_CONSOLE == Logger::GetInstance()->GetLogStatus() )
	{
		printf_s( args... );
	}
	else if ( LOG_FILE == Logger::GetInstance()->GetLogStatus() )
	{
		FILE* fp = nullptr;

		const char fileName[] = "LogFile_" __DATE__ ".txt";
		
		fopen_s( &fp, fileName, "a+" );

		if ( fp )
		{
			char temp[1024] = { 0, };
			sprintf_s( temp, args... );
			fprintf_s( fp, __DATE__ __TIME__ " %s", temp );
			fclose( fp );
		}
	}
#endif // _DEBUG
}

class Logger: public Singleton<Logger>
{
public:
	Logger();
	~Logger();
	
	void			CreateConsole();
	void			DestroyConsole();
	
	LogStatusType	GetLogStatus() const { return m_Status; }
	void			SetLogStatus( LogStatusType logStatus ) { m_Status = logStatus; }

private:
	LogStatusType	m_Status = LOG_NONE;
};