# C-Language-Project-1.

1.This project is a simple game developed in the C programming language. It is designed to demonstrate the use of basic C concepts such as loops, conditional statements, functions, arrays, and user input/output handling. The game runs in the command-line interface (CLI), making it lightweight and easy to execute on any system with a C compiler.

2.**Project Title:** Electricity Board Management System (EBMS)

**Language:** C Programming Language

**Project Type:** Console-Based Desktop Application

**Description:**
The Electricity Board Management System (EBMS) is a console-based C project designed to manage electricity board operations involving user registrations and electricity connection requests. The system offers functionalities for both electricity board users (citizens) and administrators.

**Core Functionalities:**

1. **User Management:**

   * Add new users with unique ID, name, and contact information.
   * View user details including personal information and their associated electricity connection requests.

2. **Connection Request Management:**

   * Users can place a request for a new electricity connection.
   * Each request is associated with a user ID and includes optional comments and address details.
   * View detailed status and information about each request.

3. **Admin Controls:**

   * Admins can edit existing requests by updating their status (Pending, In Progress, Rejected).
   * Admins can also add comments to the requests for further clarification.

**File Handling:**

* Data persistence is achieved using binary files:

  * `users.dat` stores user records.
  * `requests.dat` stores connection request records.

**Structure Design:**

* `User` structure contains: ID, name, contact.
* `Request` structure contains: ID, user ID, user comment, address, status, admin comment.

**Features:**

* Console UI with menu-driven navigation.
* Clear prompts and feedback for all user actions.
* Uses `getch()`/`getchar()` for pause and interaction control.
* Cross-platform friendly code (`#ifdef _WIN32` for platform-specific operations).

**Advantages:**

* Simple and effective data management system.
* Fully functional offline system without database dependency.
* Lightweight, portable, and easily modifiable for future enhancements.

**Ideal For:**

* Beginner to intermediate C programming learners.
* Students looking for a practical file handling project in C.

**Executable & Data Files:**

* `Project-EBMS.exe`: Compiled application.
* `users.dat` and `requests.dat`: Binary data files storing user and request info.
