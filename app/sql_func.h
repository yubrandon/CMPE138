/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#ifndef SQL_FUNC_H
#define SQL_FUNC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include "include/spdlog/sinks/basic_file_sink.h"

#include "user.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


/*SQL Querying Functions*/
void db_init();
void state_init();
bool user_exists(std::string user);     //return boolean indicating if username exists in db
bool ssn_exists(std::string ssn);               //return boolean indicating if ssn exists in db
void get_user(User *user);
bool verify_user(std::string user, std::string pw);         //return boolean indicating if username and password input exist in a tuple
void create_user(std::string ssn, std::string name, std::string user, std::string pw, std::string lname, std::string fname);  //add tuple to db using user inputs

std::vector<std::string> get_inventory(int dnum);
std::vector<std::string> get_subordinate(int ssn);

std::vector<int> get_bom_id(int prnum);   //returns a vector containing the material id for each material used in a product
std::vector<std::string> get_bom_desc(std::vector<int> id_vec); //returns a vector containing material descriptions for a vector of material ids

void view_IQC_list();
void create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size);
void approve_inspection(int insp_num);
void view_inspection(int insp_num);
void move_to_IPQC(int pn);

void receive_material(int pn);
void backflush_product(int pn);
void move_to_IQC(int pn);
void move_to_OQC(int pn);

#endif