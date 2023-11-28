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
#include "sha256.h"
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
void create_user(std::string ssn, std::string user, std::string pw, std::string lname, std::string fname);  //add tuple to db using user inputs

//administrator
void assign_dept(int id,int dnum);      //assign department number for a new employee
void assign_role(int id, std::string role);               //assign job_title for a new employee
void assign_dept_mgr(std::string ssn, int dnum); //assign a new manager for a department

void create_dept(int dnum,std::string d_desc);  //add a new department tuple


//Supervisor/Department manager
std::vector<int> get_supervisee(std::string ssn);       //returns vector containing supervisee ids for a supervisor (user -> ssn used in argument)
std::vector<int> get_inventory(int dnum);       //returns vector containing material_nums for their department

std::vector<int> get_bom_id(int prnum);   //returns a vector containing the material id for each material used in a product
std::vector<std::string> get_bom_desc(std::vector<int> id_vec); //returns a vector containing material descriptions for a vector of material ids

/* ------------------------------------------- VIEW SQL FUNCTIONS ----------------------------------------------------- */
void view_inspection(int insp_num);
void view_inspections(std::string dept_name, std::string title);
void view_inspection_requirements(int insp_num);

/* -------------------------------------- ADD NEW INSPECTION SQL FUNCTIONS ---------------------------------------------- */
void create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size);
int get_next_insp_num(); //lanaiya
double get_sample_size(int insp_num); //lanaiya
void add_inspection(int insp_num, int pn, int insp_qty, std::string insp_area); //brandon
void update_inspection_requirements(int insp_num, int id, std::string time, std::string insp_req, std::string insp_res, int qty_pass);

/* -------------------------------------- APPROVE INSPECTION SQL FUNCTIONS ---------------------------------------------- */
void approve_inspection(int insp_num); //still used?
int get_qty_inspected(int insp_num); //lanaiya
int calculate_fpy(int insp_num); //lanaiya
void set_insp_pf(std::string); //lanaiya

/* ------------------------------------------- ADD PART SQL FUNCTIONS --------------------------------------------------- */
int get_next_p_num(); //brandon
void add_to_materials(int mat_num, std::string mat_desc, std::string supp_name, int supp_num); //brandon

/* ------------------------------------ CREATE REQUIREMENTS SQL FUNCTIONS ------------------------------------------------ */
bool part_exists(int mat_prod_num); //brandon
void create_requirements(int mat_prod_num, std::string requirement, std::string res_type); //brandon

/* ------------------------------------ MOVE INSP TO FGI SQL FUNCTIONS ------------------------------------------------ */
void move_INSP_to_STORES(int p_num, int qty); //lanaiya
void move_STORES_to_WIP(int p_num, int qty); //lanaiya
void move_WIP_to_QC(int p_num, int qty); //lanaiya
void move_QC_to_FGI(int p_num, int qty); //lanaiya

/* -----------------------------------------INV ASSOC MENU FUNCTIONS -------------------------------------------------- */
void receive_material(int pn); //brandon
void backflush_product(int pn); //brandon
int get_insp_num(int pnum);  //brandon


//ACCESSORS AND MODIFIERS FOR SETTING INDIVIDUAL ATTRIBUTES
void set_string_attribute(std::string table, std::string attr, std::string table_value, std::string attr_value);
void set_bool_attribute(std::string table, std::string attr, std::string table_value, bool attr_value);
void set_int_attribute(std::string table, std::string attr, std::string table_value, int attr_value);
void set_double_attribute(std::string table, std::string attr, std::string table_value, double attr_value);

// SQL FUNCTIONS NO LONGER NEEDED

//SQL Queries for Adding Material and Product - removed since we are now combining mat/prod into single table
// int get_next_mat_num();
// int get_next_prod_num();

// void view_IQC_list(); //clean up, no longer needed, now in view_inspections


// MISC FUNCTIONS NO LONGER NEEDED
// void add_material(); //no longer needed
// void add_product(); //no longer needed
// int get_next_mat_num(); //should be get_part_num

#endif