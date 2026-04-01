# 🎓 College Management System (CMS)

A full-stack web application to manage Students, Faculty, and Admins for a university. Built with **C++** on the backend, **MySQL** for the database, and plain **HTML/CSS/JavaScript** for the frontend.

---

## 📚 Project Documentation

Quick links to all the guides we've created:

*   🚀 [**Quick Start (Docker)**](./DOCKER_GUIDE.md) — The easiest way to run the app in 2 minutes.
*   💻 [**Local Setup (Windows)**](#-local-setup-guide) — How to install everything manually on your PC.
*   📜 [**Development Changelog**](./CHANGELOG.md) — A step-by-step history of how this project was built (19-year-old friendly!).

---

## 🚀 Local Setup Guide

Follow these steps to run the app directly on your Windows machine without Docker.

### 📋 Table of Contents
1. [What You Need to Install First](#-what-you-need-to-install-first)
2. [Step 1: Install MySQL](#-step-1-install-mysql)
3. [Step 2: Set Up the Database](#-step-2-set-up-the-database)
4. [Step 3: Install the C++ MySQL Connector](#-step-3-install-the-c-mysql-connector)
5. [Step 4: Update the Database Connection Settings](#-step-4-update-the-database-connection-settings)
6. [Step 5: Compile the Project](#-step-5-compile-the-project)
7. [Step 6: Run the Application](#-step-6-run-the-application)
8. [Folder Structure Explained](#-folder-structure-explained)
9. [Troubleshooting](#-troubleshooting)

---

## 🧰 What You Need to Install First

Before you start, make sure you have these installed on your Windows PC:

| Tool | What is it? | Where to get it |
|------|-------------|-----------------|
| **Dev-C++** or **MinGW g++** | A C++ compiler — it turns your code into an app | [Download Dev-C++](https://sourceforge.net/projects/orwelldevcpp/) |
| **MySQL Server 8.0** | The database that stores all your data | [Download MySQL](https://dev.mysql.com/downloads/installer/) |
| **MySQL Connector/C++** | A library so your C++ code can talk to MySQL | [Download Connector](https://dev.mysql.com/downloads/connector/cpp/) |

---

## 🐬 Step 1: Install MySQL

1. Download the **MySQL Installer** from the link above.
2. Run the installer and choose **"Developer Default"** setup type.
3. During installation, it will ask you to set a **root password**. 
   - Use: `rootpassword` (or anything you like, but **remember it!**)
4. Finish the installation and make sure the MySQL service is **running**.

---

## 🗄️ Step 2: Set Up the Database

1. Open **Command Prompt** and connect to MySQL:
   ```bash
   mysql -u root -p
   ```
2. Create the database:
   ```sql
   CREATE DATABASE college_db;
   USE college_db;
   ```
3. Run the SQL file to create tables:
   ```sql
   SOURCE C:/Users/YOUR_USERNAME/Downloads/Projects/college_management/sql/database.sql;
   ```

---

## 🔌 Step 3: Install the C++ MySQL Connector

1. Install it to `C:\Program Files\MySQL\MySQL Connector C++ 8.0`.
2. Paths you'll need:
   - **Include**: `...\include\`
   - **Library**: `...\lib64\`

---

## ⚙️ Step 4: Update Connection Settings

In `config/db_config.h`, change the host to `127.0.0.1`:

```cpp
std::string host = "tcp://127.0.0.1:3306";
std::string password = "your_mysql_password";
```

---

## 🔨 Step 5: Compile the Project

### Using Dev-C++
1. Create a new **Console Project**.
2. Add all `.cpp` files from the project.
3. In **Project Options**:
   - **Directories**: Add the MySQL Include and Lib64 paths.
   - **Parameters**: Add `-lws2_32 -lmysqlcppconn` to the Linker.
   - **Compiler**: Add `-std=c++11`.
4. Press **F11**.

---

## 🚀 Step 6: Run the Application

1. Run `college_app.exe`.
2. Open: **http://localhost:8080**

---

## 📁 Folder Structure Explained

*   `public/`: The website (HTML/JS/CSS).
*   `services/`: The logic for Admins, Students, and Faculty.
*   `sql/`: The database table setups.
*   `config/`: Connection settings.

---

## 🛑 Troubleshooting

Check out [DOCKER_GUIDE.md](./DOCKER_GUIDE.md) if you're hitting issues and want an easier way to run it using Docker!

---

**Made with ❤️ using C++, MySQL, and plain HTML/JS**
