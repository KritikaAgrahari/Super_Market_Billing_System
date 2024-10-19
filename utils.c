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


// Function to create a new order for a customer in Orders table
void createOrder(MYSQL *conn){
	
}

// Function to add items to an order in OrderItems table
void addOrderItem(MYSQL *conn){
	
}

// Function to mark order as complete by updating the total amount
void completeOrder(MYSQL *conn, int order_id){
}

// Function to delete a product by ProductID
void removeProduct(MYSQL *conn, int product_id){
}

// Function to delete a customer by CustomerID
void removeCustomer(MYSQL *conn, int customer_id){
}

// Function to delete an order by OrderID
void deleteOrder(MYSQL *conn, int order_id){
	
}

// Function to delete an order item by OrderItemID
void deleteOrderItem(MYSQL *conn, int order_item_id){
	
}

// Function to search for a product by ProductID or ProductName
void searchProduct(MYSQL *conn){
}

// Function to search for a customer by CustomerID or CustomerName
void searchCustomer(MYSQL *conn){
}

// Function to search for an order by OrderID or CustomerID
void searchOrder(MYSQL *conn){
}

// Function to search for an order item by OrderItemID or OrderID
void searchOrderItem(MYSQL *conn){
}

// Function to check if a product exists by ProductID
int productExists(MYSQL *conn, int product_id){
}

// Function to check if a customer exists by CustomerID
int customerExists(MYSQL *conn, int customer_id){
}

// Function to check if an order exists by OrderID
int orderExists(MYSQL *conn, int order_id){
	
}

// Function to check if an order item exists by OrderItemID
int orderItemExists(MYSQL *conn, int order_item_id){
	
}

// Function to update product details by ProductID
void updateProduct(MYSQL *conn){
}

// Function to update customer details by CustomerID
void updateCustomer(MYSQL *conn){
}

// Function to update order details by OrderID
void updateOrder(MYSQL *conn){
}

// Function to update order item details by OrderItemID
void updateOrderItem(MYSQL *conn){
}


// Function to join the table 
void joinShow(MYSQL *conn){
}
