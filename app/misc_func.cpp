/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "misc_func.h"
#include "sql_func.h"

//Global variables
User *currUser = new User;
int LOGIN_COUNT = 0;
int CREATE_COUNT = 0;

void user_test()
{
    std::cout << currUser -> id << std::endl;
    std::cout << currUser -> username << std::endl;
    std::cout << currUser -> ssn << std::endl;

}

void initialize()
{
    //Initialize the database
    db_init();
    //state_init();
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
                delete currUser;
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
    //Save credentials of current user
    currUser -> username = user;
    get_user(currUser);

    //Log successful login
    file_logger->info("complete\n");
    std::cout << "Successful login!" << std::endl;
    /*
    call function that displays menus based on entity
        use query to obtain user's role
        create function to return flag for what they can access
     
    
    */
   //prints out user id,username,ssn - testing
   user_test();

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
    std::string ssn_str,name,user,pw,lname,fname;
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

    //Check if SSN is registered in system
    while(ssn_exists(ssn_str))
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
    }

    //If SSN is valid and doesn't exist, prompt user for: username, password, name
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

    std::cout << "Enter your first name: ";
    std::cin >> fname;

    std::cout << "Enter your last name: ";
    std::cin >> lname;

    create_user(ssn_str, name, user,hashed_pw,lname,fname);

    //Log successful creation
    file_logger->info("complete\n");
    std::cout << "Account successfully created!" << std::endl;

    //Return to main menu after completion
}

bool valid_ssn(std::string str)
{
    return str.length() == 9;
}


/* ----------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------
 * Logins for each type of employee
 * login as IQC inspector
 * login as OQC inspector
 * login as QA director
 * login as Inventory Associate
   ---------------------------------------------------------------------- */

void loginas_IQC_inspector()
{
    display_IQCinsp_menu();
}

void loginas_OQC_inspector()
{
    display_OQCinsp_menu();
}

void loginas_QAdirector()
{
    display_QAdir_menu();
}

void loginas_inv_associate()
{
    display_invassoc_menu();
}

void loginas_technician()
{
    display_tech_menu();
}

/* ------------------------------------------------------------------------
 * Menu viewing options for the various inspector types
 * - IQC inspector - inspects incoming material
 * - OQC inspector - inspects final product shipping
 * - QA director - approves final product inspections
 * - inventory associate - receives material, kits it for production
 * - technician - builds and performs in process inspections
   ---------------------------------------------------------------------- */
   
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
        
        if (option < 1 | option > 6)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view, add, or approve the IQC inspections
            switch (option)
            {
                case 1: //view inventory
                    view_all_inventory();
                    break;
                    
                case 2: //view IQC inspections list
                    view_IQC_inspections();
                    break;
                    
                case 3: //add new inspection
                    add_new_inspections("Quality", "IQC Inspector");
                    break;
                    
                case 4: //approve inspection
                    approve_inspection("Quality", "IQC Inspector");
                    break;
                    
                case 5: //send email on failed inspection
                    send_email("Quality", "IQC Inspector");
                    break;
                
                case 6: //logout
                    std::cout << "Goodbye!\n";
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
        
        if (option < 1 | option > 6)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view, add, or approve the OQC inspections
            switch (option)
            {
                case 1: //view inventory
                    view_all_inventory();
                    break;
                    
                case 2: //view IQC inspections list
                    view_OQC_inspections();
                    break;
                    
                case 3: //add new inspection
                    add_new_inspections("Quality", "OQC Inspector");
                    break;
                    
                case 4: //approve inspection
                    approve_inspection("Quality", "OQC Inspector");
                    break;
                    
                case 5: //send email on failed inspection
                    send_email("Quality", "OQC Inspector");
                    break;
                
                case 6: //logout
                    std::cout << "Goodbye!\n";
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
    
        std::cout << "\t1. View inventory\n";
        std::cout << "\t2. Add part information to inventory\n";
        std::cout << "\t3. Add product information to inventory\n";
        std::cout << "\t4. Create inspection requirement\n";
        std::cout << "\t5. View Final Inspections\n";
        std::cout << "\t6. Approve and certify final product\n";
        std::cout << "\t7. Logout\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 7)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view or approve final inspections
            switch (option)
            {
                case 1:
                    view_all_inventory();
                    break;
                    
                case 2:
                    add_part_menu();
                    break;
                    
                case 3:
                    add_product();
                    break;
                    
                case 4:
                    create_inspection_requirements();
                    break;
                    
                case 5:
                    view_final_product_inspections();
                    break;
                    
                case 6:
                    approve_inspection("Quality", "QA director");
                    break;
                case 7:
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
        std::cout << "\t3. Pull Work Order\n";
        std::cout << "\t4. Backflush product\n";
        std::cout << "\t5. Logout\n";
    
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
                    view_all_inventory();
                    break;
                    
                case 2:
                    receive_material();
                    break;
                    
                case 3:
                    pull_wo();
                    break;
                    
                case 4:
                    backflush_product();
                    break;
                    
                case 5: //logout
                    std::cout << "Goodbye!\n";      
                    goto exitwhileloop;
            };
            
        }
        
    }
    exitwhileloop:  ;
    
}

/* ------------------------------------------------------------------------ */

//Display menu options for technician
void display_tech_menu()
{
    int option;
    
    while (true)
    {
        std::cout << "Choose an option below:\n";
    
        std::cout << "\t1. View Products in Progress\n";
        std::cout << "\t2. View IPQC Inspection\n";
        std::cout << "\t3. Create IPQC Inspection\n";
        std::cout << "\t4. Approve IPQC Inspection\n";
        std::cout << "\t5. Logout\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 3)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view products, perform and approve inspections
            switch (option)
            {
                case 1:
                    std::cout << "view_inventory();\n";
                    break;
                    
                case 2:
                    std::cout << "view_IPQC_list();\n";
                    break;
                
                case 3:
                    std::cout << "add_inspection();\n";
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



void view_all_inventory()
{
    std::cout << "Viewing all inventory...\n...\n...\n";
}

void view_IQC_inspections()
{
    std::cout << "Running query view_inspections...\n";
    view_inspections("Quality", "IQC inspector");
    //considering removing c++ func and adding sql func directly into switch-case
}

void view_OQC_inspections()
{
    std::cout << "Running query view_inspections...\n";
    view_inspections("Quality", "OQC inspector");
    //considering removing c++ func and adding sql func directly into switch-case
}

void view_final_product_inspections()
{
    std::cout << "Running query view_inspections...\n";
    view_inspections("Quality", "OA Director");
    //considering removing c++ func and adding sql func directly into switch-case
}

void add_new_inspections(std::string dept_name, std::string title)
{
    int account, pn, insp_qty, insp_num;
    int choose_approve, lot_qty;
    double sample_size;
    std::string insp_area;
    
    //query to get next number for inspection
    insp_num = get_next_insp_num();
    
    //query to return the account that is currently logged in
    account = get_account();
    
    switch (account)
    {
        case 1: case 2: // IQC Inspector
            std::cout << "Enter the material part number: ";
            break;
            
        case 3: case 4: // QA Director
            std::cout << "Enter the product part number: ";
            break;

    }    
    
    std::cin >> pn;
    
    //cin qty_inspected
    std::cout << "Enter the total lot quantity: ";
    std::cin >> lot_qty;
    
    //query to pull sample_size for total quantity needed to pass
    sample_size = get_sample_size(insp_num);
    insp_qty = lot_qty * sample_size;
    
    std::cout << "Adding inspection " << insp_num << " to inspections list...\n";
    add_inspection(insp_num, pn, insp_qty, insp_area);
    
    std::cout << "Inspection has been added to inspections list!";
    
}

void approve_inspection(std::string dept_name, std::string title)
{
    int insp_num;
    int qty_passed;
    
        //get today's date
    time_t now = time(0);
    char *date = ctime(&now);
    
    if (dept_name == "Quality")
    {
        if (title == "IQC Inspector")
        {
            std::cout << "Opening IQC Inspections...\n";
            view_inspections(dept_name, title);
        }
        else if (title == "OQC Inspector")
        {
            std::cout << "Opening OQC Inspections...\n";
            view_inspections(dept_name, title);
        }
        else if (title == "QA Director")
        {
            std::cout << "Opening Finished Product Inspections...\n";
            view_inspections(dept_name, title);
        }
    }
    else if (dept_name == "Manufacturing")
    {
        std::cout << "Opening In Process Inspections...\n";
        view_inspections(dept_name);
    }
    
    std::cout << "Choose an inspection part number to approve: ";
    std::cin >> insp_num;
    
    //cout requirements and add cin for each
    //      insp_res, qty_passed 
    view_inspection_requirements(insp_num);
    
    //calculate to ensure all qty_passed == qty_inspected
    qty_passed = calculate_fpy(insp_num);
    
    //query to get qty_inspected

    if (qty_passed == get_qty_inspected(insp_num))
    {
        set_insp_pf("pass");
        std::cout << "Lot Quantity Passed! Inspection " << insp_num << " Complete.\n";
    }
    else
    {
        set_insp_pf("fail");
        std::cout << "Lot Quantity Failed. Sending email describing failed lot...\n";
        send_email(insp_num);
    }
}

void send_email(std::string dept_name, std::string title)
{
    
}

void send_email(int insp_num)
{
    
}

void add_part_menu()
{
    int supp_num;
    std::string part_desc, supp_name,product;
    
    
    std::cout << "Enter the new part description: ";
    std::cin >> part_desc;
    
    std::cout << "Enter the supplier name: ";
    std::cin >> supp_name;
    
    std::cout << "Enter the supplier's part number: ";
    std:: cin >> supp_num;
    
    std::cout << "Is this item a product? (y/n)";
    std::cin >> product;

    for(char &c : product)
    {
        c = std::tolower(c);
    }

    while(product != "y" && product != "n")
    {
        std::cout << "Invalid entry, please try again, or enter 'exit' to return: ";
        std::cin >> product;
        for(char &c : product)
        {
            c = std::tolower(c);
        }
        if(product == "exit")
        {
            //Log exit event
            //file_logger->info("exit at username input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }
    int product_int;
    if(product == "y") product_int = 1;
    else product_int = 0;
    
    //query to add information to Materials table
    std::cout << "Adding part to inventory list...";
    add_part(part_desc,product_int);



    std::cout << "Part added!\n";
}

void create_inspection_requirements()
{
    std::string pn, sample_size, mat_prod_choice;
    int mat_prod_num;
    
    std::cout << "Is this part a material or product? ";
    std::cin >> mat_prod_choice;
    
    
    std::cout << "Enter part number: ";
    std::cin >> pn;
    mat_prod_num = std::stoi(pn);
    
    //query to verify part number exists
    if (!part_exists(mat_prod_num))
    {
        std::cout << "This part number does not exist.";
    }
    
    else
    {
        // query to get description of part description
        if (mat_prod_choice == "material")
        {
            //query to get description from material table
            
            //query to set insp_area based on IQC or OQC
        }
        else if (mat_prod_choice == "product")
        {
            //query to get description from product table
            
            //query to set insp_area based on IPQC or OQC
        }
        
        //get sample size from user
        std::cout << "Enter sample size (0.00 - 1.00): ";
        std::cin >> sample_size;
        
        //query to insert into INSP_REQ
        
        //query to insert into INSP_REQ_AREA
        
        //while loop to add requirements for each part number
        bool add_more = true;
        std::string requirement, res_type, option;
        int choice;
        
        while (add_more)
        {
            
            std::cout << "Enter the requirement: ";
            std::cin >> requirement;
            
            std::cout << "Enter the result type: ";
            std::cin >> res_type;
            
            //query to add insp_PN, requirement desc, requirement result type
            create_requirements(mat_prod_num, requirement, res_type);
            
            
            std::cout << "Would you like to add another requirement to this inspection?\n";
            std::cout << "\t1. Yes\n";
            std::cout << "\t2. No\n";
            
            std::cin >> option;
            choice = std::stoi(option);
            
            if (choice == 2)
            {
                add_more = false;
                std::cout << "Requirement added to part!";
            }
        }
        
    }
    
    
}

//inventory associatae
void receive_material()
{
    //add material into stores
}

void pull_wo()
{
    //view bom
    //subtract mats from mat locations and add to wip
    //ask how many products to build
}

void backflush_product()
{
    //remove wip for a product and add to fgi
}
