-- College Management System - Database Schema

-- 1. Departments Table (Referenced by students & faculty)
CREATE TABLE IF NOT EXISTS departments (
    department_id INT PRIMARY KEY AUTO_INCREMENT,
    department_name VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 2. Admin Table
CREATE TABLE IF NOT EXISTS admin (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100),
    role VARCHAR(50)
);

-- 3. Students Table
CREATE TABLE IF NOT EXISTS students (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100),
    age INT,
    course VARCHAR(100),
    marks FLOAT
);

-- 4. Faculty Table
CREATE TABLE IF NOT EXISTS faculty (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100),
    department VARCHAR(100),
    designation VARCHAR(50),
    salary FLOAT
);

-- Dummy Data for Departments
INSERT INTO departments (department_name) VALUES ('Computer Science'), ('BCA'), ('BBA'), ('Electronics'), ('Mechanical'), ('Civil');
