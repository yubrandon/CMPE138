/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#include <iostream>
#include <fstream>
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



void initialize();                  //initialize db
void main_menu();                   //interactive menu to navigate options
void login();                       //login to account that is saved in database
void create_account();              //create account for user and save to database if successful
bool valid_ssn(std::string ssn);


void loginas_IQC_inspector();
void loginas_OQC_inspector();
void loginas_QAdirector();
void loginas_inv_associate();
void loginas_technician();

void display_IQCinsp_menu();
void display_OQCinsp_menu();
void display_QAdir_menu();
void display_invassoc_menu();
void display_tech_menu();

void view_all_inventory();
void view_IQC_inspections();
void view_OQC_inspections();
void add_new_inspections(std::string dept_name, std::string title);
void approve_inspection(std::string dept_name, std::string title);
void send_email(int insp_num);
void send_email(std::string dept_name, std::string title);
void add_material();
void add_product();
void create_inspection_requirements();
void view_final_product_inspections();
void receive_material();
void pull_wo();
void backflush_product();

/*
functions to show menu for each entity type
_view()
_view()
etc.


*/


#endif