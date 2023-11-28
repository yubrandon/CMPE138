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
    //delete stmt;
    //delete con;
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

        //delete res;
        //delete pstmt;
        //delete con;

        return valid;
}

bool user_exists(int id)
{
        sql::Driver *driver;
        sql::Connection *con;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
        con->setSchema("InventoryDB");

        //Query DB to see if username input exists in DB
        pstmt = con->prepareStatement("SELECT ID FROM employee WHERE ID = ?");
        pstmt->setInt(1,id);
        res = pstmt->executeQuery();

        //Return if query result exists
        bool valid = res -> next();

        //delete res;
        //delete pstmt;
        //delete con;

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
            //delete res;
            //delete pstmt;
            //delete con;
            return true;
        }
        //Else return false
        //delete res;
        //delete pstmt;
        //delete con;
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
    //delete pstmt;
    //delete res;
    //delete con;
    return isValid;
}

void create_user(std::string ssn, std::string user, std::string pw, std::string lname, std::string fname)
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

    pstmt = con->prepareStatement("INSERT INTO EMPLOYEE_INFO VALUES(?,NULL,NULL)");
    pstmt->setInt(1,id);
    pstmt->execute();

    //delete pstmt;
    //delete con;
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

    //Retrieve user's department number
    pstmt = con->prepareStatement("SELECT Dno FROM EMPLOYEE_INFO WHERE ID = ?");
    pstmt -> setInt(1,user->id);
    res = pstmt -> executeQuery();
    res -> next();
    user->dno = res->getInt(1);

    //Retrieve user's job title
    pstmt = con->prepareStatement("SELECT job_title FROM EMPLOYEE_INFO WHERE ID = ?");
    pstmt -> setInt(1,user->id);
    res = pstmt -> executeQuery();
    res -> next();
    user->job_title = res->getString(1);

    //delete pstmt;
    //delete con;
    //delete res;
}


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

    //delete con;
    //delete pstmt;
    //delete res;

    return id;
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

    //delete con;
    //delete pstmt;
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

    //delete con;
    //delete pstmt;
}

void view_unassigned_emp()
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res,*res2;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //prepare SQL statement
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT ID FROM EMPLOYEE_INFO WHERE Dno = NULL");
    std::vector<int>ids;
    std::vector<std::string>names;
    while(res->next())
    {
        ids.push_back(res->getInt(1));
        pstmt= con->prepareStatement("SELECT Lname, Fname FROM EMPLOYEE WHERE ID = ?");
        pstmt -> setInt(1,res->getInt(1));
        res2 = pstmt -> executeQuery();
        names.push_back(res->getString(1) + ", " + res->getString(2));
    }

    while (res->next())
    {
        names.push_back(res->getString(1) + ", " + res->getString(2));
    }
    for(int i = 0; i < ids.size(); i++)
    {
        std::cout << "\tID: " << ids[i] << "\tName: " << names[i] << std::endl;
    }
    
    //delete res;
    //delete stmt;
    //delete con;
}
void view_roleless_emp()
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res,*res2;
    sql::Statement *stmt;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //prepare SQL statement
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT ID FROM EMPLOYEE_INFO WHERE Job_title = NULL");
    std::vector<int>ids;
    std::vector<std::string>names;
    while(res->next())
    {
        ids.push_back(res->getInt(1));
        pstmt= con->prepareStatement("SELECT Lname, Fname FROM EMPLOYEE WHERE ID = ?");
        pstmt -> setInt(1,res->getInt(1));
        res2 = pstmt -> executeQuery();
        names.push_back(res->getString(1) + ", " + res->getString(2));
    }

    
    for(int i = 0; i < ids.size(); i++)
    {
        std::cout << "\tID: " << ids[i] << "\tName: " << names[i] << std::endl;
    }
    
    //delete res;
    //delete stmt;
    //delete con;
}
void view_department(int dnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res,*res2;
    sql::PreparedStatement *pstmt,*pstmt2;
        
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //prepare SQL statement
    pstmt = con->prepareStatement("SELECT ID FROM EMPLOYEE_INFO WHERE Dno = ?");
    pstmt ->setInt(1,dnum);
    res = pstmt->executeQuery();

    std::vector<int>ids;
    std::vector<std::string>names;
    while (res->next())
    {
        ids.push_back(res->getInt(1));
    }
    for(int i = 0; i < ids.size(); i++)
    {
        pstmt2 = con->prepareStatement("SELECT Lname, Fname FROM EMPLOYEE WHERE ID = ?");
        pstmt2 -> setInt(1,ids[i]);
        res2 = pstmt2->executeQuery();
        if(res2 ->next())
{        names.push_back(res2->getString(1) + ", " + res2->getString(2));
}
    }
    for(int i = 0; i < ids.size(); i++)
    {
        std::cout << "\tID: " << ids[i] << "\tName: " << names[i] << std::endl;
    }
    
    //delete res;
    //delete stmt;
}

bool dept_exists(int dnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT Dnumber FROM DEPARTMENT WHERE Dnumber = ?");
    pstmt->setInt(1,dnum);
    res = pstmt->executeQuery();

    bool exists = false;
    if(res->next()) exists = true;

    //delete con;
    //delete res;
    //delete pstmt;
    return exists;
}

void view_dept()
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::Statement *stmt;
        
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    //prepare SQL statement
    stmt = con->createStatement();
    res = stmt -> executeQuery("SELECT Dnumber, Dept_desc FROM DEPARTMENT");
    

    while (res->next())
    {
       std::cout << "Department #: " << res -> getInt(1);
       std::cout << "\tDescription: " << res->getString(2) << std::endl;
    }
    std::cout << std::endl;
    
    //delete res;
    //delete stmt;
    //delete con;
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

    //delete con;
    //delete pstmt;
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

    //delete con;
    //delete pstmt;
}

void assign_dept_mgr(int dnum, int id)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE DEPARTMENT SET Dept_mgr = ? WHERE Dnumber = ?");
    pstmt -> setInt(1,id);
    pstmt -> setInt(2,dnum);
    pstmt -> executeUpdate();

    //delete con;
    //delete pstmt;
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

    //delete con;
    //delete pstmt;
    //delete res;

    return supervisee;
}

void get_inventory(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT P_desc,P_type FROM PART WHERE P_num = ?");
    pstmt -> setInt(1,pnum);
    res = pstmt->executeQuery();
    res -> next();
    std::cout << "Item name: " << res->getString(1) << std::endl;
    std::cout << "Item type: ";
    switch (res->getInt(2))
    {
        case 0: std::cout << "Material\n"; break;
        case 1: std::cout << "Product\n"; break;
        case 2: std::cout << "Accessory\n";  break;
    }

    pstmt = con->prepareStatement("SELECT Supp_num,Supp_name FROM PART_SUPPLIER WHERE P_num = ?");
    pstmt -> setInt(1,pnum);
    res = pstmt->executeQuery();
    if(res -> next())
    {   
        std::cout << "Supplier number: " << res->getInt(1) << std::endl;
        std::cout << "Supplier name: " << res-> getString(2) << std::endl;
    }
    else
    {
        std::cout << "Supplier number: N/A" << std::endl;
        std::cout << "Supplier name: N/A" << std::endl;
    }

    pstmt = con->prepareStatement("SELECT INSP,STORES,WIP,QC,FGI FROM PART_LOCATION WHERE P_num = ?");
    pstmt -> setInt(1,pnum);
    res = pstmt->executeQuery();
    res -> next();

    std::cout << "Amount in inventory:\n";
    std::cout << "\tINSP: " << res->getInt(1) << std::endl;
    std::cout << "\tSTORES: " << res->getInt(2) << std::endl;
    std::cout << "\tWIP: " << res->getInt(3) << std::endl;
    std::cout << "\tQC: " << res->getInt(4) << std::endl;
    std::cout << "\tFGI: " << res->getInt(5) << std::endl;

    //delete con;
    //delete pstmt;
    //delete res;
}

void get_inventory_all()
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    sql::Statement *stmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    stmt = con->createStatement();
    res = stmt -> executeQuery("SELECT P_num from PART");
    std::vector<int>pnum;
    while(res->next())
    {
        pnum.push_back(res->getInt(1));
    }
    for(int i = 0; i < pnum.size(); i++)
    {
        std::cout << "Part #: " << pnum[i];
        std::cout << "\tPart Description: " << get_part_name(pnum[i]) << std::endl;
    }
    std::cout << "Would you like to inspect a specific part? (Y/N)\n";
    std::string sel;
    std::cin>>sel;
    while(tolowerstring(sel) != "y" && tolowerstring(sel) != "n")
    {
        std::cout << "Invalid input. Please re-enter your choice: ";
        std::cin>>sel;
    }
    if(tolowerstring(sel) == "y")
    {
        std::cout << "Select a part number to inspect: ";
        int n;
        std::cin >> n;
        if(part_exists(n))
        {
            get_inventory(n);
        }
        else
        {
            std::cout << "Invalid part. Returning to menu.\n";
            return;
        }
    }
    else
    {
        std::cout << "Returning to main menu.\n";
        return;
    }

    ////delete stmt;
    ////delete con;
    ////delete pstmt;
    ////delete res;
}

/* -------------------------------------------------------------- */

bool kit_exists(int prnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;


    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT PPr_num FROM PART_LIST WHERE PPr_num = ?");
    pstmt -> setInt(1,prnum);
    res = pstmt -> executeQuery();

    if(res->next()) return true;
    return false;

    //delete con;
    //delete pstmt;
    //delete res;

}

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

    //delete con;
    //delete pstmt;
    //delete res;

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

    pstmt = con->prepareStatement("SELECT P_desc FROM PART WHERE P_num = ?");
    for(int i = 0; i < id_vec.size(); i++)
    {
        pstmt -> setInt(1,id_vec[i]);
        res = pstmt -> executeQuery();
        mat_desc.push_back(res -> getString(1));
    }

    //delete con;
    //delete res;
    //delete pstmt;

    return mat_desc;

}

void pull_wo(int prnum, std::vector<int> mat_ids, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    std::vector<std::string> mat_desc;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    for(int i = 0; i < mat_ids.size(); i++)
    {
        pstmt = con ->prepareStatement("SELECT Mat_qty FROM PART_LIST WHERE PPr_num = ?");
        pstmt->setInt(1,prnum);
        res = pstmt->executeQuery();
        res->next();
        int mult = res->getInt(1);

        pstmt = con->prepareStatement("UPDATE PART_LOCATION SET WIP = WIP + ?, STORES = STORES - ? WHERE P_num = ?");
        pstmt->setInt(1,mult*qty);
        pstmt->setInt(2,mult*qty);
        pstmt->setInt(3,mat_ids[i]);
        pstmt->executeUpdate();
    }

    //delete con;
    //delete res;
    //delete pstmt;
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
    
    //delete res;
    //delete stmt;
    //delete con;
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
    
    //delete res;
    //delete stmt;
    //delete con;   
}
/* -----------------------Add inspection based on employee role-----------------------------*/
/*void add_inspection(int pn, int insp_area, std::string requirements, std::string emp_role,
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
        //delete res;
        //delete con;
    }
        
    //prepare SQL statement to add new inspection
    pstmt = con->prepareStatement("INSERT INTO INSPECTIONS(InspNum, PN, Rqmt, result, date) VALUES (?,?,?,?,?)");
    pstmt->setInt(1, max_insp_num+1);
    pstmt->setInt(2, pn);
    pstmt->setString(3, requirements);
    pstmt->setString(5, result);
    pstmt->setDateTime(6, insp_date);
    pstmt->execute();

    pstmt = con->prepareStatement("INSERT INTO INSP_AREA (insp_num, pn, insp_area, qty) VALUES (?,?,?,?)");
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

    //delete res;
    //delete pstmt;
    //delete con;
}*/

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
    
    //delete res;
    //delete pstmt;
    //delete con;
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
    
    
    
    //delete res;
    //delete pstmt;
    //delete con;   
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

    pstmt = con -> prepareStatement("SELECT INSPECTIONS.Insp_num, INSPECTIONS.Insp_pnum, INSPECTIONS.Insp_date, INSPECTIONS.Emp_id, INSP_AREA.Qty FROM INSPECTIONS INNER JOIN INSP_AREA ON INSPECTIONS.Insp_num = INSP_AREA.Insp_num AND INSP_AREA.insp_area = ?");
    
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
    res = pstmt->executeQuery();
    while (res->next())
    {
        std::cout << "Inspection #: " << res->getInt(1) << " ";
        std::cout << "Part Number: " << res->getInt(2) << " ";
        std::cout << "Inspection Date: " << res->getString(3) << " ";
        std::cout << "Performed By: " << res->getInt(4) << " ";
        std::cout << "Quantity Inspected: " << res->getInt(5) << "\n";
    }

    //delete con;
    //delete res;
    //delete pstmt;
    
}

//SQL Queries to view various inspection lists

void receive_material(int pn, int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");
    //INSP -> STORES
    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET STORES = STORES + ? WHERE P_num = ?");
    pstmt->setInt(1,qty);
    pstmt->setInt(2,pn);
    pstmt ->executeUpdate();

    //delete con;
    //delete res;
    //delete pstmt;
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

    //delete con;
    //delete pstmt;
}
void backflush_product(int pn,int qty)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con -> prepareStatement("SELECT PMat_num,Mat_qty FROM PART_LIST WHERE PPr_num = ?");
    pstmt -> setInt(1,pn);
    res = pstmt -> executeQuery(); 
    while(res->next())
    {
        pstmt = con->prepareStatement("UPDATE PART_LOCATION SET WIP = WIP - ? AND FGI = FGI + ? WHERE P_num = ?");
        pstmt->setInt(1,qty * (res->getInt(2)));
        pstmt->setInt(2,qty * (res->getInt(2)));
        pstmt->setInt(3,res->getInt(1));
        pstmt->executeUpdate();
    }
        pstmt = con->prepareStatement("UPDATE PART_LOCATION SET FGI = FGI + ? WHERE P_num = ?");
        pstmt->setInt(1,qty);
        pstmt->setInt(2,pn);
        pstmt->executeUpdate();

    //delete con;
    //delete res;
    //delete pstmt;
}

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

    //delete con;
    //delete res;
    //delete pstmt;

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
    //delete res;
    //delete pstmt;
    //delete con;
    return i;
}



void view_inspection_requirements(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res,*res2;
    sql::PreparedStatement *pstmt,*pstmt2;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT IR_pdesc, Sample_size FROM INSP_REQ WHERE IR_pnum = ?");
    pstmt -> setInt(1,pnum);
    res = pstmt->executeQuery();
    while(res->next())
    {
        std::cout << "Inspection Requirements for: " << res->getString(1);
        std::cout << " with Sample Size: " << res->getInt(2) << std::endl;
        int count = 0;
        pstmt2 = con->prepareStatement("SELECT IR_desc, Insp_area, IR_res_type FROM REQUIREMENTS WHERE IR_pnum = ?");
        pstmt2->setInt(1,pnum);
        res2 = pstmt2->executeQuery();
        while(res2->next())
        {   
            count++;
            std::cout << count << ")\tDescription: " << res2->getString(1) << std::endl;
            std::cout << "\tInspection Area: " << res2->getString(2) << std::endl;
            std::cout << "\tResult Type: " << res2->getString(3) << std::endl;
        }
    }


    //delete res;
    //delete pstmt;
    //delete con;
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

    pstmt = con->prepareStatement("SELECT sample_size FROM INSP_REQ WHERE IR_pnum = ?");
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
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO INSPECTIONS VALUES(?,?,NULL,FALSE,NULL)");
    pstmt->setInt(1,insp_num);
    pstmt->setInt(2,pn);
    pstmt -> execute();

    pstmt = con->prepareStatement("INSERT INTO INSP_AREA VALUES(?,?,?)");
    pstmt->setInt(1,insp_num);
    pstmt->setString(2,insp_area);
    pstmt->setInt(3,insp_qty);
    pstmt->execute();

    //delete con;
    //delete pstmt;
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

    pstmt = con->prepareStatement("SELECT qty_inspected FROM INSP_AREA WHERE Insp_num = ?");
    pstmt -> setInt(1, insp_num);
    res = pstmt->executeQuery();

    if (res->next())
    {
       qty_inspected = res->getInt(1);
    }
    
    //delete con;
    //delete pstmt;
    //delete res;

    return qty_inspected;
}

int calculate_fpy(int insp_num)
{
    int qty_inspected = get_qty_inspected(insp_num);

    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    int num_req, qty_passed, i=0;
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


    //for each requirement, verify the quantity that passed matches the qty to be inspected
        //if the qty passed does not match the one inspected, set to fail, else pass

    sql::ResultSet *res2;
    sql::PreparedStatement *pstmt2;

    int total_req_passed = 0;
    double fpy;

    pstmt2 = con->prepareStatement("SELECT qty_passed FROM INSP_REQ_RES WHERE Insp_num = ?");
    pstmt2 -> setInt(1, insp_num);
    res2 = pstmt2->executeQuery();
    

    while (res2->next())
    {
        qty_passed = res2->getInt(1);

        if (qty_passed != qty_inspected)
        {
            set_insp_pf(insp_num, false);
        }
        else {  total_req_passed++; } 
        //i++;
    }

    fpy = total_req_passed / num_req;

    //delete con;
    //delete res;
    //delete pstmt;
    //delete pstmt2;
    //delete res2;

    return fpy;
}

//SQL Queries for Adding Material
//int get_next_mat_num()
void set_insp_pf(int insp_num, bool pf)     // VERIFY IF THIS MATCHES 
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("UPDATE INSPECTIONS SET pass_fail = ? WHERE Insp_num = ?");
    pstmt -> setBoolean(1, pf);
    pstmt -> setInt(2, insp_num);
    res = pstmt->executeQuery();

    
    //delete con;
    //delete pstmt;
    //delete res;

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

    //delete pstmt;
    //delete con;
}

std::vector<std::string> get_insp_req(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    std::vector<std::string>reqs;

    pstmt = con->prepareStatement("SELECT IR_desc FROM REQUIREMENTS WHERE IR_pnum = ?");
    pstmt -> setInt(1,insp_num);
    res = pstmt->executeQuery();

    while(res->next())
    {
        reqs.push_back(res->getString(1));
    }

    //delete pstmt;
    //delete con;
    //delete res;
    return reqs;
}
std::vector<std::string> get_insp_res_type(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    std::vector<std::string>results;

    pstmt = con->prepareStatement("SELECT IR_res_type FROM REQUIREMENTS WHERE IR_pnum = ?");
    pstmt -> setInt(1,insp_num);
    res = pstmt->executeQuery();

    while(res->next())
    {
        results.push_back(res->getString(1));
    }

    //delete pstmt;
    //delete con;
    //delete res;
    return results;
}

std::string get_ir_desc(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    std::string name;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    std::vector<std::string>results;

    pstmt = con->prepareStatement("SELECT IR_pdesc FROM INSP_REQ WHERE IR_pnum = ?");
    pstmt -> setInt(1,insp_num);
    res = pstmt->executeQuery();

    res ->next();
    name = res->getString(1);

    //delete pstmt;
    //delete con;
    //delete res;
    return name;
}

int get_insp_pnum(int insp_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT Insp_pnum FROM INSPECTIONS WHERE Insp_num = ?");
    pstmt -> setInt(1,insp_num);
    res = pstmt->executeQuery();

    res ->next();
    int pnum = res->getInt(1);

    //delete pstmt;
    //delete con;
    //delete res;
    return pnum;
}
//SQL Queries for Part Number
int gegt_next_p_num()
{
    return 0;
}

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
    res = stmt->executeQuery("SELECT COUNT(*)+1 FROM PART");
    res->next();

    int pnum = res->getInt(1);

    pstmt = con->prepareStatement("INSERT INTO PART VALUES(?,?,?)");
    pstmt->setInt(1,pnum);
    pstmt->setString(2,pdesc);
    pstmt->setInt(3,type);

    pstmt->execute();

    //delete con;
    //delete res;
    //delete pstmt;
    //delete stmt;
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

    //delete con;
    //delete res;
    //delete pstmt;
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

    //delete con;
    //delete res;
    //delete pstmt;
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

    pstmt = con->prepareStatement("SELECT P_desc FROM PART WHERE P_desc = ?");
    pstmt->setString(1,pname);
    res = pstmt->executeQuery();

    int id = 0;
    if(res->next()) id = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;

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

    //delete con;
    //delete res;
    //delete pstmt;

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

    //delete con;
    //delete res;
    //delete pstmt;

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

    //delete con;
    //delete res;
    //delete pstmt;
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

    //delete con;
    //delete res;
    //delete pstmt;
}

int get_parts_needed(int pnum,int mat_num)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT Mat_qty FROM PART_LIST WHERE PPr_num = ? AND PMat_num = ?");
    pstmt->setInt(1,pnum);
    pstmt->setInt(2,mat_num);
    res = pstmt->executeQuery();
    int qty = 0;
    if(res->next()) qty = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;

    return qty;
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

    //delete con;
    //delete res;
    //delete pstmt;
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

    //delete con;
    //delete res;
    //delete pstmt;
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

    //delete con;
    //delete res;
    //delete pstmt;
    return exists;
}

int get_insp_count(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT INSP FROM PART_LOCATION WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();

    int count = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;
    return count;
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

    int count = 0;
    if(res->next())count = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;
    return count;
}

int get_wip_count(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT WIP FROM PART_LOCATION WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();
    
    int count = 0;
    if(res->next()) count = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;
    return count;
}
int get_qc_count(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT QC FROM PART_LOCATION WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();

    int count = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;
    return count;
}
int get_fgi_count(int pnum)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("SELECT FGI FROM PART_LOCATION WHERE P_num = ?");
    pstmt->setInt(1,pnum);
    res = pstmt->executeQuery();

    int count = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;
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

    //delete con;
    //delete res;
    //delete pstmt;
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
    int qty = 0;
    if(res->next()) qty = res->getInt(1);

    //delete con;
    //delete res;
    //delete pstmt;
    return qty;
}

void create_requirements(int pnum, std::string requirement, std::string insp_area,std::string res_type)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO REQUIREMENTS VALUES(?,?,?,?)");
    pstmt ->setInt(1,pnum);
    pstmt->setString(2,requirement);
    pstmt->setString(3,insp_area);
    pstmt->setString(4,res_type);
    pstmt->execute();

    //delete con;
    //delete res;
    //delete pstmt;
}

void create_insp_req(int pnum, std::string pdesc,int sample_size)
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "cmpe138", "");
    con->setSchema("InventoryDB");

    pstmt = con->prepareStatement("INSERT INTO INSP_REQ VALUES(?,?,?)");
    pstmt ->setInt(1,pnum);
    pstmt->setString(2,pdesc);
    pstmt->setInt(3,sample_size);
    pstmt->execute();

    //delete con;
    //delete res;
    //delete pstmt;
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
    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET INSP = INSP - ? AND STORES = STORES + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from INSP into STORES.\n";

    //delete con;
    //delete pstmt;
    //delete res;
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
    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET STORES = STORES - ? AND WIP = WIP + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from STORES into WIP.\n";

    //delete con;
    //delete pstmt;
    //delete res;
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
    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET WIP = WIP - ? AND QC = QC + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from WIP into QC.\n";

    //delete con;
    //delete pstmt;
    //delete res;
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
    pstmt = con->prepareStatement("UPDATE PART_LOCATION SET QC = QC - ? AND FGI = FGI + ? WHERE P_num = ?");
    pstmt -> setInt(1, qty);
    pstmt -> setInt(2, qty);
    pstmt -> setInt(3, p_num);
    res = pstmt->executeQuery();

    std::cout << qty << " of part number " << p_num << " has been moved from QC into FGI.\n";

    //delete con;
    //delete pstmt;
    //delete res;
}


//Sample state insertion
void state_init()
{
    SHA256 sha256;
    create_dept(1, "Human Resources");
    create_dept(2, "Quality");
    create_dept(3, "Operations");
    create_dept(4, "Engineering");
    //departments must be created first before assigning to employees

    //create departments
    create_user("123456789", "wdoe1", sha256("wdoe1"), "Doe", "Willie");
    assign_dept(1, 2);
    assign_role(1, "IQC Inspector");

    create_user("111222333", "esmith5", sha256("esmith5"), "Smith", "Eddie");
    assign_dept(2, 2);
    assign_role(2, "OQC Inspector");

    create_user("444555666", "kmunoz2", sha256("kmunoz2"), "Munoz", "Kara");
    assign_dept(3, 2);
    assign_role(3, "QA Director");

    create_user("777888999", "csharma1", sha256("csharma1"), "Sharma", "Chas");
    assign_dept(4, 3);
    assign_role(4, "Inventory Associate");

    create_user("453453453", "lpeterson1", sha256("lpeterson1"), "Peterson", "Laila");
    assign_dept(5, 1);
    assign_role(5, "CEO");

    create_user("786786786", "oreid1", sha256("oreid1"), "Reid", "Owen");
    assign_dept(6, 3);
    assign_role(6, "Technician");

    create_user("654987321", "sadil1", sha256("sadil1"), "Adil", "Syed");
    assign_dept(7, 4);
    assign_role(7, "Engineering Manager");

    create_user("123456789", "mli7", sha256("mli7"), "Li", "Mei");
    assign_dept(8, 3);
    assign_role(8, "Operations Manager");

    create_user("123456789", "admin", sha256("admin"), "Min", "Addi");
    assign_dept(9, 1);
    assign_role(9, "Administrator");

    assign_dept_mgr(4,7);
    assign_dept_mgr(3,8);
    assign_dept_mgr(1,9);

    //assign_emp_id();
    //get_emp_id("123456789");

    //argument #2 for update_inspection_requirements is id for person performing inspection
    add_inspection(1, 10, 10, "IQC");
    update_inspection_requirements(1, 1, "10-24-2023", "Verify adapter is deburred and free of damage", "visual", 10);
    update_inspection_requirements(1, 1, "10-24-2023", "Verify light turns on when plugging in", "visual", 10);

    add_inspection(2, 1, 15, "OQC");
    update_inspection_requirements(2, 2, "10-30-2023", "Test voltage output to verify 5V +- .10V", "Volts", 15);
    update_inspection_requirements(2, 2, "10-30-2023", "Turn screen on to verify all pixels are working properly", "visual", 15);

    add_inspection(3, 1, 20, "FQC");
    update_inspection_requirements(3, 3, "11-05-2023", "Verify documentation related to build is complete", "visual", 20);
    update_inspection_requirements(3, 3, "11-05-2023", "Verify label has no smears, blurs, or bumps", "visual", 20);

}
