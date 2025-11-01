#include "DBHelper.h"

DBHelper::DBHelper()
{
	SQLEnvHandle = NULL;
	SQLConnectionHandle = NULL;
	SQLStatementHandle = NULL;
	retCode = 0;
}


bool DBHelper::init()
{
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
		// Allocates the environment
		return false;

	if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		// Sets attributes that govern aspects of environments
		return false;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
		// Allocates the connection
		return false;

	if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
		// Sets attributes that govern aspects of connections
		return false;

	return true;
}

bool DBHelper::startConnection(const char connectionString[1024])
{
	SQLCHAR retConString[1024];
	switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)connectionString, SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
		// Establishes connections to a driver and a data source
	case SQL_SUCCESS:
		break;
	case SQL_SUCCESS_WITH_INFO:
		break;
	case SQL_NO_DATA_FOUND:
		showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
		retCode = -1;
		break;
	case SQL_INVALID_HANDLE:
		showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
		retCode = -1;
		break;
	case SQL_ERROR:
		showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
		retCode = -1;
		break;
	default:
		break;
	}

	if (retCode == -1)
		return false;
	else
		return true;
}

bool DBHelper::fetchSingleColumnData(const char* SQLQuery, unsigned int columnNumber, std::string& returnString)
{
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
		// Allocates the statement
		return false;

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
		// Executes a preparable statement
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		return false;
	}
	else {
		int head[1];
		//		int sub;
		int p = 0;
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
			// Fetches the next rowset of data from the result
			SQLGetData(SQLStatementHandle, columnNumber, SQL_C_DEFAULT, &head, sizeof(head), NULL);
			// Retrieves data for a single column in the result set
			returnString += head[0] + ";";
		}
		return true;
	}
}

void DBHelper::closeConnection()
{
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
