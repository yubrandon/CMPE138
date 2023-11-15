-- SJSU CMPE 138 FALL 2023 TEAM 5

-- user_exists 
SELECT username
FROM employee
WHERE EXISTS (SELECT *
              FROM employee
              WHERE username=user);

-- ssn_exists 
SELECT ssn
FROM employee
WHERE EXISTS (SELECT *
              FROM employee
              WHERE ssn=ssn);

-- verify_user 
SELECT username,password
FROM employee
WHERE EXISTS (SELECT username,password
              FROM employee);

-- create_user
SELECT COUNT(*)+1 -- query saved into id variable
FROM employee;

INSERT INTO employee VALUES (ssn, id, name, user, pw);