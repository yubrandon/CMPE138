#include <iostream>
#include <stdlib.h>

#include "sql_func.h"

int main(void)
{
    
    int sel;
    std::cout << "Welcome! Please select an option below:\n1)Log In \n2)Create Account\n3)Exit" << std::endl;
    std::cin >> sel;
    std::cout << "test" << std::endl;
    switch(sel)
    {
        case 1: 
            //login()
            break;
        case 2:
            //create_account()
            break;
        case 3:
            std::cout << "Goodbye." << std::endl;
            exit(0);
            break;
        default:std::cout << "Error: Please select a valid option." << std::endl;
    }

    return 0;
}