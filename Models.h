#pragma once
using namespace System;

namespace PlantSalesManagementSystem {
// Encapsulation: state is private and exposed through properties.
public ref class PlantItem abstract {
private:
    int id;
    String^ name;
    Decimal price;
    int stock;
public:
    PlantItem(int plantId, String^ plantName, Decimal plantPrice, int plantStock)
        : id(plantId), name(plantName), price(plantPrice), stock(plantStock) {}
    property int Id { int get() { return id; } void set(int v) { id = v; } }
    property String^ Name { String^ get() { return name; } void set(String^ v) { name = v; } }
    property Decimal Price { Decimal get() { return price; } void set(Decimal v) { if (v < 0) throw gcnew ArgumentOutOfRangeException("Price"); price = v; } }
    property int Stock { int get() { return stock; } void set(int v) { if (v < 0) throw gcnew ArgumentOutOfRangeException("Stock"); stock = v; } }
    // Abstraction: every plant subtype must provide its category.
    virtual String^ GetCategory() = 0;
    virtual String^ GetSummary() { return String::Format("{0} | {1} | Rs. {2:F2} | stock {3}", Name, GetCategory(), Price, Stock); }
};
// Inheritance + polymorphism: subclasses override GetCategory().
public ref class FlowerPlant : PlantItem {
public: FlowerPlant(int id, String^ name, Decimal price, int stock) : PlantItem(id,name,price,stock) {}
    virtual String^ GetCategory() override { return "Flowers"; }
};
public ref class HerbPlant : PlantItem {
public: HerbPlant(int id, String^ name, Decimal price, int stock) : PlantItem(id,name,price,stock) {}
    virtual String^ GetCategory() override { return "Herbal Plants"; }
};
public ref class IndoorPlant : PlantItem {
public: IndoorPlant(int id, String^ name, Decimal price, int stock) : PlantItem(id,name,price,stock) {}
    virtual String^ GetCategory() override { return "Indoor"; }
};
public ref class GardenSupply : PlantItem {
public: GardenSupply(int id, String^ name, Decimal price, int stock) : PlantItem(id,name,price,stock) {}
    virtual String^ GetCategory() override { return "Garden Supplies and Accessories"; }
};
public ref class Customer {
private: int id; String^ name; String^ phone; String^ address;
public:
    Customer(int customerId, String^ customerName, String^ customerPhone, String^ customerAddress)
        : id(customerId), name(customerName), phone(customerPhone), address(customerAddress) {}
    property int Id { int get() { return id; } }
    property String^ Name { String^ get() { return name; } void set(String^ v) { name = v; } }
    property String^ Phone { String^ get() { return phone; } void set(String^ v) { phone = v; } }
    property String^ Address { String^ get() { return address; } void set(String^ v) { address = v; } }
};
}
