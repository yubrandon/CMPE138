#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#include <iostream>
#include "sql_func.h"


//functions not directly related to querying or sql
//will add werkzeug header for hashing passwords

void main_menu();   //will change to a state based system so that users can move between menu states freely
void login();
void create_account();
bool valid_ssn(int ssn);
bool check_ssn(int ssn);


/*
functions to show menu for each entity type


*/

#endif