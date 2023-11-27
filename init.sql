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
    Dept_mgr char(9),  
    Dept_desc varchar(50),
    CONSTRAINT DEPT_PK PRIMARY KEY (Dnumber)
);

CREATE TABLE PART(
    P_num int,
    P_desc varchar(50),
    P_type int(1),
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
    Mat_qty int
    CONSTRAINT P_MAT_FK FOREIGN KEY (PMat_num) REFERENCES PART (P_num) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT P_PROD_FK FOREIGN KEY (PPr_num) REFERENCES PART (P_num) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE INSPECTION_REQ(
    IR_pnum int,
    IR_pdesc varchar(50),
    Sample_size int,
    CONSTRAINT IR_PK PRIMARY KEY (IR_pnum)
);

CREATE TABLE INSP_REQ_AREA(
    IR_pnum int,
    Insp_area varchar(5),
    CONSTRAINT IR_AREA_FK FOREIGN KEY (IR_pnum) REFERENCES INSPECTION_REQ (IR_pnum) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE REQUIREMENTS(
    IR_pnum int,
    IR_desc varchar(50),
    IR_res_type varchar(50),
    CONSTRAINT IR_REQ_FK FOREIGN KEY (IR_pnum) REFERENCES INSPECTION_REQ (IR_pnum) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE INSPECTIONS(
    Insp_num int,
    Insp_pnum varchar(5),
    Emp_id int,
    Pass_fail boolean,
    Insp_date DATE,
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


