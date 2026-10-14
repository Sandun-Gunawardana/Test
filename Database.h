#pragma once
using namespace System;
using namespace System::Data;
using namespace MySql::Data::MySqlClient;

namespace PlantSalesManagementSystem {
public ref class Database abstract sealed {
public:
    // Update the password to match your local MySQL installation.
    static property String^ ConnectionString {
        String^ get() { return "Server=localhost;Port=3306;Database=plant_sales_db;Uid=root;Pwd=YOUR_MYSQL_PASSWORD;SslMode=None;"; }
    }
    static MySqlConnection^ OpenConnection() {
        MySqlConnection^ connection = gcnew MySqlConnection(ConnectionString);
        connection->Open();
        return connection;
    }
    static DataTable^ Query(String^ sql) {
        DataTable^ table = gcnew DataTable();
        MySqlConnection^ connection = OpenConnection();
        try {
            MySqlCommand^ command = gcnew MySqlCommand(sql, connection);
            MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(command);
            adapter->Fill(table);
        } finally { delete connection; }
        return table;
    }
    static int Execute(String^ sql, ... array<Object^>^ values) {
        MySqlConnection^ connection = OpenConnection();
        try {
            MySqlCommand^ command = gcnew MySqlCommand(sql, connection);
            for (int i = 0; i < values->Length; ++i)
                command->Parameters->AddWithValue("@p" + i, values[i] == nullptr ? DBNull::Value : values[i]);
            return command->ExecuteNonQuery();
        } finally { delete connection; }
    }
};
}
