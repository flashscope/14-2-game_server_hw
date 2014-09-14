#include "stdafx.h"
#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::CreateConsole()
{
	AllocConsole();
	FILE* pFile;
	freopen_s( &pFile, "CONOUT$", "wb", stdout );
}

void Logger::DestroyConsole()
{
	FreeConsole();
}
