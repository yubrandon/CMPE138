/* SJSU CMPE 138 FALL 2023 TEAM 5 */
#include "sql_func.h"

void db_init()
{
    //Create MySQL connection
    sql::Driver *driver;
    sql::Connection *con;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con -> setSchema("InventoryDB");

    //Load SQL script
    std::ifstream sqlFile("../init.sql");
    std::string sqlScript((std::istreambuf_iterator<char>(sqlFile)),std::istreambuf_iterator<char>());

    std::vector<std::string> queries;
    std::istringstream iss(sqlScript);
    std::string query;

    while (std::getline(iss, query, ';'))
    {
        // Trim leading and trailing whitespaces
        query = std::regex_replace(query, std::regex("^\\s+|\\s+$"), "");

        // Skip empty lines and comments
        if (!query.empty() && query.find("--") != 0)
        {
            queries.push_back(query);
        }
    }

    sql::Statement *stmt = con->createStatement();

    for (const auto &singleQuery : queries)
    {
        // Check if the query is empty before executing
        if (!singleQuery.empty())
        {
            stmt->execute(singleQuery);
        }
    }
    //Free storage
    delete stmt;
    delete con;
}

bool user_exists(std::string user)
{
        //Create SQL Connection
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        //Query DB to see if username input exists in DB
        pstmt = con->prepareStatement("SELECT username FROM employee WHERE username = ?");
        pstmt->setString(1,user);
        res = pstmt->executeQuery();

        //Return if query result exists
        bool valid = res -> next();

        delete res;
        delete pstmt;
        delete con;

        return valid;
}

bool ssn_exists(std::string ssn)
{
        //Create SQL Connection
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        //Query DB to see if ssn input exists in Db
        pstmt = con->prepareStatement("SELECT ssn FROM employee WHERE ssn = ?");
        pstmt->setString(1,ssn);
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
    pstmt = con->prepareStatement("SELECT id FROM employee WHERE username = ? AND pw = ?");
    pstmt->setString(1, user);
    pstmt->setString(2, pw);
    res = pstmt->executeQuery();

    //If query result exists, isValid is true
    bool isValid = res->next();

    //Else return false
    delete pstmt;
    delete res;
    delete con;
    return isValid;
}

void create_user(std::string ssn,std::string name, std::string user, std::string pw, std::string lname, std::string fname)
{
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
        pstmt = con->prepareStatement("INSERT INTO employee VALUES (?,?,?,?,?,?)");
        pstmt -> setInt(1,id);
        pstmt -> setString(2,ssn);
        pstmt -> setString(3,lname);
        pstmt -> setString(4,fname);
        pstmt -> setString(5,user);
        pstmt -> setString(6,pw);

        pstmt -> execute();

        delete pstmt;
        delete con;
}

void get_user(User *user)
//Retrieving logged in user's information
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //Retrieve user's id
    pstmt = con->prepareStatement("SELECT ID FROM EMPLOYEE WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->id = res->getInt(1);

    //Retrieve user's ssn
    pstmt = con->prepareStatement("SELECT SSN FROM EMPLOYEE WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->ssn = res->getString(1);

    //Retrieve user's last name
    pstmt = con->prepareStatement("SELECT Lname FROM EMPLOYEE WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->lname = res->getString(1);

    //Retrieve user's first name
    pstmt = con->prepareStatement("SELECT Fname FROM EMPLOYEE WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->fname = res->getString(1);
/*  
    uncomment after adding tuples
   //Retrieve user's super_ssn
    pstmt = con->prepareStatement("SELECT super_ssn FROM EMPLOYEE_INFO WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->super_ssn = res->getInt(1);

    //Retrieve user's department number
    pstmt = con->prepareStatement("SELECT Dno FROM EMPLOYEE_INFO WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->dno = res->getInt(1);

    //Retrieve user's job title
    pstmt = con->prepareStatement("SELECT job_title FROM EMPLOYEE_INFO WHERE Username = ?");
    pstmt -> setString(1,user->username);
    res = pstmt -> executeQuery();
    res -> next();
    user->job_title = res->getString(1);*/

    delete pstmt;
    delete con;
    delete res;
}


/* -------------------------ADMINISTRATOR------------------------------- */

void assign_dept(int id,int dnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE EMPLOYEE_INFO SET Dno = ? WHERE ID = ?");
    pstmt -> setInt(1,dnum);
    pstmt -> setInt(2,id);
    pstmt -> executeUpdate();

    delete con;
    delete pstmt;
}
void assign_role(int id, std::string role)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE EMPLOYEE_INFO SET job_title = ? WHERE ID = ?");
    pstmt -> setString(1,role);
    pstmt -> setInt(2,id);
    pstmt -> executeUpdate();

    delete con;
    delete pstmt;
}
void assign_dept_mgr(std::string ssn, int dnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE DEPARTMENT SET Dept_mgr = ? WHERE Dnumber = ?");
    pstmt -> setString(1,ssn);
    pstmt -> setInt(2,dnum);
    pstmt -> executeUpdate();

    delete con;
    delete pstmt;
}

/* -------------------------------------------------------------- */
int get_emp_id(int ssn)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT ID FROM EMPLOYEE WHERE SSN = ?");
    pstmt -> setInt(1,ssn);
    res = pstmt -> executeQuery();
    int id = res->getInt(1);

    delete con;
    delete pstmt;
    delete res;

    return id;
}
/* ---------------------------SUPERVISOR--------------------------- */
std::vector<int> get_supervisee(std::string ssn)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    std::vector<int> supervisee;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con ->prepareStatement("SELECT ID FROM EMPLOYEE_INFO WHERE Super_ssn = ?");
    pstmt -> setString(1,ssn);
    res = pstmt -> executeQuery();

    while(res ->next())
    {
        supervisee.push_back(res->getInt(1));
    }

    delete con;
    delete pstmt;
    delete res;

    return supervisee;
}

std::vector<int> get_inventory(int dnum)
{

}

/* -------------------------------------------------------------- */
std::vector<int> get_bom_id(int prnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    std::vector<int> mat_id;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT PMat_num FROM PART_LIST WHERE PPr_num = ?");
    pstmt -> setInt(1,prnum);
    res = pstmt -> executeQuery();

    while(res->next())
    {
        mat_id.push_back(res->getInt(1));
    }

    delete con;
    delete pstmt;
    delete res;

    return mat_id;
}

std::vector<std::string> get_bom_desc(std::vector<int> &id_vec)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    std::vector<std::string> mat_desc;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT Mat_desc FROM MATERIAL WHERE Mat_num = ?");
    for(int i = 0; i < id_vec.size(); i++)
    {
        pstmt -> setInt(1,id_vec[i]);
        res = pstmt -> executeQuery();
        mat_desc.push_back(res -> getString(1));
    }

    delete con;
    delete res;
    delete pstmt;

    return mat_desc;

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
        res = stmt->executeQuery("SELECT * FROM INSP_REQ_AREA WHERE Insp_area = 'IQC'");
    
    //acquire PN, then use pn to reference part that is being worked on?
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
    res = stmt->executeQuery("SELECT * FROM INSP_REQ_AREA WHERE insp_area = 'OQC'");
    
    while (res->next())
    {
        std::cout << res->getString(1);
    }
    
    delete res;
    delete stmt;
    delete con;   
}
/* -----------------------Add inspection based on employee role-----------------------------*/
void add_inspection(int pn, int insp_area, std::string requirements, std::string emp_role,
                                int qty, std::string result, std::string insp_date)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;
    bool pf = false;

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
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //get inspection area from inspection number

    
    //prepare SQL statement to update pass/fail to true
    pstmt = con->prepareStatement("UPDATE inspections SET pass_fail = TRUE WHERE insp_num = ?");
    pstmt->setInt(1, insp_num);
    pstmt->execute();

    //move material to next insp_area based on current insp_area
    
    delete res;
    delete pstmt;
    delete con;
}
    

/* ------------------------------------------- VIEW SQL FUNCTIONS ----------------------------------------------------- */
void view_inspection(int insp_num, std::string emp_role)
{
         //Create SQL Connection
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    
    driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");
        
    //prepare SQL statement
    pstmt = con->prepareStatement("SELECT * FROM inspections WHERE insp_num = (SELECT insp_num FROM inspection_area WHERE insp_area = '?')");

    //set INSP area based on employee role
    if (emp_role == "IQC Inspector")
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

void view_inspections(std::string dept_name, std::string title)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con -> prepareStatement("SELECT INSPECTIONS.Insp_num, INSPECTIONS.P_num, INSPECTIONS.Insp_date, INSPECTIONS.Emp_id, INSP_AREA.qty_inspected FROM INSPECTIONS INNER JOIN INSP_AREA ON INSPECTIONS.Insp_num = INSP_AREA.Insp_num AND INSP_AREA.insp_area = ?");
    
    if (dept_name == "Quality")
    {
        if (title == "IQC Inspector") { pstmt ->setString(1, "IQC");   }

        else if (title == "OQC Inspector") {   pstmt ->setString(1, "OQC");   }

        else if (title == "QA Director") {   pstmt ->setString(1, "FQC");   }
    }

    else if (dept_name == "Operations")
    {
        if (title == "Technician") {   pstmt ->setString(1, "IPQC");   }
    }

    while (res->next())
    {
        std::cout << "Inspection #: " << res->getInt(1) << " ";
        std::cout << "Part Number: " << res->getInt(2) << " ";
        std::cout << "Inspection Date: " << res->getString(3) << " ";
        std::cout << "Performed By: " << res->getInt(4) << " ";
        std::cout << "Quantity Inspected: " << res->getInt(5) << "\n";
    }

    delete con;
    delete res;
    delete pstmt;
    
}

//SQL Queries to view various inspection lists




void move_to_OQC(int pn)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con -> prepareStatement("SELECT Qty FROM INSP_AREA WHERE Insp_pnum = ? AND Insp_area = 'IPQC'");
    pstmt -> setInt(1,get_insp_num(pn));
    res = pstmt -> executeQuery(); 
    res -> next();
    int qty = res->getInt(1);

    pstmt = con->prepareStatement("UPDATE PRODUCT_LOCATIONS SET stores = stores - ? AND ship = ship + ? WHERE Pr_num = ?");
    pstmt->setInt(1,qty);
    pstmt->setInt(2,qty);
    pstmt->setInt(3,pn);
    pstmt ->executeUpdate();

    delete con;
    delete res;
    delete pstmt;

}


int get_insp_num(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT Insp_num FROM INSPECTIONS WHERE Insp_pnum = ?");
    pstmt -> setInt(1,pnum);
    res = pstmt->executeQuery();

    //returns 0 if no inspection is found for the given part number
    int i = 0;
    if(res->next())
        i = res->getInt(1);
    delete res;
    delete pstmt;
    delete con;
    return i;
}


void view_inspection_requirements(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT (Insp_req, Insp_res, qty_passed) FROM INSP_REQ_RES WHERE INSP_REQ_RES.Insp_num = ?");
    pstmt ->setInt(1, insp_num);
    res = pstmt ->executeQuery();

    while (res->next())
    {
        std::cout << "Requirement: " << res->getString(1);
        std::cout << " Insp_res: " << res->getString(2);
        std::cout << " Qty Passed: " << res->getInt(3);
    }

    delete con;
    delete res;
    delete pstmt;
    
}

//SQL Queries for Adding New Inspections
int get_next_insp_num()
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;

    int next_num;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT MAX(Insp_num)+1 FROM INSPECTIONS");

    if (res->next())
    {
        next_num = res->getInt(1);
    }
    
    return next_num;
}

int get_account()
{
    return 0;
}

double get_sample_size(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    double sample_size;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT sample_size FROM Inspections WHERE Insp_pnum = ?");
    pstmt -> setInt(1, insp_num);
    res = pstmt->executeQuery();

    if (res->next())
    {
        sample_size = res->getDouble(1);
    }
    
    return sample_size;
}

void add_inspection(int insp_num, int pn, int insp_qty, std::string insp_area) // brandon
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO INSPECTIONS VALUES(?,?,NULL,NULL,NULL)");
    pstmt->setInt(1,insp_num);
    pstmt->setInt(2,pn);
    pstmt -> execute();

    pstmt = con->prepareStatement("INSERT INTO INSP_AREA VALUES(?,?,?)");
    pstmt->setInt(1,insp_num);
    pstmt->setString(2,insp_area);
    pstmt->setInt(3,insp_qty);
    pstmt->execute();

    delete con;
    delete res;
    delete pstmt;
}

void update_inspection_requirements(int insp_num, int id, std::string time, std::string insp_req, std::string insp_res, int qty_pass)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE INSPECTIONS SET Emp_id = ?, Insp_date = ? WHERE Insp_num = ?");
    pstmt->setInt(1,id);
    pstmt->setString(2,time);
    pstmt->setInt(3,insp_num);
    pstmt->executeUpdate();

    pstmt = con->prepareStatement("INSERT INTO INSP_REQ_RES VALUES (?,?,?,?)");
    pstmt->setInt(1,insp_num);
    pstmt->setString(2,insp_req);
    pstmt->setString(3,insp_res);
    pstmt->setInt(4,qty_pass);
    pstmt->execute();

    delete pstmt;
    delete con;
}
//SQL Queries for Approving Inspections

//SQL Query to get the quantity inspected from INSP_AREA
int get_qty_inspected(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    int qty_inspected;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT qty_inspected FROM INSP_AREA WHERE Insp_pnum = ?");
    pstmt -> setInt(1, insp_num);
    res = pstmt->executeQuery();

    if (res->next())
    {
       qty_inspected = res->getInt(1);
    }
    
    delete con;
    delete pstmt;
    delete res;

    return qty_inspected;
}

int calculate_fpy(int insp_num)
{
    int qty_inspected = get_qty_inspected(insp_num);

    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    int num_req, qty_passed, i;
    bool passed = false;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT COUNT(*) FROM INSP_REQ_RES WHERE Insp_num = ?");
    pstmt -> setInt(1, insp_num);
    res = pstmt->executeQuery();

    if (res->next())
    {
        num_req = res->getInt(1);
    }

    delete con;
    delete res;
    delete pstmt;

    //for each requirement, verify the quantity that passed matches the qty to be inspected
        //if the qty passed does not match the one inspected, set to fail, else pass

    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    int total_req_passed = 0;
    double fpy;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT qty_passed FROM INSP_REQ_RES WHERE Insp_num = ?");
    pstmt -> setInt(1, insp_num);

    while (res->next())
    {
        qty_passed = res->getInt(i);

        if (qty_passed != qty_inspected)
        {
            set_insp_pf(insp_num, 0);
        }
        else {  total_req_passed++; } 
        i++;
    }

    fpy = total_req_passed / num_req;

    delete con;
    delete res;
    delete pstmt;

    return fpy;
}

void set_insp_pf(int insp_num, bool pf)     // VERIFY IF THIS MATCHES 
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE INSPECTIONS SET pf = ? WHERE Insp_num = ?");
    pstmt -> setBoolean(1, pf);
    pstmt -> setInt(1, insp_num);
    res = pstmt->executeQuery();

    
    delete con;
    delete pstmt;
    delete res;

}

//SQL Queries for Part Number
int gegt_next_p_num()
{
    return 0;
}

void add_to_materials(int mat_num, std::string mat_desc, std::string supp_name, int supp_num)
{
    
}

//SQL Queries for Creating Inspection Requirements
bool part_exists(int mat_prod_num)
{
    return false;
}

void create_requirements(int mat_prod_num, std::string requirement, std::string res_type)
{
    
}

/* ------------------------- SQL Queries to move parts through INSP to FGI -------------------------- */
void move_INSP_to_STORES(int p_num, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //subtract from original location and add to STORES
    pstmt = con->prepareStatement("UPDATE PART_LOCATIONS SET INSP = INSP - ? AND STORES = STORES + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from INSP into STORES.\n";

    delete con;
    delete pstmt;
    delete res;
}

void move_STORES_to_WIP(int p_num, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //subtract from original location and add to WIP
    pstmt = con->prepareStatement("UPDATE PART_LOCATIONS SET STORES = STORES - ? AND WIP = WIP + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from STORES into WIP.\n";

    delete con;
    delete pstmt;
    delete res;
}

void move_WIP_to_QC(int p_num, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //subtract from original location and add to QC
    pstmt = con->prepareStatement("UPDATE PART_LOCATIONS SET WIP = WIP - ? AND QC = QC + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from WIP into QC.\n";

    delete con;
    delete pstmt;
    delete res;
}

void move_QC_to_FGI(int p_num, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //subtract from original location and add to FGI
    pstmt = con->prepareStatement("UPDATE PART_LOCATIONS SET QC = QC - ? AND FGI = FGI + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from QC into FGI.\n";

    delete con;
    delete pstmt;
    delete res;
}


/* ------------------ Accessor and Modifier Functions --------------------------------- */



//ACCESSORS AND MODIFIERS FOR SETTING INDIVIDUAL ATTRIBUTES
void set_string_attribute(std::string table, std::string attr, std::string attr_value,
                            std::string condition, std::string con_value)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE ? SET ? = ? WHERE ? = ?");
    pstmt->setString(1, table);
    pstmt->setString(2, attr);
    pstmt->setString(3, attr_value);
    pstmt->setString(4, condition);
    pstmt->setString(5, con_value);

    res = pstmt->executeQuery();


}

void set_bool_attribute(std::string table, std::string attr, bool attr_value,
                            std::string condition, bool con_value)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE ? SET ? = ? WHERE ? = ?");
    pstmt->setString(1, table);
    pstmt->setString(2, attr);
    pstmt->setBoolean(3, attr_value);
    pstmt->setString(4, condition);
    pstmt->setBoolean(5, con_value);

    res = pstmt->executeQuery();
}

void set_int_attribute(std::string table, std::string attr, std::string table_value, int attr_value)
{

}

void set_double_attribute(std::string table, std::string attr, std::string table_value, double attr_value)
{

}




/* -------------------------- Sample state insertion ---------------------------------- */

void state_init()
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;

    SHA256 sha256;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");


    //create departments
    create_user("123456789", "wdoe1", sha256("dsfsdfs"), "Doe", "Willie");
    assign_dept(1, 2);
    assign_role(1, "IQC Inspector");

    create_user("111222333", "esmith5", sha256("afwf345j"), "Smith", "Eddie");
    assign_dept(2, 2);
    assign_role(1, "OQC Inspector");

    create_user("444555666", "kmunoz2", sha256("df456sd"), "Munoz", "Kara");
    assign_dept(3, 2);
    assign_role(1, "QA Director");

    create_user("777888999", "csharma1", sha256("g5m1y6u"), "Sharma", "Chas");
    assign_dept(4, 3);
    assign_role(1, "Inventory Associate");

    create_user("453453453", "lpeterson1", sha256("65r4h6wr6"), "Peterson", "Laila");
    assign_dept(5, 1);
    assign_role(1, "CEO");

    create_user("786786786", "oreid1", sha256("rge56r"), "Reid", "Owen");
    assign_dept(6, 3);
    assign_role(1, "Technician");

    create_user("654987321", "sadil1", sha256("t1n5w6"), "Adil", "Syed");
    assign_dept(7, 4);
    assign_role(1, "Engineering Manager");

    create_user("123456789", "mli7", sha256("nrt41n"), "Li", "Mei");
    assign_dept(8, 3);
    assign_role(1, "Operations Manager");

    //assign_emp_id();
    //get_emp_id("123456789");

    create_dept(1, "Human Resources");
    create_dept(2, "Quality");
    create_dept(3, "Operations");
    create_dept(4, "Engineering");
    
    add_inspection(1, 1010, 10, "IQC");
    update_inspection_requirements(1, 1010, "10-24-2023", "Verify adapter is deburred and free of damage", "visual", 10);
    update_inspection_requirements(1, 1010, "10-24-2023", "Verify light turns on when plugging in", "visual", 10);

    add_inspection(2, 1001, 15, "OQC");
    update_inspection_requirements(2, 1001, "10-30-2023", "Test voltage output to verify 5V +- .10V", "Volts", 15);
    update_inspection_requirements(2, 1001, "10-30-2023", "Turn screen on to verify all pixels are working properly", "visual", 15);

    add_inspection(3, 1001, 20, "FQC");
    update_inspection_requirements(3, 1001, "11-05-2023", "Verify documentation related to build is complete", "visual", 20);
    update_inspection_requirements(3, 1001, "11-05-2023", "Verify label has no smears, blurs, or bumps", "visual", 20);

    delete con;
    delete stmt;
    delete pstmt;
}

// MISC FUNCTIONS NO LONGER NEEDED
// void add_material()
// {
//     int mat_num, supp_num;
//     std::string mat_desc, supp_name;
    
//     //query to get next material number
//     mat_num = get_next_mat_num();

    
//     std::cout << "Enter the new material description: ";
//     std::cin >> mat_desc;
    
//     std::cout << "Enter the supplier name: ";
//     std::cin >> supp_name;
    
//     std::cout << "Enter the supplier's part number: ";
//     std:: cin >> supp_num;
    
//     //query to add information to Materials table
//     std::cout << "Adding material to inventory list...";
//     add_to_materials(mat_num, mat_desc, supp_name, supp_num);
//     std::cout << "Material added!\n";
// }

// void add_product()
// {
//     int prod_num, prod_desc, mat_num, mat_qty, choice;
//     std::string pn, qty;
//     std::string option;
//     bool add_more = true;
    
//     //query to get next product number
//     prod_num = get_next_p_num();
    
//     std::cout << "Enter the new product description: ";
//     std::cin >> prod_desc;
    
//     while (add_more)
//     {
        
//         std::cout << "Enter the material number you would like to add to the product: ";
//         std::cin >> pn;
//         mat_num = std::stoi(pn);
        
//         std::cout << "Enter the quantity for " << pn << ": ";
//         std::cin >> qty;
//         mat_qty = std::stoi(qty);
        
        
//         std::cout << "Would you like to add another item to this product?\n";
//         std::cout << "\t1. Yes\n";
//         std::cout << "\t2. No\n";
        
//         std::cin >> option;
//         choice = std::stoi(option);
        
//         if (choice == 2)
//         {
//             add_more = false;
//             std::cout << "Product added to inventory!";
//         }
//     }
    
// }
