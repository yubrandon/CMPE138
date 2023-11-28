-- SJSU CMPE 138 FALL 2023 TEAM 5;

-- Clear database;
DROP DATABASE IF EXISTS InventoryDB;

-- Create Database;
CREATE DATABASE InventoryDB;

-- Select database;
USE InventoryDB;

-- Create tables;
CREATE TABLE EMPLOYEE(
    ID int,
    SSN char(9),
    Lname varchar(30),
    Fname varchar(30),
    Username varchar(15),
    Pw varchar(512),
    CONSTRAINT ID_PK PRIMARY KEY (ID)
);

CREATE TABLE EMPLOYEE_INFO(
    ID int,
    Dno int,
    Job_role varchar(10),
    CONSTRAINT EMP_ID_FK FOREIGN KEY (ID) REFERENCES EMPLOYEE(ID) ON DELETE SET NULL ON UPDATE CASCADE 
);

CREATE TABLE DEPARTMENT(
    Dnumber int,
    Dept_mgr int,  
    Dept_desc varchar(50),
    CONSTRAINT DEPT_PK PRIMARY KEY (Dnumber),
    CONSTRAINT DEPT_MGR_FK FOREIGN KEY (Dept_mgr) REFERENCES EMPLOYEE (ID) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE PART(
    P_num int,
    P_desc varchar(50),
    P_type int,
    CONSTRAINT P_PK PRIMARY KEY (P_num)
);

CREATE TABLE PART_LOCATION(
    P_num int,
    INSP int,
    STORES int,
    WIP int,
    QC int,
    FGI int,
    CONSTRAINT P_LOC_FK FOREIGN KEY (P_num) REFERENCES PART(P_num) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE PART_SUPPLIER(
    P_num int,
    Supp_num int,
    Supp_name varchar(50),
    CONSTRAINT P_SUPP_FK FOREIGN KEY (P_num) REFERENCES PART (P_num) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE PART_LIST(
    PMat_num int,
    PPr_num int,
    Mat_qty int,
    CONSTRAINT P_MAT_FK FOREIGN KEY (PMat_num) REFERENCES PART (P_num) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT P_PROD_FK FOREIGN KEY (PPr_num) REFERENCES PART (P_num) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE INSP_REQ(
    IR_pnum int,
    IR_pdesc varchar(50),
    Sample_size int,
    CONSTRAINT IR_PK PRIMARY KEY (IR_pnum)
);

CREATE TABLE REQUIREMENTS(
    IR_pnum int,
    IR_desc varchar(200),
    Insp_area varchar(5),
    IR_res_type varchar(50),
    CONSTRAINT IR_REQ_FK FOREIGN KEY (IR_pnum) REFERENCES INSP_REQ (IR_pnum) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE INSPECTIONS(
    Insp_num int,
    Insp_pnum int,
    Emp_id int,
    Pass_fail boolean,
    Insp_date varchar(30),
    CONSTRAINT INSP_PK PRIMARY KEY (Insp_num),
    CONSTRAINT INSP_FK FOREIGN KEY (Insp_pnum) REFERENCES PART (P_num) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE INSP_AREA(
    Insp_num int,
    Insp_area varchar(5),
    Qty int,
    CONSTRAINT INSP_AREA_FK FOREIGN KEY (Insp_num) REFERENCES INSPECTIONS (Insp_num) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE INSP_REQ_RES(
    Insp_num int,
    Insp_req varchar(50),
    Insp_res varchar(50),
    CONSTRAINT INSP_RES_FK FOREIGN KEY (Insp_num) REFERENCES INSPECTIONS (Insp_num) ON DELETE SET NULL ON UPDATE CASCADE
);

INSERT INTO PART VALUES ( 1, "15 inch monitor", 1);
INSERT INTO PART VALUES ( 2, "ComputerA", 1);
INSERT INTO PART VALUES ( 3, "ComputerB", 1);
INSERT INTO PART VALUES ( 4, "45 inch monitor", 1);
INSERT INTO PART VALUES ( 5, "15 inch TN panel", 0);
INSERT INTO PART VALUES ( 6, "scaler board (1920 x 1200)", 0);
INSERT INTO PART VALUES ( 7, "16 inch casing", 0);
INSERT INTO PART VALUES ( 8, "18 inch housing", 0);
INSERT INTO PART VALUES ( 9, "Power adapter(A)", 0);
INSERT INTO PART VALUES (10, "45 inch IPS panel", 0);
INSERT INTO PART VALUES (11, "scaler board (4K UHD)", 0);
INSERT INTO PART VALUES (12, "47 inch casing", 0);
INSERT INTO PART VALUES (13, "50 inch housing", 0);
INSERT INTO PART VALUES (14, "Power adapter(B)", 0);

INSERT INTO PART_LOCATION VALUES ( 1, 0, 5, 4, 2, 5);
INSERT INTO PART_LOCATION VALUES ( 2, 0, 6, 1, 5, 1);
INSERT INTO PART_LOCATION VALUES ( 3, 0, 3, 2, 4, 2);
INSERT INTO PART_LOCATION VALUES ( 4, 0, 7, 4, 3, 8);

INSERT INTO PART_LOCATION VALUES ( 5, 10, 15, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES ( 6, 15, 20, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES ( 7, 28, 46, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES ( 8, 16, 45, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES ( 9, 48, 25, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES (10, 42, 15, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES (11, 18, 26, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES (12, 15, 26, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES (13, 51, 45, 0, 0, 0);
INSERT INTO PART_LOCATION VALUES (14, 46, 25, 0, 0, 0);

INSERT INTO PART_SUPPLIER VALUES ( 5, 1681, "Lowes");
INSERT INTO PART_SUPPLIER VALUES ( 6, 6541, "Amazon");
INSERT INTO PART_SUPPLIER VALUES ( 7, 8165, "Lowes");
INSERT INTO PART_SUPPLIER VALUES ( 8, 3156, "Digikey");
INSERT INTO PART_SUPPLIER VALUES ( 9, 8941, "Lowes");
INSERT INTO PART_SUPPLIER VALUES (10, 6571, "McMaster");
INSERT INTO PART_SUPPLIER VALUES (11, 1864, "Lowes");
INSERT INTO PART_SUPPLIER VALUES (12, 4485, "McMaster");
INSERT INTO PART_SUPPLIER VALUES (13, 8282, "Lowes");
INSERT INTO PART_SUPPLIER VALUES (14, 9746, "Digikey");

INSERT INTO PART_LIST VALUES ( 5,  1, 1);
INSERT INTO PART_LIST VALUES ( 6,  1, 1);
INSERT INTO PART_LIST VALUES ( 7,  1, 1);
INSERT INTO PART_LIST VALUES ( 8,  1, 1);
INSERT INTO PART_LIST VALUES ( 9,  1, 1);

INSERT INTO PART_LIST VALUES ( 4, 10, 1);
INSERT INTO PART_LIST VALUES ( 4, 11, 1);
INSERT INTO PART_LIST VALUES ( 4, 12, 1);
INSERT INTO PART_LIST VALUES ( 4, 13, 1);
INSERT INTO PART_LIST VALUES ( 4, 14, 1);

INSERT INTO INSP_REQ VALUES ( 1, "15 inch monitor", 1);
INSERT INTO INSP_REQ VALUES (10, "Power adapter(A)", 1);

INSERT INTO REQUIREMENTS VALUES (10, "Verify adapter is deburred and free", "IQC", "visual");
INSERT INTO REQUIREMENTS VALUES (10, "Verify light turns on when plugging in", "IQC", "visual");
INSERT INTO REQUIREMENTS VALUES ( 1, "Test voltage output to verify 5V +- .10V", "OQC", "Volts");
INSERT INTO REQUIREMENTS VALUES ( 1, "Turn screen on to verify all pixels are working properly", "OQC", "visual");
INSERT INTO REQUIREMENTS VALUES ( 1, "Verify documentation related to build is complete", "FQC", "visual");
INSERT INTO REQUIREMENTS VALUES ( 1, "Verify label has no smears, blurs, or bumps", "FQC", "visual");


INSERT INTO INSP_REQ_RES VALUES (1, "Verify adapter is deburred and free of damage", "pass", 10);
INSERT INTO INSP_REQ_RES VALUES (1,"Verify light turns on when plugging in", "pass", 10);
INSERT INTO INSP_REQ_RES VALUES (2, "Test voltage output to verify 5V +- .10V", "5", 15);
INSERT INTO INSP_REQ_RES VALUES (2, "Turn screen on to verify all pixels are working properly", "pass", 15);
INSERT INTO INSP_REQ_RES VALUES (3, "Verify documentation related to build is complete", "pass", 20);
INSERT INTO INSP_REQ_RES VALUES (3, "Verify label has no smears, blurs, or bumps", "fail", 18);