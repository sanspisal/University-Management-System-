#include <iostream>
#include <mysql.h>
#include <sstream>
#include <windows.h>

using namespace std;

// Database connection details
const char* HOST = "localhost";
const char* USER = "root";
const char* PASSWORD = ""; // Update with your MySQL password
const char* DATABASE = "university";

// Establishing connection to the database
MYSQL* connectDB() {
    MYSQL* conn = mysql_init(0);
    conn = mysql_real_connect(conn, HOST, USER, PASSWORD, DATABASE, 3306, NULL, 0);
    if (conn) {
        cout << "Connected to database successfully!\n";
    } else {
        cout << "Database connection failed: " << mysql_error(conn) << endl;
        exit(1);
    }
    return conn;
}

// Class for University Management
class UniversityManagementSystem {
private:
    MYSQL* conn;

public:
    UniversityManagementSystem() {
        conn = connectDB();
    }

    ~UniversityManagementSystem() {
        mysql_close(conn);
    }

    // Function to create a new record
    void createRecord() {
        string name, department, role;
        int age;

        cout << "\nEnter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Role (Student/Faculty): ";
        cin >> role;

        stringstream query;
        query << "INSERT INTO records (name, department, age, role) VALUES ('" 
              << name << "', '" << department << "', " << age << ", '" << role << "');";

        if (mysql_query(conn, query.str().c_str()) == 0) {
            cout << "Record created successfully!\n";
        } else {
            cout << "Error: " << mysql_error(conn) << endl;
        }
    }

    // Function to read all records
    void readRecords() {
        string query = "SELECT * FROM records;";
        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* result = mysql_store_result(conn);
            MYSQL_ROW row;
            cout << "\nID\tName\t\tDepartment\tAge\tRole\n";
            cout << "----------------------------------------------------\n";

            while ((row = mysql_fetch_row(result))) {
                cout << row[0] << "\t" << row[1] << "\t" << row[2]
                     << "\t" << row[3] << "\t" << row[4] << endl;
            }
            mysql_free_result(result);
        } else {
            cout << "Error: " << mysql_error(conn) << endl;
        }
    }

    // Function to update a record
    void updateRecord() {
        int id;
        string column, value;
        cout << "\nEnter the ID of the record to update: ";
        cin >> id;
        cout << "Enter the column to update (name, department, age, role): ";
        cin >> column;
        cout << "Enter the new value: ";
        cin.ignore();
        getline(cin, value);

        stringstream query;
        query << "UPDATE records SET " << column << " = '" << value
              << "' WHERE id = " << id << ";";

        if (mysql_query(conn, query.str().c_str()) == 0) {
            cout << "Record updated successfully!\n";
        } else {
            cout << "Error: " << mysql_error(conn) << endl;
        }
    }

    // Function to delete a record
    void deleteRecord() {
        int id;
        cout << "\nEnter the ID of the record to delete: ";
        cin >> id;

        stringstream query;
        query << "DELETE FROM records WHERE id = " << id << ";";

        if (mysql_query(conn, query.str().c_str()) == 0) {
            cout << "Record deleted successfully!\n";
        } else {
            cout << "Error: " << mysql_error(conn) << endl;
        }
    }

    // Main menu
    void menu() {
        int choice;
        do {
            cout << "\n--- University Management System ---\n";
            cout << "1. Create Record\n";
            cout << "2. Read Records\n";
            cout << "3. Update Record\n";
            cout << "4. Delete Record\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    createRecord();
                    break;
                case 2:
                    readRecords();
                    break;
                case 3:
                    updateRecord();
                    break;
                case 4:
                    deleteRecord();
                    break;
                case 5:
                    cout << "Exiting the system. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
            Sleep(1000); // Pause for 1 second before showing menu again
        } while (choice != 5);
    }
};

// Main function
int main() {
    UniversityManagementSystem ums;
    ums.menu();
    return 0;
}
