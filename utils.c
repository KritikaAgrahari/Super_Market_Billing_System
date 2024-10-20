#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>
#include <mysql.h>
#include <mysqld_error.h>
#include "utils.h"


void displayMainMenu() {
    printf("\nMain Menu:\n");
    printf("1. Operations on Products\n");
    printf("2. Operations on Customers\n");
    printf("3. Operations on Orders\n");
    printf("4. Operations on OrderItems\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

// Function for displaying the operation menu
void displayOperationMenu() {
    printf("\nOperation Menu:\n");
    printf("1. Add\n");
    printf("2. Remove\n");
    printf("3. Search\n");
    printf("4. Update\n");
    printf("0. Go Back\n");
    printf("Enter your choice: ");
}


// function to clear the input line
void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Clear input buffer
}


// Function to encrypt a single character with a shift
char encryptChar(char ch, int shift) {
    if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        return (ch - base + shift + 26) % 26 + base;
    }
    if (isdigit(ch)) {
        return (ch - '0' + shift + 10) % 10 + '0';
    }
    return ch;
}

// Function to encrypt the entire password string
void encrypt(const char* password, int shift, char* encrypted) {
    int len = strlen(password);
    int i;
    for ( i = 0; i < len; i++) {
        encrypted[i] = encryptChar(password[i], shift);
    }
    encrypted[len] = '\0';
}

// Function to decrypt a single character with a shift
char decryptChar(char ch, int shift) {
    if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        return (ch - base - shift + 26) % 26 + base;
    }
    if (isdigit(ch)) {
        return (ch - '0' - shift + 10) % 10 + '0';
    }
    return ch;
}

// Function to decrypt the entire encrypted password string
void decrypt(const char* encrypted, int shift, char* decrypted) {
    int len = strlen(encrypted);
    int i;
    for ( i = 0; i < len; i++) {
        decrypted[i] = decryptChar(encrypted[i], shift);
    }
    decrypted[len] = '\0';
}

// Function to retrieve the encrypted password from the database
void getDBPassword(MYSQL* conn, const char* id, char* encryptedPW) {
    char query[256];
    sprintf(query, "SELECT Password FROM password WHERE Id = '%s'", id);
    if (mysql_query(conn, query)) {
        printf("Error: %s\n", mysql_error(conn));
    } else {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                strcpy(encryptedPW, row[0]);
            }
        }
    }
}



void addProduct(MYSQL *conn) {
    char *query;
    char *productName = malloc(101 * sizeof(char));
    char *category = malloc(51 * sizeof(char));
    char *priceStr = malloc(21 * sizeof(char));
    char *stockStr = malloc(11 * sizeof(char));

    if (productName == NULL || category == NULL || priceStr == NULL || stockStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Take input from user
    printf("\nEnter Product Name: ");
    clear_stdin(); // Clear input buffer to avoid skipping input
    fgets(productName, 101, stdin);
    productName[strcspn(productName, "\n")] = 0; // Remove newline character

    printf("Enter Category: ");
    fgets(category, 51, stdin);
    category[strcspn(category, "\n")] = 0; // Remove newline character

    printf("Enter Price: ");
    fgets(priceStr, 21, stdin);
    priceStr[strcspn(priceStr, "\n")] = 0; // Remove newline character

    printf("Enter Stock: ");
    fgets(stockStr, 11, stdin);
    stockStr[strcspn(stockStr, "\n")] = 0; // Remove newline character

    // Allocate memory for the query
    query = malloc((256 + strlen(productName) + strlen(category) + strlen(priceStr) + strlen(stockStr)) * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        free(productName);
        free(category);
        free(priceStr);
        free(stockStr);
        return;
    }

    // Formulate the query
    sprintf(query, "INSERT INTO Products (ProductName, Category, Price, Stock) VALUES ('%s', '%s', %s, %s)", 
            productName, category, priceStr, stockStr);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Product inserted successfully!\n");
    }

    // Free allocated memory
    free(query);
    free(productName);
    free(category);
    free(priceStr);
    free(stockStr);
}


// Function to insert new customer into Customers table
void addCustomer(MYSQL *conn) {
    char *query;
    char *customerName = malloc(101 * sizeof(char));
    char *phoneNumber = malloc(16 * sizeof(char));
    char *email = malloc(101 * sizeof(char));
    char *address = malloc(256 * sizeof(char));

    if (customerName == NULL || phoneNumber == NULL || email == NULL || address == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Take input from user
    printf("\nEnter Customer Name: ");
    clear_stdin(); // Clear input buffer to avoid skipping input
    fgets(customerName, 101, stdin);
    customerName[strcspn(customerName, "\n")] = 0; // Remove newline character

    printf("Enter Phone Number: ");
    fgets(phoneNumber, 16, stdin);
    phoneNumber[strcspn(phoneNumber, "\n")] = 0; // Remove newline character

    printf("Enter Email: ");
    fgets(email, 101, stdin);
    email[strcspn(email, "\n")] = 0; // Remove newline character

    printf("Enter Address: ");
    fgets(address, 256, stdin);
    address[strcspn(address, "\n")] = 0; // Remove newline character

    // Allocate memory for the query
    query = malloc((256 + strlen(customerName) + strlen(phoneNumber) + strlen(email) + strlen(address)) * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        free(customerName);
        free(phoneNumber);
        free(email);
        free(address);
        return;
    }

    // Formulate the query
    sprintf(query, "INSERT INTO Customers (CustomerName, PhoneNumber, Email, Address) VALUES ('%s', '%s', '%s', '%s')", 
            customerName, phoneNumber, email, address);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Customer inserted successfully!\n");
    }

    // Free allocated memory
    free(query);
    free(customerName);
    free(phoneNumber);
    free(email);
    free(address);
}

// Functions are redirect
void createOrder(MYSQL *conn) {
    char *query;
    char *customerIDStr = malloc(11 * sizeof(char));
    char *totalAmountStr = malloc(21 * sizeof(char));

    if (customerIDStr == NULL || totalAmountStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Take input from user
    printf("\nEnter Customer ID: ");
    clear_stdin(); // Clear input buffer to avoid skipping input
    fgets(customerIDStr, 11, stdin);
    customerIDStr[strcspn(customerIDStr, "\n")] = 0; // Remove newline character

    printf("Enter Total Amount: ");
    fgets(totalAmountStr, 21, stdin);
    totalAmountStr[strcspn(totalAmountStr, "\n")] = 0; // Remove newline character

    // Allocate memory for the query
    query = malloc((128 + strlen(customerIDStr) + strlen(totalAmountStr)) * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        free(customerIDStr);
        free(totalAmountStr);
        return;
    }

    // Formulate the query
    sprintf(query, "INSERT INTO Orders (CustomerID, TotalAmount) VALUES (%s, %s)", 
            customerIDStr, totalAmountStr);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Order created successfully!\n");
    }

    // Free allocated memory
    free(query);
    free(customerIDStr);
    free(totalAmountStr);
}


void addOrderItem(MYSQL *conn) {
    char *query;
    char *orderIDStr = malloc(11 * sizeof(char));
    char *productIDStr = malloc(11 * sizeof(char));
    char *quantityStr = malloc(11 * sizeof(char));
    double price = 0.0;
    int quantity;
    double subtotal;

    if (orderIDStr == NULL || productIDStr == NULL || quantityStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Take input from user
    printf("\nEnter Order ID: ");
    clear_stdin(); // Clear input buffer to avoid skipping input
    fgets(orderIDStr, 11, stdin);
    orderIDStr[strcspn(orderIDStr, "\n")] = 0; // Remove newline character

    printf("Enter Product ID: ");
    fgets(productIDStr, 11, stdin);
    productIDStr[strcspn(productIDStr, "\n")] = 0; // Remove newline character

    printf("Enter Quantity: ");
    fgets(quantityStr, 11, stdin);
    quantityStr[strcspn(quantityStr, "\n")] = 0; // Remove newline character
    quantity = atoi(quantityStr);

    // Fetch product price from Products table
    query = malloc((64 + strlen(productIDStr)) * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        free(orderIDStr);
        free(productIDStr);
        free(quantityStr);
        return;
    }
    sprintf(query, "SELECT Price FROM Products WHERE ProductID = %s", productIDStr);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to fetch product price: %s\n", mysql_error(conn));
        free(query);
        free(orderIDStr);
        free(productIDStr);
        free(quantityStr);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Failed to store result: %s\n", mysql_error(conn));
        free(query);
        free(orderIDStr);
        free(productIDStr);
        free(quantityStr);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL) {
        price = atof(row[0]);
    } else {
        fprintf(stderr, "Product not found\n");
        mysql_free_result(result);
        free(query);
        free(orderIDStr);
        free(productIDStr);
        free(quantityStr);
        return;
    }
    mysql_free_result(result);
    free(query);

    // Calculate subtotal
    subtotal = price * quantity;

    // Allocate memory for the query to insert into OrderItems table
    query = malloc((128 + strlen(orderIDStr) + strlen(productIDStr) + strlen(quantityStr)) * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        free(orderIDStr);
        free(productIDStr);
        free(quantityStr);
        return;
    }

    // Formulate the query
    sprintf(query, "INSERT INTO OrderItems (OrderID, ProductID, Quantity, Subtotal) VALUES (%s, %s, %d, %.2f)",
            orderIDStr, productIDStr, quantity, subtotal);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to insert order item: %s\n", mysql_error(conn));
    } else {
        printf("Order item added successfully!\n");
    }

    // Free allocated memory
    free(query);
    free(orderIDStr);
    free(productIDStr);
    free(quantityStr);
}

// Function to mark order as complete by updating the total amount
void completeOrder(MYSQL *conn) {
    char *query;
    double totalAmount = 0.0;
	int order_id;
	printf("Enter the orderId:  ");
	scanf("%d",&order_id);
    // Allocate memory for the query to fetch subtotal from OrderItems table
    query = malloc(128 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        return;
    }

    // Formulate the query to calculate the total amount
    sprintf(query, "SELECT SUM(Subtotal) FROM OrderItems WHERE OrderID = %d", order_id);

    // Execute the query to get the total amount
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to fetch order total: %s\n", mysql_error(conn));
        free(query);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Failed to store result: %s\n", mysql_error(conn));
        free(query);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL && row[0] != NULL) {
        totalAmount = atof(row[0]);
    } else {
        fprintf(stderr, "No order items found for the given order ID\n");
        mysql_free_result(result);
        free(query);
        return;
    }
    mysql_free_result(result);
    free(query);

    // Allocate memory for the query to update the Orders table
    query = malloc(128 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        return;
    }

    // Formulate the query to update the total amount in the Orders table
    sprintf(query, "UPDATE Orders SET TotalAmount = %.2f WHERE OrderID = %d", totalAmount, order_id);

    // Execute the query to update the Orders table
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Failed to update order total: %s\n", mysql_error(conn));
    } else {
        printf("Order marked as complete successfully! Total amount: %.2f\n", totalAmount);
    }

    // Free allocated memory
    free(query);
}

// Function to delete a product by ProductID
// Function to delete a product by ProductID
void removeProduct(MYSQL *conn) {
    char *query;
    int product_id;
    bool a = true;

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        return;
    }

    char confirm;
    while (a) {
        printf("Enter the ProductID (e.g. 101): ");
        scanf("%d", &product_id);

        // Formulate the SELECT query
        sprintf(query, "SELECT * FROM Products WHERE ProductID = %d", product_id);

        // Execute the SELECT query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Check if any rows were returned
        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printf("Product found:\n");
            printf("ProductID: %s\n", row[0]);      // ProductID is the first column
            printf("ProductName: %s\n", row[1]);    // ProductName is the second column
            printf("Category: %s\n", row[2]);       // Category is the third column
            printf("Price: %s\n", row[3]);          // Price is the fourth column
            printf("Stock: %s\n", row[4]);          // Stock is the fifth column
            printf("Confirm deletion (Y/N): ");
        } else {
            printf("No product found with ProductID %d. Please try again.\n", product_id);
            mysql_free_result(result);
            continue; // Go back to the start of the loop
        }

        // Clear the input buffer before reading a character
        while ((getchar()) != '\n'); // Clear any remaining newline characters
        scanf("%c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') {
            sprintf(query, "DELETE FROM Products WHERE ProductID = %d", product_id);
            // Execute the DELETE query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            } else {
                printf("Product deleted successfully!\n");
                a = false; // Exit the loop
            }
        } else {
            printf("Retry!\n");
        }

        mysql_free_result(result); // Free the result set
    }

    // Free allocated memory
    free(query);
}

// Function to delete a customer by CustomerID
void removeCustomer(MYSQL *conn) { 
    char *query;
    int customer_id;
    bool a = true;

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        return;
    }

    char confirm;
    while (a) {
        printf("Enter the CustomerID (e.g. 1001): ");
        scanf("%d", &customer_id);

        // Formulate the SELECT query
        sprintf(query, "SELECT * FROM Customers WHERE CustomerID = %d", customer_id);

        // Execute the SELECT query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Check if any rows were returned
        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printf("Customer found:\n");
            printf("CustomerID: %s\n", row[0]);         // CustomerID is the first column
            printf("CustomerName: %s\n", row[1]);       // CustomerName is the second column
            printf("PhoneNumber: %s\n", row[2]);        // PhoneNumber is the third column
            printf("Email: %s\n", row[3]);              // Email is the fourth column
            printf("Address: %s\n", row[4]);            // Address is the fifth column
            printf("Confirm deletion (Y/N): ");
        } else {
            printf("No customer found with CustomerID %d. Please try again.\n", customer_id);
            mysql_free_result(result);
            continue; // Go back to the start of the loop
        }

        // Clear the input buffer before reading a character
        while ((getchar()) != '\n'); // Clear any remaining newline characters
        scanf("%c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') {
            sprintf(query, "DELETE FROM Customers WHERE CustomerID = %d", customer_id);
            // Execute the DELETE query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            } else {
                printf("Customer deleted successfully!\n");
                a = false; // Exit the loop
            }
        } else {
            printf("Retry!\n");
        }

        mysql_free_result(result); // Free the result set
    }

    // Free allocated memory
    free(query);
}


// Function to delete an order by OrderID
void deleteOrder(MYSQL *conn) {
    char *query;
    int order_id;
    bool a = true;

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        return;
    }

    char confirm;
    while (a) {
        printf("Enter the OrderID (e.g. 1001): ");
        scanf("%d", &order_id);

        // Formulate the SELECT query
        sprintf(query, "SELECT * FROM Orders WHERE OrderID = %d", order_id);

        // Execute the SELECT query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Check if any rows were returned
        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printf("Order found:\n");
            printf("OrderID: %s\n", row[0]);          // OrderID is the first column
            printf("CustomerID: %s\n", row[1]);       // CustomerID is the second column
            printf("OrderDate: %s\n", row[2]);        // OrderDate is the third column
            printf("TotalAmount: %s\n", row[3]);      // TotalAmount is the fourth column
            printf("Confirm deletion (Y/N): ");
        } else {
            printf("No order found with OrderID %d. Please try again.\n", order_id);
            mysql_free_result(result);
            continue; // Go back to the start of the loop
        }

        // Clear the input buffer before reading a character
        while ((getchar()) != '\n'); // Clear any remaining newline characters
        scanf("%c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') {
            sprintf(query, "DELETE FROM Orders WHERE OrderID = %d", order_id);
            // Execute the DELETE query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            } else {
                printf("Order deleted successfully!\n");
                a = false; // Exit the loop
            }
        } else {
            printf("Retry!\n");
        }

        mysql_free_result(result); // Free the result set
    }

    // Free allocated memory
    free(query);
}


// Function to delete an order item by OrderItemID
void deleteOrderItem(MYSQL *conn) {
    char *query;
    int order_item_id;
    bool a = true;

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation for query failed\n");
        return;
    }

    char confirm;
    while (a) {
        printf("Enter the OrderItemID (e.g. 1001): ");
        scanf("%d", &order_item_id);

        // Formulate the SELECT query
        sprintf(query, "SELECT * FROM OrderItems WHERE OrderItemID = %d", order_item_id);

        // Execute the SELECT query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Check if any rows were returned
        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printf("Order Item found:\n");
            printf("OrderItemID: %s\n", row[0]);       // OrderItemID is the first column
            printf("OrderID: %s\n", row[1]);           // OrderID is the second column
            printf("ProductID: %s\n", row[2]);          // ProductID is the third column
            printf("Quantity: %s\n", row[3]);           // Quantity is the fourth column
            printf("Subtotal: %s\n", row[4]);           // Subtotal is the fifth column
            printf("Confirm deletion (Y/N): ");
        } else {
            printf("No order item found with OrderItemID %d. Please try again.\n", order_item_id);
            mysql_free_result(result);
            continue; // Go back to the start of the loop
        }

        // Clear the input buffer before reading a character
        while ((getchar()) != '\n'); // Clear any remaining newline characters
        scanf("%c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') {
            sprintf(query, "DELETE FROM OrderItems WHERE OrderItemID = %d", order_item_id);
            // Execute the DELETE query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            } else {
                printf("Order item deleted successfully!\n");
                a = false; // Exit the loop
            }
        } else {
            printf("Retry!\n");
        }

        mysql_free_result(result); // Free the result set
    }

    // Free allocated memory
    free(query);
}

// Function to search for a product by ProductID or ProductName
void searchProduct(MYSQL *conn) {
    char *query;
    char choice;
    int productId;
    char *productName = malloc(101 * sizeof(char));

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL || productName == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    printf("Press 1 to display all products, 2 to search by ProductID or ProductName, 0 to exit: ");
    scanf(" %c", &choice);

    if (choice == '1') {
        // Select all products
        sprintf(query, "SELECT * FROM Products");

        // Execute the query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Display products in table format
        printf("+-----------+---------------------+----------+--------+-------+\n");
        printf("| ProductID | ProductName         | Category | Price  | Stock |\n");
        printf("+-----------+---------------------+----------+--------+-------+\n");

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != NULL) {
            printf("| %-9s | %-20s | %-8s | %-6s | %-5s |\n", row[0], row[1], row[2], row[3], row[4]);
        }

        printf("+-----------+---------------------+----------+--------+-------+\n");

        mysql_free_result(result);
    } else if (choice == '2') {
        // Search by ProductID or ProductName
        printf("Search by (I)D or (N)ame? Enter I or N: ");
        scanf(" %c", &choice);

        if (choice == 'I' || choice == 'i') {
            // Search by ProductID
            printf("Enter the ProductID: ");
            scanf("%d", &productId);

            // Formulate the query
            sprintf(query, "SELECT * FROM Products WHERE ProductID = %d", productId);

            // Execute the query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            MYSQL_RES *result = mysql_store_result(conn);
            if (result == NULL) {
                fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            // Check if any rows were returned
            if (mysql_num_rows(result) > 0) {
                MYSQL_ROW row = mysql_fetch_row(result);
                printf("Product found:\n");
                printf("ProductID: %s\n", row[0]);
                printf("ProductName: %s\n", row[1]);
                printf("Category: %s\n", row[2]);
                printf("Price: %s\n", row[3]);
                printf("Stock: %s\n", row[4]);
            } else {
                printf("No product found with ProductID %d.\n", productId);
            }

            mysql_free_result(result);
        } else if (choice == 'N' || choice == 'n') {
            // Search by ProductName
            printf("Enter the ProductName: ");
            getchar(); // To consume the newline character
            fgets(productName, 101, stdin);
            productName[strcspn(productName, "\n")] = 0; // Remove newline character

            // Formulate the query
            sprintf(query, "SELECT * FROM Products WHERE ProductName LIKE '%%%s%%'", productName);

            // Execute the query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
                free(query);
                free(productName);
                return;
            }
			MYSQL_ROW row;
            MYSQL_RES *result = mysql_store_result(conn);
            if (result == NULL) {
                fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
                free(query);
                free(productName);
                return;
            }

            // Check if any rows were returned
            if (mysql_num_rows(result) > 0) {
                printf("Products found:\n");
                while ((row = mysql_fetch_row(result)) != NULL) {
                    printf("ProductID: %s, ProductName: %s, Category: %s, Price: %s, Stock: %s\n", row[0], row[1], row[2], row[3], row[4]);
                }
            } else {
                printf("No product found with name '%s'.\n", productName);
            }

            mysql_free_result(result);
        } else {
            printf("Invalid choice! Please enter I or N.\n");
        }
    } else if (choice == '0') {
        printf("Exiting...\n");
    } else {
        printf("Invalid option! Please enter 1, 2, or 0.\n");
    }

    // Free allocated memory
    free(query);
    free(productName);
}


// Function to search for a customer by CustomerID or CustomerName
void searchCustomer(MYSQL *conn) {
    char *query;
    char choice;
    int customerId;
    char *customerName = malloc(101 * sizeof(char));

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL || customerName == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    printf("Press 1 to display all customers, 2 to search by CustomerID or CustomerName, 0 to exit: ");
    scanf(" %c", &choice);

    if (choice == '1') {
        // Select all customers
        sprintf(query, "SELECT * FROM Customers");

        // Execute the query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Display customers in table format
        printf("+-------------+----------------------+--------------------------------+------------------+\n");
        printf("| CustomerID  | CustomerName         | PhoneNumber                    | Email            |\n");
        printf("+-------------+----------------------+--------------------------------+------------------+\n");

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != NULL) {
            printf("| %-11s | %-20s | %-30s | %-20s |\n", row[0], row[1], row[2], row[3]);
        }

        printf("+-------------+----------------------+--------------------------------+------------------+\n");

        mysql_free_result(result);
    } else if (choice == '2') {
        // Search by CustomerID or CustomerName
        printf("Search by (I)D or (N)ame? Enter I or N: ");
        scanf(" %c", &choice);

        if (choice == 'I' || choice == 'i') {
            // Search by CustomerID
            printf("Enter the CustomerID: ");
            scanf("%d", &customerId);

            // Formulate the query
            sprintf(query, "SELECT * FROM Customers WHERE CustomerID = %d", customerId);

            // Execute the query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            MYSQL_RES *result = mysql_store_result(conn);
            if (result == NULL) {
                fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            // Check if any rows were returned
            if (mysql_num_rows(result) > 0) {
                MYSQL_ROW row = mysql_fetch_row(result);
                printf("Customer found:\n");
                printf("CustomerID: %s\n", row[0]);
                printf("CustomerName: %s\n", row[1]);
                printf("PhoneNumber: %s\n", row[2]);
                printf("Email: %s\n", row[3]);
                printf("Address: %s\n", row[4]);
            } else {
                printf("No customer found with CustomerID %d.\n", customerId);
            }

            mysql_free_result(result);
        } else if (choice == 'N' || choice == 'n') {
            // Search by CustomerName
            printf("Enter the CustomerName: ");
            getchar(); // To consume the newline character
            fgets(customerName, 101, stdin);
            customerName[strcspn(customerName, "\n")] = 0; // Remove newline character

            // Formulate the query
            sprintf(query, "SELECT * FROM Customers WHERE CustomerName LIKE '%%%s%%'", customerName);

            // Execute the query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
                free(query);
                free(customerName);
                return;
            }
            MYSQL_ROW row;
            MYSQL_RES *result = mysql_store_result(conn);
            if (result == NULL) {
                fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
                free(query);
                free(customerName);
                return;
            }

            // Check if any rows were returned
            if (mysql_num_rows(result) > 0) {
                printf("Customers found:\n");
                while ((row = mysql_fetch_row(result)) != NULL) {
                    printf("CustomerID: %s, CustomerName: %s, PhoneNumber: %s, Email: %s, Address: %s\n", row[0], row[1], row[2], row[3], row[4]);
                }
            } else {
                printf("No customer found with name '%s'.\n", customerName);
            }

            mysql_free_result(result);
        } else {
            printf("Invalid choice! Please enter I or N.\n");
        }
    } else if (choice == '0') {
        printf("Exiting...\n");
    } else {
        printf("Invalid option! Please enter 1, 2, or 0.\n");
    }

    // Free allocated memory
    free(query);
    free(customerName);
}


// Function to search for an order by OrderID or CustomerID
void searchOrder(MYSQL *conn) {
    char *query;
    char choice;
    int orderId;
    int customerId;

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    printf("Press 1 to display all orders, 2 to search by OrderID or CustomerID, 0 to exit: ");
    scanf(" %c", &choice);

    if (choice == '1') {
        // Select all orders
        sprintf(query, "SELECT * FROM Orders");

        // Execute the query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Display orders in table format
        printf("+----------+------------+---------------------+------------------+\n");
        printf("| OrderID  | CustomerID | OrderDate           | TotalAmount      |\n");
        printf("+----------+------------+---------------------+------------------+\n");

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != NULL) {
            printf("| %-8s | %-10s | %-20s | %-16s |\n", row[0], row[1], row[2], row[3]);
        }

        printf("+----------+------------+---------------------+------------------+\n");

        mysql_free_result(result);
    } else if (choice == '2') {
        // Search by OrderID or CustomerID
        printf("Search by (I)D or (C)ustomerID? Enter I or C: ");
        scanf(" %c", &choice);

        if (choice == 'I' || choice == 'i') {
            // Search by OrderID
            printf("Enter the OrderID: ");
            scanf("%d", &orderId);

            // Formulate the query
            sprintf(query, "SELECT * FROM Orders WHERE OrderID = %d", orderId);

            // Execute the query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            MYSQL_RES *result = mysql_store_result(conn);
            if (result == NULL) {
                fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            // Check if any rows were returned
            if (mysql_num_rows(result) > 0) {
                MYSQL_ROW row = mysql_fetch_row(result);
                printf("Order found:\n");
                printf("OrderID: %s\n", row[0]);
                printf("CustomerID: %s\n", row[1]);
                printf("OrderDate: %s\n", row[2]);
                printf("TotalAmount: %s\n", row[3]);
            } else {
                printf("No order found with OrderID %d.\n", orderId);
            }

            mysql_free_result(result);
        } else if (choice == 'C' || choice == 'c') {
            // Search by CustomerID
            printf("Enter the CustomerID: ");
            scanf("%d", &customerId);

            // Formulate the query
            sprintf(query, "SELECT * FROM Orders WHERE CustomerID = %d", customerId);

            // Execute the query
            if (mysql_query(conn, query)) {
                fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
                free(query);
                return;
            }
			MYSQL_ROW row;
            MYSQL_RES *result = mysql_store_result(conn);
            if (result == NULL) {
                fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
                free(query);
                return;
            }

            // Check if any rows were returned
            if (mysql_num_rows(result) > 0) {
                printf("Orders found:\n");
                while ((row = mysql_fetch_row(result)) != NULL) {
                    printf("OrderID: %s, CustomerID: %s, OrderDate: %s, TotalAmount: %s\n", row[0], row[1], row[2], row[3]);
                }
            } else {
                printf("No orders found for CustomerID %d.\n", customerId);
            }

            mysql_free_result(result);
        } else {
            printf("Invalid choice! Please enter I or C.\n");
        }
    } else if (choice == '0') {
        printf("Exiting...\n");
    } else {
        printf("Invalid option! Please enter 1, 2, or 0.\n");
    }

    // Free allocated memory
    free(query);
}


// Function to search for an order item by OrderItemID or OrderID
void searchOrderItem(MYSQL *conn) {
    char *query;
    char choice;
    int orderItemId;

    // Allocate memory for the query
    query = malloc(256 * sizeof(char));
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    printf("Press 1 to display all order items, 2 to search by OrderItemID, 0 to exit: ");
    scanf(" %c", &choice);

    if (choice == '1') {
        // Select all order items
        sprintf(query, "SELECT * FROM OrderItems");

        // Execute the query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Display order items in table format
        printf("+-------------+----------+-----------+----------+----------+\n");
        printf("| OrderItemID| OrderID  | ProductID | Quantity | Price    |\n");
        printf("+-------------+----------+-----------+----------+----------+\n");

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != NULL) {
            printf("| %-11s | %-8s | %-9s | %-8s | %-8s |\n", row[0], row[1], row[2], row[3], row[4]);
        }

        printf("+-------------+----------+-----------+----------+----------+\n");

        mysql_free_result(result);
    } else if (choice == '2') {
        // Search by OrderItemID
        printf("Enter the OrderItemID: ");
        scanf("%d", &orderItemId);

        // Formulate the query
        sprintf(query, "SELECT * FROM OrderItems WHERE OrderItemID = %d", orderItemId);

        // Execute the query
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            fprintf(stderr, "Failed to retrieve result: %s\n", mysql_error(conn));
            free(query);
            return;
        }

        // Check if any rows were returned
        if (mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            printf("Order item found:\n");
            printf("OrderItemID: %s\n", row[0]);
            printf("OrderID: %s\n", row[1]);
            printf("ProductID: %s\n", row[2]);
            printf("Quantity: %s\n", row[3]);
            printf("Price: %s\n", row[4]);
        } else {
            printf("No order item found with OrderItemID %d.\n", orderItemId);
        }

        mysql_free_result(result);
    } else if (choice == '0') {
        printf("Exiting...\n");
    } else {
        printf("Invalid option! Please enter 1, 2, or 0.\n");
    }

    // Free allocated memory
    free(query);
}


// Function to check if a product exists by ProductID
int productExists(MYSQL *conn, int product_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM Products WHERE ProductID = %d", product_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 0; // Assuming product does not exist if the query fails
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        int count = atoi(row[0]);
        mysql_free_result(res);
        return count > 0; // Return true if product exists
    }

    return 0; // Product does not exist
}


// Function to check if a customer exists by CustomerID
int customerExists(MYSQL *conn, int customer_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM Customers WHERE CustomerID = %d", customer_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 0; // Assuming customer does not exist if the query fails
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        int count = atoi(row[0]);
        mysql_free_result(res);
        return count > 0; // Return true if customer exists
    }

    return 0; // Customer does not exist
}


// Function to check if an order exists by OrderID
int orderExists(MYSQL *conn, int order_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM Orders WHERE OrderID = %d", order_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 0; // Assuming order does not exist if the query fails
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        int count = atoi(row[0]);
        mysql_free_result(res);
        return count > 0; // Return true if order exists
    }

    return 0; // Order does not exist
}


// Function to check if an order item exists by OrderItemID
int orderItemExists(MYSQL *conn, int order_item_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM OrderItems WHERE OrderItemID = %d", order_item_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 0; // Assuming order item does not exist if the query fails
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        int count = atoi(row[0]);
        mysql_free_result(res);
        return count > 0; // Return true if order item exists
    }

    return 0; // Order item does not exist
}


// Function to update product details by ProductID
void updateProduct(MYSQL *conn) {
    char *query = malloc(512 * sizeof(char));
    int product_id;
    char *new_name = (char *)malloc(100 * sizeof(char)); // Dynamic allocation for Product Name
    char *new_description = (char *)malloc(200 * sizeof(char)); // Dynamic allocation for Description
    float new_price;
    int new_stock;

    // Ask the user for Product ID
    printf("Enter the Product ID to update: ");
    scanf("%d", &product_id);

    // Check if the product exists
    if (!productExists(conn, product_id)) {
        printf("Product with ID %d does not exist.\n", product_id);
        free(new_name);
        free(new_description);
        free(query);
        return;
    }

    // Retrieve and display current product details
    snprintf(query, 512, "SELECT ProductName, Description, Price, Stock FROM Products WHERE ProductID = %d", product_id);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        free(new_name);
        free(new_description);
        free(query);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error fetching result: %s\n", mysql_error(conn));
        free(new_name);
        free(new_description);
        free(query);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        printf("\nCurrent Product Details:\n");
        printf("Name: %s\n", row[0]);
        printf("Description: %s\n", row[1]);
        printf("Price: %s\n", row[2]);
        printf("Stock: %s\n", row[3]);
    }
    mysql_free_result(result);

    // Ask for confirmation to proceed
    printf("Are you sure you want to update Product ID %d? (1 for Yes, 0 for No): ", product_id);
    int confirm;
    scanf("%d", &confirm);
    if (!confirm) {
        printf("Update cancelled.\n");
        free(new_name);
        free(new_description);
        free(query);
        return;
    }

    // Get new details
    printf("Enter new Product Name: ");
    getchar(); // To consume newline left by scanf
    fgets(new_name, 100, stdin);
    new_name[strcspn(new_name, "\n")] = 0;  // Remove newline

    printf("Enter new Description: ");
    fgets(new_description, 200, stdin);
    new_description[strcspn(new_description, "\n")] = 0;  // Remove newline

    printf("Enter new Price: ");
    scanf("%f", &new_price);

    printf("Enter new Stock: ");
    scanf("%d", &new_stock);

    // Prepare SQL query to update Product
    snprintf(query, 512, "UPDATE Products SET ProductName = '%s', Description = '%s', Price = %.2f, Stock = %d WHERE ProductID = %d",
             new_name, new_description, new_price, new_stock, product_id);

    // Execute the SQL query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Product updated successfully!\n");
    }

    // Free allocated memory
    free(new_name);
    free(new_description);
    free(query);
}


// Function to update customer details by CustomerID
void updateCustomer(MYSQL *conn) {
    char *query = malloc(512 * sizeof(char));
    int customer_id;
    char *new_name = (char *)malloc(100 * sizeof(char)); // Dynamic allocation for Name
    char *new_email = (char *)malloc(100 * sizeof(char)); // Dynamic allocation for Email
    char *new_phone = (char *)malloc(20 * sizeof(char)); // Dynamic allocation for Phone

    // Ask the user for Customer ID
    printf("Enter the Customer ID to update: ");
    scanf("%d", &customer_id);

    // Check if the customer exists
    if (!customerExists(conn, customer_id)) {
        printf("Customer with ID %d does not exist.\n", customer_id);
        free(new_name);
        free(new_email);
        free(new_phone);
        free(query);
        return;
    }

    // Retrieve and display current customer details
    snprintf(query, 512, "SELECT Name, Email, Phone FROM Customers WHERE CustomerID = %d", customer_id);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        free(new_name);
        free(new_email);
        free(new_phone);
        free(query);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error fetching result: %s\n", mysql_error(conn));
        free(new_name);
        free(new_email);
        free(new_phone);
        free(query);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        printf("\nCurrent Customer Details:\n");
        printf("Name: %s\n", row[0]);
        printf("Email: %s\n", row[1]);
        printf("Phone: %s\n", row[2]);
    }
    mysql_free_result(result);

    // Ask for confirmation to proceed
    printf("Are you sure you want to update Customer ID %d? (1 for Yes, 0 for No): ", customer_id);
    int confirm;
    scanf("%d", &confirm);
    if (!confirm) {
        printf("Update cancelled.\n");
        free(new_name);
        free(new_email);
        free(new_phone);
        free(query);
        return;
    }

    // Get new details
    printf("Enter new Name: ");
    getchar(); // To consume newline left by scanf
    fgets(new_name, 100, stdin);
    new_name[strcspn(new_name, "\n")] = 0;  // Remove newline

    printf("Enter new Email: ");
    fgets(new_email, 100, stdin);
    new_email[strcspn(new_email, "\n")] = 0;  // Remove newline

    printf("Enter new Phone: ");
    fgets(new_phone, 20, stdin);
    new_phone[strcspn(new_phone, "\n")] = 0;  // Remove newline

    // Prepare SQL query to update Customer
    snprintf(query, 512, "UPDATE Customers SET Name = '%s', Email = '%s', Phone = '%s' WHERE CustomerID = %d",
             new_name, new_email, new_phone, customer_id);

    // Execute the SQL query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Customer updated successfully!\n");
    }

    // Free allocated memory
    free(new_name);
    free(new_email);
    free(new_phone);
    free(query);
}


// Function to update order details by OrderID
void updateOrder(MYSQL *conn) {
    char *query = malloc(512 * sizeof(char));
    int order_id;
    char *new_status = (char *)malloc(50 * sizeof(char)); // Dynamic allocation for Status
    char *new_date = (char *)malloc(20 * sizeof(char)); // Dynamic allocation for Date

    // Ask the user for Order ID
    printf("Enter the Order ID to update: ");
    scanf("%d", &order_id);

    // Check if the order exists
    if (!orderExists(conn, order_id)) {
        printf("Order with ID %d does not exist.\n", order_id);
        free(new_status);
        free(new_date);
        free(query);
        return;
    }

    // Retrieve and display current order details
    snprintf(query, 512, "SELECT Status, OrderDate FROM Orders WHERE OrderID = %d", order_id);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        free(new_status);
        free(new_date);
        free(query);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error fetching result: %s\n", mysql_error(conn));
        free(new_status);
        free(new_date);
        free(query);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        printf("\nCurrent Order Details:\n");
        printf("Status: %s\n", row[0]);
        printf("Order Date: %s\n", row[1]);
    }
    mysql_free_result(result);

    // Ask for confirmation to proceed
    printf("Are you sure you want to update Order ID %d? (1 for Yes, 0 for No): ", order_id);
    int confirm;
    scanf("%d", &confirm);
    if (!confirm) {
        printf("Update cancelled.\n");
        free(new_status);
        free(new_date);
        free(query);
        return;
    }

    // Get new details
    printf("Enter new Status: ");
    getchar(); // To consume newline left by scanf
    fgets(new_status, 50, stdin);
    new_status[strcspn(new_status, "\n")] = 0;  // Remove newline

    printf("Enter new Order Date (YYYY-MM-DD): ");
    fgets(new_date, 20, stdin);
    new_date[strcspn(new_date, "\n")] = 0;  // Remove newline

    // Prepare SQL query to update Order
    snprintf(query, 512, "UPDATE Orders SET Status = '%s', OrderDate = '%s' WHERE OrderID = %d",
             new_status, new_date, order_id);

    // Execute the SQL query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Order updated successfully!\n");
    }

    // Free allocated memory
    free(new_status);
    free(new_date);
    free(query);
}


// Function to update order item details by OrderItemID
void updateOrderItem(MYSQL *conn) {
    char *query = malloc(512 * sizeof(char));
    int order_item_id;
    int new_product_id;
    int new_quantity;

    // Ask the user for Order Item ID
    printf("Enter the Order Item ID to update: ");
    scanf("%d", &order_item_id);

    // Check if the order item exists
    if (!orderItemExists(conn, order_item_id)) {
        printf("Order Item with ID %d does not exist.\n", order_item_id);
        free(query);
        return;
    }

    // Retrieve and display current order item details
    snprintf(query, 512, "SELECT ProductID, Quantity FROM OrderItems WHERE OrderItemID = %d", order_item_id);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        free(query);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error fetching result: %s\n", mysql_error(conn));
        free(query);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        printf("\nCurrent Order Item Details:\n");
        printf("Product ID: %s\n", row[0]);
        printf("Quantity: %s\n", row[1]);
    }
    mysql_free_result(result);

    // Ask for confirmation to proceed
    printf("Are you sure you want to update Order Item ID %d? (1 for Yes, 0 for No): ", order_item_id);
    int confirm;
    scanf("%d", &confirm);
    if (!confirm) {
        printf("Update cancelled.\n");
        free(query);
        return;
    }

    // Get new details
    printf("Enter new Product ID: ");
    scanf("%d", &new_product_id);
    printf("Enter new Quantity: ");
    scanf("%d", &new_quantity);

    // Prepare SQL query to update Order Item
    snprintf(query, 512, "UPDATE OrderItems SET ProductID = %d, Quantity = %d WHERE OrderItemID = %d",
             new_product_id, new_quantity, order_item_id);

    // Execute the SQL query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        printf("Order Item updated successfully!\n");
    }

    // Free allocated memory
    free(query);
}



