#pragma once
#include "ObjectPool.h"
#include "MemoryPool.h"
#include "CircularBuffer.h"
#include "SyncExecutable.h"

#define BUFSIZE	65536

class ClientSession ;
class SessionManager;
class Player;

enum IOType
{
	IO_NONE,
	IO_SEND,
	IO_SEND_FIRST,
	IO_RECV,
	IO_RECV_ZERO,
	IO_ACCEPT,
	IO_CONNECT,
	IO_DISCONNECT
} ;

enum DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
};

struct OverlappedIOContext
{
	OverlappedIOContext(ClientSession* owner, IOType ioType);

	OVERLAPPED		mOverlapped ;
	ClientSession*	mSessionObject ;
	IOType			mIoType ;
	WSABUF			mWsaBuf;
	
} ;

struct OverlappedSendContext : public OverlappedIOContext, public ObjectPool<OverlappedSendContext>
{
	OverlappedSendContext(ClientSession* owner) : OverlappedIOContext(owner, IO_SEND)
	{
	}
};

struct OverlappedRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedRecvContext>
{
	OverlappedRecvContext(ClientSession* owner) : OverlappedIOContext(owner, IO_RECV)
	{
	}
};

struct OverlappedPreRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedPreRecvContext>
{
	OverlappedPreRecvContext(ClientSession* owner) : OverlappedIOContext(owner, IO_RECV_ZERO)
	{
	}
};

struct OverlappedPreSendContext: public OverlappedIOContext, public ObjectPool < OverlappedPreSendContext >
{
	OverlappedPreSendContext( ClientSession* owner ): OverlappedIOContext( owner, IO_SEND_FIRST )
	{
	}
};

struct OverlappedDisconnectContext : public OverlappedIOContext, public ObjectPool<OverlappedDisconnectContext>
{
	OverlappedDisconnectContext(ClientSession* owner, DisconnectReason dr) 
	: OverlappedIOContext(owner, IO_DISCONNECT), mDisconnectReason(dr)
	{}

	DisconnectReason mDisconnectReason;
};

struct OverlappedAcceptContext : public OverlappedIOContext, public ObjectPool<OverlappedAcceptContext>
{
	OverlappedAcceptContext(ClientSession* owner) : OverlappedIOContext(owner, IO_ACCEPT)
	{}
};

struct OverlappedConnectContext: public OverlappedIOContext, public ObjectPool < OverlappedConnectContext >
{
	OverlappedConnectContext( ClientSession* owner ): OverlappedIOContext( owner, IO_CONNECT )
	{
	}
};

void DeleteIoContext(OverlappedIOContext* context) ;


class ClientSession : public PooledAllocatable
{
public:
	ClientSession();
	~ClientSession() {}

	void	SessionReset();

	bool	IsConnected() const { return !!mConnected; }

	bool	PostConnect();
	void	ConnectCompletion();

	bool	PreSend();
	bool	PreRecv(); ///< zero byte recv

	bool	PostRecv();
	void	RecvCompletion(DWORD transferred);

	bool	PostSend();
	void	SendCompletion(DWORD transferred);
	
	void	DisconnectRequest(DisconnectReason dr);
	void	DisconnectCompletion(DisconnectReason dr);
	
	void	AddRef();
	void	ReleaseRef();

	void	SetSocket(SOCKET sock) { mSocket = sock; }
	SOCKET	GetSocket() const { return mSocket;  }

public:
	//std::shared_ptr<Player> mPlayer;

private:
	
	SOCKET			mSocket ;

	SOCKADDR_IN		mClientAddr ;
		
	FastSpinlock	mBufferLock;

	CircularBuffer	mBuffer;

	volatile long	mRefCount;
	volatile long	mConnected;

	friend class SessionManager;
} ;



