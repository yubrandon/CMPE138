/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#ifndef SQL_FUNC_H
#define SQL_FUNC_H

#include <iostream>
#include "include/spdlog/sinks/basic_file_sink.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


/*SQL Querying Functions*/
bool user_exists(std::string user);
bool ssn_exists(int ssn);

bool verify_user(std::string user, std::string pw);
void create_user(int ssn, std::string name, std::string user, std::string pw);



#endif