#pragma once

class ClientSession;

struct OverlappedSendContext;
struct OverlappedPreRecvContext;
struct OverlappedRecvContext;
struct OverlappedDisconnectContext;
struct OverlappedAcceptContext;

static bool	mCLIENT_RUNNING;

class IocpManager
{
public:
	IocpManager();
	~IocpManager();

	bool Initialize();
	void Finalize();

	bool StartIoThreads();
	void StartConnect();


	HANDLE GetComletionPort()	{ return mCompletionPort; }
	int	GetIoThreadCount()		{ return mIoThreadCount;  }

	//SOCKET* GetListenSocket()  { return &mListenSocket;  }

	static char mAcceptBuf[64];
	static LPFN_DISCONNECTEX mFnDisconnectEx;
	static LPFN_ACCEPTEX mFnAcceptEx;
	static LPFN_CONNECTEX mFnConnectEx;


	// 시간도 없고 깔끔하게 안나온다!! 락잡고 그냥 일단 구현하자!
	UINT			GSendBytes = 0;
	UINT			GRecvBytes = 0;

private:

	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);

	static bool PreReceiveCompletion(ClientSession* client, OverlappedPreRecvContext* context, DWORD dwTransferred);
	static bool ReceiveCompletion(ClientSession* client, OverlappedRecvContext* context, DWORD dwTransferred);
	static bool SendCompletion(ClientSession* client, OverlappedSendContext* context, DWORD dwTransferred);

private:

	HANDLE	mCompletionPort;
	int		mIoThreadCount;

	
	//SOCKET	mListenSocket;
};

extern IocpManager* GIocpManager;


BOOL DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved);

BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
	DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);

BOOL ConnectEx(SOCKET s, const struct sockaddr *name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped	);