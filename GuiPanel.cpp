//
// Created by Mateusz on 29.12.2021.
//

#include "GuiPanel.h"
#include "GuiFrame.h"


GuiPanel::GuiPanel(wxPanel *parent)
:wxPanel(parent, -1, wxPoint(-1,-1), wxSize(-1,-1), wxBORDER_SUNKEN)
{

    this->step_up[4] = new wxButton(this, wxID_ANY, wxT("+"), wxPoint(10,10));
    Connect(this->step_up[4]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnReflectorIncrease));
    this->step_up[3] = new wxButton(this, wxID_ANY, wxT("+"), wxPoint(100,10));
    Connect(this->step_up[3]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnLeftmostIncrease));
    this->step_up[2] = new wxButton(this, wxID_ANY, wxT("+"), wxPoint(200,10));
    Connect(this->step_up[2]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnLeftIncrease));
    this->step_up[1] = new wxButton(this, wxID_ANY, wxT("+"), wxPoint(300,10));
    Connect(this->step_up[1]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnMiddleIncrease));
    this->step_up[0] = new wxButton(this, wxID_ANY, wxT("+"), wxPoint(400,10));
    Connect(this->step_up[0]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnRightIncrease));

    state[4] = new wxStaticText(this, -1, wxT("0"), wxPoint(50, 60));
    state[3] = new wxStaticText(this, -1, wxT("0"), wxPoint(145, 60));
    state[2] = new wxStaticText(this, -1, wxT("0"), wxPoint(245, 60));
    state[1] = new wxStaticText(this, -1, wxT("0"), wxPoint(345, 60));
    state[0] = new wxStaticText(this, -1, wxT("0"), wxPoint(445, 60));


    this->step_down[4] = new wxButton(this, wxID_ANY, wxT("-"), wxPoint(10,100));
    Connect(this->step_down[4]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnReflectorDecrease));
    this->step_down[3] = new wxButton(this, wxID_ANY, wxT("-"), wxPoint(100,100));
    Connect(this->step_down[3]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnLeftmostDecrease));
    this->step_down[2] = new wxButton(this, wxID_ANY, wxT("-"), wxPoint(200,100));
    Connect(this->step_down[2]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnLeftDecrease));
    this->step_down[1] = new wxButton(this, wxID_ANY, wxT("-"), wxPoint(300,100));
    Connect(this->step_down[1]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnMiddleDecrease));
    this->step_down[0] = new wxButton(this, wxID_ANY, wxT("-"), wxPoint(400,100));
    Connect(this->step_down[0]->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnRightDecrease));

    this->edit_box = new wxTextCtrl(this, wxID_ANY, "Enter your message here!", wxPoint(40, 140), wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);
    this->trigger = new wxButton(this, wxID_ANY, wxT("Process!"), wxPoint(250,200));
    Connect(this->trigger->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GuiPanel::OnEncryptClicked));
}

void GuiPanel::OnEncryptClicked(wxCommandEvent &event) {
    std::string msg;
    msg = this->edit_box->GetValue();
    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto resp = frame->logic->enigma.process_message(msg);
    frame->logic->output->SetLabel(wxString(resp.c_str()));
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->main_panel->state[0]->SetLabel(wxString::Format(wxT("%d"), r+1));
    frame->main_panel->state[1]->SetLabel(wxString::Format(wxT("%d"), m+1));
    frame->main_panel->state[2]->SetLabel(wxString::Format(wxT("%d"), l+1));
    frame->main_panel->state[3]->SetLabel(wxString::Format(wxT("%d"), lm+1));
    frame->main_panel->state[4]->SetLabel(wxString::Format(wxT("%d"), ref+1));
}

void GuiPanel::OnLeftIncrease(wxCommandEvent &event) {
    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[2] = ++l % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, l, lm);

    frame->main_panel->state[2]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[2]+1));
}

void GuiPanel::OnMiddleIncrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[1] = ++m % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, l, lm);

    frame->main_panel->state[1]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[1]+1));
}

void GuiPanel::OnRightIncrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[0] = ++r % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, l, lm);

    frame->main_panel->state[0]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[0]+1));
}

void GuiPanel::OnLeftmostIncrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[3] = ++lm % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, l, lm);

    frame->main_panel->state[3]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[3]+1));
}

void GuiPanel::OnReflectorIncrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[4] = ++ref % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, l, lm);

    frame->main_panel->state[4]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[4]+1));
}

void GuiPanel::OnLeftDecrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[2] = (l + 25) % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, (l+25)%26, lm);

    frame->main_panel->state[2]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[2]+1));
}

void GuiPanel::OnMiddleDecrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[1] = (m + 25) % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, (m+25)%26, l, lm);

    frame->main_panel->state[1]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[1]+1));
}

void GuiPanel::OnRightDecrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[0] = (r + 25) % 26;
    frame->logic->enigma.set_drum_offsets(ref, (r+25)%26, m, l, lm);

    frame->main_panel->state[0]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[0]+1));
}

void GuiPanel::OnLeftmostDecrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[3] = (lm+25) % 26;
    frame->logic->enigma.set_drum_offsets(ref, r, m, l, (lm+25)%26);

    frame->main_panel->state[3]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[3]+1));
}

void GuiPanel::OnReflectorDecrease(wxCommandEvent &event) {

    GuiFrame *frame = (GuiFrame* ) this->m_parent->GetParent();
    auto [r,m,l,lm,ref] = frame->logic->enigma.get_offsets();
    frame->logic->offsets[4] = (ref+25)  % 26;
    frame->logic->enigma.set_drum_offsets((ref+25) % 26, r, m, l, lm);

    frame->main_panel->state[4]->SetLabel(wxString::Format(wxT("%d"), frame->logic->offsets[4]+1));
}
