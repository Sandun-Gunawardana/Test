#pragma once
#include "Database.h"
#include "Models.h"
using namespace System;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace PlantSalesManagementSystem {
public ref class MainForm : public Form {
private:
    TabControl^ tabs;
    DataGridView^ plantsGrid; DataGridView^ customersGrid; DataGridView^ salesGrid;
    TextBox^ plantName; ComboBox^ plantCategory; TextBox^ plantPrice; TextBox^ plantStock;
    TextBox^ customerName; TextBox^ customerPhone; TextBox^ customerAddress;
    ComboBox^ saleCustomer; ComboBox^ salePlant; NumericUpDown^ saleQuantity;
    Label^ status;
public:
    MainForm() {
        Text="Fresh Farm | Plant Sales Management"; WindowState=FormWindowState::Maximized; MinimumSize=Drawing::Size(900,620); BackColor=Color::FromArgb(246,248,230);
        Label^ heading=gcnew Label(); heading->Text="FRESH FARM  |  Plant Sales Management"; heading->Font=gcnew Drawing::Font("Segoe UI",20,FontStyle::Bold); heading->ForeColor=Color::FromArgb(83,120,35); heading->Dock=DockStyle::Top; heading->Height=58; heading->TextAlign=ContentAlignment::MiddleLeft; Controls->Add(heading);
        tabs=gcnew TabControl(); tabs->Dock=DockStyle::Fill; Controls->Add(tabs);
        BuildPlantsTab(); BuildCustomersTab(); BuildSalesTab();
        status=gcnew Label(); status->Text="Ready"; status->Dock=DockStyle::Bottom; status->Height=26; status->TextAlign=ContentAlignment::MiddleLeft; Controls->Add(status);
        try { RefreshAll(); } catch(Exception^ ex) { MessageBox::Show("Database load failed:\n"+ex->Message,"MySQL",MessageBoxButtons::OK,MessageBoxIcon::Warning); }
    }
private:
    DataGridView^ NewGrid() { DataGridView^ g=gcnew DataGridView(); g->Dock=DockStyle::Fill; g->ReadOnly=true; g->AllowUserToAddRows=false; g->SelectionMode=DataGridViewSelectionMode::FullRowSelect; g->MultiSelect=false; g->AutoSizeColumnsMode=DataGridViewAutoSizeColumnsMode::Fill; return g; }
    Button^ AddButton(Panel^ panel,String^ text,int x,int y,EventHandler^ handler) { Button^ b=gcnew Button(); b->Text=text; b->Location=Point(x,y); b->Size=Drawing::Size(100,32); b->Click+=handler; panel->Controls->Add(b); return b; }
    Label^ AddLabel(Panel^ panel,String^ text,int x,int y) { Label^ l=gcnew Label(); l->Text=text; l->Location=Point(x,y+4); l->AutoSize=true; panel->Controls->Add(l); return l; }
    TextBox^ AddText(Panel^ panel,int x,int y,int width=145) { TextBox^ t=gcnew TextBox(); t->Location=Point(x,y); t->Width=width; panel->Controls->Add(t); return t; }
    void BuildPlantsTab() {
        TabPage^ page=gcnew TabPage("Plants"); tabs->TabPages->Add(page);
        Panel^ editor=gcnew Panel(); editor->Dock=DockStyle::Top; editor->Height=118; page->Controls->Add(editor);
        AddLabel(editor,"Plant name",12,10); plantName=AddText(editor,95,8,155);
        AddLabel(editor,"Category",265,10); plantCategory=gcnew ComboBox(); plantCategory->Location=Point(330,8); plantCategory->Width=205; plantCategory->DropDownStyle=ComboBoxStyle::DropDownList; plantCategory->Items->AddRange(gcnew array<Object^>{"Flowers","Herbal Plants","Indoor","Garden Supplies and Accessories"}); plantCategory->SelectedIndex=0; editor->Controls->Add(plantCategory);
        AddLabel(editor,"Price (Rs.)",550,10); plantPrice=AddText(editor,625,8,95);
        AddLabel(editor,"Stock",735,10); plantStock=AddText(editor,785,8,75);
        AddButton(editor,"Add",875,6,gcnew EventHandler(this,&MainForm::AddPlant)); AddButton(editor,"Update",980,6,gcnew EventHandler(this,&MainForm::UpdatePlant)); AddButton(editor,"Delete",1085,6,gcnew EventHandler(this,&MainForm::DeletePlant)); AddButton(editor,"Clear",1190,6,gcnew EventHandler(this,&MainForm::ClearPlant)); AddButton(editor,"Refresh",1295,6,gcnew EventHandler(this,&MainForm::RefreshPlants));
        plantsGrid=NewGrid(); plantsGrid->Dock=DockStyle::Fill; plantsGrid->SelectionChanged+=gcnew EventHandler(this,&MainForm::PlantSelected); page->Controls->Add(plantsGrid); plantsGrid->BringToFront();
    }
    void BuildCustomersTab() {
        TabPage^ page=gcnew TabPage("Customers"); tabs->TabPages->Add(page);
        Panel^ editor=gcnew Panel(); editor->Dock=DockStyle::Top; editor->Height=115; page->Controls->Add(editor);
        AddLabel(editor,"Customer name",12,10); customerName=AddText(editor,115,8,180); AddLabel(editor,"Phone",315,10); customerPhone=AddText(editor,370,8,140); AddLabel(editor,"Address",530,10); customerAddress=AddText(editor,600,8,210);
        AddButton(editor,"Add",825,6,gcnew EventHandler(this,&MainForm::AddCustomer)); AddButton(editor,"Update",930,6,gcnew EventHandler(this,&MainForm::UpdateCustomer)); AddButton(editor,"Delete",1035,6,gcnew EventHandler(this,&MainForm::DeleteCustomer)); AddButton(editor,"Clear",1140,6,gcnew EventHandler(this,&MainForm::ClearCustomer)); AddButton(editor,"Refresh",1245,6,gcnew EventHandler(this,&MainForm::RefreshCustomers));
        customersGrid=NewGrid(); page->Controls->Add(customersGrid); customersGrid->SelectionChanged+=gcnew EventHandler(this,&MainForm::CustomerSelected);
    }
    void BuildSalesTab() {
        TabPage^ page=gcnew TabPage("Sales"); tabs->TabPages->Add(page);
        Panel^ editor=gcnew Panel(); editor->Dock=DockStyle::Top; editor->Height=100; page->Controls->Add(editor);
        AddLabel(editor,"Customer",12,10); saleCustomer=gcnew ComboBox(); saleCustomer->Location=Point(85,8); saleCustomer->Width=220; saleCustomer->DropDownStyle=ComboBoxStyle::DropDownList; editor->Controls->Add(saleCustomer);
        AddLabel(editor,"Plant",325,10); salePlant=gcnew ComboBox(); salePlant->Location=Point(375,8); salePlant->Width=250; salePlant->DropDownStyle=ComboBoxStyle::DropDownList; editor->Controls->Add(salePlant);
        AddLabel(editor,"Quantity",645,10); saleQuantity=gcnew NumericUpDown(); saleQuantity->Location=Point(710,8); saleQuantity->Minimum=1; saleQuantity->Maximum=10000; saleQuantity->Value=1; editor->Controls->Add(saleQuantity);
        AddButton(editor,"Record Sale",835,6,gcnew EventHandler(this,&MainForm::RecordSale)); AddButton(editor,"Refresh",940,6,gcnew EventHandler(this,&MainForm::RefreshSales));
        salesGrid=NewGrid(); page->Controls->Add(salesGrid);
    }
    int SelectedId(DataGridView^ grid,String^ column) { if(grid->CurrentRow==nullptr || grid->CurrentRow->Cells[column]->Value==nullptr) return 0; return Convert::ToInt32(grid->CurrentRow->Cells[column]->Value); }
    void RefreshAll() { RefreshPlants(nullptr,nullptr); RefreshCustomers(nullptr,nullptr); RefreshSales(nullptr,nullptr); LoadSaleChoices(); }
    void RefreshPlants(Object^,EventArgs^) { plantsGrid->DataSource=Database::Query("SELECT plant_id,name,category,price,stock FROM plants ORDER BY plant_id"); status->Text="Plant records loaded"; }
    void RefreshCustomers(Object^,EventArgs^) { customersGrid->DataSource=Database::Query("SELECT customer_id,username AS name,phone,address FROM customers ORDER BY customer_id"); LoadSaleChoices(); status->Text="Customer records loaded"; }
    void RefreshSales(Object^,EventArgs^) { salesGrid->DataSource=Database::Query("SELECT s.sale_id,c.username AS customer,p.name AS plant,s.quantity,s.total,s.sale_date FROM sales s JOIN customers c ON c.customer_id=s.customer_id JOIN plants p ON p.plant_id=s.plant_id ORDER BY s.sale_id DESC"); status->Text="Sales loaded"; }
    void LoadSaleChoices() {
        try {
            DataTable^ c=Database::Query("SELECT customer_id,username FROM customers ORDER BY username"); saleCustomer->DataSource=c; saleCustomer->DisplayMember="username"; saleCustomer->ValueMember="customer_id";
            DataTable^ p=Database::Query("SELECT plant_id,name FROM plants WHERE stock>0 ORDER BY name"); salePlant->DataSource=p; salePlant->DisplayMember="name"; salePlant->ValueMember="plant_id";
        } catch(Exception^) {}
    }
    void PlantSelected(Object^,EventArgs^) { if(plantsGrid->CurrentRow==nullptr || plantsGrid->CurrentRow->Cells["plant_id"]->Value==nullptr) return; DataGridViewRow^ r=plantsGrid->CurrentRow; plantName->Text=Convert::ToString(r->Cells["name"]->Value); plantCategory->Text=Convert::ToString(r->Cells["category"]->Value); plantPrice->Text=Convert::ToString(r->Cells["price"]->Value); plantStock->Text=Convert::ToString(r->Cells["stock"]->Value); }
    void CustomerSelected(Object^,EventArgs^) { if(customersGrid->CurrentRow==nullptr || customersGrid->CurrentRow->Cells["customer_id"]->Value==nullptr) return; DataGridViewRow^ r=customersGrid->CurrentRow; customerName->Text=Convert::ToString(r->Cells["name"]->Value); customerPhone->Text=Convert::ToString(r->Cells["phone"]->Value); customerAddress->Text=Convert::ToString(r->Cells["address"]->Value); }
    void ValidatePlant() { Decimal p; int s; if(String::IsNullOrWhiteSpace(plantName->Text)) throw gcnew Exception("Plant name is required."); if(!Decimal::TryParse(plantPrice->Text,p)||p<0) throw gcnew Exception("Enter a valid non-negative price."); if(!Int32::TryParse(plantStock->Text,s)||s<0) throw gcnew Exception("Enter a valid non-negative stock quantity."); }
    void AddPlant(Object^,EventArgs^) { try { ValidatePlant(); Database::Execute("INSERT INTO plants(name,category,price,stock) VALUES(@p0,@p1,@p2,@p3)",plantName->Text->Trim(),plantCategory->Text,Decimal::Parse(plantPrice->Text),Int32::Parse(plantStock->Text)); RefreshPlants(nullptr,nullptr); LoadSaleChoices(); ClearPlant(nullptr,nullptr); MessageBox::Show("Plant added."); } catch(Exception^ ex){ ShowError(ex); } }
    void UpdatePlant(Object^,EventArgs^) { try { int id=SelectedId(plantsGrid,"plant_id"); if(id==0) throw gcnew Exception("Select a plant first."); ValidatePlant(); Database::Execute("UPDATE plants SET name=@p0,category=@p1,price=@p2,stock=@p3 WHERE plant_id=@p4",plantName->Text->Trim(),plantCategory->Text,Decimal::Parse(plantPrice->Text),Int32::Parse(plantStock->Text),id); RefreshPlants(nullptr,nullptr); LoadSaleChoices(); MessageBox::Show("Plant updated."); } catch(Exception^ ex){ ShowError(ex); } }
    void DeletePlant(Object^,EventArgs^) { try { int id=SelectedId(plantsGrid,"plant_id"); if(id==0) throw gcnew Exception("Select a plant first."); if(MessageBox::Show("Delete selected plant?","Confirm",MessageBoxButtons::YesNo,MessageBoxIcon::Question)!=DialogResult::Yes) return; Database::Execute("DELETE FROM plants WHERE plant_id=@p0",id); RefreshPlants(nullptr,nullptr); LoadSaleChoices(); } catch(Exception^ ex){ ShowError(ex); } }
    void ClearPlant(Object^,EventArgs^) { plantName->Clear(); plantPrice->Clear(); plantStock->Clear(); if(plantCategory->Items->Count>0) plantCategory->SelectedIndex=0; plantsGrid->ClearSelection(); }
    void AddCustomer(Object^,EventArgs^) { try { if(String::IsNullOrWhiteSpace(customerName->Text)) throw gcnew Exception("Customer name is required."); Database::Execute("INSERT INTO customers(username,phone,address) VALUES(@p0,@p1,@p2)",customerName->Text->Trim(),customerPhone->Text->Trim(),customerAddress->Text->Trim()); RefreshCustomers(nullptr,nullptr); ClearCustomer(nullptr,nullptr); MessageBox::Show("Customer added."); } catch(Exception^ ex){ ShowError(ex); } }
    void UpdateCustomer(Object^,EventArgs^) { try { int id=SelectedId(customersGrid,"customer_id"); if(id==0) throw gcnew Exception("Select a customer first."); if(String::IsNullOrWhiteSpace(customerName->Text)) throw gcnew Exception("Customer name is required."); Database::Execute("UPDATE customers SET username=@p0,phone=@p1,address=@p2 WHERE customer_id=@p3",customerName->Text->Trim(),customerPhone->Text->Trim(),customerAddress->Text->Trim(),id); RefreshCustomers(nullptr,nullptr); MessageBox::Show("Customer updated."); } catch(Exception^ ex){ ShowError(ex); } }
    void DeleteCustomer(Object^,EventArgs^) { try { int id=SelectedId(customersGrid,"customer_id"); if(id==0) throw gcnew Exception("Select a customer first."); if(MessageBox::Show("Delete selected customer? Existing sales may prevent deletion.","Confirm",MessageBoxButtons::YesNo,MessageBoxIcon::Question)!=DialogResult::Yes) return; Database::Execute("DELETE FROM customers WHERE customer_id=@p0",id); RefreshCustomers(nullptr,nullptr); } catch(Exception^ ex){ ShowError(ex); } }
    void ClearCustomer(Object^,EventArgs^) { customerName->Clear(); customerPhone->Clear(); customerAddress->Clear(); customersGrid->ClearSelection(); }
    void RecordSale(Object^,EventArgs^) { try { if(saleCustomer->SelectedValue==nullptr || salePlant->SelectedValue==nullptr) throw gcnew Exception("Add at least one customer and one in-stock plant first."); int customerId=Convert::ToInt32(saleCustomer->SelectedValue); int plantId=Convert::ToInt32(salePlant->SelectedValue); int quantity=Decimal::ToInt32(saleQuantity->Value);
        MySqlConnection^ conn=Database::OpenConnection(); try { MySqlTransaction^ tx=conn->BeginTransaction(); try { MySqlCommand^ select=gcnew MySqlCommand("SELECT price,stock FROM plants WHERE plant_id=@id FOR UPDATE",conn,tx); select->Parameters->AddWithValue("@id",plantId); MySqlDataReader^ reader=select->ExecuteReader(); if(!reader->Read()) throw gcnew Exception("Plant no longer exists."); Decimal price=reader->GetDecimal(0); int stock=reader->GetInt32(1); reader->Close(); if(stock<quantity) throw gcnew Exception("Not enough stock available."); MySqlCommand^ insert=gcnew MySqlCommand("INSERT INTO sales(customer_id,plant_id,quantity,total) VALUES(@c,@p,@q,@t)",conn,tx); insert->Parameters->AddWithValue("@c",customerId); insert->Parameters->AddWithValue("@p",plantId); insert->Parameters->AddWithValue("@q",quantity); insert->Parameters->AddWithValue("@t",price*quantity); insert->ExecuteNonQuery(); MySqlCommand^ update=gcnew MySqlCommand("UPDATE plants SET stock=stock-@q WHERE plant_id=@p",conn,tx); update->Parameters->AddWithValue("@q",quantity); update->Parameters->AddWithValue("@p",plantId); update->ExecuteNonQuery(); tx->Commit(); } catch(Exception^) { tx->Rollback(); throw; } } finally { delete conn; } RefreshAll(); MessageBox::Show("Sale recorded successfully."); } catch(Exception^ ex){ ShowError(ex); } }
    void ShowError(Exception^ ex) { MessageBox::Show(ex->Message,"Operation failed",MessageBoxButtons::OK,MessageBoxIcon::Warning); }
};
}
