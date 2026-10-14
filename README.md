# Fresh Farm Plant Sales Management System (rebuilt starter)

This is a clean rebuilt C++/CLI Windows Forms version based on the assignment scenario. It is a separate version and does not overwrite your friend's original source.

## Features
- Login screen (demo account seeded by SQL: `admin` / `admin123`)
- Plant management: create, read, update, delete
- Customer management: create, read, update, delete
- Sales recording with stock validation and a database transaction
- Sales history
- Parameterized SQL commands for CRUD and transactional sales
- OOP examples: encapsulation, abstraction, inheritance, polymorphism, classes and objects

## Requirements
- Visual Studio with **Desktop development with C++** workload
- C++/CLI support and .NET Framework 4.7.2 targeting pack
- MySQL Server and MySQL Workbench
- MySQL Connector/NET NuGet package `MySql.Data` 8.4.0

## Setup
1. In MySQL Workbench, run `plant_sales_db.sql`.
2. In Visual Studio, open `Plant Sales Management System.vcxproj`.
3. Restore NuGet packages. If Visual Studio does not restore automatically, use **Tools > NuGet Package Manager > Package Manager Console** and run:
   `Update-Package -reinstall`
   or install `MySql.Data` version 8.4.0 through **Manage NuGet Packages**.
4. Open `Database.h` and replace `YOUR_MYSQL_PASSWORD` with your local MySQL root password. If your MySQL user is not `root`, change `Uid` too.
5. Build using **Build > Build Solution** and run with **F5**.
6. Sign in with `admin` / `admin123`.

## Notes
- The demo login password is only for assignment demonstration. Change it before using real customer data.
- The MySQL connector must be restored and referenced by Visual Studio; this ZIP cannot bundle a licensed/runtime dependency automatically.
- I cannot execute Visual Studio in this environment, so the final build must be tested in your Windows VM. If a compiler error appears, send the full Error List and we can correct it.

## OOP mapping
- **Class:** `PlantItem`, `Customer`, `MainForm`, `LoginForm`
- **Object:** objects created from forms and plant subtype classes
- **Encapsulation:** private model fields with public properties
- **Abstraction:** abstract `PlantItem` and abstract `GetCategory()` contract
- **Inheritance:** `FlowerPlant`, `HerbPlant`, `IndoorPlant`, `GardenSupply` inherit `PlantItem`
- **Polymorphism:** each subtype overrides `GetCategory()`
