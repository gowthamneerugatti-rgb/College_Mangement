# 🔌 Database Connection Guide

This guide explains how your C++ application connects to the MySQL database and where you can find or change the "Connection String" settings.

---

## 📍 Where is the connection info?

Unlike some apps that use a single long text string, this project stores the connection details in a **C++ Structure**.

You can find and edit these settings in this file:
📂 `config/db_config.h`

### The "Connection String" Parts:
Inside that file, you will see the `DbConfig` struct:

```cpp
struct DbConfig {
    std::string host = "tcp://db:3306";      // The address of the database
    std::string user = "root";               // Your MySQL username
    std::string password = "rootpassword";   // Your MySQL password
    std::string db_name = "college_db";      // The name of the database
};
```

---

## 🚀 1. If you are using Docker (Easiest)

In Docker, the database service is named `db`. 

*   **Host**: Should stay as `tcp://db:3306`.
*   **Username**: `root`
*   **Password**: `rootpassword` (This matches what is set in your `docker-compose.yml` file).

> [!TIP]
> If you change the password in `docker-compose.yml`, you **must** also change it in `db_config.h` and rebuild the app using:
> `docker-compose up --build`

---

## 💻 2. If you are running locally (Windows / Dev-C++)

If you installed MySQL directly on your Windows PC, you need to tell the app to look at your own machine instead of the Docker network.

*   **Host**: Change to `tcp://127.0.0.1:3306`. 
    *   (`127.0.0.1` is the "Home" IP address for your own computer).
*   **Username**: Usually `root`.
*   **Password**: Use the password you chose when you installed MySQL on your PC.

---

## 🛠️ How it works in the code

If you're curious about how the connection actually happens, look at:
📂 `config/database_connection.cpp`

The code uses a library called `sql::Driver` which takes those 4 pieces of information and opens a "tunnel" to the database:

1.  `driver->connect(config.host, config.user, config.password)`
2.  `conn->setSchema(config.db_name)`

**That's it! Any time you change your database password or host, `config/db_config.h` is the only place you need to go.**
