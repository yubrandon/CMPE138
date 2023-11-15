/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#include <iostream>
#include "global_variables.h"
#include "sql_func.h"
#include "sha256.h"
#include "include/spdlog/sinks/basic_file_sink.h"


//functions not directly related to querying or sql
    //logging
    //initializing log file
    //auto file_logger = spdlog::basic_logger_mt("name of this error case (must be unique)","../logfile.txt");
    //test logs
    //file_logger->info("message");
    /* logging levels/types
        trace
        debug
        info
        warn
        error
        critical
    */



/*Non-SQL related functions*/
void initialize();  //initialize db and log file

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