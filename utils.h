#ifndef UTILS_H   // Header guard start
#define UTILS_H // Define the header guard

#include <mysql.h>
#include <ctype.h>

// 
void clear_stdin();
void encrypt(const char* password, int shift, char* encrypted);
void decrypt(const char* encrypted, int shift, char* decrypted);
void getDBPassword(MYSQL* conn, const char* id, char* encryptedPW);


// Function to insert new product into Products table
void addProduct(MYSQL *conn);

// Function to insert new customer into Customers table
void addCustomer(MYSQL *conn);

// Function to create a new order for a customer in Orders table
void createOrder(MYSQL *conn);

// Function to add items to an order in OrderItems table
void addOrderItem(MYSQL *conn);

// Function to mark order as complete by updating the total amount
void completeOrder(MYSQL *conn, int order_id);

// Function to delete a product by ProductID
void removeProduct(MYSQL *conn, int product_id);

// Function to delete a customer by CustomerID
void removeCustomer(MYSQL *conn, int customer_id);

// Function to delete an order by OrderID
void deleteOrder(MYSQL *conn, int order_id);

// Function to delete an order item by OrderItemID
void deleteOrderItem(MYSQL *conn, int order_item_id);

// Function to search for a product by ProductID or ProductName
void searchProduct(MYSQL *conn);

// Function to search for a customer by CustomerID or CustomerName
void searchCustomer(MYSQL *conn);

// Function to search for an order by OrderID or CustomerID
void searchOrder(MYSQL *conn);

// Function to search for an order item by OrderItemID or OrderID
void searchOrderItem(MYSQL *conn);

// Function to check if a product exists by ProductID
int productExists(MYSQL *conn, int product_id);

// Function to check if a customer exists by CustomerID
int customerExists(MYSQL *conn, int customer_id);

// Function to check if an order exists by OrderID
int orderExists(MYSQL *conn, int order_id);

// Function to check if an order item exists by OrderItemID
int orderItemExists(MYSQL *conn, int order_item_id);

// Function to update product details by ProductID
void updateProduct(MYSQL *conn);

// Function to update customer details by CustomerID
void updateCustomer(MYSQL *conn);

// Function to update order details by OrderID
void updateOrder(MYSQL *conn);

// Function to update order item details by OrderItemID
void updateOrderItem(MYSQL *conn);




// Function to join the table 
void joinShow(MYSQL *conn);

#endif  // Header guard end

