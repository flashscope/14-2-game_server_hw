#include "stdafx.h"
#include "SQLTest.h"
#include "SQLStatement.h"
#include "DBHelper.h"
//--todo: �Ʒ��� DbTestFunc ������ �� ����Ǵ��� �׽�Ʈ! (�Ʒ��� �Լ��� ClientSession���� ������ ������ ������ ȣ����Ѻ� ��)

void DbTestFunc()
{
	{
		DbHelper dbHelper;

		dbHelper.BindParamText( L"DbTestPlayer" );
		if ( dbHelper.Execute( SQL_CreatePlayer ) )
		{
			if ( dbHelper.FetchRow() )
			{
				printf( "ok1 \n" );
			}
		}
	}

	{
		DbHelper dbHelper;

		int uid = 101;
		float x = 2301.34f;
		float y = 56000.78f;
		float z = 990002.32f;

		dbHelper.BindParamInt( &uid );
		dbHelper.BindParamFloat( &x );
		dbHelper.BindParamFloat( &y );
		dbHelper.BindParamFloat( &z );

		if ( dbHelper.Execute( SQL_UpdatePlayerPosition ) )
		{
			if ( dbHelper.FetchRow() )
			{
				printf( "ok2 \n" );
			}
		}
	}

	{
		DbHelper dbHelper;

		int uid = 101;

		dbHelper.BindParamInt( &uid );
		dbHelper.BindParamText( L"Update�� �ڸ�Ʈ..�Դϴ�." );
		if ( dbHelper.Execute( SQL_UpdatePlayerComment ) )
		{
			if ( dbHelper.FetchRow() )
			{
				printf( "ok3 \n" );
			}
		}
	}

	{
		DbHelper dbHelper;

		int uid = 101;
		bool v = true;
		dbHelper.BindParamInt( &uid );
		dbHelper.BindParamBool( &v );
		if ( dbHelper.Execute( SQL_UpdatePlayerValid ) )
		{
			if ( dbHelper.FetchRow() )
			{
				printf( "ok4 \n" );
			}
		}
	}

	{
		DbHelper dbHelper;

		int uid = 101;
		dbHelper.BindParamInt( &uid );

		wchar_t name[32];
		float x = 0;
		float y = 0;
		float z = 0;
		bool valid = false;
		wchar_t comment[256];

		dbHelper.BindResultColumnText( name, 32 );
		dbHelper.BindResultColumnFloat( &x );
		dbHelper.BindResultColumnFloat( &y );
		dbHelper.BindResultColumnFloat( &z );
		dbHelper.BindResultColumnBool( &valid );
		dbHelper.BindResultColumnText( comment, 256 );

		if ( dbHelper.Execute( SQL_LoadPlayer ) )
		{
			if ( dbHelper.FetchRow() )
			{
				printf( "%ls %f %f %f %d %ls\n", name, x, y, z, valid, comment );
			}
		}
	}

	{
		DbHelper dbHelper;

		int uid = 100;

		dbHelper.BindParamInt( &uid );
		if ( dbHelper.Execute( SQL_DeletePlayer ) )
		{
			if ( dbHelper.FetchRow() )
			{
				printf( "ok5 \n" );
			}
		}
	}

}

