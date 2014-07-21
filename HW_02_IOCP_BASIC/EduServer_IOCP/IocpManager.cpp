#include "stdafx.h"
#include "IocpManager.h"
#include "EduServer_IOCP.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "MacroSet.h"

#define GQCS_TIMEOUT	20

__declspec(thread) int LIoThreadId = 0;
IocpManager* GIocpManager = nullptr;

IocpManager::IocpManager() : mCompletionPort(NULL), mIoThreadCount(2), mListenSocket(NULL)
{
}


IocpManager::~IocpManager()
{
}

bool IocpManager::Initialize()
{
	//TODO: mIoThreadCount = ...;GetSystemInfo�����ؼ� set num of I/O threads
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	mIoThreadCount = si.dwNumberOfProcessors * 2;



	/// winsock initializing
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
		

	/// Create I/O Completion Port
	//TODO: mCompletionPort = CreateIoCompletionPort(...)
	mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!mCompletionPort)
	{
		return false;
	}


	/// create TCP socket
	//TODO: mListenSocket = ...
	if ( INVALID_SOCKET == ( m_ListenSocket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED ) ) )
	{
		printf_s( "Socket() Failed with Error Code %d \n", WSAGetLastError() );
		return false;
	}

	int opt = 1;
	setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	//TODO:  bind
	//if (SOCKET_ERROR == bind(mListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
	//	return false;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9001);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(mListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
	{
		return false;
	}


	return true;
}


bool IocpManager::StartIoThreads()
{
	/// I/O Thread
	for (int id = 0; id < mIoThreadCount; ++id)
	{
		DWORD dwThreadId;
		//TODO: HANDLE hThread = (HANDLE)_beginthreadex...);
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, IoWorkerThread, (LPVOID)id, 0, (unsigned*)&dwThreadId);
		
		if (!hThread)
		{
			return false;
		}
	}

	return true;
}


bool IocpManager::StartAcceptLoop()
{
	/// listen
	if (SOCKET_ERROR == listen(mListenSocket, SOMAXCONN))
	{
		return false;
	}

	/// accept loop
	while (true)
	{
		SOCKET acceptedSock = accept(mListenSocket, NULL, NULL);
		if (acceptedSock == INVALID_SOCKET)
		{
			printf_s("accept: invalid socket\n");
			continue;
		}

		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(acceptedSock, (SOCKADDR*)&clientaddr, &addrlen);

		/// ���� ���� ����ü �Ҵ��� �ʱ�ȭ
		ClientSession* client = GSessionManager->CreateClientSession(acceptedSock);

		/// Ŭ�� ���� ó��
		if (false == client->OnConnect(&clientaddr))
		{
			client->Disconnect(DR_ONCONNECT_ERROR);
			GSessionManager->DeleteClientSession(client);
		}
	}

	return true;
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
	HANDLE hComletionPort = GIocpManager->GetComletionPort();

	while (true)
	{
		DWORD dwTransferred = 0;
		OverlappedIOContext* context = nullptr;
		ClientSession* asCompletionKey = nullptr;

		///<���⿡�� GetQueuedCompletionStatus(hComletionPort, ..., GQCS_TIMEOUT)�� ������ �������� ����
		int ret = GetQueuedCompletionStatus(hComletionPort, &dwTransferred, (PULONG_PTR)&asCompletionKey, (LPOVERLAPPED*)&context, GQCS_TIMEOUT);

		/// check time out first 
		if (ret == 0 && GetLastError() == WAIT_TIMEOUT)
		{
			continue;
		}
		
		if (ret == 0 || dwTransferred == 0)
		{
			/// connection closing
			asCompletionKey->Disconnect(DR_RECV_ZERO);
			GSessionManager->DeleteClientSession(asCompletionKey);
			continue;
		}
		
		// if (nullptr == context) �� ���� ó��
		if (nullptr == context)
		{
			asCompletionKey->Disconnect(DR_RECV_ZERO);
			GSessionManager->DeleteClientSession(asCompletionKey);
			continue;
		}

		bool completionOk = true;
		switch (context->mIoType)
		{
		case IO_SEND:
			Log("IO_SEND I/O \n");
			completionOk = SendCompletion(asCompletionKey, context, dwTransferred);
			break;

		case IO_RECV:
			Log("IO_RECV I/O \n");
			completionOk = ReceiveCompletion(asCompletionKey, context, dwTransferred);
			break;

		default:
			Log("Unknown I/O Type: %d\n", context->mIoType);
			break;
		}

		if ( !completionOk )
		{
			/// connection closing
			asCompletionKey->Disconnect(DR_COMPLETION_ERROR);
			GSessionManager->DeleteClientSession(asCompletionKey);
		}

	}

	return 0;
}

bool IocpManager::ReceiveCompletion(const ClientSession* client, OverlappedIOContext* context, DWORD dwTransferred)
{
	Log("ReceiveCompletion %s [%d] \n", context->mBuffer, dwTransferred);

	/// echo back ó�� client->PostSend()����.
	client->PostSend(context->mBuffer, dwTransferred);


	delete context;
	return client->PostRecv();
}

bool IocpManager::SendCompletion(const ClientSession* client, OverlappedIOContext* context, DWORD dwTransferred)
{
	/// ���� �� �Ǿ����� Ȯ���ϴ� �� ó��..
	//if (context->mWsaBuf.len != dwTransferred) {...}
	if (context->mWsaBuf.len != dwTransferred)
	{
		Log("SendCompletion error \n");
		delete context;
		return false;
	}

	delete context;
	return true;
}
