/* SJSU CMPE 138 FALL 2023 TEAM 5 */
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
        res = stmt->executeQuery("SELECT username FROM employee");
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

    /*} catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }*/

}

bool ssn_exists(int ssn)
{
    /*try
    {*/
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT ssn FROM employee");
        while(res -> next())
        {
            if(ssn == res->getInt("ssn"))
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

    /*} catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }*/
}

bool verify_user(std::string user, std::string pw)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT username,password FROM employee");

    while(res->next())
    {
        if(user == res->getString(1) && pw == res->getString(2))
        {
            delete stmt;
            delete res;
            delete con;
            return true;
        }
    }

    delete stmt;
    delete res;
    delete con;
    return false;
}

void create_user(int ssn,std::string name, std::string user, std::string pw)
{
    /*try
    {*/
        sql::Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        stmt = con -> createStatement();
        res = stmt ->executeQuery("SELECT COUNT(*)+1 FROM employee");
        res->next();
        int id = res->getInt(1);

        pstmt = con->prepareStatement("INSERT INTO employee VALUES (?,?,?,?,?)");
        pstmt -> setInt(1,ssn);
        pstmt -> setInt(2,id);
        pstmt -> setString(3,name);
        pstmt -> setString(4,user);
        pstmt -> setString(5,pw);

        pstmt -> execute();

        delete pstmt;
        delete con;


    /*} catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }*/
}