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


/* -------------------------SUPERVISOR/DEPARTMENT MGR------------------------------- */

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

void create_dept(int dnum,std::string d_desc)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO DEPARTMENT VALUES(?,NULL,?)");
    pstmt -> setInt(1,dnum);
    pstmt -> setString(2,d_desc);
    pstmt -> execute();

    delete con;
    delete pstmt;
}
void edit_dept(int dnum, std::string d_desc)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE DEPARTMENT SET Description = ? WHERE Dnumber = ?");
    pstmt -> setString(1,d_desc);
    pstmt -> setInt(2,dnum);
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

std::vector<int> get_bom_id(int pnum)
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
    pstmt -> setInt(1,pnum);
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

    pstmt = con->prepareStatement("SELECT P_desc FROM MATERIAL WHERE P_num = ?");
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

void pull_wo(int prnum, std::vector<int> mat_ids, int qty)
{
    
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
    

/* -----------------------View inspections based on employee role-----------------------------*/
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

/* -----------------------------SQL QUERIES------------------------------ */

//SQL Queries to view various inspection lists

void view_inspections(std::string dept_name)
{
    
}
void receive_material(int pn, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET INSP = INSP + ? WHERE P_num = ?");
    pstmt->setInt(1,qty);
    pstmt->setInt(2,pn);
    pstmt ->executeUpdate();

    delete con;
    delete res;
    delete pstmt;
}

void receive_material_accessory(int pn, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET stores = stores + ? WHERE P_num = ?");
    pstmt->setInt(1,qty);
    pstmt->setInt(2,pn);
    pstmt ->executeUpdate();

    delete con;
    delete pstmt;
}
void backflush_product(int pn);

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

    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET stores = stores - ? AND ship = ship + ? WHERE Pr_num = ?");
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


void view_inspections(std::string dept_name, std::string title)
{
    
}

void view_inspection_requirements(int insp_num)
{
    
}

//SQL Queries for Adding New Inspections
int get_next_insp_num()
{
    return 0;
}

int get_account()
{
    return 0;
}

int get_sample_size(int insp_num)
{
    return 0;
}

void add_inspection(int insp_num, int pn, int insp_qty, std::string insp_area)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO INSPECTIONS VALUES(?,?)");
    pstmt->setInt(1,insp_num);
    pstmt -> execute();


    delete con;
    delete res;
    delete pstmt;
}

//SQL Quesries for Approving Inspections
int get_qty_inspected(int insp_num)
{
    return 0;
}

int calculate_fpy(int insp_num)
{
    return 0;
}

void set_insp_pf(std::string)
{
    
}

//SQL Queries for Adding Material
/*int get_next_mat_num()
{
    return 0;
}*/

void add_part(std::string pdesc, int type)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM PART");
    res->next();

    int pnum = res->getInt(1);

    pstmt = con->prepareStatement("INSERT INTO PART VALUES(?,?,?)");
    pstmt->setInt(1,pnum);
    pstmt->setString(2,pdesc);
    pstmt->setInt(3,type);

    pstmt->execute();

    delete con;
    delete res;
    delete pstmt;
    delete stmt;
}

void edit_part_name(int pnum, std::string pdesc)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE PART SET P_desc = ? WHERE P_num");
    pstmt->setString(1,pdesc);
    pstmt->setInt(2,pnum);

    pstmt->executeUpdate();

    delete con;
    delete res;
    delete pstmt;
}

bool part_exists(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT P_num FROM PART WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();

    bool exists = false;
    if(res->next()) exists = true;

    delete con;
    delete res;
    delete pstmt;
    return exists;
}

int get_part_id(std::string pname)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT P_desc FROM MAT_LOCATIONS WHERE Pr_desc = ?");
    pstmt->setString(1,pname);
    res = pstmt->executeQuery();

    int id = 0;
    if(res->next()) id = res->getInt(1);

    delete con;
    delete res;
    delete pstmt;

    return id;
}

int get_part_type(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT P_type FROM PART WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();
    res->next();

    int type = res->getInt(1);

    delete con;
    delete res;
    delete pstmt;

    return type;
}

std::string get_part_name(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT P_desc FROM PART WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();
    res->next();

    std::string name = res->getString(1);

    delete con;
    delete res;
    delete pstmt;

    return name;
}





void add_part_supplier(int pnum, int supp_num, std::string supp_name)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO PART_SUPPLIER VALUES(?,?,?)");
    pstmt->setInt(1,pnum);
    pstmt->setInt(2,supp_num);
    pstmt->setString(3,supp_name);

    pstmt->execute();

    delete con;
    delete res;
    delete pstmt;
}

void edit_part_supplier(int pnum, int supp_num, std::string supp_name)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE PART_SUPPLIER set Supp_num = ?, Supp_name = ? WHERE P_num = ?");
    pstmt->setInt(1,supp_num);
    pstmt->setString(2,supp_name);
    pstmt->setInt(3,pnum);

    pstmt->executeUpdate();

    delete con;
    delete res;
    delete pstmt;
}

void add_part_location(int pnum, int INSP, int STORES, int WIP, int QC, int FGI)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO MAT_LOCATIONS VALUES(?,?,?,?,?,?)");
    pstmt->setInt(1,pnum);
    pstmt->setInt(2,INSP);
    pstmt->setInt(3,STORES);
    pstmt->setInt(4,WIP);
    pstmt->setInt(5,QC);
    pstmt->setInt(6,FGI);

    pstmt -> execute();

    delete con;
    delete res;
    delete pstmt;
}

void edit_part_location(int pnum, int INSP, int STORES, int WIP, int QC, int FGI)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE PART_LOCATION set INSP = ?, STORES = ?, WIP = ?, QC = ?, FGI = ? WHERE P_num = ?");
    pstmt->setInt(1,INSP);
    pstmt->setInt(2,STORES);
    pstmt->setInt(3,WIP);
    pstmt->setInt(4,QC);
    pstmt->setInt(5,FGI);
    pstmt->setInt(6,pnum);

    pstmt -> executeUpdate();

    delete con;
    delete res;
    delete pstmt;
}

bool part_loc_exists(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT P_num FROM PART_LOCATION WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();

    bool exists = false;
    if(res->next()) exists = true;

    delete con;
    delete res;
    delete pstmt;
    return exists;
}

int get_stores_count(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT STORES FROM PART_LOCATION WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();

    int count = res->getInt(1);

    delete con;
    delete res;
    delete pstmt;
    return count;
}

//SQL Queries for Adding Product
int get_next_prod_num()
{
    return 0;
}


void add_kit(int pr_num,std::vector<int>&mat_list,std::vector<int>&qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    for(int i=0; i < mat_list.size(); i++)
    {
        pstmt = con->prepareStatement("INSERT INTO PART_LIST VALUES(?,?,?)");
        pstmt->setInt(1,pr_num);
        pstmt->setInt(2,mat_list[i]);
        pstmt->setInt(3,qty[i]);
        pstmt->execute();
    }

    delete con;
    delete res;
    delete pstmt;
}

int get_mat_quantity(int pr_num,int mat_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT Mat_Qty FROM PART_LIST WHERE Pmat_num = ? AND PPr_num = ?");
    pstmt -> setInt(1,mat_num);
    pstmt->setInt(2,pr_num);
    res = pstmt->executeQuery();
    res->next();

    int qty = res->getInt(1);

    delete con;
    delete res;
    delete pstmt;
    return qty;
}

void create_requirements(int mat_prod_num, std::string requirement, std::string res_type)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    

    delete con;
    delete res;
    delete pstmt;
}




//Sample state insertion
/*void state_init()
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

    

    delete con;
    delete stmt;
    delete pstmt;
}*/