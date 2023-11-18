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
bool user_exists(std::string user);     //return boolean indicating if username exists in db
bool ssn_exists(int ssn);               //return boolean indicating if ssn exists in db

bool verify_user(std::string user, std::string pw);         //return boolean indicating if username and password input exist in a tuple
void create_user(int ssn, std::string name, std::string user, std::string pw);  //add tuple to db using user inputs



#endif