#ifndef SQL_FUNC2_H
#define SQL_FUNC2_H

#include <iostream>
#include "mysql_connection.h"

#include <cppcon/driver.h>
#include <cppcon/exception.h>
#include <cppcon/resultset.h>
#include <cppcon/statement.h>
#include <cppcon/prepared_statement.h>

//SQL Query Functions
void view_IQC_list();
void create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size);
void approve_inspection(int insp_num);
void view_inspection(int insp_num);
void move_to_IPQC(int pn);

void receive_material(int pn);
void backflush_product(int pn);
void move_to_IQC(int pn);


#endif