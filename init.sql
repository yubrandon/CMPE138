-- SJSU CMPE 138 FALL 2023 TEAM 5

-- Clear database
--DELETE DATABASE InventoryDB

-- Create database
CREATE DATABASE InventoryDB;

-- Create tables        insert keys
CREATE TABLE EMPLOYEE(
    ID int,
    SSN char(9),
    Lname varchar(30),
    Fname varchar(30),
    Username varchar(15),
    Pw varchar(256),
    Super_ID int,
    Dno int,
    Job_role varchar(10)

CREATE TABLE DEPARTMENT(
    Dnumber int,
    Dept_mgr char(9),  
    Dept_desc varchar(50)
);

CREATE TABLE MATERIAL{
    Pnum int,
    Pdesc varchar(50),
    SPnum int,
    SPname varchar(30),
    APnum int
};

CREATE TABLE STORAGE_LOCATION{
    Pnum int,
    INSP int,
    STORES int,
    WIP int,
    FGI int
};

CREATE TABLE PRODUCT{
    APnum int,
    APdesc varchar(50)
};

CREATE TABLE Materials{

};

CREATE TABLE INSPECTION_REQ{
    PN int,
    PNDesc varchar(50),
    Requirements varchar(10),
    Result_type varchar(10),
    Sample_size varchar(10)
};

CREATE TABLE INSP_REQ_AREA{
    PN int,
    Insp_area varchar(5)
};

CREATE TABLE INSPECTIONS{
    InspNum int,
    Requirements varchar(10),
    Pass_Fail varchar(4),
    Result varchar(30),
    Insp_date DATE
};

CREATE TABLE INSP_AREA{
    PN int,
    Insp_area varchar(5),
    Quantity int
};

-- Sample state insertion
INSERT INTO EMPLOYEE VALUES(

);

