#include "stdafx.h"
#include "SQLStatement.h"
#include "Log.h"
#include "PlayerDBContext.h"
#include "DBHelper.h"
#include "ClientSession.h"


//-todo: CreatePlayerDataContext ����
bool CreatePlayerDataContext::OnSQLExecute()
{
	DBHelper dbHelper;

	dbHelper.BindResultColumnText( mPlayerName, MAX_NAME_LEN );

	if ( dbHelper.Execute( SQL_CreatePlayer ) ) 
	{
		if ( dbHelper.FetchRow() )
		{
			return true;
		}
	}

	return false;
}

void CreatePlayerDataContext::OnSuccess()
{
	mSessionObject->mPlayer.TestCreatePlayerData( mPlayerName );
}

void CreatePlayerDataContext::SetPlayerName( const wchar_t* name )
{
	wcscpy_s( mPlayerName, name );
}



//-todo: DeletePlayerDataContext ����
bool DeletePlayerDataContext::OnSQLExecute()
{
	DBHelper dbHelper;

	dbHelper.BindParamInt( &mPlayerId );

	if ( dbHelper.Execute( SQL_DeletePlayer ) )
	{
		if ( dbHelper.FetchRow() )
		{
			return true;
		}
	}

	return false;
}

void DeletePlayerDataContext::OnSuccess()
{
	mSessionObject->mPlayer.TestDeletePlayerData( mPlayerId );
}

void DeletePlayerDataContext::OnFail()
{
	EVENT_LOG( "DeletePlayerDataContext fail", mPlayerId );
}



bool LoadPlayerDataContext::OnSQLExecute()
{
	DBHelper dbHelper;

	dbHelper.BindParamInt(&mPlayerId);

	dbHelper.BindResultColumnText(mPlayerName, MAX_NAME_LEN);
	dbHelper.BindResultColumnFloat(&mPosX);
	dbHelper.BindResultColumnFloat(&mPosY);
	dbHelper.BindResultColumnFloat(&mPosZ);
	dbHelper.BindResultColumnBool(&mIsValid);
	dbHelper.BindResultColumnText(mComment, MAX_COMMENT_LEN);

	if (dbHelper.Execute(SQL_LoadPlayer))
	{
		if (dbHelper.FetchRow())
		{
			return true;
		}
	}

	return false;
}

void LoadPlayerDataContext::OnSuccess()
{
	//-todo: �÷��̾� �ε� ������ ó���ϱ�
	mSessionObject->mPlayer.ResponseLoad(mPlayerId, mPosX, mPosY, mPosZ, mIsValid, mPlayerName, mComment );
}

void LoadPlayerDataContext::OnFail()
{
	EVENT_LOG("LoadPlayerDataContext fail", mPlayerId);
}


bool UpdatePlayerPositionContext::OnSQLExecute()
{
	DBHelper dbHelper;
	int result = 0;

	dbHelper.BindParamInt(&mPlayerId);
	dbHelper.BindParamFloat(&mPosX);
	dbHelper.BindParamFloat(&mPosY);
	dbHelper.BindParamFloat(&mPosZ);

	dbHelper.BindResultColumnInt(&result);

	if (dbHelper.Execute(SQL_UpdatePlayerPosition))
	{
		if (dbHelper.FetchRow())
		{
			return result != 0;
		}
	}

	return false;
}

void UpdatePlayerPositionContext::OnSuccess()
{
	mSessionObject->mPlayer.ResponseUpdatePosition(mPosX, mPosY, mPosZ);
}




bool UpdatePlayerCommentContext::OnSQLExecute()
{
	DBHelper dbHelper;
	int result = 0;
	dbHelper.BindParamInt(&mPlayerId);
	dbHelper.BindParamText(mComment);

	dbHelper.BindResultColumnInt(&result);

	if (dbHelper.Execute(SQL_UpdatePlayerComment))
	{
		if (dbHelper.FetchRow())
		{
			return result != 0;
		}
	}

	return false;
}

void UpdatePlayerCommentContext::SetNewComment(const wchar_t* comment)
{
	wcscpy_s(mComment, comment);
}

void UpdatePlayerCommentContext::OnSuccess()
{
	mSessionObject->mPlayer.ResponseUpdateComment(mComment);
}



bool UpdatePlayerValidContext::OnSQLExecute()
{
	DBHelper dbHelper;
	int result = 0;

	dbHelper.BindParamInt(&mPlayerId);
	dbHelper.BindParamBool(&mIsValid);

	dbHelper.BindResultColumnInt(&result);

	if (dbHelper.Execute(SQL_UpdatePlayerValid))
	{
		if (dbHelper.FetchRow())
		{
			return result !=0 ;
		}
	}

	return false;
}

void UpdatePlayerValidContext::OnSuccess()
{
	mSessionObject->mPlayer.ResponseUpdateValidation(mIsValid);
}

