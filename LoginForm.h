#pragma once
#include "Database.h"
#include "MainForm.h"
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace PlantSalesManagementSystem {
public ref class LoginForm : public Form {
private: TextBox^ username; TextBox^ password; Label^ message;
public:
    LoginForm() {
        Text = "Fresh Farm | Sign in"; StartPosition = FormStartPosition::CenterScreen;
        ClientSize = Drawing::Size(520, 330); BackColor = Color::FromArgb(245,247,222);
        Label^ title = gcnew Label(); title->Text = "Fresh Farm"; title->Font = gcnew Drawing::Font("Segoe UI", 24, FontStyle::Bold); title->ForeColor = Color::FromArgb(91,125,35); title->AutoSize = true; title->Location = Point(160,25); Controls->Add(title);
        Label^ userLabel = gcnew Label(); userLabel->Text = "Username"; userLabel->Location = Point(70,105); userLabel->AutoSize=true; Controls->Add(userLabel);
        username=gcnew TextBox(); username->Location=Point(190,100); username->Width=240; username->Text="admin"; Controls->Add(username);
        Label^ passLabel=gcnew Label(); passLabel->Text="Password"; passLabel->Location=Point(70,150); passLabel->AutoSize=true; Controls->Add(passLabel);
        password=gcnew TextBox(); password->Location=Point(190,145); password->Width=240; password->UseSystemPasswordChar=true; Controls->Add(password);
        Button^ login=gcnew Button(); login->Text="Sign in"; login->Location=Point(190,195); login->Width=110; login->BackColor=Color::FromArgb(45,170,105); login->ForeColor=Color::White; login->Click += gcnew EventHandler(this,&LoginForm::LoginClicked); Controls->Add(login);
        Button^ cancel=gcnew Button(); cancel->Text="Exit"; cancel->Location=Point(320,195); cancel->Width=110; cancel->Click += gcnew EventHandler(this,&LoginForm::CancelClicked); Controls->Add(cancel);
        message=gcnew Label(); message->ForeColor=Color::Firebrick; message->Location=Point(70,245); message->Size=Drawing::Size(390,40); Controls->Add(message);
        AcceptButton=login; CancelButton=cancel;
    }
private:
    void LoginClicked(Object^, EventArgs^) {
        if (String::IsNullOrWhiteSpace(username->Text) || String::IsNullOrWhiteSpace(password->Text)) { message->Text="Enter both username and password."; return; }
        try {
            DataTable^ result=Database::Query("SELECT user_id FROM app_users WHERE username='" + username->Text->Replace("'","''") + "' AND password_hash=SHA2('" + password->Text->Replace("'","''") + "',256) AND active=1 LIMIT 1");
            if (result->Rows->Count == 1) { MainForm^ main=gcnew MainForm(); main->Show(); Hide(); }
            else message->Text="Invalid username or password.";
        } catch (Exception^ ex) { MessageBox::Show("Could not connect to MySQL. Check Database.h and run the SQL script.\n\n"+ex->Message,"Database connection",MessageBoxButtons::OK,MessageBoxIcon::Error); }
    }
    void CancelClicked(Object^, EventArgs^) { Close(); }
};
}
