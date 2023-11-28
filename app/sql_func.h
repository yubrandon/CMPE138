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
#include "misc_func.h"
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
int get_emp_id(int ssn);

//Supervisor/Department manager
void assign_dept(int id,int dnum);      //assign department number for a new employee
void assign_role(int id, std::string role);               //assign job_title for a new employee

void create_dept(int dnum,std::string d_desc);  //add a new department tuple
void edit_dept(int dnum, std::string d_desc);    //edit description for a department
void assign_dept_mgr(int dnum, int id); //assign a new manager for a department

std::vector<int> get_supervisee(std::string ssn);       //returns vector containing supervisee ids for a supervisor (user -> ssn used in argument)
void get_inventory(int pnum);       //returns information for a part number
void get_inventory_all();

std::vector<int> get_bom_id(int prnum);   //returns a vector containing the material id for each material used in a product
std::vector<std::string> get_bom_desc(std::vector<int> id_vec); //returns a vector containing material descriptions for a vector of material ids
void pull_wo(int prnum, std::vector<int> mat_ids, int qty);

void view_IQC_list();
void create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size);
void approve_inspection(int insp_num);

void move_to_IPQC(int pn);
void move_to_FQC(int pn);
void receive_material(int pn, int qty);
void receive_material_accessory(int pn, int qty);
void backflush_product(int pn,int qty);
void move_to_IQC(int pn);
void move_to_OQC(int pn);        //moves product from IPQC to OQC (STORES to SHIP in PRODUCT table)
int get_insp_num(int pnum);     //get inspection number for a given product/part number


//SQL Queries to view various inspection lists all below
void view_inspection(int insp_num, std::string emp_role);
void view_inspection_requirements(int pnum);
/* ------------------------------------------- VIEW SQL FUNCTIONS ----------------------------------------------------- */
void view_inspection(int insp_num);
void view_inspections(std::string dept_name, std::string title);
void view_inspection_requirements(int insp_num);

/* -------------------------------------- ADD NEW INSPECTION SQL FUNCTIONS ---------------------------------------------- */
void create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size);
int get_next_insp_num(); //lanaiya
double get_sample_size(int insp_num); //lanaiya
void add_inspection(int insp_num, int pn, int insp_qty, std::string insp_area); //brandon

/* -------------------------------------- APPROVE INSPECTION SQL FUNCTIONS ---------------------------------------------- */
void approve_inspection(int insp_num); //still used?
int get_qty_inspected(int insp_num); //lanaiya
int calculate_fpy(int insp_num); //lanaiya

void set_insp_pf(int insp_num, bool pf); //lanaiya
void update_inspection_requirements(int insp_num,int id, std::string time, std::string insp_req, std::string insp_res, int qty_pass); //brandon
std::vector<std::string> get_insp_req(int insp_num);
std::vector<std::string> get_insp_res_type(int insp_num);
std::string get_ir_desc(int insp_num);
int get_insp_pnum(int insp_num);
void set_insp_pf(std::string); //lanaiya

//SQL Queries for PART
//int get_next_mat_num();
void add_part(std::string pdesc, int type);
void edit_part_name(int pnum, std::string pdesc);
bool part_exists(int pnum);                       //check if material is already present in material 
int get_part_id(std::string pname);               //get material id from a user's string input - returns 0 if none found
int get_part_type(int pnum);
std::string get_part_name(int pnum);

//SQL Queries for PART_SUPPLIER 
void add_part_supplier(int pnum, int supp_num, std::string supp_name);
void edit_part_supplier(int pnum, int supp_num, std::string supp_name);
int get_parts_needed(int pnum,int mat_num);  //returns qty of material needed for a product

//SQL Queries for PART_LOCATION
void add_part_location(int pnum, int INSP, int STORES, int WIP, int QC, int FGI);     //add tuple to part locations 
void edit_part_location(int pnum, int INSP, int STORES, int WIP, int QC, int FGI);
bool part_loc_exists(int pnum);                   //check if a material is already present in material locations 

int get_insp_count(int pnum);
int get_stores_count(int pnum);         //returns values in each location for a part
int get_wip_count(int pnum);
int get_qc_count(int pnum);
int get_fgi_count(int pnum);

//SQL Queries for PART_LISt
void add_kit(int pr_num,std::vector<int>&mat_list,std::vector<int>&qty);            //add a part list for a product and its materials, reference material id and quantity through passed arrays
int get_mat_quantity(int pr_num,int mat_num);

//SQL Queries for Creating Inspection Requirements
void create_requirements(int pnum, std::string requirement, std::string insp_area,std::string res_type);
void create_insp_req(int pnum, std::string pdesc,int sample_size);

/* ------------------------------------ MOVE INSP TO FGI SQL FUNCTIONS ------------------------------------------------ */
void move_INSP_to_STORES(int p_num, int qty); //lanaiya
void move_STORES_to_WIP(int p_num, int qty); //lanaiya
void move_WIP_to_QC(int p_num, int qty); //lanaiya
void move_QC_to_FGI(int p_num, int qty); //lanaiya

//ACCESSORS AND MODIFIERS FOR SETTING INDIVIDUAL ATTRIBUTES
void set_string_attribute(std::string table, std::string attr, std::string table_value, std::string attr_value);
void set_bool_attribute(std::string table, std::string attr, std::string table_value, bool attr_value);
void set_int_attribute(std::string table, std::string attr, std::string table_value, int attr_value);
void set_double_attribute(std::string table, std::string attr, std::string table_value, double attr_value);

// SQL FUNCTIONS NO LONGER NEEDED


#endif