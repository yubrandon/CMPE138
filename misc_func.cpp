#include "misc_func.h"


void main_menu()
{
    int sel;
    std::cout << "Welcome! Please select an option below:\n1)Log In \n2)Create Account\n3)Exit" << std::endl;
    std::cin >> sel;
    switch(sel)
    {
        case 1: 
            login();
            break;
        case 2:
            create_account();
            break;
        case 3:
            std::cout << "Goodbye." << std::endl;
            exit(0);
            break;
        default:std::cout << "Error: Please select a valid option." << std::endl;
    }
}
void login()
{
    std::string user;
    std::cout << "Please enter your username: ";
    std::cin >> user;
    //verify user exists in db
    while(!(user_exists(user)))
    {
        std::cout << "Invalid username, please enter a valid username, or 'Exit' to return:";
        std::cin >> user;
        if(user == "Exit" || user == "exit")
        {
            std::cout << "Successfully exited." << std::endl;
            exit(0);
        }
    }
    
    std::cout << "Please enter your password: ";
    std::string pw;
    std::cin >> pw;

    //verify login
    //while(!password doenst match)  ...

    /*
    call function that displays menus based on entity
        use query to obtain user's role
        create function to return flag for what they can access
     
    
    */
   std::cout << "Completed login." << std::endl;

}
void create_account()
{
    int ssn;

    std::cout << "Enter your SSN: ";
    std::cin >> ssn;
    while (!valid_ssn(ssn))
    {
        std::cout << "Invalid SSN, please input a new value: ";
        std::cin >> ssn;
    }
    
    // valid ssn entered -> check db to see if it already exists -> return boolean
    /*
    if (already exists) throw message -?> give option to go back or continue
    else 
    {
        string name, password, etc.
        create query to insert to database
            ask necessary questions for other data
        indicate completed account creation
        return to login
    }
    */



}

bool valid_ssn(int ssn)
{
    return std::to_string(ssn).length() == 9;
}

bool check_ssn(int ssn)
{

}