#include "misc_func2.h"

void loginas_IQC_inspector()
{
    display_IQCinsp_menu();
}

void loginas_OQC_inspector()
{
    display_OQCinsp_menu();
}

void loginas_technician()
{
    display_tech_menu();
}

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
void display_QAdir_menu();
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
                    
                case 2:
                    std::cout << "approve_inspection();\n";
                    break;
                case 3:
                    std::cout << "Goodbye!\n";
                    goto exitwhileloop;
            }
            
        }
        
    }
    exitwhileloop:  ;
    
}

/* ------------------------------------------------------------------------ */

//Display menu options for inventory associate
void display_invassoc_menu();
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
