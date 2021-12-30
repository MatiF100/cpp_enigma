//
// Created by Mateusz on 29.12.2021.
//

#ifndef CPPPROJECT_GUIFRAME_H
#define CPPPROJECT_GUIFRAME_H

#include <wx/wx.h>
#include "GuiPanel.h"
#include "GuiLogic.h"

class GuiFrame : public wxFrame{
public:
    explicit GuiFrame(const wxString& title);

    wxPanel* parent;
    GuiLogic* logic;
    GuiPanel* main_panel;



};


#endif //CPPPROJECT_GUIFRAME_H
