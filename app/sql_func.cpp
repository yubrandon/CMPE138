/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "sql_func.h"


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

/* -----------------------View IQC inspections-----------------------------*/
// void view_IQC_list()
// {
//     //Create SQL Connection
//     sql::Driver *driver;
//     sql::Connection *con;
//     sql::ResultSet *res;
//     sql::Statement *stmt;
    
//     driver = get_driver_instance();
//         con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
//         con->setSchema("InventoryDB");
        
//     //prepare SQL statement
//     stmt = con->createStatement();
//     res = stmt->executeQuery("SELECT * FROM Material WHERE insp_area = 'IQC'");
    
//     delete res;
//     delete stmt;
//     delete con;
// }

// void view_OQC_list()
// {
//          //Create SQL Connection
//     sql::Driver *driver;
//     sql::Connection *con;
//     sql::ResultSet *res;
//     sql::Statement *stmt;
    
//     driver = get_driver_instance();
//         con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
//         con->setSchema("InventoryDB");
        
//     //prepare SQL statement
//     stmt = con->createStatement();
//     res = stmt->executeQuery("SELECT * FROM Material WHERE insp_area = 'OQC'");
    
//     delete res;
//     delete stmt;
//     delete con;
// }

/* -----------------------Add inspection based on employee role-----------------------------*/
void add_inspection(int pn, int insp_area, std::string requirements, std::string emp_role,
                                int qty, std::string result, std::string insp_date)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;
    bool pf = FALSE;

    int max_insp_num;
    
    driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

    //prepare SQL statement to retrieve last inspection number
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT MAX(inspNum) FROM inspections");

    while (res->next())
    {
        max_insp_num = res->getInt(1);
        delete res;
        delete con;
    }
        
    //prepare SQL statement to add new inspection
    pstmt = con->prepareStatement("INSERT INTO inspections(InspNum, PN, Rqmt, result, date) VALUES (?,?,?,?,?)");
    pstmt->setInt(1, max_insp_num+1);
    pstmt->setInt(2, pn);
    pstmt->setString(3, requirements);
    pstmt->setString(5, result);
    pstmt->setDateTime(6, insp_date);
    pstmt->execute();

    pstmt = con->prepareStatement("INSERT INTO inspections_area (insp_num, pn, insp_area, qty) VALUES (?,?,?,?)");
    pstmt->setInt(1, max_insp_num+1);
    pstmt->setInt(2, pn);
    pstmt->setInt(4, qty);

    //set INSP area based on employee role
    if (emp_role == "IQC insepctor")
        {   pstmt->setString(3, "IQC"); }

    else if (emp_role == "technician")
        {   pstmt->setString(3, "PQC");    }
    
    else if (emp_role == "OQC inspector")
        {   pstmt->setString(3, "OQC"); }
    
    pstmt->execute();

    delete res;
    delete pstmt;
    delete con;
}

/* --------------------------------Approve inspection------------------------------------*/
bool approve_inspection(int insp_num, std::string insp_area)
{
         //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    std::string insp_area;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //get inspection area from inspection number

    
    //prepare SQL statement to update pass/fail to true
    pstmt = con->prepareStatement("UPDATE inspections SET pf = TRUE WHERE insp_num = ?");
    pstmt->setInt(1, insp_num);
    pstmt->execute();

    //move material to next insp_area based on current insp_area
    
    delete res;
    delete pstmt;
    delete con;
}
    

/* -----------------------View inspections based on employee role-----------------------------*/
void view_inspection(int insp_num, std::string emp_role)
{
         //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    std::string emp_role;
    
    driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");
        
    //prepare SQL statement
    pstmt = con->prepareStatement("SELECT * FROM inspections WHERE insp_num = (SELECT insp_num FROM inspection_area WHERE insp_area = '?')");

    //set INSP area based on employee role
    if (emp_role == "IQC insepctor")
        {   pstmt->setString(1, "IQC"); }

    else if (emp_role == "technician")
        {   pstmt->setString(1, "PQC");    }
    
    else if (emp_role == "OQC inspector")
        {   pstmt->setString(1, "OQC"); }
    
    pstmt->execute();
    
    
    
    delete res;
    delete pstmt;
    delete con;   
}

// void move_to_IPQC(int pn)
// {
//      no longer needed, implemented in approve_inspection
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
