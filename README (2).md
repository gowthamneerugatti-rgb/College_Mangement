# College Management System — Development Changelog

**Project:** College Management System (CMS)  
**Stack:** C++ (cpp-httplib) · MySQL 8.0 · Docker · Docker Compose · HTML/CSS/JS  
**Author:** Jaya Krishna Sai  

---

## Overview

This document describes every significant change made to the codebase during development — from the initial monolithic prototype to the fully containerized, database-backed 3-tier web application.

---

## Phase 1: Architecture Refactoring (Monolith → Modular)

### Problem
The original `main.cpp` was a single large file containing all HTTP route handlers, mock in-memory data storage using `std::vector`, and hardcoded data — all in one place. This was not scalable or maintainable.

### Changes Made

#### `main.cpp` — Refactored Entry Point
- **Before:** Contained all API route logic (~180 lines), hardcoded mock DB name (`college_db_mock`), and all data stored in `std::vector` arrays.
- **After:** Slim orchestration file (~43 lines). Now only:
  1. Initializes the database connection via `config::initDatabase()`
  2. Creates the HTTP server
  3. Delegates routes to service modules
  4. Mounts the `public/` folder as static file server
  5. Starts the server on `0.0.0.0:8080`

```cpp
// Removed hardcoded override:
// dbCfg.db_name = "college_db_mock";  ← DELETED
```

#### `services/admin_service.cpp` — New File
- Extracted all Admin API routes from `main.cpp`
- Replaced `std::vector<Admin>` in-memory storage with MySQL `PreparedStatement` queries
- Routes: `GET /api/admin/all`, `POST /api/admin/add`, `DELETE /api/admin/delete`

#### `services/student_service.cpp` — New File
- Extracted all Student API routes from `main.cpp`
- Replaced `std::vector<Student>` in-memory storage with MySQL queries
- Routes: `GET /api/student/all`, `POST /api/student/add`, `DELETE /api/student/delete`

#### `services/faculty_service.cpp` — New File
- Extracted all Faculty API routes from `main.cpp`
- Replaced `std::vector<Faculty>` in-memory storage with MySQL queries
- Routes: `GET /api/faculty/all`, `POST /api/faculty/add`, `DELETE /api/faculty/delete`

---

## Phase 2: Database Integration (MySQL Connector/C++)

### Problem
The system had zero persistence — all data was lost every time the application restarted.

### Changes Made

#### `config/db_config.h` — New File
- Defined `DbConfig` struct holding `host`, `user`, `password`, `db_name`
- Reads all values from **environment variables** (`DB_HOST`, `DB_USER`, `DB_PASS`, `DB_NAME`)
- No hardcoded credentials anywhere

#### `config/database_connection.cpp` — New File
- Implemented `initDatabase()` using the MySQL Connector/C++ (`cppconn`) driver
- Implemented a **retry loop (10 attempts, 3s apart)** to handle the race condition where the app starts before the MySQL container is fully ready
- Implemented `getConnection()` singleton accessor used by all service files
- On complete failure, calls `exit(1)` with a fatal error message

#### `sql/database.sql` — Rewritten Schema
- Removed hardcoded `CREATE DATABASE` statements (handled by Docker environment)
- Created 4 tables matching the C++ model structs exactly:
  - `admin (id, name, role)`
  - `students (id, name, age, course, marks)`
  - `faculty (id, name, department, designation, salary)`
  - `departments (department_id, department_name)` with seed data

#### SQL Table Name Bug Fix
- All original queries used **PascalCase** table names (`Students`, `Admin`, `Faculty`)
- MySQL on Linux is **strictly case-sensitive** for table names
- **Fixed:** Changed all queries to lowercase to match the schema (`students`, `admin`, `faculty`)

---

## Phase 3: Containerization (Docker)

### Problem
The application needed to run consistently across environments without manual dependency installation.

### Changes Made

#### `Dockerfile` — New File (Multi-Stage Build)
**Stage 1 — Builder:**
- Base image: `debian:bookworm-slim`
- Installs: `g++`, `libmysqlcppconn-dev`
- Compiles: `main.cpp` + all service/config files → `college_app` binary
- Compiler flags: `-lpthread`, `-lmysqlcppconn`, `-std=c++11`

**Stage 2 — Runtime:**
- Base image: `debian:bookworm-slim` (clean, no build tools)
- Installs only the runtime library: `libmysqlcppconn-dev`
- Copies only the compiled binary and `public/` folder from Stage 1
- Uses `apt-get clean && rm -rf /var/lib/apt/lists/*` to minimize image size

> **Note:** Package `libmysqlcppconn9` (Debian Bullseye) was replaced with `libmysqlcppconn-dev` (Debian Bookworm) to resolve a build failure caused by package availability differences between Debian versions.

#### `docker-compose.yml` — New File
Orchestrates two services:

**`db` service:**
- Image: `mysql:8.0`
- Sets `MYSQL_ROOT_PASSWORD=rootpassword`, `MYSQL_DATABASE=college_db`
- Mounts `./sql/` → `/docker-entrypoint-initdb.d/` for automatic schema provisioning on first boot
- Exposes port `3306`

**`app` service:**
- Built from the local `Dockerfile`
- Injects DB connection as environment variables: `DB_HOST=db`, `DB_USER=root`, `DB_PASS=rootpassword`, `DB_NAME=college_db`
- Mounts `./public/` → `/app/public/` as a **live volume** (HTML/JS/CSS changes reflect without rebuilding)
- `depends_on: db` ensures the DB container starts first
- `restart: unless-stopped` for resilience

#### `.dockerignore` — New File
- Excludes `build/`, `.vscode/`, `*.o`, `*.exe`, `*.rtf` from the Docker build context to keep builds fast and image size small

---

## Phase 4: Frontend — JavaScript Extraction

### Problem
All JavaScript logic was embedded inline within `<script>` tags inside each HTML file, making it hard to maintain and violating separation of concerns.

### Changes Made

#### `public/student.js` — New File
- Extracted all JS from `student.html`
- Functions: `loadStudents()`, form submit handler, `deleteStudent(id)`
- Uses `fetch('/api/student/all')` and `fetch('/api/student/add', ...)`

#### `public/admin.js` — New File
- Extracted all JS from `admin.html`
- Functions: `loadAdmins()`, form submit handler, `deleteAdmin(id)`
- Uses `fetch('/api/admin/all')` and `fetch('/api/admin/add', ...)`

#### `public/faculty.js` — New File
- Extracted all JS from `faculty.html`
- Functions: `loadFaculty()`, form submit handler, `deleteFaculty(id)`
- Uses `fetch('/api/faculty/all')` and `fetch('/api/faculty/add', ...)`

#### `admin.html`, `student.html`, `faculty.html` — Updated
- Replaced `<script>` inline blocks with `<script type="module" src="*.js"></script>`

> **Important:** `type="module"` is required because the JS files use top-level `await`. Without it, the browser silently fails to execute any code past the first `await`, which caused the admin list to never load even though the backend API was working correctly.

---

## Phase 5: Code Quality & Accessibility Fixes

### JavaScript (`admin.js`, `student.js`, `faculty.js`)
| Issue | Fix |
|---|---|
| `parseInt()` | Replaced with `Number.parseInt()` |
| `parseFloat()` | Replaced with `Number.parseFloat()` |
| Silent catch blocks | Added `throw e` to properly propagate errors |
| Bare function call at top level | Changed to `await loadXxx()` (enabled by `type="module"`) |

### HTML (`admin.html`, `student.html`, `faculty.html`)
| Issue | Fix |
|---|---|
| Missing `lang` attribute on `<html>` | Added `lang="en"` to `student.html` and `faculty.html` |
| Form labels not associated with inputs | Added `for="inputId"` on all `<label>` elements |
| Low contrast button colors (`#1abc9c`) | Replaced with higher-contrast equivalents (`#138a72`) |

---

## Final Architecture

```
college_management/
├── Dockerfile                  ← Multi-stage Docker build
├── docker-compose.yml          ← Orchestrates app + db containers
├── main.cpp                    ← Slim entry point: init DB, register routes, start server
├── httplib.h                   ← Header-only C++ HTTP server library
├── config/
│   ├── db_config.h             ← DbConfig struct, reads env vars
│   └── database_connection.cpp ← MySQL connection init with retry logic
├── models/
│   ├── admin.h                 ← Admin struct
│   ├── student.h               ← Student struct
│   └── faculty.h               ← Faculty struct (+ others)
├── services/
│   ├── admin_service.cpp       ← Admin CRUD routes → MySQL
│   ├── student_service.cpp     ← Student CRUD routes → MySQL
│   └── faculty_service.cpp     ← Faculty CRUD routes → MySQL
├── utils/
│   └── helper_functions.h      ← JSON string parsing helpers
├── sql/
│   └── database.sql            ← Schema (auto-loaded by MySQL container on first boot)
└── public/
    ├── admin.html              ← Admin management UI
    ├── admin.js                ← Admin API fetch logic
    ├── student.html            ← Student management UI
    ├── student.js              ← Student API fetch logic
    ├── faculty.html            ← Faculty management UI
    └── faculty.js              ← Faculty API fetch logic
```

---

## How to Run

```bash
# First run (or after code changes):
docker-compose down -v && docker-compose up --build

# Subsequent runs (no code changes):
docker-compose up

# Access the application:
# http://localhost:8080
```

> **Note:** PowerShell does not support `&&`. Use `;` instead:
> `docker-compose down -v; docker-compose up --build`

---

## Known Behaviours

| Behaviour | Explanation |
|---|---|
| IDE shows `cppconn/driver.h not found` | MySQL headers only exist inside the Docker container, not on the Windows host. The build inside Docker succeeds correctly. |
| `docker-compose down -v` wipes all data | The `-v` flag removes volumes. Use `docker-compose down` (without `-v`) to stop without deleting the database. |
| HTML/JS/CSS changes are instant | The `public/` folder is mounted as a live Docker volume — refresh the browser to see changes without rebuilding. |
| C++ code changes require rebuild | Run `docker-compose up --build` after modifying any `.cpp` or `.h` file. |
