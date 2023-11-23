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
    Pw varchar(256),
    Super_ssn int,
    Dno int,
    Job_role varchar(10)
);

CREATE TABLE DEPARTMENT(
    Dnumber int,
    Dept_mgr char(9),  
    Dept_desc varchar(50)
);

CREATE TABLE MATERIAL(
    Mat_num int,
    Mat_esc varchar(50),
    Sup_num int,
    Supp_name varchar(30)
);

CREATE TABLE STORAGE_LOCATION(
    Mat_num int,
    RECEIVED int,
    INSP int,
    STORES int,
    WIP int,
    SHIP int,
    FGI int
);

CREATE TABLE PRODUCT(
    Pr_num int,
    Pr_desc varchar(50),
    STORES int,
    FGI int
);

CREATE TABLE PART_LIST(
    PMat_num int,
    PPr_num int
);

CREATE TABLE INSPECTION_REQ(
    IR_pnum int,
    IR_pdesc varchar(50),
    Sample_size int
);

CREATE TABLE INSP_REQ_AREA(
    IR_num int,
    Insp_area varchar(5)
);

CREATE TABLE REQUIREMENTS(
    IR_pnum int,
    IR_desc varchar(50),
    IR_res_type varchar(50)
);

CREATE TABLE INSPECTIONS(
    Insp_num int,
    Insp_pnum varchar(5),
    Emp_id int,
    Pass_fail boolean,
    Insp_date DATE
);

CREATE TABLE INSP_AREA(
    Insp_num int,
    Insp_area varchar(5),
    Qty int
);

CREATE TABLE INSP_REQ_RES(
    Insp_num int,
    Insp_req varchar(50),
    Insp_res varchar(50)
);

-- Sample state insertion
-- INSERT INTO EMPLOYEE VALUES();

