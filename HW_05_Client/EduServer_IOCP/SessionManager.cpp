#include "stdafx.h"
#include "FastSpinlock.h"
#include "MemoryPool.h"
#include "ThreadLocal.h"
#include "EduServer_IOCP.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "IocpManager.h"


SessionManager* GSessionManager = nullptr;

SessionManager::SessionManager() : mCurrentIssueCount(0), mCurrentReturnCount(0), mLock(LO_FIRST_CLASS)
{
}

SessionManager::~SessionManager()
{
	for (auto it : mFreeSessionList)
	{
		xdelete(it);
	}
}

void SessionManager::PrepareSessions()
{
	CRASH_ASSERT(LThreadType == THREAD_MAIN);

	for (int i = 0; i < MAX_CONNECTION_SESSION; ++i)
	{
		ClientSession* client = xnew<ClientSession>();
			
		mFreeSessionList.push_back(client);
	}
}





void SessionManager::ReturnClientSession(ClientSession* client)
{
	FastSpinlockGuard guard(mLock);

	CRASH_ASSERT(client->mConnected == 0 && client->mRefCount == 0);

	client->SessionReset();

	mFreeSessionList.push_back(client);

	++mCurrentReturnCount;
}

bool SessionManager::SessionsConnect()
{
	FastSpinlockGuard guard(mLock);

	while (mCurrentIssueCount - mCurrentReturnCount < MAX_CONNECTION_SESSION)
	{
		ClientSession* newClient = mFreeSessionList.back();
		mFreeSessionList.pop_back();

		++mCurrentIssueCount;

		newClient->AddRef(); ///< refcount +1 for issuing 
		
		if (false == newClient->PostConnect())
			return false;
	}


	return true;
}

bool SessionManager::NoMoreClients()
{
	if ( MAX_CONNECTION_SESSION == mFreeSessionList.size() )
	{
		return true;
	}

	return false;
}
