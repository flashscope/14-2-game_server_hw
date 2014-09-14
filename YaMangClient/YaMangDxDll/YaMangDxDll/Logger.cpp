#include "stdafx.h"
#include "Logger.h"
#include <stdio.h>

Logger::Logger()
{
	AllocConsole();
	FILE* pFile;
	freopen_s( &pFile, "CONOUT$", "wb", stdout );
}

Logger::~Logger()
{
	FreeConsole();
}
