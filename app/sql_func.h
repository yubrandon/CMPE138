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
void init();
bool user_exists(std::string user);     //return boolean indicating if username exists in db
bool ssn_exists(int ssn);               //return boolean indicating if ssn exists in db

bool verify_user(std::string user, std::string pw);         //return boolean indicating if username and password input exist in a tuple
void create_user(int ssn, std::string name, std::string user, std::string pw);  //add tuple to db using user inputs


void view_IQC_list();
void create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size);
void approve_inspection(int insp_num);
void view_inspection(int insp_num);
void move_to_IPQC(int pn);

void receive_material(int pn);
void backflush_product(int pn);
void move_to_IQC(int pn);

#endif