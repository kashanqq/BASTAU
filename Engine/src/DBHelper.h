#pragma once
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include "NodeTree.h"
#include <iostream>
#include <string>

class DBHelper {
/*private:
	SQLHANDLE SQLEnvHandle;
	SQLHANDLE SQLConnectionHandle;
	SQLHANDLE SQLStatementHandle;
	SQLRETURN retCode;
public:

	DBHelper();
	bool init();
	bool startConnection(const char connectionString[1024]);
	bool fetchSingleColumnData(const char* SQLQuery, unsigned int columnNumber, std::string& returnString);
	void closeConnection();


	void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
	{
		SQLCHAR SQLState[1024];
		SQLCHAR message[1024];
		if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
			// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
			std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
	}*/
};