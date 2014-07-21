#include "stdafx.h"
#include "Exception.h"
#include "EduServer_IOCP.h"
#include "ClientSession.h"
#include "IocpManager.h"
#include "SessionManager.h"
#include "MacroSet.h"

ClientSession::ClientSession(SOCKET sock) : mSocket(sock), mConnected(false)
{
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
}

ClientSession::~ClientSession()
{
}

bool ClientSession::OnConnect(SOCKADDR_IN* addr)
{
	mTest = 2;
	//TODO: �� ���� lock���� ��ȣ �� ��
	FastSpinlockGuard EnterLock(m_Lock);

	CRASH_ASSERT(LThreadType == THREAD_MAIN_ACCEPT);

	/// make socket non-blocking
	u_long arg = 1 ;
	ioctlsocket(mSocket, FIONBIO, &arg) ;

	/// turn off nagle
	int opt = 1 ;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)) ;

	opt = 0;
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)) )
	{
		printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError()) ;
		return false;
	}
	//TODO: ���⿡�� CreateIoCompletionPort((HANDLE)mSocket, ...);�����Ͽ� ������ ��
	HANDLE handle = CreateIoCompletionPort((HANDLE)mSocket, GIocpManager->GetComletionPort(), (ULONG_PTR)this, 0);
	printf_s("SessionNumber :%d \n", (ULONG_PTR)this);

	if (handle != GIocpManager->GetComletionPort())
	{
		printf_s("[DEBUG] CreateIoCompletionPort error: %d\n", GetLastError());
		return false;
	}

	memcpy(&mClientAddr, addr, sizeof(SOCKADDR_IN));
	mConnected = true ;

	printf_s("[DEBUG] Client Connected: IP=%s, PORT=%d\n", inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port));

	GSessionManager->IncreaseConnectionCount();



	return PostRecv() ;
}

void ClientSession::Disconnect(DisconnectReason dr)
{
	printf_s("Disconnect clientSession :%d \n", this);
	//TODO: �� ���� lock���� ��ȣ�� ��
	FastSpinlockGuard EnterLock(m_Lock);

	if (!mConnected)
	{
		return;
	}
	
	LINGER lingerOption ;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)) )
	{
		printf_s("[DEBUG] setsockopt linger option error: %d\n", GetLastError());
	}

	printf_s("[DEBUG] Client Disconnected: Reason=%d IP=%s, PORT=%d \n", dr, inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port));
	
	GSessionManager->DecreaseConnectionCount();

	closesocket(mSocket) ;

	mConnected = false ;

}

bool ClientSession::PostRecv() const
{
	Log("PostRecv clientSession :[%d] \n", this);
	if (!IsConnected())
	{
		return false;
	}
		

	OverlappedIOContext* recvContext = new OverlappedIOContext(this, IO_RECV);

	//TODO: WSARecv �����Ͽ� ������ ��
	recvContext->mWsaBuf.len = BUFSIZE;
	recvContext->mWsaBuf.buf = recvContext->mBuffer;


	DWORD recvbytes = 0;
	DWORD flags = 0;
	int ret = WSARecv(mSocket, &(recvContext->mWsaBuf), 1, &recvbytes, &flags, &(recvContext->mOverlapped), NULL);
	
	if (ret == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();

		if (errorCode != ERROR_IO_PENDING)
		{
			// context  삭제 아래도...
			return false;
		}
	}


	return true;
}

bool ClientSession::PostSend(const char* buf, int len) const
{
	Log("PostSend clientSession :[%d] [%s] [%d] \n", this, buf, len);
	if (!IsConnected())
	{
		return false;
	}
		
	OverlappedIOContext* sendContext = new OverlappedIOContext(this, IO_SEND);

	/// copy for echoing back..
	memcpy_s(sendContext->mBuffer, BUFSIZE, buf, len);
	sendContext->mWsaBuf.buf = sendContext->mBuffer;
	sendContext->mWsaBuf.len = len;

	//TODO: WSASend �����Ͽ� ������ ��
	DWORD sendbytes;

	int ret = WSASend(mSocket, &(sendContext->mWsaBuf), 1, &sendbytes, 0, &(sendContext->mOverlapped), NULL);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}

	return true;
}

