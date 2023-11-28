/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "misc_func.h"
#include "sql_func.h"

//Global variables
User *currUser = new User;
int LOGIN_COUNT = 0;
int CREATE_COUNT = 0;
int IQC_SESSION_COUNT = 0;
int OQC_SESSION_COUNT = 0;
int QA_SESSION_COUNT = 0;
int INV_ASSOC_SESSION_COUNT = 0;
int TECH_SESSION_COUNT = 0;

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
        std::string exitcheck = tolowerstring(user);
        if(exitcheck == "exit")
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
        std::string exitcheck = tolowerstring(pw);
        if(exitcheck == "exit")
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
    
    std::string job = tolowerstring(currUser->job_title);
    if(job == "iqc inspector") loginas_IQC_inspector();
    else if(job == "oqc inspector") loginas_OQC_inspector();
    else if(job == "qa director") loginas_QAdirector();
    else if(job == "inventory associate") loginas_inv_associate();
    else if(job == "technician") loginas_technician();

    //user_test();

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
        
        std::string exitcheck = tolowerstring(ssn_str);
        if(exitcheck == "exit")
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
        std::string exitcheck = tolowerstring(ssn_str);
        if(exitcheck == "exit")
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
            exitcheck = tolowerstring(ssn_str);
            if(exitcheck == "exit")
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

        std::string exitcheck = tolowerstring(user);
        if(exitcheck == "exit")
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

    //Returns to main menu after completion
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
    IQC_SESSION_COUNT++;
    int option;
    auto file_logger = spdlog::basic_logger_mt("User: " + currUser->username + " (IQC)" + "session_" + std::to_string(IQC_SESSION_COUNT),"../logfile.txt");
    file_logger->info("start");
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
                    file_logger->info("viewed inventory");
                    view_all_inventory();
                    break;
                    
                case 2: //view IQC inspections list
                    file_logger->info("viewed inspections");
                    view_IQC_inspections();
                    break;
                    
                case 3: //add new inspection
                    file_logger->info("added inspection");
                    add_new_inspections("Quality", "IQC Inspector");
                    break;
                    
                case 4: //approve inspection
                    file_logger->info("approved inspection");
                    approve_inspection("Quality", "IQC Inspector");
                    break;
                    
                case 5: //send email on failed inspection
                    file_logger->info("sent email");  
                    send_email("Quality", "IQC Inspector");
                    break;
                
                case 6: //logout
                    std::cout << "Goodbye!\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    file_logger->info("end");
}

/* ------------------------------------------------------------------------ */

//Display menu options for OQC Inspector
void display_OQCinsp_menu()
{
    OQC_SESSION_COUNT++;
    int option;
    auto file_logger = spdlog::basic_logger_mt("User: " + currUser->username + " (OQC)" + "session_" + std::to_string(OQC_SESSION_COUNT),"../logfile.txt");
    file_logger->info("start");
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
                    file_logger->info("viewed inventory");
                    view_all_inventory();
                    break;
                    
                case 2: //view IQC inspections list
                    file_logger->info("viewed inspections");
                    view_OQC_inspections();
                    break;
                    
                case 3: //add new inspection
                    file_logger->info("added inspection");
                    add_new_inspections("Quality", "OQC Inspector");
                    break;
                    
                case 4: //approve inspection
                    file_logger->info("approved inspection");
                    approve_inspection("Quality", "OQC Inspector");
                    break;
                    
                case 5: //send email on failed inspection
                    file_logger->info("sent email");
                    send_email("Quality", "OQC Inspector");
                    break;
                
                case 6: //logout
                    std::cout << "Goodbye!\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    file_logger->info("end");
}

/* ------------------------------------------------------------------------ */

//Display menu options for QA director
void display_QAdir_menu()
{
    QA_SESSION_COUNT++;
    int option;
    auto file_logger = spdlog::basic_logger_mt("User: " + currUser->username + " (QA)" + "session_" + std::to_string(QA_SESSION_COUNT),"../logfile.txt");
    file_logger->info("start");
    while (true)
    {
        std::cout << "Choose an option below:\n";
    
        std::cout << "\t1. View inventory\n";
        std::cout << "\t2. Add part information to inventory\n";
        std::cout << "\t3. Create inspection requirement\n";
        std::cout << "\t4. View Final Inspections\n";
        std::cout << "\t5. Approve and certify final product\n";
        std::cout << "\t6. Logout\n";
    
        std::cin >> option;

        //view or approve final inspections
        switch (option)
        {
            case 1:
                file_logger->info("viewed inventory");
                view_all_inventory();
                break;
                
            case 2:
                file_logger->info("adding part");
                add_part_menu();
                break;
                
            case 3:
                file_logger->info("created inspection requirement");
                create_inspection_requirements();
                break;
                
            case 4:
                file_logger->info("viewed inspections");
                view_final_product_inspections();
                break;
                
            case 5:
                file_logger->info("approved inspection");
                approve_inspection("Quality", "QA director");
                break;
            case 6:
                std::cout << "Goodbye!\n";
                goto exitwhileloop;
            default: std::cout << "Option is not valid. Please try again.\n";
        }
        
    }
    exitwhileloop:  ;
    file_logger->info("end");
}

/* ------------------------------------------------------------------------ */

//Display menu options for inventory associate
void display_invassoc_menu()
{
    INV_ASSOC_SESSION_COUNT++;
    int option;
    auto file_logger = spdlog::basic_logger_mt("User: " + currUser->username + " (IQC)" + "session_" + std::to_string(INV_ASSOC_SESSION_COUNT),"../logfile.txt");
    file_logger->info("start");
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
                    file_logger->info("viewed inventory");
                    view_all_inventory();
                    break;
                    
                case 2:
                    file_logger->info("received part");
                    receive_part_menu();
                    break;
                    
                case 3:
                    file_logger->info("pulled work order");
                    pull_wo_menu();
                    break;
                    
                case 4:
                    file_logger->info("backflushed product");
                    backflush_product();
                    break;
                    
                case 5: //logout
                    std::cout << "Goodbye!\n";      
                    goto exitwhileloop;
            };
            
        }
        
    }
    exitwhileloop:  ;
    file_logger->info("end");
}

/* ------------------------------------------------------------------------ */

//Display menu options for technician
void display_tech_menu()
{
    TECH_SESSION_COUNT++;
    int option;
    auto file_logger = spdlog::basic_logger_mt("User: " + currUser->username + " (IQC)" + "session_" + std::to_string(TECH_SESSION_COUNT),"../logfile.txt");
    file_logger->info("start");
    while (true)
    {
        std::cout << "Choose an option below:\n";
    
        std::cout << "\t1. View Products in Progress\n";
        std::cout << "\t2. View IPQC Inspection\n";
        std::cout << "\t3. Create IPQC Inspection\n";
        std::cout << "\t4. Approve IPQC Inspection\n";
        std::cout << "\t5. Logout\n";
    
        std::cin >> option;
        
        if (option < 1 | option > 5)
        {
            std::cout << "Option is not valid. Please try again.\n";
        }
        else
        {
            //view products, perform and approve inspections
            switch (option)
            {
                case 1:
                    file_logger->info("viewed inventory");
                    //std::cout << "view_inventory();\n";
                    view_all_inventory();
                    break;
                    
                case 2:
                file_logger->info("viewed inspections");
                    std::cout << "view_IPQC_list();\n";
                    break;
                
                case 3:
                file_logger->info("added inspection");
                    std::cout << "add_inspection();\n";
                    break;
                    
                case 4:
                file_logger->info("approved inspection");
                    std::cout << "approve_inspection();\n";
                    break;
                    
                case 5:
                    std::cout << "Goodbye!\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    file_logger->info("end");
}



void view_all_inventory()
{
    std::cout << "Displaying entire inventory.\n";
    get_inventory_all();
}

void view_IQC_inspections()
{
    std::cout << "Running query view_inspections...\n";
    //view_inspection("Quality", "IQC inspector");
    //considering removing c++ func and adding sql func directly into switch-case
}

void view_OQC_inspections()
{
    std::cout << "Running query view_inspections...\n";
    //view_inspection("Quality", "OQC inspector");
    //considering removing c++ func and adding sql func directly into switch-case
}

void view_final_product_inspections()
{
    std::cout << "Running query view_inspections...\n";
    //view_inspection("Quality", "OA Director");
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
            //view_inspection(dept_name, title);
        }
        else if (title == "OQC Inspector")
        {
            std::cout << "Opening OQC Inspections...\n";
            //view_inspection(dept_name, title);
        }
        else if (title == "QA Director")
        {
            std::cout << "Opening Finished Product Inspections...\n";
            //view_inspection(dept_name, title);
        }
    }
    else if (dept_name == "Manufacturing")
    {
        std::cout << "Opening In Process Inspections...\n";
        //view_inspection(dept_name);
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
    std::string part_desc, supp_name,item;
    
    
    std::cout << "Enter the new part description: ";
    std::cin >> part_desc;
    
    std::cout << "Enter the supplier name: ";
    std::cin >> supp_name;
    
    std::cout << "Enter the supplier's part number: ";
    std:: cin >> supp_num;
    
    std::cout << "Is this item a material, product, or accessory? ";
    std::cin >> item;
    

    item = tolowerstring(item);
    while(item != "material" && item != "product" && item != "accessory")
    {
        std::cout << "Invalid input, please re-enter: ";
        std::cin >> item;
        item = tolowerstring(item);
    }

    int sel;
    if(item == "material") sel = 0;
    else if (item == "product") sel = 1;
    else sel = 2;

    //query to add information to Materials table
    std::cout << "Adding part to inventory list...";
    add_part(tolowerstring(part_desc),sel);
    add_part_supplier(get_part_id(tolowerstring(part_desc)),supp_num,supp_name);
    add_part_location(get_part_id(tolowerstring(part_desc)),0,0,0,0,0);

    std::cout << "Part added!\n";
}

void create_inspection_requirements()
{
    std::string pn, mat_prod_choice,insp_area;
    int mat_prod_num;
    float sample_size;
    
    std::cout << "Is this part a material or product? ";
    std::cin >> mat_prod_choice;
    
    
    std::cout << "Enter a part number or name: ";
    std::cin >> pn;

    recheck:;
    mat_prod_num = std::stoi(pn);
    
    //query to verify part number exists
    if (!part_exists(mat_prod_num))
    {
        std::cout << "This part number does not exist. Enter a new value or type 'exit' to return to the menu: ";
        std::cin >> pn;
        if(tolowerstring(pn) == "exit") return;
        else goto recheck;
    }
    
    else
    {
        // query to get description of part description
        if (get_part_type(mat_prod_num) == 0)
        {
            //query to get description from material table
            get_inventory(mat_prod_num);
            
            //query to set insp_area based on IQC or OQC
            int sel;
            std::cout << "Select (1)IQC or (2)OQC for this inspection.\n";
            std::cin>>sel;
            while(true)
            {
                switch(sel)
                {
                    case 0:
                        std::cout << "Exiting to menu.\n";
                        return;
                    case 1: 
                        insp_area = "IQC";
                        goto cont;
                    case 2:
                        insp_area = "OQC";
                        goto cont;
                    default:
                        std::cout << "Invalid input, please select a valid option, or input 0 to leave.\n";

                }
            }
        }
        else if (get_part_type(mat_prod_num) == 1)
        {
            //query to get description from product table
            get_inventory(mat_prod_num);
            
            //query to set insp_area based on IPQC or OQC
            int sel;
            std::cout << "Select (1)IPQC or (2)OQC for this inspection.\n";
            std::cin>>sel;
            while(true)
            {
                switch(sel)
                {
                    case 0:
                        std::cout << "Exiting to menu.\n";
                        return;
                    case 1: 
                        insp_area = "IPQC";
                        goto cont;
                    case 2:
                        insp_area = "OQC";
                        goto cont;
                    default:
                        std::cout << "Invalid input, please select a valid option, or input 0 to leave.\n";

                }
            }
        }
        cont:;
        //get sample size from user
        std::cout << "Enter sample size (0.00 - 1.00): ";
        std::cin >> sample_size;
        
        //query to insert into INSP_REQ
        create_insp_req(mat_prod_num,get_part_name(mat_prod_num),sample_size);
        
        
        //while loop to add requirements for each part number
        bool add_more = true;
        std::string requirement, res_type, option;
        std::string choice;
        
        while (add_more)
        {
            
            std::cout << "Enter the requirement: ";
            std::cin >> requirement;
            
            std::cout << "Enter the result type: ";
            std::cin >> res_type;
            
            //query to add insp_PN, requirement desc, requirement result type
            create_requirements(mat_prod_num, requirement,insp_area, res_type);
            
            
            std::cout << "Would you like to add another requirement to this inspection? (Y/N)\n";
            resel:;
            std::cin >> option;
            choice = tolowerstring(option);
            while(choice != "y" && choice != "n")
            {
                std::cout << "Please enter a valid input (Y/N): ";
                goto resel;
            }
            
            if (choice == "n")
            {
                add_more = false;
                std::cout << "Requirement added to part!";
            }
        }
        
    }
    
    
}

//inventory associatae
void receive_part_menu()
{
    std::string pdesc;
    std::cout << "Enter the name of the part that is being received: ";
    std::cin >> pdesc;



    while(!(part_exists(get_part_id(pdesc))))
    {
        std::cout << "That part does not exist in the system, please enter a new name, or 'exit' to return: ";
        std::cin >> pdesc;
        std::string exitcheck = tolowerstring(pdesc);
        if(exitcheck == "exit")
        {
            //Log exit event
            //file_logger->info("exit at username input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }

    int qty;
    std::cout << "What is the quantity being received? ";
    std::cin >> qty;

    int type = get_part_type(get_part_id(pdesc));
    switch(type)
    {
        case 0: 
            receive_material(get_part_id(pdesc),qty);
            std::cout << "Quantity of: " << qty << " successfully received! Returning to menu.";
            break;

        case 1:
            std::cout << "ERROR: Attempting to add product. Returning to menu.\n";
            break;

        case 2:
            receive_material_accessory(get_part_id(pdesc),qty);
            std::cout << "Quantity of: " << qty << " successfully received! Returning to menu.";
            break;

    }
}

void pull_wo_menu()
{
    std::string name;

    std::cout << "Enter the name of the product to see the BOM for: ";
    std::cin >> name;

    while(!(part_exists(get_part_id(name))) && get_part_type(get_part_id(name)) != 1)
    {
        std::cout << "Product not found. Please re-enter the name or type 'exit' to return: ";
        std::cin >> name;

        std::string exitcheck = tolowerstring(name);
        if(exitcheck == "exit")
        {
            //Log exit event
            //file_logger->info("exit at username input");
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }

    if(!part_loc_exists(get_part_id(name)))
    {
        std::cout << "ERROR: Part is not in storage. Returning to main menu.\n";
        return;
    }

    std::vector<int>mats = get_bom_id(get_part_id(name));

    for(int i = 0; i < mats.size(); i++)
    {
        if(!part_loc_exists(mats[i]))
        {
            std::cout << "Material: " << tolowerstring(name) << " is not available in storage, returning to main menu.\n";
            return;
        }
    }
    std::cout << "How many products would you like to order? ";
    int qty;
    std::cin >> qty;
    getquant: ;
    
    for(int i = 0; i < mats.size(); i++)
    {
        if((qty*get_mat_quantity(get_part_id(name),mats[i])) > get_stores_count(mats[i]))
        {
            std::cout << "ERROR: Insufficient quantity of: " << tolowerstring(get_part_name(mats[i])) << " available in stores.\n";
            std::string str;
            std::cout << "Please re-enter a value or type 'exit' to return to the menu: ";
            std::cin >> str;
            if(str == "exit")
            {
                //Log exit event
                //file_logger->info("exit at username input");
                std::cout << "Returning to main menu." << std::endl;
                return;
            }
            //add integer checking error handling
            qty = std::stoi(str);
            goto getquant;
        }
    }

    pull_wo(get_part_id(name),mats,qty);

    std::cout << "\nOrder successfully placed! Returning to main menu." << std::endl;
}

void backflush_product()
{
    std::string name;
    
    std::cout << "Enter the name of a product to backflush: ";
    std::cin>>name;
    while(!part_exists(get_part_id(name)) & get_part_type(get_part_id(name)) != 1)
    {
        std::cout << "Invalid product. Please re-enter a name, or type 'exit' to leave: ";
        std::cin>>name;
        std::string exitcheck = tolowerstring(name);
        if(exitcheck == "exit")
        {
               //Log exit event
                //file_logger->info("exit at username input");
                std::cout << "Returning to main menu." << std::endl;
                return;
        }
    }
    int product = get_part_id(name);
    std::vector<int>mats = get_bom_id(get_part_id(name));       //vector containing materials in order
    std::vector<int>count;                                      //vector containing the quantity for each material
    std::vector<int>max;        //vector containing maximum products that can be created with available material 
    for(int i = 0; i < mats.size();i++)
    {
        int qty = get_wip_count(mats[i]);
        if(qty == 0)
        {
            std::cout<< "Quantity for material '" << get_part_name(mats[i]) << "' is 0. Unable to backflush this product. Returning to main menu.\n";
            return;
        }
        count.push_back(qty);
        int n = get_parts_needed(product,mats[i])/qty;
        if(n == 0)
        {
            std::cout << "Insufficient material available to backflush this product. Returning to the main menu.\n";
            return;
        }
        max.push_back(get_parts_needed(product,mats[i])/qty);
    }

    int min = *std::min_element(max.begin(), max.end());
    std::string exitcheck;
    std::cout << min << " products can be backflushed. Would you like to continue? (Y/N)";
    std::cin >> exitcheck;

    while(tolowerstring(exitcheck) != "n" and tolowerstring(exitcheck) != "y")
    {
        std::cout << "Invalid input, please enter 'Y' to continue, or 'N' to return to the menu.\n";
        std::cin >> exitcheck;
        if(tolowerstring(exitcheck) == "n")
        {
            std::cout << "Process cancelled. Returning main menu.\n";
            return;
        }
        else if(tolowerstring(exitcheck) == "y") break;

    }

    int backflush;
    std::cout << "Please enter the desired quantity to backflush: ";
    std::cin>>backflush;
    while(backflush > min)
    {
        std::cout << "Invalid amount, please enter a new value: ";
        std::cin >> backflush;
    }
    backflush_product(product,backflush);
    std::cout<<"Order completed. Returning to main menu.\n";
    return;
}

std::string tolowerstring(std::string str)
{
    std::string strl = str;
    for(char &c : strl)
    {
        c = std::tolower(c);
    }
    return strl;
}

std::string get_user_insp_area()
{
    std::string temp = tolowerstring(currUser->job_title);
    if(temp == "iqc inspector") return "IQC";
    else if (temp == "oqc inspector") return "OQC";
    else if (temp == "qa director") return "FQC";
    else if (temp == "technician") return "IPQC";
}
