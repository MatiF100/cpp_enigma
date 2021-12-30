//
// Created by Mateusz on 29.12.2021.
//

#include "GuiFrame.h"
#include "GuiPanel.h"

GuiFrame::GuiFrame(const wxString &title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 720))
{


    this->parent = new wxPanel(this, wxID_ANY);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    this->main_panel = new GuiPanel(this->parent);
    this->logic = new GuiLogic(this->parent);

    hbox->Add(this->main_panel, 1, wxEXPAND | wxALL, 5);
    hbox->Add(this->logic, 1, wxEXPAND | wxALL, 5);

    this->parent->SetSizer(hbox);
    this->Centre();


}


