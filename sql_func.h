#ifndef SQL_FUNC_H
#define SQL_FUNC_H

#include <iostream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

/*SQL Querying Functions*/
bool user_exists(std::string user);




#endif