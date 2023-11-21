/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "misc_func.h"
int LOGIN_COUNT = 0;
int CREATE_COUNT = 0;

void initialize()
{
    //Initialize the database; defined in sql_func
    init();
}

void main_menu()
{
    int sel;
    //Main menu will be the default state of program
    while(true)
    {
        std::cout << "Welcome! Please select an option below:\n1)Log In \n2)Create Account\n3)Exit" << std::endl;
        std::cin >> sel;
        //Call login(), create_account, or exit() based on input
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
    //Increment global counter for logging
    LOGIN_COUNT++;

    //Log start of login process
    auto file_logger = spdlog::basic_logger_mt("login_" + std::to_string(LOGIN_COUNT),"../logfile.txt");
    file_logger->info("start");

    std::string user;
    std::cout << "Please enter your username: ";
    std::cin >> user;
    //Verify user exists in db
    while(!(user_exists(user)))
    {
        //If entered username does not exist, log event
        file_logger->info("invalid username input");

        //Offer option to retry or return to main menu
        std::cout << "User does not exist, please enter a valid username, or 'exit' to return: ";
        std::cin >> user;
        if(user == "Exit" || user == "exit")
        {
            //Log exit event
            file_logger->info("exit at username input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }
    //If user exists, move to password input
    std::cout << "Please enter your password: ";
    std::string pw;
    std::cin >> pw;
    SHA256 sha256;  //Hashing object to avoid saving password directly

    //Verify username and password combination is valid
    while(!(verify_user(user,sha256(pw))))
    {
        //If password input is incorrect, log event
        file_logger->info("invalid password input");

        //Offer option to retry or return to main menu
        std::cout << "Invalid password, try again or type 'exit' to return to the main menu: ";
        std::cin >> pw;
        if(user == "Exit" || user == "exit")
        {
            //Log exit event
            file_logger->info("exit at password input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }
    //Log successful login
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
    /*
     * Account creation process involves checking user's ssn to see if employee is already registered
     * If the ssn has not been registered, prompt user for username and password to create with ssn
     * SSN is saved as a string first for easier exit prompts
    */

    //Increment global counter of account creation for current session
    CREATE_COUNT++;

    //Log start of account creation process
    auto file_logger = spdlog::basic_logger_mt("create_account_" + std::to_string(CREATE_COUNT),"../logfile.txt");
    file_logger->info("start");
    int ssn;
    std::string ssn_str,name,user,pw;
    char temp;

    std::cout << "Enter your SSN: ";
    std::cin >> ssn_str;
    //Check if entered SSN is valid (9 digits)
    while (!valid_ssn(ssn_str))
    {
        //If ssn input is invalid, log event
        file_logger->info("invalid ssn input");
        std::cout << "Invalid SSN, input a new value, or type 'exit' to return to the main menu: ";
        std::cin >> ssn_str;
        //Offer option to retry or return to main menu
        if(user == "Exit" || user == "exit")
        {
            //Log exit event
            file_logger->info("exit at invalid ssn");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }
    // If ssn entered is valid, save an integer version
    ssn = std::stoi(ssn_str);

    //Check if SSN is registered in system
    while(ssn_exists(ssn))
    {
        //If SSN already exists, log event
        file_logger->info("duplicate ssn input");
        std::cout << "An account for this SSN already exists, try again or type 'exit' to return to the main menu: \n";
        std::cin >> ssn_str;

        //Offer option to retry to return to main menu
        if(ssn_str == "Exit" || ssn_str == "exit")
        {
            file_logger->info("exit at duplicate ssn input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }

        while(!(valid_ssn(ssn_str)))
        {
            //If invalid value entered, log event
            file_logger->info("invalid ssn input");
            std::cout << "Invalid SSN, input a new value, or type 'exit' to return to the main menu: ";
            std::cin >> ssn_str;

            //Offer option to retry or return to main menu
            if(ssn_str == "Exit" || ssn_str == "exit")
            {
                //Log exit event
                file_logger->info("exit at invalid ssn");
                std::cout << "Returning to main menu." << std::endl;
                return;
            }
        }
        //If entered value was not 'exit', check for validity, and if valid, save new value as integer for comparison
        ssn = std::stoi(ssn_str);
    }

    //If SSN is valid and doesn't exist, prompt user for: name, username, password
    std::cout << "Enter your name: ";
    std::cin >> name;

    std::cout << "Enter your desired username: ";
    std::cin >> user;

    //Check if username is taken
    while(user_exists(user))
    {
        //If username is already taken, log event
        file_logger->info("duplicate username input");
        std::cout << "Username has already been taken, please enter a new username." << std::endl;
        std::cout << "If you would like to cancel your account creation, type 'exit' " << std::endl;
        std::cin >> user;

        //Offer option to retry or return to main menu
        if(user == "Exit" || user == "exit")
        {
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }

    std::cout << "Enter your desired password: ";
    std::cin >> pw;
    SHA256 sha256;
    //Hash the password before passing to user creation method
    std::string hashed_pw = sha256(pw);

    create_user(ssn, name, user,hashed_pw);

    //Log successful creation
    file_logger->info("complete");
    std::cout << "Account successfully created!" << std::endl;

    //Return to main menu after completion
}

bool valid_ssn(std::string str)
{
    return str.length() == 9;
}

/* ----------------------------------------------------------------------------------------- */

void loginas_IQC_inspector()
{
    display_IQCinsp_menu();
}

void loginas_OQC_inspector()
{
    display_OQCinsp_menu();
}
/*
void loginas_technician()
{
    display_tech_menu();
}*/

void loginas_QAdirector()
{
    display_QAdir_menu();
}

void loginas_inv_associate()
{
    display_invassoc_menu();
}

/* ------------------------------------------------------------------------ */

//Display menu options for IQC inspections
void display_IQCinsp_menu()
{
    int option;
    
    while (true)
    {
        std::cout << "Choose an option below:\n";
        
        std::cout << "\t1. View Inventory\n";
        std::cout << "\t2. View IQC Inspections\n";
        std::cout << "\t3. Add inspection\n";
        std::cout << "\t4. Approve IQC Inspection\n";
        std::cout << "\t5. Send email for failed inspections\n";
        std::cout << "\t6. Logout\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 3)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view, add, or approve the IQC inspections
            switch (option)
            {
                case 1: //view inventory
                    std::cout << "view_inventory();\n";
                    break;
                    
                case 2: //view IQC inspections list
                    std::cout << "view_IQC_list();\n";
                    break;
                    
                case 3: //create new inspection
                    std::cout << "create_inspection()\n";
                    break;
                    
                case 4: //approve inspection
                    std::cout << "approve_inspection();\n";
                    break;
                    
                case 5: //send email on failed inspection
                    std::cout << "send_email();\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    
}

/* ------------------------------------------------------------------------ */

//Display menu options for OQC Inspector
void display_OQCinsp_menu()
{
    int option;
    
    while (true)
    {
        std::cout << "Choose an option below:\n";
        
        std::cout << "\t1. View Inventory\n";
        std::cout << "\t2. View OQC Inspections\n";
        std::cout << "\t3. Add OQC inspection\n";
        std::cout << "\t4. Approve OQC Inspection\n";
        std::cout << "\t5. Send email for failed inspections\n";
        std::cout << "\t6. Logout\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 3)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view, add, or approve the OQC inspections
            switch (option)
            {
                case 1: //view inventory
                    std::cout << "view_inventory();\n";
                    break;
                    
                case 2: //view OQC inspections list
                    std::cout << "view_OQC_list();\n";
                    break;
                    
                case 3: //create new inspection
                    std::cout << "create_inspection()\n";
                    break;
                    
                case 4: //approve inspection
                    std::cout << "approve_inspection();\n";
                    break;
                    
                case 5: //send email on failed inspection
                    std::cout << "send_email();\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    
}

/* ------------------------------------------------------------------------ */

//Display menu options for QA director
void display_QAdir_menu()
{
    int option;
    
    while (true)
    {
        std::cout << "Choose an option below:\n";
    
        std::cout << "\t1. Add material information to inventory\n";
        std::cout << "\t2. Add product information to inventory\n";
        std::cout << "\t3. View Final Inspections\n";
        std::cout << "\t4. Approve and certify final product\n";
        std::cout << "\t5. Logout\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 5)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view or approve final inspections
            switch (option)
            {
                case 1:
                    std::cout << "view_inventory();\n";
                    break;
                    
                case 2:
                    std::cout << "add_material();\n";
                    break;
                    
                case 3:
                    std::cout << "add_product();\n";
                    break;
                    
                case 4:
                    std::cout << "approve_inspection();\n";
                    break;
                case 5:
                    std::cout << "Goodbye!\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    
}

/* ------------------------------------------------------------------------ */

//Display menu options for inventory associate
void display_invassoc_menu()
{
    int option;
    
    while (true)
    {
        std::cout << "Choose an option below:\n";
    
        std::cout << "\t1. View inventory\n";
        std::cout << "\t2. Receive Material\n";
        std::cout << "\t3. Backflush product\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 3)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            switch (option)
            {
                case 1:
                    std::cout << "view_inventory();\n";
                    break;
                    
                case 2:
                    std::cout << "approve_inspection();\n";
                    break;
                    
                case 3: //logout
                    std::cout << "Goodbye!\n";      
                    goto exitwhileloop;
            };
            
        }
        
    }
    exitwhileloop:  ;
    
}

/* ------------------------------------------------------------------------ */

//Display menu options for technician
// void display_tech_menu();
// {
//     int option;
    
//     while (true)
//     {
//         std::cout << "Choose an option below:\n";
    
//         std::cout << "\t1. View Products in Progress\n";
//         std::cout << "\t2. View IPQC Inspection\n";
//         std::cout << "\t3. Create IPQC Inspection\n";
//         std::cout << "\t4. Approve IPQC Inspection\n";
//         std::cout << "\t5. Logout\n";
    
//         std::cin >> option;
        
//         if (option < 1 | option > 3)
//         {
//             std::cout << "Option is not valid. Please try again.\n";
//         }
//         else
//         {
//             //view products, perform and approve inspections
//             switch (option)
//             {
//                 case 1:
//                     std::cout << "view_inventory();\n";
//                     break;
                    
//                 case 2:
//                     std::cout << "view_IPQC_list();\n";
//                     break;
                
//                 case 3:
//                     std::cout << "add_inspection();\n";
//                     break;
                    
//                 case 4:
//                     std::cout << "approve_inspection();\n";
//                     break;
                    
//                 case 5:
//                     std::cout << "Goodbye!\n";
//                     goto exitwhileloop;
//             }
            
//         }
        
//     }
//     exitwhileloop:  ;
    
// }