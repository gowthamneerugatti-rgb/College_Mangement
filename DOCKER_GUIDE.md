# 🐳 Docker Deployment Guide: The "Easy Button" 🚀

If you don't want to manually install MySQL, C++ compilers, or library files on your computer, **Docker** is the perfect solution. It packs everything into a container so it "just works."

---

## 🛠️ What You Need to Install

1.  **Docker Desktop**: [Download here](https://www.docker.com/products/docker-desktop/) (Choose the Windows version).
2.  Once installed, make sure it's **running** (you should see a little whale icon in your system tray).

---

## 🏃‍♂️ Step 1: Run the Application

This is the best part—you only need **one command** to start everything.

1.  Open **PowerShell** or **Command Prompt** in this folder.
2.  Run this command:
    ```powershell
    docker-compose up --build
    ```
    *   **What this does**: It reads the `docker-compose.yml` file, builds your C++ app, starts a MySQL database, and connects them together automatically.

---

## 🌐 Step 2: Access the App

Once you see the message `Starting College Management Web Server...` in your terminal:

1.  Open your browser.
2.  Go to: `http://localhost:8080`

**That's it! Everything is running.** 🎉

---

## 💾 Managing Your Data

*   **To Stop the App**: Press `Ctrl + C` in the terminal.
*   **To Start Again (Keeping Data)**: Just run `docker-compose up`.
*   **To Wipe Everything (Fresh Start)**: If you want to delete all entries in the database and start over, run:
    ```powershell
    docker-compose down -v; docker-compose up --build
    ```
    *(The `-v` flag deletes the "Volume" where the database was saved).*

---

## 🤔 Why use Docker instead of Local Setup?

| Feature | Local Setup | Docker |
|---|---|---|
| **Installation** | Hard (MySQL, C++, Connectors) | Easy (Just Docker) |
| **Speed to Start** | Slow (Manual setup takes ~30 mins) | Fast (Starts in ~2 mins) |
| **Cleanliness** | Messy (Installs tools on your OS) | Clean (Everything stays inside a "box") |
| **Consistency** | Risk of "it works on my machine" | Works the same everywhere |

---

## 📁 How the Docker files work

*   **Dockerfile**: The "Recipe" for your C++ app. It tells Docker headers to install and how to compile your code.
*   **docker-compose.yml**: The "Manager." It tells Docker to start both the `app` (your code) and the `db` (MySQL) and makes sure they can talk to each other.

---

**Now go build something awesome! 🚀**
