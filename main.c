#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>
#include <mysql.h>
#include <mysqld_error.h>
#include "utils.h"

// Macro definitions
#define PW "Shyamsql@123"
#define User "root"
#define localhost "localhost"
#define database "SupermarketDB"

// Function to establish a connection to the MySQL database
MYSQL* connectToDatabase() {
    MYSQL *conn = mysql_init(NULL); // Initialize MySQL object
    if (conn == NULL) {
        printf("mysql_init() failed\n");
        return NULL;
    }

    // Establish connection to the MySQL database
    if (mysql_real_connect(conn, localhost, User, PW, database, 0, NULL, 0) == NULL) {
        printf("mysql_real_connect() failed\n");
        printf("Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    printf("Connection to MySQL database successful!\n");
    return conn;
}

// Main function for testing connection
int main() {
    MYSQL *conn = connectToDatabase();
    if (conn) {
        mysql_close(conn);
    }

    return 0;
}

