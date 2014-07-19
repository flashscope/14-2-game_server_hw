#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"

SessionManager* GSessionManager = nullptr;

SessionManager::SessionManager() : mCurrentConnectionCount(0)
{
	InitializeSRWLock(&m_Lock);
}


ClientSession* SessionManager::CreateClientSession(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock);

	
	//TODO: lock으로 보호할 것
	AcquireSRWLockExclusive(&m_Lock);
	{
		mClientList.insert(ClientList::value_type(sock, client));
	}
	ReleaseSRWLockExclusive(&m_Lock);

	return client;
}


void SessionManager::DeleteClientSession(ClientSession* client)
{
	//TODO: lock으로 보호할 것
	AcquireSRWLockExclusive(&m_Lock);
	{
		mClientList.erase(client->mSocket);
	}
	
	delete client;
	ReleaseSRWLockExclusive(&m_Lock);
}