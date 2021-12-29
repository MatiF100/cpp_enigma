//
// Created by Mateusz on 29.12.2021.
//

#ifndef CPPPROJECT_GUIPANEL_H
#define CPPPROJECT_GUIPANEL_H

#include <wx/wx.h>
#include <string>

class GuiPanel : public wxPanel{
public:
    GuiPanel(wxPanel *parent);

    void OnEncryptClicked(wxCommandEvent& event);

    wxButton* step_up[5]{};
    wxButton* step_down[5]{};

    wxStaticText* state[5]{};

    wxTextCtrl* edit_box{};
    wxButton* trigger;

    //!Method increasing left drum offset on click
    void OnLeftIncrease(wxCommandEvent &event);
    //!Method increasing middle drum offset on click
    void OnMiddleIncrease(wxCommandEvent &event);
    //!Method increasing right drum offset on click
    void OnRightIncrease(wxCommandEvent &event);

    //!Method increasing leftmost drum offset on click
    void OnLeftmostIncrease(wxCommandEvent &event);
    //!Method increasing reflector drum offset on click
    void OnReflectorIncrease(wxCommandEvent &event);

    //!Method decreasing left drum offset on click
    void OnLeftDecrease(wxCommandEvent &event);
    //!Method decreasing middle drum offset on click
    void OnMiddleDecrease(wxCommandEvent &event);
    //!Method decreasing right drum offset on click
    void OnRightDecrease(wxCommandEvent &event);

    //!Method decreasing leftmost drum offset on click
    void OnLeftmostDecrease(wxCommandEvent &event);
    //!Method decreasing reflector drum offset on click
    void OnReflectorDecrease(wxCommandEvent &event);
};


#endif //CPPPROJECT_GUIPANEL_H
