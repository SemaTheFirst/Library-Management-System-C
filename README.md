# Library-Management-System-C

A comprehensive, console-based Library Management System developed using the C programming language. This project strictly adheres to structured programming principles and demonstrates advanced memory management techniques.

## ✨ Features
* **Dynamic Memory Allocation:** Efficient use of `malloc`, `realloc`, and `free` to handle flexible amounts of student, author, and book data without memory leaks.
* **Relational Data Management:** Links authors to their specific books using function pointers and dynamic matching arrays.
* **Persistent Storage:** Real-time reading and writing of data into multiple `.csv` files (`Students.csv`, `Books.csv`, `authors.csv`, etc.).
* **Advanced Tracking & Penalty System:** Tracks borrowed and on-shelf books using `enum` and `union` structures, automatically calculating return delays and deducting library scores for books returned later than 15 days.

## 🚀 How to Run

This project is built with standard C libraries and is fully cross-platform (can run on Windows, Linux, or macOS). 

1. **Clone the repository:**
   ```bash
   git clone https://github.com/SemaTheFirst/Library-Management-System-C.git
   
Navigate to the project directory and compile: You can use any standard C compiler like GCC.

Bash
gcc main.c -o library_system

Run the program:

On Windows: .\library_system.exe
On Linux/Mac: ./library_system

(Note: On the very first run, the system will securely auto-generate the required .csv database files if they do not exist.)

## 🖥️ How to Use

* **Start:** Run the executable to open the main console menu.

* **Navigation:** The system operates on a 15-item numerical menu. Simply type the number of the operation you want to perform and press Enter.

* **Core Operations:**

   * Data Entry: Add students, authors, and books. (Names can include spaces, and ISBNs are standard 13-digit formats).

   * Relational Matching: Link authors to their books via Menu 11.

   * Borrow & Return: Issue books to students using system-generated Tag Numbers.

   * Penalty Tracking: Check overdue books (Menu 10) by entering the current date (DD.MM.YYYY). The system automatically applies a 15-day deadline rule and decreases the student's library score if late.

* **Exit:** Always use Menu 15 to exit safely. This ensures all dynamic memory is freed and modifications are saved to the CSV files.

## 🛠️ Technical Details

* **Language:** C

* **Core Libraries:** <stdio.h>, <stdlib.h>, <string.h>

* **Key Architecture & Concepts:** Dynamic Memory Management: Extensive use of malloc, realloc, and free to prevent memory leaks and handle flexible array sizes (Segmentation Fault resilient).

* **Data Structures:** Implementation of Linked Lists, Structs, Unions, and Enums.

* **Relational Database Logic:** Real-time CSV File I/O operations with synchronization across multiple data files.

* **Advanced C Features:** Function Pointers (to maintain the DRY principle during list filtering) and array decay management.
