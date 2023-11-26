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


//SQL Queries to view various inspection lists
void view_inspections(std::string dept_name);
void view_inspections(std::string dept_name, std::string title);
void view_inspection_requirements(int insp_num);

//SQL Queries for Adding New Inspections
int get_next_insp_num();
int get_account();
int get_sample_size(int insp_num);
void add_inspection(int insp_num, int pn, int insp_qty, std::string insp_area);

//SQL Quesries for Approving Inspections
int get_qty_inspected(int insp_num);
int calculate_fpy(int insp_num);
void set_insp_pf(std::string);

//SQL Queries for Adding Material
int get_next_mat_num();
void add_to_materials(int mat_num, std::string mat_desc, std::string supp_name, int supp_num);

//SQL Queries for Adding Product
int get_next_prod_num();

//SQL Queries for Creating Inspection Requirements
bool part_exists(int mat_prod_num);
void create_requirements(int mat_prod_num, std::string requirement, std::string res_type);

#endif