/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "misc_func.h"
int LOGIN_COUNT = 0;
int CREATE_COUNT = 0;

void initialize()
{
    //include sql db intialization here


}

void main_menu()
{
    int sel;
    while(true)
    {
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
                std::cout << "Exited." << std::endl;
                exit(0);
                break;
            default:std::cout << "Error: Please select a valid option." << std::endl;
        }
    }
}
void login()
{
    LOGIN_COUNT++;
    auto file_logger = spdlog::basic_logger_mt("login_" + std::to_string(LOGIN_COUNT),"../logfile.txt");
    file_logger->info("start");

    std::string user;
    std::cout << "Please enter your username: ";
    std::cin >> user;
    //verify user exists in db
    while(!(user_exists(user)))
    {
        file_logger->info("invalid username input");
        std::cout << "User does not exist, please enter a valid username, or 'exit' to return: ";
        std::cin >> user;
        if(user == "Exit" || user == "exit")
        {
            file_logger->info("exit at username input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }

    std::cout << "Please enter your password: ";
    std::string pw;
    std::cin >> pw;
    SHA256 sha256;

    while(!(verify_user(user,sha256(pw))))
    {
        file_logger->info("invalid password input");
        std::cout << "Invalid password, try again or type 'exit' to return to the main menu: ";
        std::cin >> pw;
        if(user == "Exit" || user == "exit")
        {
            file_logger->info("exit at password input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }
    file_logger->info("complete");
    std::cout << "Successful login!" << std::endl;
    /*
    call function that displays menus based on entity
        use query to obtain user's role
        create function to return flag for what they can access
     
    
    */

}
void create_account()
{
    CREATE_COUNT++;
    auto file_logger = spdlog::basic_logger_mt("create_account_" + std::to_string(CREATE_COUNT),"../logfile.txt");
    file_logger->info("start");
    int ssn;
    std::string ssn_str,name,user,pw;
    char temp;

    std::cout << "Enter your SSN: ";
    std::cin >> ssn_str;
    while (!valid_ssn(ssn_str))
    {
        file_logger->info("invalid ssn input");
        std::cout << "Invalid SSN, input a new value, or type 'exit' to return to the main menu: ";
        std::cin >> ssn_str;
        if(user == "Exit" || user == "exit")
        {
            file_logger->info("exit at invalid ssn");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }

    ssn = std::stoi(ssn_str);

    while(ssn_exists(ssn))
    {
        file_logger->info("duplicate ssn input");
        std::cout << "An account for this SSN already exists, try again or type 'exit' to return to the main menu: \n";
        std::cin >> ssn_str;
        if(ssn_str == "Exit" || ssn_str == "exit")
        {
            file_logger->info("exit at duplicate ssn input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
        ssn = std::stoi(ssn_str);
    }
    std::cout << "Enter your name: ";
    std::cin >> name;

    std::cout << "Enter your desired username: ";
    std::cin >> user;
    while(user_exists(user))
    {
        file_logger->info("duplicate username input");
        std::cout << "Username has already been taken, please enter a new username." << std::endl;
        std::cout << "If you would like to cancel your account creation, type 'exit' " << std::endl;
        std::cin >> user;
        if(user == "Exit" || user == "exit")
        {
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }

    std::cout << "Enter your desired password: ";
    std::cin >> pw;
    SHA256 sha256;
    
    std::string hashed_pw = sha256(pw);

    create_user(ssn, name, user,hashed_pw);
    file_logger->info("complete");
    std::cout << "Account successfully created!" << std::endl;

    //returns to main menu after completion
}

bool valid_ssn(std::string str)
{
    return str.length() == 9;
}