/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "sql_func.h"


bool user_exists(std::string user)
{
    /*try
    {*/
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        pstmt = con->prepareStatement("SELECT username FROM employee WHERE EXISTS(SELECT username FROM employee WHERE username = ?)");
        pstmt->setString(1,user);
        res = pstmt->executeQuery();

        if(res -> next())
        {
            delete res;
            delete pstmt;
            delete con;
            return true;
        }
    
        delete res;
        delete pstmt;
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
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        pstmt = con->prepareStatement("SELECT ssn FROM employee WHERE EXISTS (SELECT ssn FROM employee WHERE ssn = ?)");
        pstmt->setInt(1,ssn);
        res = pstmt->executeQuery();
        if(res -> next())
        {
            delete res;
            delete pstmt;
            delete con;
            return true;
        }
    
        delete res;
        delete pstmt;
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
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT username,password FROM employee WHERE EXISTS (SELECT username,password FROM employee WHERE username = ? AND password = ?)");
    pstmt->setString(1,user);
    pstmt->setString(2,pw);

    res = pstmt->executeQuery();

    if(res->next())
    {
        delete pstmt;
        delete res;
        delete con;
        return true;
    }

    delete pstmt;
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