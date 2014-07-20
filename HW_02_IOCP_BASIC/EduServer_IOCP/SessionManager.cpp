#include "stdafx.h"
#include "ClientSession.h"
#include "SessionManager.h"

SessionManager* GSessionManager = nullptr;

SessionManager::SessionManager() : mCurrentConnectionCount(0)
{
}


ClientSession* SessionManager::CreateClientSession(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock);

	
	//TODO: lock���� ��ȣ�� ��
	FastSpinlockGuard EnterLock(m_Lock);
	{
		mClientList.insert(ClientList::value_type(sock, client));
	}

	return client;
}


void SessionManager::DeleteClientSession(ClientSession* client)
{
	//TODO: lock���� ��ȣ�� ��
	FastSpinlockGuard EnterLock(m_Lock);
	{
		mClientList.erase(client->mSocket);
	}
	
	delete client;
}