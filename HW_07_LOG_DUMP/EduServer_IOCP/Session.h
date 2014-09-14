#pragma once
#include "CircularBuffer.h"
#include "OverlappedIOContext.h"


class Session
{
public:
	Session(size_t sendBufSize, size_t recvBufSize);
	virtual ~Session() {}

	bool IsConnected() const { return !!mConnected; }

	void DisconnectRequest(DisconnectReason dr) ;

	bool PreRecv(); ///< zero byte recv
	bool PostRecv() ;

	bool PostSend(const char* data, size_t len);
	bool FlushSend() ;

	void DisconnectCompletion(DisconnectReason dr) ;
	void SendCompletion(DWORD transferred) ;
	void RecvCompletion(DWORD transferred) ;

	void AddRef();
	void ReleaseRef();

	virtual void OnDisconnect(DisconnectReason dr) {}
	virtual void OnRelease() {}

	void	SetSocket(SOCKET sock) { mSocket = sock; }
	SOCKET	GetSocket() const { return mSocket; }

	void EchoBack();

	void Handle_CS_LOGIN();
	bool Handle_SC_LOGIN();
	void Handle_CS_CHAT();
	bool Handle_SC_CHAT();
	void Handle_CS_MOVE();
	bool Handle_SC_MOVE();

protected:

	SOCKET			mSocket;

	CircularBuffer	mRecvBuffer;
	CircularBuffer	mSendBuffer;
	FastSpinlock	mSendBufferLock;
	int				mSendPendingCount;

	volatile long	mRefCount;
	volatile long	mConnected;

	int				mPlayerID = -1;

};


extern __declspec(thread) std::deque<Session*>* LSendRequestSessionList;