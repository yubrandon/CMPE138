/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#include <iostream>
#include "sql_func.h"
#include "sha256.h"
#include "spdlog/sinks/basic_file_sink.h"

//functions not directly related to querying or sql
//add spdlog functions into all functions

/*Non-SQL related functions*/
void main_menu();   //all current functions will loop back to main_menu()
void login();
void create_account();
bool valid_ssn(std::string ssn);


/*
functions to show menu for each entity type
_view()
_view()
etc.


*/

#endif