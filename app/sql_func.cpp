/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "sql_func.h"

void init()
{
    //Create MySQL connection
    sql::Driver *driver;
    sql::Connection *con;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");

    //Load SQL script
    std::ifstream script("../init.sql");
    std::string sql((std::istreambuf_iterator<char>(script)),std::istreambuf_iterator<char>());

    //Create statement and execute script
    sql::Statement *stmt = con->createStatement();
    stmt->execute(sql);

    //Free storage
    delete stmt;
    delete con;
}

bool user_exists(std::string user)
{
    /*try
    {*/
        //Create SQL Connection
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        //Query DB to see if username input exists in DB
        pstmt = con->prepareStatement("SELECT username FROM employee WHERE EXISTS(SELECT username FROM employee WHERE username = ?)");
        pstmt->setString(1,user);
        res = pstmt->executeQuery();

        //If query result exists, return true - username is taken
        if(res -> next())
        {
            delete res;
            delete pstmt;
            delete con;
            return true;
        }
        //Else return false
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
        //Create SQL Connection
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        //Query DB to see if ssn input exists in Db
        pstmt = con->prepareStatement("SELECT ssn FROM employee WHERE EXISTS (SELECT ssn FROM employee WHERE ssn = ?)");
        pstmt->setInt(1,ssn);
        res = pstmt->executeQuery();

        //If query result exists, return true - ssn is already registered in database
        if(res -> next())
        {
            delete res;
            delete pstmt;
            delete con;
            return true;
        }
        //Else return false
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
    //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");
    //Query DB for tuples that have user-inputted username and password
    pstmt = con->prepareStatement("SELECT username,password FROM employee WHERE EXISTS (SELECT username,password FROM employee WHERE username = ? AND password = ?)");
    pstmt->setString(1,user);
    pstmt->setString(2,pw);

    res = pstmt->executeQuery();
    //If query result exists, return true - user and password combination are valid
    if(res->next())
    {
        delete pstmt;
        delete res;
        delete con;
        return true;
    }
    //Else return false
    delete pstmt;
    delete res;
    delete con;
    return false;
}

void create_user(int ssn,std::string name, std::string user, std::string pw)
{
    /*try
    {*/
        //Create SQL Connection
        sql::Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt;
        sql::Statement *stmt;
        sql::ResultSet *res;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        stmt = con -> createStatement();
        //Save count of employees to assign ID for new employee
        res = stmt ->executeQuery("SELECT COUNT(*)+1 FROM employee");
        res->next();
        int id = res->getInt(1);

        //Create new employee tuple with user inputted values
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

/* --------------------------------------------------------------------------------------- */

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
    
    while (res->next())
    {
        std::cout << res->getString(1);
    }
    
    delete res;
    delete stmt;
    delete con;
}

void view_OQC_list()
{
         //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
    
    driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("");
        
    //prepare SQL statement
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT * FROM Material WHERE insp_area = 'OQC'");
    
    while (res->next())
    {
        std::cout << res->getString(1);
    }
    
    delete res;
    delete stmt;
    delete con;
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
