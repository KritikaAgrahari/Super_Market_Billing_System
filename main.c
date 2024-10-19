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

// Main Function
int main() {
    MYSQL *conn = connectToDatabase();
    if (conn == NULL) {
        printf("Error: %s\n", mysql_error(conn));
        return 1;
    } else {
        printf("Logged In Database!\n");
    }

    int shift = 7;
    int exitFlag = 0;
    bool userAuthenticated = false;  // Flag to track if user is successfully signed in

    while (!exitFlag && !userAuthenticated) {
        system("cls");
        printf("1. Signup.\n");
        printf("2. SignIn.\n");
        printf("0. Exit.\n");
        printf("Enter Your Choice: ");
        int val;
        scanf("%d", &val);

        if (val == 1) {
            system("cls");
            char id[50], pw[50], encryptedPW[50];
            printf("Enter ID for Signup: ");
            scanf("%s", id);
            printf("Enter A Strong Password: ");
            scanf("%s", pw);

            // Encrypt the password
            encrypt(pw, shift, encryptedPW);

            // Insert the encrypted password into the database
            char signupQuery[256];
            sprintf(signupQuery, "INSERT INTO password (Id, PW) VALUES ('%s', '%s')", id, encryptedPW);
            if (mysql_query(conn, signupQuery)) {
                printf("Error: %s\n", mysql_error(conn));
            } else {
                printf("Signup Successfully\n");
            }
            Sleep(3000);

        } else if (val == 2) {
            system("cls");
            char id[50], pw[50], encryptedDBPassword[50], decryptedPW[50];
            printf("Enter ID: ");
            scanf("%s", id);
            printf("Enter Your Password: ");
            scanf("%s", pw);

            // Retrieve the encrypted password from the database
            getDBPassword(conn, id, encryptedDBPassword);

            if (strlen(encryptedDBPassword) > 0) {
                // Decrypt the password from the database
                decrypt(encryptedDBPassword, shift, decryptedPW);

                // Check if the entered password matches the decrypted password
                if (strcmp(decryptedPW, pw) == 0) {
                    printf("Login Successful! Welcome!\n");
                    userAuthenticated = true;  // User successfully signed in
                } else {
                    printf("Incorrect Password. Try Again!\n");
                }
            } else {
                printf("User ID not found. Try Again!\n");
            }
            Sleep(2000);

        } else if (val == 0) {
            exitFlag = 1;
            printf("Bye!\n");

        } else {
            printf("Invalid Input\n");
        }
    }

    // If user is authenticated, start further process
    if (userAuthenticated) {
    	system("cls");
        printf("\nStarting further process...\n");
        
		joinShow(conn);

		int var1, var2, var3;
		bool a=true, b;
    // Main loop for operations
    while (a) {
        system("cls");
        displayMainMenu();
        scanf("%d", &var1);

        switch (var1) {
            case 1:
                b = true;
                while (b) {
                    system("cls");
                    printf("Select an option for performing operations on Products.\n");
                    displayOperationMenu();
                    scanf("%d", &var2);
                    switch (var2) {
                        case 1:
                            addProduct(conn);
                            break;
                        case 2:
                            removeProduct(conn, -1); // Pass actual product_id here
                            break;
                        case 3:
                            searchProduct(conn);
                            break;
                        case 4:
                            updateProduct(conn);
                            break;
                        case 0:
                            b = false;
                            break;
                        default:
                            printf("Invalid Operation. Try again!\n");
                    }
                    Sleep(3000);
                }
                break;

            case 2:
                b = true;
                while (b) {
                    system("cls");
                    printf("Select an option for performing operations on Customers.\n");
                    displayOperationMenu();
                    scanf("%d", &var2);
                    switch (var2) {
                        case 1:
                            addCustomer(conn);
                            break;
                        case 2:
                            removeCustomer(conn, -1); // Pass actual customer_id here
                            break;
                        case 3:
                            searchCustomer(conn);
                            break;
                        case 4:
                            updateCustomer(conn);
                            break;
                        case 0:
                            b = false;
                            break;
                        default:
                            printf("Invalid Operation. Try again!\n");
                    }
                    Sleep(3000);
                }
                break;

            case 3:
                b = true;
                while (b) {
                    system("cls");
                    printf("Select an option for performing operations on Orders.\n");
                    displayOperationMenu();
                    scanf("%d", &var2);
                    switch (var2) {
                        case 1:
                            createOrder(conn);
                            break;
                        case 2:
                            deleteOrder(conn, -1); // Pass actual order_id here
                            break;
                        case 3:
                            searchOrder(conn);
                            break;
                        case 4:
                            updateOrder(conn);
                            break;
                        case 0:
                            b = false;
                            break;
                        default:
                            printf("Invalid Operation. Try again!\n");
                    }
                    Sleep(3000);
                }
                break;

            case 4:
                b = true;
                while (b) {
                    system("cls");
                    printf("Select an option for performing operations on OrderItems.\n");
                    displayOperationMenu();
                    scanf("%d", &var2);
                    switch (var2) {
                        case 1:
                            addOrderItem(conn);
                            break;
                        case 2:
                            deleteOrderItem(conn, -1); // Pass actual order_item_id here
                            break;
                        case 3:
                            searchOrderItem(conn);
                            break;
                        case 4:
                            updateOrderItem(conn);
                            break;
                        case 0:
                            b = false;
                            break;
                        default:
                            printf("Invalid Operation. Try again!\n");
                    }
                    Sleep(3000);
                }
                break;

            case 0:
                a = false;
                break;

            default:
                printf("Invalid Option. Please try again!\n");
                break;
        }
        
    }

    // Close the database connection
    mysql_close(conn);
    return 0;
}
}

