-- Fresh Farm Plant Sales Management System
-- Run this script in MySQL Workbench. For a clean reset, drop the database first manually.
CREATE DATABASE IF NOT EXISTS plant_sales_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE plant_sales_db;

CREATE TABLE IF NOT EXISTS app_users (
  user_id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(60) NOT NULL UNIQUE,
  password_hash CHAR(64) NOT NULL,
  active TINYINT(1) NOT NULL DEFAULT 1,
  created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS plants (
  plant_id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  category VARCHAR(60) NOT NULL,
  price DECIMAL(10,2) NOT NULL,
  stock INT NOT NULL DEFAULT 0,
  CHECK (price >= 0), CHECK (stock >= 0)
);
CREATE TABLE IF NOT EXISTS customers (
  customer_id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(100) NOT NULL,
  phone VARCHAR(20),
  address VARCHAR(200)
);
CREATE TABLE IF NOT EXISTS sales (
  sale_id INT AUTO_INCREMENT PRIMARY KEY,
  customer_id INT NOT NULL,
  plant_id INT NOT NULL,
  quantity INT NOT NULL,
  total DECIMAL(10,2) NOT NULL,
  sale_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  CHECK (quantity > 0), CHECK (total >= 0),
  CONSTRAINT fk_sales_customer FOREIGN KEY (customer_id) REFERENCES customers(customer_id),
  CONSTRAINT fk_sales_plant FOREIGN KEY (plant_id) REFERENCES plants(plant_id)
);

-- Demo account: username admin, password admin123 (stored as SHA-256 hash).
INSERT INTO app_users(username,password_hash,active)
VALUES('admin',SHA2('admin123',256),1)
ON DUPLICATE KEY UPDATE password_hash=VALUES(password_hash), active=1;

INSERT INTO customers(username,phone,address)
SELECT 'Nimal Perera','0711234567','Avissawella' WHERE NOT EXISTS (SELECT 1 FROM customers WHERE username='Nimal Perera');
INSERT INTO customers(username,phone,address)
SELECT 'Kamal Silva','0772345678','Colombo' WHERE NOT EXISTS (SELECT 1 FROM customers WHERE username='Kamal Silva');
INSERT INTO plants(name,category,price,stock)
SELECT 'Rose','Flowers',500.00,50 WHERE NOT EXISTS (SELECT 1 FROM plants WHERE name='Rose' AND category='Flowers');
INSERT INTO plants(name,category,price,stock)
SELECT 'Jasmine','Flowers',350.00,40 WHERE NOT EXISTS (SELECT 1 FROM plants WHERE name='Jasmine' AND category='Flowers');
INSERT INTO plants(name,category,price,stock)
SELECT 'Money Plant','Indoor',450.00,40 WHERE NOT EXISTS (SELECT 1 FROM plants WHERE name='Money Plant' AND category='Indoor');
INSERT INTO plants(name,category,price,stock)
SELECT 'Mint','Herbal Plants',150.00,40 WHERE NOT EXISTS (SELECT 1 FROM plants WHERE name='Mint' AND category='Herbal Plants');
INSERT INTO plants(name,category,price,stock)
SELECT 'Watering Can','Garden Supplies and Accessories',850.00,15 WHERE NOT EXISTS (SELECT 1 FROM plants WHERE name='Watering Can' AND category='Garden Supplies and Accessories');
