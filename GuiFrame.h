//
// Created by Mateusz on 29.12.2021.
//

#ifndef CPPPROJECT_GUIFRAME_H
#define CPPPROJECT_GUIFRAME_H
#include <wx/wx.h>



class GuiFrame : public wxFrame{
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    enum
    {
        ID_Hello = 1
    };
public:
    GuiFrame();
};


#endif //CPPPROJECT_GUIFRAME_H
