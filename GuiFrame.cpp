//
// Created by Mateusz on 29.12.2021.
//

#include "GuiFrame.h"

GuiFrame::GuiFrame()
:wxFrame(NULL, wxID_ANY, "Hello World")
{    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &GuiFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &GuiFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &GuiFrame::OnExit, this, wxID_EXIT);
}

void GuiFrame::OnHello(wxCommandEvent &event) {
    wxLogMessage("Test message");
}

void GuiFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void GuiFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox("Test msg", "Test caption", wxOK | wxICON_INFORMATION);
}
