// EduServer_IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Exception.h"
#include "EduServer_IOCP.h"
#include "ServerSession.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "IocpManager.h"
#include "DBManager.h"
#include "mypacket.pb.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/text_format.h>

using namespace google;

int _tmain(int argc, _TCHAR* argv[])
{
	LThreadType = THREAD_MAIN;


	/*
	proto test


	MyPacket::Position inputPosition;
	float testX = 1.1f;
	float testY = 1.2f;
	float testZ = 1.3f;
	inputPosition.set_x( testX );
	inputPosition.set_y( testY );
	inputPosition.set_z( testZ );

	int bufSize = inputPosition.ByteSize();
	char* outputBuf = new char[bufSize];

	memset( outputBuf, 0, sizeof( char )*bufSize );

	protobuf::io::ArrayOutputStream os( outputBuf, bufSize );
	inputPosition.SerializeToZeroCopyStream( &os );

	protobuf::io::ArrayInputStream is( outputBuf, bufSize );
	MyPacket::Position outputPosition;
	outputPosition.ParseFromZeroCopyStream( &is );

	float x = outputPosition.x();

	printf_s( "%f \n", x );

	getchar();
	*/

	/// for dump on crash
	SetUnhandledExceptionFilter(ExceptionFilter);

	/// Global Managers
	GClientSessionManager = new ClientSessionManager;
	GIocpManager = new IocpManager;
	GDatabaseManager = new DBManager;
	

	if (false == GIocpManager->Initialize())
		return -1;

	if (false == GDatabaseManager->Initialize())
		return -1;

	if (false == GDatabaseManager->StartDatabaseThreads())
		return -1;

	if (false == GIocpManager->StartIoThreads())
		return -1;

	printf_s("Start Server\n");

 	GIocpManager->StartAccept(); ///< block here...


	GDatabaseManager->Finalize();
	GIocpManager->Finalize();

	printf_s("End Server\n");


	delete GDatabaseManager;
	delete GIocpManager;
	delete GClientSessionManager;

	return 0;
}

