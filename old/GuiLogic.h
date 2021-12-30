//
// Created by Mateusz on 29.12.2021.
//

#ifndef CPPPROJECT_GUILOGIC_H
#define CPPPROJECT_GUILOGIC_H

#include <wx/wx.h>
#include "GuiPanel.h"
#include "Machine.h"


class GuiLogic : public wxPanel {
public:
    GuiLogic(wxPanel* parent);

    Machine enigma;
    uint8_t offsets[5] = {0,0,0,0,0};

    wxStaticText* output;

};


#endif //CPPPROJECT_GUILOGIC_H
