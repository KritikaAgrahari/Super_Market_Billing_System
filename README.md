

# Supermarket Billing System

A Command-Line Interface (CLI) based Supermarket Billing System written in C with MySQL as the database. This project allows the management of products, customer details, and transactions, generating bills for customers and managing the product inventory of a supermarket.

## Features

- **Customer Management**: Add and manage customer information.
- **Product Inventory**: View available products with their prices and stock levels.
- **Cart System**: Add items to a customer's cart and update the cart with quantities.
- **Billing System**: Calculate and generate bills for customers.
- **Order Management**: Track orders with detailed information about the products, quantities, and total bill amount.
- **Stock Management**: Automatically update product stock after each purchase.

## Project Structure

- **Main Menu**
  - Add New Customer
  - View Available Products
  - Add Items to Cart
  - Generate Bill
  - Exit

- **Database Structure**
  - Products: Manage product details like name, category, price, and stock.
  - Customers: Store customer information such as name, contact number, and address.
  - Orders: Track customer orders with order ID, customer ID, and total amount.
  - OrderItems: Keep track of products purchased in each order and the quantity.

## Database Schema

```sql
CREATE TABLE Products (
    ProductID INT PRIMARY KEY AUTO_INCREMENT,
    ProductName VARCHAR(100) NOT NULL,
    Category VARCHAR(50),
    Price DECIMAL(10, 2) NOT NULL,
    Stock INT NOT NULL
);

CREATE TABLE Customers (
    CustomerID INT PRIMARY KEY AUTO_INCREMENT,
    CustomerName VARCHAR(100) NOT NULL,
    PhoneNumber VARCHAR(15),
    Email VARCHAR(100),
    Address TEXT
);

CREATE TABLE Orders (
    OrderID INT PRIMARY KEY AUTO_INCREMENT,
    CustomerID INT,
    OrderDate TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    TotalAmount DECIMAL(10, 2),
    FOREIGN KEY (CustomerID) REFERENCES Customers(CustomerID)
);

CREATE TABLE OrderItems (
    OrderItemID INT PRIMARY KEY AUTO_INCREMENT,
    OrderID INT,
    ProductID INT,
    Quantity INT,
    Subtotal DECIMAL(10, 2),
    FOREIGN KEY (OrderID) REFERENCES Orders(OrderID),
    FOREIGN KEY (ProductID) REFERENCES Products(ProductID)
);

Create TABLE PASSWORD(
Id VARCHAR(20),
Password VARCHAR(30));
```

## Prerequisites

- **C Compiler**: Ensure you have a working C compiler (e.g., GCC).
- **MySQL**: MySQL should be installed and running.
- **MySQL C Connector**: Install the MySQL development libraries to connect MySQL with your C program.

## How to Run

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/supermarket-billing-system.git
   cd supermarket-billing-system
   ```

2. **Set Up the Database**
   - Log into MySQL:
     ```bash
     mysql -u root -p
     ```
   - Create a new database:
     ```sql
     CREATE DATABASE SupermarketDB;
     USE SupermarketDB;
     ```
   - Create the tables using the schema provided above.

3. **Compile the Program**
   ```bash
   gcc -o billing_system main.c -lmysqlclient
   ```

4. **Run the Program**
   ```bash
   ./billing_system
   ```

## Usage

- **Add New Customer**: Enter customer details such as name, phone number, email, and address.
- **View Products**: Displays a list of available products in the supermarket.
- **Add Items to Cart**: Select items from the list of products, specify the quantity, and add them to the cart.
- **Generate Bill**: Automatically calculates the total amount, updates the stock, and prints the bill.

## Example

```bash
Welcome to Supermarket Billing System
------------------------------------
1. Add New Customer
2. View Products
3. Add Items to Cart
4. Generate Bill
5. Exit
------------------------------------
Enter your choice: 1
Enter customer name: John Doe
Enter phone number: 1234567890
Enter email: john@example.com
Enter address: 123, Main Street
------------------------------------
Customer added successfully!
```

## Future Enhancements

- Implement discount codes and promotional offers.
- Add an option to search for products by category or name.
- Generate and save invoices as text files or PDFs.
- Implement user authentication for staff and admin.

## Technologies Used

- **C**: For the CLI program.
- **MySQL**: For database management.
- **MySQL C API**: To connect C with MySQL.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Kritika Agrahari
```

This `README.md` file provides a comprehensive overview of the project, including features, database structure, usage, and how to set it up. You can modify it further to include specific instructions or details related to your project.
