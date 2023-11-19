#include "sql_func.h"

void view_IQC_list()
{
    //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
    
    driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");
        
    //prepare SQL statement
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT * FROM Material WHERE insp_area = 'IQC'");
    
    while (res->next)
    {
        std::cout << res->getString();
    }
    
    delete *res;
    delete *stmt;
    delete *con;
}

void view_OQC_list()
{
         //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
    
    driver = get_driver_instance();
        con = driver->connect("", "", "");
        con->setSchema("");
        
    //prepare SQL statement
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT * FROM Material WHERE insp_area = 'OQC'");
    
    while (res->next())
    {
        std::cout << res->getString(1);
    }
    
    delete *res;
    delete *stmt;
    delete *con;
// }

// void header::create_inspection(int pn, std::string pdesc, int insp_area, std::string requirements, std::string result_type, int sample_size)
// {
    
// }

// void header::approve_inspection(int insp_num)
// {
    
// }

// void header::view_inspection(int insp_num)
// {
    
// }

// void header::move_to_IPQC(int pn)
// {
    
// }

// void header::receive_material(int pn)
// {
    
// }

// void header::backflush_product(int pn)
// {
    
// }

// void header::move_to_IQC(int pn)
// {
    
// }
