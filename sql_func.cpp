#include "sql_func.h"


bool user_exists(std::string user)
{
    /*try
    {*/
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        stmt = con->createStatement();
        res = stmt->executeQuery(" SELECT * FROM employee");
        while(res -> next())
        {
            if(user == res->getString(1))
            {
                delete res;
                delete stmt;
                delete con;
                return true;
            }
        }
    
        delete res;
        delete stmt;
        delete con;

        return false;
/*
    } catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }*/

}
