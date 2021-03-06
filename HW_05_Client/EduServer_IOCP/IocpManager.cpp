#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "Timer.h"
#include "LockOrderChecker.h"
#include "IocpManager.h"
#include "EduServer_IOCP.h"
#include "ClientSession.h"
#include "SessionManager.h"

#define GQCS_TIMEOUT	INFINITE //20

IocpManager* GIocpManager = nullptr;

LPFN_DISCONNECTEX IocpManager::mFnDisconnectEx = nullptr;
LPFN_ACCEPTEX IocpManager::mFnAcceptEx = nullptr;
LPFN_CONNECTEX IocpManager::mFnConnectEx = nullptr;

char IocpManager::mAcceptBuf[64] = { 0, };


BOOL DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved)
{
	return IocpManager::mFnDisconnectEx(hSocket, lpOverlapped, dwFlags, reserved);
}

BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
	DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
{
	return IocpManager::mFnAcceptEx(sListenSocket, sAcceptSocket, lpOutputBuffer, dwReceiveDataLength,
		dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped);
}

BOOL ConnectEx( SOCKET s, const struct sockaddr *name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped )
{
	return IocpManager::mFnConnectEx( s, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped );
}
IocpManager::IocpManager() : mCompletionPort(NULL), mIoThreadCount(2)
{	
}


IocpManager::~IocpManager()
{
}

bool IocpManager::Initialize()
{
	/// set num of I/O threads
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	mIoThreadCount = si.dwNumberOfProcessors;
	
	/// winsock initializing
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	/// Create I/O Completion Port
	mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mCompletionPort == NULL)
		return false;

	/// create Temp TCP socket
	SOCKET mTempSocket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
	if (mTempSocket == INVALID_SOCKET)
		return false;

	/*
	HANDLE handle = CreateIoCompletionPort((HANDLE)mTempSocket, mCompletionPort, 0, 0);
	if (handle != mCompletionPort)
	{
		printf_s("[DEBUG] listen socket IOCP register error: %d\n", GetLastError());
		return false;
	}
	

	int opt = 1;
	setsockopt(mTempSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));
	

	/// bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(LISTEN_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(mTempSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
		return false;
	*/

	GUID guidDisconnectEx = WSAID_DISCONNECTEX ;
	DWORD bytes = 0 ;
	if (SOCKET_ERROR == WSAIoctl(mTempSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&guidDisconnectEx, sizeof(GUID), &mFnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, NULL, NULL) )
		return false;

	GUID guidAcceptEx = WSAID_ACCEPTEX ;
	if (SOCKET_ERROR == WSAIoctl(mTempSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(GUID), &mFnAcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
		return false;






	GUID guidConnecttEx = WSAID_CONNECTEX;
	if ( SOCKET_ERROR == WSAIoctl( mTempSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnecttEx, sizeof( GUID ), &mFnConnectEx, sizeof( LPFN_CONNECTEX ), &bytes, NULL, NULL ) )
		return false;


	if ( SOCKET_ERROR == closesocket( mTempSocket ) )
	{
		return FALSE;
	}
		
	mCLIENT_RUNNING = TRUE;
	
	/// make session pool
	GSessionManager->PrepareSessions();

	return true;
}


bool IocpManager::StartIoThreads()
{
	/// I/O Thread
	for (int i = 0; i < mIoThreadCount; ++i)
	{
		DWORD dwThreadId;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, IoWorkerThread, (LPVOID)(i+1), 0, (unsigned int*)&dwThreadId);
		if (hThread == NULL)
			return false;
	}

	return true;
}


void IocpManager::StartConnect()
{

	ULONGLONG startTickTime = GetTickCount64();
	while (GSessionManager->SessionsConnect())
	{
		Sleep(100);
		ULONGLONG nowTickTime = GetTickCount64();
		ULONGLONG sec = (nowTickTime - startTickTime) / 1000;
		if ( sec >= TIME_LIMIT)
		{
			mCLIENT_RUNNING = FALSE;
			break;
		}
	}

	// 그냥 강제 종료를 하겠다!
	return;
	/*
	while ( !GSessionManager->NoMoreClients() )
	{
		printf_s( "waiting \n" );
	}
	*/
}


void IocpManager::Finalize()
{
	CloseHandle(mCompletionPort);

	/// winsock finalizing
	WSACleanup();

}

unsigned int WINAPI IocpManager::IoWorkerThread(LPVOID lpParam)
{
	LThreadType = THREAD_IO_WORKER;
	LIoThreadId = reinterpret_cast<int>(lpParam);
	LTimer = new Timer;
	LLockOrderChecker = new LockOrderChecker(LIoThreadId);

	HANDLE hComletionPort = GIocpManager->GetComletionPort();

	while (true)
	{
		/// 타이머 작업은 항상 돌리고
		LTimer->DoTimerJob();

		/// IOCP 작업 돌리기
		DWORD dwTransferred = 0;
		OverlappedIOContext* context = nullptr;
		ULONG_PTR completionKey = 0;

		int ret = GetQueuedCompletionStatus(hComletionPort, &dwTransferred, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&context, GQCS_TIMEOUT);

		// 이러면 안되지만!!
		if ( mCLIENT_RUNNING == false )
		{
			return 0;
		}

		ClientSession* theClient = context ? context->mSessionObject : nullptr ;
		
		if (ret == 0 || dwTransferred == 0)
		{
			int gle = GetLastError();

			/// check time out first 
			if (gle == WAIT_TIMEOUT)
				continue;
		
			if ( context->mIoType == IO_RECV || context->mIoType == IO_SEND || context->mIoType == IO_SEND_FIRST )
			{
				CRASH_ASSERT(nullptr != theClient);

				/// In most cases in here: ERROR_NETNAME_DELETED(64)

				theClient->DisconnectRequest(DR_COMPLETION_ERROR);

				DeleteIoContext(context);

				continue;
			}
		}

		CRASH_ASSERT(nullptr != theClient);
	
		bool completionOk = false;
		switch (context->mIoType)
		{
		case IO_DISCONNECT:
			theClient->DisconnectCompletion(static_cast<OverlappedDisconnectContext*>(context)->mDisconnectReason);
			completionOk = true;
			break;

		case IO_CONNECT:
			theClient->ConnectCompletion();
			completionOk = true;
			break;

		case IO_RECV_ZERO:
			completionOk = PreReceiveCompletion(theClient, static_cast<OverlappedPreRecvContext*>(context), dwTransferred);
			break;

		case IO_SEND:
			completionOk = SendCompletion(theClient, static_cast<OverlappedSendContext*>(context), dwTransferred);
			break;

		case IO_SEND_FIRST:
			completionOk = SendCompletion( theClient, static_cast<OverlappedSendContext*>( context ), dwTransferred );
			break;

		case IO_RECV:
			completionOk = ReceiveCompletion(theClient, static_cast<OverlappedRecvContext*>(context), dwTransferred);
			break;

		default:
			printf_s("Unknown I/O Type: %d\n", context->mIoType);
			CRASH_ASSERT(false);
			break;
		}

		if ( !completionOk )
		{
			
			if ( FALSE == mCLIENT_RUNNING )
			{
				theClient->DisconnectRequest( DR_TEST_TIME_OUT );
			}
			

			/// connection closing
			theClient->DisconnectRequest(DR_IO_REQUEST_ERROR);
		}

		DeleteIoContext(context);
	}

	return 0;
}

bool IocpManager::PreReceiveCompletion(ClientSession* client, OverlappedPreRecvContext* context, DWORD dwTransferred)
{
	/// real receive...
	return client->PostRecv();
}

bool IocpManager::ReceiveCompletion(ClientSession* client, OverlappedRecvContext* context, DWORD dwTransferred)
{

	client->RecvCompletion(dwTransferred);

	if ( !mCLIENT_RUNNING )
	{
		return false;
	}

	/// echo back
	return client->PostSend();
}

bool IocpManager::SendCompletion(ClientSession* client, OverlappedSendContext* context, DWORD dwTransferred)
{
	client->SendCompletion(dwTransferred);

	if (context->mWsaBuf.len != dwTransferred)
	{
		printf_s("Partial SendCompletion requested [%d], sent [%d]\n", context->mWsaBuf.len, dwTransferred) ;
		return false;
	}
	
	/// zero receive
	return client->PreRecv();
}


